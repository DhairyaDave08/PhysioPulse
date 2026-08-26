import json
from datetime import datetime, timezone
from io import StringIO
from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, status, Response
from fastapi.responses import StreamingResponse
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import User, Patient, SensorSession
from app.schemas import SensorSessionCreate, SensorSessionSummary, SensorSessionDetail
from app.auth import get_current_user_optional, get_current_user

router = APIRouter(prefix="/api/sessions", tags=["sessions"])

def to_summary(s: SensorSession) -> SensorSessionSummary:
    try:
        stats = json.loads(s.stats_json) if s.stats_json else {}
    except Exception:
        stats = {}
    try:
        rows = json.loads(s.rows_json) if s.rows_json else []
        pts = len(rows)
    except Exception:
        pts = 0
    return SensorSessionSummary(
        id=s.id,
        patient_name=s.patient_name,
        exercise=s.exercise,
        notes=s.notes or "",
        user_id=s.user_id,
        patient_id=s.patient_id,
        date=s.date,
        duration_s=s.duration_s,
        interval_ms=s.interval_ms,
        stats=stats,
        data_points=pts,
    )

@router.post("", response_model=SensorSessionSummary)
def save_session(
    data: SensorSessionCreate,
    current_user: Optional[User] = Depends(get_current_user_optional),
    db: Session = Depends(get_db)
):
    patient_name = data.patient_name or data.patient or (current_user.name if current_user else "Unknown")
    user_id = current_user.id if current_user else None
    patient_id = data.patient_id

    if not patient_id and current_user and current_user.role == "patient" and current_user.patient_profile:
        patient_id = current_user.patient_profile.id

    stats_str = json.dumps(data.stats or {})
    rows_str = json.dumps(data.rows or [])

    new_session = SensorSession(
        patient_name=patient_name,
        exercise=data.exercise or "Unknown",
        notes=data.notes or "",
        user_id=user_id,
        patient_id=patient_id,
        date=data.date or datetime.now(timezone.utc),
        duration_s=data.duration_s or 0,
        interval_ms=data.interval_ms or 50,
        stats_json=stats_str,
        rows_json=rows_str,
    )
    db.add(new_session)
    db.commit()
    db.refresh(new_session)
    return to_summary(new_session)

@router.get("", response_model=list[SensorSessionSummary])
def list_sessions(
    patient_id: Optional[int] = None,
    current_user: Optional[User] = Depends(get_current_user_optional),
    db: Session = Depends(get_db)
):
    query = db.query(SensorSession)
    if patient_id:
        query = query.filter(SensorSession.patient_id == patient_id)
    elif current_user:
        if current_user.role == "patient" and current_user.patient_profile:
            query = query.filter(
                (SensorSession.patient_id == current_user.patient_profile.id) |
                (SensorSession.user_id == current_user.id)
            )
    sessions = query.order_by(SensorSession.date.desc()).all()
    return [to_summary(s) for s in sessions]

@router.get("/{session_id}", response_model=SensorSessionDetail)
def get_session_detail(
    session_id: int,
    db: Session = Depends(get_db)
):
    session_obj = db.query(SensorSession).filter(SensorSession.id == session_id).first()
    if not session_obj:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Session not found")

    try:
        stats = json.loads(session_obj.stats_json) if session_obj.stats_json else {}
    except Exception:
        stats = {}
    try:
        rows = json.loads(session_obj.rows_json) if session_obj.rows_json else []
    except Exception:
        rows = []

    return SensorSessionDetail(
        id=session_obj.id,
        patient_name=session_obj.patient_name,
        exercise=session_obj.exercise,
        notes=session_obj.notes or "",
        user_id=session_obj.user_id,
        patient_id=session_obj.patient_id,
        date=session_obj.date,
        duration_s=session_obj.duration_s,
        interval_ms=session_obj.interval_ms,
        stats=stats,
        data_points=len(rows),
        rows=rows,
    )

@router.get("/{session_id}/csv")
def download_session_csv(
    session_id: int,
    db: Session = Depends(get_db)
):
    session_obj = db.query(SensorSession).filter(SensorSession.id == session_id).first()
    if not session_obj:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Session not found")

    try:
        rows = json.loads(session_obj.rows_json) if session_obj.rows_json else []
    except Exception:
        rows = []

    output = StringIO()
    output.write("# PhysioPulse Session Export\n")
    output.write(f"# Patient: {session_obj.patient_name}\n")
    output.write(f"# Exercise: {session_obj.exercise}\n")
    output.write(f"# Date: {session_obj.date.isoformat()}\n")
    output.write(f"# Notes: {session_obj.notes or ''}\n")
    output.write("timestamp_ms,accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,gyro_z_dps,temperature_c\n")

    for i, r in enumerate(rows):
        t = r.get("t", i * session_obj.interval_ms)
        ax = r.get("ax", 0)
        ay = r.get("ay", 0)
        az = r.get("az", 0)
        gx = r.get("gx", 0)
        gy = r.get("gy", 0)
        gz = r.get("gz", 0)
        tp = r.get("tp", 0)
        output.write(f"{t},{ax},{ay},{az},{gx},{gy},{gz},{tp}\n")

    output.seek(0)
    filename = f"PhysioPulse_{session_obj.patient_name.replace(' ', '_')}_{session_obj.date.strftime('%Y%m%d_%H%M%S')}.csv"
    return StreamingResponse(
        iter([output.getvalue()]),
        media_type="text/csv",
        headers={"Content-Disposition": f'attachment; filename="{filename}"'}
    )

@router.delete("/{session_id}")
def delete_session(
    session_id: int,
    db: Session = Depends(get_db)
):
    session_obj = db.query(SensorSession).filter(SensorSession.id == session_id).first()
    if not session_obj:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Session not found")
    db.delete(session_obj)
    db.commit()
    return {"message": "Session deleted successfully"}

@router.delete("")
def delete_all_sessions(db: Session = Depends(get_db)):
    db.query(SensorSession).delete()
    db.commit()
    return {"message": "All sessions deleted"}
