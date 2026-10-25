/**
 * PhysioPulse – Auth Module
 * Handles registration, login, session tokens, and all user/patient/threshold DB stores.
 * Connects directly to FastAPI backend with automatic IndexedDB caching & offline support.
 */

const Auth = (() => {
  // ─── IndexedDB Config ─────────────────────────────────────────────────────
  const DB_NAME    = 'PhysioPulseAuth';
  const DB_VERSION = 2;
  let db = null;

  // Store names
  const STORES = {
    USERS:     'users',
    PATIENTS:  'patients',
    THRESHOLDS:'thresholds',
    EX_HISTORY:'exerciseHistory',
  };

  // ─── DB Open / Upgrade ────────────────────────────────────────────────────
  function openDB() {
    return new Promise((resolve, reject) => {
      const req = indexedDB.open(DB_NAME, DB_VERSION);

      req.onupgradeneeded = (e) => {
        const d = e.target.result;

        // users store
        if (!d.objectStoreNames.contains(STORES.USERS)) {
          const us = d.createObjectStore(STORES.USERS, { keyPath: 'id', autoIncrement: true });
          us.createIndex('email', 'email', { unique: true });
          us.createIndex('role', 'role', { unique: false });
        }

        // patients store
        if (!d.objectStoreNames.contains(STORES.PATIENTS)) {
          const ps = d.createObjectStore(STORES.PATIENTS, { keyPath: 'id', autoIncrement: true });
          ps.createIndex('userId',   'userId',   { unique: true });
          ps.createIndex('doctorId', 'doctorId', { unique: false });
        }

        // thresholds store
        if (!d.objectStoreNames.contains(STORES.THRESHOLDS)) {
          const ts = d.createObjectStore(STORES.THRESHOLDS, { keyPath: 'id', autoIncrement: true });
          ts.createIndex('patientId', 'patientId', { unique: false });
          ts.createIndex('doctorId',  'doctorId',  { unique: false });
        }

        // exercise history store
        if (!d.objectStoreNames.contains(STORES.EX_HISTORY)) {
          const hs = d.createObjectStore(STORES.EX_HISTORY, { keyPath: 'id', autoIncrement: true });
          hs.createIndex('patientId', 'patientId', { unique: false });
          hs.createIndex('date',      'date',       { unique: false });
        }
      };

      req.onsuccess = (e) => { db = e.target.result; resolve(db); };
      req.onerror   = (e) => reject(e.target.error);
    });
  }

  // ─── Generic DB Helpers ───────────────────────────────────────────────────
  function dbAdd(storeName, obj) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve(null);
      const tx = db.transaction(storeName, 'readwrite');
      const store = tx.objectStore(storeName);
      const req = store.add(obj);
      req.onsuccess = () => resolve(req.result);
      req.onerror   = () => reject(req.error);
    });
  }

  function dbPut(storeName, obj) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve(null);
      const tx = db.transaction(storeName, 'readwrite');
      const store = tx.objectStore(storeName);
      const req = store.put(obj);
      req.onsuccess = () => resolve(req.result);
      req.onerror   = () => reject(req.error);
    });
  }

  function dbGet(storeName, key) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve(null);
      const tx = db.transaction(storeName, 'readonly');
      const store = tx.objectStore(storeName);
      const req = store.get(key);
      req.onsuccess = () => resolve(req.result);
      req.onerror   = () => reject(req.error);
    });
  }

  function dbGetAll(storeName) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve([]);
      const tx = db.transaction(storeName, 'readonly');
      const store = tx.objectStore(storeName);
      const req = store.getAll();
      req.onsuccess = () => resolve(req.result || []);
      req.onerror   = () => reject(req.error);
    });
  }

  function dbGetByIndex(storeName, indexName, value) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve([]);
      const tx = db.transaction(storeName, 'readonly');
      const store = tx.objectStore(storeName);
      const index = store.index(indexName);
      const req = index.getAll(value);
      req.onsuccess = () => resolve(req.result || []);
      req.onerror   = () => reject(req.error);
    });
  }

  function dbGetOneByIndex(storeName, indexName, value) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve(null);
      const tx = db.transaction(storeName, 'readonly');
      const store = tx.objectStore(storeName);
      const index = store.index(indexName);
      const req = index.get(value);
      req.onsuccess = () => resolve(req.result || null);
      req.onerror   = () => reject(req.error);
    });
  }

  function dbDelete(storeName, key) {
    return new Promise((resolve, reject) => {
      if (!db) return resolve();
      const tx = db.transaction(storeName, 'readwrite');
      const store = tx.objectStore(storeName);
      const req = store.delete(key);
      req.onsuccess = () => resolve();
      req.onerror   = () => reject(req.error);
    });
  }

  // ─── Session Management ───────────────────────────────────────────────────
  const SESSION_KEY = 'pp_session';
  const SESSION_EXPIRY_MS = 7 * 24 * 60 * 60 * 1000; // 7 days

  function saveSession(session) {
    const data = { ...session, expiry: Date.now() + SESSION_EXPIRY_MS };
    localStorage.setItem(SESSION_KEY, JSON.stringify(data));
  }

  function getSession() {
    try {
      const raw = localStorage.getItem(SESSION_KEY);
      if (!raw) return null;
      const data = JSON.parse(raw);
      if (Date.now() > data.expiry) {
        localStorage.removeItem(SESSION_KEY);
        if (typeof API !== 'undefined') API.setToken(null);
        return null;
      }
      return data;
    } catch {
      return null;
    }
  }

  function clearSession() {
    localStorage.removeItem(SESSION_KEY);
    if (typeof API !== 'undefined') API.setToken(null);
  }

  // ─── Registration ─────────────────────────────────────────────────────────
  async function register(role, name, email, password, doctorCode) {
    if (!name || !email || !password) throw new Error('All fields are required');
    if (password.length < 6) throw new Error('Password must be at least 6 characters');

    try {
      if (typeof API !== 'undefined') {
        const res = await API.register(role, name, email, password, doctorCode);
        const session = {
          userId: res.user.id,
          role: res.user.role,
          name: res.user.name,
          email: res.user.email,
          doctorId: res.doctor_id || null,
          doctorCode: res.user.doctor_code || null,
          patientId: res.patient_id || null,
        };
        saveSession(session);
        return session;
      }
    } catch (apiErr) {
      // If it's a validation error from the server (e.g. 400), surface it directly
      if (apiErr.message && !apiErr.message.includes('Failed to fetch') && !apiErr.message.includes('NetworkError')) {
        throw apiErr;
      }
      console.warn('[Auth] Backend registration failed, falling back to local storage:', apiErr);
    }

    // Local IndexedDB Fallback
    const existing = await dbGetOneByIndex(STORES.USERS, 'email', email.toLowerCase());
    if (existing) throw new Error('An account with this email already exists');

    let doctorId = null;
    if (role === 'patient') {
      if (!doctorCode || doctorCode.trim() === '') throw new Error('Doctor Code is required for patient registration');
      const allUsers = await dbGetAll(STORES.USERS);
      const doctors = allUsers.filter(u => u.role === 'doctor');
      const matchingDoctor = doctors.find(d => (d.doctorCode || '').toUpperCase() === doctorCode.trim().toUpperCase());
      if (!matchingDoctor) throw new Error('Invalid Doctor Code. Please check with your doctor.');
      doctorId = matchingDoctor.id;
    }

    const user = {
      name: name.trim(),
      email: email.toLowerCase().trim(),
      role,
      doctorId,
      createdAt: new Date().toISOString(),
    };

    const userId = await dbAdd(STORES.USERS, user);
    let code = null;
    if (role === 'doctor') {
      code = 'DR' + userId.toString().padStart(4, '0');
      user.doctorCode = code;
      user.id = userId;
      await dbPut(STORES.USERS, user);
    } else if (role === 'patient') {
      await dbAdd(STORES.PATIENTS, {
        userId,
        doctorId,
        name: name.trim(),
        age: null,
        condition: '',
        createdAt: new Date().toISOString(),
        hasBaselineData: false,
      });
    }

    const session = {
      userId,
      role,
      name: name.trim(),
      email: email.toLowerCase().trim(),
      doctorId,
      doctorCode: code,
    };
    saveSession(session);
    return session;
  }

  // ─── Login ────────────────────────────────────────────────────────────────
  async function login(email, password) {
    if (!email || !password) throw new Error('Email and password are required');

    try {
      if (typeof API !== 'undefined') {
        const res = await API.login(email, password);
        const session = {
          userId: res.user.id,
          role: res.user.role,
          name: res.user.name,
          email: res.user.email,
          doctorId: res.doctor_id || null,
          doctorCode: res.user.doctor_code || null,
          patientId: res.patient_id || null,
        };
        saveSession(session);
        return session;
      }
    } catch (apiErr) {
      if (apiErr.message && !apiErr.message.includes('Failed to fetch') && !apiErr.message.includes('NetworkError')) {
        throw apiErr;
      }
      console.warn('[Auth] Backend login failed, falling back to local storage:', apiErr);
    }

    // Local IndexedDB Fallback
    const user = await dbGetOneByIndex(STORES.USERS, 'email', email.toLowerCase().trim());
    if (!user) throw new Error('No account found with this email');

    let patientId = null;
    if (user.role === 'patient') {
      const p = await dbGetOneByIndex(STORES.PATIENTS, 'userId', user.id);
      if (p) patientId = p.id;
    }

    const session = {
      userId: user.id,
      role: user.role,
      name: user.name,
      email: user.email,
      doctorId: user.doctorId || null,
      doctorCode: user.doctorCode || null,
      patientId,
    };
    saveSession(session);
    return session;
  }

  // ─── Logout ───────────────────────────────────────────────────────────────
  function logout() {
    clearSession();
  }

  // ─── Patient Profile CRUD ─────────────────────────────────────────────────
  async function getPatientByUserId(userId) {
    try {
      if (typeof API !== 'undefined') {
        const p = await API.getPatientByUserId(userId);
        if (p) {
          // Normalize field names
          const norm = {
            id: p.id,
            userId: p.user_id,
            doctorId: p.doctor_id,
            name: p.name,
            age: p.age,
            condition: p.condition,
            hasBaselineData: p.has_baseline_data,
            doctorName: p.doctor_name,
            doctorCode: p.doctor_code,
          };
          dbPut(STORES.PATIENTS, norm).catch(() => {});
          return norm;
        }
      }
    } catch (e) {
      console.warn('[Auth] getPatientByUserId API error, falling back:', e.message);
    }
    return dbGetOneByIndex(STORES.PATIENTS, 'userId', userId);
  }

  async function getPatientById(patientId) {
    try {
      if (typeof API !== 'undefined' && patientId) {
        const p = await API.getPatient(patientId);
        if (p) {
          const norm = {
            id: p.id,
            userId: p.user_id,
            doctorId: p.doctor_id,
            name: p.name,
            age: p.age,
            condition: p.condition,
            hasBaselineData: p.has_baseline_data,
            doctorName: p.doctor_name,
            doctorCode: p.doctor_code,
          };
          dbPut(STORES.PATIENTS, norm).catch(() => {});
          return norm;
        }
      }
    } catch (e) {
      console.warn('[Auth] getPatientById API error, falling back:', e.message);
    }
    return dbGet(STORES.PATIENTS, patientId);
  }

  async function getPatientsByDoctorId(doctorId) {
    try {
      if (typeof API !== 'undefined') {
        const list = await API.getPatients();
        if (Array.isArray(list)) {
          const mapped = list.map(p => ({
            id: p.id,
            userId: p.user_id,
            doctorId: p.doctor_id,
            name: p.name,
            age: p.age,
            condition: p.condition,
            hasBaselineData: p.has_baseline_data,
            doctorName: p.doctor_name,
            doctorCode: p.doctor_code,
          }));
          mapped.forEach(p => dbPut(STORES.PATIENTS, p).catch(() => {}));
          return mapped;
        }
      }
    } catch (e) {
      console.warn('[Auth] getPatientsByDoctorId API error, falling back:', e.message);
    }
    return dbGetByIndex(STORES.PATIENTS, 'doctorId', doctorId);
  }

  async function updatePatientProfile(patientObj) {
    try {
      if (typeof API !== 'undefined' && patientObj.id) {
        await API.updatePatient(patientObj.id, {
          name: patientObj.name,
          age: patientObj.age,
          condition: patientObj.condition,
          has_baseline_data: patientObj.hasBaselineData,
        });
      }
    } catch (e) {
      console.warn('[Auth] updatePatientProfile API error:', e.message);
    }
    return dbPut(STORES.PATIENTS, patientObj);
  }

  // ─── Threshold CRUD ───────────────────────────────────────────────────────
  async function saveThreshold(thresholdObj) {
    try {
      if (typeof API !== 'undefined' && thresholdObj.patientId) {
        const saved = await API.saveThreshold(thresholdObj.patientId, {
          min_angle: thresholdObj.minAngle,
          max_angle: thresholdObj.maxAngle,
          target_reps: thresholdObj.targetReps,
          motion_limit: thresholdObj.motionLimit,
          temp_limit: thresholdObj.tempLimit,
          exercise_type: thresholdObj.exerciseType,
          video_url: thresholdObj.video_url || thresholdObj.videoUrl || '',
          strict_limit: thresholdObj.strict_limit !== undefined ? thresholdObj.strict_limit : (thresholdObj.strictLimit !== undefined ? thresholdObj.strictLimit : true),
          notes: thresholdObj.notes || '',
        });
        const norm = {
          id: saved.id,
          patientId: saved.patient_id,
          doctorId: saved.doctor_id,
          minAngle: saved.min_angle,
          maxAngle: saved.max_angle,
          targetReps: saved.target_reps,
          motionLimit: saved.motion_limit,
          tempLimit: saved.temp_limit,
          exerciseType: saved.exercise_type,
          videoUrl: saved.video_url,
          video_url: saved.video_url,
          strictLimit: saved.strict_limit,
          strict_limit: saved.strict_limit,
          notes: saved.notes,
          createdAt: saved.created_at,
          updatedAt: saved.updated_at,
        };
        dbPut(STORES.THRESHOLDS, norm).catch(() => {});
        return norm;
      }
    } catch (e) {
      console.warn('[Auth] saveThreshold API error:', e.message);
    }

    // Local IndexedDB fallback
    const existing = await dbGetByIndex(STORES.THRESHOLDS, 'patientId', thresholdObj.patientId);
    if (existing && existing.length > 0) {
      const latest = existing[existing.length - 1];
      const updated = { ...latest, ...thresholdObj, updatedAt: new Date().toISOString() };
      return dbPut(STORES.THRESHOLDS, updated);
    } else {
      const newThreshold = { ...thresholdObj, createdAt: new Date().toISOString(), updatedAt: new Date().toISOString() };
      return dbAdd(STORES.THRESHOLDS, newThreshold);
    }
  }

  async function getThresholdForPatient(patientId) {
    try {
      if (typeof API !== 'undefined' && patientId) {
        const t = await API.getActiveThreshold(patientId);
        if (t) {
          return {
            id: t.id,
            patientId: t.patient_id,
            doctorId: t.doctor_id,
            minAngle: t.min_angle,
            maxAngle: t.max_angle,
            targetReps: t.target_reps,
            motionLimit: t.motion_limit,
            tempLimit: t.temp_limit,
            exerciseType: t.exercise_type,
            videoUrl: t.video_url,
            video_url: t.video_url,
            strictLimit: t.strict_limit,
            strict_limit: t.strict_limit,
            notes: t.notes,
            createdAt: t.created_at,
            updatedAt: t.updated_at,
          };
        }
      }
    } catch (e) {
      console.warn('[Auth] getThresholdForPatient API error, falling back:', e.message);
    }

    const all = await dbGetByIndex(STORES.THRESHOLDS, 'patientId', patientId);
    if (!all || all.length === 0) return null;
    return all.sort((a, b) => new Date(b.updatedAt || b.createdAt) - new Date(a.updatedAt || a.createdAt))[0];
  }

  async function getThresholdHistoryForPatient(patientId) {
    try {
      if (typeof API !== 'undefined' && patientId) {
        const list = await API.getThresholdHistory(patientId);
        if (Array.isArray(list)) {
          return list.map(t => ({
            id: t.id,
            patientId: t.patient_id,
            doctorId: t.doctor_id,
            minAngle: t.min_angle,
            maxAngle: t.max_angle,
            targetReps: t.target_reps,
            motionLimit: t.motion_limit,
            tempLimit: t.temp_limit,
            exerciseType: t.exercise_type,
            videoUrl: t.video_url,
            video_url: t.video_url,
            strictLimit: t.strict_limit,
            strict_limit: t.strict_limit,
            notes: t.notes,
            createdAt: t.created_at,
            updatedAt: t.updated_at,
          }));
        }
      }
    } catch (e) {
      console.warn('[Auth] getThresholdHistoryForPatient API error:', e.message);
    }
    const all = await dbGetByIndex(STORES.THRESHOLDS, 'patientId', patientId);
    if (!all) return [];
    return all.sort((a, b) => new Date(b.updatedAt || b.createdAt) - new Date(a.updatedAt || a.createdAt));
  }

  // ─── Exercise History CRUD ────────────────────────────────────────────────
  async function saveExerciseRecord(record) {
    try {
      if (typeof API !== 'undefined') {
        const saved = await API.saveExerciseRecord({
          patient_id: record.patientId,
          date: record.date || new Date().toISOString(),
          reps_completed: record.repsCompleted,
          target_reps: record.targetReps,
          max_angle_reached: record.maxAngleReached,
          min_angle_reached: record.minAngleReached,
          avg_temp: record.avgTemp,
          duration_s: record.duration_s,
          status: record.status,
          exercise_type: record.exerciseType,
          notes: record.notes || '',
        });
        const norm = {
          id: saved.id,
          patientId: saved.patient_id,
          doctorId: saved.doctor_id,
          date: saved.date,
          repsCompleted: saved.reps_completed,
          targetReps: saved.target_reps,
          maxAngleReached: saved.max_angle_reached,
          minAngleReached: saved.min_angle_reached,
          avgTemp: saved.avg_temp,
          duration_s: saved.duration_s,
          status: saved.status,
          exerciseType: saved.exercise_type,
          notes: saved.notes,
        };
        dbAdd(STORES.EX_HISTORY, norm).catch(() => {});
        return norm;
      }
    } catch (e) {
      console.warn('[Auth] saveExerciseRecord API error:', e.message);
    }
    const obj = { ...record, date: record.date || new Date().toISOString() };
    return dbAdd(STORES.EX_HISTORY, obj);
  }

  async function getExerciseHistoryForPatient(patientId) {
    try {
      if (typeof API !== 'undefined' && patientId) {
        const list = await API.getExerciseHistory(patientId);
        if (Array.isArray(list)) {
          return list.map(h => ({
            id: h.id,
            patientId: h.patient_id,
            doctorId: h.doctor_id,
            date: h.date,
            repsCompleted: h.reps_completed,
            targetReps: h.target_reps,
            maxAngleReached: h.max_angle_reached,
            minAngleReached: h.min_angle_reached,
            avgTemp: h.avg_temp,
            duration_s: h.duration_s,
            status: h.status,
            exerciseType: h.exercise_type,
            notes: h.notes,
          }));
        }
      }
    } catch (e) {
      console.warn('[Auth] getExerciseHistoryForPatient API error:', e.message);
    }
    const all = await dbGetByIndex(STORES.EX_HISTORY, 'patientId', patientId);
    return all.sort((a, b) => new Date(b.date) - new Date(a.date));
  }

  // ─── Doctor Info ──────────────────────────────────────────────────────────
  async function getDoctorById(doctorId) {
    const all = await getAllDoctors();
    return all.find(d => d.id === doctorId) || dbGet(STORES.USERS, doctorId);
  }

  async function getAllDoctors() {
    try {
      if (typeof API !== 'undefined') {
        const docs = await API.getDoctors();
        if (Array.isArray(docs)) {
          return docs.map(d => ({
            id: d.id,
            name: d.name,
            email: d.email,
            role: d.role,
            doctorCode: d.doctor_code,
          }));
        }
      }
    } catch (e) {}
    const all = await dbGetAll(STORES.USERS);
    return all.filter(u => u.role === 'doctor');
  }

  // ─── Init ─────────────────────────────────────────────────────────────────
  async function init() {
    await openDB();
    if (typeof API !== 'undefined') {
      const online = await API.checkHealth();
      console.log(`[Auth] API is ${online ? 'ONLINE' : 'OFFLINE'}. IndexedDB ready.`);
      if (online && API.getToken()) {
        try {
          const meData = await API.getMe();
          if (meData && meData.user) {
            const updatedSession = {
              userId: meData.user.id,
              role: meData.user.role,
              name: meData.user.name,
              email: meData.user.email,
              doctorId: meData.doctor_id || null,
              doctorCode: meData.doctor_code || null,
              patientId: meData.patient_id || null,
            };
            saveSession(updatedSession);
          }
        } catch (e) {
          console.log('[Auth] Token validation error on init:', e.message);
        }
      }
    }
  }


  async function createPatient(data) {
    if (typeof API !== 'undefined') {
      return API.createPatient(data);
    }
    throw new Error("Offline mode not supported for creating patients");
  }

  async function resetPatientPassword(patientId, password) {
    if (typeof API !== 'undefined') {
      return API.resetPatientPassword(patientId, password);
    }
    throw new Error("Offline mode not supported for resetting passwords");
  }

  // ─── Public API ───

  return {
    init,
    register,
    login,
    logout,
    getSession,
    // patient
    getPatientById,
    createPatient,
    resetPatientPassword,
    getPatientByUserId,
    getPatientsByDoctorId,
    updatePatientProfile,
    // thresholds
    saveThreshold,
    getThresholdForPatient,
    getThresholdHistoryForPatient,
    // exercise history
    saveExerciseRecord,
    getExerciseHistoryForPatient,
    // doctor
    getDoctorById,
    getAllDoctors,
    // db helpers
    _dbGet: dbGet,
    _dbGetAll: dbGetAll,
    _dbGetByIndex: dbGetByIndex,
    _dbPut: dbPut,
  };
})();
