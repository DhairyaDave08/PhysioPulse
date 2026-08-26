| **publishDate** | 2026-08-25 |
| :--- | :--- |
| **title** | PhysioPulse - Smart Physiotherapy Monitoring Sleeve |
| **excerpt** | A wearable physiotherapy monitoring system combining MYOSA sensing... |
| **image** | ![PhysioPulse Cover](physiopulse-myosa-cover-image.PNG) |
| **tags** | MYOSA, Physiotherapy, Biomechanics, LinearAlgebra, WearableTechnology, MotionAnalysis, WebBluetooth, FastAPI, DigitalHealth |

> From raw sensor vectors to biologically meaningful movement — PhysioPulse makes rehabilitation measurable, explainable, and personalized.

---

## Acknowledgements

PhysioPulse was developed for MYOSA 6.0 by a team from Dhirubhai Ambani University:

- Dave Dhairya Kalpeshkumar
- Rudra Mehul Bhatt
- Dhruvam Kanaiyalal Panchal
- Shubh Kashyap Patel

We thank Dr. Manish Kumar for serving as our faculty mentor and Dhirubhai Ambani University for providing the required facilities.

We sincerely acknowledge MYOSA for providing the sensing platform and embedded environment that enabled us to explore the integration of wearable sensing, biomechanics, linear algebra, and physiotherapy-oriented motion analysis.

---

## Overview

After an orthopaedic operation, the orthopaedic doctor normally prescribes physiotherapy exercises. The patient then travels to a physiotherapy centre every day to perform those exercises under supervision. Frequent visits can become expensive and time-consuming, especially during a long rehabilitation period.

PhysioPulse changes this workflow. The orthopaedic doctor can provide the MYOSA-based kit directly to the patient along with the prescribed exercise and safe angle targets. The patient performs the exercise at home, while the kit measures movement and records the session for later review. This keeps the doctor's prescription at the centre of the workflow and reduces the need for daily travel to a physiotherapy centre.

PhysioPulse combines an ESP32, an MPU-6050 motion sensor, a browser dashboard, and a FastAPI patient-history service. It is designed to make prescribed physiotherapy exercises more consistent and measurable without replacing clinical judgement.

Beyond the wearable itself, PhysioPulse is built as an end-to-end, IoT-powered digital physiotherapy and orthopedic rehabilitation platform. It bridges the communication gap between physiotherapists and patients by pairing the lightweight ESP32/MPU6050 kit with a real-time biomechanical analysis engine running in the browser and a dual-role web portal — a **Doctor Portal** for prescription, calibration, and monitoring, and a **Patient Portal** for guided, feedback-driven home sessions.

However, a physiotherapy repetition is not simply a movement from point A to point B. A patient may complete ten repetitions while using an incorrect range of motion, moving outside the intended plane, compensating with another body segment, or performing the movement too quickly.

This creates the central technical problem behind PhysioPulse:

### **How can a wearable sensor distinguish a physiologically meaningful exercise from arbitrary limb movement?**

The MPU6050 provides acceleration and angular velocity, but it does not understand concepts such as a bicep curl or shoulder raise. PhysioPulse therefore creates a pipeline evaluating not only whether the limb moved, but whether it moved through an appropriate range and trajectory for the selected exercise — and a portal layer that turns that evaluation into a supervised, doctor-in-the-loop workflow.

**Key features:**
* 3D motion analysis using MPU6050 data.
* Vector and plane-based movement analysis.
* Exercise-specific movement constraints & patient-specific calibration.
* Repetition detection using state machines.
* Plane-deviation and form analysis.
* Browser-based dashboard over Web Bluetooth — no companion app required.
* Doctor Portal for prescriptions, PIN-protected calibration, and patient analytics.
* Patient Portal with a live angle/sway HUD and one-click "Report Pain" escalation.
* FastAPI + JWT-secured backend for patient history, thresholds, and audit logs.

---

## Demo / Examples

### **Images**
<p align="center">
  <img src="physiopulse-myosa-patient-dashboard.jpeg" width="800"><br/>
  <i>Patient Dashboard</i>
</p>

<p align="center">
  <img src="physiopulse-myosa-patient-exercise.jpeg" width="800"><br/>
  <i>Patient Exercise</i>
</p>

<p align="center">
  <img src="physiopulse-myosa-doctor.png" width="800"><br/>
  <i>Doctor Dashboard</i>
</p>

<p align="center">
  <img src="physiopulse-myosa-hand.jpeg" width="800"><br/>
  <i>Hand Exercise</i>
</p>

<p align="center">
  <img src="physiopulse-myosa-leg.jpeg" width="800"><br/>
  <i>Leg Exercise</i>
</p>


### **Videos**

<video controls width="100%">
  <source src="physiopulse-demo.mp4" type="video/mp4">
</video>

---
## Features (Detailed)

### **1. From Sensor Data to Biomechanics**
The MPU6050 produces three-axis acceleration and angular velocity:
```text
a = [ax, ay, az]
ω = [ωx, ωy, ωz]
```
These measurements exist in the sensor coordinate system. PhysioPulse converts them into quantities that can be interpreted in terms of human movement. The accelerometer also provides a gravity reference:
```text
g = [gx, gy, gz]
ĝ = g / ||g||
```
Normalization allows the system to focus on direction rather than raw magnitude.

### **2. Linear Algebra for Movement Analysis**
**Dot Product**
For normalized vectors:
```text
â · b̂ = cos(θ)
therefore:
θ = acos(â · b̂)
```
This provides the angular relationship between movement directions.

**Cross Product**
Two movement vectors can define a plane:
```text
n = u × v
```
where `n` is perpendicular to the intended movement plane. This is important because rehabilitation correctness depends not only on how far a limb moves, but where it moves. A movement can therefore be evaluated using `movement angle + plane deviation` rather than a single threshold.

### **3. Biology Determines the Algorithm**
Different exercises represent different anatomical movements, so they cannot all use the same correctness rule.

| Exercise | Biological movement | Main measurement |
|---|---|---|
| Bicep Curl | Elbow flexion/extension | Angular range |
| Front Raise | Shoulder elevation | Angle + forward plane |
| Side Raise | Shoulder abduction | Angle + lateral plane |
| Lower Leg Raise | Lower-limb elevation | Trajectory + range |
| Hand Circle | Circular limb motion | Spatial trajectory |
| Wrist Circle | Wrist rotation | Angular velocity + direction |

For example, a shoulder raise reaching the correct height can still be incorrect if it significantly deviates from the intended plane. This is why PhysioPulse combines geometry with exercise-specific biomechanical constraints.

### **4. Personalized Calibration**
Human range of motion differs between patients because of flexibility, mobility, injury, recovery stage, and individual anatomy. Therefore a fixed rule such as `90° = correct` cannot represent every patient.

During calibration, PhysioPulse collects the user's movement distribution:
```text
Θ = {θ1, θ2, θ3, ..., θn}
```
and derives exercise-specific thresholds from the observed movement. A percentile-based target can be represented as:
```text
target = P90(Θ)
```
This changes the question from *"What angle should every person reach?"* to *"What movement range does this patient demonstrate during calibration?"*

### **5. Sensor Placement and Coordinate Systems**
Sensor placement is part of the algorithm. If the wearable rotates relative to the limb, the same biological movement can produce different sensor measurements.
Therefore:
`Mechanical placement → Sensor coordinate system → Calibration → Mathematical interpretation → Movement correctness`
Calibration is consequently both a software and mechanical requirement.

### **6. Exercise State Machine**
Sensor data is continuous, while rehabilitation repetitions are discrete. PhysioPulse converts continuous motion into states such as:
`REST → RAISING → PEAK / HOLD → LOWERING → REP COMPLETE`

A repetition is counted only after the expected sequence occurs. Hysteresis is used between movement thresholds so small sensor fluctuations do not repeatedly trigger false repetitions.

### **7. Software Architecture: From Hardware to Web**
While the ESP32 firmware focuses on lightweight sensor streaming, all biomechanical computation for the monitoring interface is handled by a browser-based software layer. The firmware exposes calibrated raw sensor frames over Bluetooth Low Energy (BLE) GATT notifications at a 50 Hz streaming rate, keeping on-chip logic minimal and shifting geometry, calibration, and rep-counting into **PhysioEngine**, a client-side math engine running directly in the browser.

```text
MYOSA Kit (ESP32 + MPU6050)
        │  BLE GATT @ 50 Hz
        ↓
Web Bluetooth API (Browser)
        ↓
PhysioEngine (Vanilla JS, ES6+)
        ↓
Doctor Portal   ⇄   Patient Portal
        ↓
FastAPI + SQLAlchemy + SQLite
```

This keeps the wearable simple and power-efficient while the vector mathematics from Sections 1–2 (dot products, cross products, plane-deviation checks) run in real time in the browser rather than on the microcontroller.

### **8. PhysioEngine — Client-Side Biomechanical Engine**
PhysioEngine (v6.1) extends the vector mathematics above into a continuously running engine:
* A dynamic 3D complementary filter blends the accelerometer's gravity reference at rest with gyroscope integration during fast motion, reducing drift without a full Kalman filter.
* An analytical Gram-Schmidt, two-step geometric projection recovers the intended movement plane regardless of how the sensor is mechanically mounted — extending the plane-deviation concept from Section 2 into placement-agnostic calibration, so the same kit can be strapped on at any angle.
* An analytical zero-point normalization step maps the patient's resting posture to exactly `0.0°`, removing baseline drift from the angle reported to both patient and doctor.
* The same six exercise-specific finite state machines from Section 6 run inside PhysioEngine with hysteresis thresholding and rate limiting, so a rep is only counted once the full `REST → RAISING → PEAK/HOLD → LOWERING → REP COMPLETE` sequence is observed.

### **9. Doctor Portal**
The Doctor Portal turns the calibration and prescription workflow from Section 4 into a guided clinical process:
* **Authentication & patient management** — doctors log in with a unique doctor code and create patient profiles (name, age, diagnosis); the portal auto-generates a Patient ID and password to hand off to the patient.
* **Guided 2-step + baseline calibration** — the doctor connects to the sensor from their own browser and records a resting posture, a raised posture, and a PIN-protected 15-second baseline motion test. Motion-onset triggering (movement beyond roughly 10°) captures around 150 kinematic samples, from which the 90th-percentile range of motion described in Section 4 is calculated automatically.
* **Prescription & threshold customizer** — minimum angle, target reps, maximum safe angle, and maximum lateral sway can be set per patient, with strict-limit enforcement toggled on or off, plus optional exercise videos and rehab notes.
* **Analytics & audit trail** — session history with local timestamps, day-by-day ROM progression charts, a threshold change log, and a review panel for patient-reported pain events.

### **10. Patient Portal**
The Patient Portal is what the patient uses during a home session:
* Simplified login by Patient ID or email.
* An exercise HUD showing the doctor's active prescription — target reps, target ROM, and an assigned demonstration video.
* One-click Web Bluetooth pairing directly to the sensor, with no companion app to install.
* A live angle gauge (0°–180°) with green/red safety-zone coloring, plus a live lateral-sway indicator (0°–30°), both driven by PhysioEngine in real time.
* Automatic rep counting through the same six exercise state machines — bicep curl, front raise, side raise, lower leg raise, hand circle, wrist circle — with audio feedback for reps, limits, and form warnings.
* A "Report Pain" action that pauses the session and immediately sends the exact angle, rep count, and timestamp to the doctor's dashboard, mirroring the gesture-based pain logging on the wearable itself.
* A personal history view of past sessions — dates, reps completed, duration, and peak angles.

### **11. Backend & Data Layer**
Patient history, prescriptions, and pain events are persisted through a FastAPI service:
* Python 3.10+ with FastAPI as an asynchronous ASGI service, served by Uvicorn.
* SQLAlchemy ORM over SQLite, with tables for users, patient profiles, prescriptions, thresholds, exercise history, and pain logs.
* OAuth2 password-bearer authentication with JWT tokens and bcrypt-salted password hashing.

### **12. Safety Design**
Because a patient is exercising unsupervised at home, the software layer treats safety as a first-class concern rather than an add-on: if a patient exceeds the doctor-prescribed maximum safe angle or target reps, PhysioEngine immediately freezes rep accumulation, plays an audible warning, and shows a safety banner — applying the same hysteresis-based discipline used for rep counting in Section 6 to patient safety instead of just repetition accuracy.

---
## Usage Instructions

1. Connect the MYOSA hardware and sensors.
2. Place the wearable securely on the limb.
3. Power the ESP32.
4. Select the required exercise.
5. Perform the calibration movement and allow the system to establish personalized thresholds.
6. Set the target number of repetitions.
7. Perform the exercise while following the feedback on the OLED.
8. Use the gesture interface to register a pain event if required.
9. Complete the session and review the generated results.

### Doctor & Patient Portal Workflow
1. The doctor (or clinic) requests our company to use the product; we then provide the doctor with their portal credentials.
2. The doctor logs into the Doctor Portal with their doctor code and creates a new patient profile.
3. The doctor pairs the MYOSA kit to their own browser via Web Bluetooth and runs the 2-step calibration followed by the PIN-protected 15-second baseline test.
4. The doctor sets the prescription — target reps, minimum/maximum angle, and lateral sway limit — and hands the generated Patient ID and password to the patient.
5. The patient logs into the Patient Portal, pairs the same kit to their own browser, and follows the assigned exercise HUD.
6. During the session, the patient can log pain via the gesture interface or the "Report Pain" button; either path timestamps the event and flags it on the doctor's dashboard.
7. The doctor reviews session history, ROM progression, and any pain alerts from the Doctor Portal's analytics view.

---

## Tech Stack
* **Hardware:** MYOSA Motherboard (ESP32), MPU6050 (Accel + Gyro), Charging Module (TP4056), 2000mAh 3.7V LiPo Cell
* **Firmware:** Arduino C++, ESP32 Wi-Fi / WebServer, I2C, ESP32 Preferences
* **Mathematical Methods:** 3D Linear Algebra, Vector Normalization, Dot/Cross Product, Plane Geometry, Percentile-Based Calibration, Hysteresis, Finite-State Machines
* **Communication:** Bluetooth Low Energy (BLE) GATT notifications, 50 Hz streaming
* **Client Math Engine:** PhysioEngine v6.1 (Vanilla JavaScript ES6+), Dynamic Complementary Filter, Gram-Schmidt Calibration, Analytical Zero-Point Normalization
* **Frontend:** HTML5, CSS3 (Glassmorphic Dark UI), Vanilla JavaScript (ES6+), Web Bluetooth API, Web Audio API, Chart.js, PWA (Service Worker + Web Manifest)
* **Backend:** Python 3.10+, FastAPI, SQLAlchemy, SQLite, Uvicorn, OAuth2 / JWT, bcrypt

---

## Requirements / Installation

### Firmware
Install the Arduino IDE with ESP32 board support and the required libraries.

Upload the firmware to the ESP32 using the Arduino IDE.

### Backend
```bash
# Python dependencies
pip install fastapi uvicorn sqlalchemy python-jose[cryptography] passlib[bcrypt]
```
```bash
uvicorn main:app --reload
```

### Frontend / Web Portal
No installation is required. Open the Doctor or Patient Portal in a Web Bluetooth-capable browser (Chrome or Edge on desktop, or Chrome on Android). The portal can also be installed as a PWA using the browser's "Add to Home Screen" / "Install" prompt for a native-app-like experience.

---

## File Structure 

```text
PhysioPulse/
├── backend/
│   ├── app/
│   │   ├── routers/
│   │   ├── __init__.py
│   │   ├── auth.py
│   │   ├── config.py
│   │   ├── database.py
│   │   ├── main.py
│   │   ├── models.py
│   │   ├── schemas.py
│   │   └── seed.py
│   ├── tests/
│   │   ├── __init__.py
│   │   └── test_api.py
│   ├── requirements.txt
│   └── run.py
│
├── firmware/
│   └── main.ino
│
├── webapp/
│   ├── css/
│   │   └── style.css
│   ├── js/
│   │   ├── api.js
│   │   ├── app.js
│   │   ├── auth.js
│   │   ├── backend-config.js
│   │   ├── charts.js
│   │   ├── connection.js
│   │   ├── doctor-portal.js
│   │   ├── patient-portal.js
│   │   ├── physio-engine.js
│   │   └── session.js
│   ├── index.html
│   ├── manifest.json
│   └── sw.js
│
├── .gitignore
├── README.md
├── physiopulse-myosa-cover-image.PNG
├── physiopulse-myosa-doctor.png
├── physiopulse-myosa-hand.jpeg
├── physiopulse-myosa-leg.jpeg
├── physiopulse-myosa-patient-dashboard.jpeg
└── physiopulse-myosa-patient-exercise.jpeg
```

---

## Conclusion
PhysioPulse treats physiotherapy as a measurable movement-analysis problem. The MPU6050 observes motion. Linear algebra transforms sensor measurements into vectors, angles, and planes. Biomechanics determines what those quantities mean for each exercise. Calibration adapts the system to the patient. State machines transform continuous motion into repetitions. The embedded system then converts these decisions into immediate patient feedback.

On the software side, PhysioEngine carries this same geometry into the browser, while the Doctor and Patient Portals turn calibrated thresholds into a supervised, evidence-generating home rehabilitation workflow — connecting the orthopaedic doctor to the patient without requiring a daily clinic visit.

Measure the motion. Model the geometry. Respect the biology. Personalize the recovery. Connect the doctor and the patient.
