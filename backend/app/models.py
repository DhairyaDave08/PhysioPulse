from datetime import datetime, timezone
from sqlalchemy import (
    Column, Integer, String, Float, Boolean, DateTime, ForeignKey, Text
)
from sqlalchemy.orm import relationship
from app.database import Base

def utc_now():
    return datetime.now(timezone.utc)

class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String(255), nullable=False)
    email = Column(String(255), unique=True, index=True, nullable=False)
    password_hash = Column(String(255), nullable=False)
    role = Column(String(50), nullable=False)  # "doctor" or "patient"
    doctor_code = Column(String(50), nullable=True, index=True)
    patient_code = Column(String(50), nullable=True, index=True, unique=True)  # For doctors, e.g. "DR0001"
    created_at = Column(DateTime, default=utc_now)

    # Relationships
    patient_profile = relationship("Patient", back_populates="user", uselist=False, foreign_keys="Patient.user_id")
    assigned_patients = relationship("Patient", back_populates="doctor", foreign_keys="Patient.doctor_id")

class Patient(Base):
    __tablename__ = "patients"

    id = Column(Integer, primary_key=True, index=True)
    user_id = Column(Integer, ForeignKey("users.id"), unique=True, nullable=False)
    doctor_id = Column(Integer, ForeignKey("users.id"), nullable=False, index=True)
    name = Column(String(255), nullable=False)
    age = Column(Integer, nullable=True)
    condition = Column(String(255), default="")
    has_baseline_data = Column(Boolean, default=False)
    created_at = Column(DateTime, default=utc_now)

    # Relationships
    user = relationship("User", foreign_keys=[user_id], back_populates="patient_profile")
    doctor = relationship("User", foreign_keys=[doctor_id], back_populates="assigned_patients")
    thresholds = relationship("Threshold", back_populates="patient", cascade="all, delete-orphan")
    exercise_history = relationship("ExerciseHistory", back_populates="patient", cascade="all, delete-orphan")
    sensor_sessions = relationship("SensorSession", back_populates="patient", cascade="all, delete-orphan")
    pain_alerts = relationship("PainAlert", back_populates="patient", cascade="all, delete-orphan")

class Threshold(Base):
    __tablename__ = "thresholds"

    id = Column(Integer, primary_key=True, index=True)
    patient_id = Column(Integer, ForeignKey("patients.id"), nullable=False, index=True)
    doctor_id = Column(Integer, ForeignKey("users.id"), nullable=False)
    min_angle = Column(Float, default=30.0)
    max_angle = Column(Float, default=120.0)
    target_reps = Column(Integer, default=10)
    motion_limit = Column(Float, default=100.0)
    temp_limit = Column(Float, default=40.0)
    exercise_type = Column(String(255), default="Bicep Curl")
    video_url = Column(String(500), nullable=True)  # YouTube demo link
    strict_limit = Column(Boolean, default=True)    # Don't exceed max angle / reps
    notes = Column(Text, default="")
    created_at = Column(DateTime, default=utc_now)
    updated_at = Column(DateTime, default=utc_now, onupdate=utc_now)

    # Relationships
    patient = relationship("Patient", back_populates="thresholds")
    doctor = relationship("User", foreign_keys=[doctor_id])

class PainAlert(Base):
    __tablename__ = "pain_alerts"

    id = Column(Integer, primary_key=True, index=True)
    patient_id = Column(Integer, ForeignKey("patients.id"), nullable=False, index=True)
    doctor_id = Column(Integer, ForeignKey("users.id"), nullable=False, index=True)
    date = Column(DateTime, default=utc_now, index=True)
    angle_at_pain = Column(Float, nullable=True)
    reps_at_pain = Column(Integer, default=0)
    pain_level = Column(String(50), default="Moderate")  # e.g. "Mild", "Moderate", "Severe" or 1-10
    notes = Column(Text, default="")
    status = Column(String(50), default="new")  # "new", "reviewed", "contacted"
    created_at = Column(DateTime, default=utc_now)

    # Relationships
    patient = relationship("Patient", back_populates="pain_alerts")
    doctor = relationship("User", foreign_keys=[doctor_id])

class ExerciseHistory(Base):
    __tablename__ = "exercise_history"

    id = Column(Integer, primary_key=True, index=True)
    patient_id = Column(Integer, ForeignKey("patients.id"), nullable=False, index=True)
    doctor_id = Column(Integer, ForeignKey("users.id"), nullable=True)
    date = Column(DateTime, default=utc_now, index=True)
    reps_completed = Column(Integer, default=0)
    target_reps = Column(Integer, default=0)
    max_angle_reached = Column(Float, nullable=True)
    min_angle_reached = Column(Float, nullable=True)
    avg_temp = Column(Float, nullable=True)
    duration_s = Column(Integer, default=0)
    status = Column(String(50), default="completed")  # "completed", "partial", "failed"
    exercise_type = Column(String(255), default="Exercise")
    notes = Column(Text, default="")

    # Relationships
    patient = relationship("Patient", back_populates="exercise_history")

class SensorSession(Base):
    __tablename__ = "sensor_sessions"

    id = Column(Integer, primary_key=True, index=True)
    patient_name = Column(String(255), nullable=False)
    exercise = Column(String(255), default="Unknown")
    notes = Column(Text, default="")
    user_id = Column(Integer, ForeignKey("users.id"), nullable=True, index=True)
    patient_id = Column(Integer, ForeignKey("patients.id"), nullable=True, index=True)
    date = Column(DateTime, default=utc_now, index=True)
    duration_s = Column(Integer, default=0)
    interval_ms = Column(Integer, default=50)
    stats_json = Column(Text, default="{}")  # JSON string
    rows_json = Column(Text, default="[]")   # JSON array string

    # Relationships
    patient = relationship("Patient", back_populates="sensor_sessions")
