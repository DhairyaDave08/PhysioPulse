/**
 * PhysioPulse – Doctor Portal Module
 * Manages:
 * - Patient list & profiles
 * - Exercise threshold management + YouTube demo video link
 * - Real-time & persisted Patient Pain & Limitation Alerts
 * - Historical compliance & exercise sessions
 */

const DoctorPortal = (() => {
  let session = null; // current doctor session
  let currentActivePatientId = null; // active patient ID for threshold & profile forms

  function updateDoctorLiveSensor(data) {
    const liveAngleEl = document.getElementById('dpLiveAngle');
    const twistErrorEl = document.getElementById('dpTwistError');
    if (data && data.engine) {
      if (liveAngleEl) liveAngleEl.textContent = data.engine.liveAngle.toFixed(1) + '°';
      if (twistErrorEl) twistErrorEl.textContent = data.engine.twistError.toFixed(1) + '°';
    } else if (data) {
      if (liveAngleEl) liveAngleEl.textContent = (data.pitch || 0).toFixed(1) + '°';
      if (twistErrorEl) twistErrorEl.textContent = '0.0°';
    }
  }

  // ─── Init ─────────────────────────────────────────────────────────────────
  async function init(doctorSession) {
    session = doctorSession;
    await renderDoctorDashboard();
    bindDoctorEvents();
  }


  // ─── Doctor Dashboard ─────────────────────────────────────────────────────
  async function renderDoctorDashboard() {
    // Update doctor info in UI
    const nameEl = document.getElementById('doctorName');
    const codeEl = document.getElementById('doctorCode');
    if (nameEl) nameEl.textContent = session.name;
    if (codeEl) codeEl.textContent = session.doctorCode || 'DR0001';

    // Count patients
    const patients = await Auth.getPatientsByDoctorId(session.userId);
    const countEl = document.getElementById('doctorPatientCount');
    if (countEl) countEl.textContent = patients.length;

    // Render Pain & Limitation Alerts
    await renderPainAlerts();

    // Render General Activity Alerts
    await renderRecentAlerts(patients);

    // Patient list in doctor dashboard
    await renderPatientCards(patients);
  }

  // ─── Pain Alerts ──────────────────────────────────────────────────────────
  async function renderPainAlerts() {
    const listEl = document.getElementById('doctorPainAlertsList');
    const badgeEl = document.getElementById('doctorPainBadge');
    if (!listEl) return;

    let alerts = [];
    try {
      if (typeof API !== 'undefined') {
        alerts = await API.getDoctorPainAlerts();
      }
    } catch (e) {
      console.warn('[DoctorPortal] Could not load pain alerts from API:', e.message);
    }

    const newAlerts = alerts.filter(a => a.status === 'new');
    if (badgeEl) {
      if (newAlerts.length > 0) {
        badgeEl.textContent = newAlerts.length;
        badgeEl.classList.remove('hidden');
      } else {
        badgeEl.classList.add('hidden');
      }
    }

    if (alerts.length === 0) {
      listEl.innerHTML = `<div class="empty-mini">No pain alerts reported – all patients comfortable ✓</div>`;
      return;
    }

    listEl.innerHTML = alerts.slice(0, 8).map(a => `
      <div class="pain-alert-card ${a.status === 'reviewed' ? 'reviewed' : ''}" data-id="${a.id}">
        <div class="pain-alert-info">
          <div class="pain-alert-header">
            <span class="pain-alert-patient">🚨 ${a.patient_name || 'Patient'}</span>
            <span class="pain-severity-pill ${a.pain_level || 'Moderate'}">${a.pain_level || 'Moderate'}</span>
            <span style="font-size:0.75rem;color:rgba(255,255,255,0.4)">${typeof API !== 'undefined' && API.formatDateLocal ? API.formatDateLocal(a.date) : new Date(a.date).toLocaleString()}</span>
          </div>
          <div class="pain-alert-meta">
            Felt at <strong>${a.angle_at_pain != null ? a.angle_at_pain.toFixed(1) + '°' : '—'}</strong> joint angle (Rep <strong>${a.reps_at_pain || 0}</strong>)
          </div>
          ${a.notes ? `<div class="pain-alert-notes">"${a.notes}"</div>` : ''}
        </div>
        <div>
          ${a.status === 'new' ? `
            <button type="button" class="btn-secondary btn-sm" onclick="DoctorPortal.markAlertReviewed(${a.id})" style="font-size:0.75rem;padding:0.4rem 0.75rem;white-space:nowrap">
              ✓ Mark Reviewed
            </button>
          ` : `<span style="font-size:0.75rem;color:#22c55e">✓ Reviewed</span>`}
        </div>
      </div>
    `).join('');
  }

  async function markAlertReviewed(alertId) {
    try {
      if (typeof API !== 'undefined') {
        await API.resolvePainAlert(alertId, 'reviewed');
      }
      App.showToast('Pain alert marked as reviewed', 'success');
      await renderPainAlerts();
    } catch (e) {
      App.showToast('Error updating alert', 'error');
    }
  }

  // ─── Recent Activity Alerts ────────────────────────────────────────────────
  async function renderRecentAlerts(patients) {
    const alertsEl = document.getElementById('doctorAlertsList');
    if (!alertsEl) return;

    let alerts = [];
    for (const p of patients) {
      const history = await Auth.getExerciseHistoryForPatient(p.id);
      const failedSessions = history.filter(h => h.status === 'failed' || h.status === 'partial');
      failedSessions.slice(0, 3).forEach(h => {
        alerts.push({ patient: p.name, date: h.date, status: h.status, reps: h.repsCompleted, target: h.targetReps });
      });
    }

    alerts.sort((a, b) => new Date(b.date) - new Date(a.date));
    const recent = alerts.slice(0, 5);

    if (recent.length === 0) {
      alertsEl.innerHTML = `<div class="empty-mini">No activity alerts – all patients on track ✓</div>`;
      return;
    }

    alertsEl.innerHTML = recent.map(a => `
      <div class="alert-item ${a.status}">
        <div class="alert-patient-name">${a.patient}</div>
        <div class="alert-detail">${new Date(a.date).toLocaleDateString()} – ${a.reps || 0}/${a.target || '?'} reps (${a.status})</div>
      </div>
    `).join('');
  }

  // ─── Patient Cards ────────────────────────────────────────────────────────
  async function renderPatientCards(patients) {
    const listEl = document.getElementById('doctorPatientCards');
    if (!listEl) return;

    if (patients.length === 0) {
      listEl.innerHTML = `
        <div class="empty-state" style="grid-column:1/-1">
          <div class="empty-icon">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87M16 3.13a4 4 0 0 1 0 7.75"/></svg>
          </div>
          <h2>No patients yet</h2>
          <p>Share your Doctor Code <strong>${session.doctorCode || 'DR????'}</strong> with patients for them to register and link to you.</p>
        </div>`;
      return;
    }

    const cards = await Promise.all(patients.map(async (p) => {
      const threshold = await Auth.getThresholdForPatient(p.id);
      const history   = await Auth.getExerciseHistoryForPatient(p.id);
      const lastSession = history[0];
      const compliance = history.length > 0
        ? Math.round(history.filter(h => h.status === 'completed').length / history.length * 100)
        : null;

      return `
        <div class="patient-card" data-patient-id="${p.id}" onclick="DoctorPortal.openPatientDetail(${p.id})">
          <div class="pc-header">
            <div class="pc-avatar">${(p.name || 'P').charAt(0).toUpperCase()}</div>
            <div class="pc-info">
              <div class="pc-name">${p.name}</div>
              <div class="pc-meta">${p.condition || 'No condition set'} · Age: ${p.age || '—'}</div>
            </div>
            <div class="pc-compliance ${compliance === null ? '' : compliance >= 80 ? 'good' : compliance >= 50 ? 'warn' : 'bad'}">
              ${compliance === null ? '—' : compliance + '%'}
            </div>
          </div>
          <div class="pc-thresholds">
            ${threshold ? `
              <span class="pc-thresh-item">Min: <strong>${threshold.minAngle}°</strong></span>
              <span class="pc-thresh-item">Max: <strong>${threshold.maxAngle}°</strong></span>
              <span class="pc-thresh-item">Reps: <strong>${threshold.targetReps}</strong></span>
              ${threshold.video_url || threshold.videoUrl ? '<span class="pc-thresh-item" style="color:#00d4ff">📹 Demo Video</span>' : ''}
            ` : `<span class="pc-thresh-none">No thresholds set yet</span>`}
          </div>
          <div class="pc-footer">
            <span class="pc-last">${lastSession ? 'Last: ' + new Date(lastSession.date).toLocaleDateString() : 'No sessions yet'}</span>
            <span class="pc-sessions">${history.length} sessions</span>
          </div>
        </div>
      `;
    }));

    listEl.innerHTML = cards.join('');
  }

  // ─── Patient Detail / Threshold Form ─────────────────────────────────────
  async function openPatientDetail(patientId) {
    currentActivePatientId = patientId;
    const patient   = await Auth.getPatientById(patientId);

    const threshold = await Auth.getThresholdForPatient(patientId);
    const history   = await Auth.getExerciseHistoryForPatient(patientId);
    const thresholdHistory = await Auth.getThresholdHistoryForPatient(patientId);

    // Navigate to patient detail page
    App.navigateTo('doctor-patient');

    // Fill in patient info
    _setEl('dpPatientName', patient.name);
    _setEl('dpPatientAge', patient.age || '—');
    _setEl('dpPatientCondition', patient.condition || 'Not specified');
    _setEl('dpSessionCount', history.length);

    const compliance = history.length > 0
      ? Math.round(history.filter(h => h.status === 'completed').length / history.length * 100)
      : null;
    _setEl('dpCompliance', compliance !== null ? compliance + '%' : '—');

    // Fill threshold form
    if (threshold) {
      _setVal('dpMinAngle', threshold.minAngle);
      _setVal('dpMaxAngle', threshold.maxAngle);
      _setVal('dpTargetReps', threshold.targetReps);
      _setVal('dpMotionLimit', threshold.motionLimit || 100);
      
      _setVal('dpExerciseType', threshold.exerciseType || 'Knee Flexion / Extension');
      _setVal('dpVideoUrl', threshold.video_url || threshold.videoUrl || '');
      _setVal('dpNotes', threshold.notes || '');
      const strictEl = document.getElementById('dpStrictLimit');
      if (strictEl) strictEl.checked = threshold.strict_limit !== false;
    } else {
      _setVal('dpMinAngle', 30);
      _setVal('dpMaxAngle', 120);
      _setVal('dpTargetReps', 10);
      _setVal('dpMotionLimit', 100);
      
      _setVal('dpVideoUrl', '');
      _setVal('dpNotes', '');
    }

    // Store patient ID in form for save action
    const form = document.getElementById('thresholdForm');
    if (form) form.dataset.patientId = patientId;

    // Patient profile edit fields
    _setVal('dpEditName', patient.name);
    _setVal('dpEditAge', patient.age || '');
    _setVal('dpEditCondition', patient.condition || '');
    const profileForm = document.getElementById('patientProfileForm');
    if (profileForm) profileForm.dataset.patientId = patientId;

    // Exercise history table
    renderPatientHistory(history, threshold);

    // Threshold history
    renderThresholdHistory(thresholdHistory);
  }

  function renderPatientHistory(history, threshold) {
    const histEl = document.getElementById('dpHistoryTable');
    if (!histEl) return;

    if (history.length === 0) {
      histEl.innerHTML = `<div class="empty-mini">No exercise sessions recorded yet</div>`;
      return;
    }

    histEl.innerHTML = `
      <table class="data-table">
        <thead>
          <tr>
            <th>Date</th>
            <th>Reps</th>
            <th>Max Angle</th>
            <th>Min Angle</th>
            
            <th>Status</th>
            <th>Duration</th>
          </tr>
        </thead>
        <tbody>
          ${history.map(h => `
            <tr>
              <td>${typeof API !== 'undefined' && API.formatDateLocal ? API.formatDateLocal(h.date) : new Date(h.date).toLocaleString()}</td>
              <td>${h.repsCompleted || 0}${threshold ? '/' + threshold.targetReps : ''}</td>
              <td>${h.maxAngleReached != null ? h.maxAngleReached.toFixed(1) + '°' : '—'}</td>
              <td>${h.minAngleReached != null ? h.minAngleReached.toFixed(1) + '°' : '—'}</td>
              
              <td><span class="status-badge ${h.status}">${h.status}</span></td>
              <td>${h.duration_s ? _formatDuration(h.duration_s) : '—'}</td>
            </tr>
          `).join('')}
        </tbody>
      </table>
    `;
  }

  function renderThresholdHistory(history) {
    const el = document.getElementById('dpThresholdHistory');
    if (!el) return;

    if (history.length === 0) {
      el.innerHTML = `<div class="empty-mini">No threshold changes recorded</div>`;
      return;
    }

    el.innerHTML = history.map((t, i) => `
      <div class="thresh-history-item ${i === 0 ? 'current' : ''}">
        <div class="thi-date">${typeof API !== 'undefined' && API.formatDateLocal ? API.formatDateLocal(t.updatedAt || t.created_at) : new Date(t.updatedAt || t.created_at).toLocaleString()} ${i === 0 ? '<span class="current-badge">Current</span>' : ''}</div>
        <div class="thi-values">
          Min: ${t.minAngle || t.min_angle}° · Max: ${t.maxAngle || t.max_angle}° · Reps: ${t.targetReps || t.target_reps} · Motion: ${t.motionLimit || t.motion_limit || 100}°/s
        </div>
        ${t.video_url || t.videoUrl ? `<div class="thi-notes" style="color:#00d4ff">📹 ${t.video_url || t.videoUrl}</div>` : ''}
        ${t.notes ? `<div class="thi-notes">${t.notes}</div>` : ''}
      </div>
    `).join('');
  }

  // ─── Save Threshold ────────────────────────────────────────────────────────
  async function saveThreshold() {
    const form = document.getElementById('thresholdForm');
    const patientId = currentActivePatientId || (form && form.dataset.patientId ? parseInt(form.dataset.patientId) : null);

    if (!patientId || isNaN(patientId)) {
      App.showToast('Please select a patient first.', 'error');
      return;
    }

    const minAngle  = parseFloat(document.getElementById('dpMinAngle')?.value) || 30;
    const maxAngle  = parseFloat(document.getElementById('dpMaxAngle')?.value) || 120;
    const targetReps= parseInt(document.getElementById('dpTargetReps')?.value)  || 10;
    const motionLimit= parseFloat(document.getElementById('dpMotionLimit')?.value) || 100;
    const tempLimit = 40;
    const exerciseType = document.getElementById('dpExerciseType')?.value || 'Bicep Curl';
    const videoUrl   = document.getElementById('dpVideoUrl')?.value.trim() || '';
    const strictLimit= document.getElementById('dpStrictLimit')?.checked ?? true;
    const notes      = document.getElementById('dpNotes')?.value.trim() || '';

    if (minAngle >= maxAngle) {
      App.showToast('Min angle must be strictly less than max angle', 'error');
      return;
    }

    try {
      await Auth.saveThreshold({
        patientId,
        doctorId: session.userId,
        minAngle,
        maxAngle,
        targetReps,
        motionLimit,
        tempLimit,
        exerciseType,
        video_url: videoUrl,
        videoUrl,
        strict_limit: strictLimit,
        notes,
      });

      // Mark patient as having baseline data
      const patient = await Auth.getPatientById(patientId);
      if (patient) {
        patient.hasBaselineData = true;
        patient.condition = patient.condition || exerciseType;
        await Auth.updatePatientProfile(patient);
      }

      App.showToast('✓ Thresholds & Prescription saved successfully!', 'success');

      // Refresh threshold history
      const threshHistory = await Auth.getThresholdHistoryForPatient(patientId);
      renderThresholdHistory(threshHistory);

      // Refresh patient cards
      const patients = await Auth.getPatientsByDoctorId(session.userId);
      await renderPatientCards(patients);

    } catch (e) {
      console.error('[DoctorPortal] Threshold save error:', e);
      App.showToast('Error saving thresholds: ' + e.message, 'error');
    }
  }

  // ─── Save Prescription (YouTube Video Section) ────────────────────────────
  async function savePrescription() {
    const form = document.getElementById('thresholdForm');
    const patientId = currentActivePatientId || (form && form.dataset.patientId ? parseInt(form.dataset.patientId) : null);

    if (!patientId || isNaN(patientId)) {
      App.showToast('Please select a patient first.', 'error');
      return;
    }

    const videoUrl = document.getElementById('dpVideoUrl')?.value.trim() || '';
    const videoTitle = document.getElementById('dpVideoTitle')?.value.trim() || '';
    const videoNotes = document.getElementById('dpVideoNotes')?.value.trim() || '';

    try {
      const existing = await Auth.getThresholdForPatient(patientId);
      if (existing) {
        existing.video_url = videoUrl;
        existing.videoUrl = videoUrl;
        if (videoNotes) existing.notes = videoNotes;
        await Auth.saveThreshold(existing);
      } else {
        await Auth.saveThreshold({
          patientId,
          doctorId: session.userId,
          minAngle: parseFloat(document.getElementById('dpMinAngle')?.value) || 30,
          maxAngle: parseFloat(document.getElementById('dpMaxAngle')?.value) || 110,
          targetReps: parseInt(document.getElementById('dpTargetReps')?.value) || 12,
          exerciseType: document.getElementById('dpExerciseType')?.value || 'Bicep Curl',
          video_url: videoUrl,
          videoUrl,
          notes: videoNotes
        });
      }

      App.showToast('✓ Exercise video prescription updated!', 'success');
      const threshHistory = await Auth.getThresholdHistoryForPatient(patientId);
      renderThresholdHistory(threshHistory);
    } catch (e) {
      console.error('[DoctorPortal] Prescription save error:', e);
      App.showToast('Error saving prescription: ' + e.message, 'error');
    }
  }

  // ─── Save Patient Profile ─────────────────────────────────────────────────
  async function savePatientProfile() {
    const form = document.getElementById('patientProfileForm');
    const patientId = currentActivePatientId || (form && form.dataset.patientId ? parseInt(form.dataset.patientId) : null);

    const name      = document.getElementById('dpEditName')?.value.trim();
    const age       = parseInt(document.getElementById('dpEditAge')?.value) || null;
    const condition = document.getElementById('dpEditCondition')?.value.trim() || '';

    if (!name) { App.showToast('Patient name is required', 'error'); return; }

    try {
      const patient = await Auth.getPatientById(patientId);
      if (patient) {
        patient.name = name;
        patient.age = age;
        patient.condition = condition;
        await Auth.updatePatientProfile(patient);
      }

      _setEl('dpPatientName', name);
      _setEl('dpPatientAge', age || '—');
      _setEl('dpPatientCondition', condition || 'Not specified');

      App.showToast('Patient profile updated!', 'success');
    } catch (e) {
      App.showToast('Error updating profile: ' + e.message, 'error');
    }
  }

  // ─── Bind Events ──────────────────────────────────────────────────────────
  function bindDoctorEvents() {
    document.getElementById('thresholdSaveBtn')?.addEventListener('click', saveThreshold);
    document.getElementById('prescriptionSaveBtn')?.addEventListener('click', savePrescription);
    document.getElementById('patientProfileSaveBtn')?.addEventListener('click', savePatientProfile);

    // ─── 3D Calibration Step 1: Set Resting ───
    document.getElementById('dpCalStep1Btn')?.addEventListener('click', () => {
      const statusEl = document.getElementById('dpCalibStatus');
      const exType = document.getElementById('dpExerciseType')?.value || 'Bicep Curl';
      PhysioEngine.setPrescription({ exerciseType: exType });

      PhysioEngine.startStep1({
        onProgress: (count, total, status) => {
          if (statusEl) statusEl.textContent = `Capturing Rest (${count}/${total})...`;
        },
        onComplete: () => {
          if (statusEl) statusEl.textContent = "Step 1 OK. Move arm UP slightly for Step 2";
          App.showToast("✅ Step 1 (Rest) captured! Now move limb up slightly and click Step 2.", "success");
        },
        onError: (err) => {
          if (statusEl) statusEl.textContent = err;
          App.showToast("⚠️ " + err, "error");
        }
      });
    });

    // ─── 3D Calibration Step 2: Set Raised (Direction) ───
    document.getElementById('dpCalStep2Btn')?.addEventListener('click', () => {
      const statusEl = document.getElementById('dpCalibStatus');
      PhysioEngine.startStep2({
        onProgress: (count, total, status) => {
          if (statusEl) statusEl.textContent = `Capturing Direction (${count}/${total})...`;
        },
        onComplete: () => {
          if (statusEl) statusEl.textContent = "Calibrated & Ready (0.0° Locked)";
          App.showToast("✅ Coordinate frame locked! Rest is calibrated to 0.0°.", "success");
        },
        onError: (err) => {
          if (statusEl) statusEl.textContent = err;
          App.showToast("⚠️ " + err, "error");
        }
      });
    });

    // ─── 15s Doctor Baseline Recording ───
    document.getElementById('dpRecordBaselineBtn')?.addEventListener('click', () => {
      const pin = prompt("Enter Doctor PIN to record 15-second baseline:");
      if (!pin) return;

      const statusEl = document.getElementById('dpCalibStatus');
      const exType = document.getElementById('dpExerciseType')?.value || 'Bicep Curl';
      PhysioEngine.setPrescription({ exerciseType: exType });

      const ok = PhysioEngine.startDoctorBaseline(pin, {
        onStatus: (msg, count, total) => {
          if (statusEl) statusEl.textContent = msg;
        },
        onComplete: (res) => {
          if (statusEl) statusEl.textContent = `Baseline Saved! Target: ${res.targetMaxAngle}°, Sway: ${res.maxDeviationPlane}°`;
          App.showToast(`🎉 15s Baseline captured! Max ROM: ${res.targetMaxAngle}°, Max Sway: ${res.maxDeviationPlane}°`, "success");

          // Auto-fill form fields with calculated 90th percentile baseline metrics
          _setVal('dpMinAngle', 10);
          _setVal('dpMaxAngle', res.targetMaxAngle);
          _setVal('dpMotionLimit', res.maxDeviationPlane);
        },
        onError: (err) => {
          if (statusEl) statusEl.textContent = err;
          App.showToast("⚠️ " + err, "error");
        }
      });

      if (!ok && pin !== "1234") {
        App.showToast("Incorrect Doctor PIN", "error");
      }
    });

    
    // Add Patient Modal Openers & Handlers
    function openAddPatientModal() {
      const modal = document.getElementById('addPatientModal');
      const err = document.getElementById('addPatientError');
      if (modal) modal.classList.remove('hidden');
      if (err) err.classList.add('hidden');
      const nameInput = document.getElementById('addPtName');
      if (nameInput) {
        nameInput.value = '';
        setTimeout(() => nameInput.focus(), 100);
      }
      const ageInput = document.getElementById('addPtAge');
      if (ageInput) ageInput.value = '';
      const condInput = document.getElementById('addPtCondition');
      if (condInput) condInput.value = '';
      const pwdInput = document.getElementById('addPtPassword');
      if (pwdInput) pwdInput.value = 'patient123';
    }

    function closeAddPatientModal() {
      const modal = document.getElementById('addPatientModal');
      if (modal) modal.classList.add('hidden');
    }

    document.getElementById('doctorAddPatientBtn')?.addEventListener('click', openAddPatientModal);
    document.getElementById('doctorAddPatientBtn2')?.addEventListener('click', openAddPatientModal);
    document.getElementById('closeAddPatientModal')?.addEventListener('click', closeAddPatientModal);
    document.getElementById('cancelAddPatientModal')?.addEventListener('click', closeAddPatientModal);

    document.getElementById('addPatientForm')?.addEventListener('submit', async (e) => {
      e.preventDefault();
      const errorEl = document.getElementById('addPatientError');
      const submitBtn = document.getElementById('submitAddPatientBtn');
      if (errorEl) errorEl.classList.add('hidden');
      if (submitBtn) {
        submitBtn.disabled = true;
        submitBtn.textContent = 'Creating...';
      }

      try {
        const nameVal = document.getElementById('addPtName')?.value?.trim();
        const ageVal = parseInt(document.getElementById('addPtAge')?.value) || null;
        const condVal = document.getElementById('addPtCondition')?.value?.trim() || '';
        const pwdVal = document.getElementById('addPtPassword')?.value || 'patient123';

        if (!nameVal) throw new Error('Patient name is required');
        if (!pwdVal || pwdVal.length < 6) throw new Error('Password must be at least 6 characters');

        const res = await Auth.createPatient({
          name: nameVal,
          age: ageVal,
          condition: condVal,
          password: pwdVal
        });

        closeAddPatientModal();
        const ptCode = res?.patient_code || res?.user?.patient_code || 'PT-XXXX';
        App.showToast(`Patient created! Login ID: ${ptCode}`, 'success');

        // Show credentials popup modal
        const credModal = document.getElementById('patientCredentialsModal');
        if (credModal) {
          const nameEl = document.getElementById('ptCredName');
          const codeEl = document.getElementById('ptCredCode');
          const pwdEl = document.getElementById('ptCredPassword');
          if (nameEl) nameEl.textContent = nameVal;
          if (codeEl) codeEl.textContent = ptCode;
          if (pwdEl) pwdEl.textContent = pwdVal;
          credModal.classList.remove('hidden');
        }
        
        // Refresh dashboard & patient list
        await renderDoctorDashboard();
      } catch (err) {
        if (errorEl) {
          errorEl.textContent = err.message || 'Failed to create patient';
          errorEl.classList.remove('hidden');
        } else {
          App.showToast('Error: ' + err.message, 'error');
        }
      } finally {
        if (submitBtn) {
          submitBtn.disabled = false;
          submitBtn.textContent = '➕ Create Patient & Generate ID';
        }
      }
    });

    // Patient Credentials Modal Listeners
    function closePtCredModal() {
      const modal = document.getElementById('patientCredentialsModal');
      if (modal) modal.classList.add('hidden');
    }
    document.getElementById('closePtCredModal')?.addEventListener('click', closePtCredModal);
    document.getElementById('dismissPtCredModal')?.addEventListener('click', closePtCredModal);

    document.getElementById('copyPtCodeBtn')?.addEventListener('click', () => {
      const code = document.getElementById('ptCredCode')?.textContent;
      if (code) {
        navigator.clipboard.writeText(code).then(() => {
          App.showToast(`Copied Patient ID: ${code}`, 'success');
        });
      }
    });

    document.getElementById('copyPtPwdBtn')?.addEventListener('click', () => {
      const pwd = document.getElementById('ptCredPassword')?.textContent;
      if (pwd) {
        navigator.clipboard.writeText(pwd).then(() => {
          App.showToast('Copied Password!', 'success');
        });
      }
    });

    // Reset Password Logic
    document.getElementById('dpResetPasswordBtn')?.addEventListener('click', async () => {
      if (!currentPatientId) return;
      const pwInput = document.getElementById('dpResetPasswordInput');
      const newPassword = pwInput.value.trim();
      if (newPassword.length < 6) {
        App.showToast('Password must be at least 6 characters', 'error');
        return;
      }
      try {
        await Auth.resetPatientPassword(currentPatientId, newPassword);
        pwInput.value = '';
        App.showToast('Patient password reset successfully', 'success');
      } catch(err) {
        App.showToast('Error resetting password: ' + err.message, 'error');
      }
    });

    // Calibration & Demo Sensing Logic
    const demoBtn = document.getElementById('calibDemoBtn');
    demoBtn?.addEventListener('click', () => {
      if (App.isDemo && App.isDemo()) {
        App.stopDemo();
        demoBtn.textContent = '⚡ Demo Motion';
        demoBtn.style.background = 'rgba(124,58,237,0.2)';
        App.showToast('Demo motion stopped');
      } else {
        App.startDemo();
        demoBtn.textContent = '⏹ Stop Demo';
        demoBtn.style.background = 'rgba(239,68,68,0.25)';
        App.showToast('Demo motion active! Move or click Capture buttons.', 'success');
      }
    });

    document.getElementById('calibMinBtn')?.addEventListener('click', () => {
      let data = App.getLatestData();
      if (!data || (data.pitch === 0 && !App.isDemo())) {
        // Auto-start demo if no live stream is active
        App.startDemo();
        data = App.getLatestData();
        App.showToast('Simulated sensor started. Capturing min angle...', 'info');
      }
      const angle = Math.abs(data?.pitch || (Math.random() * 20 + 15));
      document.getElementById('dpMinAngle').value = Math.round(angle);
      App.showToast(`Min angle captured: ${Math.round(angle)}°`, 'success');
    });

    document.getElementById('calibMaxBtn')?.addEventListener('click', () => {
      let data = App.getLatestData();
      if (!data || (data.pitch === 0 && !App.isDemo())) {
        App.startDemo();
        data = App.getLatestData();
        App.showToast('Simulated sensor started. Capturing max angle...', 'info');
      }
      const angle = Math.abs(data?.pitch ? (Math.abs(data.pitch) + 60) : (Math.random() * 30 + 85));
      const clamped = Math.min(175, Math.max(45, Math.round(angle)));
      document.getElementById('dpMaxAngle').value = clamped;
      App.showToast(`Max angle captured: ${clamped}°`, 'success');
    });

    document.getElementById('calibClearBtn')?.addEventListener('click', () => {
      document.getElementById('dpMinAngle').value = '30';
      document.getElementById('dpMaxAngle').value = '120';
      App.showToast('Calibration reset to defaults (30° – 120°)');
    });

    document.getElementById('backToPatients')?.addEventListener('click', () => {
      App.navigateTo('doctor-patients');
    });

    // Doctor patients nav refresh
    document.getElementById('nav-doctor-patients')?.addEventListener('click', async () => {
      const patients = await Auth.getPatientsByDoctorId(session.userId);
      await renderPatientCards(patients);
    });

    document.getElementById('nav-doctor-dashboard')?.addEventListener('click', renderDoctorDashboard);
  }

  // ─── Refresh (called after role changes) ─────────────────────────────────
  async function refresh() {
    const patients = await Auth.getPatientsByDoctorId(session.userId);
    await renderPatientCards(patients);
    await renderDoctorDashboard();
  }

  // ─── Helpers ──────────────────────────────────────────────────────────────
  function _setEl(id, val) {
    const el = document.getElementById(id);
    if (el) el.textContent = val;
  }
  function _setVal(id, val) {
    const el = document.getElementById(id);
    if (el) el.value = val;
  }
  function _formatDuration(s) {
    const h = Math.floor(s / 3600);
    const m = Math.floor((s % 3600) / 60);
    const sec = s % 60;
    if (h > 0) return `${h}h ${m}m`;
    if (m > 0) return `${m}m ${sec}s`;
    return `${sec}s`;
  }

  return { init, refresh, openPatientDetail, saveThreshold, savePrescription, savePatientProfile, markAlertReviewed, updateDoctorLiveSensor };
})();
