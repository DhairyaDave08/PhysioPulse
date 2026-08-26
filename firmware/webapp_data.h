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


/* ═══════════════════════════════════════════════════════════════════════════
   AUTH SYSTEM STYLES
   ═══════════════════════════════════════════════════════════════════════════ */

.auth-screen {
  position: fixed;
  inset: 0;
  background: var(--bg-base);
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 1.5rem;
  z-index: 100;
  background-image:
    radial-gradient(ellipse 60% 50% at 20% 30%, rgba(0,212,255,0.08) 0%, transparent 60%),
    radial-gradient(ellipse 50% 60% at 80% 70%, rgba(124,58,237,0.10) 0%, transparent 60%);
}

.auth-card {
  background: rgba(14, 21, 37, 0.92);
  backdrop-filter: blur(20px);
  border: 1px solid rgba(0,212,255,0.15);
  border-radius: 24px;
  padding: 2.5rem 2rem;
  width: 100%;
  max-width: 440px;
  box-shadow: 0 0 0 1px rgba(255,255,255,0.04), 0 24px 64px rgba(0,0,0,0.6), 0 0 80px rgba(0,212,255,0.06);
  animation: slideUp 0.5s cubic-bezier(0.22, 0.61, 0.36, 1) both;
}

@keyframes slideUp {
  from { opacity: 0; transform: translateY(28px) scale(0.97); }
  to   { opacity: 1; transform: translateY(0) scale(1); }
}

.auth-logo {
  display: flex;
  align-items: center;
  gap: 0.85rem;
  margin-bottom: 2rem;
}
.auth-logo svg { width: 48px; height: 48px; flex-shrink: 0; }
.auth-brand { font-size: 1.5rem; font-weight: 800; background: linear-gradient(135deg, var(--cyan), var(--purple-light)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; background-clip: text; line-height: 1.1; }
.auth-brand-sub { font-size: 0.72rem; color: var(--text-muted); letter-spacing: 0.1em; text-transform: uppercase; margin-top: 0.2rem; }

.auth-title { font-size: 1.5rem; font-weight: 700; color: var(--text-primary); margin-bottom: 0.35rem; }
.auth-subtitle { font-size: 0.9rem; color: var(--text-secondary); margin-bottom: 1.75rem; }

.auth-error {
  background: rgba(239,68,68,0.12);
  border: 1px solid rgba(239,68,68,0.3);
  border-radius: 10px;
  padding: 0.75rem 1rem;
  font-size: 0.875rem;
  color: #fca5a5;
  margin-bottom: 1.25rem;
}

.auth-submit { width: 100%; padding: 0.875rem; font-size: 1rem; margin-top: 0.5rem; display: flex; align-items: center; justify-content: center; gap: 0.5rem; }

.auth-switch { text-align: center; margin-top: 1.25rem; font-size: 0.875rem; color: var(--text-secondary); }
.auth-switch a { color: var(--cyan); text-decoration: none; font-weight: 600; }
.auth-switch a:hover { text-decoration: underline; }

.role-selector { display: flex; gap: 0.75rem; margin-bottom: 1.75rem; }
.role-btn {
  flex: 1; display: flex; align-items: center; justify-content: center; gap: 0.5rem;
  padding: 0.75rem 1rem; border-radius: 12px; border: 1.5px solid var(--border);
  background: var(--bg-elevated); color: var(--text-secondary); font-size: 0.9rem; font-weight: 600;
  cursor: pointer; transition: all 0.2s ease;
}
.role-btn svg { width: 18px; height: 18px; }
.role-btn:hover { border-color: rgba(0,212,255,0.3); color: var(--text-primary); }
.role-btn.active { border-color: var(--cyan); background: rgba(0,212,255,0.1); color: var(--cyan); box-shadow: 0 0 20px rgba(0,212,255,0.15); }

.btn-loader { display: inline-block; animation: spin 0.8s linear infinite; }
@keyframes spin { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }

/* ─── Sidebar User ───────────────────────────────────────────────────────── */
.sidebar-user { display: flex; align-items: center; gap: 0.75rem; padding: 0.75rem 1.25rem; margin: 0.5rem 0; background: rgba(255,255,255,0.04); border-top: 1px solid var(--border); border-bottom: 1px solid var(--border); }
.sidebar-user-avatar { width: 36px; height: 36px; border-radius: 50%; background: linear-gradient(135deg, var(--cyan), var(--purple)); display: flex; align-items: center; justify-content: center; font-weight: 700; font-size: 0.9rem; color: #fff; flex-shrink: 0; }
.sidebar-user-name { font-size: 0.875rem; font-weight: 600; color: var(--text-primary); }
.sidebar-user-role { font-size: 0.75rem; color: var(--text-secondary); margin-top: 0.1rem; }

/* ─── Logout Button ──────────────────────────────────────────────────────── */
.btn-logout { display: flex; align-items: center; gap: 0.5rem; width: 100%; padding: 0.6rem 1rem; border-radius: 10px; border: 1px solid rgba(239,68,68,0.25); background: rgba(239,68,68,0.08); color: #fca5a5; font-size: 0.85rem; font-weight: 500; cursor: pointer; transition: all 0.2s ease; margin-bottom: 0.75rem; }
.btn-logout svg { width: 16px; height: 16px; }
.btn-logout:hover { background: rgba(239,68,68,0.16); border-color: rgba(239,68,68,0.4); }

/* ─── Role Badge ─────────────────────────────────────────────────────────── */
.role-badge { font-size: 0.75rem; font-weight: 600; padding: 0.25rem 0.65rem; border-radius: 20px; letter-spacing: 0.02em; }
.role-badge.role-doctor  { background: rgba(0,212,255,0.12); border: 1px solid rgba(0,212,255,0.3);   color: var(--cyan); }
.role-badge.role-patient { background: rgba(168,85,247,0.12); border: 1px solid rgba(168,85,247,0.3); color: var(--purple-light); }

/* ─── Doctor Code Badge ──────────────────────────────────────────────────── */
.doctor-code-badge { font-size: 0.8rem; color: var(--text-secondary); background: var(--bg-elevated); border: 1px solid var(--border); padding: 0.35rem 0.85rem; border-radius: 20px; }
.doctor-code-badge strong { color: var(--cyan); font-family: var(--font-mono); letter-spacing: 0.1em; }

/* ═══════════════════════════════════════════════════════════════════════════
   DOCTOR PORTAL STYLES
   ═══════════════════════════════════════════════════════════════════════════ */

.doctor-welcome-bar { display: flex; align-items: center; justify-content: space-between; gap: 1.5rem; background: var(--bg-card); border: 1px solid var(--border); border-radius: 18px; padding: 1.5rem 2rem; margin-bottom: 2rem; flex-wrap: wrap; }
.doctor-avatar-wrap { display: flex; align-items: center; gap: 1rem; }
.doctor-avatar { width: 52px; height: 52px; border-radius: 50%; background: linear-gradient(135deg, rgba(0,212,255,0.2), rgba(124,58,237,0.2)); border: 2px solid rgba(0,212,255,0.3); display: flex; align-items: center; justify-content: center; font-size: 1.6rem; }
.doctor-welcome-name { font-size: 1.1rem; font-weight: 700; color: var(--text-primary); }
.doctor-welcome-sub  { font-size: 0.8rem; color: var(--text-secondary); margin-top: 0.25rem; }
.doctor-stats-row    { display: flex; gap: 1.5rem; }
.doctor-stat-card    { text-align: center; padding: 0.75rem 1.25rem; background: rgba(0,212,255,0.07); border-radius: 12px; border: 1px solid rgba(0,212,255,0.15); }
.ds-val   { display: block; font-size: 1.8rem; font-weight: 800; color: var(--cyan); font-family: var(--font-mono); }
.ds-label { font-size: 0.75rem; color: var(--text-secondary); text-transform: uppercase; letter-spacing: 0.08em; }

.section-title { font-size: 0.8rem; font-weight: 700; text-transform: uppercase; letter-spacing: 0.12em; color: var(--text-muted); margin-bottom: 1rem; display: flex; align-items: center; justify-content: space-between; gap: 1rem; }

.alerts-panel { background: var(--bg-card); border: 1px solid var(--border); border-radius: 14px; padding: 1rem; min-height: 80px; }
.alert-item { display: flex; align-items: center; gap: 1rem; padding: 0.6rem 0.75rem; border-radius: 10px; margin-bottom: 0.5rem; font-size: 0.85rem; }
.alert-item:last-child { margin-bottom: 0; }
.alert-item.failed  { background: rgba(239,68,68,0.1);  border: 1px solid rgba(239,68,68,0.2); }
.alert-item.partial { background: rgba(245,158,11,0.1); border: 1px solid rgba(245,158,11,0.2); }
.alert-patient-name { font-weight: 600; color: var(--text-primary); min-width: 100px; }
.alert-detail       { color: var(--text-secondary); font-size: 0.8rem; }

.patient-cards-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 1.25rem; }
.patient-card { background: var(--bg-card); border: 1px solid var(--border); border-radius: 16px; padding: 1.25rem; cursor: pointer; transition: all 0.25s ease; }
.patient-card:hover { border-color: rgba(0,212,255,0.25); transform: translateY(-2px); box-shadow: 0 8px 30px rgba(0,0,0,0.3), 0 0 20px rgba(0,212,255,0.08); }
.pc-header { display: flex; align-items: center; gap: 0.85rem; margin-bottom: 0.85rem; }
.pc-avatar { width: 40px; height: 40px; border-radius: 50%; background: linear-gradient(135deg, var(--cyan), var(--purple)); display: flex; align-items: center; justify-content: center; font-weight: 700; color: #fff; font-size: 1rem; flex-shrink: 0; }
.pc-info   { flex: 1; min-width: 0; }
.pc-name   { font-size: 0.95rem; font-weight: 700; color: var(--text-primary); }
.pc-meta   { font-size: 0.775rem; color: var(--text-secondary); margin-top: 0.15rem; }
.pc-compliance { font-size: 0.85rem; font-weight: 700; padding: 0.3rem 0.6rem; border-radius: 8px; }
.pc-compliance.good { background: rgba(34,197,94,0.15);  color: var(--green); }
.pc-compliance.warn { background: rgba(245,158,11,0.15); color: var(--amber); }
.pc-compliance.bad  { background: rgba(239,68,68,0.15);  color: var(--red); }
.pc-thresholds { display: flex; flex-wrap: wrap; gap: 0.4rem; margin-bottom: 0.75rem; }
.pc-thresh-item { font-size: 0.75rem; background: var(--bg-elevated); border: 1px solid var(--border); padding: 0.2rem 0.55rem; border-radius: 6px; color: var(--text-secondary); }
.pc-thresh-item strong { color: var(--cyan); }
.pc-thresh-none { font-size: 0.78rem; color: var(--text-muted); font-style: italic; }
.pc-footer { display: flex; justify-content: space-between; font-size: 0.775rem; color: var(--text-muted); padding-top: 0.6rem; border-top: 1px solid var(--border); }

.dp-layout { display: grid; grid-template-columns: 380px 1fr; gap: 1.5rem; align-items: start; }
.dp-profile-row { display: flex; justify-content: space-between; align-items: center; padding: 0.5rem 0; border-bottom: 1px solid var(--border); }
.dp-profile-label { font-size: 0.8rem; color: var(--text-secondary); }
.dp-profile-val   { font-size: 0.875rem; font-weight: 600; color: var(--text-primary); }
.divider { border: none; border-top: 1px solid var(--border); margin: 1rem 0; }
.thresh-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 0.75rem; }
.form-hint   { font-size: 0.7rem; color: var(--text-muted); font-weight: 400; }
.settings-desc { font-size: 0.825rem; color: var(--text-secondary); margin-bottom: 1.25rem; line-height: 1.55; }

.data-table { width: 100%; border-collapse: collapse; font-size: 0.825rem; }
.data-table th { text-align: left; padding: 0.6rem 0.75rem; font-size: 0.72rem; text-transform: uppercase; letter-spacing: 0.08em; color: var(--text-muted); border-bottom: 1px solid var(--border); }
.data-table td { padding: 0.6rem 0.75rem; border-bottom: 1px solid rgba(255,255,255,0.04); color: var(--text-secondary); }
.data-table tr:last-child td { border-bottom: none; }
.data-table tr:hover td { background: rgba(255,255,255,0.02); color: var(--text-primary); }

.status-badge { display: inline-block; padding: 0.2rem 0.55rem; border-radius: 6px; font-size: 0.75rem; font-weight: 600; text-transform: capitalize; }
.status-badge.completed { background: rgba(34,197,94,0.15);  color: var(--green); }
.status-badge.partial   { background: rgba(245,158,11,0.15); color: var(--amber); }
.status-badge.failed    { background: rgba(239,68,68,0.15);  color: var(--red); }

.thresh-history-item { padding: 0.85rem 1rem; border-radius: 10px; background: var(--bg-elevated); border: 1px solid var(--border); margin-bottom: 0.6rem; }
.thresh-history-item.current { border-color: rgba(0,212,255,0.25); background: rgba(0,212,255,0.05); }
.thi-date   { font-size: 0.78rem; color: var(--text-secondary); margin-bottom: 0.35rem; display: flex; align-items: center; gap: 0.5rem; }
.thi-values { font-size: 0.85rem; color: var(--text-primary); font-weight: 500; }
.thi-notes  { font-size: 0.78rem; color: var(--text-muted); font-style: italic; margin-top: 0.35rem; }
.current-badge { background: rgba(0,212,255,0.15); color: var(--cyan); font-size: 0.68rem; padding: 0.15rem 0.45rem; border-radius: 4px; font-weight: 700; text-transform: uppercase; }

.btn-sm     { font-size: 0.78rem; padding: 0.35rem 0.85rem; }
.empty-mini { text-align: center; color: var(--text-muted); font-size: 0.85rem; padding: 1rem; font-style: italic; }

/* ═══════════════════════════════════════════════════════════════════════════
   PATIENT PORTAL STYLES
   ═══════════════════════════════════════════════════════════════════════════ */

.patient-welcome-bar { display: flex; align-items: center; justify-content: space-between; gap: 1.5rem; background: linear-gradient(135deg, rgba(124,58,237,0.12) 0%, rgba(0,212,255,0.08) 100%); border: 1px solid rgba(124,58,237,0.2); border-radius: 18px; padding: 1.5rem 2rem; margin-bottom: 1.75rem; flex-wrap: wrap; }
.patient-welcome-left   { display: flex; align-items: center; gap: 1rem; }
.patient-welcome-avatar { width: 50px; height: 50px; border-radius: 50%; background: linear-gradient(135deg, var(--purple), var(--cyan)); display: flex; align-items: center; justify-content: center; font-weight: 700; color: #fff; font-size: 1.2rem; }
.patient-welcome-name { font-size: 1.15rem; font-weight: 700; color: var(--text-primary); }
.patient-welcome-sub  { font-size: 0.8rem; color: var(--text-secondary); margin-top: 0.2rem; }

.pt-stats-row { display: grid; grid-template-columns: repeat(3, 1fr); gap: 1rem; margin-bottom: 1.75rem; }
.pt-stat-card { text-align: center; padding: 1rem; background: var(--bg-card); border: 1px solid var(--border); border-radius: 14px; }
.pts-val   { display: block; font-size: 1.5rem; font-weight: 800; color: var(--purple-light); font-family: var(--font-mono); }
.pts-label { font-size: 0.75rem; color: var(--text-secondary); text-transform: uppercase; letter-spacing: 0.08em; margin-top: 0.25rem; }

.threshold-display-card { background: var(--bg-card); border: 1px solid rgba(0,212,255,0.2); border-radius: 16px; padding: 1.25rem 1.5rem; margin-bottom: 1.75rem; }
.tdc-header { display: flex; align-items: center; gap: 0.65rem; font-size: 0.85rem; font-weight: 600; color: var(--cyan); margin-bottom: 1rem; flex-wrap: wrap; }
.tdc-header svg { width: 18px; height: 18px; flex-shrink: 0; }
.tdc-type   { margin-left: auto; font-size: 0.8rem; background: rgba(0,212,255,0.1); border: 1px solid rgba(0,212,255,0.2); padding: 0.2rem 0.6rem; border-radius: 6px; }
.tdc-grid   { display: flex; flex-wrap: wrap; gap: 0.75rem; }
.tdc-item   { flex: 1; min-width: 100px; text-align: center; padding: 0.75rem; background: var(--bg-elevated); border-radius: 10px; border: 1px solid var(--border); }
.tdc-label  { display: block; font-size: 0.7rem; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.08em; margin-bottom: 0.35rem; }
.tdc-val    { display: block; font-size: 1.1rem; font-weight: 700; color: var(--text-primary); font-family: var(--font-mono); }

.no-thresh-card { display: flex; flex-direction: column; align-items: center; text-align: center; gap: 0.75rem; padding: 2.5rem; background: rgba(245,158,11,0.05); border: 1px dashed rgba(245,158,11,0.3); border-radius: 16px; margin-bottom: 1.75rem; color: var(--text-secondary); }
.no-thresh-card svg { width: 40px; height: 40px; color: var(--amber); opacity: 0.7; }
.no-thresh-card h3 { font-size: 1rem; font-weight: 700; color: var(--amber); }
.no-thresh-card p  { font-size: 0.85rem; line-height: 1.6; max-width: 420px; }

.exercise-layout { display: grid; grid-template-columns: 360px 1fr; gap: 1.5rem; align-items: start; }
.exercise-control-panel { display: flex; flex-direction: column; gap: 1.25rem; }

.rep-ring-wrap { position: relative; display: flex; align-items: center; justify-content: center; background: var(--bg-card); border: 1px solid var(--border); border-radius: 20px; padding: 2rem; }
.rep-ring { width: 160px; height: 160px; filter: drop-shadow(0 0 16px rgba(0,212,255,0.25)); }
.rep-ring-inner { position: absolute; display: flex; flex-direction: column; align-items: center; justify-content: center; }
.rep-count-big   { font-size: 3rem; font-weight: 800; color: var(--text-primary); line-height: 1; font-family: var(--font-mono); }
.rep-count-label { font-size: 0.7rem; text-transform: uppercase; letter-spacing: 0.12em; color: var(--text-muted); margin-top: 0.15rem; }

.exercise-info-banner { display: flex; gap: 0.75rem; flex-wrap: wrap; }
.eib-item  { flex: 1; min-width: 80px; text-align: center; padding: 0.65rem; background: var(--bg-elevated); border: 1px solid var(--border); border-radius: 10px; }
.eib-label { display: block; font-size: 0.68rem; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.08em; margin-bottom: 0.3rem; }
.eib-val   { display: block; font-size: 0.875rem; font-weight: 700; color: var(--purple-light); }

.exercise-live-stats { display: grid; grid-template-columns: 1fr 1fr; gap: 0.65rem; }
.els-item  { padding: 0.65rem 0.85rem; background: var(--bg-elevated); border: 1px solid var(--border); border-radius: 10px; }
.els-label { display: block; font-size: 0.7rem; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.07em; margin-bottom: 0.3rem; }
.els-val   { font-size: 1rem; font-weight: 700; color: var(--cyan); font-family: var(--font-mono); }

.exercise-alert { display: flex; align-items: center; gap: 0.75rem; padding: 0.85rem 1rem; border-radius: 12px; font-size: 0.875rem; font-weight: 500; animation: alertPulse 0.4s ease; }
@keyframes alertPulse { from { transform: scale(0.97); opacity: 0.7; } to { transform: scale(1); opacity: 1; } }
.exercise-alert svg { width: 18px; height: 18px; flex-shrink: 0; }
.exercise-alert.success { background: rgba(34,197,94,0.12);  border: 1px solid rgba(34,197,94,0.3);  color: #86efac; }
.exercise-alert.warning { background: rgba(245,158,11,0.12); border: 1px solid rgba(245,158,11,0.3); color: #fde68a; }
.exercise-alert.error   { background: rgba(239,68,68,0.12);  border: 1px solid rgba(239,68,68,0.3);  color: #fca5a5; }

.exercise-btns { display: flex; flex-direction: column; gap: 0.75rem; }
.exercise-timer { font-size: 1.1rem; font-weight: 700; color: var(--cyan); letter-spacing: 0.05em; }
.exercise-charts-panel { display: flex; flex-direction: column; gap: 1.25rem; }

/* ═══════════════════════════════════════════════════════════════════════════
   RESPONSIVE OVERRIDES
   ═══════════════════════════════════════════════════════════════════════════ */

.btn-pain {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
  width: 100%;
  padding: 0.85rem 1rem;
  background: rgba(239, 68, 68, 0.12);
  border: 1px solid rgba(239, 68, 68, 0.35);
  color: #fca5a5;
  border-radius: 12px;
  font-weight: 600;
  font-size: 0.875rem;
  cursor: pointer;
  transition: all 0.2s ease;
}
.btn-pain:hover {
  background: rgba(239, 68, 68, 0.22);
  border-color: #ef4444;
  color: #fff;
  box-shadow: 0 0 16px rgba(239, 68, 68, 0.3);
}
.btn-pain svg { width: 18px; height: 18px; flex-shrink: 0; }

.exercise-alert.critical-alert {
  background: rgba(239, 68, 68, 0.22);
  border: 2px solid #ef4444;
  color: #fee2e2;
  font-weight: 700;
  animation: criticalPulse 0.6s infinite alternate;
}
@keyframes criticalPulse {
  from { box-shadow: 0 0 8px rgba(239,68,68,0.4); }
  to { box-shadow: 0 0 20px rgba(239,68,68,0.8); }
}

.pain-alerts-panel {
  display: flex;
  flex-direction: column;
  gap: 0.75rem;
  margin-bottom: 1.5rem;
}
.pain-alert-card {
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: rgba(239, 68, 68, 0.08);
  border: 1px solid rgba(239, 68, 68, 0.25);
  border-radius: 10px;
  padding: 0.85rem 1.25rem;
  transition: all 0.2s ease;
}
.pain-alert-card.reviewed {
  opacity: 0.5;
  border-color: rgba(255,255,255,0.1);
  background: rgba(255,255,255,0.02);
}
.pain-alert-info { display: flex; flex-direction: column; gap: 0.2rem; }
.pain-alert-header { display: flex; align-items: center; gap: 0.5rem; }
.pain-alert-patient { font-weight: 700; color: #fff; font-size: 0.95rem; }
.pain-severity-pill {
  padding: 0.15rem 0.5rem;
  border-radius: 12px;
  font-size: 0.7rem;
  font-weight: 700;
  text-transform: uppercase;
}
.pain-severity-pill.Mild { background: rgba(245,158,11,0.2); color: #fde68a; border: 1px solid rgba(245,158,11,0.4); }
.pain-severity-pill.Moderate { background: rgba(239,68,68,0.2); color: #fca5a5; border: 1px solid rgba(239,68,68,0.4); }
.pain-severity-pill.Severe { background: rgba(220,38,38,0.35); color: #fee2e2; border: 1px solid #dc2626; }
.pain-alert-meta { font-size: 0.8rem; color: rgba(255,255,255,0.5); }
.pain-alert-notes { font-size: 0.82rem; color: rgba(255,255,255,0.8); font-style: italic; margin-top: 0.2rem; }

.badge-count {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  background: #ef4444;
  color: #fff;
  font-size: 0.7rem;
  font-weight: 800;
  border-radius: 12px;
  padding: 0.1rem 0.5rem;
  min-width: 20px;
}

.pain-level-btn.active {
  background: rgba(239, 68, 68, 0.25) !important;
  border-color: #ef4444 !important;
  color: #fff !important;
  font-weight: 700;
}

.video-card { overflow: hidden; padding: 0; }
.video-card .chart-card-header { padding: 0.85rem 1.25rem; }
.video-embed-wrap {
  position: relative;
  width: 100%;
  padding-bottom: 56.25%; /* 16:9 Aspect Ratio */
  height: 0;
  background: #000;
}
.video-embed-wrap iframe {
  position: absolute;
  top: 0; left: 0;
  width: 100%; height: 100%;
  border: none;
}

.modal-celebration {
  background: radial-gradient(circle at 50% 0%, rgba(34,197,94,0.15), var(--bg-card) 70%) !important;
  border: 1px solid rgba(34,197,94,0.3) !important;
}

@keyframes bounce {
  0%, 20%, 50%, 80%, 100% { transform: translateY(0); }
  40% { transform: translateY(-12px); }
  60% { transform: translateY(-6px); }
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

<!-- ═══ AUTH SCREENS ═══════════════════════════════════════════════════════ -->
<div id="auth-screen" class="auth-screen hidden">
  <!-- Login Page -->
  <div id="auth-login" class="auth-card">
    <div class="auth-logo">
      <svg viewBox="0 0 48 48" fill="none">
        <circle cx="24" cy="24" r="22" stroke="url(#authGrad)" stroke-width="1.5"/>
        <path d="M10 24 L16 16 L22 28 L28 10 L34 22 L40 24" stroke="url(#authGrad)" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/>
        <defs>
          <linearGradient id="authGrad" x1="10" y1="10" x2="40" y2="38" gradientUnits="userSpaceOnUse">
            <stop offset="0%" stop-color="#00d4ff"/>
            <stop offset="100%" stop-color="#7c3aed"/>
          </linearGradient>
        </defs>
      </svg>
      <div>
        <h1 class="auth-brand">PhysioPulse</h1>
        <p class="auth-brand-sub">MYOSA 6.0</p>
      </div>
    </div>
    <h2 class="auth-title">Welcome Back</h2>
    <p class="auth-subtitle">Sign in to your account</p>

    <div id="loginError" class="auth-error hidden"></div>

    <div class="form-group">
      <label class="form-label" for="loginUserId">User ID</label>
      <input type="text" class="form-input" id="loginUserId" placeholder="e.g. DR123456" autocomplete="username" style="text-transform:uppercase" />
    </div>
    <div class="form-group">
      <label class="form-label" for="loginPassword">Password</label>
      <input type="password" class="form-input" id="loginPassword" placeholder="••••••••" autocomplete="current-password" />
    </div>
    <button class="btn-primary auth-submit" id="loginBtn">
      <span id="loginBtnText">Sign In</span>
      <span id="loginBtnLoader" class="btn-loader hidden">⟳</span>
    </button>
    <p class="auth-switch">
      Don't have an account? <a href="#" id="goToRegister">Register here</a>
    </p>
  </div>

  <!-- Register Page -->
  <div id="auth-register" class="auth-card hidden">
    <div class="auth-logo">
      <svg viewBox="0 0 48 48" fill="none">
        <circle cx="24" cy="24" r="22" stroke="url(#authGrad2)" stroke-width="1.5"/>
        <path d="M10 24 L16 16 L22 28 L28 10 L34 22 L40 24" stroke="url(#authGrad2)" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/>
        <defs>
          <linearGradient id="authGrad2" x1="10" y1="10" x2="40" y2="38" gradientUnits="userSpaceOnUse">
            <stop offset="0%" stop-color="#00d4ff"/>
            <stop offset="100%" stop-color="#7c3aed"/>
          </linearGradient>
        </defs>
      </svg>
      <div>
        <h1 class="auth-brand">PhysioPulse</h1>
        <p class="auth-brand-sub">Create Account</p>
      </div>
    </div>

    <!-- Role selector -->
    <div class="role-selector">
      <button class="role-btn active" id="roleDoctor" data-role="doctor">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M20 7h-3a2 2 0 0 0-2-2H9a2 2 0 0 0-2 2H4a2 2 0 0 0-2 2v11a2 2 0 0 0 2 2h16a2 2 0 0 0 2-2V9a2 2 0 0 0-2-2z"/><circle cx="12" cy="14" r="3"/><path d="M12 11v-1M12 17v1M9.5 14H8.5M15.5 14h-1"/></svg>
        <span>Doctor</span>
      </button>
      <button class="role-btn" id="rolePatient" data-role="patient">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"/><circle cx="12" cy="7" r="4"/></svg>
        <span>Patient</span>
      </button>
    </div>

    <div id="registerError" class="auth-error hidden"></div>

    <div class="form-group">
      <label class="form-label" for="regName">Full Name</label>
      <input type="text" class="form-input" id="regName" placeholder="Dr. John Smith / Jane Doe" autocomplete="name" />
    </div>
    <div class="form-group">
      <label class="form-label" for="regUserId">User ID <span style="color:rgba(255,255,255,0.4);font-size:0.75rem">(optional, auto-generated if empty)</span></label>
      <input type="text" class="form-input" id="regUserId" placeholder="e.g. DR123456 / PT123456" autocomplete="username" style="text-transform:uppercase" />
    </div>
    <div class="form-group">
      <label class="form-label" for="regPassword">Password <span style="color:rgba(255,255,255,0.4);font-size:0.75rem">(min 6 chars)</span></label>
      <input type="password" class="form-input" id="regPassword" placeholder="••••••••" autocomplete="new-password" />
    </div>

    <!-- Patient-only: Doctor Code -->
    <div class="form-group" id="doctorCodeGroup" style="display:none">
      <label class="form-label" for="regDoctorUserId">Doctor User ID <span style="color:rgba(255,255,255,0.4);font-size:0.75rem">(from your doctor)</span></label>
      <input type="text" class="form-input" id="regDoctorUserId" placeholder="e.g. DR123456" style="text-transform:uppercase" />
    </div>

    <button class="btn-primary auth-submit" id="registerBtn">
      <span id="registerBtnText">Create Account</span>
      <span id="registerBtnLoader" class="btn-loader hidden">⟳</span>
    </button>
    <p class="auth-switch">
      Already have an account? <a href="#" id="goToLogin">Sign in</a>
    </p>
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

    <!-- User info in sidebar -->
    <div class="sidebar-user" id="sidebarUser">
      <div class="sidebar-user-avatar" id="sidebarUserAvatar">?</div>
      <div class="sidebar-user-info">
        <div class="sidebar-user-name" id="sidebarUserName">—</div>
        <div class="sidebar-user-role" id="sidebarUserRole">—</div>
      </div>
    </div>

    <!-- DOCTOR NAV -->
    <nav class="sidebar-nav" id="nav-doctor" style="display:none">
      <a href="#" class="nav-item active" data-page="doctor-dashboard" id="nav-doctor-dashboard">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7" height="7" rx="1"/><rect x="14" y="3" width="7" height="7" rx="1"/><rect x="3" y="14" width="7" height="7" rx="1"/><rect x="14" y="14" width="7" height="7" rx="1"/></svg></span>
        <span class="nav-label">Dashboard</span>
      </a>
      <a href="#" class="nav-item" data-page="doctor-patients" id="nav-doctor-patients">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87M16 3.13a4 4 0 0 1 0 7.75"/></svg></span>
        <span class="nav-label">My Patients</span>
      </a>
      <a href="#" class="nav-item" data-page="live" id="nav-live-doctor">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"/></svg></span>
        <span class="nav-label">Live Charts</span>
      </a>
      <a href="#" class="nav-item" data-page="settings" id="nav-settings-doctor">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06A1.65 1.65 0 0 0 4.68 15a1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06A1.65 1.65 0 0 0 9 4.68a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06A1.65 1.65 0 0 0 19.4 9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg></span>
        <span class="nav-label">Settings</span>
      </a>
    </nav>

    <!-- PATIENT NAV -->
    <nav class="sidebar-nav" id="nav-patient" style="display:none">
      <a href="#" class="nav-item active" data-page="patient-dashboard" id="nav-patient-dashboard">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7" height="7" rx="1"/><rect x="14" y="3" width="7" height="7" rx="1"/><rect x="3" y="14" width="7" height="7" rx="1"/><rect x="14" y="14" width="7" height="7" rx="1"/></svg></span>
        <span class="nav-label">My Dashboard</span>
      </a>
      <a href="#" class="nav-item" data-page="patient-exercise" id="nav-patient-exercise">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M18 8h1a4 4 0 0 1 0 8h-1"/><path d="M2 8h16v9a4 4 0 0 1-4 4H6a4 4 0 0 1-4-4V8z"/><line x1="6" y1="1" x2="6" y2="4"/><line x1="10" y1="1" x2="10" y2="4"/><line x1="14" y1="1" x2="14" y2="4"/></svg></span>
        <span class="nav-label">Exercise</span>
      </a>
      <a href="#" class="nav-item" data-page="patient-history" id="nav-patient-history">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/><line x1="16" y1="13" x2="8" y2="13"/><line x1="16" y1="17" x2="8" y2="17"/></svg></span>
        <span class="nav-label">My History</span>
      </a>
      <a href="#" class="nav-item" data-page="live" id="nav-live-patient">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"/></svg></span>
        <span class="nav-label">Live Charts</span>
      </a>
      <a href="#" class="nav-item" data-page="settings" id="nav-settings-patient">
        <span class="nav-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06A1.65 1.65 0 0 0 4.68 15a1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06A1.65 1.65 0 0 0 9 4.68a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06A1.65 1.65 0 0 0 19.4 9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg></span>
        <span class="nav-label">Settings</span>
      </a>
    </nav>

    <div class="sidebar-footer">
      <button class="btn-logout" id="logoutBtn">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"/><polyline points="16 17 21 12 16 7"/><line x1="21" y1="12" x2="9" y2="12"/></svg>
        Sign Out
      </button>
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
        <div class="role-badge" id="roleBadge"></div>
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

    <!-- ══════════════════════════════════════════════════════════════════ -->
    <!-- ═══ DOCTOR PAGES ════════════════════════════════════════════════ -->
    <!-- ══════════════════════════════════════════════════════════════════ -->

    <!-- ── DOCTOR: DASHBOARD ─────────────────────────────────────────── -->
    <section class="page" id="page-doctor-dashboard">
      <div class="page-header">
        <h1>Doctor Dashboard</h1>
        <div class="page-meta">
          <span class="doctor-code-badge">Your User ID: <strong id="doctorCode">—</strong></span>
        </div>
      </div>

      <div class="doctor-welcome-bar">
        <div class="doctor-avatar-wrap">
          <div class="doctor-avatar">🩺</div>
          <div>
            <div class="doctor-welcome-name">Dr. <span id="doctorName">—</span></div>
            <div class="doctor-welcome-sub">Share your User ID with patients to link them to your account</div>
          </div>
        </div>
        <div class="doctor-stats-row">
          <div class="doctor-stat-card">
            <span class="ds-val" id="doctorPatientCount">0</span>
            <span class="ds-label">Patients</span>
          </div>
        </div>
      </div>

      <!-- Pain & Limitation Alerts Panel -->
      <div class="section-title" style="display:flex;align-items:center;gap:0.5rem">
        <span>🚨 Patient Discomfort & Limitation Alerts</span>
        <span class="badge-count hidden" id="doctorPainBadge">0</span>
      </div>
      <div class="alerts-panel pain-alerts-panel" id="doctorPainAlertsList">
        <div class="empty-mini">No pain alerts reported ✓</div>
      </div>

      <!-- General Activity Alerts Panel -->
      <div class="section-title" style="margin-top:1.5rem">Recent Activity & Compliance Alerts</div>
      <div class="alerts-panel" id="doctorAlertsList">
        <div class="empty-mini">Loading...</div>
      </div>

      <!-- Patient Cards -->
      <div class="section-title" style="margin-top:2rem">My Patients
        <button class="btn-secondary btn-sm" onclick="App.navigateTo('doctor-patients')">View All</button>
      </div>
      <div class="patient-cards-grid" id="doctorPatientCards"></div>
    </section>

    <!-- ── DOCTOR: PATIENTS LIST ──────────────────────────────────────── -->
    <section class="page" id="page-doctor-patients">
      <div class="page-header">
        <h1>My Patients</h1>
        <div class="page-meta">
          <span class="doctor-code-badge">User ID: <strong id="doctorCodePatients">—</strong></span>
        </div>
      </div>
      <div class="patient-cards-grid" id="doctorPatientCardsFull"></div>
    </section>

    <!-- ── DOCTOR: PATIENT DETAIL + THRESHOLD FORM ───────────────────── -->
    <section class="page" id="page-doctor-patient">
      <div class="page-header">
        <button class="btn-secondary btn-sm" id="backToPatients">← Back to Patients</button>
        <h1 id="dpPatientName">Patient</h1>
      </div>

      <div class="dp-layout">
        <!-- Left: patient profile + threshold form -->
        <div class="dp-left">

          <!-- Patient Profile Card -->
          <div class="settings-card">
            <h2 class="settings-title">Patient Profile</h2>
            <div class="dp-profile-info">
              <div class="dp-profile-row"><span class="dp-profile-label">Age</span><span class="dp-profile-val" id="dpPatientAge">—</span></div>
              <div class="dp-profile-row"><span class="dp-profile-label">Condition</span><span class="dp-profile-val" id="dpPatientCondition">—</span></div>
              <div class="dp-profile-row"><span class="dp-profile-label">Sessions</span><span class="dp-profile-val" id="dpSessionCount">0</span></div>
              <div class="dp-profile-row"><span class="dp-profile-label">Compliance</span><span class="dp-profile-val" id="dpCompliance">—</span></div>
            </div>
            <hr class="divider" />
            <h3 class="form-title" style="margin-bottom:1rem">Edit Profile</h3>
            <form id="patientProfileForm">
              <div class="form-group">
                <label class="form-label" for="dpEditName">Name</label>
                <input type="text" class="form-input" id="dpEditName" />
              </div>
              <div class="form-group">
                <label class="form-label" for="dpEditAge">Age</label>
                <input type="number" class="form-input" id="dpEditAge" min="1" max="120" />
              </div>
              <div class="form-group">
                <label class="form-label" for="dpEditCondition">Condition / Diagnosis</label>
                <input type="text" class="form-input" id="dpEditCondition" placeholder="e.g. ACL Tear, Shoulder Injury" />
              </div>
              <button type="button" class="btn-secondary" id="patientProfileSaveBtn" style="width:100%">Save Profile</button>
            </form>
          </div>

          <!-- Threshold Form -->
          <div class="settings-card" style="margin-top:1.5rem">
            <h2 class="settings-title">Exercise Thresholds</h2>
            <p class="settings-desc">Set the safe angle range and exercise targets for this patient. These will be used to alert the patient during home exercise sessions.</p>
            <form id="thresholdForm">
              <div class="form-group">
                <label class="form-label" for="dpExerciseType">Exercise Type (Hand & Arm)</label>
                <select class="form-select" id="dpExerciseType">
                  <option value="Bicep Curl">Bicep Curl</option>
                  <option value="Wrist Circle / Rotation">Wrist Circle / Rotation</option>
                  <option value="Front Raise">Front Raise</option>
                  <option value="Side Raise">Side Raise</option>
                  <option value="Hand Circle">Hand Circle</option>
                  <option value="Custom Hand Exercise">Custom Hand Exercise</option>
                </select>
              </div>
              <div class="thresh-grid">
                <div class="form-group">
                  <label class="form-label" for="dpMinAngle">Min Angle (°) <span class="form-hint">target minimum flexion</span></label>
                  <input type="number" class="form-input" id="dpMinAngle" value="30" min="0" max="180" />
                </div>
                <div class="form-group">
                  <label class="form-label" for="dpMaxAngle">Max Angle (°) <span class="form-hint">safe upper limit</span></label>
                  <input type="number" class="form-input" id="dpMaxAngle" value="120" min="0" max="180" />
                </div>
                <div class="form-group">
                  <label class="form-label" for="dpTargetReps">Target Reps <span class="form-hint">per session</span></label>
                  <input type="number" class="form-input" id="dpTargetReps" value="10" min="1" max="100" />
                </div>
                <div class="form-group">
                  <label class="form-label" for="dpMotionLimit">Motion Limit (°/s) <span class="form-hint">gyro alert</span></label>
                  <input type="number" class="form-input" id="dpMotionLimit" value="100" min="10" max="500" />
                </div>
                <div class="form-group">
                  <label class="form-label" for="dpTempLimit">Temp Alert (°C)</label>
                  <input type="number" class="form-input" id="dpTempLimit" value="40" min="30" max="50" />
                </div>
              </div>
              <div class="form-group">
                <label class="form-label" for="dpVideoUrl">YouTube Exercise Demo URL <span class="form-hint">plays directly in patient app</span></label>
                <input type="url" class="form-input" id="dpVideoUrl" placeholder="https://www.youtube.com/watch?v=kYJjT6lYqA8" />
              </div>
              <div class="form-group">
                <label class="form-label" for="dpNotes">Clinical Notes & Instructions</label>
                <textarea class="form-textarea" id="dpNotes" rows="2" placeholder="e.g. Focus on gentle movement. Stop immediately if pain exceeds 4/10."></textarea>
              </div>
              <div class="form-group" style="display:flex;align-items:center;justify-content:space-between;background:rgba(255,255,255,0.03);padding:0.75rem 1rem;border-radius:8px;border:1px solid rgba(255,255,255,0.06)">
                <div>
                  <div style="font-weight:600;font-size:0.9rem">Strict Limit Enforcement</div>
                  <div style="font-size:0.75rem;color:rgba(255,255,255,0.4)">Freeze reps and sound caution when exceeding Max Angle or Target Reps</div>
                </div>
                <label class="switch"><input type="checkbox" id="dpStrictLimit" checked /><span class="switch-slider"></span></label>
              </div>
              <button type="button" class="btn-primary" id="thresholdSaveBtn" style="width:100%;margin-top:1rem">
                💾 Save Thresholds & Prescription
              </button>
            </form>
          </div>
        </div>

        <!-- Right: history + threshold history -->
        <div class="dp-right">
          <!-- Exercise History -->
          <div class="settings-card">
            <h2 class="settings-title">Exercise History</h2>
            <div id="dpHistoryTable"><div class="empty-mini">Loading...</div></div>
          </div>

          <!-- Threshold Change History -->
          <div class="settings-card" style="margin-top:1.5rem">
            <h2 class="settings-title">Threshold History</h2>
            <p class="settings-desc">All previous threshold settings for this patient.</p>
            <div id="dpThresholdHistory"><div class="empty-mini">No changes yet</div></div>
          </div>

          <div class="settings-card" style="margin-top:1.5rem">
            <h2 class="settings-title">Exercise Prescriptions (YouTube)</h2>
            <form id="prescriptionForm">
              <div class="form-group">
                <label class="form-label" for="dpVideoTitle">Video Title</label>
                <input type="text" class="form-input" id="dpVideoTitle" placeholder="e.g. Knee Mobility Routine" />
              </div>
              <div class="form-group">
                <label class="form-label" for="dpVideoUrl">YouTube URL</label>
                <input type="url" class="form-input" id="dpVideoUrl" placeholder="https://www.youtube.com/watch?v=..." />
              </div>
              <div class="form-group">
                <label class="form-label" for="dpVideoNotes">Notes</label>
                <textarea class="form-textarea" id="dpVideoNotes" rows="2" placeholder="Usage notes for patient"></textarea>
              </div>
              <button type="button" class="btn-primary" id="prescriptionSaveBtn" style="width:100%">Add / Update Prescription</button>
            </form>
            <hr class="divider" />
            <div id="dpPrescriptionList"><div class="empty-mini">No exercise videos assigned yet</div></div>
          </div>
        </div>
      </div>
    </section>

    <!-- ══════════════════════════════════════════════════════════════════ -->
    <!-- ═══ PATIENT PAGES ═══════════════════════════════════════════════ -->
    <!-- ══════════════════════════════════════════════════════════════════ -->

    <!-- ── PATIENT: DASHBOARD ────────────────────────────────────────── -->
    <section class="page" id="page-patient-dashboard">
      <div class="page-header">
        <h1>My Dashboard</h1>
        <div class="page-meta">
          <button class="btn-secondary btn-sm" id="ptRefreshThreshold">↻ Sync Thresholds</button>
        </div>
      </div>

      <!-- Patient welcome -->
      <div class="patient-welcome-bar">
        <div class="patient-welcome-left">
          <div class="patient-welcome-avatar" id="ptAvatarLetter">P</div>
          <div>
            <div class="patient-welcome-name" id="ptPatientName">—</div>
            <div class="patient-welcome-sub">Assigned Doctor: <span id="ptDoctorInfo">—</span></div>
          </div>
        </div>
        <button class="btn-primary" onclick="App.navigateTo('patient-exercise')">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polygon points="10 8 16 12 10 16 10 8"/></svg>
          Start Exercise
        </button>
      </div>

      <!-- Summary Stats -->
      <div class="pt-stats-row">
        <div class="pt-stat-card">
          <span class="pts-val" id="ptTotalSessions">0</span>
          <span class="pts-label">Sessions</span>
        </div>
        <div class="pt-stat-card">
          <span class="pts-val" id="ptCompliance">—</span>
          <span class="pts-label">Compliance</span>
        </div>
        <div class="pt-stat-card">
          <span class="pts-val" id="ptLastSession">—</span>
          <span class="pts-label">Last Session</span>
        </div>
      </div>

      <!-- Threshold Info Card -->
      <div class="threshold-display-card" id="ptThreshCard">
        <div class="tdc-header">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"/><polyline points="22 4 12 14.01 9 11.01"/></svg>
          <span>Your Exercise Prescription</span>
          <span class="tdc-type" id="ptExercise">—</span>
        </div>
        <div class="tdc-grid">
          <div class="tdc-item">
            <span class="tdc-label">Min Angle</span>
            <span class="tdc-val" id="ptMinAngle">—</span>
          </div>
          <div class="tdc-item">
            <span class="tdc-label">Max Angle</span>
            <span class="tdc-val" id="ptMaxAngle">—</span>
          </div>
          <div class="tdc-item">
            <span class="tdc-label">Target Reps</span>
            <span class="tdc-val" id="ptTargetReps">—</span>
          </div>
          <div class="tdc-item">
            <span class="tdc-label">Motion Limit</span>
            <span class="tdc-val" id="ptMotionLimit">—</span>
          </div>
          <div class="tdc-item">
            <span class="tdc-label">Temp Alert</span>
            <span class="tdc-val" id="ptTempLimit">—</span>
          </div>
        </div>

        <div class="settings-card" style="margin-top:1rem">
          <h2 class="settings-title">Doctor Assigned Exercise Videos</h2>
          <div id="ptPrescriptionList"><div class="empty-mini">No exercise videos assigned yet</div></div>
        </div>
      </div>

      <!-- No threshold state -->
      <div class="no-thresh-card" id="ptNoThresh">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><circle cx="12" cy="12" r="10"/><line x1="12" y1="8" x2="12" y2="12"/><line x1="12" y1="16" x2="12.01" y2="16"/></svg>
        <h3>No Exercise Prescription Yet</h3>
        <p>Your doctor hasn't set your exercise thresholds yet. Please visit your doctor for your initial assessment, or click "Sync Thresholds" if your doctor has already entered them.</p>
      </div>

      <!-- Sensor cards (live data) -->
      <div class="section-title" style="margin-top:2rem">Live Sensor Data</div>
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
            <div class="axis-reading"><span class="axis-label axis-x">X</span><span class="axis-value mono" id="accelX">0.000</span><span class="axis-unit">g</span><div class="axis-bar"><div class="axis-fill ax-fill" id="accelXBar"></div></div></div>
            <div class="axis-reading"><span class="axis-label axis-y">Y</span><span class="axis-value mono" id="accelY">0.000</span><span class="axis-unit">g</span><div class="axis-bar"><div class="axis-fill ay-fill" id="accelYBar"></div></div></div>
            <div class="axis-reading"><span class="axis-label axis-z">Z</span><span class="axis-value mono" id="accelZ">0.000</span><span class="axis-unit">g</span><div class="axis-bar"><div class="axis-fill az-fill" id="accelZBar"></div></div></div>
          </div>
          <div class="card-magnitude"><span class="mag-label">Magnitude</span><span class="mag-value mono" id="accelMag">0.000 g</span></div>
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
            <div class="axis-reading"><span class="axis-label axis-x">X</span><span class="axis-value mono" id="gyroX">0.000</span><span class="axis-unit">°/s</span><div class="axis-bar"><div class="axis-fill gx-fill" id="gyroXBar"></div></div></div>
            <div class="axis-reading"><span class="axis-label axis-y">Y</span><span class="axis-value mono" id="gyroY">0.000</span><span class="axis-unit">°/s</span><div class="axis-bar"><div class="axis-fill gy-fill" id="gyroYBar"></div></div></div>
            <div class="axis-reading"><span class="axis-label axis-z">Z</span><span class="axis-value mono" id="gyroZ">0.000</span><span class="axis-unit">°/s</span><div class="axis-bar"><div class="axis-fill gz-fill" id="gyroZBar"></div></div></div>
          </div>
          <div class="card-magnitude"><span class="mag-label">Total Rate</span><span class="mag-value mono" id="gyroMag">0.000 °/s</span></div>
          <canvas class="mini-chart" id="miniGyroChart" height="60"></canvas>
        </div>

        <!-- Temperature Card -->
        <div class="sensor-card card-temp">
          <div class="card-header">
            <div class="card-title-group">
              <div class="card-icon temp-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"/></svg>
              </div>
              <div><h2 class="card-title">Temperature</h2><p class="card-subtitle">Sensor junction temp (°C)</p></div>
            </div>
          </div>
          <div class="temp-display">
            <div class="temp-gauge-wrap">
              <svg class="temp-gauge" viewBox="0 0 160 100" id="tempGaugeSVG">
                <path d="M 20 90 A 70 70 0 0 1 140 90" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="12" stroke-linecap="round"/>
                <path d="M 20 90 A 70 70 0 0 1 140 90" fill="none" stroke="url(#tempGrad)" stroke-width="12" stroke-linecap="round" stroke-dasharray="220" stroke-dashoffset="220" id="tempGaugeFill"/>
                <line x1="80" y1="90" x2="80" y2="30" stroke="#fff" stroke-width="2" stroke-linecap="round" transform="rotate(0, 80, 90)" id="tempNeedle"/>
                <circle cx="80" cy="90" r="4" fill="#fff"/>
                <defs>
                  <linearGradient id="tempGrad" x1="20" y1="90" x2="140" y2="90" gradientUnits="userSpaceOnUse">
                    <stop offset="0%" stop-color="#00d4ff"/><stop offset="50%" stop-color="#f59e0b"/><stop offset="100%" stop-color="#ef4444"/>
                  </linearGradient>
                </defs>
              </svg>
              <div class="temp-value-wrap"><span class="temp-value mono" id="tempValue">--.-</span><span class="temp-unit">°C</span></div>
            </div>
            <div class="temp-stats">
              <div class="temp-stat"><span class="ts-label">Min</span><span class="ts-val mono" id="tempMin">--.-</span></div>
              <div class="temp-stat"><span class="ts-label">Max</span><span class="ts-val mono" id="tempMax">--.-</span></div>
              <div class="temp-stat"><span class="ts-label">Avg</span><span class="ts-val mono" id="tempAvg">--.-</span></div>
            </div>
          </div>
          <canvas class="mini-chart" id="miniTempChart" height="60"></canvas>
        </div>

        <!-- ROM Card -->
        <div class="sensor-card card-rom">
          <div class="card-header">
            <div class="card-title-group">
              <div class="card-icon rom-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 22V12"/><path d="m15 19-3 3-3-3"/><path d="M20 12a8 8 0 1 0-16 0"/></svg>
              </div>
              <div><h2 class="card-title">Range of Motion</h2><p class="card-subtitle">Computed joint angles</p></div>
            </div>
          </div>
          <div class="rom-grid">
            <div class="rom-item"><span class="rom-label">Pitch (X)</span><div class="rom-arc-wrap"><svg class="rom-arc" viewBox="0 0 80 50"><path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="8"/><path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="#00d4ff" stroke-width="8" stroke-dasharray="116" stroke-dashoffset="116" id="pitchArc"/></svg><span class="rom-value mono" id="pitchVal">0°</span></div></div>
            <div class="rom-item"><span class="rom-label">Roll (Y)</span><div class="rom-arc-wrap"><svg class="rom-arc" viewBox="0 0 80 50"><path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="8"/><path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="#a855f7" stroke-width="8" stroke-dasharray="116" stroke-dashoffset="116" id="rollArc"/></svg><span class="rom-value mono" id="rollVal">0°</span></div></div>
            <div class="rom-item"><span class="rom-label">Yaw (Z)</span><div class="rom-arc-wrap"><svg class="rom-arc" viewBox="0 0 80 50"><path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="rgba(255,255,255,0.08)" stroke-width="8"/><path d="M 5 45 A 37 37 0 0 1 75 45" fill="none" stroke="#f59e0b" stroke-width="8" stroke-dasharray="116" stroke-dashoffset="116" id="yawArc"/></svg><span class="rom-value mono" id="yawVal">0°</span></div></div>
          </div>
          <div class="rom-alert hidden" id="romAlert">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
            <span id="romAlertText">Motion limit exceeded</span>
          </div>
        </div>
      </div>
    </section>

    <!-- ── PATIENT: EXERCISE PAGE ─────────────────────────────────────── -->
    <section class="page" id="page-patient-exercise">
      <div class="page-header">
        <h1>Exercise Session</h1>
        <div class="page-meta">
          <span class="exercise-timer mono" id="ptExerciseTimer">00:00</span>
        </div>
      </div>

      <div class="exercise-layout">
        <!-- Rep Counter + Controls -->
        <div class="exercise-control-panel">
          <!-- Big Rep Ring -->
          <div class="rep-ring-wrap">
            <svg class="rep-ring" viewBox="0 0 120 120">
              <circle cx="60" cy="60" r="54" fill="none" stroke="rgba(255,255,255,0.07)" stroke-width="10"/>
              <circle cx="60" cy="60" r="54" fill="none" stroke="rgba(255,255,255,0.15)" stroke-width="10"
                stroke-dasharray="339.29" stroke-dashoffset="339.29"
                stroke-linecap="round" id="repProgressRing"
                style="transition:stroke-dashoffset 0.4s ease, stroke 0.4s ease; transform-origin:60px 60px; transform:rotate(-90deg)"/>
            </svg>
            <div class="rep-ring-inner">
              <span class="rep-count-big mono" id="ptRepCountBig">0</span>
              <span class="rep-count-label">reps</span>
            </div>
          </div>

          <!-- Exercise Info Banner -->
          <div class="exercise-info-banner">
            <div class="eib-item">
              <span class="eib-label">Target</span>
              <span class="eib-val" id="ptExerciseTarget">— reps</span>
            </div>
            <div class="eib-item">
              <span class="eib-label">Exercise</span>
              <span class="eib-val" id="ptExerciseName">—</span>
            </div>
            <div class="eib-item">
              <span class="eib-label">Range</span>
              <span class="eib-val" id="ptExerciseRange">—° – —°</span>
            </div>
          </div>

          <!-- Live stats during exercise -->
          <div class="exercise-live-stats">
            <div class="els-item">
              <span class="els-label">Current Angle</span>
              <span class="els-val mono" id="ptLivePitch">0.0°</span>
            </div>
            <div class="els-item">
              <span class="els-label">Motion</span>
              <span class="els-val mono" id="ptLiveGyro">0.0 °/s</span>
            </div>
            <div class="els-item">
              <span class="els-label">Temperature</span>
              <span class="els-val mono" id="ptLiveTemp">—</span>
            </div>
            <div class="els-item">
              <span class="els-label">Reps Done</span>
              <span class="els-val mono" id="ptRepCount">0</span>
            </div>
          </div>

          <!-- Critical Limit Alert Box -->
          <div class="exercise-alert critical-alert hidden" id="ptMaxLimitAlert">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><line x1="4.93" y1="4.93" x2="19.07" y2="19.07"/></svg>
            <span id="ptMaxLimitText">⛔ DOCTOR LIMIT: Maximum Angle Reached! Stop movement now.</span>
          </div>

          <!-- Alert Box -->
          <div class="exercise-alert hidden" id="ptExerciseAlert">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
            <span id="ptExerciseAlertText">Alert</span>
          </div>

          <!-- Start / Stop Buttons -->
          <div class="exercise-btns">
            <button class="btn-record" id="ptStartExercise">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polygon points="10 8 16 12 10 16 10 8"/></svg>
              Start Exercise
            </button>
            <button class="btn-stop hidden" id="ptStopExercise">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><rect x="9" y="9" width="6" height="6"/></svg>
              Stop & Save
            </button>
          </div>

          <!-- Pain & Discomfort Button -->
          <button type="button" class="btn-pain" id="ptReportPainBtn">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
            🚨 Report Pain / Can't Reach Threshold
          </button>
        </div>

        <!-- Live Sensor Charts + Prescribed Demo -->
        <div class="exercise-charts-panel">
          <!-- YouTube Video Demo Card -->
          <div class="chart-card video-card hidden" id="ptVideoDemoCard">
            <div class="chart-card-header">
              <h2>📹 Prescribed Exercise Demonstration</h2>
              <span class="badge-tag">Doctor's Video</span>
            </div>
            <div class="video-embed-wrap" id="ptVideoEmbedWrap">
              <iframe id="ptVideoIframe" src="" title="Exercise Demonstration" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
            </div>
          </div>

          <div class="chart-card">
            <div class="chart-card-header"><h2>Accelerometer (g)</h2></div>
            <div class="chart-wrap"><canvas id="sessionAccelChart"></canvas></div>
          </div>
          <div class="chart-card">
            <div class="chart-card-header"><h2>Gyroscope (°/s)</h2></div>
            <div class="chart-wrap"><canvas id="sessionGyroChart"></canvas></div>
          </div>
        </div>
      </div>
    </section>

    <!-- ── PATIENT: HISTORY ───────────────────────────────────────────── -->
    <section class="page" id="page-patient-history">
      <div class="page-header">
        <h1>Exercise History</h1>
      </div>

      <div id="ptHistoryEmpty" class="empty-state">
        <div class="empty-icon">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/></svg>
        </div>
        <h2>No sessions recorded yet</h2>
        <p>Complete an exercise session to see your history here.</p>
      </div>

      <div class="history-list" id="ptHistoryList"></div>
    </section>

    <!-- ══════════════════════════════════════════════════════════════════ -->
    <!-- ═══ SHARED PAGES ════════════════════════════════════════════════ -->
    <!-- ══════════════════════════════════════════════════════════════════ -->

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
        <div class="chart-card"><div class="chart-card-header"><h2>Accelerometer (g)</h2><div class="chart-legend"><span class="legend-dot" style="background:#ef4444"></span><span>X</span><span class="legend-dot" style="background:#22c55e"></span><span>Y</span><span class="legend-dot" style="background:#3b82f6"></span><span>Z</span></div></div><div class="chart-wrap"><canvas id="accelChart"></canvas></div></div>
        <div class="chart-card"><div class="chart-card-header"><h2>Gyroscope (°/s)</h2><div class="chart-legend"><span class="legend-dot" style="background:#f97316"></span><span>X</span><span class="legend-dot" style="background:#a855f7"></span><span>Y</span><span class="legend-dot" style="background:#06b6d4"></span><span>Z</span></div></div><div class="chart-wrap"><canvas id="gyroChart"></canvas></div></div>
        <div class="chart-card chart-card-half"><div class="chart-card-header"><h2>Temperature (°C)</h2></div><div class="chart-wrap"><canvas id="tempChart"></canvas></div></div>
        <div class="chart-card chart-card-half"><div class="chart-card-header"><h2>Range of Motion (°)</h2><div class="chart-legend"><span class="legend-dot" style="background:#00d4ff"></span><span>Pitch</span><span class="legend-dot" style="background:#a855f7"></span><span>Roll</span><span class="legend-dot" style="background:#f59e0b"></span><span>Yaw</span></div></div><div class="chart-wrap"><canvas id="romChart"></canvas></div></div>
      </div>
    </section>

    <!-- ── PAGE: SETTINGS ────────────────────────────────────────────── -->
    <section class="page" id="page-settings">
      <div class="page-header"><h1>Settings</h1></div>
      <div class="settings-grid">
        <div class="settings-card">
          <h2 class="settings-title">Connection</h2>
          <div class="setting-row">
            <div class="setting-info"><span class="setting-label">Connection Mode</span><span class="setting-desc">WiFi WebSocket or Web Bluetooth BLE</span></div>
            <div class="toggle-group" id="connModeToggle">
              <button class="toggle-btn" data-mode="wifi">WiFi</button>
              <button class="toggle-btn active" data-mode="ble">BLE</button>
            </div>
          </div>
          <div class="setting-row" id="wifiSettings">
            <div class="setting-info"><span class="setting-label">WebSocket URL</span><span class="setting-desc">ws://192.168.4.1:81 (default AP)</span></div>
            <input type="text" class="form-input" id="wsUrl" value="ws://192.168.4.1:81" style="max-width:240px" />
          </div>
          <div class="setting-row">
            <div class="setting-info"><span class="setting-label">Auto-Reconnect</span><span class="setting-desc">Reconnect on connection loss</span></div>
            <label class="switch"><input type="checkbox" id="autoReconnect" checked /><span class="switch-slider"></span></label>
          </div>
          <div class="setting-row"><button class="btn-primary" id="connectBtn" style="width:100%">Connect to ESP32</button></div>
          <div class="setting-row"><button class="btn-secondary" id="calibrateBtn" style="width:100%">Recalibrate Gyroscope</button></div>
        </div>
        <div class="settings-card">
          <h2 class="settings-title">Display</h2>
          <div class="setting-row">
            <div class="setting-info"><span class="setting-label">Chart Buffer Size</span><span class="setting-desc">Data points to display in charts</span></div>
            <select class="form-select" id="bufferSize" style="max-width:120px"><option value="100">100 pts</option><option value="200" selected>200 pts</option><option value="500">500 pts</option></select>
          </div>
          <div class="setting-row">
            <div class="setting-info"><span class="setting-label">Motion Alert Threshold</span><span class="setting-desc">Gyro magnitude to trigger alert (°/s)</span></div>
            <input type="number" class="form-input" id="motionThreshold" value="100" min="10" max="250" style="max-width:100px"/>
          </div>
          <div class="setting-row">
            <div class="setting-info"><span class="setting-label">Temperature Alert</span><span class="setting-desc">Alert above this temp (°C)</span></div>
            <input type="number" class="form-input" id="tempThreshold" value="40" min="30" max="50" style="max-width:100px"/>
          </div>
        </div>
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

  </main>
</div>

<!-- ═══ TOAST NOTIFICATIONS ═══════════════════════════════════════════════ -->
<div class="toast-container" id="toastContainer"></div>

<!-- ═══ PAIN & DISCOMFORT REPORT MODAL ════════════════════════════════════ -->
<div id="painModal" class="modal hidden">
  <div class="modal-card">
    <div class="modal-header">
      <div style="display:flex;align-items:center;gap:0.5rem">
        <span style="font-size:1.4rem">🚨</span>
        <h2 style="margin:0;font-size:1.2rem">Report Pain or Limitation</h2>
      </div>
      <button class="modal-close" id="closePainModal">✕</button>
    </div>
    <div class="modal-body" style="padding:1.5rem">
      <p style="color:rgba(255,255,255,0.7);font-size:0.875rem;margin-bottom:1.25rem;line-height:1.4">
        Your doctor will be notified immediately with your current angle and rep metrics so they can review your progression or adjust your limits.
      </p>

      <div class="pain-summary-badge" style="display:flex;justify-content:space-around;background:rgba(239,68,68,0.1);border:1px solid rgba(239,68,68,0.3);padding:0.75rem;border-radius:8px;margin-bottom:1.25rem">
        <div>Angle at Discomfort: <strong id="painModalAngle" class="mono" style="color:#ef4444">0.0°</strong></div>
        <div>Reps Completed: <strong id="painModalReps" class="mono" style="color:#ef4444">0</strong></div>
      </div>

      <div class="form-group">
        <label class="form-label">Pain Severity Level</label>
        <div class="pain-level-selector" style="display:grid;grid-template-columns:repeat(3,1fr);gap:0.5rem">
          <button type="button" class="pain-level-btn active" data-level="Mild" style="padding:0.6rem;background:rgba(255,255,255,0.06);border:1px solid rgba(255,255,255,0.15);color:#fff;border-radius:8px;cursor:pointer">😊 Mild (1-3)</button>
          <button type="button" class="pain-level-btn" data-level="Moderate" style="padding:0.6rem;background:rgba(255,255,255,0.06);border:1px solid rgba(255,255,255,0.15);color:#fff;border-radius:8px;cursor:pointer">😐 Moderate (4-6)</button>
          <button type="button" class="pain-level-btn" data-level="Severe" style="padding:0.6rem;background:rgba(255,255,255,0.06);border:1px solid rgba(255,255,255,0.15);color:#fff;border-radius:8px;cursor:pointer">😣 Severe (7-10)</button>
        </div>
      </div>

      <div class="form-group" style="margin-top:1rem">
        <label class="form-label" for="painModalNotes">What are you feeling? (Optional)</label>
        <textarea class="form-textarea" id="painModalNotes" rows="3" placeholder="e.g. Sharp pain at 65° flexion, knee stiffness, pinch in joint..."></textarea>
      </div>
    </div>
    <div class="modal-footer" style="padding:1rem 1.5rem;display:flex;justify-content:flex-end;gap:0.75rem;border-top:1px solid rgba(255,255,255,0.08)">
      <button class="btn-secondary" id="cancelPainModal">Cancel</button>
      <button class="btn-primary" id="submitPainAlert" style="background:#ef4444;border-color:#ef4444">
        🚨 Send Alert to Doctor
      </button>
    </div>
  </div>
</div>

<!-- ═══ MOTIVATIONAL POST-EXERCISE CELEBRATION MODAL ══════════════════════ -->
<div id="motivationalModal" class="modal hidden">
  <div class="modal-card modal-celebration" style="max-width:480px;text-align:center">
    <div class="modal-body" style="padding:2rem">
      <div style="font-size:3.5rem;margin-bottom:0.5rem;animation:bounce 1s infinite">🎉</div>
      <h2 id="motivationalTitle" style="font-size:1.6rem;margin-bottom:0.25rem;color:#fff">Session Completed!</h2>
      <div class="motivational-badge" id="motivationalBadge" style="display:inline-block;padding:0.35rem 1rem;border-radius:20px;font-weight:600;font-size:0.85rem;margin-bottom:1rem;background:rgba(34,197,94,0.15);color:#22c55e;border:1px solid rgba(34,197,94,0.3)">
        🌟 Outstanding Dedication!
      </div>
      
      <p id="motivationalQuote" style="font-style:italic;color:rgba(255,255,255,0.8);font-size:0.95rem;line-height:1.5;margin-bottom:1.5rem">
        "Every rep is a step closer to full recovery. Consistency is your superpower!"
      </p>

      <div class="session-summary-grid" style="display:grid;grid-template-columns:repeat(2,1fr);gap:0.75rem;background:rgba(255,255,255,0.03);padding:1rem;border-radius:12px;border:1px solid rgba(255,255,255,0.06);margin-bottom:1.5rem;text-align:left">
        <div>
          <div style="font-size:0.75rem;color:rgba(255,255,255,0.4)">Reps Completed</div>
          <div class="mono" id="mRepsDone" style="font-size:1.25rem;font-weight:700;color:#00d4ff">0</div>
        </div>
        <div>
          <div style="font-size:0.75rem;color:rgba(255,255,255,0.4)">Max Angle Reached</div>
          <div class="mono" id="mMaxAngle" style="font-size:1.25rem;font-weight:700;color:#22c55e">0.0°</div>
        </div>
        <div>
          <div style="font-size:0.75rem;color:rgba(255,255,255,0.4)">Session Duration</div>
          <div class="mono" id="mDuration" style="font-size:1.25rem;font-weight:700;color:#f59e0b">0s</div>
        </div>
        <div>
          <div style="font-size:0.75rem;color:rgba(255,255,255,0.4)">Compliance Status</div>
          <div class="mono" id="mCompliance" style="font-size:1.25rem;font-weight:700;color:#a855f7">100%</div>
        </div>
      </div>

      <div class="recovery-tip" id="motivationalTip" style="background:rgba(0,212,255,0.08);border:1px solid rgba(0,212,255,0.2);padding:0.75rem;border-radius:8px;font-size:0.8rem;color:rgba(255,255,255,0.85);text-align:left;margin-bottom:1.5rem">
        💡 <strong>Recovery Tip:</strong> Hydrate and gently stretch. Allow 24 hours of rest before high-intensity loading.
      </div>

      <button class="btn-primary" id="closeMotivationalBtn" style="width:100%;padding:0.85rem;font-size:1rem">
        Great, Continue
      </button>
    </div>
  </div>
</div>

<!-- ═══ SCRIPTS ════════════════════════════════════════════════════════════ -->
<script>/**
 * PhysioPulse – API Client Module
 * Communicates with the FastAPI Backend for Patient History, Thresholds, and Sessions.
 */

const API = (() => {
  // Determine API base URL
  const DEFAULT_BACKEND_URL = 'http://localhost:8000';
  let isBackendOnline = false;

  function getBaseUrl() {
    const origin = window.location.origin;
    if (origin && !origin.startsWith('file:') && !origin.includes('192.168.4.1')) {
      return origin;
    }
    return DEFAULT_BACKEND_URL;
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
          if (errData.detail) errorMsg = errData.detail;
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

  return {
    checkHealth,
    isOnline: () => isBackendOnline,
    getToken,
    setToken,
    getBaseUrl,
    // Auth
    register,
    login,
    getMe,
    getDoctors,
    // Patients
    getPatients,
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
</script>
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
</script>
<script>/**
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

  // ─── Public API ───────────────────────────────────────────────────────────
  return {
    init,
    register,
    login,
    logout,
    getSession,
    // patient
    getPatientById,
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
</script>
<script>/**
 * PhysioPulse – Doctor Portal Module
 * Manages:
 * - Patient list & profiles
 * - Exercise threshold management + YouTube demo video link
 * - Real-time & persisted Patient Pain & Limitation Alerts
 * - Historical compliance & exercise sessions
 */

const DoctorPortal = (() => {
  let session = null; // current doctor session

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
            <span style="font-size:0.75rem;color:rgba(255,255,255,0.4)">${new Date(a.date).toLocaleString()}</span>
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
      _setVal('dpTempLimit', threshold.tempLimit || 40);
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
      _setVal('dpTempLimit', 40);
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
            <th>Avg Temp</th>
            <th>Status</th>
            <th>Duration</th>
          </tr>
        </thead>
        <tbody>
          ${history.map(h => `
            <tr>
              <td>${new Date(h.date).toLocaleString()}</td>
              <td>${h.repsCompleted || 0}${threshold ? '/' + threshold.targetReps : ''}</td>
              <td>${h.maxAngleReached != null ? h.maxAngleReached.toFixed(1) + '°' : '—'}</td>
              <td>${h.minAngleReached != null ? h.minAngleReached.toFixed(1) + '°' : '—'}</td>
              <td>${h.avgTemp != null ? h.avgTemp.toFixed(1) + ' °C' : '—'}</td>
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
        <div class="thi-date">${new Date(t.updatedAt || t.created_at).toLocaleString()} ${i === 0 ? '<span class="current-badge">Current</span>' : ''}</div>
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
    if (!form) return;
    const patientId = parseInt(form.dataset.patientId);

    const minAngle  = parseFloat(document.getElementById('dpMinAngle')?.value) || 30;
    const maxAngle  = parseFloat(document.getElementById('dpMaxAngle')?.value) || 120;
    const targetReps= parseInt(document.getElementById('dpTargetReps')?.value)  || 10;
    const motionLimit= parseFloat(document.getElementById('dpMotionLimit')?.value) || 100;
    const tempLimit  = parseFloat(document.getElementById('dpTempLimit')?.value)  || 40;
    const exerciseType = document.getElementById('dpExerciseType')?.value || 'Knee Flexion / Extension';
    const videoUrl   = document.getElementById('dpVideoUrl')?.value.trim() || '';
    const strictLimit= document.getElementById('dpStrictLimit')?.checked ?? true;
    const notes      = document.getElementById('dpNotes')?.value.trim() || '';

    if (minAngle >= maxAngle) {
      App.showToast('Min angle must be less than max angle', 'error');
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

      App.showToast('Thresholds & YouTube Demo saved successfully!', 'success');

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

  // ─── Save Patient Profile ─────────────────────────────────────────────────
  async function savePatientProfile() {
    const form = document.getElementById('patientProfileForm');
    if (!form) return;
    const patientId = parseInt(form.dataset.patientId);

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
    document.getElementById('patientProfileSaveBtn')?.addEventListener('click', savePatientProfile);
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

  return { init, refresh, openPatientDetail, saveThreshold, savePatientProfile, markAlertReviewed };
})();
</script>
<script>/**
 * PhysioPulse – Patient Portal Module
 * Handles:
 * - Patient dashboard & doctor prescriptions
 * - Safe angle & rep limit enforcement (prevents over-exercising past doctor's max)
 * - "Report Pain / Can't Reach" button with live angle reporting to doctor
 * - In-app YouTube exercise demo video player
 * - Post-exercise motivational feedback & celebratory summary
 */

const PatientPortal = (() => {
  let session = null;
  let patient = null;
  let threshold = null;

  // ─── Rep Counting & Session State ─────────────────────────────────────────
  let repCount = 0;
  let wasAboveMin = false;
  let sessionActive = false;
  let sessionStartTime = null;
  let sessionAngles = [];
  let sessionTemps = [];
  let repTimestamps = [];
  let alertFired = false;
  let maxLimitExceeded = false;
  let painAlertsLogged = 0;

  // Selected pain level in modal
  let selectedPainLevel = 'Mild';

  // ─── Motivational Quotes Pool ─────────────────────────────────────────────
  const MOTIVATIONAL_QUOTES = [
    { quote: "Every rep is a step closer to full recovery. Consistency is your superpower!", tip: "Hydrate well and gently stretch. Allow adequate rest before your next session." },
    { quote: "Small daily improvements over time lead to stunning long-term results.", tip: "Elevate and apply gentle cold therapy if you notice mild post-exercise swelling." },
    { quote: "Your body achieves what your mind believes. Fantastic dedication today!", tip: "Maintain good posture throughout the rest of your day to protect your joint alignment." },
    { quote: "Patience and steady progress beat rushed recovery every single time.", tip: "Take 5 minutes of deep breathing to help lower muscle tension." },
    { quote: "Strength doesn't come from what you can do; it comes from overcoming the things you once couldn't.", tip: "Log your energy level and notify your doctor if joint stiffness lingers." }
  ];

  // ─── YouTube URL Helper ───────────────────────────────────────────────────
  function getYouTubeEmbedUrl(url) {
    if (!url || typeof url !== 'string') return null;
    const trimmed = url.trim();
    if (!trimmed) return null;

    // Direct embed URL already?
    if (trimmed.includes('youtube.com/embed/')) {
      return trimmed;
    }

    // Match patterns: youtube.com/watch?v=ID, youtu.be/ID, youtube.com/shorts/ID, etc.
    const regExp = /(?:youtube\.com\/(?:watch\?v=|embed\/|v\/|shorts\/)|youtu\.be\/)([a-zA-Z0-9_-]{11})/i;
    const match = trimmed.match(regExp);
    if (match && match[1]) {
      return `https://www.youtube.com/embed/${match[1]}?rel=0&modestbranding=1`;
    }
    // If already an embed or valid URL
    if (trimmed.startsWith('http://') || trimmed.startsWith('https://')) {
      return trimmed;
    }
    return null;
  }

  // ─── Init ─────────────────────────────────────────────────────────────────
  async function init(patientSession) {
    session = patientSession;
    patient = await Auth.getPatientByUserId(session.userId);
    if (patient) {
      threshold = await Auth.getThresholdForPatient(patient.id);
    }

    renderPatientDashboard();
    renderExercisePage();
    await renderPatientHistory();
    bindPatientEvents();
  }

  // ─── Render Video Demo & Exercise Page Info ────────────────────────────────
  function renderVideoDemo() {
    const card = document.getElementById('ptVideoDemoCard');
    const iframe = document.getElementById('ptVideoIframe');
    if (!card || !iframe) return;

    const videoUrl = threshold?.videoUrl || threshold?.video_url;
    const embedUrl = getYouTubeEmbedUrl(videoUrl);

    if (embedUrl) {
      if (iframe.src !== embedUrl) {
        iframe.src = embedUrl;
      }
      card.classList.remove('hidden');
    } else {
      iframe.src = '';
      card.classList.add('hidden');
    }
  }

  function renderExercisePage() {
    if (threshold) {
      const targetReps = threshold.targetReps || threshold.target_reps || 10;
      const minAngle = threshold.minAngle || threshold.min_angle || 30;
      const maxAngle = threshold.maxAngle || threshold.max_angle || 120;
      const exType = threshold.exerciseType || threshold.exercise_type || 'Knee Flexion';

      _setEl('ptExerciseTarget', `${targetReps} reps`);
      _setEl('ptExerciseName', exType);
      _setEl('ptExerciseRange', `${minAngle}° – ${maxAngle}°`);
    } else {
      _setEl('ptExerciseTarget', '— reps');
      _setEl('ptExerciseName', '—');
      _setEl('ptExerciseRange', '—° – —°');
    }
    renderVideoDemo();
  }

  // ─── Patient Dashboard ─────────────────────────────────────────────────────
  async function renderPatientDashboard() {
    _setEl('ptPatientName', session.name);

    if (!patient) {
      _setEl('ptDoctorInfo', 'Not linked to a doctor');
      return;
    }

    const doctor = await Auth.getDoctorById(patient.doctorId);
    _setEl('ptDoctorInfo', doctor ? `Dr. ${doctor.name}` : 'Unknown Doctor');

    if (threshold) {
      _setEl('ptMinAngle', threshold.minAngle + '°');
      _setEl('ptMaxAngle', threshold.maxAngle + '°');
      _setEl('ptTargetReps', threshold.targetReps + ' reps');
      _setEl('ptExercise', threshold.exerciseType || 'Exercise');
      _setEl('ptMotionLimit', threshold.motionLimit + ' °/s');
      _setEl('ptTempLimit', threshold.tempLimit + ' °C');
      document.getElementById('ptThreshCard')?.classList.remove('hidden');
      document.getElementById('ptNoThresh')?.classList.add('hidden');
    } else {
      document.getElementById('ptThreshCard')?.classList.add('hidden');
      document.getElementById('ptNoThresh')?.classList.remove('hidden');
    }

    const history = await Auth.getExerciseHistoryForPatient(patient.id);
    _setEl('ptTotalSessions', history.length);
    const completed = history.filter(h => h.status === 'completed').length;
    _setEl('ptCompliance', history.length > 0 ? Math.round(completed / history.length * 100) + '%' : '—');
    const lastSession = history[0];
    _setEl('ptLastSession', lastSession ? new Date(lastSession.date).toLocaleDateString() : 'None yet');
  }

  // ─── Rep Counting & Max Limit Enforcement ──────────────────────────────────
  function processSensorForReps(data) {
    if (!sessionActive || !threshold) return;

    const pitch = data._pitch !== undefined ? data._pitch : 0;
    const absPitch = Math.abs(pitch);
    const gyroMag = Math.sqrt((data.gx || 0) ** 2 + (data.gy || 0) ** 2 + (data.gz || 0) ** 2);
    const temp = data.tp || 0;

    sessionAngles.push(absPitch);
    sessionTemps.push(temp);

    const maxLimitAlert = document.getElementById('ptMaxLimitAlert');
    const maxLimitText = document.getElementById('ptMaxLimitText');

    // ── 1. Strict Max Angle Enforcement ──
    if (absPitch > threshold.maxAngle) {
      maxLimitExceeded = true;
      if (maxLimitAlert) {
        maxLimitAlert.classList.remove('hidden');
        if (maxLimitText) {
          maxLimitText.textContent = `⛔ DOCTOR LIMIT: Angle (${absPitch.toFixed(1)}°) exceeds maximum allowed (${threshold.maxAngle}°)! Stop movement.`;
        }
      }
    } else {
      maxLimitExceeded = false;
      if (maxLimitAlert && repCount < threshold.targetReps) {
        maxLimitAlert.classList.add('hidden');
      }
    }

    // ── 2. Rep Counting with Max Target Cap ──
    // If target reached, do not allow over-exercising past doctor's prescription
    if (repCount >= threshold.targetReps) {
      if (!alertFired) {
        alertFired = true;
        _showExerciseAlert('success', `🎉 Target reached! ${repCount}/${threshold.targetReps} reps completed. Doctor recommends finishing session.`);
        App.showToast(`🎉 Prescribed target of ${threshold.targetReps} reps completed!`, 'success');
        if (maxLimitAlert) {
          maxLimitAlert.classList.remove('hidden');
          if (maxLimitText) maxLimitText.textContent = `✓ Target Complete (${threshold.targetReps} reps). Please stop and save your session.`;
        }
      }
    } else {
      // Normal rep crossing detection
      if (absPitch >= threshold.minAngle && !maxLimitExceeded) {
        wasAboveMin = true;
      } else if (wasAboveMin && absPitch < threshold.minAngle) {
        repCount++;
        repTimestamps.push(Date.now());
        wasAboveMin = false;
        _setEl('ptRepCount', repCount);
        _setEl('ptRepCountBig', repCount);

        if (repCount >= threshold.targetReps) {
          alertFired = true;
          _showExerciseAlert('success', `🎉 Target reached! ${repCount} reps completed!`);
          App.showToast(`Great job! ${repCount} reps done!`, 'success');
        }
      }
    }

    // ── 3. Motion & Temperature Safety Alerts ──
    if (gyroMag > (threshold.motionLimit || 100)) {
      _showExerciseAlert('warning', `⚠ Motion too fast! Slow down for safety.`);
    }

    if (temp > (threshold.tempLimit || 40)) {
      _showExerciseAlert('error', `🌡 High temperature detected: ${temp.toFixed(1)}°C. Take a rest!`);
    }

    // Update live metrics
    _setEl('ptLivePitch', absPitch.toFixed(1) + '°');
    _setEl('ptLiveGyro', gyroMag.toFixed(1) + ' °/s');
    _setEl('ptLiveTemp', temp.toFixed(1) + ' °C');

    if (threshold.targetReps > 0) {
      const pct = Math.min(repCount / threshold.targetReps, 1);
      _updateRepRing(pct);
    }
  }

  function _updateRepRing(pct) {
    const ring = document.getElementById('repProgressRing');
    if (!ring) return;
    const circumference = 2 * Math.PI * 54;
    ring.style.strokeDashoffset = circumference - pct * circumference;
    ring.style.stroke = pct >= 1 ? '#22c55e' : pct > 0 ? '#00d4ff' : 'rgba(255,255,255,0.15)';
  }

  function _showExerciseAlert(type, msg) {
    const alertEl = document.getElementById('ptExerciseAlert');
    const alertText = document.getElementById('ptExerciseAlertText');
    if (!alertEl || !alertText) return;
    alertEl.className = `exercise-alert ${type}`;
    alertText.textContent = msg;
    alertEl.classList.remove('hidden');

    if (type !== 'error' && type !== 'success') {
      clearTimeout(alertEl._timeout);
      alertEl._timeout = setTimeout(() => alertEl.classList.add('hidden'), 4000);
    }
  }

  // ─── Start / Stop Exercise ────────────────────────────────────────────────
  function startExercise() {
    if (!threshold) {
      App.showToast('No thresholds set by your doctor yet. Please wait.', 'error');
      return;
    }

    sessionActive = true;
    sessionStartTime = Date.now();
    repCount = 0;
    wasAboveMin = false;
    alertFired = false;
    maxLimitExceeded = false;
    painAlertsLogged = 0;
    sessionAngles = [];
    sessionTemps = [];
    repTimestamps = [];

    _setEl('ptRepCount', 0);
    _setEl('ptRepCountBig', 0);
    _updateRepRing(0);

    document.getElementById('ptStartExercise')?.classList.add('hidden');
    document.getElementById('ptStopExercise')?.classList.remove('hidden');
    document.getElementById('ptExerciseAlert')?.classList.add('hidden');
    document.getElementById('ptMaxLimitAlert')?.classList.add('hidden');

    renderVideoDemo();

    App.showToast('Exercise session started! Follow the demonstration.', 'success');

    const timerEl = document.getElementById('ptExerciseTimer');
    window._ptTimerInterval = setInterval(() => {
      const elapsed = Date.now() - sessionStartTime;
      const m = Math.floor(elapsed / 60000);
      const s = Math.floor((elapsed % 60000) / 1000);
      if (timerEl) timerEl.textContent = `${m.toString().padStart(2, '0')}:${s.toString().padStart(2, '0')}`;
    }, 500);
  }

  async function stopExercise() {
    if (!sessionActive) return;

    sessionActive = false;
    clearInterval(window._ptTimerInterval);

    const duration_s = Math.round((Date.now() - sessionStartTime) / 1000);
    const maxAngleReached = sessionAngles.length > 0 ? Math.max(...sessionAngles) : 0;
    const minAngleReached = sessionAngles.length > 0 ? Math.min(...sessionAngles) : 0;
    const avgTemp = sessionTemps.length > 0 ? sessionTemps.reduce((a, b) => a + b, 0) / sessionTemps.length : 0;

    const pct = threshold ? repCount / threshold.targetReps : 0;
    const status = pct >= 1 ? 'completed' : pct >= 0.5 ? 'partial' : 'failed';

    document.getElementById('ptStartExercise')?.classList.remove('hidden');
    document.getElementById('ptStopExercise')?.classList.add('hidden');
    document.getElementById('ptMaxLimitAlert')?.classList.add('hidden');

    // Save record to backend & local
    if (patient) {
      try {
        await Auth.saveExerciseRecord({
          patientId: patient.id,
          doctorId: patient.doctorId,
          date: new Date().toISOString(),
          repsCompleted: repCount,
          targetReps: threshold?.targetReps || 0,
          maxAngleReached: parseFloat(maxAngleReached.toFixed(2)),
          minAngleReached: parseFloat(minAngleReached.toFixed(2)),
          avgTemp: parseFloat(avgTemp.toFixed(2)),
          duration_s,
          status,
          exerciseType: threshold?.exerciseType || 'Unknown',
        });
        App.showToast('Exercise session saved!', 'success');
        await renderPatientHistory();
      } catch (e) {
        console.error('[PatientPortal] Save error:', e);
        App.showToast('Error saving session', 'error');
      }
    }

    // Trigger Post-Exercise Motivational Modal
    showMotivationalModal(repCount, threshold?.targetReps || 0, maxAngleReached, duration_s, status);

    await renderPatientDashboard();
  }

  // ─── Motivational Celebration Modal ────────────────────────────────────────
  function showMotivationalModal(repsDone, targetReps, maxAngle, duration_s, status) {
    const modal = document.getElementById('motivationalModal');
    if (!modal) return;

    const titleEl = document.getElementById('motivationalTitle');
    const badgeEl = document.getElementById('motivationalBadge');
    const quoteEl = document.getElementById('motivationalQuote');
    const tipEl   = document.getElementById('motivationalTip');

    _setEl('mRepsDone', `${repsDone}/${targetReps}`);
    _setEl('mMaxAngle', `${maxAngle.toFixed(1)}°`);
    _setEl('mDuration', _formatDuration(duration_s));
    _setEl('mCompliance', status === 'completed' ? '100% (Full)' : status === 'partial' ? 'Partial' : 'Incomplete');

    // Select random inspirational quote
    const item = MOTIVATIONAL_QUOTES[Math.floor(Math.random() * MOTIVATIONAL_QUOTES.length)];

    if (status === 'completed') {
      if (titleEl) titleEl.textContent = '🎉 Goal Achieved!';
      if (badgeEl) {
        badgeEl.textContent = '🌟 100% Target Completed!';
        badgeEl.style.color = '#22c55e';
      }
      if (quoteEl) quoteEl.textContent = `"${item.quote}"`;
    } else if (painAlertsLogged > 0) {
      if (titleEl) titleEl.textContent = '🩹 Smart Pacing!';
      if (badgeEl) {
        badgeEl.textContent = '⚠️ Discomfort Reported to Doctor';
        badgeEl.style.color = '#f59e0b';
      }
      if (quoteEl) quoteEl.textContent = `"Listening to your body is the smartest path to lasting recovery."`;
    } else {
      if (titleEl) titleEl.textContent = '💪 Great Effort!';
      if (badgeEl) {
        badgeEl.textContent = '⭐ Every Rep Builds Strength';
        badgeEl.style.color = '#00d4ff';
      }
      if (quoteEl) quoteEl.textContent = `"${item.quote}"`;
    }

    if (tipEl) {
      tipEl.innerHTML = `💡 <strong>Recovery Tip:</strong> ${item.tip}`;
    }

    modal.classList.remove('hidden');
  }

  // ─── Pain / Limitation Alert Modal ─────────────────────────────────────────
  function openPainModal() {
    const modal = document.getElementById('painModal');
    if (!modal) return;

    const currentPitch = document.getElementById('ptLivePitch')?.textContent || '0.0°';
    _setEl('painModalAngle', currentPitch);
    _setEl('painModalReps', repCount);

    const notes = document.getElementById('painModalNotes');
    if (notes) notes.value = '';

    // Reset pain buttons
    document.querySelectorAll('.pain-level-btn').forEach(b => {
      b.classList.toggle('active', b.dataset.level === 'Mild');
    });
    selectedPainLevel = 'Mild';

    modal.classList.remove('hidden');
  }

  function closePainModal() {
    document.getElementById('painModal')?.classList.add('hidden');
  }

  async function submitPainAlert() {
    if (!patient) return;

    const angleStr = document.getElementById('painModalAngle')?.textContent || '0';
    const angle = parseFloat(angleStr) || 0;
    const notes = document.getElementById('painModalNotes')?.value.trim() || '';

    try {
      if (typeof API !== 'undefined') {
        await API.sendPainAlert(patient.id, {
          angle_at_pain: angle,
          reps_at_pain: repCount,
          pain_level: selectedPainLevel,
          notes,
        });
      }
      painAlertsLogged++;
      closePainModal();
      _showExerciseAlert('warning', `🚨 Discomfort reported to Dr. at ${angle.toFixed(1)}° (${selectedPainLevel}). Safe pacing advised.`);
      App.showToast('🚨 Alert sent to your doctor! Rest and do not force movement.', 'warning');
    } catch (e) {
      console.warn('[PatientPortal] Pain alert error:', e.message);
      closePainModal();
      App.showToast('Discomfort recorded locally. Doctor will be notified upon sync.', 'info');
    }
  }

  // ─── Patient History ───────────────────────────────────────────────────────
  async function renderPatientHistory() {
    const histEl = document.getElementById('ptHistoryList');
    if (!histEl || !patient) return;

    const history = await Auth.getExerciseHistoryForPatient(patient.id);
    const emptyEl = document.getElementById('ptHistoryEmpty');

    if (history.length === 0) {
      histEl.innerHTML = '';
      emptyEl?.classList.remove('hidden');
      return;
    }
    emptyEl?.classList.add('hidden');

    histEl.innerHTML = history.map(h => `
      <div class="history-item">
        <div class="history-meta">
          <span class="history-patient">${h.exerciseType || 'Exercise'}</span>
          <span class="history-detail">${new Date(h.date).toLocaleString()} · Duration: ${h.duration_s ? _formatDuration(h.duration_s) : '—'}</span>
        </div>
        <div class="history-stats">
          <div class="h-stat"><span class="h-stat-label">Reps</span><span class="h-stat-val">${h.repsCompleted || 0}/${h.targetReps || '?'}</span></div>
          <div class="h-stat"><span class="h-stat-label">Max Angle</span><span class="h-stat-val">${h.maxAngleReached != null ? h.maxAngleReached.toFixed(1) + '°' : '—'}</span></div>
          <div class="h-stat"><span class="h-stat-label">Avg Temp</span><span class="h-stat-val">${h.avgTemp != null ? h.avgTemp.toFixed(1) + ' °C' : '—'}</span></div>
          <div class="h-stat"><span class="h-stat-label">Status</span><span class="h-stat-val"><span class="status-badge ${h.status}">${h.status}</span></span></div>
        </div>
      </div>
    `).join('');
  }

  async function refreshThreshold() {
    if (!patient) return;
    threshold = await Auth.getThresholdForPatient(patient.id);
    await renderPatientDashboard();
    renderVideoDemo();
    await renderPatientHistory();
  }

  // ─── Bind Events ──────────────────────────────────────────────────────────
  function bindPatientEvents() {
    document.getElementById('ptStartExercise')?.addEventListener('click', startExercise);
    document.getElementById('ptStopExercise')?.addEventListener('click', stopExercise);
    document.getElementById('ptReportPainBtn')?.addEventListener('click', openPainModal);
    document.getElementById('closePainModal')?.addEventListener('click', closePainModal);
    document.getElementById('cancelPainModal')?.addEventListener('click', closePainModal);
    document.getElementById('submitPainAlert')?.addEventListener('click', submitPainAlert);

    // Pain level buttons
    document.querySelectorAll('.pain-level-btn').forEach(btn => {
      btn.addEventListener('click', () => {
        document.querySelectorAll('.pain-level-btn').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        selectedPainLevel = btn.dataset.level || 'Mild';
      });
    });

    // Motivational modal close
    document.getElementById('closeMotivationalBtn')?.addEventListener('click', () => {
      document.getElementById('motivationalModal')?.classList.add('hidden');
    });
    document.getElementById('closeMotivationalModal')?.addEventListener('click', () => {
      document.getElementById('motivationalModal')?.classList.add('hidden');
    });

    document.getElementById('ptRefreshThreshold')?.addEventListener('click', () => {
      refreshThreshold().then(() => App.showToast('Thresholds & Video refreshed from doctor', 'info'));
    });
  }

  function _setEl(id, val) {
    const el = document.getElementById(id);
    if (el) el.textContent = val;
  }

  function _formatDuration(s) {
    const m = Math.floor(s / 60);
    const sec = s % 60;
    if (m > 0) return `${m}m ${sec}s`;
    return `${sec}s`;
  }

  return {
    init,
    processSensorForReps,
    startExercise,
    stopExercise,
    renderExercisePage,
    renderPatientHistory,
    refreshThreshold,
    isSessionActive: () => sessionActive,
  };
})();
</script>
<script>/**
 * PhysioPulse – Main App Controller
 * Coordinates: auth gating, role-based routing, data pipeline, sensor UI updates, settings, demo mode
 */

const App = (() => {
  // ─── State ───────────────────────────────────────────────────────────────
  let currentPage = null;
  let userRole    = null;   // 'doctor' | 'patient'
  let userSession = null;

  let motionThreshold = 100;
  let tempThreshold   = 40;
  let demoMode        = false;
  let demoInterval    = null;
  let lastUpdateTime  = 0;
  let frameCount      = 0;
  let rateInterval    = null;
  let rate            = 0;

  // Temp stats
  let tempMin = Infinity, tempMax = -Infinity, tempSum = 0, tempCount = 0;

  // Gyro integration for yaw
  let yaw = 0;
  let lastDataTime = null;

  // ─── Toast Notification System ───────────────────────────────────────────
  function showToast(msg, type = 'info') {
    const container = document.getElementById('toastContainer');
    if (!container) return;
    const icons = { success: '✓', error: '✕', info: 'ℹ', warning: '⚠' };
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    toast.innerHTML = `<span style="font-size:1rem">${icons[type]||'ℹ'}</span> ${msg}`;
    container.appendChild(toast);
    setTimeout(() => {
      toast.style.transition = '0.3s ease';
      toast.style.opacity = '0';
      toast.style.transform = 'translateX(20px)';
      setTimeout(() => toast.remove(), 350);
    }, 3500);
  }

  // ─── Page Routing ─────────────────────────────────────────────────────────
  function navigateTo(page) {
    if (currentPage === page) return;
    currentPage = page;

    // Deactivate all nav items and pages
    document.querySelectorAll('.nav-item').forEach(el => el.classList.remove('active'));
    document.querySelectorAll('.page').forEach(el => el.classList.remove('active'));

    // Activate selected page
    const pageEl = document.getElementById('page-' + page);
    if (pageEl) pageEl.classList.add('active');

    // Activate nav item(s) matching data-page
    document.querySelectorAll(`.nav-item[data-page="${page}"]`).forEach(el => el.classList.add('active'));

    // Update topbar title
    const titles = {
      'doctor-dashboard': 'Doctor Dashboard',
      'doctor-patients':  'My Patients',
      'doctor-patient':   'Patient Detail',
      'patient-dashboard':'My Dashboard',
      'patient-exercise': 'Exercise Session',
      'patient-history':  'Exercise History',
      live:               'Live Charts',
      settings:           'Settings',
      about:              'About PhysioPulse',
    };
    const topbarTitle = document.getElementById('topbarTitle');
    if (topbarTitle) topbarTitle.textContent = titles[page] || page;

    // Page-specific actions
    if (page === 'doctor-patients') {
      _refreshDoctorPatients();
    }
    if (page === 'patient-exercise') {
      PatientPortal.renderExercisePage();
    }
    if (page === 'patient-history') {
      PatientPortal.renderPatientHistory();
    }

    closeSidebar();
  }

  async function _refreshDoctorPatients() {
    if (!userSession) return;
    const patients = await Auth.getPatientsByDoctorId(userSession.userId);
    // mirror into full list
    const el = document.getElementById('doctorPatientCardsFull');
    const codeEl = document.getElementById('doctorCodePatients');
    if (codeEl) codeEl.textContent = userSession.doctorCode || '—';
    if (el && patients.length === 0) {
      el.innerHTML = `<div class="empty-state" style="grid-column:1/-1"><div class="empty-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87M16 3.13a4 4 0 0 1 0 7.75"/></svg></div><h2>No patients yet</h2><p>Your User ID is <strong>${userSession.doctorCode || '—'}</strong>. Share it with patients so they can register and link to you.</p></div>`;
    } else if (el) {
      // Delegate rendering to DoctorPortal helper by inserting placeholder and calling render
      const cards = await Promise.all(patients.map(async (p) => {
        const threshold = await Auth.getThresholdForPatient(p.id);
        const history   = await Auth.getExerciseHistoryForPatient(p.id);
        const lastSession = history[0];
        const compliance = history.length > 0
          ? Math.round(history.filter(h => h.status === 'completed').length / history.length * 100)
          : null;
        return `
          <div class="patient-card" onclick="DoctorPortal.openPatientDetail('${p.id}')">
            <div class="pc-header">
              <div class="pc-avatar">${(p.name || 'P').charAt(0).toUpperCase()}</div>
              <div class="pc-info"><div class="pc-name">${p.name}</div><div class="pc-meta">${p.condition || 'No condition set'} · Age: ${p.age || '—'}</div></div>
              <div class="pc-compliance ${compliance === null ? '' : compliance >= 80 ? 'good' : compliance >= 50 ? 'warn' : 'bad'}">${compliance === null ? '—' : compliance + '%'}</div>
            </div>
            <div class="pc-thresholds">${threshold ? `<span class="pc-thresh-item">Min: <strong>${threshold.minAngle}°</strong></span><span class="pc-thresh-item">Max: <strong>${threshold.maxAngle}°</strong></span><span class="pc-thresh-item">Reps: <strong>${threshold.targetReps}</strong></span>` : '<span class="pc-thresh-none">No thresholds set</span>'}</div>
            <div class="pc-footer"><span class="pc-last">${lastSession ? 'Last: ' + new Date(lastSession.date).toLocaleDateString() : 'No sessions yet'}</span><span class="pc-sessions">${history.length} sessions</span></div>
          </div>
        `;
      }));
      el.innerHTML = cards.join('');
    }
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

  // ─── Auth: Show / Hide Screens ────────────────────────────────────────────
  function showAuthScreen() {
    document.getElementById('splash-screen')?.classList.add('fade-out');
    document.getElementById('auth-screen')?.classList.remove('hidden');
    document.getElementById('app')?.classList.add('hidden');
  }

  function showApp(session) {
    userSession = session;
    userRole = session.role;

    document.getElementById('auth-screen')?.classList.add('hidden');
    document.getElementById('app')?.classList.remove('hidden');

    // Update sidebar user info
    const name = session.name;
    const role = session.role;
    _setEl('sidebarUserName', name);
    _setEl('sidebarUserRole', role === 'doctor' ? '🩺 Doctor' : '🧑 Patient');
    _setEl('sidebarUserAvatar', name.charAt(0).toUpperCase());

    // Role badge in topbar
    const roleBadge = document.getElementById('roleBadge');
    if (roleBadge) {
      roleBadge.textContent = role === 'doctor' ? '🩺 Doctor' : '🧑 Patient';
      roleBadge.className = `role-badge role-${role}`;
    }

    // Show correct nav
    document.getElementById('nav-doctor')?.style.setProperty('display', role === 'doctor' ? 'block' : 'none');
    document.getElementById('nav-patient')?.style.setProperty('display', role === 'patient' ? 'block' : 'none');

    // Navigate to home page
    if (role === 'doctor') {
      navigateTo('doctor-dashboard');
      DoctorPortal.init(session);
    } else {
      navigateTo('patient-dashboard');
      PatientPortal.init(session);
      // Fill exercise info banner
      _fillExerciseBanner();
    }

    // Start sensor connection
    const host = window.location.hostname;
    const isESP32Host = host === '192.168.4.1' || host === 'physiopulse.local' || host.startsWith('192.168.4.');
    if (isESP32Host) {
      const wsUrl = `ws://${host}:81`;
      // Default to BLE since WiFi is not loading
      setTimeout(() => Connection.connect({ mode: 'ble', autoReconnect: true }), 800);
      const wsUrlEl = document.getElementById('wsUrl');
      if (wsUrlEl) wsUrlEl.value = wsUrl;
    } else {
      setTimeout(() => {
        showToast('Not connected to ESP32. Demo mode active.', 'info');
        startDemo();
      }, 1500);
    }
  }

  async function _fillExerciseBanner() {
    try {
      const patient = await Auth.getPatientByUserId(userSession.userId);
      if (!patient) return;
      const threshold = await Auth.getThresholdForPatient(patient.id);
      if (!threshold) return;
      _setEl('ptExerciseTarget', threshold.targetReps + ' reps');
      _setEl('ptExerciseName', threshold.exerciseType || 'Exercise');
      _setEl('ptExerciseRange', threshold.minAngle + '° – ' + threshold.maxAngle + '°');
      // Update motion/temp thresholds used by generic alert system too
      motionThreshold = threshold.motionLimit || 100;
      tempThreshold   = threshold.tempLimit   || 40;
    } catch (e) { /* ignore */ }
  }

  // ─── Logout ───────────────────────────────────────────────────────────────
  function logout() {
    stopDemo();
    Auth.logout();
    userSession = null;
    userRole    = null;
    currentPage = null;

    // Reset UI
    document.getElementById('app')?.classList.add('hidden');
    document.getElementById('auth-screen')?.classList.remove('hidden');

    // Show login form
    document.getElementById('auth-login')?.classList.remove('hidden');
    document.getElementById('auth-register')?.classList.add('hidden');

    showToast('Signed out successfully', 'info');
  }

  // ─── Sensor Data Processing ───────────────────────────────────────────────
  function processSensorData(raw) {
    const now = performance.now();
    const dt = lastDataTime ? (now - lastDataTime) / 1000 : 0.05;
    lastDataTime = now;

    const ax = parseFloat(raw.ax) || 0;
    const ay = parseFloat(raw.ay) || 0;
    const az = parseFloat(raw.az) || 0;
    const gx = parseFloat(raw.gx) || 0;
    const gy = parseFloat(raw.gy) || 0;
    const gz = parseFloat(raw.gz) || 0;
    const tp = parseFloat(raw.tp) || 0;

    const pitch = Math.atan2(ax, Math.sqrt(ay*ay + az*az)) * (180/Math.PI);
    const roll  = Math.atan2(ay, Math.sqrt(ax*ax + az*az)) * (180/Math.PI);
    yaw += gz * dt;
    if (yaw > 180)  yaw -= 360;
    if (yaw < -180) yaw += 360;

    const data = { ax, ay, az, gx, gy, gz, tp, _pitch: pitch, _roll: roll, _yaw: yaw };

    updateSensorUI(data);
    Charts.push(data);
    Session.addDataPoint(data);

    // Patient rep counting
    if (userRole === 'patient') {
      PatientPortal.processSensorForReps(data);
    }

    frameCount++;
    lastUpdateTime = now;
    return data;
  }

  // ─── Sensor UI Updater ────────────────────────────────────────────────────
  function updateSensorUI(d) {
    const { ax, ay, az, gx, gy, gz, tp, _pitch, _roll, _yaw } = d;

    const accelMag = Math.sqrt(ax**2 + ay**2 + az**2);
    _setEl('accelX', ax.toFixed(3));
    _setEl('accelY', ay.toFixed(3));
    _setEl('accelZ', az.toFixed(3));
    _setEl('accelMag', accelMag.toFixed(3) + ' g');
    _setBar('accelXBar', ax, 2);
    _setBar('accelYBar', ay, 2);
    _setBar('accelZBar', az, 2);

    const gyroMag = Math.sqrt(gx**2 + gy**2 + gz**2);
    _setEl('gyroX', gx.toFixed(3));
    _setEl('gyroY', gy.toFixed(3));
    _setEl('gyroZ', gz.toFixed(3));
    _setEl('gyroMag', gyroMag.toFixed(3) + ' °/s');
    _setBar('gyroXBar', gx, 250);
    _setBar('gyroYBar', gy, 250);
    _setBar('gyroZBar', gz, 250);

    _setEl('tempValue', tp.toFixed(1));
    if (tp < tempMin) tempMin = tp;
    if (tp > tempMax) tempMax = tp;
    tempSum += tp; tempCount++;
    _setEl('tempMin', tempMin.toFixed(1));
    _setEl('tempMax', tempMax.toFixed(1));
    _setEl('tempAvg', (tempSum / tempCount).toFixed(2));
    _updateTempGauge(tp);

    _setEl('pitchVal', Math.round(_pitch) + '°');
    _setEl('rollVal',  Math.round(_roll) + '°');
    _setEl('yawVal',   Math.round(_yaw) + '°');
    _setArcOffset('pitchArc', _pitch, 116);
    _setArcOffset('rollArc',  _roll,  116);
    _setArcOffset('yawArc',   _yaw,   116);

    // ROM alert (generic, overridden by patient-specific alerts during exercise)
    const alert = document.getElementById('romAlert');
    const alertText = document.getElementById('romAlertText');
    if (gyroMag > motionThreshold) {
      alert?.classList.remove('hidden');
      if (alertText) alertText.textContent = `High motion: ${gyroMag.toFixed(1)} °/s`;
    } else if (tp > tempThreshold) {
      alert?.classList.remove('hidden');
      if (alertText) alertText.textContent = `High temp: ${tp.toFixed(1)} °C`;
    } else {
      alert?.classList.add('hidden');
    }

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
    const pct = Math.max(0, Math.min(1, (temp - 20) / 30));
    fill.setAttribute('stroke-dashoffset', 220 - pct * 220);
    needle.setAttribute('transform', `rotate(${-90 + pct * 180}, 80, 90)`);
  }
  function _setArcOffset(id, angleDeg, maxDash) {
    const el = document.getElementById(id);
    if (!el) return;
    const pct = Math.max(0, Math.min(1, Math.abs(angleDeg) / 180));
    el.setAttribute('stroke-dashoffset', maxDash - pct * maxDash);
  }

  // ─── Rate Counter ──────────────────────────────────────────────────────────
  function startRateCounter() {
    rateInterval = setInterval(() => {
      rate = frameCount;
      frameCount = 0;
    }, 1000);
  }

  // ─── Demo Mode ────────────────────────────────────────────────────────────
  function startDemo() {
    demoMode = true;
    let t = 0;
    demoInterval = setInterval(() => {
      t += 0.05;
      const noise = () => (Math.random() - 0.5) * 0.02;
      processSensorData({
        ax: Math.sin(t * 0.8) * 0.3 + noise(),
        ay: Math.cos(t * 0.6) * 0.2 + noise(),
        az: 0.95 + Math.sin(t * 1.2) * 0.1 + noise(),
        gx: Math.sin(t * 1.5) * 15 + noise() * 5,
        gy: Math.cos(t * 1.1) * 10 + noise() * 5,
        gz: Math.sin(t * 0.7) * 8  + noise() * 3,
        tp: 36.5 + Math.sin(t * 0.1) * 0.5 + noise() * 0.1,
        t:  Date.now(),
        cal: true,
      });
    }, 50);
  }
  function stopDemo() {
    demoMode = false;
    clearInterval(demoInterval);
  }

  // ─── Auth Event Listeners ─────────────────────────────────────────────────
  function bindAuthEvents() {
    // Toggle login/register
    document.getElementById('goToRegister')?.addEventListener('click', (e) => {
      e.preventDefault();
      document.getElementById('auth-login').classList.add('hidden');
      document.getElementById('auth-register').classList.remove('hidden');
    });
    document.getElementById('goToLogin')?.addEventListener('click', (e) => {
      e.preventDefault();
      document.getElementById('auth-register').classList.add('hidden');
      document.getElementById('auth-login').classList.remove('hidden');
    });

    // Role selector in register
    document.querySelectorAll('.role-btn').forEach(btn => {
      btn.addEventListener('click', () => {
        document.querySelectorAll('.role-btn').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        const isDoctorCode = btn.dataset.role === 'patient';
        document.getElementById('doctorCodeGroup').style.display = isDoctorCode ? 'block' : 'none';
      });
    });

    // Login
    document.getElementById('loginBtn')?.addEventListener('click', doLogin);
    document.getElementById('loginPassword')?.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') doLogin();
    });

    // Register
    document.getElementById('registerBtn')?.addEventListener('click', doRegister);
    document.getElementById('regDoctorUserId')?.addEventListener('input', (e) => {
      e.target.value = e.target.value.toUpperCase();
    });
    document.getElementById('regUserId')?.addEventListener('input', (e) => {
      e.target.value = e.target.value.toUpperCase();
    });
    document.getElementById('loginUserId')?.addEventListener('input', (e) => {
      e.target.value = e.target.value.toUpperCase();
    });

    // Logout
    document.getElementById('logoutBtn')?.addEventListener('click', logout);
  }

  async function doLogin() {
    const loginId  = document.getElementById('loginUserId')?.value.trim().toUpperCase();
    const password = document.getElementById('loginPassword')?.value;
    const errorEl  = document.getElementById('loginError');
    const btnText  = document.getElementById('loginBtnText');
    const btnLoad  = document.getElementById('loginBtnLoader');

    if (btnText) btnText.classList.add('hidden');
    if (btnLoad) btnLoad.classList.remove('hidden');
    if (errorEl) errorEl.classList.add('hidden');

    try {
      const session = await Auth.login(loginId, password);
      showApp(session);
    } catch (e) {
      if (errorEl) { errorEl.textContent = e.message; errorEl.classList.remove('hidden'); }
    } finally {
      if (btnText) btnText.classList.remove('hidden');
      if (btnLoad) btnLoad.classList.add('hidden');
    }
  }

  async function doRegister() {
    const name         = document.getElementById('regName')?.value.trim();
    const preferredId  = document.getElementById('regUserId')?.value.trim().toUpperCase();
    const password     = document.getElementById('regPassword')?.value;
    const doctorUserId = document.getElementById('regDoctorUserId')?.value.trim().toUpperCase();
    const activeRole = document.querySelector('.role-btn.active')?.dataset.role || 'doctor';
    const errorEl    = document.getElementById('registerError');
    const btnText    = document.getElementById('registerBtnText');
    const btnLoad    = document.getElementById('registerBtnLoader');

    if (btnText) btnText.classList.add('hidden');
    if (btnLoad) btnLoad.classList.remove('hidden');
    if (errorEl) errorEl.classList.add('hidden');

    try {
      const registration = await Auth.register(activeRole, name, preferredId, password, doctorUserId);
      showToast(`Account created. Your User ID: ${registration.loginId}`, 'success');
      const session = await Auth.login(registration.loginId, password);
      showApp(session);
    } catch (e) {
      if (errorEl) { errorEl.textContent = e.message; errorEl.classList.remove('hidden'); }
    } finally {
      if (btnText) btnText.classList.remove('hidden');
      if (btnLoad) btnLoad.classList.add('hidden');
    }
  }

  // ─── App Event Listeners ──────────────────────────────────────────────────
  function bindAppEvents() {
    // Navigation – all .nav-item clicks
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

    // Connection badge click → settings
    document.getElementById('connBadge')?.addEventListener('click', () => navigateTo('settings'));

    // Settings: connection mode toggle
    document.getElementById('connModeToggle')?.addEventListener('click', (e) => {
      const btn = e.target.closest('.toggle-btn');
      if (!btn) return;
      document.querySelectorAll('.toggle-btn').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
    });

    // Settings: connect
    document.getElementById('connectBtn')?.addEventListener('click', () => {
      const modeEl = document.querySelector('.toggle-btn.active');
      const mode   = modeEl?.dataset.mode || 'wifi';
      const url    = document.getElementById('wsUrl')?.value.trim();
      const autoRec= document.getElementById('autoReconnect')?.checked;
      stopDemo();
      Connection.connect({ mode, url, autoReconnect: autoRec });
    });

    // Settings: calibrate
    document.getElementById('calibrateBtn')?.addEventListener('click', () => Connection.calibrate());

    // Settings: refresh device info
    document.getElementById('refreshDeviceInfo')?.addEventListener('click', () => Connection.fetchDeviceInfo());

    // Time window
    document.getElementById('timeWindow')?.addEventListener('change', (e) => {
      Charts.setBufferSize(parseInt(e.target.value) * 20);
    });

    // Buffer size
    document.getElementById('bufferSize')?.addEventListener('change', (e) => {
      Charts.setBufferSize(parseInt(e.target.value));
    });
  }

  // ─── Connection Handlers ───────────────────────────────────────────────────
  function setupConnectionHandlers() {
    Connection.on('data', processSensorData);
    Connection.on('status', (status) => {
      if (status === 'connected') stopDemo();
    });
    Connection.on('error', (msg) => showToast('Connection error: ' + msg, 'error'));
  }

  // ─── Splash Screen ────────────────────────────────────────────────────────
  function runSplash(onComplete) {
    const fill = document.getElementById('loaderFill');
    const text = document.getElementById('loaderText');
    const steps = [
      [20,  'Initializing…'],
      [40,  'Loading Auth…'],
      [60,  'Opening Database…'],
      [80,  'Checking Session…'],
      [100, 'Ready!'],
    ];
    let i = 0;
    const advance = () => {
      if (i >= steps.length) {
        setTimeout(() => {
          document.getElementById('splash-screen').classList.add('fade-out');
          onComplete();
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

  // ─── Init ─────────────────────────────────────────────────────────────────
  async function init() {
    try {
      await Auth.init();
      await Session.init();
      Charts.init();
      bindAuthEvents();
      bindAppEvents();
      setupConnectionHandlers();
      startRateCounter();
    } catch (e) {
      console.error('[App] Init failed:', e);
      const splash = document.getElementById('splash-screen');
      splash?.classList.add('fade-out');
      showAuthScreen();
      showToast(e.message || 'Initialization failed', 'error');
      return;
    }

    runSplash(async () => {
      // Check for existing valid session
      const existingSession = Auth.getSession();
      if (existingSession) {
        showApp(existingSession);
      } else {
        showAuthScreen();
      }
    });

    console.log('[App] PhysioPulse initialized ✓');
  }

  return { init, showToast, navigateTo, logout };
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
