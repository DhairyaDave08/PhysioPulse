/**
 * PhysioPulse – API Client Module
 * Communicates with the FastAPI Backend for Patient History, Thresholds, and Sessions.
 */

const API = (() => {
  // Determine API base URL
  const DEFAULT_BACKEND_URL = 'http://localhost:8000';
  let isBackendOnline = false;

  function getBaseUrl() {
    const origin = window.location.origin || '';
    const hostname = window.location.hostname || 'localhost';
    const port = window.location.port;

    // If served directly by FastAPI backend on port 8000
    if (port === '8000') {
      return origin;
    }

    // If served by a frontend dev server (Live Server 5500/5501, Vite 5173, React 3000, WebServer 8080, etc.)
    // Route API requests to FastAPI backend on port 8000 on the same host
    if (hostname === 'localhost' || hostname === '127.0.0.1' || hostname.startsWith('192.168.') || hostname.startsWith('10.')) {
      const protocol = window.location.protocol === 'https:' ? 'https:' : 'http:';
      return `${protocol}//${hostname}:8000`;
    }

    // ESP32 AP Mode fallback or default
    if (origin.includes('192.168.4.1') || origin.startsWith('file:')) {
      return DEFAULT_BACKEND_URL;
    }

    return DEFAULT_BACKEND_URL;
  }

  function formatDateLocal(dateStr) {
    if (!dateStr) return '—';
    try {
      // If dateStr is an ISO string without trailing Z or offset, append Z so it parses as UTC
      let s = String(dateStr).trim();
      if (!s.endsWith('Z') && !s.includes('+') && !s.includes('-') && s.includes('T')) {
        s += 'Z';
      } else if (s.includes(' ') && !s.includes('+')) {
        s = s.replace(' ', 'T') + 'Z';
      }
      const d = new Date(s);
      if (isNaN(d.getTime())) return new Date(dateStr).toLocaleString();
      return d.toLocaleString();
    } catch (e) {
      return new Date(dateStr).toLocaleString();
    }
  }

  const TOKEN_KEY = 'pp_jwt_token';

  function getToken() {
    return localStorage.getItem(TOKEN_KEY);
  }

  function setToken(token) {
    if (token) {
      localStorage.setItem(TOKEN_KEY, token);
    } else {
      localStorage.removeItem(TOKEN_KEY);
    }
  }

  async function checkHealth() {
    try {
      const res = await fetch(`${getBaseUrl()}/api/health`, {
        method: 'GET',
        headers: { 'Accept': 'application/json' },
        signal: AbortSignal.timeout(2000),
      });
      if (res.ok) {
        isBackendOnline = true;
        return true;
      }
    } catch (e) {
      isBackendOnline = false;
    }
    return false;
  }

  async function request(endpoint, options = {}) {
    const url = `${getBaseUrl()}${endpoint}`;
    const token = getToken();

    const headers = {
      'Content-Type': 'application/json',
      'Accept': 'application/json',
      ...(options.headers || {}),
    };

    if (token) {
      headers['Authorization'] = `Bearer ${token}`;
    }

    try {
      const res = await fetch(url, {
        ...options,
        headers,
      });

      if (res.status === 401) {
        // Token expired or invalid
        setToken(null);
      }

      if (!res.ok) {
        let errorMsg = `HTTP Error ${res.status}`;
        try {
          const errData = await res.json();
          if (typeof errData.detail === 'string') {
            errorMsg = errData.detail;
          } else if (Array.isArray(errData.detail)) {
            // FastAPI validation error array [{loc, msg, type}]
            errorMsg = errData.detail.map(d => d.msg || JSON.stringify(d)).join(', ');
          } else if (errData.detail && typeof errData.detail === 'object') {
            errorMsg = JSON.stringify(errData.detail);
          }
        } catch (_) {}
        throw new Error(errorMsg);
      }

      // If response is json
      const contentType = res.headers.get('content-type');
      if (contentType && contentType.includes('application/json')) {
        return await res.json();
      }
      return await res.text();
    } catch (err) {
      console.warn(`[API] Request failed (${endpoint}):`, err.message);
      throw err;
    }
  }

  // ─── Auth API ─────────────────────────────────────────────────────────────
  async function register(role, name, email, password, doctorCode) {
    const payload = { role, name, email, password, doctor_code: doctorCode || null };
    const res = await request('/api/auth/register', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
    if (res.access_token) {
      setToken(res.access_token);
    }
    return res;
  }

  async function login(email, password) {
    const payload = { email, password };
    const res = await request('/api/auth/login', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
    if (res.access_token) {
      setToken(res.access_token);
    }
    return res;
  }

  async function getMe() {
    return request('/api/auth/me');
  }

  async function getDoctors() {
    return request('/api/auth/doctors');
  }

  // ─── Patient API ──────────────────────────────────────────────────────────
  async function getPatients() {
    return request('/api/patients');
  }

  async function getPatient(patientId) {
    return request(`/api/patients/${patientId}`);
  }

  async function getPatientByUserId(userId) {
    return request(`/api/patients/by-user/${userId}`);
  }

  async function updatePatient(patientId, data) {
    return request(`/api/patients/${patientId}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    });
  }

  // ─── Thresholds API ───────────────────────────────────────────────────────
  async function getActiveThreshold(patientId) {
    return request(`/api/patients/${patientId}/threshold`);
  }

  async function saveThreshold(patientId, data) {
    return request(`/api/patients/${patientId}/threshold`, {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async function getThresholdHistory(patientId) {
    return request(`/api/patients/${patientId}/thresholds`);
  }

  // ─── Exercise History API ─────────────────────────────────────────────────
  async function saveExerciseRecord(data) {
    return request('/api/exercise-history', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async function getExerciseHistory(patientId) {
    if (patientId) {
      return request(`/api/patients/${patientId}/exercise-history`);
    }
    return request('/api/exercise-history');
  }

  async function getPatientAnalytics(patientId) {
    return request(`/api/patients/${patientId}/analytics`);
  }

  // ─── Sessions API ─────────────────────────────────────────────────────────
  async function saveSession(data) {
    return request('/api/sessions', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async function getSessions(patientId) {
    const query = patientId ? `?patient_id=${patientId}` : '';
    return request(`/api/sessions${query}`);
  }

  async function getSessionDetail(sessionId) {
    return request(`/api/sessions/${sessionId}`);
  }

  async function deleteSession(sessionId) {
    return request(`/api/sessions/${sessionId}`, { method: 'DELETE' });
  }

  async function deleteAllSessions() {
    return request('/api/sessions', { method: 'DELETE' });
  }

  // ─── Pain Alerts API ───────────────────────────────────────────────────
  async function sendPainAlert(patientId, data) {
    return request(`/api/patients/${patientId}/pain-alert`, {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async function getPatientPainAlerts(patientId) {
    return request(`/api/patients/${patientId}/pain-alerts`);
  }

  async function getDoctorPainAlerts() {
    return request('/api/doctor/pain-alerts');
  }

  async function resolvePainAlert(alertId, status = 'reviewed') {
    return request(`/api/pain-alerts/${alertId}/resolve`, {
      method: 'PUT',
      body: JSON.stringify({ status }),
    });
  }

  function getCsvUrl(sessionId) {
    return `${getBaseUrl()}/api/sessions/${sessionId}/csv`;
  }


  async function createPatient(data) {
    return request('/api/patients/create', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async function resetPatientPassword(patientId, password) {
    return request(`/api/patients/${patientId}/reset-password`, {
      method: 'PUT',
      body: JSON.stringify({ password }),
    });
  }

  return {
    checkHealth,
    isOnline: () => isBackendOnline,
    getToken,
    setToken,
    getBaseUrl,
    formatDateLocal,
    // Auth
    register,
    login,
    getMe,
    getDoctors,
    // Patients
    createPatient,
    resetPatientPassword,

    getPatients,
    createPatient,
    resetPatientPassword,
    getPatient,
    getPatientByUserId,
    updatePatient,
    // Thresholds
    getActiveThreshold,
    saveThreshold,
    getThresholdHistory,
    // Exercise
    saveExerciseRecord,
    getExerciseHistory,
    getPatientAnalytics,
    // Pain Alerts
    sendPainAlert,
    getPatientPainAlerts,
    getDoctorPainAlerts,
    resolvePainAlert,
    // Sessions
    saveSession,
    getSessions,
    getSessionDetail,
    deleteSession,
    deleteAllSessions,
    getCsvUrl,
  };
})();
