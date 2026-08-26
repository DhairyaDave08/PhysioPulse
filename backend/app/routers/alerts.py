from datetime import datetime, timezone
from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import User, Patient, PainAlert
from app.schemas import PainAlertCreate, PainAlertResponse, PainAlertResolve
from app.auth import get_current_user

router = APIRouter(tags=["pain-alerts"])

def to_pain_alert_response(alert: PainAlert) -> PainAlertResponse:
    patient = alert.patient
    return PainAlertResponse(
        id=alert.id,
        patient_id=alert.patient_id,
        doctor_id=alert.doctor_id,
        patient_name=patient.name if patient else "Patient",
        date=alert.date,
        angle_at_pain=alert.angle_at_pain,
        reps_at_pain=alert.reps_at_pain,
        pain_level=alert.pain_level,
        notes=alert.notes,
        status=alert.status,
        created_at=alert.created_at,
    )

@router.post("/api/patients/{patient_id}/pain-alert", response_model=PainAlertResponse)
def create_pain_alert(
    patient_id: int,
    data: PainAlertCreate,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient = db.query(Patient).filter(Patient.id == patient_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient not found")

    alert = PainAlert(
        patient_id=patient.id,
        doctor_id=patient.doctor_id,
        date=datetime.now(timezone.utc),
        angle_at_pain=data.angle_at_pain,
        reps_at_pain=data.reps_at_pain,
        pain_level=data.pain_level or "Moderate",
        notes=data.notes or "",
        status="new",
    )
    db.add(alert)
    db.commit()
    db.refresh(alert)
    return to_pain_alert_response(alert)

@router.get("/api/patients/{patient_id}/pain-alerts", response_model=list[PainAlertResponse])
def get_patient_pain_alerts(
    patient_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    alerts = (
        db.query(PainAlert)
        .filter(PainAlert.patient_id == patient_id)
        .order_by(PainAlert.date.desc())
        .all()
    )
    return [to_pain_alert_response(a) for a in alerts]

@router.get("/api/doctor/pain-alerts", response_model=list[PainAlertResponse])
def get_doctor_pain_alerts(
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    if current_user.role != "doctor":
        raise HTTPException(status_code=status.HTTP_403_FORBIDDEN, detail="Only doctors can view doctor pain alerts")

    alerts = (
        db.query(PainAlert)
        .filter(PainAlert.doctor_id == current_user.id)
        .order_by(PainAlert.date.desc())
        .all()
    )
    return [to_pain_alert_response(a) for a in alerts]

@router.put("/api/pain-alerts/{alert_id}/resolve", response_model=PainAlertResponse)
def resolve_pain_alert(
    alert_id: int,
    data: PainAlertResolve,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    alert = db.query(PainAlert).filter(PainAlert.id == alert_id).first()
    if not alert:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Pain alert not found")

    alert.status = data.status or "reviewed"
    db.commit()
    db.refresh(alert)
    return to_pain_alert_response(alert)
