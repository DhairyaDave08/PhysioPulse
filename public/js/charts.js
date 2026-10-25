/**
 * PhysioPulse – Charts Manager
 * Real-time Chart.js charts for sensor data visualization
 */

const Charts = (() => {
  // ─── Default Config ───────────────────────────────────────────────────
  let bufferSize = 200;   // Number of data points to keep
  let timeWindowS = 30;   // Time window in seconds

  // ─── Chart instances ──────────────────────────────────────────────────
  const charts = {};

  // ─── Color Palette ─────────────────────────────────────────────────────
  const C = {
    axisX: '#ef4444', axisY: '#22c55e', axisZ: '#3b82f6',
    gyroX: '#f97316', gyroY: '#a855f7', gyroZ: '#06b6d4',
    cyan: '#00d4ff', amber: '#f59e0b', green: '#22c55e',
    purple: '#a855f7',
    gridLine: 'rgba(255,255,255,0.04)',
    tickColor: 'rgba(255,255,255,0.25)',
  };

  // ─── Chart.js Global Defaults ─────────────────────────────────────────
  function applyDefaults() {
    Chart.defaults.color = C.tickColor;
    Chart.defaults.borderColor = C.gridLine;
    Chart.defaults.font.family = "'Inter', sans-serif";
    Chart.defaults.font.size = 11;
    Chart.defaults.plugins.legend.display = false;
    Chart.defaults.animation = false; // disable for real-time perf
  }

  // ─── Common Chart Options ──────────────────────────────────────────────
  function baseLineOptions(yLabel = '', yMin = undefined, yMax = undefined) {
    return {
      responsive: true,
      maintainAspectRatio: false,
      animation: { duration: 0 },
      interaction: { mode: 'index', intersect: false },
      plugins: {
        legend: { display: false },
        tooltip: {
          backgroundColor: 'rgba(14,21,37,0.95)',
          borderColor: 'rgba(0,212,255,0.2)',
          borderWidth: 1,
          titleColor: 'rgba(255,255,255,0.7)',
          bodyColor: 'rgba(255,255,255,0.9)',
          padding: 10,
        }
      },
      scales: {
        x: {
          type: 'category',
          ticks: { color: C.tickColor, maxTicksLimit: 6, maxRotation: 0 },
          grid: { color: C.gridLine, drawBorder: false },
          border: { display: false }
        },
        y: {
          ticks: { color: C.tickColor, maxTicksLimit: 6 },
          grid: { color: C.gridLine, drawBorder: false },
          border: { display: false },
          title: { display: !!yLabel, text: yLabel, color: C.tickColor, font: { size: 10 } },
          ...(yMin !== undefined ? { min: yMin } : {}),
          ...(yMax !== undefined ? { max: yMax } : {}),
        }
      }
    };
  }

  function miniOptions() {
    return {
      responsive: true,
      maintainAspectRatio: false,
      animation: { duration: 0 },
      plugins: { legend: { display: false }, tooltip: { enabled: false } },
      scales: {
        x: { display: false },
        y: { display: false }
      },
      elements: { point: { radius: 0 } }
    };
  }

  // ─── Dataset factory ──────────────────────────────────────────────────
  function ds(label, color, fill = false) {
    return {
      label,
      data: [],
      borderColor: color,
      backgroundColor: fill ? color + '18' : 'transparent',
      borderWidth: 1.5,
      tension: 0.3,
      pointRadius: 0,
      pointHoverRadius: 3,
      fill,
    };
  }

  function miniDs(color) {
    return {
      data: [],
      borderColor: color,
      backgroundColor: color + '15',
      borderWidth: 1.5,
      tension: 0.4,
      pointRadius: 0,
      fill: true,
    };
  }

  // ─── Time Labels Buffer ───────────────────────────────────────────────
  const timeLabels = [];
  function nowLabel() {
    const d = new Date();
    return `${d.getMinutes().toString().padStart(2,'0')}:${d.getSeconds().toString().padStart(2,'0')}.${Math.floor(d.getMilliseconds()/100)}`;
  }
  function pushTime(labels) {
    labels.push(nowLabel());
    if (labels.length > bufferSize) labels.shift();
  }

  // ─── Initialize All Charts ────────────────────────────────────────────
  
  function safeCreateChart(id, config) {
    const el = document.getElementById(id);
    if (!el) return null;
    return new Chart(el, config);
  }

  function init() {
    applyDefaults();

    // ── Mini Charts (Dashboard Cards) ──
    charts.miniAccel = createMiniChart('miniAccelChart', [C.axisX, C.axisY, C.axisZ]);
    charts.miniGyro  = createMiniChart('miniGyroChart', [C.gyroX, C.gyroY, C.gyroZ]);
    charts.miniTemp  = createMiniChart('miniTempChart', [C.amber], true);

    // ── Live Charts ──
    charts.accel = safeCreateChart('accelChart', {
      type: 'line',
      data: {
        labels: [],
        datasets: [
          ds('Accel X', C.axisX),
          ds('Accel Y', C.axisY),
          ds('Accel Z', C.axisZ),
        ]
      },
      options: baseLineOptions('g')
    });

    charts.gyro = safeCreateChart('gyroChart', {
      type: 'line',
      data: {
        labels: [],
        datasets: [
          ds('Gyro X', C.gyroX),
          ds('Gyro Y', C.gyroY),
          ds('Gyro Z', C.gyroZ),
        ]
      },
      options: baseLineOptions('°/s')
    });

    charts.temp = safeCreateChart('tempChart', {
      type: 'line',
      data: {
        labels: [],
        datasets: [ds('Temperature', C.amber, true)]
      },
      options: baseLineOptions('°C')
    });

    charts.rom = safeCreateChart('romChart', {
      type: 'line',
      data: {
        labels: [],
        datasets: [
          ds('Pitch', C.cyan),
          ds('Roll', C.purple),
          ds('Yaw', C.amber),
        ]
      },
      options: { ...baseLineOptions('°'), scales: { ...baseLineOptions().scales, y: { ...baseLineOptions().scales.y, min: -180, max: 180 } } }
    });

    // ── Session Charts ──
    charts.sessionAccel = safeCreateChart('sessionAccelChart', {
      type: 'line',
      data: {
        labels: [],
        datasets: [
          ds('Accel X', C.axisX),
          ds('Accel Y', C.axisY),
          ds('Accel Z', C.axisZ),
        ]
      },
      options: baseLineOptions('g')
    });

    charts.sessionGyro = safeCreateChart('sessionGyroChart', {
      type: 'line',
      data: {
        labels: [],
        datasets: [
          ds('Gyro X', C.gyroX),
          ds('Gyro Y', C.gyroY),
          ds('Gyro Z', C.gyroZ),
        ]
      },
      options: baseLineOptions('°/s')
    });

    console.log('[Charts] All charts initialized');
  }

  function createMiniChart(canvasId, colors, fill = false) {
    const canvas = document.getElementById(canvasId);
    if (!canvas) return null;
    return new Chart(canvas, {
      type: 'line',
      data: {
        labels: [],
        datasets: colors.map((c, i) => ({
          data: [],
          borderColor: c,
          backgroundColor: fill ? c + '18' : 'transparent',
          borderWidth: 1.5,
          tension: 0.4,
          pointRadius: 0,
          fill: i === 0 && fill,
        }))
      },
      options: miniOptions()
    });
  }

  // ─── Push Sensor Data to All Live Charts ────────────────────────────────
  function push(sensorData) {
    const { ax, ay, az, gx, gy, gz, tp } = sensorData;
    const pitch = sensorData._pitch || 0;
    const roll  = sensorData._roll  || 0;
    const yaw   = sensorData._yaw   || 0;
    const label = nowLabel();

    // Helper: push to chart with rolling window
    function addPoint(chart, label, ...values) {
      if (!chart) return;
      const ds = chart.data.datasets;
      const labels = chart.data.labels;
      labels.push(label);
      values.forEach((v, i) => {
        if (ds[i]) ds[i].data.push(typeof v === 'number' ? parseFloat(v.toFixed(4)) : v);
      });
      if (labels.length > bufferSize) {
        labels.shift();
        ds.forEach(d => d.data.shift());
      }
      chart.update('quiet');
    }

    // Mini charts (dashboard)
    addPoint(charts.miniAccel, label, ax, ay, az);
    addPoint(charts.miniGyro, label, gx, gy, gz);
    // removed temp

    // Live charts
    addPoint(charts.accel, label, ax, ay, az);
    addPoint(charts.gyro, label, gx, gy, gz);
    // removed temp
    addPoint(charts.rom, label, pitch, roll, yaw);

    // Session charts (only if recording)
    if (Session.isRecording()) {
      addPoint(charts.sessionAccel, label, ax, ay, az);
      addPoint(charts.sessionGyro, label, gx, gy, gz);
    }
  }

  // ─── Playback Charts (create from session data) ──────────────────────────
  function renderPlayback(sessionData) {
    // Destroy existing playback charts
    ['playbackAccelChart', 'playbackGyroChart', 'playbackTempChart'].forEach(id => {
      const existing = Chart.getChart(id);
      if (existing) existing.destroy();
    });

    const rows = sessionData.rows;
    const labels = rows.map((r, i) => (i * (sessionData.interval_ms / 1000)).toFixed(1) + 's');

    safeCreateChart('playbackAccelChart', {
      type: 'line',
      data: {
        labels,
        datasets: [
          { ...ds('X', C.axisX), data: rows.map(r => r.ax) },
          { ...ds('Y', C.axisY), data: rows.map(r => r.ay) },
          { ...ds('Z', C.axisZ), data: rows.map(r => r.az) },
        ]
      },
      options: baseLineOptions('g')
    });

    safeCreateChart('playbackGyroChart', {
      type: 'line',
      data: {
        labels,
        datasets: [
          { ...ds('X', C.gyroX), data: rows.map(r => r.gx) },
          { ...ds('Y', C.gyroY), data: rows.map(r => r.gy) },
          { ...ds('Z', C.gyroZ), data: rows.map(r => r.gz) },
        ]
      },
      options: baseLineOptions('°/s')
    });

    safeCreateChart('playbackTempChart', {
      type: 'line',
      data: {
        labels,
        datasets: [{ ...ds('Temp', C.amber, true), data: rows.map(r => r.tp) }]
      },
      options: baseLineOptions('°C')
    });
  }

  // ─── Clear All Charts ─────────────────────────────────────────────────────
  function clearAll() {
    Object.values(charts).forEach(chart => {
      if (!chart) return;
      chart.data.labels = [];
      chart.data.datasets.forEach(d => d.data = []);
      chart.update('quiet');
    });
  }

  // ─── Clear Session Charts ─────────────────────────────────────────────────
  function clearSessionCharts() {
    ['sessionAccel', 'sessionGyro'].forEach(key => {
      const chart = charts[key];
      if (!chart) return;
      chart.data.labels = [];
      chart.data.datasets.forEach(d => d.data = []);
      chart.update('quiet');
    });
  }

  // ─── Set Buffer Size ──────────────────────────────────────────────────────
  function setBufferSize(n) { bufferSize = parseInt(n) || 200; }

  return { init, push, renderPlayback, clearAll, clearSessionCharts, setBufferSize };
})();
