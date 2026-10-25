/**
 * PhysioPulse – Service Worker
 * Provides offline capability and caches static assets
 */

const CACHE_NAME = 'physiopulse-v2.0.0';
const STATIC_ASSETS = [
  '/',
  '/index.html',
  '/css/style.css',
  '/js/app.js',
  '/js/auth.js',
  '/js/doctor-portal.js',
  '/js/patient-portal.js',
  '/js/connection.js',
  '/js/charts.js',
  '/js/session.js',
  '/js/backend-config.js',
  '/manifest.json',
  // Chart.js is served from CDN – we cache it too via runtime caching
];

// ─── Install: Cache static assets ─────────────────────────────────────────
self.addEventListener('install', (event) => {
  console.log('[SW] Installing PhysioPulse service worker...');
  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => {
      console.log('[SW] Caching static assets');
      return cache.addAll(STATIC_ASSETS).catch(err => {
        console.warn('[SW] Some assets failed to cache (OK for dev):', err);
      });
    }).then(() => self.skipWaiting())
  );
});

// ─── Activate: Clean old caches ───────────────────────────────────────────
self.addEventListener('activate', (event) => {
  console.log('[SW] Activating...');
  event.waitUntil(
    caches.keys().then((keys) => {
      return Promise.all(
        keys.filter(k => k !== CACHE_NAME).map(k => {
          console.log('[SW] Deleting old cache:', k);
          return caches.delete(k);
        })
      );
    }).then(() => self.clients.claim())
  );
});

// ─── Fetch: Network-first with cache fallback ─────────────────────────────
self.addEventListener('fetch', (event) => {
  const url = new URL(event.request.url);

  // Skip WebSocket requests
  if (event.request.url.startsWith('ws://') || event.request.url.startsWith('wss://')) {
    return;
  }

  // Skip API requests (always fresh)
  if (url.pathname.startsWith('/api/')) {
    return;
  }

  // For CDN resources (Chart.js etc.) – cache-first
  if (url.hostname !== self.location.hostname) {
    event.respondWith(
      caches.match(event.request).then(cached => {
        if (cached) return cached;
        return fetch(event.request).then(resp => {
          if (resp && resp.status === 200) {
            const clone = resp.clone();
            caches.open(CACHE_NAME).then(c => c.put(event.request, clone));
          }
          return resp;
        }).catch(() => cached);
      })
    );
    return;
  }

  // For local assets – network-first, fallback to cache
  event.respondWith(
    fetch(event.request)
      .then(resp => {
        if (resp && resp.status === 200 && resp.type === 'basic') {
          const clone = resp.clone();
          caches.open(CACHE_NAME).then(c => c.put(event.request, clone));
        }
        return resp;
      })
      .catch(() => {
        return caches.match(event.request).then(cached => {
          if (cached) return cached;
          // Return offline fallback for HTML navigation
          if (event.request.mode === 'navigate') {
            return caches.match('/index.html');
          }
        });
      })
  );
});

console.log('[SW] PhysioPulse Service Worker loaded');
