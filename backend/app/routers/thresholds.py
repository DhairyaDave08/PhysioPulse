from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import User, Patient, Threshold
from app.schemas import ThresholdCreate, ThresholdResponse
from app.auth import get_current_user

router = APIRouter(prefix="/api/patients", tags=["thresholds"])

@router.get("/{patient_id}/threshold", response_model=Optional[ThresholdResponse])
def get_active_threshold(
    patient_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    threshold = (
        db.query(Threshold)
        .filter(Threshold.patient_id == patient_id)
        .order_by(Threshold.updated_at.desc())
        .first()
    )
    if not threshold:
        return None
    return ThresholdResponse.model_validate(threshold)

@router.post("/{patient_id}/threshold", response_model=ThresholdResponse)
def save_threshold(
    patient_id: int,
    data: ThresholdCreate,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient = db.query(Patient).filter(Patient.id == patient_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient not found")

    if data.min_angle >= data.max_angle:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Min angle must be strictly less than max angle"
        )

    # Doctor ID: either current logged in doctor or the assigned doctor
    doctor_id = current_user.id if current_user.role == "doctor" else patient.doctor_id

    # Create new threshold record for history tracking
    threshold = Threshold(
        patient_id=patient.id,
        doctor_id=doctor_id,
        min_angle=data.min_angle,
        max_angle=data.max_angle,
        target_reps=data.target_reps,
        motion_limit=data.motion_limit,
        temp_limit=data.temp_limit,
        exercise_type=data.exercise_type,
        video_url=data.video_url or "",
        strict_limit=data.strict_limit if data.strict_limit is not None else True,
        notes=data.notes or "",
    )
    db.add(threshold)

    # Update patient baseline flag and condition if not set
    patient.has_baseline_data = True
    if not patient.condition:
        patient.condition = data.exercise_type

    db.commit()
    db.refresh(threshold)
    return ThresholdResponse.model_validate(threshold)

@router.get("/{patient_id}/thresholds", response_model=list[ThresholdResponse])
def get_threshold_history(
    patient_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    thresholds = (
        db.query(Threshold)
        .filter(Threshold.patient_id == patient_id)
        .order_by(Threshold.updated_at.desc())
        .all()
    )
    return [ThresholdResponse.model_validate(t) for t in thresholds]
