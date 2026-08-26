/**
 * PhysioPulse – Session Manager
 * Handles recording, backend API persistence, IndexedDB caching, history, and CSV export
 */

const Session = (() => {
  // ─── State ────────────────────────────────────────────────────────────
  let recording = false;
  let currentSession = null;
  let timerInterval = null;
  let startTime = null;
  let sessionBuffer = [];

  // Session stats
  let maxAccelMag = 0;
  let maxGyroMag = 0;
  let tempSum = 0;
  let tempCount = 0;

  // ─── IndexedDB Setup ──────────────────────────────────────────────────
  const DB_NAME    = 'PhysioPulse';
  const DB_VERSION = 1;
  const STORE_NAME = 'sessions';
  let db = null;

  function openDB() {
    return new Promise((resolve, reject) => {
      const req = indexedDB.open(DB_NAME, DB_VERSION);
      req.onupgradeneeded = (e) => {
        const db = e.target.result;
        if (!db.objectStoreNames.contains(STORE_NAME)) {
          const store = db.createObjectStore(STORE_NAME, { keyPath: 'id', autoIncrement: true });
          store.createIndex('date', 'date', { unique: false });
        }
      };
      req.onsuccess = (e) => { db = e.target.result; resolve(db); };
      req.onerror = (e) => reject(e.target.error);
    });
  }

  async function saveSessionToStorage(session) {
    let backendId = null;
    try {
      if (typeof API !== 'undefined') {
        const saved = await API.saveSession({
          patient: session.patient,
          patient_name: session.patient,
          exercise: session.exercise,
          notes: session.notes,
          patient_id: session.patientId || null,
          date: session.date,
          duration_s: session.duration_s,
          interval_ms: session.interval_ms,
          stats: session.stats,
          rows: session.rows,
        });
        if (saved && saved.id) {
          backendId = saved.id;
        }
      }
    } catch (e) {
      console.warn('[Session] Backend save error, saving locally:', e.message);
    }

    return new Promise((resolve, reject) => {
      if (!db) return resolve(backendId || Date.now());
      const tx = db.transaction(STORE_NAME, 'readwrite');
      const store = tx.objectStore(STORE_NAME);
      const toSave = backendId ? { ...session, id: backendId } : session;
      const req = store.put(toSave);
      req.onsuccess = () => resolve(req.result);
      req.onerror = () => resolve(backendId || Date.now());
    });
  }

  async function getAllSessions() {
    try {
      if (typeof API !== 'undefined') {
        const list = await API.getSessions();
        if (Array.isArray(list)) {
          return list.map(s => ({
            id: s.id,
            patient: s.patient_name,
            exercise: s.exercise,
            notes: s.notes,
            date: s.date,
            duration_s: s.duration_s,
            interval_ms: s.interval_ms,
            stats: s.stats || { dataPoints: s.data_points },
            rows: [],
          }));
        }
      }
    } catch (e) {
      console.warn('[Session] Backend getAllSessions error, falling back to local DB:', e.message);
    }

    return new Promise((resolve, reject) => {
      if (!db) return resolve([]);
      const tx = db.transaction(STORE_NAME, 'readonly');
      const store = tx.objectStore(STORE_NAME);
      const req = store.getAll();
      req.onsuccess = () => resolve(req.result ? req.result.reverse() : []);
      req.onerror = () => reject(req.error);
    });
  }

  async function getSessionById(id) {
    try {
      if (typeof API !== 'undefined') {
        const detail = await API.getSessionDetail(id);
        if (detail) {
          return {
            id: detail.id,
            patient: detail.patient_name,
            exercise: detail.exercise,
            notes: detail.notes,
            date: detail.date,
            duration_s: detail.duration_s,
            interval_ms: detail.interval_ms,
            stats: detail.stats,
            rows: detail.rows || [],
          };
        }
      }
    } catch (e) {
      console.warn('[Session] Backend getSessionById error:', e.message);
    }

    return new Promise((resolve, reject) => {
      if (!db) return resolve(null);
      const tx = db.transaction(STORE_NAME, 'readonly');
      const store = tx.objectStore(STORE_NAME);
      const req = store.get(id);
      req.onsuccess = () => resolve(req.result);
      req.onerror = () => reject(req.error);
    });
  }

  async function deleteAllSessions() {
    try {
      if (typeof API !== 'undefined') {
        await API.deleteAllSessions();
      }
    } catch (e) {
      console.warn('[Session] Backend deleteAllSessions error:', e.message);
    }

    return new Promise((resolve, reject) => {
      if (!db) return resolve();
      const tx = db.transaction(STORE_NAME, 'readwrite');
      const store = tx.objectStore(STORE_NAME);
      const req = store.clear();
      req.onsuccess = () => resolve();
      req.onerror = () => reject(req.error);
    });
  }

  async function deleteSession(id) {
    try {
      if (typeof API !== 'undefined') {
        await API.deleteSession(id);
      }
    } catch (e) {
      console.warn('[Session] Backend deleteSession error:', e.message);
    }

    return new Promise((resolve, reject) => {
      if (!db) return resolve();
      const tx = db.transaction(STORE_NAME, 'readwrite');
      const store = tx.objectStore(STORE_NAME);
      const req = store.delete(id);
      req.onsuccess = () => resolve();
      req.onerror = () => reject(req.error);
    });
  }

  // ─── Recording Controls ───────────────────────────────────────────────
  function start() {
    if (recording) return;
    recording = true;
    startTime = Date.now();
    sessionBuffer = [];
    maxAccelMag = 0;
    maxGyroMag = 0;
    tempSum = 0;
    tempCount = 0;

    // Get auth session for userId tagging
    const authSession = typeof Auth !== 'undefined' ? Auth.getSession() : null;
    const userId = authSession?.userId || null;
    const patientId = authSession?.patientId || null;

    // Get patient details from form
    const patientName = document.getElementById('patientNameInput')?.value.trim() || authSession?.name || 'Unknown';
    const exercise    = document.getElementById('exerciseType')?.value || 'Unknown';
    const notes       = document.getElementById('sessionNotes')?.value.trim() || '';

    currentSession = {
      patient: patientName,
      exercise,
      notes,
      userId,
      patientId,
      date: new Date().toISOString(),
      rows: [],
      interval_ms: 50,
    };

    // Update UI
    const recordBtn = document.getElementById('recordBtn');
    const stopBtn = document.getElementById('stopBtn');
    const sessionIconWrap = document.getElementById('sessionIconWrap');
    const sessionStateLabel = document.getElementById('sessionStateLabel');
    const sessionStateDesc = document.getElementById('sessionStateDesc');
    const sessionTimer = document.getElementById('sessionTimer');
    const patientSessionEl = document.getElementById('patientSession');

    if (recordBtn) recordBtn.classList.add('hidden');
    if (stopBtn) stopBtn.classList.remove('hidden');
    if (sessionIconWrap) sessionIconWrap.classList.add('recording');
    if (sessionStateLabel) sessionStateLabel.textContent = 'Recording…';
    if (sessionStateDesc) sessionStateDesc.textContent = `Patient: ${patientName} · ${exercise}`;
    if (sessionTimer) sessionTimer.classList.add('recording');
    if (patientSessionEl) {
      patientSessionEl.textContent = `Recording: ${exercise}`;
      document.getElementById('patientName').textContent = patientName;
    }

    // Update patient avatar
    const avatar = document.querySelector('.patient-avatar');
    if (avatar) avatar.textContent = patientName.charAt(0).toUpperCase() || 'P';

    // Start timer
    timerInterval = setInterval(() => {
      const elapsed = Date.now() - startTime;
      const h = Math.floor(elapsed / 3600000);
      const m = Math.floor((elapsed % 3600000) / 60000);
      const s = Math.floor((elapsed % 60000) / 1000);
      if (sessionTimer) {
        sessionTimer.textContent = `${h.toString().padStart(2,'0')}:${m.toString().padStart(2,'0')}:${s.toString().padStart(2,'0')}`;
      }
    }, 500);

    Charts.clearSessionCharts();
    App.showToast('Recording started!', 'success');
    console.log('[Session] Recording started');
  }

  function stop() {
    if (!recording) return;
    recording = false;
    clearInterval(timerInterval);

    // Finalize session
    currentSession.rows = [...sessionBuffer];
    currentSession.duration_s = Math.round((Date.now() - startTime) / 1000);
    currentSession.stats = {
      maxAccelMag: parseFloat(maxAccelMag.toFixed(4)),
      maxGyroMag:  parseFloat(maxGyroMag.toFixed(4)),
      avgTemp:     tempCount > 0 ? parseFloat((tempSum / tempCount).toFixed(2)) : null,
      dataPoints:  sessionBuffer.length,
    };

    // Reset UI
    const recordBtn = document.getElementById('recordBtn');
    const stopBtn = document.getElementById('stopBtn');
    const sessionIconWrap = document.getElementById('sessionIconWrap');
    const sessionStateLabel = document.getElementById('sessionStateLabel');
    const sessionStateDesc = document.getElementById('sessionStateDesc');
    const sessionTimer = document.getElementById('sessionTimer');
    const patientSession = document.getElementById('patientSession');

    if (recordBtn) recordBtn.classList.remove('hidden');
    if (stopBtn) stopBtn.classList.add('hidden');
    if (sessionIconWrap) sessionIconWrap.classList.remove('recording');
    if (sessionStateLabel) sessionStateLabel.textContent = 'Session Saved';
    if (sessionStateDesc) sessionStateDesc.textContent = `${currentSession.stats.dataPoints} data points recorded`;
    if (sessionTimer) sessionTimer.classList.remove('recording');
    if (patientSession) patientSession.textContent = 'No active session';

    // Save to backend & local DB
    saveSessionToStorage(currentSession)
      .then(id => {
        console.log('[Session] Saved with ID:', id);
        App.showToast(`Session saved! (${currentSession.stats.dataPoints} points)`, 'success');
        renderHistory();
      })
      .catch(e => {
        console.error('[Session] Save error:', e);
        App.showToast('Error saving session', 'error');
      });
  }

  // ─── Add data point to current session ────────────────────────────────
  function addDataPoint(data) {
    if (!recording) return;
    const row = {
      ax: parseFloat(data.ax), ay: parseFloat(data.ay), az: parseFloat(data.az),
      gx: parseFloat(data.gx), gy: parseFloat(data.gy), gz: parseFloat(data.gz),
      tp: parseFloat(data.tp),
      t:  data.t,
    };
    sessionBuffer.push(row);

    // Update stats
    const accelMag = Math.sqrt(row.ax**2 + row.ay**2 + row.az**2);
    const gyroMag  = Math.sqrt(row.gx**2 + row.gy**2 + row.gz**2);
    if (accelMag > maxAccelMag) maxAccelMag = accelMag;
    if (gyroMag > maxGyroMag) maxGyroMag = gyroMag;
    tempSum += row.tp;
    tempCount++;

    // Update session stats UI
    const dp = document.getElementById('sDataPoints');
    const ma = document.getElementById('sMaxAccel');
    const mg = document.getElementById('sMaxGyro');
    const at = document.getElementById('sAvgTemp');
    if (dp) dp.textContent = sessionBuffer.length;
    if (ma) ma.textContent = maxAccelMag.toFixed(3) + ' g';
    if (mg) mg.textContent = maxGyroMag.toFixed(1) + ' °/s';
    if (at) at.textContent = (tempSum / tempCount).toFixed(2) + ' °C';
  }

  // ─── Render History List ──────────────────────────────────────────────
  async function renderHistory() {
    const list = document.getElementById('historyList');
    const empty = document.getElementById('historyEmpty');
    if (!list) return;

    let sessions = [];
    try {
      sessions = await getAllSessions();
    } catch (e) {
      console.error('[Session] DB read error:', e);
    }

    if (sessions.length === 0) {
      list.innerHTML = '';
      if (empty) empty.classList.remove('hidden');
      return;
    }
    if (empty) empty.classList.add('hidden');

    list.innerHTML = sessions.map(s => {
      const date = new Date(s.date);
      const dateStr = date.toLocaleDateString() + ' ' + date.toLocaleTimeString();
      const dur = s.duration_s ? _formatDuration(s.duration_s) : '—';
      const pts = s.stats?.dataPoints ?? (s.rows?.length ?? 0);
      const maxA = s.stats?.maxAccelMag != null ? s.stats.maxAccelMag.toFixed(3) : '—';
      const maxG = s.stats?.maxGyroMag != null ? s.stats.maxGyroMag.toFixed(1) : '—';

      return `
        <div class="history-item" data-id="${s.id}" onclick="Session.openPlayback(${s.id})">
          <div class="history-meta">
            <span class="history-patient">${s.patient || 'Patient'}</span>
            <span class="history-detail">${s.exercise || '—'} · ${dateStr} · Duration: ${dur}</span>
            ${s.notes ? `<span class="history-detail" style="color:rgba(255,255,255,0.35);font-style:italic">${s.notes.substring(0,80)}${s.notes.length>80?'…':''}</span>` : ''}
          </div>
          <div class="history-stats">
            <div class="h-stat">
              <span class="h-stat-label">Points</span>
              <span class="h-stat-val">${pts}</span>
            </div>
            <div class="h-stat">
              <span class="h-stat-label">Max Accel</span>
              <span class="h-stat-val">${maxA} g</span>
            </div>
            <div class="h-stat">
              <span class="h-stat-label">Max Gyro</span>
              <span class="h-stat-val">${maxG} °/s</span>
            </div>
            <div class="h-stat">
              <span class="h-stat-label">Avg Temp</span>
              <span class="h-stat-val">${s.stats?.avgTemp ?? '—'} °C</span>
            </div>
          </div>
          <div class="history-actions">
            <button class="btn-icon" title="Download CSV"
              onclick="event.stopPropagation(); Session.downloadCSV(${s.id})">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/><polyline points="7 10 12 15 17 10"/><line x1="12" y1="15" x2="12" y2="3"/></svg>
            </button>
            <button class="btn-icon" title="Delete"
              onclick="event.stopPropagation(); Session.deleteOne(${s.id})">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="3 6 5 6 21 6"/><path d="M19 6l-1 14H6L5 6"/><path d="M10 11v6M14 11v6"/></svg>
            </button>
          </div>
        </div>
      `;
    }).join('');
  }

  // ─── Playback Modal ───────────────────────────────────────────────────
  async function openPlayback(id) {
    let session;
    try {
      session = await getSessionById(id);
    } catch (e) {
      App.showToast('Error loading session', 'error');
      return;
    }
    if (!session) return;

    const modal = document.getElementById('playbackModal');
    const title = document.getElementById('playbackTitle');
    const meta  = document.getElementById('playbackMeta');

    if (title) title.textContent = `${session.patient} – ${session.exercise}`;
    if (meta) {
      const date = new Date(session.date);
      meta.innerHTML = `
        Date: ${date.toLocaleString()} &nbsp;|&nbsp;
        Duration: ${_formatDuration(session.duration_s)} &nbsp;|&nbsp;
        Points: ${session.rows?.length ?? session.stats?.dataPoints ?? 0} &nbsp;|&nbsp;
        Max Accel: ${session.stats?.maxAccelMag ?? '—'} g &nbsp;|&nbsp;
        
        ${session.notes ? `<br><em>${session.notes}</em>` : ''}
      `;
    }

    modal.classList.remove('hidden');
    Charts.renderPlayback(session);

    // Attach download button
    const dlBtn = document.getElementById('downloadCsvBtn');
    if (dlBtn) {
      dlBtn.onclick = () => _doDownloadCSV(session);
    }
  }

  // ─── CSV Export ───────────────────────────────────────────────────────
  async function downloadCSV(id) {
    let session;
    try {
      session = await getSessionById(id);
    } catch (e) { return; }
    if (session) _doDownloadCSV(session);
  }

  function _doDownloadCSV(session) {
    if (!session.rows || session.rows.length === 0) {
      // If we don't have rows loaded, fetch detail
      getSessionById(session.id).then(full => {
        if (full && full.rows) _exportCsvBlob(full);
      });
      return;
    }
    _exportCsvBlob(session);
  }

  function _exportCsvBlob(session) {
    const header = 'timestamp_ms,accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,gyro_z_dps,temperature_c\n';
    const rows = (session.rows || []).map((r, i) => {
      const t = r.t || (i * (session.interval_ms || 50));
      return `${t},${r.ax},${r.ay},${r.az},${r.gx},${r.gy},${r.gz},${r.tp}`;
    }).join('\n');

    const blob = new Blob([
      `# PhysioPulse Session Export\n`,
      `# Patient: ${session.patient}\n`,
      `# Exercise: ${session.exercise}\n`,
      `# Date: ${session.date}\n`,
      `# Notes: ${session.notes || ''}\n`,
      header,
      rows
    ], { type: 'text/csv' });

    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `PhysioPulse_${(session.patient || 'Patient').replace(/\s/g,'_')}_${new Date(session.date).toISOString().slice(0,10)}.csv`;
    a.click();
    URL.revokeObjectURL(url);
    App.showToast('CSV downloaded!', 'success');
  }

  // ─── Delete one session ───────────────────────────────────────────────
  async function deleteOne(id) {
    if (!confirm('Delete this session?')) return;
    await deleteSession(id);
    App.showToast('Session deleted', 'info');
    renderHistory();
  }

  // ─── Clear All ─────────────────────────────────────────────────────────
  async function clearAll() {
    if (!confirm('Delete all sessions? This cannot be undone.')) return;
    await deleteAllSessions();
    App.showToast('All sessions cleared', 'info');
    renderHistory();
  }

  // ─── Helpers ───────────────────────────────────────────────────────────
  function _formatDuration(s) {
    const h = Math.floor(s / 3600);
    const m = Math.floor((s % 3600) / 60);
    const sec = s % 60;
    if (h > 0) return `${h}h ${m}m ${sec}s`;
    if (m > 0) return `${m}m ${sec}s`;
    return `${sec}s`;
  }

  // ─── Init ──────────────────────────────────────────────────────────────
  async function init() {
    try {
      await openDB();
      console.log('[Session] Ready');
    } catch (e) {
      console.error('[Session] IndexedDB failed:', e);
    }
  }

  function isRecording() { return recording; }

  return {
    init, start, stop, addDataPoint,
    renderHistory, openPlayback, downloadCSV, deleteOne, clearAll,
    isRecording
  };
})();
