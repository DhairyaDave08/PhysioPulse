-- ============================================================================
-- PhysioPulse - Supabase PostgreSQL Schema & Seed Data
-- ============================================================================
-- You can run this script directly in the Supabase Dashboard:
-- SQL Editor -> New Query -> Paste & Run
-- ============================================================================

-- 1. Create Tables
-- ----------------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    role VARCHAR(50) NOT NULL, -- 'doctor' or 'patient'
    doctor_code VARCHAR(50),
    patient_code VARCHAR(50) UNIQUE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW())
);

CREATE INDEX IF NOT EXISTS ix_users_email ON users(email);
CREATE INDEX IF NOT EXISTS ix_users_doctor_code ON users(doctor_code);
CREATE INDEX IF NOT EXISTS ix_users_patient_code ON users(patient_code);

CREATE TABLE IF NOT EXISTS patients (
    id SERIAL PRIMARY KEY,
    user_id INTEGER UNIQUE NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    doctor_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    age INTEGER,
    condition VARCHAR(255) DEFAULT '',
    has_baseline_data BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW())
);

CREATE INDEX IF NOT EXISTS ix_patients_doctor_id ON patients(doctor_id);

CREATE TABLE IF NOT EXISTS thresholds (
    id SERIAL PRIMARY KEY,
    patient_id INTEGER NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
    doctor_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    min_angle FLOAT DEFAULT 30.0,
    max_angle FLOAT DEFAULT 120.0,
    target_reps INTEGER DEFAULT 10,
    motion_limit FLOAT DEFAULT 100.0,
    temp_limit FLOAT DEFAULT 40.0,
    exercise_type VARCHAR(255) DEFAULT 'Bicep Curl',
    video_url VARCHAR(500) DEFAULT '',
    strict_limit BOOLEAN DEFAULT TRUE,
    notes TEXT DEFAULT '',
    created_at TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW()),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW())
);

CREATE INDEX IF NOT EXISTS ix_thresholds_patient_id ON thresholds(patient_id);

CREATE TABLE IF NOT EXISTS pain_alerts (
    id SERIAL PRIMARY KEY,
    patient_id INTEGER NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
    doctor_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    date TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW()),
    angle_at_pain FLOAT,
    reps_at_pain INTEGER DEFAULT 0,
    pain_level VARCHAR(50) DEFAULT 'Moderate',
    notes TEXT DEFAULT '',
    status VARCHAR(50) DEFAULT 'new', -- 'new', 'reviewed', 'contacted'
    created_at TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW())
);

CREATE INDEX IF NOT EXISTS ix_pain_alerts_patient_id ON pain_alerts(patient_id);
CREATE INDEX IF NOT EXISTS ix_pain_alerts_doctor_id ON pain_alerts(doctor_id);
CREATE INDEX IF NOT EXISTS ix_pain_alerts_date ON pain_alerts(date);

CREATE TABLE IF NOT EXISTS exercise_history (
    id SERIAL PRIMARY KEY,
    patient_id INTEGER NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
    doctor_id INTEGER REFERENCES users(id) ON DELETE SET NULL,
    date TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW()),
    reps_completed INTEGER DEFAULT 0,
    target_reps INTEGER DEFAULT 0,
    max_angle_reached FLOAT,
    min_angle_reached FLOAT,
    avg_temp FLOAT,
    duration_s INTEGER DEFAULT 0,
    status VARCHAR(50) DEFAULT 'completed', -- 'completed', 'partial', 'failed'
    exercise_type VARCHAR(255) DEFAULT 'Exercise',
    notes TEXT DEFAULT ''
);

CREATE INDEX IF NOT EXISTS ix_exercise_history_patient_id ON exercise_history(patient_id);
CREATE INDEX IF NOT EXISTS ix_exercise_history_date ON exercise_history(date);

CREATE TABLE IF NOT EXISTS sensor_sessions (
    id SERIAL PRIMARY KEY,
    patient_name VARCHAR(255) NOT NULL,
    exercise VARCHAR(255) DEFAULT 'Unknown',
    notes TEXT DEFAULT '',
    user_id INTEGER REFERENCES users(id) ON DELETE SET NULL,
    patient_id INTEGER REFERENCES patients(id) ON DELETE SET NULL,
    date TIMESTAMP WITH TIME ZONE DEFAULT TIMEZONE('utc', NOW()),
    duration_s INTEGER DEFAULT 0,
    interval_ms INTEGER DEFAULT 50,
    stats_json TEXT DEFAULT '{}',
    rows_json TEXT DEFAULT '[]'
);

CREATE INDEX IF NOT EXISTS ix_sensor_sessions_user_id ON sensor_sessions(user_id);
CREATE INDEX IF NOT EXISTS ix_sensor_sessions_patient_id ON sensor_sessions(patient_id);
CREATE INDEX IF NOT EXISTS ix_sensor_sessions_date ON sensor_sessions(date);

-- ----------------------------------------------------------------------------
-- 2. Seed Default Demo Accounts
-- Passwords:
-- Doctor:  doctor@physiopulse.com / doctor123
-- Patient: john@example.com / patient123
-- Patient: jane@example.com / patient123
-- ----------------------------------------------------------------------------

-- Seed Doctor (Dr. Sarah Jenkins)
INSERT INTO users (id, name, email, password_hash, role, doctor_code, created_at)
VALUES (
    1,
    'Dr. Sarah Jenkins',
    'doctor@physiopulse.com',
    '$2b$12$1Qnvrg2UF9/5T/GnizbRQOy3N/r/I2mfNJRSU5fwPupkR1m2N45ay', -- bcrypt for 'doctor123'
    'doctor',
    'DR0001',
    NOW()
) ON CONFLICT (email) DO NOTHING;

-- Seed Patient User 1 (John Doe)
INSERT INTO users (id, name, email, password_hash, role, created_at)
VALUES (
    2,
    'John Doe',
    'john@example.com',
    '$2b$12$1/7Ws7P8931A.JR.GBxoOeKVKemI6UTGLnD3ud7BfrqzjUpmsR3/6', -- bcrypt for 'patient123'
    'patient',
    NOW()
) ON CONFLICT (email) DO NOTHING;

-- Seed Patient User 2 (Jane Smith)
INSERT INTO users (id, name, email, password_hash, role, created_at)
VALUES (
    3,
    'Jane Smith',
    'jane@example.com',
    '$2b$12$1/7Ws7P8931A.JR.GBxoOeKVKemI6UTGLnD3ud7BfrqzjUpmsR3/6', -- bcrypt for 'patient123'
    'patient',
    NOW()
) ON CONFLICT (email) DO NOTHING;

-- Seed Patient Profiles
INSERT INTO patients (id, user_id, doctor_id, name, age, condition, has_baseline_data, created_at)
VALUES 
    (1, 2, 1, 'John Doe', 42, 'Hand & Arm Mobility Rehabilitation', TRUE, NOW()),
    (2, 3, 1, 'Jane Smith', 36, 'Wrist Extension & Mobility Protocol', TRUE, NOW())
ON CONFLICT (id) DO NOTHING;

-- Seed Prescription Threshold for John Doe (Bicep Curl)
INSERT INTO thresholds (patient_id, doctor_id, min_angle, max_angle, target_reps, motion_limit, temp_limit, exercise_type, video_url, strict_limit, notes)
VALUES 
    (1, 1, 30.0, 110.0, 12, 120.0, 39.5, 'Bicep Curl', 'https://www.youtube.com/watch?v=in7PaeYlhrM', TRUE, 'Controlled bicep curl. Keep wrist aligned.')
ON CONFLICT DO NOTHING;

-- Seed Prescription Threshold for Jane Smith (Wrist Circle)
INSERT INTO thresholds (patient_id, doctor_id, min_angle, max_angle, target_reps, motion_limit, temp_limit, exercise_type, video_url, strict_limit, notes)
VALUES 
    (2, 1, 15.0, 85.0, 10, 90.0, 39.0, 'Wrist Circle / Rotation', 'https://www.youtube.com/watch?v=in7PaeYlhrM', TRUE, 'Gentle rotational mobility.')
ON CONFLICT DO NOTHING;

-- Reset sequence counters so subsequent SERIAL inserts don't collide
SELECT setval('users_id_seq', (SELECT COALESCE(MAX(id), 1) FROM users));
SELECT setval('patients_id_seq', (SELECT COALESCE(MAX(id), 1) FROM patients));
SELECT setval('thresholds_id_seq', (SELECT COALESCE(MAX(id), 1) FROM thresholds));
SELECT setval('pain_alerts_id_seq', (SELECT COALESCE(MAX(id), 1) FROM pain_alerts));
SELECT setval('exercise_history_id_seq', (SELECT COALESCE(MAX(id), 1) FROM exercise_history));
SELECT setval('sensor_sessions_id_seq', (SELECT COALESCE(MAX(id), 1) FROM sensor_sessions));
