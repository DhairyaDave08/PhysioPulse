/**
 * PhysioPulse – Main App Controller
 * Coordinates: routing, data pipeline, sensor UI updates, settings, demo mode
 */

const App = (() => {
  // ─── State ───────────────────────────────────────────────────────────────
  let currentPage = 'dashboard';
  let motionThreshold = 100;  // °/s for alert
  let tempThreshold   = 40;   // °C for alert
  let demoMode = false;
  let demoInterval = null;
  let lastUpdateTime = 0;
  let frameCount = 0;
  let rateInterval = null;
  let rate = 0;

  // Temp stats
  let tempMin = Infinity, tempMax = -Infinity, tempSum = 0, tempCount = 0;

  // Gyro integration for yaw
  let yaw = 0;
  let lastDataTime = null;

  // ─── Toast Notification System ───────────────────────────────────────────
  function showToast(msg, type = 'info') {
    const container = document.getElementById('toastContainer');
    if (!container) return;
    const icons = { success: '✓', error: '✕', info: 'ℹ' };
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    toast.innerHTML = `<span style="font-size:1rem">${icons[type]||'ℹ'}</span> ${msg}`;
    container.appendChild(toast);
    setTimeout(() => {
      toast.style.transition = '0.3s ease';
      toast.style.opacity = '0';
      toast.style.transform = 'translateX(20px)';
      setTimeout(() => toast.remove(), 350);
    }, 3000);
  }

  // ─── Page Routing ─────────────────────────────────────────────────────────
  function navigateTo(page) {
    if (currentPage === page) return;
    currentPage = page;

    // Deactivate all nav items and pages
    document.querySelectorAll('.nav-item').forEach(el => el.classList.remove('active'));
    document.querySelectorAll('.page').forEach(el => el.classList.remove('active'));

    // Activate selected
    const navEl = document.getElementById('nav-' + page);
    const pageEl = document.getElementById('page-' + page);
    if (navEl) navEl.classList.add('active');
    if (pageEl) pageEl.classList.add('active');

    // Update topbar title
    const titles = {
      dashboard: 'Dashboard',
      live:      'Live Charts',
      session:   'Session Recording',
      history:   'Session History',
      settings:  'Settings',
      about:     'About PhysioPulse',
    };
    const topbarTitle = document.getElementById('topbarTitle');
    if (topbarTitle) topbarTitle.textContent = titles[page] || page;

    // Load page-specific data
    if (page === 'history') Session.renderHistory();

    // Close mobile sidebar
    closeSidebar();
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

  // ─── Sensor Data Processing ───────────────────────────────────────────────
  function processSensorData(raw) {
    const now = performance.now();
    const dt = lastDataTime ? (now - lastDataTime) / 1000 : 0.05;
    lastDataTime = now;

    // Parse values
    const ax = parseFloat(raw.ax) || 0;
    const ay = parseFloat(raw.ay) || 0;
    const az = parseFloat(raw.az) || 0;
    const gx = parseFloat(raw.gx) || 0;
    const gy = parseFloat(raw.gy) || 0;
    const gz = parseFloat(raw.gz) || 0;
    const tp = parseFloat(raw.tp) || 0;

    // Compute pitch and roll from accelerometer (atan2 method)
    const pitch = Math.atan2(ax, Math.sqrt(ay*ay + az*az)) * (180/Math.PI);
    const roll  = Math.atan2(ay, Math.sqrt(ax*ax + az*az)) * (180/Math.PI);
    // Integrate gyroZ for yaw
    yaw += gz * dt;
    if (yaw > 180)  yaw -= 360;
    if (yaw < -180) yaw += 360;

    // Enriched data
    const data = { ax, ay, az, gx, gy, gz, tp, _pitch: pitch, _roll: roll, _yaw: yaw };

    // Update UI
    updateDashboard(data);
    Charts.push(data);
    Session.addDataPoint(data);

    // Frame rate tracking
    frameCount++;
    lastUpdateTime = now;

    return data;
  }

  // ─── Dashboard UI Updater ─────────────────────────────────────────────────
  function updateDashboard(d) {
    const { ax, ay, az, gx, gy, gz, tp, _pitch, _roll, _yaw } = d;

    // ── Accelerometer ──
    const accelMag = Math.sqrt(ax**2 + ay**2 + az**2);
    _setEl('accelX', ax.toFixed(3));
    _setEl('accelY', ay.toFixed(3));
    _setEl('accelZ', az.toFixed(3));
    _setEl('accelMag', accelMag.toFixed(3) + ' g');
    // Axis bars: map ±2g → 0–100%
    _setBar('accelXBar', ax, 2);
    _setBar('accelYBar', ay, 2);
    _setBar('accelZBar', az, 2);

    // ── Gyroscope ──
    const gyroMag = Math.sqrt(gx**2 + gy**2 + gz**2);
    _setEl('gyroX', gx.toFixed(3));
    _setEl('gyroY', gy.toFixed(3));
    _setEl('gyroZ', gz.toFixed(3));
    _setEl('gyroMag', gyroMag.toFixed(3) + ' °/s');
    _setBar('gyroXBar', gx, 250);
    _setBar('gyroYBar', gy, 250);
    _setBar('gyroZBar', gz, 250);

    // ── Temperature ──
    _setEl('tempValue', tp.toFixed(1));
    // Stats
    if (tp < tempMin) tempMin = tp;
    if (tp > tempMax) tempMax = tp;
    tempSum += tp; tempCount++;
    _setEl('tempMin', tempMin.toFixed(1));
    _setEl('tempMax', tempMax.toFixed(1));
    _setEl('tempAvg', (tempSum / tempCount).toFixed(2));
    // Gauge
    _updateTempGauge(tp);

    // ── ROM (Range of Motion) ──
    _setEl('pitchVal', Math.round(_pitch) + '°');
    _setEl('rollVal',  Math.round(_roll) + '°');
    _setEl('yawVal',   Math.round(_yaw) + '°');
    _setArcOffset('pitchArc', _pitch, 116);
    _setArcOffset('rollArc',  _roll,  116);
    _setArcOffset('yawArc',   _yaw,   116);

    // Alerts
    const alert = document.getElementById('romAlert');
    const alertText = document.getElementById('romAlertText');
    if (gyroMag > motionThreshold) {
      if (alert) alert.classList.remove('hidden');
      if (alertText) alertText.textContent = `High motion detected! ${gyroMag.toFixed(1)} °/s`;
    } else if (tp > tempThreshold) {
      if (alert) alert.classList.remove('hidden');
      if (alertText) alertText.textContent = `High temperature: ${tp.toFixed(1)} °C`;
    } else {
      if (alert) alert.classList.add('hidden');
    }

    // Timestamp
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
    // Temp range: 20°C – 50°C → 0–100%
    const pct = Math.max(0, Math.min(1, (temp - 20) / 30));
    const maxDash = 220;
    fill.setAttribute('stroke-dashoffset', maxDash - pct * maxDash);
    // Needle rotation: -90° to +90°
    const angle = -90 + pct * 180;
    needle.setAttribute('transform', `rotate(${angle}, 80, 90)`);
  }

  function _setArcOffset(id, angleDeg, maxDash) {
    const el = document.getElementById(id);
    if (!el) return;
    // angle ±180° → 0–maxDash
    const pct = Math.max(0, Math.min(1, (Math.abs(angleDeg) / 180)));
    el.setAttribute('stroke-dashoffset', maxDash - pct * maxDash);
  }

  // ─── Rate Counter ──────────────────────────────────────────────────────────
  function startRateCounter() {
    rateInterval = setInterval(() => {
      rate = frameCount;
      frameCount = 0;
      _setEl('rateBadge', rate + ' Hz');
    }, 1000);
  }

  // ─── Demo Mode (simulate sensor data when not connected) ─────────────────
  function startDemo() {
    demoMode = true;
    showToast('Demo mode active – simulating sensor data', 'info');
    let t = 0;
    demoInterval = setInterval(() => {
      t += 0.05;
      const noise = () => (Math.random() - 0.5) * 0.02;
      const data = {
        ax: Math.sin(t * 0.8) * 0.3 + noise(),
        ay: Math.cos(t * 0.6) * 0.2 + noise(),
        az: 0.95 + Math.sin(t * 1.2) * 0.1 + noise(),
        gx: Math.sin(t * 1.5) * 15 + noise() * 5,
        gy: Math.cos(t * 1.1) * 10 + noise() * 5,
        gz: Math.sin(t * 0.7) * 8  + noise() * 3,
        tp: 36.5 + Math.sin(t * 0.1) * 0.5 + noise() * 0.1,
        t:  Date.now(),
        cal: true,
      };
      processSensorData(data);
    }, 50);
  }

  function stopDemo() {
    demoMode = false;
    clearInterval(demoInterval);
  }

  // ─── Splash Screen ────────────────────────────────────────────────────────
  function runSplash() {
    const fill = document.getElementById('loaderFill');
    const text = document.getElementById('loaderText');
    const steps = [
      [20,  'Initializing…'],
      [40,  'Loading Charts…'],
      [60,  'Opening Database…'],
      [80,  'Connecting UI…'],
      [100, 'Ready!'],
    ];
    let i = 0;
    const advance = () => {
      if (i >= steps.length) {
        // Hide splash, show app
        setTimeout(() => {
          document.getElementById('splash-screen').classList.add('fade-out');
          document.getElementById('app').classList.remove('hidden');
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

  // ─── Settings: Apply ─────────────────────────────────────────────────────
  function applySettings() {
    const wsUrlInput = document.getElementById('wsUrl');
    const autoRec = document.getElementById('autoReconnect');
    const bufSize = document.getElementById('bufferSize');
    const mThresh = document.getElementById('motionThreshold');
    const tThresh = document.getElementById('tempThreshold');

    if (wsUrlInput) {
      const url = wsUrlInput.value.trim();
      if (url) Connection.connect({ url, autoReconnect: autoRec?.checked !== false });
    }
    if (bufSize) Charts.setBufferSize(parseInt(bufSize.value));
    if (mThresh) motionThreshold = parseInt(mThresh.value) || 100;
    if (tThresh) tempThreshold   = parseInt(tThresh.value) || 40;
  }

  // ─── Event Listeners ──────────────────────────────────────────────────────
  function bindEvents() {
    // Navigation
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

    // Session buttons
    document.getElementById('recordBtn')?.addEventListener('click', () => {
      if (Connection.getStatus() !== 'connected' && !demoMode) {
        showToast('Not connected – start demo mode or connect to ESP32', 'error');
        return;
      }
      Session.start();
    });
    document.getElementById('stopBtn')?.addEventListener('click', Session.stop.bind(Session));
    document.getElementById('quickStartBtn')?.addEventListener('click', () => {
      navigateTo('session');
    });

    // History clear
    document.getElementById('clearHistoryBtn')?.addEventListener('click', Session.clearAll.bind(Session));

    // Playback modal close
    document.getElementById('playbackClose')?.addEventListener('click', () => {
      document.getElementById('playbackModal')?.classList.add('hidden');
    });
    document.getElementById('playbackModal')?.addEventListener('click', (e) => {
      if (e.target === e.currentTarget) e.currentTarget.classList.add('hidden');
    });

    // Settings: connection mode toggle
    document.getElementById('connModeToggle')?.addEventListener('click', (e) => {
      const btn = e.target.closest('.toggle-btn');
      if (!btn) return;
      document.querySelectorAll('.toggle-btn').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
    });

    // Settings: connect button
    document.getElementById('connectBtn')?.addEventListener('click', () => {
      const modeEl = document.querySelector('.toggle-btn.active');
      const mode = modeEl?.dataset.mode || 'wifi';
      const url = document.getElementById('wsUrl')?.value.trim();
      const autoRec = document.getElementById('autoReconnect')?.checked;
      stopDemo();
      Connection.connect({ mode, url, autoReconnect: autoRec });
    });

    // Settings: calibrate
    document.getElementById('calibrateBtn')?.addEventListener('click', () => {
      Connection.calibrate();
    });

    // Settings: refresh device info
    document.getElementById('refreshDeviceInfo')?.addEventListener('click', () => {
      Connection.fetchDeviceInfo();
    });

    // Time window
    document.getElementById('timeWindow')?.addEventListener('change', (e) => {
      const seconds = parseInt(e.target.value);
      const size = seconds * 20; // 20 Hz
      Charts.setBufferSize(size);
    });

    // Buffer size
    document.getElementById('bufferSize')?.addEventListener('change', (e) => {
      Charts.setBufferSize(parseInt(e.target.value));
    });

    // Connection badge click → go to settings
    document.getElementById('connBadge')?.addEventListener('click', () => {
      navigateTo('settings');
    });
  }

  // ─── Connection Data Handler ───────────────────────────────────────────────
  function setupConnectionHandlers() {
    Connection.on('data', (raw) => {
      processSensorData(raw);
    });

    Connection.on('status', (status) => {
      if (status === 'connected') {
        stopDemo();
      }
    });

    Connection.on('error', (msg) => {
      showToast('Connection error: ' + msg, 'error');
    });
  }

  // ─── Init ─────────────────────────────────────────────────────────────────
  async function init() {
    // Run splash
    runSplash();

    // Init subsystems
    Charts.init();
    await Session.init();
    bindEvents();
    setupConnectionHandlers();
    startRateCounter();

    // Auto-connect to ESP32 on page load (if on the ESP32's own network)
    // Detect if we're served from ESP32 (192.168.4.x or physiopulse.local)
    const host = window.location.hostname;
    const isESP32Host = host === '192.168.4.1' || host === 'physiopulse.local' || host.startsWith('192.168.4.');

    if (isESP32Host) {
      const wsUrl = `ws://${host}:81`;
      setTimeout(() => Connection.connect({ mode: 'wifi', url: wsUrl, autoReconnect: true }), 800);
      document.getElementById('wsUrl').value = wsUrl;
    } else {
      // Not on ESP32 network – offer demo mode after a short delay
      setTimeout(() => {
        showToast('Not connected to ESP32. Running in demo mode. Go to Settings to connect.', 'info');
        startDemo();
      }, 1500);
    }

    console.log('[App] PhysioPulse initialized ✓');
  }

  return { init, showToast, navigateTo };
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
