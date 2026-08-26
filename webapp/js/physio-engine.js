/**
 * ╔════════════════════════════════════════════════════════════════════════════╗
 * ║        PhysioEngine.js – Software Vector Math & Biomechanical Engine        ║
 * ║        Ported from PhysioTracker V6.1 Core (Zero-Point Fix Restored)       ║
 * ╚════════════════════════════════════════════════════════════════════════════╝
 */

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define([], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory();
  } else {
    root.PhysioEngine = factory();
  }
}(typeof self !== 'undefined' ? self : this, function () {
  'use strict';

  // ─── Tunable Constants ───────────────────────────────────────────────────
  const COLLECT_SAMPLES           = 40;
  const STABILITY_STD_LIMIT       = 0.05;
  const CROSSING_HYSTERESIS       = 5.0;
  const MAX_BASELINE_SAMPLES      = 150;
  const BASELINE_SAMPLE_INTERVAL  = 100; // 100ms = 10Hz
  const BASELINE_TIMEOUT_MS       = 6000;
  const BASELINE_MOTION_ONSET_DEG = 10.0;
  const WRIST_CHEAT_ALIGNMENT     = 0.7;
  const DOCTOR_PIN                = "1234";

  // ─── 3D Vector Math Helpers ──────────────────────────────────────────────
  const Vec3 = {
    create: (x = 0, y = 0, z = 0) => ({ x: Number(x) || 0, y: Number(y) || 0, z: Number(z) || 0 }),
    
    normalize: (v) => {
      const mag = Math.sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
      if (mag < 1e-6) return { x: 0, y: 0, z: 0 };
      return { x: v.x / mag, y: v.y / mag, z: v.z / mag };
    },

    cross: (a, b) => ({
      x: a.y * b.z - a.z * b.y,
      y: a.z * b.x - a.x * b.z,
      z: a.x * b.y - a.y * b.x
    }),

    dot: (a, b) => a.x * b.x + a.y * b.y + a.z * b.z,

    add: (a, b) => ({ x: a.x + b.x, y: a.y + b.y, z: a.z + b.z }),

    scale: (a, s) => ({ x: a.x * s, y: a.y * s, z: a.z * s }),

    angleDiff: (a, b) => {
      let d = ((b - a + 180.0) % 360.0);
      if (d < 0) d += 360.0;
      return d - 180.0;
    },

    constrain: (val, min, max) => Math.min(Math.max(val, min), max)
  };

  function sortArray(arr) {
    return arr.slice().sort((a, b) => a - b);
  }

  // ─── Engine State ────────────────────────────────────────────────────────
  let filteredGravity   = { x: 0, y: 0, z: -1 };
  let lastFusionTime    = 0;
  let fusionInitialized = false;

  // Orthonormal Basis Plane
  let u  = { x: 0, y: 0, z: 0 }; // Step 1: Rest Vector
  let v  = { x: 0, y: 0, z: 0 }; // Step 2: Direction Vector
  let n  = { x: 0, y: 0, z: 0 }; // Plane Normal (u × v)
  let e1 = { x: 0, y: 0, z: 0 }; // Basis 1 (u)
  let e2 = { x: 0, y: 0, z: 0 }; // Basis 2 (n × u)
  let lastGyro = { x: 0, y: 0, z: 0 };

  // Calibration State: 0 = None, 1 = Step 1 Rest Done, 2 = Step 2 Direction Done (Calibrated), 3 = Baseline Active, 4 = Ready
  let calibState = 0;
  let formStatus = "Awaiting Step 1 (Resting)";

  // Collection State (Step 1 & 2)
  let isCollecting = false;
  let collectStage = 0;
  let collectSamples = [];
  let collectCallbacks = { onProgress: null, onComplete: null, onError: null };

  // Real-time Angle & Rep State
  let rawAngleForUnwrap = 0.0;
  let liveAngle         = 0.0;
  let twistError        = 0.0;
  let circleRawForUnwrap = 0.0;
  let circleLiveAngle   = 0.0;
  let lastLoopCount     = 0;
  let isRaised          = false;
  let reps              = 0;
  let repTimestamps     = [];

  // Prescription Targets (Configured by Doctor or Patient Session)
  let currentExercise   = "Bicep Curl";
  let targetMaxAngle    = 90.0;
  let maxDeviationPlane = 20.0;
  let targetReps        = 10;
  let isDoctorConfirmed = false;

  // 15s Baseline State
  let baselineActive    = false;
  let baselineStarted   = false;
  let baselineStartTime = 0;
  let lastSampleTime    = 0;
  let angleBuffer       = [];
  let swayBuffer        = [];
  let baselineCallbacks = { onStatus: null, onComplete: null, onError: null };

  // ─── 3D Gravity Fusion (Complementary Vector Filter) ─────────────────────
  function updateGravityEstimate(gyroDeg, accelG, dt) {
    const gyroRad = {
      x: gyroDeg.x * (Math.PI / 180.0),
      y: gyroDeg.y * (Math.PI / 180.0),
      z: gyroDeg.z * (Math.PI / 180.0)
    };

    const accelMps = {
      x: accelG.x * 9.80665,
      y: accelG.y * 9.80665,
      z: accelG.z * 9.80665
    };

    if (!fusionInitialized || isCollecting) {
      filteredGravity = Vec3.normalize(accelMps);
      fusionInitialized = true;
      return filteredGravity;
    }

    // 1. Gyro integration: predict gravity vector rotation
    const crossGyro = Vec3.cross(filteredGravity, gyroRad);
    let predicted = Vec3.add(filteredGravity, Vec3.scale(crossGyro, dt));
    predicted = Vec3.normalize(predicted);

    // 2. Accel trust weighting: dynamic confidence
    const accelMag = Math.sqrt(accelMps.x ** 2 + accelMps.y ** 2 + accelMps.z ** 2);
    const magError = Math.abs(accelMag - 9.80665) / 9.80665;
    const magConfidence = Vec3.constrain(1.0 - magError * 3.0, 0.0, 1.0);

    // Dynamic trust: high when gyro is near zero (static hold), low during fast motion
    const gyroMagRad = Math.sqrt(gyroRad.x ** 2 + gyroRad.y ** 2 + gyroRad.z ** 2);
    const gyroMagDeg = gyroMagRad * (180.0 / Math.PI);
    // At rest (gyro < 5°/s): trust = 0.5 (very fast convergence)
    // During motion (gyro > 50°/s): trust = 0.02 (gyro-dominant)
    const baseTrust = gyroMagDeg < 5.0 ? 0.5 : (gyroMagDeg < 50.0 ? 0.1 : 0.02);
    const accelTrust = baseTrust * magConfidence;

    // 3. Complementary Vector Fusion
    const accelDir = Vec3.normalize(accelMps);
    const fused = Vec3.add(Vec3.scale(predicted, 1.0 - accelTrust), Vec3.scale(accelDir, accelTrust));
    filteredGravity = Vec3.normalize(fused);
    return filteredGravity;
  }

  // ─── 2-Step Calibration Engine ───────────────────────────────────────────
  function startStep1(callbacks = {}) {
    collectStage = 1;
    collectSamples = [];
    isCollecting = true;
    collectCallbacks = callbacks;
    formStatus = "Hold still: capturing resting position...";
    if (collectCallbacks.onProgress) collectCallbacks.onProgress(0, COLLECT_SAMPLES, formStatus);
  }

  function startStep2(callbacks = {}) {
    if (calibState < 1) {
      const err = "Please perform Step 1 (Set Resting) first!";
      if (callbacks.onError) callbacks.onError(err);
      return false;
    }
    collectStage = 2;
    collectSamples = [];
    isCollecting = true;
    collectCallbacks = callbacks;
    formStatus = "Hold still: capturing raised direction...";
    if (collectCallbacks.onProgress) collectCallbacks.onProgress(0, COLLECT_SAMPLES, formStatus);
    return true;
  }

  function _serviceCollection(g) {
    if (!isCollecting) return;

    collectSamples.push({ x: g.x, y: g.y, z: g.z });
    const count = collectSamples.length;

    if (collectCallbacks.onProgress) {
      collectCallbacks.onProgress(count, COLLECT_SAMPLES, formStatus);
    }

    if (count >= COLLECT_SAMPLES) {
      isCollecting = false;

      // Compute mean vector
      let sumX = 0, sumY = 0, sumZ = 0;
      let sumSqX = 0, sumSqY = 0, sumSqZ = 0;
      for (const s of collectSamples) {
        sumX += s.x; sumY += s.y; sumZ += s.z;
        sumSqX += s.x * s.x; sumSqY += s.y * s.y; sumSqZ += s.z * s.z;
      }
      const mean = { x: sumX / count, y: sumY / count, z: sumZ / count };
      const meanSq = { x: sumSqX / count, y: sumSqY / count, z: sumSqZ / count };

      const varX = meanSq.x - mean.x * mean.x;
      const varY = meanSq.y - mean.y * mean.y;
      const varZ = meanSq.z - mean.z * mean.z;
      const stdTotal = Math.sqrt(Math.max(varX, 0) + Math.max(varY, 0) + Math.max(varZ, 0));

      // Stability verification
      if (stdTotal > STABILITY_STD_LIMIT) {
        formStatus = collectStage === 1 ? "Not still enough — retry Step 1" : "Not still enough — retry Step 2";
        if (collectCallbacks.onError) collectCallbacks.onError(formStatus);
        return;
      }

      const result = Vec3.normalize(mean);

      if (collectStage === 1) {
        u = result;
        calibState = 1;
        formStatus = "Step 1 OK. Move arm UP slightly for Step 2";
        if (collectCallbacks.onComplete) collectCallbacks.onComplete({ stage: 1, u });
      } else {
        v = result;
        const crossRaw = Vec3.cross(u, v);
        const crossMag = Math.sqrt(crossRaw.x ** 2 + crossRaw.y ** 2 + crossRaw.z ** 2);

        if (crossMag < 0.15) {
          calibState = 1;
          formStatus = "Move arm a bit more, then retry Step 2";
          if (collectCallbacks.onError) collectCallbacks.onError(formStatus);
          return;
        }

        // Establish orthonormal basis plane
        n  = Vec3.normalize(crossRaw);
        e1 = u;
        e2 = Vec3.normalize(Vec3.cross(n, u));

        // ------------------------------------------------------------------
        // ANALYTICAL ZERO (Rest = 0.0 degrees)
        // Set rawAngleForUnwrap & liveAngle to current raised angle (angle_v).
        // When arm drops back down to rest (u), rawAngle evaluates to 0.0° exactly.
        // ------------------------------------------------------------------
        const x_v = Vec3.dot(v, e1);
        const y_v = Vec3.dot(v, e2);
        const angle_v = Math.atan2(y_v, x_v) * (180.0 / Math.PI);

        rawAngleForUnwrap  = angle_v;
        liveAngle          = angle_v;
        circleRawForUnwrap = 0.0;
        circleLiveAngle    = 0.0;
        reps               = 0;
        isRaised           = false;
        calibState         = 2;
        formStatus         = "Calibrated & Ready (0.0° Locked)";

        if (collectCallbacks.onComplete) {
          collectCallbacks.onComplete({ stage: 2, u, v, n, e1, e2 });
        }
      }
    }
  }

  // ─── 15-Second Doctor Baseline Recording ──────────────────────────────────
  function startDoctorBaseline(pin, callbacks = {}) {
    if (pin !== DOCTOR_PIN) {
      if (callbacks.onError) callbacks.onError("Incorrect Doctor PIN!");
      return false;
    }
    if (calibState < 2) {
      if (callbacks.onError) callbacks.onError("Please complete Step 1 & Step 2 calibration first!");
      return false;
    }

    baselineActive    = true;
    baselineStarted   = false;
    baselineStartTime = performance.now();
    lastSampleTime    = 0;
    angleBuffer       = [];
    swayBuffer        = [];
    baselineCallbacks = callbacks;
    calibState        = 3;
    formStatus        = "Waiting for initial movement...";

    if (baselineCallbacks.onStatus) baselineCallbacks.onStatus(formStatus, 0, MAX_BASELINE_SAMPLES);
    return true;
  }

  function _serviceBaseline(gFused, now) {
    if (!baselineActive) return;

    if (!baselineStarted) {
      // Check timeout (5 seconds to begin motion)
      if (now - baselineStartTime > BASELINE_TIMEOUT_MS) {
        baselineActive = false;
        calibState = 2;
        formStatus = "Error: Baseline Timeout (No motion detected)";
        if (baselineCallbacks.onError) baselineCallbacks.onError(formStatus);
        return;
      }

      const angularDeviation = Math.acos(Vec3.constrain(Vec3.dot(gFused, u), -1.0, 1.0)) * (180.0 / Math.PI);
      if (angularDeviation > BASELINE_MOTION_ONSET_DEG) {
        if (liveAngle < 0) {
          baselineActive = false;
          calibState = 2;
          formStatus = "Error: Move Upward First";
          if (baselineCallbacks.onError) baselineCallbacks.onError(formStatus);
          return;
        } else {
          baselineStarted = true;
          lastSampleTime  = now;
          formStatus      = "Recording Baseline (15s)...";
        }
      }
      return;
    }

    // Sample at 10Hz (every 100ms)
    if (now - lastSampleTime >= BASELINE_SAMPLE_INTERVAL) {
      lastSampleTime = now;
      if (angleBuffer.length < MAX_BASELINE_SAMPLES) {
        angleBuffer.push(liveAngle);
        const swayVal = (currentExercise === "Wrist Circumduction") ? twistError : Math.abs(twistError);
        swayBuffer.push(swayVal);

        if (baselineCallbacks.onStatus) {
          baselineCallbacks.onStatus(
            `Recording: ${angleBuffer.length}/${MAX_BASELINE_SAMPLES} samples (${Math.round(angleBuffer.length / 10)}s)`,
            angleBuffer.length,
            MAX_BASELINE_SAMPLES
          );
        }
      }

      if (angleBuffer.length >= MAX_BASELINE_SAMPLES) {
        baselineActive = false;

        // Validation: Wrist Circumduction (Twist based)
        if (currentExercise === "Wrist Circumduction") {
          const meanSway = swayBuffer.reduce((a, b) => a + b, 0) / MAX_BASELINE_SAMPLES;
          let crossings = 0;
          let above = swayBuffer[0] > meanSway;
          for (let i = 1; i < MAX_BASELINE_SAMPLES; i++) {
            if (above && swayBuffer[i] < (meanSway - 1.0)) { crossings++; above = false; }
            else if (!above && swayBuffer[i] > (meanSway + 1.0)) { crossings++; above = true; }
          }
          if (crossings < 6) {
            calibState = 2;
            formStatus = "Error: Insufficient Motion (Perform at least 3 full circles)";
            if (baselineCallbacks.onError) baselineCallbacks.onError(formStatus);
            return;
          }

          const sortedSway = sortArray(swayBuffer);
          targetMaxAngle    = sortedSway[Math.floor(MAX_BASELINE_SAMPLES * 0.85)];
          maxDeviationPlane = sortedSway[Math.floor(MAX_BASELINE_SAMPLES * 0.15)];
        } else {
          // Standard Angle-based Validation
          const meanAngle = angleBuffer.reduce((a, b) => a + b, 0) / MAX_BASELINE_SAMPLES;
          let crossings = 0;
          let above = angleBuffer[0] > meanAngle;
          for (let i = 1; i < MAX_BASELINE_SAMPLES; i++) {
            if (above && angleBuffer[i] < (meanAngle - CROSSING_HYSTERESIS)) { crossings++; above = false; }
            else if (!above && angleBuffer[i] > (meanAngle + CROSSING_HYSTERESIS)) { crossings++; above = true; }
          }
          if (crossings < 6) {
            calibState = 2;
            formStatus = "Error: Insufficient Motion (Perform at least 3 full reps)";
            if (baselineCallbacks.onError) baselineCallbacks.onError(formStatus);
            return;
          }

          const sortedAngle = sortArray(angleBuffer);
          const sortedSway  = sortArray(swayBuffer);
          targetMaxAngle    = sortedAngle[Math.floor(MAX_BASELINE_SAMPLES * 0.90)];
          maxDeviationPlane = Math.max(10.0, sortedSway[Math.floor(MAX_BASELINE_SAMPLES * 0.90)]);
        }

        isDoctorConfirmed = true;
        calibState = 4;
        formStatus = `Baseline Saved! Target: ${targetMaxAngle.toFixed(1)}°, Sway: ${maxDeviationPlane.toFixed(1)}°`;

        if (baselineCallbacks.onComplete) {
          baselineCallbacks.onComplete({
            targetMaxAngle: parseFloat(targetMaxAngle.toFixed(1)),
            maxDeviationPlane: parseFloat(maxDeviationPlane.toFixed(1)),
            exercise: currentExercise
          });
        }
      }
    }
  }

  // ─── 6 Exercise Rep Engines ──────────────────────────────────────────────
  function processBicepCurl() {
    const safeTarget = Math.min(targetMaxAngle, 175.0);
    const THRESHOLD_UP = safeTarget - 15.0;
    let THRESHOLD_DOWN = 20.0;
    if (THRESHOLD_DOWN >= THRESHOLD_UP) THRESHOLD_DOWN = THRESHOLD_UP - 10.0;
    const HYPEREXTEND_LIMIT = Math.min(-15.0, THRESHOLD_DOWN - 20.0);
    const MAX_SWAY = maxDeviationPlane + 5.0;

    if (Math.abs(twistError) > MAX_SWAY) {
      formStatus = "Bad Form: Keep Movement Aligned!";
    } else if (liveAngle < HYPEREXTEND_LIMIT) {
      formStatus = "Bad Form: Arm Dropped";
    } else {
      if (liveAngle > THRESHOLD_UP && !isRaised) {
        isRaised = true;
        formStatus = "Hold Peak Curl...";
      } else if (liveAngle < THRESHOLD_DOWN && isRaised) {
        isRaised = false;
        reps++;
        repTimestamps.push(Date.now());
        formStatus = "Rep Complete! Ready";
      } else if (liveAngle > THRESHOLD_DOWN && liveAngle < THRESHOLD_UP) {
        formStatus = isRaised ? "Lowering..." : "Curling Up...";
      }
    }
  }

  function processFrontRaise() {
    const safeTarget = Math.min(targetMaxAngle, 175.0);
    const THRESHOLD_UP = safeTarget - 15.0;
    let THRESHOLD_DOWN = 20.0;
    if (THRESHOLD_DOWN >= THRESHOLD_UP) THRESHOLD_DOWN = THRESHOLD_UP - 10.0;
    const HYPEREXTEND_LIMIT = Math.min(-15.0, THRESHOLD_DOWN - 20.0);
    const MAX_SWAY = maxDeviationPlane + 5.0;

    if (Math.abs(twistError) > MAX_SWAY) {
      formStatus = "Bad Form: Stop Swaying Outward!";
    } else if (liveAngle < HYPEREXTEND_LIMIT) {
      formStatus = "Bad Form: Dropped Too Low";
    } else {
      if (liveAngle > THRESHOLD_UP && !isRaised) {
        isRaised = true;
        formStatus = "Hold Height...";
      } else if (liveAngle < THRESHOLD_DOWN && isRaised) {
        isRaised = false;
        reps++;
        repTimestamps.push(Date.now());
        formStatus = "Rep Complete! Ready";
      } else if (liveAngle > THRESHOLD_DOWN && liveAngle < THRESHOLD_UP) {
        formStatus = isRaised ? "Lowering Control..." : "Raising Forward...";
      }
    }
  }

  function processSideRaise() {
    const safeTarget = Math.min(targetMaxAngle, 175.0);
    const THRESHOLD_UP = safeTarget - 15.0;
    let THRESHOLD_DOWN = 20.0;
    if (THRESHOLD_DOWN >= THRESHOLD_UP) THRESHOLD_DOWN = THRESHOLD_UP - 10.0;
    const HYPEREXTEND_LIMIT = Math.min(-15.0, THRESHOLD_DOWN - 20.0);
    const MAX_SWAY = maxDeviationPlane + 3.0;

    if (Math.abs(twistError) > MAX_SWAY) {
      formStatus = "Bad Form: Keep Arm Strictly Sideways!";
    } else if (liveAngle < HYPEREXTEND_LIMIT) {
      formStatus = "Bad Form: Dropped Too Low";
    } else {
      if (liveAngle > THRESHOLD_UP && !isRaised) {
        isRaised = true;
        formStatus = "Hold Height...";
      } else if (liveAngle < THRESHOLD_DOWN && isRaised) {
        isRaised = false;
        reps++;
        repTimestamps.push(Date.now());
        formStatus = "Rep Complete! Ready";
      } else if (liveAngle > THRESHOLD_DOWN && liveAngle < THRESHOLD_UP) {
        formStatus = isRaised ? "Lowering Control..." : "Raising Sideways...";
      }
    }
  }

  function processLowerLegRaise() {
    const safeTarget = Math.min(targetMaxAngle, 175.0);
    const THRESHOLD_UP = safeTarget - 15.0;
    let THRESHOLD_DOWN = 20.0;
    if (THRESHOLD_DOWN >= THRESHOLD_UP) THRESHOLD_DOWN = THRESHOLD_UP - 10.0;
    const HYPEREXTEND_LIMIT = Math.min(-15.0, THRESHOLD_DOWN - 20.0);
    const MAX_SWAY = maxDeviationPlane + 5.0;

    if (Math.abs(twistError) > MAX_SWAY) {
      formStatus = "Bad Form: Keep Leg Aligned!";
    } else if (liveAngle < HYPEREXTEND_LIMIT) {
      formStatus = "Bad Form: Leg Dropped";
    } else {
      if (liveAngle > THRESHOLD_UP && !isRaised) {
        isRaised = true;
        formStatus = "Hold Peak Raise...";
      } else if (liveAngle < THRESHOLD_DOWN && isRaised) {
        isRaised = false;
        reps++;
        repTimestamps.push(Date.now());
        formStatus = "Rep Complete! Ready";
      } else if (liveAngle > THRESHOLD_DOWN && liveAngle < THRESHOLD_UP) {
        formStatus = isRaised ? "Lowering Leg..." : "Raising Leg...";
      }
    }
  }

  function processHandCircle() {
    const currentLoop = Math.floor(Math.abs(circleLiveAngle) / 360.0);
    if (currentLoop > lastLoopCount) {
      reps += (currentLoop - lastLoopCount);
      lastLoopCount = currentLoop;
      repTimestamps.push(Date.now());
      formStatus = "Loop Complete! Keep Circling...";
    } else {
      formStatus = "Circling Hand...";
    }
  }

  function processWristCircle() {
    const THRESHOLD_UP = targetMaxAngle - 2.0;
    let THRESHOLD_DOWN = maxDeviationPlane + 2.0;
    if (THRESHOLD_DOWN >= THRESHOLD_UP) THRESHOLD_DOWN = THRESHOLD_UP - 1.0;

    if (twistError > THRESHOLD_UP && !isRaised) {
      isRaised = true;
      formStatus = "Hold Peak Turn...";
    } else if (twistError < THRESHOLD_DOWN && isRaised) {
      isRaised = false;
      reps++;
      repTimestamps.push(Date.now());
      formStatus = "Rep Complete! Ready";
    } else if (twistError > THRESHOLD_DOWN && twistError < THRESHOLD_UP) {
      formStatus = isRaised ? "Turning back..." : "Turning forward...";
    }

    const gyroMag = Math.sqrt(lastGyro.x ** 2 + lastGyro.y ** 2 + lastGyro.z ** 2);
    if (gyroMag > 0.3) {
      const normG = Vec3.normalize(lastGyro);
      const axisAlignment = Math.abs(Vec3.dot(normG, u));
      if (axisAlignment > WRIST_CHEAT_ALIGNMENT) {
        formStatus = "Bad Form: Rotate Hand Only, Not Forearm!";
      }
    }
  }

  function routeActiveExercise() {
    if (currentExercise === "Bicep Curl") processBicepCurl();
    else if (currentExercise === "Front Shoulder Raise" || currentExercise === "Front Raise") processFrontRaise();
    else if (currentExercise === "Side Shoulder Raise" || currentExercise === "Side Raise") processSideRaise();
    else if (currentExercise === "Lower Leg Raise" || currentExercise === "Leg Raise") processLowerLegRaise();
    else if (currentExercise === "Hand Circular Movement" || currentExercise === "Hand Circle") processHandCircle();
    else if (currentExercise === "Wrist Circumduction" || currentExercise === "Wrist Circle") processWristCircle();
    else processBicepCurl();
  }

  // ─── Main Sensor Processing Entry Point ──────────────────────────────────
  function processSensorData(raw) {
    const now = performance.now();
    let dt = lastFusionTime ? (now - lastFusionTime) / 1000 : 0.02;
    // Clamp dt: if called faster than 5ms (test/batch), assume 20ms (50Hz BLE)
    if (dt < 0.005) dt = 0.02;
    if (dt > 0.5) dt = 0.02; // cap at 500ms to reject stale gaps
    lastFusionTime = now;

    const accelG = {
      x: parseFloat(raw.ax) || 0,
      y: parseFloat(raw.ay) || 0,
      z: parseFloat(raw.az) || 0
    };

    const gyroDeg = {
      x: parseFloat(raw.gx) || 0,
      y: parseFloat(raw.gy) || 0,
      z: parseFloat(raw.gz) || 0
    };
    lastGyro = gyroDeg;

    // 1. Update 3D Gravity Fusion
    const gFused = updateGravityEstimate(gyroDeg, accelG, dt);

    // 2. Process Step 1 / 2 collection
    if (isCollecting) {
      _serviceCollection(gFused);
    }

    // 3. If Calibrated: project into Basis Plane
    if (calibState >= 2) {
      const n_dot = Vec3.constrain(Vec3.dot(gFused, n), -1.0, 1.0);
      twistError = Math.asin(n_dot) * (180.0 / Math.PI);

      const x = Vec3.dot(gFused, e1);
      const y = Vec3.dot(gFused, e2);
      const rawAngle = Math.atan2(y, x) * (180.0 / Math.PI);

      liveAngle += Vec3.angleDiff(rawAngleForUnwrap, rawAngle);
      rawAngleForUnwrap = rawAngle;

      if (liveAngle > 360.0) liveAngle -= 360.0;
      else if (liveAngle < -360.0) liveAngle += 360.0;

      // Circular projection
      const cx = Vec3.dot(gFused, n);
      const cy = Vec3.dot(gFused, e2);
      const cRaw = Math.atan2(cy, cx) * (180.0 / Math.PI);
      circleLiveAngle += Vec3.angleDiff(circleRawForUnwrap, cRaw);
      circleRawForUnwrap = cRaw;

      // 4. Baseline State (State 3)
      if (calibState === 3) {
        _serviceBaseline(gFused, now);
      }
      // 5. Active Exercise State (State 4 or Calibrated)
      else if (calibState === 4 || calibState === 2) {
        routeActiveExercise();
      }
    }

    const isCircular = (currentExercise === "Hand Circular Movement" || currentExercise === "Wrist Circumduction");
    const displayAngle = isCircular ? (Math.abs(circleLiveAngle) % 360.0) : Math.abs(liveAngle);

    return {
      liveAngle: displayAngle,
      twistError: Math.abs(twistError),
      signedAngle: liveAngle,
      signedTwist: twistError,
      reps,
      formStatus,
      calibState,
      isRaised,
      fusedGravity: gFused,
      accel: accelG,
      gyro: gyroDeg,
      temp: parseFloat(raw.tp) || 36.5
    };
  }

  // ─── Prescription / Config Setters ───────────────────────────────────────
  function setPrescription(config) {
    if (config.exerciseType) currentExercise = config.exerciseType;
    if (config.maxAngle !== undefined) targetMaxAngle = parseFloat(config.maxAngle);
    if (config.maxDeviationPlane !== undefined) maxDeviationPlane = parseFloat(config.maxDeviationPlane);
    if (config.motionLimit !== undefined) maxDeviationPlane = parseFloat(config.motionLimit);
    if (config.targetReps !== undefined) targetReps = parseInt(config.targetReps);
    isDoctorConfirmed = true;
  }

  function resetSession() {
    reps = 0;
    isRaised = false;
    liveAngle = 0.0;
    rawAngleForUnwrap = 0.0;
    circleLiveAngle = 0.0;
    circleRawForUnwrap = 0.0;
    lastLoopCount = 0;
    repTimestamps = [];
    formStatus = "Ready";
  }

  // ─── Public API ──────────────────────────────────────────────────────────
  return {
    Vec3,
    startStep1,
    startStep2,
    startDoctorBaseline,
    processSensorData,
    setPrescription,
    resetSession,
    isCalibrated: () => calibState >= 2,
    getCalibState: () => calibState,
    getReps: () => reps,
    getLiveAngle: () => Math.abs(liveAngle),
    getTwistError: () => Math.abs(twistError),
    getFormStatus: () => formStatus,
    getPrescription: () => ({
      exerciseType: currentExercise,
      targetMaxAngle,
      maxDeviationPlane,
      targetReps
    })
  };
}));
