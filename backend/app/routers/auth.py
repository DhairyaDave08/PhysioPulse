from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import User, Patient
from app.schemas import UserRegister, UserLogin, AuthResponse, UserResponse
from app.auth import hash_password, verify_password, create_access_token, get_current_user

router = APIRouter(prefix="/api/auth", tags=["auth"])

def generate_doctor_code(user_id: int) -> str:
    return f"DR{user_id:04d}"

@router.post("/register", response_model=AuthResponse)
def register(user_data: UserRegister, db: Session = Depends(get_db)):
    # Public self-registration is closed. Doctor accounts are issued directly by the organization.
    raise HTTPException(
        status_code=status.HTTP_403_FORBIDDEN,
        detail="Public registration is closed. Doctor accounts are issued directly by the company, and patients are created by their assigned doctor."
    )

    email_clean = user_data.email.lower().strip()
    existing = db.query(User).filter(User.email == email_clean).first()
    if existing:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="An account with this email already exists"
        )

    doctor: Optional[User] = None
    if user_data.role == "patient":
        if not user_data.doctor_code or not user_data.doctor_code.strip():
            raise HTTPException(
                status_code=status.HTTP_400_BAD_REQUEST,
                detail="Doctor Code is required for patient registration"
            )
        code = user_data.doctor_code.strip().upper()
        doctor = db.query(User).filter(User.role == "doctor", User.doctor_code == code).first()
        if not doctor:
            raise HTTPException(
                status_code=status.HTTP_400_BAD_REQUEST,
                detail="Invalid Doctor Code. Please check with your doctor."
            )

    # Create user
    new_user = User(
        name=user_data.name.strip(),
        email=email_clean,
        password_hash=hash_password(user_data.password),
        role=user_data.role,
    )
    db.add(new_user)
    db.flush()

    patient_id = None
    doctor_id = None

    if user_data.role == "doctor":
        new_user.doctor_code = generate_doctor_code(new_user.id)
        db.flush()
    elif user_data.role == "patient" and doctor:
        doctor_id = doctor.id
        patient_profile = Patient(
            user_id=new_user.id,
            doctor_id=doctor.id,
            name=user_data.name.strip(),
            age=None,
            condition="",
            has_baseline_data=False,
        )
        db.add(patient_profile)
        db.flush()
        patient_id = patient_profile.id

    db.commit()
    db.refresh(new_user)

    token = create_access_token({"sub": str(new_user.id), "role": new_user.role, "name": new_user.name})
    return AuthResponse(
        access_token=token,
        token_type="bearer",
        user=UserResponse.model_validate(new_user),
        patient_id=patient_id,
        doctor_id=doctor_id,
    )

@router.post("/login", response_model=AuthResponse)
def login(login_data: UserLogin, db: Session = Depends(get_db)):
    login_id = login_data.email.strip()
    if login_id.upper().startswith("PT-"):
        user = db.query(User).filter(User.patient_code == login_id.upper()).first()
    else:
        user = db.query(User).filter(User.email == login_id.lower()).first()
        
    if not user or not verify_password(login_data.password, user.password_hash):
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Incorrect User ID/Email or password",
        )

    patient_id = None
    doctor_id = None
    if user.role == "patient" and user.patient_profile:
        patient_id = user.patient_profile.id
        doctor_id = user.patient_profile.doctor_id

    token = create_access_token({"sub": str(user.id), "role": user.role, "name": user.name})
    return AuthResponse(
        access_token=token,
        token_type="bearer",
        user=UserResponse.model_validate(user),
        patient_id=patient_id,
        doctor_id=doctor_id,
    )

@router.get("/me")
def get_me(current_user: User = Depends(get_current_user), db: Session = Depends(get_db)):
    patient_id = None
    doctor_id = None
    if current_user.role == "patient" and current_user.patient_profile:
        patient_id = current_user.patient_profile.id
        doctor_id = current_user.patient_profile.doctor_id

    return {
        "user": UserResponse.model_validate(current_user),
        "patient_id": patient_id,
        "doctor_id": doctor_id,
        "doctor_code": current_user.doctor_code,
    }

@router.get("/doctors", response_model=list[UserResponse])
def get_all_doctors(db: Session = Depends(get_db)):
    doctors = db.query(User).filter(User.role == "doctor").all()
    return [UserResponse.model_validate(d) for d in doctors]
