/**
 * PhysioPulse – Main App Controller
 * Coordinates: auth gating, role-based routing, data pipeline, sensor UI updates, settings, demo mode
 */

const App = (() => {
  // ─── State ───────────────────────────────────────────────────────────────
  let currentPage = null;
  let userRole    = null;   // 'doctor' | 'patient'
  let userSession = null;

  let motionThreshold = 100;
  let tempThreshold   = 40;
  let demoMode        = false;
  let demoInterval    = null;
  let lastUpdateTime  = 0;
  let frameCount      = 0;
  let rateInterval    = null;
  let rate            = 0;
  let latestSensorData = { pitch: 0, roll: 0 };

  // Temp stats
  let tempMin = Infinity, tempMax = -Infinity, tempSum = 0, tempCount = 0;

  // Gyro integration for yaw
  let yaw = 0;
  let lastDataTime = null;

  // ─── Toast Notification System ───────────────────────────────────────────
  function showToast(msg, type = 'info') {
    const container = document.getElementById('toastContainer');
    if (!container) return;
    const icons = { success: '✓', error: '✕', info: 'ℹ', warning: '⚠' };
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    toast.innerHTML = `<span style="font-size:1rem">${icons[type]||'ℹ'}</span> ${msg}`;
    container.appendChild(toast);
    setTimeout(() => {
      toast.style.transition = '0.3s ease';
      toast.style.opacity = '0';
      toast.style.transform = 'translateX(20px)';
      setTimeout(() => toast.remove(), 350);
    }, 3500);
  }

  // ─── Page Routing ─────────────────────────────────────────────────────────
  function navigateTo(page) {
    if (currentPage === page) return;
    currentPage = page;

    // Deactivate all nav items and pages
    document.querySelectorAll('.nav-item').forEach(el => el.classList.remove('active'));
    document.querySelectorAll('.page').forEach(el => el.classList.remove('active'));

    // Activate selected page
    const pageEl = document.getElementById('page-' + page);
    if (pageEl) pageEl.classList.add('active');

    // Activate nav item(s) matching data-page
    document.querySelectorAll(`.nav-item[data-page="${page}"]`).forEach(el => el.classList.add('active'));

    // Update topbar title
    const titles = {
      'doctor-dashboard': 'Doctor Dashboard',
      'doctor-patients':  'My Patients',
      'doctor-patient':   'Patient Detail',
      'patient-dashboard':'My Dashboard',
      'patient-exercise': 'Exercise Session',
      'patient-history':  'Exercise History',
      live:               'Live Charts',
      settings:           'Settings',
      about:              'About PhysioPulse',
    };
    const topbarTitle = document.getElementById('topbarTitle');
    if (topbarTitle) topbarTitle.textContent = titles[page] || page;

    // Page-specific actions
    if (page === 'doctor-patients') {
      _refreshDoctorPatients();
    }
    if (page === 'patient-exercise') {
      PatientPortal.renderExercisePage();
    }
    if (page === 'patient-history') {
      PatientPortal.renderPatientHistory();
    }

    closeSidebar();
  }

  async function _refreshDoctorPatients() {
    if (!userSession) return;
    const patients = await Auth.getPatientsByDoctorId(userSession.userId);
    // mirror into full list
    const el = document.getElementById('doctorPatientCardsFull');
    const codeEl = document.getElementById('doctorCodePatients');
    if (codeEl) codeEl.textContent = userSession.doctorCode || '—';
    if (el && patients.length === 0) {
      el.innerHTML = `<div class="empty-state" style="grid-column:1/-1"><div class="empty-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87M16 3.13a4 4 0 0 1 0 7.75"/></svg></div><h2>No patients yet</h2><p>Your User ID is <strong>${userSession.doctorCode || '—'}</strong>. Share it with patients so they can register and link to you.</p></div>`;
    } else if (el) {
      // Delegate rendering to DoctorPortal helper by inserting placeholder and calling render
      const cards = await Promise.all(patients.map(async (p) => {
        const threshold = await Auth.getThresholdForPatient(p.id);
        const history   = await Auth.getExerciseHistoryForPatient(p.id);
        const lastSession = history[0];
        const compliance = history.length > 0
          ? Math.round(history.filter(h => h.status === 'completed').length / history.length * 100)
          : null;
        return `
          <div class="patient-card" onclick="DoctorPortal.openPatientDetail('${p.id}')">
            <div class="pc-header">
              <div class="pc-avatar">${(p.name || 'P').charAt(0).toUpperCase()}</div>
              <div class="pc-info"><div class="pc-name">${p.name}</div><div class="pc-meta">${p.condition || 'No condition set'} · Age: ${p.age || '—'}</div></div>
              <div class="pc-compliance ${compliance === null ? '' : compliance >= 80 ? 'good' : compliance >= 50 ? 'warn' : 'bad'}">${compliance === null ? '—' : compliance + '%'}</div>
            </div>
            <div class="pc-thresholds">${threshold ? `<span class="pc-thresh-item">Min: <strong>${threshold.minAngle}°</strong></span><span class="pc-thresh-item">Max: <strong>${threshold.maxAngle}°</strong></span><span class="pc-thresh-item">Reps: <strong>${threshold.targetReps}</strong></span>` : '<span class="pc-thresh-none">No thresholds set</span>'}</div>
            <div class="pc-footer"><span class="pc-last">${lastSession ? 'Last: ' + new Date(lastSession.date).toLocaleDateString() : 'No sessions yet'}</span><span class="pc-sessions">${history.length} sessions</span></div>
          </div>
        `;
      }));
      el.innerHTML = cards.join('');
    }
  }

  // ─── Sidebar (Mobile) ─────────────────────────────────────────────────────
  function openSidebar() {
    document.getElementById('sidebar')?.classList.add('open');
    document.getElementById('sidebarOverlay')?.classList.add('active');
  }
  function closeSidebar() {
    document.getElementById('sidebar')?.classList.remove('open');
    document.getElementById('sidebarOverlay')?.classList.remove('active');
  }

  // ─── Auth: Show / Hide Screens ────────────────────────────────────────────
  function showAuthScreen() {
    document.getElementById('splash-screen')?.classList.add('fade-out');
    document.getElementById('auth-screen')?.classList.remove('hidden');
    document.getElementById('app')?.classList.add('hidden');
  }

  function showApp(session) {
    userSession = session;
    userRole = session.role;

    document.getElementById('auth-screen')?.classList.add('hidden');
    document.getElementById('app')?.classList.remove('hidden');

    // Update sidebar user info
    const name = session.name;
    const role = session.role;
    _setEl('sidebarUserName', name);
    _setEl('sidebarUserRole', role === 'doctor' ? '🩺 Doctor' : '🧑 Patient');
    _setEl('sidebarUserAvatar', name.charAt(0).toUpperCase());

    // Role badge in topbar
    const roleBadge = document.getElementById('roleBadge');
    if (roleBadge) {
      roleBadge.textContent = role === 'doctor' ? '🩺 Doctor' : '🧑 Patient';
      roleBadge.className = `role-badge role-${role}`;
    }

    // Show correct nav
    document.getElementById('nav-doctor')?.style.setProperty('display', role === 'doctor' ? 'block' : 'none');
    document.getElementById('nav-patient')?.style.setProperty('display', role === 'patient' ? 'block' : 'none');

    // Navigate to home page
    if (role === 'doctor') {
      navigateTo('doctor-dashboard');
      DoctorPortal.init(session);
    } else {
      navigateTo('patient-dashboard');
      PatientPortal.init(session);
      // Fill exercise info banner
      _fillExerciseBanner();
    }

    // Start sensor connection
    const host = window.location.hostname;
    const isESP32Host = host === '192.168.4.1' || host === 'physiopulse.local' || host.startsWith('192.168.4.');
    if (isESP32Host) {
      const wsUrl = `ws://${host}:81`;
      // Default to BLE since WiFi is not loading
      setTimeout(() => Connection.connect({ mode: 'ble', autoReconnect: true }), 800);
      const wsUrlEl = document.getElementById('wsUrl');
      if (wsUrlEl) wsUrlEl.value = wsUrl;
    } else {
      setTimeout(() => {
        showToast('Not connected to ESP32. Demo mode active.', 'info');
        startDemo();
      }, 1500);
    }
  }

  async function _fillExerciseBanner() {
    try {
      bindAuthEvents();
      const patient = await Auth.getPatientByUserId(userSession.userId);
      if (!patient) return;
      const threshold = await Auth.getThresholdForPatient(patient.id);
      if (!threshold) return;
      _setEl('ptExerciseTarget', threshold.targetReps + ' reps');
      _setEl('ptExerciseName', threshold.exerciseType || 'Exercise');
      _setEl('ptExerciseRange', threshold.minAngle + '° – ' + threshold.maxAngle + '°');
      // Update motion/temp thresholds used by generic alert system too
      motionThreshold = threshold.motionLimit || 100;
      tempThreshold   = threshold.tempLimit   || 40;
    } catch (e) { /* ignore */ }
  }

  // ─── Logout ───────────────────────────────────────────────────────────────
  function logout() {
    stopDemo();
    Auth.logout();
    userSession = null;
    userRole    = null;
    currentPage = null;

    // Reset UI
    document.getElementById('app')?.classList.add('hidden');
    document.getElementById('auth-screen')?.classList.remove('hidden');

    // Show login form
    document.getElementById('auth-login')?.classList.remove('hidden');
    document.getElementById('auth-register')?.classList.add('hidden');

    showToast('Signed out successfully', 'info');
  }

  // ─── Sensor Data Processing ───────────────────────────────────────────────
  function processSensorData(raw) {
    const now = performance.now();
    const dt = lastDataTime ? (now - lastDataTime) / 1000 : 0.05;
    lastDataTime = now;

    const ax = parseFloat(raw.ax) || 0;
    const ay = parseFloat(raw.ay) || 0;
    const az = parseFloat(raw.az) || 0;
    const gx = parseFloat(raw.gx) || 0;
    const gy = parseFloat(raw.gy) || 0;
    const gz = parseFloat(raw.gz) || 0;
    const tp = parseFloat(raw.tp) || 0;

    let engineResult = null;
    if (typeof PhysioEngine !== 'undefined' && PhysioEngine.processSensorData) {
      engineResult = PhysioEngine.processSensorData({ ax, ay, az, gx, gy, gz, tp });
    }

    let pitch = 0;
    if (engineResult && engineResult.calibState >= 2) {
      pitch = engineResult.liveAngle;
    } else if (raw.pitch !== undefined && raw.pitch !== null) {
      pitch = parseFloat(raw.pitch) || 0;
    } else if (raw.a !== undefined && raw.a !== null) {
      pitch = parseFloat(raw.a) || 0;
    } else if (ax !== 0 || ay !== 0 || az !== 0) {
      pitch = Math.atan2(ax, Math.sqrt(ay * ay + az * az)) * (180 / Math.PI);
    }

    const roll = raw.roll !== undefined ? raw.roll : (ax !== 0 || ay !== 0 || az !== 0 ? Math.atan2(ay, Math.sqrt(ax * ax + az * az)) * (180 / Math.PI) : 0);
    yaw += gz * dt;
    if (yaw > 180)  yaw -= 360;
    if (yaw < -180) yaw += 360;

    const hardwareReps = engineResult ? engineResult.reps : (raw.reps !== undefined ? raw.reps : (raw.r !== undefined ? raw.r : 0));
    const statusMsg = engineResult ? engineResult.formStatus : (raw.message || raw.msg || '');

    latestSensorData = {
      pitch: Math.abs(pitch),
      roll,
      reps: hardwareReps,
      message: statusMsg,
      ax, ay, az, gx, gy, gz, tp,
      _pitch: Math.abs(pitch),
      _roll: roll,
      _yaw: yaw,
      engine: engineResult
    };

    const data = {
      ax, ay, az, gx, gy, gz, tp,
      pitch: Math.abs(pitch),
      roll,
      _pitch: Math.abs(pitch),
      _roll: roll,
      _yaw: yaw,
      reps: hardwareReps,
      message: statusMsg,
      engine: engineResult
    };

    updateSensorUI(data);
    Charts.push(data);
    Session.addDataPoint(data);

    // Patient rep counting — always process if an exercise session is currently active
    if (typeof PatientPortal !== 'undefined' && PatientPortal.processSensorForReps) {
      PatientPortal.processSensorForReps(data);
    }

    // Doctor live calibration & baseline sensor update
    if (typeof DoctorPortal !== 'undefined' && DoctorPortal.updateDoctorLiveSensor) {
      DoctorPortal.updateDoctorLiveSensor(data);
    }

    frameCount++;
    lastUpdateTime = now;
    return data;
  }

  // ─── Sensor UI Updater ────────────────────────────────────────────────────
  function updateSensorUI(d) {
    const { ax, ay, az, gx, gy, gz, tp, _pitch, _roll, _yaw } = d;

    const accelMag = Math.sqrt(ax**2 + ay**2 + az**2);
    _setEl('accelX', ax.toFixed(3));
    _setEl('accelY', ay.toFixed(3));
    _setEl('accelZ', az.toFixed(3));
    _setEl('accelMag', accelMag.toFixed(3) + ' g');
    _setBar('accelXBar', ax, 2);
    _setBar('accelYBar', ay, 2);
    _setBar('accelZBar', az, 2);

    const gyroMag = Math.sqrt(gx**2 + gy**2 + gz**2);
    _setEl('gyroX', gx.toFixed(3));
    _setEl('gyroY', gy.toFixed(3));
    _setEl('gyroZ', gz.toFixed(3));
    _setEl('gyroMag', gyroMag.toFixed(3) + ' °/s');
    _setBar('gyroXBar', gx, 250);
    _setBar('gyroYBar', gy, 250);
    _setBar('gyroZBar', gz, 250);

    _setEl('tempValue', tp.toFixed(1));
    if (tp < tempMin) tempMin = tp;
    if (tp > tempMax) tempMax = tp;
    tempSum += tp; tempCount++;
    _setEl('tempMin', tempMin.toFixed(1));
    _setEl('tempMax', tempMax.toFixed(1));
    _setEl('tempAvg', (tempSum / tempCount).toFixed(2));
    _updateTempGauge(tp);

    _setEl('pitchVal', Math.round(_pitch) + '°');
    _setEl('rollVal',  Math.round(_roll) + '°');
    _setEl('yawVal',   Math.round(_yaw) + '°');
    _setArcOffset('pitchArc', _pitch, 116);
    _setArcOffset('rollArc',  _roll,  116);
    _setArcOffset('yawArc',   _yaw,   116);

    // ROM alert (generic, overridden by patient-specific alerts during exercise)
    const alert = document.getElementById('romAlert');
    const alertText = document.getElementById('romAlertText');
    if (gyroMag > motionThreshold) {
      alert?.classList.remove('hidden');
      if (alertText) alertText.textContent = `High motion: ${gyroMag.toFixed(1)} °/s`;
    } else if (tp > tempThreshold) {
      alert?.classList.remove('hidden');
      if (alertText) alertText.textContent = `High temp: ${tp.toFixed(1)} °C`;
    } else {
      alert?.classList.add('hidden');
    }

    const ts = new Date();
    _setEl('dashTimestamp', ts.toLocaleTimeString() + '.' + ts.getMilliseconds().toString().padStart(3,'0'));
  }

  // ─── Helpers ──────────────────────────────────────────────────────────────
  function _setEl(id, val) {
    const el = document.getElementById(id);
    if (el) el.textContent = val;
  }
  function _setBar(id, value, maxVal) {
    const el = document.getElementById(id);
    if (!el) return;
    const pct = Math.min(Math.abs(value) / maxVal * 100, 100);
    el.style.width = pct + '%';
  }
  function _updateTempGauge(temp) {
    const fill = document.getElementById('tempGaugeFill');
    const needle = document.getElementById('tempNeedle');
    if (!fill || !needle) return;
    const pct = Math.max(0, Math.min(1, (temp - 20) / 30));
    fill.setAttribute('stroke-dashoffset', 220 - pct * 220);
    needle.setAttribute('transform', `rotate(${-90 + pct * 180}, 80, 90)`);
  }
  function _setArcOffset(id, angleDeg, maxDash) {
    const el = document.getElementById(id);
    if (!el) return;
    const pct = Math.max(0, Math.min(1, Math.abs(angleDeg) / 180));
    el.setAttribute('stroke-dashoffset', maxDash - pct * maxDash);
  }

  // ─── Rate Counter ──────────────────────────────────────────────────────────
  function startRateCounter() {
    rateInterval = setInterval(() => {
      rate = frameCount;
      frameCount = 0;
    }, 1000);
  }

  // ─── Demo Mode ────────────────────────────────────────────────────────────
  function startDemo() {
    if (demoInterval) clearInterval(demoInterval); // prevent double-start
    demoMode = true;
    let t = 0;
    demoInterval = setInterval(() => {
      t += 0.05;
      const noise = () => (Math.random() - 0.5) * 0.8;

      let pitchDeg;
      if (t < 3.0) {
        // ── Quick calibration phase (first 3s): resting arm at 5° ──
        pitchDeg = 5.0 + noise() * 0.2;
      } else {
        // ── Active phase: full bicep curl cycle 5° → 85° → 5° (~1 rep every 3s) ──
        const activeT = t - 3.0;
        // Swing from 5° up to 85° (amplitude 80°)
        const curl = 5.0 + 80.0 * Math.abs(Math.sin(activeT * 1.05));
        pitchDeg = Math.max(0, curl + noise());
      }

      const gx = Math.cos(t) * 15;
      const gy = Math.sin(t * 0.7) * 10;
      const gz = Math.sin(t * 0.3) * 2; // low lateral twist error

      processSensorData({
        pitch: pitchDeg,
        a:     pitchDeg,
        ax: Math.sin(pitchDeg * Math.PI / 180),
        ay: 0.02,
        az: Math.cos(pitchDeg * Math.PI / 180),
        gx, gy, gz,
        tp: 36.5 + Math.sin(t * 0.1) * 0.5,
        t:  Date.now(),
        cal: true,
      });
    }, 50);
  }
  function stopDemo() {
    demoMode = false;
    clearInterval(demoInterval);
    demoInterval = null;
  }



  // ─── Auth Event Listeners ─────────────────────────────────────────────────
  function bindAuthEvents() {
    // Login
    document.getElementById('loginBtn')?.addEventListener('click', doLogin);
    document.getElementById('loginPassword')?.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') doLogin();
    });

    document.getElementById('loginUserId')?.addEventListener('input', (e) => {
      e.target.value = e.target.value.toUpperCase();
    });

    // Logout
    document.getElementById('logoutBtn')?.addEventListener('click', logout);
  }

  async function doLogin() {
    const loginId  = document.getElementById('loginUserId')?.value?.trim()?.toUpperCase() || '';
    const password = document.getElementById('loginPassword')?.value;
    const errorEl  = document.getElementById('loginError');
    const btnText  = document.getElementById('loginBtnText');
    const btnLoad  = document.getElementById('loginBtnLoader');

    if (btnText) btnText.classList.add("hidden");
    if (btnLoad) btnLoad.classList.remove('hidden');
    if (errorEl) errorEl.classList.add('hidden');

    try {
      bindAuthEvents();
      const session = await Auth.login(loginId, password);
      showApp(session);
    } catch (e) {
      if (errorEl) { errorEl.textContent = e.message; errorEl.classList.remove('hidden'); }
    } finally {
      if (btnText) btnText.classList.remove('hidden');
      if (btnLoad) btnLoad.classList.add('hidden');
    }
  }

  async function doRegister() {
    const name         = document.getElementById('regName')?.value?.trim();
    const preferredId  = document.getElementById('regUserId')?.value?.trim()?.toUpperCase() || '';
    const password     = document.getElementById('regPassword')?.value;
    const doctorUserId = document.getElementById('regDoctorUserId')?.value?.trim()?.toUpperCase() || '';
    const activeRole = document.querySelector('.role-btn.active')?.dataset.role || 'doctor';
    const errorEl    = document.getElementById('registerError');
    const btnText    = document.getElementById('registerBtnText');
    const btnLoad    = document.getElementById('registerBtnLoader');

    if (btnText) btnText.classList.add("hidden");
    if (btnLoad) btnLoad.classList.remove('hidden');
    if (errorEl) errorEl.classList.add('hidden');

    try {
      bindAuthEvents();
      const registration = await Auth.register(activeRole, name, preferredId, password, doctorUserId);
      showToast(`Account created. Your User ID: ${registration.loginId}`, 'success');
      const session = await Auth.login(registration.loginId, password);
      showApp(session);
    } catch (e) {
      if (errorEl) { errorEl.textContent = e.message; errorEl.classList.remove('hidden'); }
    } finally {
      if (btnText) btnText.classList.remove('hidden');
      if (btnLoad) btnLoad.classList.add('hidden');
    }
  }

  // ─── App Event Listeners ──────────────────────────────────────────────────
  function bindAppEvents() {
    // Navigation – all .nav-item clicks
    document.querySelectorAll('.nav-item').forEach(el => {
      el.addEventListener('click', (e) => {
        e.preventDefault();
        navigateTo(el.dataset.page);
      });
    });

    // Sidebar mobile
    document.getElementById('menuBtn')?.addEventListener('click', openSidebar);
    document.getElementById('sidebarClose')?.addEventListener('click', closeSidebar);
    document.getElementById('sidebarOverlay')?.addEventListener('click', closeSidebar);

    // Connection badge click → settings
    document.getElementById('connBadge')?.addEventListener('click', () => navigateTo('settings'));

    // Settings: connection mode toggle
    document.getElementById('connModeToggle')?.addEventListener('click', (e) => {
      const btn = e.target.closest('.toggle-btn');
      if (!btn) return;
      document.querySelectorAll('.toggle-btn').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
    });

    // Settings: connect
    document.getElementById('connectBtn')?.addEventListener('click', () => {
      const modeEl = document.querySelector('.toggle-btn.active');
      const mode   = modeEl?.dataset.mode || 'wifi';
      const url    = document.getElementById('wsUrl')?.value?.trim();
      const autoRec= document.getElementById('autoReconnect')?.checked;
      stopDemo();
      Connection.connect({ mode, url, autoReconnect: autoRec });
    });

    // Settings: calibrate
    document.getElementById('calibrateBtn')?.addEventListener('click', () => Connection.calibrate());

    // Settings: refresh device info
    document.getElementById('refreshDeviceInfo')?.addEventListener('click', () => Connection.fetchDeviceInfo());

    // Time window
    document.getElementById('timeWindow')?.addEventListener('change', (e) => {
      Charts.setBufferSize(parseInt(e.target.value) * 20);
    });

    // Buffer size
    document.getElementById('bufferSize')?.addEventListener('change', (e) => {
      Charts.setBufferSize(parseInt(e.target.value));
    });
  }

  // ─── Connection Handlers ───────────────────────────────────────────────────
  function setupConnectionHandlers() {
    Connection.on('data', processSensorData);
    Connection.on('status', (status) => {
      if (status === 'connected') stopDemo();
    });
    Connection.on('error', (msg) => showToast('Connection error: ' + msg, 'error'));
  }

  // ─── Splash Screen ────────────────────────────────────────────────────────
  function runSplash(onComplete) {
    const fill = document.getElementById('loaderFill');
    const text = document.getElementById('loaderText');
    const steps = [
      [20,  'Initializing…'],
      [40,  'Loading Auth…'],
      [60,  'Opening Database…'],
      [80,  'Checking Session…'],
      [100, 'Ready!'],
    ];
    let i = 0;
    const advance = () => {
      if (i >= steps.length) {
        setTimeout(() => {
          document.getElementById('splash-screen').classList.add('fade-out');
          onComplete();
        }, 300);
        return;
      }
      const [pct, msg] = steps[i++];
      if (fill) fill.style.width = pct + '%';
      if (text) text.textContent = msg;
      setTimeout(advance, 400);
    };
    setTimeout(advance, 200);
  }

  // ─── Init ─────────────────────────────────────────────────────────────────
  async function init() {
    try {
      bindAuthEvents();
      await Auth.init();
      await Session.init();
      Charts.init();
       
      bindAppEvents();
      setupConnectionHandlers();
      startRateCounter();
    } catch (e) {
      console.error('[App] Init failed:', e);
      const splash = document.getElementById('splash-screen');
      splash?.classList.add('fade-out');
      showAuthScreen();
      showToast(e.message || 'Initialization failed', 'error');
      return;
    }

    runSplash(async () => {
      // Check for existing valid session
      const existingSession = Auth.getSession();
      if (existingSession) {
        showApp(existingSession);
      } else {
        showAuthScreen();
      }
    });

    console.log('[App] PhysioPulse initialized ✓');
  }

  return { init, showToast, navigateTo, logout, startDemo, stopDemo, isDemo: () => demoMode, getLatestData: () => latestSensorData };
})();

// ─── Bootstrap ───────────────────────────────────────────────────────────────
document.addEventListener('DOMContentLoaded', App.init.bind(App));

// ─── PWA Service Worker Registration ─────────────────────────────────────────
if ('serviceWorker' in navigator) {
  window.addEventListener('load', () => {
    navigator.serviceWorker.register('/sw.js')
      .then(reg => console.log('[PWA] Service worker registered:', reg.scope))
      .catch(err => console.warn('[PWA] SW registration failed:', err));
  });
}
