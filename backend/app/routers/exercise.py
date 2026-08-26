from datetime import datetime, timezone
from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import User, Patient, ExerciseHistory
from app.schemas import ExerciseHistoryCreate, ExerciseHistoryResponse
from app.auth import get_current_user

router = APIRouter(tags=["exercise-history"])

@router.post("/api/exercise-history", response_model=ExerciseHistoryResponse)
def save_exercise_record(
    record: ExerciseHistoryCreate,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient_id = record.patient_id
    if not patient_id:
        if current_user.role == "patient" and current_user.patient_profile:
            patient_id = current_user.patient_profile.id
        else:
            raise HTTPException(
                status_code=status.HTTP_400_BAD_REQUEST,
                detail="patient_id is required"
            )

    patient = db.query(Patient).filter(Patient.id == patient_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient not found")

    # Determine status if not already calculated
    status_val = record.status
    if not status_val:
        if record.target_reps > 0:
            ratio = record.reps_completed / record.target_reps
            if ratio >= 1.0:
                status_val = "completed"
            elif ratio >= 0.5:
                status_val = "partial"
            else:
                status_val = "failed"
        else:
            status_val = "completed"

    entry = ExerciseHistory(
        patient_id=patient.id,
        doctor_id=patient.doctor_id,
        date=record.date or datetime.now(timezone.utc),
        reps_completed=record.reps_completed,
        target_reps=record.target_reps,
        max_angle_reached=record.max_angle_reached,
        min_angle_reached=record.min_angle_reached,
        avg_temp=record.avg_temp,
        duration_s=record.duration_s,
        status=status_val,
        exercise_type=record.exercise_type or "Exercise",
        notes=record.notes or "",
    )
    db.add(entry)
    db.commit()
    db.refresh(entry)
    return ExerciseHistoryResponse.model_validate(entry)

@router.get("/api/patients/{patient_id}/exercise-history", response_model=list[ExerciseHistoryResponse])
def get_patient_exercise_history(
    patient_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    records = (
        db.query(ExerciseHistory)
        .filter(ExerciseHistory.patient_id == patient_id)
        .order_by(ExerciseHistory.date.desc())
        .all()
    )
    return [ExerciseHistoryResponse.model_validate(r) for r in records]

@router.get("/api/exercise-history", response_model=list[ExerciseHistoryResponse])
def get_all_accessible_exercise_history(
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    if current_user.role == "patient" and current_user.patient_profile:
        records = (
            db.query(ExerciseHistory)
            .filter(ExerciseHistory.patient_id == current_user.patient_profile.id)
            .order_by(ExerciseHistory.date.desc())
            .all()
        )
    elif current_user.role == "doctor":
        records = (
            db.query(ExerciseHistory)
            .filter(ExerciseHistory.doctor_id == current_user.id)
            .order_by(ExerciseHistory.date.desc())
            .all()
        )
    else:
        records = []
    return [ExerciseHistoryResponse.model_validate(r) for r in records]

@router.get("/api/patients/{patient_id}/analytics")
def get_patient_analytics(
    patient_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient = db.query(Patient).filter(Patient.id == patient_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient not found")

    records = (
        db.query(ExerciseHistory)
        .filter(ExerciseHistory.patient_id == patient_id)
        .order_by(ExerciseHistory.date.asc())
        .all()
    )

    total_sessions = len(records)
    completed_sessions = sum(1 for r in records if r.status == "completed")
    compliance_rate = round((completed_sessions / total_sessions * 100), 1) if total_sessions > 0 else 0
    total_reps = sum(r.reps_completed for r in records)

    rom_progression = [
        {
            "date": r.date.isoformat(),
            "max_angle": r.max_angle_reached,
            "min_angle": r.min_angle_reached,
            "reps": r.reps_completed,
            "status": r.status,
        }
        for r in records
    ]

    return {
        "patient_id": patient.id,
        "patient_name": patient.name,
        "total_sessions": total_sessions,
        "completed_sessions": completed_sessions,
        "compliance_rate": compliance_rate,
        "total_reps": total_reps,
        "rom_progression": rom_progression,
    }
