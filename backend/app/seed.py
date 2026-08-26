import sys
from pathlib import Path

backend_dir = Path(__file__).resolve().parent.parent
if str(backend_dir) not in sys.path:
    sys.path.insert(0, str(backend_dir))

from datetime import datetime, timedelta, timezone
from app.database import SessionLocal, engine, Base
from app.models import User, Patient, Threshold, ExerciseHistory, SensorSession, PainAlert
from app.auth import hash_password

def seed_database():
    Base.metadata.create_all(bind=engine)
    db = SessionLocal()
    try:
        # Check if already seeded
        if db.query(User).filter(User.email == "doctor@physiopulse.com").first():
            print("[Seed] Database already contains initial data. Skipping.")
            return

        print("[Seed] Seeding sample doctor and patients...")

        # 1. Create Doctor
        doctor = User(
            name="Dr. Sarah Jenkins",
            email="doctor@physiopulse.com",
            password_hash=hash_password("doctor123"),
            role="doctor",
            doctor_code="DR0001",
        )
        db.add(doctor)
        db.flush()

        # 2. Create Patient 1 (John Doe)
        patient_user_1 = User(
            name="John Doe",
            email="john@example.com",
            password_hash=hash_password("patient123"),
            role="patient",
        )
        db.add(patient_user_1)
        db.flush()

        patient_1 = Patient(
            user_id=patient_user_1.id,
            doctor_id=doctor.id,
            name="John Doe",
            age=42,
            condition="Hand & Arm Mobility Rehabilitation",
            has_baseline_data=True,
        )
        db.add(patient_1)
        db.flush()

        # Threshold for Patient 1 (Hand / Bicep Curl)
        threshold_1 = Threshold(
            patient_id=patient_1.id,
            doctor_id=doctor.id,
            min_angle=30.0,
            max_angle=110.0,
            target_reps=12,
            motion_limit=120.0,
            temp_limit=39.5,
            exercise_type="Bicep Curl",
            video_url="https://www.youtube.com/watch?v=in7PaeYlhrM",
            strict_limit=True,
            notes="Controlled bicep curl. Keep wrist aligned and avoid jerky movements.",
        )
        db.add(threshold_1)

        # Past Exercise History for Patient 1 (3 sessions)
        now = datetime.now(timezone.utc)
        histories = [
            ExerciseHistory(
                patient_id=patient_1.id,
                doctor_id=doctor.id,
                date=now - timedelta(days=4),
                reps_completed=12,
                target_reps=12,
                max_angle_reached=108.5,
                min_angle_reached=32.1,
                avg_temp=36.8,
                duration_s=180,
                status="completed",
                exercise_type="Bicep Curl",
                notes="Good elbow flexion ROM, smooth rep rhythm.",
            ),
            ExerciseHistory(
                patient_id=patient_1.id,
                doctor_id=doctor.id,
                date=now - timedelta(days=2),
                reps_completed=10,
                target_reps=12,
                max_angle_reached=105.0,
                min_angle_reached=34.0,
                avg_temp=37.1,
                duration_s=165,
                status="partial",
                exercise_type="Bicep Curl",
                notes="Felt mild forearm muscle fatigue after rep 10.",
            ),
            ExerciseHistory(
                patient_id=patient_1.id,
                doctor_id=doctor.id,
                date=now - timedelta(hours=6),
                reps_completed=12,
                target_reps=12,
                max_angle_reached=110.0,
                min_angle_reached=30.5,
                avg_temp=36.9,
                duration_s=195,
                status="completed",
                exercise_type="Bicep Curl",
                notes="Target reps achieved smoothly with full elbow flexion ROM.",
            ),
        ]
        db.add_all(histories)

        # Sample Pain Alert for Patient 1
        pain_alert_1 = PainAlert(
            patient_id=patient_1.id,
            doctor_id=doctor.id,
            date=now - timedelta(days=2, hours=1),
            angle_at_pain=105.0,
            reps_at_pain=10,
            pain_level="Moderate",
            notes="Felt forearm tightness and wrist discomfort when flexing past 105 degrees.",
            status="new",
        )
        db.add(pain_alert_1)

        # 3. Create Patient 2 (Jane Miller)
        patient_user_2 = User(
            name="Jane Miller",
            email="jane@example.com",
            password_hash=hash_password("patient123"),
            role="patient",
        )
        db.add(patient_user_2)
        db.flush()

        patient_2 = Patient(
            user_id=patient_user_2.id,
            doctor_id=doctor.id,
            name="Jane Miller",
            age=29,
            condition="Rotator Cuff Tendinopathy",
            has_baseline_data=True,
        )
        db.add(patient_2)
        db.flush()

        threshold_2 = Threshold(
            patient_id=patient_2.id,
            doctor_id=doctor.id,
            min_angle=20.0,
            max_angle=90.0,
            target_reps=10,
            motion_limit=90.0,
            temp_limit=38.0,
            exercise_type="Wrist Circle / Rotation",
            video_url="https://www.youtube.com/watch?v=kYJjT6lYqA8",
            strict_limit=True,
            notes="Gentle wrist rotation and arm raises up to 90 degrees.",
        )
        db.add(threshold_2)

        history_jane = ExerciseHistory(
            patient_id=patient_2.id,
            doctor_id=doctor.id,
            date=now - timedelta(days=1),
            reps_completed=10,
            target_reps=10,
            max_angle_reached=88.4,
            min_angle_reached=21.0,
            avg_temp=36.6,
            duration_s=150,
            status="completed",
            exercise_type="Wrist Circle / Rotation",
            notes="Completed full wrist rotation protocol with good stability.",
        )
        db.add(history_jane)

        db.commit()
        print("[Seed] Sample doctor and patients created successfully!")
        print("  Doctor: doctor@physiopulse.com / doctor123 (Code: DR0001)")
        print("  Patient 1: john@example.com / patient123")
        print("  Patient 2: jane@example.com / patient123")
    except Exception as e:
        db.rollback()
        print(f"[Seed] Error: {e}")
    finally:
        db.close()

if __name__ == "__main__":
    seed_database()
