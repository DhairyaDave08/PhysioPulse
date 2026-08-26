from datetime import datetime
from typing import Optional, Any
from pydantic import BaseModel, EmailStr, Field, ConfigDict

# ─── Auth Schemas ────────────────────────────────────────────────────────────
class UserRegister(BaseModel):
    name: str = Field(..., min_length=1, max_length=255)
    email: EmailStr
    password: str = Field(..., min_length=6)
    role: str = Field("doctor", pattern="^(doctor|patient)$")
    doctor_code: Optional[str] = None
    patient_code: Optional[str] = None

class UserLogin(BaseModel):
    email: str
    password: str

class UserResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    name: str
    email: str
    role: str
    doctor_code: Optional[str] = None
    patient_code: Optional[str] = None
    created_at: Optional[datetime] = None

class AuthResponse(BaseModel):
    access_token: str
    token_type: str = "bearer"
    user: UserResponse
    patient_id: Optional[int] = None
    doctor_id: Optional[int] = None

# ─── Patient Schemas ─────────────────────────────────────────────────────────

class DoctorCreatePatient(BaseModel):
    name: str = Field(..., min_length=1, max_length=255)
    age: Optional[int] = None
    condition: Optional[str] = None
    password: str = Field(..., min_length=6)

class PasswordReset(BaseModel):
    password: str = Field(..., min_length=6)

class PatientUpdate(BaseModel):
    name: Optional[str] = None
    age: Optional[int] = None
    condition: Optional[str] = None
    has_baseline_data: Optional[bool] = None

class PatientResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    user_id: int
    doctor_id: int
    name: str
    age: Optional[int] = None
    condition: Optional[str] = None
    has_baseline_data: bool = False
    created_at: Optional[datetime] = None
    doctor_name: Optional[str] = None
    doctor_code: Optional[str] = None
    patient_code: Optional[str] = None

# ─── Threshold Schemas ───────────────────────────────────────────────────────
class ThresholdCreate(BaseModel):
    min_angle: float = Field(30.0, ge=0, le=180)
    max_angle: float = Field(120.0, ge=0, le=180)
    target_reps: int = Field(10, ge=1, le=1000)
    motion_limit: float = Field(100.0, ge=0)
    temp_limit: float = Field(40.0, ge=0)
    exercise_type: str = "Bicep Curl"
    video_url: Optional[str] = None
    strict_limit: bool = True
    notes: Optional[str] = ""

class ThresholdResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    patient_id: int
    doctor_id: int
    min_angle: float
    max_angle: float
    target_reps: int
    motion_limit: float
    temp_limit: float
    exercise_type: str
    video_url: Optional[str] = None
    strict_limit: bool = True
    notes: Optional[str] = ""
    created_at: Optional[datetime] = None
    updated_at: Optional[datetime] = None

# ─── Pain Alert Schemas ─────────────────────────────────────────────────────
class PainAlertCreate(BaseModel):
    patient_id: Optional[int] = None
    angle_at_pain: Optional[float] = None
    reps_at_pain: int = 0
    pain_level: str = "Moderate"
    notes: Optional[str] = ""

class PainAlertResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    patient_id: int
    doctor_id: int
    patient_name: Optional[str] = None
    date: datetime
    angle_at_pain: Optional[float] = None
    reps_at_pain: int = 0
    pain_level: str
    notes: Optional[str] = ""
    status: str = "new"
    created_at: Optional[datetime] = None

class PainAlertResolve(BaseModel):
    status: str = "reviewed"

# ─── Exercise History Schemas ───────────────────────────────────────────────
class ExerciseHistoryCreate(BaseModel):
    patient_id: Optional[int] = None
    date: Optional[datetime] = None
    reps_completed: int = 0
    target_reps: int = 0
    max_angle_reached: Optional[float] = None
    min_angle_reached: Optional[float] = None
    avg_temp: Optional[float] = None
    duration_s: int = 0
    status: str = "completed"  # completed, partial, failed
    exercise_type: Optional[str] = "Exercise"
    notes: Optional[str] = ""

class ExerciseHistoryResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    patient_id: int
    doctor_id: Optional[int] = None
    date: datetime
    reps_completed: int
    target_reps: int
    max_angle_reached: Optional[float] = None
    min_angle_reached: Optional[float] = None
    avg_temp: Optional[float] = None
    duration_s: int
    status: str
    exercise_type: str
    notes: Optional[str] = ""

# ─── Sensor Session Schemas ─────────────────────────────────────────────────
class SensorSessionCreate(BaseModel):
    patient: Optional[str] = None
    patient_name: Optional[str] = None
    exercise: Optional[str] = "Unknown"
    notes: Optional[str] = ""
    patient_id: Optional[int] = None
    date: Optional[datetime] = None
    duration_s: Optional[int] = 0
    interval_ms: Optional[int] = 50
    stats: Optional[dict[str, Any]] = None
    rows: Optional[list[dict[str, Any]]] = None

class SensorSessionSummary(BaseModel):
    id: int
    patient_name: str
    exercise: str
    notes: Optional[str] = ""
    user_id: Optional[int] = None
    patient_id: Optional[int] = None
    date: datetime
    duration_s: int
    interval_ms: int
    stats: Optional[dict[str, Any]] = None
    data_points: int = 0

class SensorSessionDetail(SensorSessionSummary):
    rows: list[dict[str, Any]] = []
