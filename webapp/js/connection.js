/**
 * PhysioPulse – Connection Manager
 * Handles WiFi WebSocket and Web Bluetooth BLE connections to the ESP32
 */

const Connection = (() => {
  // ─── State ──────────────────────────────────────────────────────────────
  let ws = null;
  let reconnectTimer = null;
  let reconnectAttempts = 0;
  const MAX_RECONNECT = 10;
  const RECONNECT_DELAY = [1000, 2000, 3000, 5000, 8000]; // exponential backoff

  // BLE state
  let bleDevice = null;
  let bleServer = null;
  let bleService = null;

  // BLE UUIDs (matching firmware cheat sheet)
  const BLE_SERVICE_UUID         = '4fafc201-1fb5-459e-8fcc-c5c9c331914b';
  const BLE_CHAR_TELEMETRY_UUID  = 'beb5483e-36e1-4688-b7f5-ea07361b26a1';
  const BLE_CHAR_COMMAND_UUID    = 'beb5483e-36e1-4688-b7f5-ea07361b26a2';

  let charTelemetry = null;
  let charCommand = null;

  // Connection mode: 'wifi' | 'ble'
  let mode = 'wifi';
  let wsUrl = 'ws://192.168.4.1:81';
  let autoReconnect = true;

  // Status: 'disconnected' | 'connecting' | 'connected'
  let status = 'disconnected';

  // Callbacks
  const handlers = {
    data:       [],
    status:     [],
    error:      [],
  };

  // ─── Public: Register Callbacks ─────────────────────────────────────────
  function on(event, fn) {
    if (handlers[event]) handlers[event].push(fn);
  }
  function emit(event, data) {
    (handlers[event] || []).forEach(fn => fn(data));
  }

  // ─── Status Helpers ──────────────────────────────────────────────────────
  function setStatus(s) {
    status = s;
    emit('status', s);
    _updateStatusUI(s);
  }

  function _updateStatusUI(s) {
    const badge = document.getElementById('connBadge');
    const statusText = document.getElementById('connStatus');
    const connDot = document.getElementById('sidebarConnDot');
    const connLabel = document.getElementById('sidebarConnLabel');
    const liveInd = document.getElementById('liveIndicator');

    if (!badge) return;

    badge.className = 'connection-badge ' + s;
    if (statusText) {
      statusText.textContent = {
        disconnected: 'Disconnected',
        connecting:   'Connecting…',
        connected:    'Connected',
      }[s] || s;
    }
    if (connDot) connDot.className = 'conn-dot ' + s;
    if (connLabel) connLabel.textContent = {
      disconnected: 'Disconnected',
      connecting:   'Connecting…',
      connected:    'Connected',
    }[s] || s;
    if (liveInd) {
      s === 'connected' ? liveInd.classList.remove('hidden') : liveInd.classList.add('hidden');
    }
  }

  // ─── WiFi WebSocket Connection ────────────────────────────────────────────
  function connectWiFi(url) {
    if (url) wsUrl = url;
    if (ws && (ws.readyState === WebSocket.OPEN || ws.readyState === WebSocket.CONNECTING)) {
      ws.close();
    }

    setStatus('connecting');
    console.log('[WS] Connecting to:', wsUrl);

    try {
      ws = new WebSocket(wsUrl);
    } catch (e) {
      console.error('[WS] Failed to create WebSocket:', e);
      setStatus('disconnected');
      emit('error', 'Invalid WebSocket URL');
      return;
    }

    ws.onopen = () => {
      console.log('[WS] Connected');
      setStatus('connected');
      reconnectAttempts = 0;
      clearTimeout(reconnectTimer);
      App.showToast('Connected to PhysioPulse ESP32!', 'success');
      // Fetch device info
      fetchDeviceInfo();
    };

    ws.onmessage = (evt) => {
      try {
        const data = JSON.parse(evt.data);
        if (data.type === 'welcome') {
          console.log('[WS] Welcome:', data);
          return;
        }
        if (data.type === 'calibrated') {
          App.showToast('Gyroscope calibrated!', 'success');
          return;
        }
        if (data.type === 'pong') return;
        // Normal sensor data packet
        emit('data', data);
      } catch (e) {
        // Not JSON, ignore
      }
    };

    ws.onerror = (e) => {
      console.error('[WS] Error:', e);
      emit('error', 'WebSocket error');
    };

    ws.onclose = (e) => {
      console.log('[WS] Closed:', e.code, e.reason);
      setStatus('disconnected');
      if (autoReconnect && reconnectAttempts < MAX_RECONNECT) {
        const delay = RECONNECT_DELAY[Math.min(reconnectAttempts, RECONNECT_DELAY.length - 1)];
        console.log(`[WS] Reconnecting in ${delay}ms (attempt ${reconnectAttempts + 1})`);
        reconnectAttempts++;
        reconnectTimer = setTimeout(() => connectWiFi(), delay);
      }
    };
  }

  async function connectBLE() {
    if (!navigator.bluetooth) {
      App.showToast('Web Bluetooth not supported on this browser/device', 'error');
      return;
    }
    setStatus('connecting');
    try {
      bleDevice = await navigator.bluetooth.requestDevice({
        filters: [
          { namePrefix: 'PhysioTracker' },
          { namePrefix: 'PhysioPulse' },
          { services: [BLE_SERVICE_UUID] }
        ],
        optionalServices: [BLE_SERVICE_UUID]
      });

      bleDevice.addEventListener('gattserverdisconnected', _onBLEDisconnect);

      bleServer = await bleDevice.gatt.connect();
      bleService = await bleServer.getPrimaryService(BLE_SERVICE_UUID);

      // Subscribe to Telemetry Characteristic
      charTelemetry = await bleService.getCharacteristic(BLE_CHAR_TELEMETRY_UUID);
      await charTelemetry.startNotifications();
      charTelemetry.addEventListener('characteristicvaluechanged', (evt) => {
        const decoder = new TextDecoder();
        const json = decoder.decode(evt.target.value);
        try {
          const data = JSON.parse(json);
          // Normalize telemetry data: { r, a, tw, st, msg, ax, ay, az, gx, gy, gz, tp } -> standard app format
          emit('data', {
            pitch: data.a !== undefined ? parseFloat(data.a) : (data.pitch !== undefined ? parseFloat(data.pitch) : 0),
            reps: data.r !== undefined ? parseInt(data.r) : 0,
            timeWindow: data.tw !== undefined ? data.tw : 0,
            stage: data.st !== undefined ? data.st : 0,
            message: data.msg || '',
            ax: data.ax !== undefined ? parseFloat(data.ax) : 0,
            ay: data.ay !== undefined ? parseFloat(data.ay) : 0,
            az: data.az !== undefined ? parseFloat(data.az) : 0,
            gx: data.gx !== undefined ? parseFloat(data.gx) : 0,
            gy: data.gy !== undefined ? parseFloat(data.gy) : 0,
            gz: data.gz !== undefined ? parseFloat(data.gz) : 0,
            tp: data.tp !== undefined ? parseFloat(data.tp) : 36.5,
            raw: data
          });
        } catch (e) {
          console.warn('[BLE JSON parse error]', e, json);
        }
      });

      // Get Command Characteristic
      try {
        charCommand = await bleService.getCharacteristic(BLE_CHAR_COMMAND_UUID);
      } catch (err) {
        console.warn('[BLE] Command characteristic not found:', err);
      }

      setStatus('connected');
      App.showToast(`BLE connected: ${bleDevice.name}`, 'success');
    } catch (e) {
      console.error('[BLE] Error:', e);
      setStatus('disconnected');
      if (e.name !== 'NotFoundError') {
        App.showToast('BLE connection failed: ' + e.message, 'error');
      }
    }
  }

  async function sendCommand(cmd) {
    if (status !== 'connected') {
      console.warn('[Connection] Cannot send command, not connected');
      return;
    }
    if (mode === 'ble' && charCommand) {
      const encoder = new TextEncoder();
      await charCommand.writeValueWithoutResponse(encoder.encode(cmd));
      console.log('[BLE TX CMD]', cmd);
    } else {
      send(cmd);
    }
  }

  function _onBLEDisconnect() {
    console.log('[BLE] Disconnected');
    setStatus('disconnected');
    if (autoReconnect) {
      setTimeout(() => {
        if (bleDevice && bleDevice.gatt) {
          setStatus('connecting');
          bleDevice.gatt.connect()
            .then(s => { bleServer = s; setStatus('connected'); })
            .catch(() => setStatus('disconnected'));
        }
      }, 2000);
    }
  }

  // ─── Send command over WebSocket ──────────────────────────────────────────
  function send(cmd) {
    if (ws && ws.readyState === WebSocket.OPEN) {
      ws.send(typeof cmd === 'string' ? cmd : JSON.stringify(cmd));
    }
  }

  // ─── Calibrate (send command) ─────────────────────────────────────────────
  function calibrate() {
    if (mode === 'wifi' && status === 'connected') {
      send('calibrate');
      App.showToast('Calibration sent… keep sensor still', 'info');
    } else if (mode === 'wifi') {
      // Use REST API
      const httpUrl = wsUrl.replace('ws://', 'http://').replace(':81', '').replace('/ws', '');
      fetch(httpUrl + '/api/calibrate', { method: 'POST' })
        .then(r => r.json())
        .then(() => App.showToast('Gyroscope calibrated!', 'success'))
        .catch(() => App.showToast('Calibration failed (not connected)', 'error'));
    }
  }

  // ─── Fetch Device Info ──────────────────────────────────────────────────
  function fetchDeviceInfo() {
    const httpUrl = wsUrl.replace('ws://', 'http://').replace(':81', '').replace(/\/ws$/, '');
    fetch(httpUrl + '/api/info')
      .then(r => r.json())
      .then(info => {
        const set = (id, val) => { const el = document.getElementById(id); if(el) el.textContent = val || '—'; };
        set('dDevice', info.device);
        set('dFirmware', 'v' + info.version);
        set('dMAC', info.mac);
        set('dIP', info.ap_ip);
        set('dSensor', `MPU-6050 (0x${info.i2c_addr?.toString(16)})`);
        set('dUptime', info.uptime_s + 's');
        set('dHeap', Math.round(info.free_heap / 1024) + ' KB');
      })
      .catch(() => console.log('[HTTP] Could not fetch device info (offline?)'));
  }

  // ─── Disconnect ───────────────────────────────────────────────────────────
  function disconnect() {
    clearTimeout(reconnectTimer);
    autoReconnect = false;
    if (ws) ws.close();
    if (bleDevice && bleDevice.gatt && bleDevice.gatt.connected) bleDevice.gatt.disconnect();
    setStatus('disconnected');
  }

  // ─── Public API ───────────────────────────────────────────────────────────
  function connect(options = {}) {
    if (options.mode) mode = options.mode;
    if (options.url) wsUrl = options.url;
    if (options.autoReconnect !== undefined) autoReconnect = options.autoReconnect;

    if (mode === 'ble') {
      connectBLE();
    } else {
      connectWiFi(wsUrl);
    }
  }

  function getStatus() { return status; }
  function getMode() { return mode; }

  return { on, connect, disconnect, send, sendCommand, calibrate, fetchDeviceInfo, getStatus, getMode };
})();
