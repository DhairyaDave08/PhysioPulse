from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import User, Patient
from app.schemas import PatientResponse, PatientUpdate
from app.auth import get_current_user
from app.schemas import DoctorCreatePatient, PasswordReset, UserResponse
from app.auth import hash_password

router = APIRouter(prefix="/api/patients", tags=["patients"])


def generate_patient_code(user_id: int) -> str:
    return f"PT-{user_id:04d}"

@router.post("/create", response_model=UserResponse)
def create_patient(
    patient_data: DoctorCreatePatient,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    if current_user.role != "doctor":
        raise HTTPException(status_code=403, detail="Only doctors can create patients")

    # Create dummy email for patient (since email is unique)
    dummy_email = f"patient_{current_user.id}_{patient_data.name.lower().replace(' ', '')}@physiopulse.local"
    # Ensure it's unique
    import time
    dummy_email = f"{int(time.time())}_{dummy_email}"
    
    new_user = User(
        name=patient_data.name.strip(),
        email=dummy_email,
        password_hash=hash_password(patient_data.password),
        role="patient",
    )
    db.add(new_user)
    db.flush()
    
    new_user.patient_code = generate_patient_code(new_user.id)
    db.flush()
    
    patient_profile = Patient(
        user_id=new_user.id,
        doctor_id=current_user.id,
        name=patient_data.name.strip(),
        age=patient_data.age,
        condition=patient_data.condition or "",
        has_baseline_data=False,
    )
    db.add(patient_profile)
    db.commit()
    db.refresh(new_user)
    return UserResponse.model_validate(new_user)

@router.put("/{patient_id}/reset-password")
def reset_patient_password(
    patient_id: int,
    reset_data: PasswordReset,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    if current_user.role != "doctor":
        raise HTTPException(status_code=403, detail="Only doctors can reset passwords")
        
    patient = db.query(Patient).filter(Patient.id == patient_id, Patient.doctor_id == current_user.id).first()
    if not patient:
        raise HTTPException(status_code=404, detail="Patient not found or not assigned to you")
        
    user = db.query(User).filter(User.id == patient.user_id).first()
    if not user:
        raise HTTPException(status_code=404, detail="User account not found")
        
    user.password_hash = hash_password(reset_data.password)
    db.commit()
    return {"message": "Password reset successfully"}




def to_patient_response(patient: Patient) -> PatientResponse:
    doctor = patient.doctor
    return PatientResponse(
        id=patient.id,
        user_id=patient.user_id,
        doctor_id=patient.doctor_id,
        name=patient.name,
        age=patient.age,
        condition=patient.condition,
        has_baseline_data=patient.has_baseline_data,
        created_at=patient.created_at,
        doctor_name=doctor.name if doctor else None,
        doctor_code=doctor.doctor_code if doctor else None,
        patient_code=patient.user.patient_code if patient.user else None,
    )

@router.get("", response_model=list[PatientResponse])
def list_patients(
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    if current_user.role == "doctor":
        patients = db.query(Patient).filter(Patient.doctor_id == current_user.id).all()
        return [to_patient_response(p) for p in patients]
    elif current_user.role == "patient":
        patient = db.query(Patient).filter(Patient.user_id == current_user.id).first()
        return [to_patient_response(patient)] if patient else []
    else:
        return []

@router.get("/by-user/{user_id}", response_model=PatientResponse)
def get_patient_by_user_id(
    user_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient = db.query(Patient).filter(Patient.user_id == user_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient profile not found")
    return to_patient_response(patient)

@router.get("/{patient_id}", response_model=PatientResponse)
def get_patient(
    patient_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient = db.query(Patient).filter(Patient.id == patient_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient not found")
    return to_patient_response(patient)

@router.put("/{patient_id}", response_model=PatientResponse)
def update_patient(
    patient_id: int,
    updates: PatientUpdate,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    patient = db.query(Patient).filter(Patient.id == patient_id).first()
    if not patient:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Patient not found")

    if updates.name is not None:
        patient.name = updates.name.strip()
    if updates.age is not None:
        patient.age = updates.age
    if updates.condition is not None:
        patient.condition = updates.condition.strip()
    if updates.has_baseline_data is not None:
        patient.has_baseline_data = updates.has_baseline_data

    db.commit()
    db.refresh(patient)
    return to_patient_response(patient)
