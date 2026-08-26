#pragma once

const char webapp_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover" />
  <meta name="theme-color" content="#0a0e1a" />
  <meta name="description" content="PhysioPulse – Real-time physiotherapy monitoring dashboard for accelerometer, gyroscope and temperature data from MYOSA ESP32 sensor." />
  <meta name="apple-mobile-web-app-capable" content="yes" />
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />
  <meta name="apple-mobile-web-app-title" content="PhysioPulse" />
  <title>PhysioPulse – Physiotherapy Monitor</title>
  
  <link rel="preconnect" href="https://fonts.googleapis.com" />
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800&family=Roboto+Mono:wght@400;500;600&display=swap" rel="stylesheet" />
  <style>/* ═══════════════════════════════════════════════════════════════════════════
   PhysioPulse – Premium Dark Clinical CSS Theme
   MYOSA 6.0 Physiotherapy Monitoring System
   ═══════════════════════════════════════════════════════════════════════════ */

/* ─── Google Fonts + Reset ──────────────────────────────────────────────── */
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800&family=Roboto+Mono:wght@400;500;600&display=swap');

*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

/* ─── Design Tokens ─────────────────────────────────────────────────────── */
:root {
  /* Color Palette */
  --bg-base:      #080c18;
  --bg-surface:   #0e1525;
  --bg-elevated:  #141c2e;
  --bg-card:      #1a2540;
  --bg-hover:     #1e2d4d;
  --border:       rgba(255,255,255,0.07);
  --border-glow:  rgba(0,212,255,0.25);

  /* Brand */
  --cyan:         #00d4ff;
  --cyan-dim:     rgba(0,212,255,0.15);
  --cyan-glow:    rgba(0,212,255,0.4);
  --purple:       #7c3aed;
  --purple-dim:   rgba(124,58,237,0.15);
  --purple-light: #a855f7;
  --amber:        #f59e0b;
  --red:          #ef4444;
  --green:        #22c55e;
  --blue:         #3b82f6;
  --orange:       #f97316;
  --teal:         #06b6d4;

  /* Text */
  --text-primary:   #f0f4ff;
  --text-secondary: #8b9bbf;
  --text-muted:     #4a5a7a;
  --text-accent:    var(--cyan);

  /* Axis Colors */
  --axis-x: #ef4444;
  --axis-y: #22c55e;
  --axis-z: #3b82f6;

  /* Fonts */
  --font-sans:  'Inter', system-ui, sans-serif;
  --font-mono:  'Roboto Mono', 'Courier New', monospace;

  /* Spacing */
  --sidebar-width: 260px;
  --topbar-h:      64px;
  --radius-sm:  8px;
  --radius-md:  12px;
  --radius-lg:  18px;
  --radius-xl:  24px;

  /* Shadows */
  --shadow-card:  0 4px 24px rgba(0,0,0,0.4), 0 1px 4px rgba(0,0,0,0.3);
  --shadow-glow:  0 0 24px rgba(0,212,255,0.15);
  --shadow-modal: 0 24px 80px rgba(0,0,0,0.7);

  /* Transitions */
  --trans-fast:   0.15s ease;
  --trans-base:   0.25s ease;
  --trans-slow:   0.4s ease;
}

/* ─── Base ──────────────────────────────────────────────────────────────── */
html { font-size: 16px; scroll-behavior: smooth; }

body {
  font-family: var(--font-sans);
  background: var(--bg-base);
  color: var(--text-primary);
  min-height: 100vh;
  overflow-x: hidden;
  -webkit-font-smoothing: antialiased;
}

.mono { font-family: var(--font-mono); }
.hidden { display: none !important; }

/* ─── Animated Background ───────────────────────────────────────────────── */
body::before {
  content: '';
  position: fixed;
  inset: 0;
  background:
    radial-gradient(ellipse 80% 60% at 20% 0%, rgba(0,212,255,0.04) 0%, transparent 60%),
    radial-gradient(ellipse 60% 50% at 80% 100%, rgba(124,58,237,0.05) 0%, transparent 60%);
  pointer-events: none;
  z-index: 0;
}

/* ═══ SPLASH SCREEN ═════════════════════════════════════════════════════ */
.splash {
  position: fixed;
  inset: 0;
  background: var(--bg-base);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999;
  transition: opacity 0.6s ease, visibility 0.6s ease;
}
.splash.fade-out { opacity: 0; visibility: hidden; }

.splash-content { text-align: center; }

.splash-icon {
  width: 96px;
  height: 96px;
  margin: 0 auto 1.5rem;
  animation: splashPulse 2s ease-in-out infinite;
}
@keyframes splashPulse {
  0%, 100% { transform: scale(1); filter: drop-shadow(0 0 12px var(--cyan-glow)); }
  50% { transform: scale(1.06); filter: drop-shadow(0 0 24px var(--cyan-glow)); }
}

.splash-title {
  font-size: 2.5rem;
  font-weight: 800;
  background: linear-gradient(135deg, var(--cyan), var(--purple-light));
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  letter-spacing: -0.5px;
  margin-bottom: 0.5rem;
}
.splash-subtitle {
  color: var(--text-secondary);
  font-size: 1rem;
  font-weight: 400;
  margin-bottom: 0.25rem;
}
.splash-team {
  color: var(--text-muted);
  font-size: 0.8rem;
  font-weight: 500;
  letter-spacing: 2px;
  text-transform: uppercase;
  margin-bottom: 3rem;
}

.loader-bar {
  width: 260px;
  height: 4px;
  background: rgba(255,255,255,0.07);
  border-radius: 99px;
  overflow: hidden;
  margin: 0 auto 1rem;
}
.loader-fill {
  height: 100%;
  width: 0%;
  background: linear-gradient(90deg, var(--cyan), var(--purple-light));
  border-radius: 99px;
  transition: width 0.4s ease;
  box-shadow: 0 0 12px var(--cyan-glow);
}
.loader-text {
  color: var(--text-muted);
  font-size: 0.8rem;
  font-weight: 500;
}

/* ═══ APP LAYOUT ════════════════════════════════════════════════════════ */
.app {
  display: flex;
  min-height: 100vh;
  position: relative;
  z-index: 1;
}

/* ═══ SIDEBAR ═══════════════════════════════════════════════════════════ */
.sidebar {
  position: fixed;
  top: 0; left: 0;
  width: var(--sidebar-width);
  height: 100vh;
  background: var(--bg-surface);
  border-right: 1px solid var(--border);
  display: flex;
  flex-direction: column;
  z-index: 500;
  transition: transform var(--trans-base);
  backdrop-filter: blur(20px);
}

.sidebar-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 1.5rem 1.25rem 1rem;
  border-bottom: 1px solid var(--border);
}

.brand { display: flex; align-items: center; gap: 0.75rem; }
.brand-icon { width: 36px; height: 36px; flex-shrink: 0; }
.brand-name {
  font-size: 1rem;
  font-weight: 700;
  background: linear-gradient(135deg, var(--cyan), var(--purple-light));
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}
.brand-tag {
  display: block;
  font-size: 0.65rem;
  color: var(--text-muted);
  font-weight: 500;
  letter-spacing: 1.5px;
  text-transform: uppercase;
  margin-top: -2px;
}

.sidebar-close {
  background: none;
  border: none;
  color: var(--text-muted);
  font-size: 1rem;
  cursor: pointer;
  padding: 4px;
  border-radius: var(--radius-sm);
  transition: color var(--trans-fast);
  display: none;
}
.sidebar-close:hover { color: var(--text-primary); }

.sidebar-connection {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.75rem 1.25rem;
  border-bottom: 1px solid var(--border);
}
.conn-dot {
  width: 8px; height: 8px;
  border-radius: 50%;
  background: var(--text-muted);
  transition: background var(--trans-base);
}
.conn-dot.connected { background: var(--green); box-shadow: 0 0 8px var(--green); animation: connPulse 2s infinite; }
.conn-dot.connecting { background: var(--amber); animation: connPulse 0.7s infinite; }
@keyframes connPulse { 0%,100% { opacity: 1; } 50% { opacity: 0.4; } }

.conn-label { font-size: 0.75rem; color: var(--text-muted); font-weight: 500; }

/* ─── Nav Items ─ */
.sidebar-nav {
  flex: 1;
  padding: 1rem 0.75rem;
  display: flex;
  flex-direction: column;
  gap: 2px;
  overflow-y: auto;
}
.nav-item {
  display: flex;
  align-items: center;
  gap: 0.85rem;
  padding: 0.7rem 0.85rem;
  border-radius: var(--radius-md);
  color: var(--text-secondary);
  text-decoration: none;
  font-size: 0.88rem;
  font-weight: 500;
  transition: all var(--trans-fast);
  position: relative;
}
.nav-item:hover {
  background: var(--bg-hover);
  color: var(--text-primary);
}
.nav-item.active {
  background: var(--cyan-dim);
  color: var(--cyan);
  box-shadow: inset 0 0 0 1px var(--border-glow);
}
.nav-item.active::before {
  content: '';
  position: absolute;
  left: 0; top: 50%;
  transform: translateY(-50%);
  width: 3px; height: 60%;
  background: var(--cyan);
  border-radius: 0 2px 2px 0;
  box-shadow: 0 0 8px var(--cyan-glow);
}
.nav-icon { width: 18px; height: 18px; flex-shrink: 0; }
.nav-icon svg { width: 100%; height: 100%; }

.sidebar-footer {
  padding: 1rem 1.25rem;
  border-top: 1px solid var(--border);
}
.hw-badge {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  font-size: 0.72rem;
  color: var(--text-muted);
}
.hw-dot {
  width: 6px; height: 6px;
  border-radius: 50%;
  background: var(--purple-light);
  box-shadow: 0 0 6px var(--purple);
}

.sidebar-overlay {
  position: fixed;
  inset: 0;
  background: rgba(0,0,0,0.5);
  z-index: 499;
  display: none;
  backdrop-filter: blur(2px);
}

/* ═══ MAIN CONTENT ══════════════════════════════════════════════════════ */
.main-content {
  margin-left: var(--sidebar-width);
  flex: 1;
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}

/* ─── Top Bar ─── */
.topbar {
  position: sticky; top: 0;
  height: var(--topbar-h);
  background: rgba(8,12,24,0.85);
  backdrop-filter: blur(20px);
  border-bottom: 1px solid var(--border);
  display: flex;
  align-items: center;
  gap: 1rem;
  padding: 0 1.5rem;
  z-index: 100;
}
.topbar-menu {
  display: none;
  background: none;
  border: none;
  color: var(--text-secondary);
  width: 36px; height: 36px;
  cursor: pointer;
  border-radius: var(--radius-sm);
  transition: color var(--trans-fast);
}
.topbar-menu svg { width: 20px; height: 20px; }
.topbar-menu:hover { color: var(--text-primary); }

.topbar-title {
  font-size: 1rem;
  font-weight: 700;
  color: var(--text-primary);
  flex: 1;
}

.topbar-actions {
  display: flex;
  align-items: center;
  gap: 0.75rem;
}

.connection-badge {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.35rem 0.85rem;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: 99px;
  font-size: 0.75rem;
  font-weight: 500;
  color: var(--text-muted);
  cursor: pointer;
  transition: all var(--trans-fast);
}
.connection-badge.connected {
  border-color: rgba(34,197,94,0.3);
  color: var(--green);
  background: rgba(34,197,94,0.08);
}
.connection-badge.connecting {
  border-color: rgba(245,158,11,0.3);
  color: var(--amber);
  background: rgba(245,158,11,0.08);
}

.conn-pulse {
  width: 7px; height: 7px;
  border-radius: 50%;
  background: currentColor;
}
.connection-badge.connected .conn-pulse { animation: connPulse 2s infinite; }

.live-indicator {
  display: flex;
  align-items: center;
  gap: 0.4rem;
  padding: 0.25rem 0.7rem;
  background: rgba(239,68,68,0.15);
  border: 1px solid rgba(239,68,68,0.3);
  border-radius: 99px;
  font-size: 0.7rem;
  font-weight: 700;
  color: var(--red);
  letter-spacing: 1px;
}
.live-dot {
  width: 6px; height: 6px;
  border-radius: 50%;
  background: var(--red);
  animation: connPulse 0.8s infinite;
}

/* ═══ PAGES ════════════════════════════════════════════════════════════ */
.page {
  display: none;
  padding: 1.5rem;
  flex: 1;
  animation: pageIn 0.3s ease;
}
.page.active { display: block; }
@keyframes pageIn { from { opacity: 0; transform: translateY(8px); } to { opacity: 1; transform: none; } }

.page-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 1.5rem;
  flex-wrap: wrap;
  gap: 0.75rem;
}
.page-header h1 {
  font-size: 1.5rem;
  font-weight: 800;
  color: var(--text-primary);
}
.page-meta {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  font-size: 0.8rem;
  color: var(--text-muted);
}

.rate-badge {
  padding: 0.2rem 0.6rem;
  background: var(--cyan-dim);
  border: 1px solid var(--border-glow);
  border-radius: 99px;
  font-size: 0.7rem;
  color: var(--cyan);
  font-weight: 600;
  font-family: var(--font-mono);
}

/* ─── Patient Bar ── */
.patient-bar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1rem 1.25rem;
  margin-bottom: 1.5rem;
  gap: 1rem;
  flex-wrap: wrap;
}
.patient-info { display: flex; align-items: center; gap: 0.85rem; }
.patient-avatar {
  width: 44px; height: 44px;
  border-radius: 50%;
  background: linear-gradient(135deg, var(--cyan), var(--purple));
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: 700;
  font-size: 1.1rem;
  color: #fff;
  flex-shrink: 0;
}
.patient-name { display: block; font-weight: 600; font-size: 0.95rem; }
.patient-session { font-size: 0.78rem; color: var(--text-muted); }

/* ═══ SENSOR CARDS ══════════════════════════════════════════════════════ */
.cards-grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 1.25rem;
}

.sensor-card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1.25rem;
  transition: border-color var(--trans-base), box-shadow var(--trans-base);
  position: relative;
  overflow: hidden;
}
.sensor-card::before {
  content: '';
  position: absolute;
  top: 0; left: 0; right: 0;
  height: 2px;
  opacity: 0.7;
  border-radius: var(--radius-lg) var(--radius-lg) 0 0;
}
.card-accel::before { background: linear-gradient(90deg, var(--axis-x), var(--axis-y), var(--axis-z)); }
.card-gyro::before { background: linear-gradient(90deg, var(--orange), var(--purple-light), var(--teal)); }
.card-temp::before { background: linear-gradient(90deg, var(--cyan), var(--amber), var(--red)); }
.card-rom::before { background: linear-gradient(90deg, var(--cyan), var(--purple-light)); }

.sensor-card:hover {
  border-color: rgba(255,255,255,0.12);
  box-shadow: var(--shadow-card);
}

.card-header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  margin-bottom: 1.1rem;
}
.card-title-group { display: flex; align-items: center; gap: 0.75rem; }

.card-icon {
  width: 36px; height: 36px;
  border-radius: var(--radius-sm);
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}
.card-icon svg { width: 18px; height: 18px; }
.accel-icon { background: rgba(239,68,68,0.15); color: var(--axis-x); }
.gyro-icon { background: rgba(168,85,247,0.15); color: var(--purple-light); }
.temp-icon { background: rgba(245,158,11,0.15); color: var(--amber); }
.rom-icon { background: rgba(0,212,255,0.15); color: var(--cyan); }

.card-title { font-size: 0.9rem; font-weight: 700; }
.card-subtitle { font-size: 0.72rem; color: var(--text-muted); margin-top: 1px; }
.card-badge {
  font-size: 0.65rem;
  font-weight: 600;
  color: var(--text-muted);
  padding: 0.15rem 0.5rem;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: 99px;
  font-family: var(--font-mono);
}

/* ─── Axis Readings ─── */
.axes-grid { display: flex; flex-direction: column; gap: 0.55rem; margin-bottom: 0.85rem; }
.axis-reading {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}
.axis-label {
  font-size: 0.7rem;
  font-weight: 700;
  font-family: var(--font-mono);
  width: 14px;
  text-align: center;
  flex-shrink: 0;
}
.axis-x { color: var(--axis-x); }
.axis-y { color: var(--axis-y); }
.axis-z { color: var(--axis-z); }

.axis-value {
  font-size: 0.85rem;
  font-weight: 600;
  width: 60px;
  text-align: right;
  flex-shrink: 0;
}
.axis-unit {
  font-size: 0.65rem;
  color: var(--text-muted);
  width: 24px;
  flex-shrink: 0;
}
.axis-bar {
  flex: 1;
  height: 4px;
  background: rgba(255,255,255,0.06);
  border-radius: 99px;
  overflow: hidden;
}
.axis-fill {
  height: 100%;
  width: 50%;
  border-radius: 99px;
  transition: width 0.15s ease;
}
.ax-fill { background: var(--axis-x); box-shadow: 0 0 6px rgba(239,68,68,0.4); }
.ay-fill { background: var(--axis-y); box-shadow: 0 0 6px rgba(34,197,94,0.4); }
.az-fill { background: var(--axis-z); box-shadow: 0 0 6px rgba(59,130,246,0.4); }
.gx-fill { background: var(--orange); }
.gy-fill { background: var(--purple-light); }
.gz-fill { background: var(--teal); }

.card-magnitude {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0.5rem 0;
  border-top: 1px solid var(--border);
  margin-bottom: 0.5rem;
}
.mag-label { font-size: 0.72rem; color: var(--text-muted); }
.mag-value { font-size: 0.85rem; font-weight: 600; }

/* Mini Charts */
.mini-chart { display: block; margin-top: 0.5rem; border-radius: var(--radius-sm); max-height: 60px !important; width: 100% !important; }

/* ─── Temperature Card ─── */
.temp-display { display: flex; align-items: center; gap: 1rem; flex-wrap: wrap; }
.temp-gauge-wrap { position: relative; flex-shrink: 0; }
.temp-gauge { width: 160px; }
.temp-value-wrap {
  position: absolute;
  bottom: 4px; left: 50%;
  transform: translateX(-50%);
  text-align: center;
}
.temp-value { font-size: 1.6rem; font-weight: 700; }
.temp-unit { font-size: 0.75rem; color: var(--text-muted); margin-left: 2px; }

.temp-stats { display: flex; gap: 1.25rem; flex-wrap: wrap; }
.temp-stat { text-align: center; }
.ts-label { display: block; font-size: 0.65rem; color: var(--text-muted); margin-bottom: 2px; }
.ts-val { display: block; font-size: 0.85rem; font-weight: 600; }

/* ─── ROM Card ─── */
.rom-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 0.5rem; }
.rom-item { text-align: center; }
.rom-label { display: block; font-size: 0.68rem; color: var(--text-muted); margin-bottom: 0.25rem; }
.rom-arc-wrap { position: relative; display: inline-block; }
.rom-arc { width: 80px; display: block; }
.rom-value {
  position: absolute;
  bottom: 2px; left: 50%;
  transform: translateX(-50%);
  font-size: 0.8rem;
  font-weight: 600;
  white-space: nowrap;
}

.rom-alert {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  margin-top: 0.75rem;
  padding: 0.5rem 0.75rem;
  background: rgba(239,68,68,0.1);
  border: 1px solid rgba(239,68,68,0.3);
  border-radius: var(--radius-sm);
  font-size: 0.78rem;
  color: var(--red);
  animation: alertPulse 1.5s ease-in-out infinite;
}
.rom-alert svg { width: 16px; height: 16px; flex-shrink: 0; }
@keyframes alertPulse {
  0%, 100% { background: rgba(239,68,68,0.1); }
  50% { background: rgba(239,68,68,0.18); }
}

/* ═══ CHARTS SECTION ═══════════════════════════════════════════════════ */
.chart-section { display: grid; grid-template-columns: 1fr 1fr; gap: 1.25rem; }

.chart-card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1.25rem;
  transition: border-color var(--trans-fast);
}
.chart-card:hover { border-color: rgba(255,255,255,0.1); }
.chart-card-half { grid-column: span 1; }

.chart-card-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 1rem;
}
.chart-card-header h2 { font-size: 0.9rem; font-weight: 700; }

.chart-legend {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  font-size: 0.72rem;
  color: var(--text-muted);
}
.legend-dot {
  width: 8px; height: 8px;
  border-radius: 50%;
  flex-shrink: 0;
}

.chart-wrap { position: relative; height: 200px; }
.chart-wrap canvas { width: 100% !important; height: 100% !important; }

.time-select {
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  color: var(--text-secondary);
  font-size: 0.78rem;
  padding: 0.35rem 0.75rem;
  border-radius: var(--radius-sm);
  cursor: pointer;
  outline: none;
  font-family: var(--font-sans);
}

/* ═══ SESSION PAGE ══════════════════════════════════════════════════════ */
.session-layout { display: grid; grid-template-columns: 360px 1fr; gap: 1.25rem; }

.session-control-card,
.session-form-card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1.5rem;
  margin-bottom: 1rem;
}

.session-status-display {
  display: flex;
  align-items: center;
  gap: 1rem;
  margin-bottom: 1.5rem;
}
.session-icon-wrap {
  width: 52px; height: 52px;
  border-radius: 50%;
  background: var(--bg-elevated);
  border: 2px solid var(--border);
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}
.session-icon-wrap.recording {
  border-color: var(--red);
  background: rgba(239,68,68,0.1);
  animation: sessionPulse 1s infinite;
}
@keyframes sessionPulse {
  0%, 100% { box-shadow: 0 0 0 0 rgba(239,68,68,0.4); }
  50% { box-shadow: 0 0 0 8px rgba(239,68,68,0); }
}
.session-icon-wrap svg { width: 22px; height: 22px; }

.session-timer-display {
  text-align: center;
  padding: 1.25rem;
  background: var(--bg-elevated);
  border-radius: var(--radius-md);
  margin-bottom: 1.25rem;
}
.session-timer-label { display: block; font-size: 0.7rem; color: var(--text-muted); margin-bottom: 0.35rem; }
.session-timer {
  display: block;
  font-size: 2.5rem;
  font-weight: 700;
  letter-spacing: 4px;
  color: var(--cyan);
}
.session-timer.recording { color: var(--red); animation: timerFlash 1s ease-in-out infinite; }
@keyframes timerFlash { 0%,100%{ opacity:1; } 50%{ opacity:0.7; } }

.session-controls { display: flex; flex-direction: column; gap: 0.75rem; }
.form-title { font-size: 0.9rem; font-weight: 700; margin-bottom: 1rem; }

.session-stats-row {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 0.75rem;
  margin-bottom: 1rem;
}
.session-stat-card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-md);
  padding: 0.85rem;
  text-align: center;
}
.sstat-label { display: block; font-size: 0.65rem; color: var(--text-muted); margin-bottom: 0.3rem; }
.sstat-value { font-size: 0.88rem; font-weight: 600; font-family: var(--font-mono); }

/* ═══ HISTORY PAGE ══════════════════════════════════════════════════════ */
.empty-state {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 4rem 2rem;
  text-align: center;
  color: var(--text-muted);
}
.empty-icon { width: 64px; height: 64px; margin-bottom: 1.5rem; opacity: 0.3; }
.empty-icon svg { width: 100%; height: 100%; }
.empty-state h2 { font-size: 1.1rem; font-weight: 600; color: var(--text-secondary); margin-bottom: 0.5rem; }
.empty-state p { font-size: 0.85rem; }

.history-list { display: flex; flex-direction: column; gap: 0.85rem; }
.history-item {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1.1rem 1.25rem;
  display: flex;
  align-items: center;
  justify-content: space-between;
  cursor: pointer;
  transition: all var(--trans-fast);
  gap: 1rem;
  flex-wrap: wrap;
}
.history-item:hover {
  border-color: var(--border-glow);
  background: var(--bg-hover);
  box-shadow: 0 0 0 1px var(--border-glow);
}
.history-meta { display: flex; flex-direction: column; gap: 0.25rem; }
.history-patient { font-weight: 600; font-size: 0.9rem; }
.history-detail { font-size: 0.75rem; color: var(--text-muted); }
.history-stats { display: flex; gap: 1.5rem; flex-wrap: wrap; }
.h-stat { text-align: center; }
.h-stat-label { display: block; font-size: 0.62rem; color: var(--text-muted); }
.h-stat-val { font-size: 0.82rem; font-weight: 600; font-family: var(--font-mono); }
.history-actions { display: flex; gap: 0.5rem; }

/* ─── Modal ─── */
.modal-overlay {
  position: fixed;
  inset: 0;
  background: rgba(0,0,0,0.7);
  z-index: 1000;
  display: flex;
  align-items: flex-start;
  justify-content: center;
  padding: 1.5rem;
  overflow-y: auto;
  backdrop-filter: blur(4px);
}
.modal {
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: var(--radius-xl);
  width: 100%;
  max-width: 800px;
  box-shadow: var(--shadow-modal);
  animation: modalIn 0.3s ease;
}
@keyframes modalIn { from { opacity: 0; transform: scale(0.96) translateY(20px); } to { opacity: 1; transform: none; } }
.modal-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 1.25rem 1.5rem;
  border-bottom: 1px solid var(--border);
}
.modal-title { font-size: 1rem; font-weight: 700; }
.modal-close {
  background: none; border: none;
  color: var(--text-muted);
  font-size: 1rem; cursor: pointer;
  padding: 4px; border-radius: var(--radius-sm);
  transition: color var(--trans-fast);
}
.modal-close:hover { color: var(--text-primary); }
.modal-body { padding: 1.5rem; display: flex; flex-direction: column; gap: 1rem; }
.playback-meta { padding: 0.75rem; background: var(--bg-card); border-radius: var(--radius-md); font-size: 0.8rem; color: var(--text-muted); }

/* ═══ SETTINGS PAGE ══════════════════════════════════════════════════════ */
.settings-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 1.25rem; align-items: start; }
.settings-card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1.5rem;
}
.settings-title {
  font-size: 0.88rem;
  font-weight: 700;
  color: var(--text-primary);
  margin-bottom: 1.25rem;
  padding-bottom: 0.75rem;
  border-bottom: 1px solid var(--border);
}
.setting-row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 1rem;
  padding: 0.85rem 0;
  border-bottom: 1px solid var(--border);
  flex-wrap: wrap;
}
.setting-row:last-child { border-bottom: none; padding-bottom: 0; }
.setting-info { display: flex; flex-direction: column; gap: 3px; flex: 1; }
.setting-label { font-size: 0.85rem; font-weight: 500; }
.setting-desc { font-size: 0.72rem; color: var(--text-muted); }

/* Toggle group */
.toggle-group { display: flex; gap: 2px; background: var(--bg-elevated); border-radius: var(--radius-sm); padding: 2px; }
.toggle-btn {
  padding: 0.3rem 0.7rem;
  border: none;
  background: none;
  color: var(--text-muted);
  font-size: 0.78rem;
  font-weight: 600;
  cursor: pointer;
  border-radius: calc(var(--radius-sm) - 2px);
  transition: all var(--trans-fast);
  font-family: var(--font-sans);
}
.toggle-btn.active {
  background: var(--cyan-dim);
  color: var(--cyan);
  border: 1px solid var(--border-glow);
}

/* Switch */
.switch { position: relative; display: inline-block; width: 44px; height: 24px; flex-shrink: 0; }
.switch input { opacity: 0; width: 0; height: 0; }
.switch-slider {
  position: absolute; inset: 0;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: 12px;
  cursor: pointer;
  transition: all var(--trans-base);
}
.switch-slider::before {
  content: '';
  position: absolute;
  width: 18px; height: 18px;
  left: 2px; top: 2px;
  background: var(--text-muted);
  border-radius: 50%;
  transition: all var(--trans-base);
}
.switch input:checked + .switch-slider { background: var(--cyan-dim); border-color: var(--border-glow); }
.switch input:checked + .switch-slider::before {
  transform: translateX(20px);
  background: var(--cyan);
  box-shadow: 0 0 6px var(--cyan-glow);
}

/* Device Info */
.device-info-grid { display: flex; flex-direction: column; gap: 0.5rem; }
.dinfo-row { display: flex; justify-content: space-between; align-items: center; font-size: 0.8rem; }
.dinfo-label { color: var(--text-muted); }
.dinfo-val { font-size: 0.78rem; color: var(--cyan); }

/* ═══ ABOUT PAGE ════════════════════════════════════════════════════════ */
.about-layout { display: flex; flex-direction: column; gap: 1.5rem; max-width: 900px; }
.about-hero {
  text-align: center;
  padding: 2.5rem;
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-xl);
  background-image: radial-gradient(ellipse at 50% 0%, rgba(0,212,255,0.07) 0%, transparent 70%);
}
.about-logo { width: 80px; height: 80px; margin: 0 auto 1.25rem; }
.about-title {
  font-size: 2rem; font-weight: 800;
  background: linear-gradient(135deg, var(--cyan), var(--purple-light));
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  margin-bottom: 0.35rem;
}
.about-version { font-size: 0.78rem; color: var(--text-muted); margin-bottom: 1rem; }
.about-desc { font-size: 0.9rem; color: var(--text-secondary); line-height: 1.7; max-width: 560px; margin: 0 auto; }

.about-cards { display: grid; grid-template-columns: repeat(3, 1fr); gap: 1rem; }
.about-info-card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  border-radius: var(--radius-lg);
  padding: 1.25rem;
}
.about-info-card h3 {
  font-size: 0.82rem;
  font-weight: 700;
  color: var(--cyan);
  margin-bottom: 0.85rem;
  text-transform: uppercase;
  letter-spacing: 1px;
}
.about-info-card ul, .about-info-card ol { padding-left: 1rem; }
.about-info-card li { font-size: 0.8rem; color: var(--text-secondary); margin-bottom: 0.35rem; line-height: 1.5; }
.about-info-card .label { color: var(--text-muted); }
.about-info-card code {
  font-family: var(--font-mono);
  background: var(--bg-elevated);
  padding: 0.1em 0.4em;
  border-radius: 4px;
  font-size: 0.85em;
  color: var(--amber);
}
.about-footer { text-align: center; font-size: 0.8rem; color: var(--text-muted); }
.about-footer p { margin-bottom: 0.25rem; }
.about-footer strong { color: var(--text-secondary); }

/* ═══ FORM ELEMENTS ═════════════════════════════════════════════════════ */
.form-group { margin-bottom: 1rem; }
.form-label {
  display: block;
  font-size: 0.78rem;
  font-weight: 600;
  color: var(--text-secondary);
  margin-bottom: 0.4rem;
}
.form-input, .form-select, .form-textarea {
  width: 100%;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  color: var(--text-primary);
  font-size: 0.88rem;
  padding: 0.6rem 0.85rem;
  border-radius: var(--radius-sm);
  outline: none;
  transition: border-color var(--trans-fast), box-shadow var(--trans-fast);
  font-family: var(--font-sans);
}
.form-input:focus, .form-select:focus, .form-textarea:focus {
  border-color: var(--border-glow);
  box-shadow: 0 0 0 3px rgba(0,212,255,0.1);
}
.form-textarea { resize: vertical; }
.form-select { cursor: pointer; }
.form-select option { background: var(--bg-elevated); }

/* ═══ BUTTONS ═══════════════════════════════════════════════════════════ */
.btn-primary {
  display: inline-flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.65rem 1.25rem;
  background: linear-gradient(135deg, rgba(0,212,255,0.2), rgba(124,58,237,0.2));
  border: 1px solid var(--border-glow);
  color: var(--cyan);
  font-size: 0.85rem;
  font-weight: 600;
  border-radius: var(--radius-md);
  cursor: pointer;
  transition: all var(--trans-fast);
  font-family: var(--font-sans);
}
.btn-primary:hover {
  background: linear-gradient(135deg, rgba(0,212,255,0.3), rgba(124,58,237,0.3));
  box-shadow: 0 0 16px rgba(0,212,255,0.2);
}
.btn-primary svg { width: 16px; height: 16px; }

.btn-secondary {
  display: inline-flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.55rem 1rem;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  color: var(--text-secondary);
  font-size: 0.82rem;
  font-weight: 600;
  border-radius: var(--radius-md);
  cursor: pointer;
  transition: all var(--trans-fast);
  font-family: var(--font-sans);
}
.btn-secondary:hover { border-color: rgba(255,255,255,0.15); color: var(--text-primary); }

.btn-record {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.6rem;
  width: 100%;
  padding: 0.85rem;
  background: linear-gradient(135deg, rgba(34,197,94,0.15), rgba(0,212,255,0.1));
  border: 1px solid rgba(34,197,94,0.35);
  color: var(--green);
  font-size: 0.9rem;
  font-weight: 700;
  border-radius: var(--radius-md);
  cursor: pointer;
  transition: all var(--trans-fast);
  font-family: var(--font-sans);
}
.btn-record:hover { background: rgba(34,197,94,0.2); box-shadow: 0 0 16px rgba(34,197,94,0.2); }
.btn-record svg { width: 18px; height: 18px; }

.btn-stop {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.6rem;
  width: 100%;
  padding: 0.85rem;
  background: linear-gradient(135deg, rgba(239,68,68,0.15), rgba(245,158,11,0.1));
  border: 1px solid rgba(239,68,68,0.35);
  color: var(--red);
  font-size: 0.9rem;
  font-weight: 700;
  border-radius: var(--radius-md);
  cursor: pointer;
  transition: all var(--trans-fast);
  font-family: var(--font-sans);
  animation: stopPulse 1.5s ease-in-out infinite;
}
.btn-stop:hover { background: rgba(239,68,68,0.2); }
.btn-stop svg { width: 18px; height: 18px; }
@keyframes stopPulse { 0%,100%{ box-shadow: 0 0 0 0 rgba(239,68,68,0.3); } 50%{ box-shadow: 0 0 0 6px rgba(239,68,68,0); } }

.btn-icon {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  width: 32px; height: 32px;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  cursor: pointer;
  color: var(--text-muted);
  transition: all var(--trans-fast);
}
.btn-icon:hover { color: var(--text-primary); border-color: rgba(255,255,255,0.12); }
.btn-icon svg { width: 14px; height: 14px; }

/* ═══ TOAST NOTIFICATIONS ════════════════════════════════════════════════ */
.toast-container {
  position: fixed;
  bottom: 1.5rem;
  right: 1.5rem;
  z-index: 9000;
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  align-items: flex-end;
}
.toast {
  display: flex;
  align-items: center;
  gap: 0.6rem;
  padding: 0.7rem 1rem;
  background: var(--bg-elevated);
  border: 1px solid var(--border);
  border-radius: var(--radius-md);
  font-size: 0.82rem;
  font-weight: 500;
  color: var(--text-primary);
  box-shadow: var(--shadow-modal);
  max-width: 320px;
  animation: toastIn 0.3s ease;
  backdrop-filter: blur(12px);
}
.toast.success { border-color: rgba(34,197,94,0.35); }
.toast.error { border-color: rgba(239,68,68,0.35); }
.toast.info { border-color: var(--border-glow); }
@keyframes toastIn { from { opacity: 0; transform: translateX(20px); } to { opacity: 1; transform: none; } }

/* ═══ SCROLLBAR ══════════════════════════════════════════════════════════ */
::-webkit-scrollbar { width: 6px; height: 6px; }
::-webkit-scrollbar-track { background: transparent; }
::-webkit-scrollbar-thumb { background: rgba(255,255,255,0.08); border-radius: 3px; }
::-webkit-scrollbar-thumb:hover { background: rgba(255,255,255,0.15); }

/* ═══ RESPONSIVE ═════════════════════════════════════════════════════════ */
@media (max-width: 1100px) {
  .chart-section { grid-template-columns: 1fr; }
  .settings-grid { grid-template-columns: 1fr; }
  .about-cards { grid-template-columns: 1fr 1fr; }
}

@media (max-width: 900px) {
  .cards-grid { grid-template-columns: 1fr; }
  .session-layout { grid-template-columns: 1fr; }
  .session-stats-row { grid-template-columns: repeat(2, 1fr); }
}

@media (max-width: 768px) {
  :root { --sidebar-width: 0px; }
  .sidebar {
    width: 280px;
    transform: translateX(-100%);
    border-right-color: var(--border);
  }
  .sidebar.open {
    transform: translateX(0);
    box-shadow: var(--shadow-modal);
  }
  .sidebar-close { display: block; }
  .sidebar-overlay.active { display: block; }
  .main-content { margin-left: 0; }
  .topbar-menu { display: flex; align-items: center; justify-content: center; }
  .page { padding: 1rem; }
  .page-header h1 { font-size: 1.2rem; }
  .about-cards { grid-template-columns: 1fr; }
  .cards-grid { gap: 0.85rem; }
  .chart-section { grid-template-columns: 1fr; }
  .rom-grid { grid-template-columns: repeat(3, 1fr); }
  .session-stats-row { grid-template-columns: repeat(2, 1fr); }
}

@media (max-width: 480px) {
  .session-stats-row { grid-template-columns: 1fr 1fr; }
  .history-item { flex-direction: column; align-items: flex-start; }
  .history-stats { gap: 1rem; }
}
</style>
  <!-- Chart.js CDN -->
  <script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
</head>
<body>

<!-- ═══ SPLASH SCREEN ═══════════════════════════════════════════════════ -->
<div id="splash-screen" class="splash">
  <div class="splash-content">
    <div class="splash-logo">
      <div class="splash-icon">
        <svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg">
          <circle cx="32" cy="32" r="30" stroke="url(#splashGrad)" stroke-width="2"/>
          <path d="M12 32 L20 20 L28 36 L36 14 L44 28 L52 32" stroke="url(#splashGrad)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"/>
          <defs>
            <linearGradient id="splashGrad" x1="12" y1="14" x2="52" y2="50" gradientUnits="userSpaceOnUse">
              <stop offset="0%" stop-color="#00d4ff"/>
              <stop offset="100%" stop-color="#7c3aed"/>
            </linearGradient>
          </defs>
        </svg>
      </div>
      <h1 class="splash-title">PhysioPulse</h1>
      <p class="splash-subtitle">Physiotherapy Monitoring System</p>
      <p class="splash-team">MYOSA 6.0 Prototype</p>
    </div>
    <div class="splash-loader">
      <div class="loader-bar"><div class="loader-fill" id="loaderFill"></div></div>
      <p class="loader-text" id="loaderText">Initializing...</p>
    </div>
  </div>
</div>

<!-- ═══ APP SHELL ═══════════════════════════════════════════════════════ -->
<div id="app" class="app hidden">

  <!-- ─── SIDEBAR NAVIGATION ──────────────────────────────────────────── -->
  <aside class="sidebar" id="sidebar">
    <div class="sidebar-header">
      <div class="brand">
        <div class="brand-icon">
          <svg viewBox="0 0 40 40" fill="none">
            <circle cx="20" cy="20" r="18" stroke="url(#navGrad)" stroke-width="1.5"/>
            <path d="M8 20 L13 13 L18 22 L23 9 L28 17 L32 20" stroke="url(#navGrad)" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/>
            <defs>
              <linearGradient id="navGrad" x1="8" y1="9" x2="32" y2="31" gradientUnits="userSpaceOnUse">
                <stop offset="0%" stop-color="#00d4ff"/>
                <stop offset="100%" stop-color="#7c3aed"/>
              </linearGradient>
            </defs>
          </svg>
        </div>
        <div class="brand-text">
          <span class="brand-name">PhysioPulse</span>
          <span class="brand-tag">MYOSA 6.0</span>
        </div>
      </div>
      <button class="sidebar-close" id="sidebarClose" aria-label="Close menu">✕</button>
    </div>

    <!-- Connection Status in sidebar -->
    <div class="sidebar-connection">
      <div class="conn-dot" id="sidebarConnDot"></div>
      <span class="conn-label" id="sidebarConnLabel">Disconnected</span>
    </div>

    <nav class="sidebar-nav">
      <a href="#" class="nav-item active" data-page="dashboard" id="nav-dashboard">
        <span class="nav-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7" height="7" rx="1"/><rect x="14" y="3" width="7" height="7" rx="1"/><rect x="3" y="14" width="7" height="7" rx="1"/><rect x="14" y="14" width="7" height="7" rx="1"/></svg>
        </span>
        <span class="nav-label">Dashboard</span>
      </a>
      <a href="#" class="nav-item" data-page="live" id="nav-live">
        <span class="nav-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"/></svg>
        </span>
        <span class="nav-label">Live Charts</span>
      </a>
      <a href="#" class="nav-item" data-page="session" id="nav-session">
        <span class="nav-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg>
        </span>
        <span class="nav-label">Session</span>
      </a>
      <a href="#" class="nav-item" data-page="history" id="nav-history">
        <span class="nav-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/><line x1="16" y1="13" x2="8" y2="13"/><line x1="16" y1="17" x2="8" y2="17"/><polyline points="10 9 9 9 8 9"/></svg>
        </span>
        <span class="nav-label">History</span>
      </a>
      <a href="#" class="nav-item" data-page="settings" id="nav-settings">
        <span class="nav-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06A1.65 1.65 0 0 0 4.68 15a1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06A1.65 1.65 0 0 0 9 4.68a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06A1.65 1.65 0 0 0 19.4 9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
        </span>
        <span class="nav-label">Settings</span>
      </a>
      <a href="#" class="nav-item" data-page="about" id="nav-about">
        <span class="nav-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><line x1="12" y1="16" x2="12" y2="12"/><line x1="12" y1="8" x2="12.01" y2="8"/></svg>
        </span>
        <span class="nav-label">About</span>
      </a>
    </nav>

    <div class="sidebar-footer">
      <div class="hw-badge">
        <span class="hw-dot"></span>
        <span>ESP32 + MPU-6050</span>
      </div>
    </div>
  </aside>
  <div class="sidebar-overlay" id="sidebarOverlay"></div>

  <!-- ─── MAIN CONTENT ─────────────────────────────────────────────────── -->
  <main class="main-content">

    <!-- Top Bar -->
    <header class="topbar">
      <button class="topbar-menu" id="menuBtn" aria-label="Open menu">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
      </button>
      <div class="topbar-title" id="topbarTitle">Dashboard</div>
      <div class="topbar-actions">
        <div class="connection-badge" id="connBadge">
          <div class="conn-pulse" id="connPulse"></div>
          <span id="connStatus">Disconnected</span>
        </div>
        <div class="live-indicator hidden" id="liveIndicator">
          <span class="live-dot"></span>
          <span>LIVE</span>
        </div>
      </div>
    </header>

    <!-- ── PAGE: DASHBOARD ────────────────────────────────────────────── -->
    <section class="page active" id="page-dashboard">
      <div class="page-header">
        <h1>Patient Dashboard</h1>
        <div class="page-meta">
          <span id="dashTimestamp">—</span>
          <span class="rate-badge" id="rateBadge">0 Hz</span>
        </div>
      </div>

      <!-- Patient Info Bar -->
      <div class="patient-bar">
        <div class="patient-info">
          <div class="patient-avatar">P</div>
          <div class="patient-details">
            <span class="patient-name" id="patientName">Patient</span>
            <span class="patient-session" id="patientSession">No active session</span>
          </div>
        </div>
        <button class="btn-primary" id="quickStartBtn">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polygon points="10 8 16 12 10 16 10 8"/></svg>
          Start Session
        </button>
      </div>

      <!-- Sensor Cards Grid -->
      <div class="cards-grid">
        <!-- Accelerometer Card -->
        <div class="sensor-card card-accel">
          <div class="card-header">
            <div class="card-title-group">
              <div class="card-icon accel-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M5 12h14M12 5l7 7-7 7"/></svg>
              </div>
              <div>
                <h2 class="card-title">Accelerometer</h2>
                <p class="card-subtitle">Linear acceleration (g)</p>
              </div>
            </div>
            <div class="card-badge">±2g</div>
          </div>
          <div class="axes-grid">
            <div class="axis-reading">
              <span class="axis-label axis-x">X</span>
              <span class="axis-value mono" id="accelX">0.000</span>
              <span class="axis-unit">g</span>
              <div class="axis-bar"><div class="axis-fill ax-fill" id="accelXBar"></div></div>
            </div>
            <div class="axis-reading">
              <span class="axis-label axis-y">Y</span>
              <span class="axis-value mono" id="accelY">0.000</span>
              <span class="axis-unit">g</span>
              <div class="axis-bar"><div class="axis-fill ay-fill" id="accelYBar"></div></div>
            </div>
            <div class="axis-reading">
              <span class="axis-label axis-z">Z</span>
              <span class="axis-value mono" id="accelZ">0.000</span>
              <span class="axis-unit">g</span>
              <div class="axis-bar"><div class="axis-fill az-fill" id="accelZBar"></div></div>
            </div>
          </div>
          <div class="card-magnitude">
            <span class="mag-label">Magnitude</span>
            <span class="mag-value mono" id="accelMag">0.000 g</span>
          </div>
          <canvas class="mini-chart" id="miniAccelChart" height="60"></canvas>
        </div>

        <!-- Gyroscope Card -->
        <div class="sensor-card card-gyro">
          <div class="card-header">
            <div class="card-title-group">
              <div class="card-icon gyro-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="9"/><path d="M12 3a9 9 0 0 1 9 9"/><path d="M21 12a9 9 0 0 1-9 9"/><circle cx="12" cy="12" r="1" fill="currentColor"/></svg>
              </div>
              <div>
                <h2 class="card-title">Gyroscope</h2>
                <p class="card-subtitle">Angular velocity (°/s)</p>
              </div>
            </div>
            <div class="card-badge">±250°/s</div>
          </div>
          <div class="axes-grid">
            <div class="axis-reading">
              <span class="axis-label axis-x">X</span>
              <span class="axis-value mono" id="gyroX">0.000</span>
              <span class="axis-unit">°/s</span>
              <div class="axis-bar"><div class="axis-fill gx-fill" id="gyroXBar"></div></div>
            </div>
            <div class="axis-reading">
              <span class="axis-label axis-y">Y</span>
              <span class="axis-value mono" id="gyroY">0.000</span>
              <span class="axis-unit">°/s</span>
              <div class="axis-bar"><div class="axis-fill gy-fill" id="gyroYBar"></div></div>
            </div>
            <div class="axis-reading">
              <span class="axis-label axis-z">Z</span>
              <span class="axis-value mono" id="gyroZ">0.000</span>
              <span class="axis-unit">°/s</span>
              <div class="axis-bar"><div class="axis-fill gz-fill" id="gyroZBar"></div></div>
            </div>
          </div>
          <div class="card-magnitude">
            <span class="mag-label">Total Rate</span>
            <span class="mag-value mono" id="gyroMag">0.000 °/s</span>
          </div>
          <canvas class="mini-chart" id="miniGyroChart" height="60"></canvas>
        </div>

        <!-- Temperature Card -->
        <div class="sensor-card card-temp">
          <div class="card-header">
            <div class="card-title-group">
              <div class="card-icon temp-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"/></svg>
              </div>
              <div>
                <h2 class="card-title">Temperature</h2>
                <p class="card-subtitle">Sensor junction temp (°C)</p>
              </div>
            </div>
          </div>
          <div class="temp-display">
            <div class="temp-gauge-wrap">
              <svg class="temp-gauge" viewBox="0 0 160 100" id="tempGaugeSVG">
                <!-- Gauge arc background -->
                <path d="M 20 90 A 70 70 0 0 1 140 90" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="12" stroke-linecap="round"/>
                <!-- Gauge arc fill -->
                <path d="M 20 90 A 70 70 0 0 1 140 90" fill="none" stroke="url(#tempGrad)" stroke-width="12" stroke-linecap="round"
                  stroke-dasharray="220" stroke-dashoffset="220" id="tempGaugeFill"/>
                <!-- Needle -->
                <line x1="80" y1="90" x2="80" y2="30" stroke="#fff" stroke-width="2" stroke-linecap="round"
                  transform="rotate(0, 80, 90)" id="tempNeedle"/>
                <circle cx="80" cy="90" r="4" fill="#fff"/>
                <defs>
                  <linearGradient id="tempGrad" x1="20" y1="90" x2="140" y2="90" gradientUnits="userSpaceOnUse">
                    <stop offset="0%" stop-color="#00d4ff"/>
                    <stop offset="50%" stop-color="#f59e0b"/>
                    <stop offset="100%" stop-color="#ef4444"/>
                  </linearGradient>
                </defs>
              </svg>
              <div class="temp-value-wrap">
                <span class="temp-value mono" id="tempValue">--.-</span>
                <span class="temp-unit">°C</span>
              </div>
            </div>
            <div class="temp-stats">
              <div class="temp-stat">
                <span class="ts-label">Min</span>
                <span class="ts-val mono" id="tempMin">--.-</span>
              </div>
              <div class="temp-stat">
                <span class="ts-label">Max</span>
                <span class="ts-val mono" id="tempMax">--.-</span>
              </div>
              <div class="temp-stat">
                <span class="ts-label">Avg</span>
                <span class="ts-val mono" id="tempAvg">--.-</span>
              </div>
            </div>
          </div>
          <canvas class="mini-chart" id="miniTempChart" height="60"></canvas>
        </div>

        <!-- ROM Card (Range of Motion) -->
        <div class="sensor-card card-rom">
          <div class="card-header">
            <div class="card-title-group">
              <div class="card-icon rom-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 22V12"/><path d="m15 19-3 3-3-3"/><path d="M20 12a8 8 0 1 0-16 0"/></svg>
              </div>
              <div>
                <h2 class="card-title">Range of Motion</h2>
                <p class="card-subtitle">Computed joint angles</p>
              </div>
            </div>
          </div>
          <div class="rom-grid">
            <div class="rom-item">
              <span class="rom-label">Pitch (X)</span>
              <div class="rom-arc-wrap">
                <svg class="rom-arc" viewBox="0 0 80 50">
                  <path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="8"/>
                  <path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="#00d4ff" stroke-width="8"
                    stroke-dasharray="116" stroke-dashoffset="116" id="pitchArc"/>
                </svg>
                <span class="rom-value mono" id="pitchVal">0°</span>
              </div>
            </div>
            <div class="rom-item">
              <span class="rom-label">Roll (Y)</span>
              <div class="rom-arc-wrap">
                <svg class="rom-arc" viewBox="0 0 80 50">
                  <path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="8"/>
                  <path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="#a855f7" stroke-width="8"
                    stroke-dasharray="116" stroke-dashoffset="116" id="rollArc"/>
                </svg>
                <span class="rom-value mono" id="rollVal">0°</span>
              </div>
            </div>
            <div class="rom-item">
              <span class="rom-label">Yaw (Z)</span>
              <div class="rom-arc-wrap">
                <svg class="rom-arc" viewBox="0 0 80 50">
                  <path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="8"/>
                  <path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="#f59e0b" stroke-width="8"
                    stroke-dasharray="116" stroke-dashoffset="116" id="yawArc"/>
                </svg>
                <span class="rom-value mono" id="yawVal">0°</span>
              </div>
            </div>
          </div>
          <div class="rom-alert hidden" id="romAlert">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
            <span id="romAlertText">Motion limit exceeded</span>
          </div>
        </div>
      </div>
    </section>

    <!-- ── PAGE: LIVE CHARTS ──────────────────────────────────────────── -->
    <section class="page" id="page-live">
      <div class="page-header">
        <h1>Live Charts</h1>
        <div class="page-meta">
          <select class="time-select" id="timeWindow">
            <option value="10">10s window</option>
            <option value="30" selected>30s window</option>
            <option value="60">60s window</option>
          </select>
        </div>
      </div>

      <div class="chart-section">
        <div class="chart-card">
          <div class="chart-card-header">
            <h2>Accelerometer (g)</h2>
            <div class="chart-legend">
              <span class="legend-dot" style="background:#ef4444"></span><span>X</span>
              <span class="legend-dot" style="background:#22c55e"></span><span>Y</span>
              <span class="legend-dot" style="background:#3b82f6"></span><span>Z</span>
            </div>
          </div>
          <div class="chart-wrap"><canvas id="accelChart"></canvas></div>
        </div>

        <div class="chart-card">
          <div class="chart-card-header">
            <h2>Gyroscope (°/s)</h2>
            <div class="chart-legend">
              <span class="legend-dot" style="background:#f97316"></span><span>X</span>
              <span class="legend-dot" style="background:#a855f7"></span><span>Y</span>
              <span class="legend-dot" style="background:#06b6d4"></span><span>Z</span>
            </div>
          </div>
          <div class="chart-wrap"><canvas id="gyroChart"></canvas></div>
        </div>

        <div class="chart-card chart-card-half">
          <div class="chart-card-header">
            <h2>Temperature (°C)</h2>
          </div>
          <div class="chart-wrap"><canvas id="tempChart"></canvas></div>
        </div>

        <div class="chart-card chart-card-half">
          <div class="chart-card-header">
            <h2>Range of Motion (°)</h2>
            <div class="chart-legend">
              <span class="legend-dot" style="background:#00d4ff"></span><span>Pitch</span>
              <span class="legend-dot" style="background:#a855f7"></span><span>Roll</span>
              <span class="legend-dot" style="background:#f59e0b"></span><span>Yaw</span>
            </div>
          </div>
          <div class="chart-wrap"><canvas id="romChart"></canvas></div>
        </div>
      </div>
    </section>

    <!-- ── PAGE: SESSION ─────────────────────────────────────────────── -->
    <section class="page" id="page-session">
      <div class="page-header">
        <h1>Session Recording</h1>
      </div>

      <div class="session-layout">
        <div class="session-panel">
          <div class="session-control-card">
            <div class="session-status-display">
              <div class="session-icon-wrap" id="sessionIconWrap">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" id="sessionIcon"><circle cx="12" cy="12" r="10"/><polygon points="10 8 16 12 10 16 10 8"/></svg>
              </div>
              <div class="session-info">
                <h2 id="sessionStateLabel">Ready to Record</h2>
                <p id="sessionStateDesc">Press Start to begin monitoring patient session</p>
              </div>
            </div>
            <div class="session-timer-display">
              <span class="session-timer-label">Session Duration</span>
              <span class="session-timer mono" id="sessionTimer">00:00:00</span>
            </div>
            <div class="session-controls">
              <button class="btn-record" id="recordBtn">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polygon points="10 8 16 12 10 16 10 8"/></svg>
                Start Recording
              </button>
              <button class="btn-stop hidden" id="stopBtn">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><rect x="9" y="9" width="6" height="6"/></svg>
                Stop & Save
              </button>
            </div>
          </div>

          <!-- Patient Details Form -->
          <div class="session-form-card">
            <h3 class="form-title">Patient Details</h3>
            <div class="form-group">
              <label class="form-label" for="patientNameInput">Patient Name / ID</label>
              <input type="text" class="form-input" id="patientNameInput" placeholder="e.g., Patient #001 or John D." />
            </div>
            <div class="form-group">
              <label class="form-label" for="exerciseType">Exercise Type</label>
              <select class="form-select" id="exerciseType">
                <option>Knee Flexion / Extension</option>
                <option>Shoulder Abduction</option>
                <option>Wrist Rotation</option>
                <option>Ankle Dorsiflexion</option>
                <option>Hip Rotation</option>
                <option>Spinal Flexion</option>
                <option>Custom Exercise</option>
              </select>
            </div>
            <div class="form-group">
              <label class="form-label" for="sessionNotes">Notes</label>
              <textarea class="form-textarea" id="sessionNotes" rows="3" placeholder="Clinical observations..."></textarea>
            </div>
          </div>
        </div>

        <!-- Session Live Mini Charts -->
        <div class="session-charts-panel">
          <div class="session-stats-row">
            <div class="session-stat-card">
              <span class="sstat-label">Data Points</span>
              <span class="sstat-value mono" id="sDataPoints">0</span>
            </div>
            <div class="session-stat-card">
              <span class="sstat-label">Max Accel</span>
              <span class="sstat-value mono" id="sMaxAccel">0.00 g</span>
            </div>
            <div class="session-stat-card">
              <span class="sstat-label">Max Gyro</span>
              <span class="sstat-value mono" id="sMaxGyro">0.0 °/s</span>
            </div>
            <div class="session-stat-card">
              <span class="sstat-label">Avg Temp</span>
              <span class="sstat-value mono" id="sAvgTemp">--.- °C</span>
            </div>
          </div>
          <div class="chart-card">
            <div class="chart-card-header"><h2>Session Accelerometer</h2></div>
            <div class="chart-wrap"><canvas id="sessionAccelChart"></canvas></div>
          </div>
          <div class="chart-card">
            <div class="chart-card-header"><h2>Session Gyroscope</h2></div>
            <div class="chart-wrap"><canvas id="sessionGyroChart"></canvas></div>
          </div>
        </div>
      </div>
    </section>

    <!-- ── PAGE: HISTORY ─────────────────────────────────────────────── -->
    <section class="page" id="page-history">
      <div class="page-header">
        <h1>Session History</h1>
        <div class="page-meta">
          <button class="btn-secondary" id="clearHistoryBtn">Clear All</button>
        </div>
      </div>

      <div id="historyEmpty" class="empty-state">
        <div class="empty-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/></svg>
        </div>
        <h2>No sessions recorded yet</h2>
        <p>Start a session from the Session tab to see history here.</p>
      </div>

      <div class="history-list" id="historyList"></div>

      <!-- Session Playback Modal -->
      <div class="modal-overlay hidden" id="playbackModal">
        <div class="modal">
          <div class="modal-header">
            <h2 class="modal-title" id="playbackTitle">Session Playback</h2>
            <button class="modal-close" id="playbackClose">✕</button>
          </div>
          <div class="modal-body">
            <div class="playback-meta" id="playbackMeta"></div>
            <div class="chart-card">
              <div class="chart-card-header"><h2>Accelerometer</h2></div>
              <div class="chart-wrap"><canvas id="playbackAccelChart"></canvas></div>
            </div>
            <div class="chart-card">
              <div class="chart-card-header"><h2>Gyroscope</h2></div>
              <div class="chart-wrap"><canvas id="playbackGyroChart"></canvas></div>
            </div>
            <div class="chart-card">
              <div class="chart-card-header"><h2>Temperature</h2></div>
              <div class="chart-wrap"><canvas id="playbackTempChart"></canvas></div>
            </div>
            <button class="btn-primary" id="downloadCsvBtn" style="width:100%;margin-top:1rem;">
              ⬇ Download CSV
            </button>
          </div>
        </div>
      </div>
    </section>

    <!-- ── PAGE: SETTINGS ────────────────────────────────────────────── -->
    <section class="page" id="page-settings">
      <div class="page-header">
        <h1>Settings</h1>
      </div>

      <div class="settings-grid">
        <!-- Connection Settings -->
        <div class="settings-card">
          <h2 class="settings-title">Connection</h2>
          <div class="setting-row">
            <div class="setting-info">
              <span class="setting-label">Connection Mode</span>
              <span class="setting-desc">WiFi WebSocket or Web Bluetooth BLE</span>
            </div>
            <div class="toggle-group" id="connModeToggle">
              <button class="toggle-btn active" data-mode="wifi">WiFi</button>
              <button class="toggle-btn" data-mode="ble">BLE</button>
            </div>
          </div>
          <div class="setting-row" id="wifiSettings">
            <div class="setting-info">
              <span class="setting-label">WebSocket URL</span>
              <span class="setting-desc">ws://192.168.4.1:81 (default AP)</span>
            </div>
            <input type="text" class="form-input" id="wsUrl" value="ws://192.168.4.1:81" style="max-width:240px" />
          </div>
          <div class="setting-row">
            <div class="setting-info">
              <span class="setting-label">Auto-Reconnect</span>
              <span class="setting-desc">Reconnect on connection loss</span>
            </div>
            <label class="switch">
              <input type="checkbox" id="autoReconnect" checked />
              <span class="switch-slider"></span>
            </label>
          </div>
          <div class="setting-row">
            <button class="btn-primary" id="connectBtn" style="width:100%">Connect to ESP32</button>
          </div>
          <div class="setting-row">
            <button class="btn-secondary" id="calibrateBtn" style="width:100%">Recalibrate Gyroscope</button>
          </div>
        </div>

        <!-- Display Settings -->
        <div class="settings-card">
          <h2 class="settings-title">Display</h2>
          <div class="setting-row">
            <div class="setting-info">
              <span class="setting-label">Chart Buffer Size</span>
              <span class="setting-desc">Data points to display in charts</span>
            </div>
            <select class="form-select" id="bufferSize" style="max-width:120px">
              <option value="100">100 pts</option>
              <option value="200" selected>200 pts</option>
              <option value="500">500 pts</option>
            </select>
          </div>
          <div class="setting-row">
            <div class="setting-info">
              <span class="setting-label">Motion Alert Threshold</span>
              <span class="setting-desc">Gyro magnitude to trigger alert (°/s)</span>
            </div>
            <input type="number" class="form-input" id="motionThreshold" value="100" min="10" max="250" style="max-width:100px"/>
          </div>
          <div class="setting-row">
            <div class="setting-info">
              <span class="setting-label">Temperature Alert</span>
              <span class="setting-desc">Alert above this temp (°C)</span>
            </div>
            <input type="number" class="form-input" id="tempThreshold" value="40" min="30" max="50" style="max-width:100px"/>
          </div>
        </div>

        <!-- Device Info -->
        <div class="settings-card" id="deviceInfoCard">
          <h2 class="settings-title">Device Information</h2>
          <div class="device-info-grid">
            <div class="dinfo-row"><span class="dinfo-label">Device</span><span class="dinfo-val mono" id="dDevice">—</span></div>
            <div class="dinfo-row"><span class="dinfo-label">Firmware</span><span class="dinfo-val mono" id="dFirmware">—</span></div>
            <div class="dinfo-row"><span class="dinfo-label">MAC Address</span><span class="dinfo-val mono" id="dMAC">—</span></div>
            <div class="dinfo-row"><span class="dinfo-label">AP IP</span><span class="dinfo-val mono" id="dIP">—</span></div>
            <div class="dinfo-row"><span class="dinfo-label">Sensor</span><span class="dinfo-val mono" id="dSensor">—</span></div>
            <div class="dinfo-row"><span class="dinfo-label">Uptime</span><span class="dinfo-val mono" id="dUptime">—</span></div>
            <div class="dinfo-row"><span class="dinfo-label">Free Heap</span><span class="dinfo-val mono" id="dHeap">—</span></div>
          </div>
          <button class="btn-secondary" id="refreshDeviceInfo" style="width:100%;margin-top:1rem">Refresh Info</button>
        </div>
      </div>
    </section>

    <!-- ── PAGE: ABOUT ────────────────────────────────────────────────── -->
    <section class="page" id="page-about">
      <div class="page-header">
        <h1>About PhysioPulse</h1>
      </div>
      <div class="about-layout">
        <div class="about-hero">
          <div class="about-logo">
            <svg viewBox="0 0 80 80" fill="none">
              <circle cx="40" cy="40" r="38" stroke="url(#aboutGrad)" stroke-width="2"/>
              <path d="M16 40 L25 26 L34 44 L43 18 L52 33 L64 40" stroke="url(#aboutGrad)" stroke-width="3.5" stroke-linecap="round" stroke-linejoin="round"/>
              <defs>
                <linearGradient id="aboutGrad" x1="16" y1="18" x2="64" y2="62" gradientUnits="userSpaceOnUse">
                  <stop offset="0%" stop-color="#00d4ff"/>
                  <stop offset="100%" stop-color="#7c3aed"/>
                </linearGradient>
              </defs>
            </svg>
          </div>
          <h2 class="about-title">PhysioPulse</h2>
          <p class="about-version">v1.0.0 · MYOSA 6.0 Prototype</p>
          <p class="about-desc">Real-time physiotherapy monitoring system using MYOSA ESP32 hardware and MPU-6050 (GY-521) sensor for accelerometer, gyroscope, and temperature data visualization.</p>
        </div>

        <div class="about-cards">
          <div class="about-info-card">
            <h3>Hardware</h3>
            <ul>
              <li><span class="label">MCU:</span> MYOSA ESP32-WROVER (WiFi + BLE)</li>
              <li><span class="label">Sensor:</span> MYOSA MPU-6050 / GY-521</li>
              <li><span class="label">I2C Address:</span> 0x69</li>
              <li><span class="label">Axes:</span> Accel 3-axis ± 2g, Gyro 3-axis ± 250°/s</li>
              <li><span class="label">Sample Rate:</span> 20 Hz (configurable)</li>
            </ul>
          </div>
          <div class="about-info-card">
            <h3>Technology</h3>
            <ul>
              <li><span class="label">Frontend:</span> Vanilla HTML5 + CSS3 + JavaScript</li>
              <li><span class="label">Charts:</span> Chart.js v4</li>
              <li><span class="label">Protocol:</span> WebSocket (WiFi) + Web BLE</li>
              <li><span class="label">Storage:</span> IndexedDB (session history)</li>
              <li><span class="label">PWA:</span> Installable on any device</li>
            </ul>
          </div>
          <div class="about-info-card">
            <h3>Connection Guide</h3>
            <ol>
              <li>Power on the MYOSA ESP32 board</li>
              <li>Connect your phone/PC WiFi to <strong>PhysioPulse-XXXX</strong></li>
              <li>Password: <code>physio123</code></li>
              <li>Open browser → <strong>http://192.168.4.1</strong></li>
              <li>Or go to Settings and press <strong>Connect</strong></li>
            </ol>
          </div>
        </div>

        <div class="about-footer">
          <p>Built for <strong>MYOSA 6.0 Competition</strong> – Stage 2 Working Prototype</p>
          <p>Using MakeSense EduTech MYOSA hardware platform</p>
        </div>
      </div>
    </section>

  </main>
</div>

<!-- ═══ TOAST NOTIFICATIONS ═══════════════════════════════════════════════ -->
<div class="toast-container" id="toastContainer"></div>

<!-- ═══ SCRIPTS ════════════════════════════════════════════════════════════ -->
<script>/**
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

  // BLE UUIDs (must match firmware)
  const BLE_SERVICE_UUID    = '4fafc201-1fb5-459e-8fcc-c5c9c331914b';
  const BLE_CHAR_JSON_UUID  = 'beb5483e-36e1-4688-b7f5-ea07361b26a4';
  const BLE_CHAR_ACCEL_UUID = 'beb5483e-36e1-4688-b7f5-ea07361b26a1';
  const BLE_CHAR_GYRO_UUID  = 'beb5483e-36e1-4688-b7f5-ea07361b26a2';
  const BLE_CHAR_TEMP_UUID  = 'beb5483e-36e1-4688-b7f5-ea07361b26a3';

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

  // ─── BLE Connection ───────────────────────────────────────────────────────
  async function connectBLE() {
    if (!navigator.bluetooth) {
      App.showToast('Web Bluetooth not supported on this browser/device', 'error');
      return;
    }
    setStatus('connecting');
    try {
      bleDevice = await navigator.bluetooth.requestDevice({
        filters: [
          { namePrefix: 'PhysioPulse' },
          { services: [BLE_SERVICE_UUID] }
        ],
        optionalServices: [BLE_SERVICE_UUID]
      });

      bleDevice.addEventListener('gattserverdisconnected', _onBLEDisconnect);

      bleServer = await bleDevice.gatt.connect();
      bleService = await bleServer.getPrimaryService(BLE_SERVICE_UUID);

      // Subscribe to JSON characteristic (easiest way - single notify)
      const charJSON = await bleService.getCharacteristic(BLE_CHAR_JSON_UUID);
      await charJSON.startNotifications();
      charJSON.addEventListener('characteristicvaluechanged', (evt) => {
        const decoder = new TextDecoder();
        const json = decoder.decode(evt.target.value);
        try {
          const data = JSON.parse(json);
          emit('data', data);
        } catch (e) {}
      });

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

  return { on, connect, disconnect, send, calibrate, fetchDeviceInfo, getStatus, getMode };
})();
</script>
<script>/**
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
  function init() {
    applyDefaults();

    // ── Mini Charts (Dashboard Cards) ──
    charts.miniAccel = createMiniChart('miniAccelChart', [C.axisX, C.axisY, C.axisZ]);
    charts.miniGyro  = createMiniChart('miniGyroChart', [C.gyroX, C.gyroY, C.gyroZ]);
    charts.miniTemp  = createMiniChart('miniTempChart', [C.amber], true);

    // ── Live Charts ──
    charts.accel = new Chart(document.getElementById('accelChart'), {
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

    charts.gyro = new Chart(document.getElementById('gyroChart'), {
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

    charts.temp = new Chart(document.getElementById('tempChart'), {
      type: 'line',
      data: {
        labels: [],
        datasets: [ds('Temperature', C.amber, true)]
      },
      options: baseLineOptions('°C')
    });

    charts.rom = new Chart(document.getElementById('romChart'), {
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
    charts.sessionAccel = new Chart(document.getElementById('sessionAccelChart'), {
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

    charts.sessionGyro = new Chart(document.getElementById('sessionGyroChart'), {
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
    addPoint(charts.miniTemp, label, tp);

    // Live charts
    addPoint(charts.accel, label, ax, ay, az);
    addPoint(charts.gyro, label, gx, gy, gz);
    addPoint(charts.temp, label, tp);
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

    new Chart(document.getElementById('playbackAccelChart'), {
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

    new Chart(document.getElementById('playbackGyroChart'), {
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

    new Chart(document.getElementById('playbackTempChart'), {
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
</script>
<script>/**
 * PhysioPulse – Session Manager
 * Handles recording, IndexedDB storage, history, and CSV export
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

  function saveSession(session) {
    return new Promise((resolve, reject) => {
      const tx = db.transaction(STORE_NAME, 'readwrite');
      const store = tx.objectStore(STORE_NAME);
      const req = store.add(session);
      req.onsuccess = () => resolve(req.result);
      req.onerror = () => reject(req.error);
    });
  }

  function getAllSessions() {
    return new Promise((resolve, reject) => {
      const tx = db.transaction(STORE_NAME, 'readonly');
      const store = tx.objectStore(STORE_NAME);
      const req = store.getAll();
      req.onsuccess = () => resolve(req.result.reverse()); // newest first
      req.onerror = () => reject(req.error);
    });
  }

  function deleteAllSessions() {
    return new Promise((resolve, reject) => {
      const tx = db.transaction(STORE_NAME, 'readwrite');
      const store = tx.objectStore(STORE_NAME);
      const req = store.clear();
      req.onsuccess = () => resolve();
      req.onerror = () => reject(req.error);
    });
  }

  function deleteSession(id) {
    return new Promise((resolve, reject) => {
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

    // Get patient details from form
    const patientName = document.getElementById('patientNameInput')?.value.trim() || 'Unknown';
    const exercise    = document.getElementById('exerciseType')?.value || 'Unknown';
    const notes       = document.getElementById('sessionNotes')?.value.trim() || '';

    currentSession = {
      patient: patientName,
      exercise,
      notes,
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

    // Save to DB
    saveSession(currentSession)
      .then(id => {
        console.log('[Session] Saved with ID:', id);
        App.showToast(`Session saved! (${currentSession.stats.dataPoints} points)`, 'success');
        // Refresh history list
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
      const maxA = s.stats?.maxAccelMag?.toFixed(3) ?? '—';
      const maxG = s.stats?.maxGyroMag?.toFixed(1) ?? '—';

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
      const all = await getAllSessions();
      session = all.find(s => s.id === id);
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
        Points: ${session.rows?.length ?? 0} &nbsp;|&nbsp;
        Max Accel: ${session.stats?.maxAccelMag ?? '—'} g &nbsp;|&nbsp;
        Avg Temp: ${session.stats?.avgTemp ?? '—'} °C
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
      const all = await getAllSessions();
      session = all.find(s => s.id === id);
    } catch (e) { return; }
    if (session) _doDownloadCSV(session);
  }

  function _doDownloadCSV(session) {
    const header = 'timestamp_ms,accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,gyro_z_dps,temperature_c\n';
    const rows = session.rows.map((r, i) => {
      const t = r.t || (i * (session.interval_ms || 50));
      return `${t},${r.ax},${r.ay},${r.az},${r.gx},${r.gy},${r.gz},${r.tp}`;
    }).join('\n');

    const blob = new Blob([
      `# PhysioPulse Session Export\n`,
      `# Patient: ${session.patient}\n`,
      `# Exercise: ${session.exercise}\n`,
      `# Date: ${session.date}\n`,
      `# Notes: ${session.notes}\n`,
      header,
      rows
    ], { type: 'text/csv' });

    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `PhysioPulse_${session.patient.replace(/\s/g,'_')}_${new Date(session.date).toISOString().slice(0,10)}.csv`;
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
      console.log('[Session] IndexedDB ready');
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
</script>
<script>/**
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
    // SW removed for single-file version
      .then(reg => console.log('[PWA] Service worker registered:', reg.scope))
      .catch(err => console.warn('[PWA] SW registration failed:', err));
  });
}
</script>
</body>
</html>
)rawliteral";
