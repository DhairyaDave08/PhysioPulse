import pytest
from fastapi.testclient import TestClient
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.pool import StaticPool

from app.database import Base, get_db
from app.main import app

# Create in-memory SQLite database for fast and isolated testing
SQLALCHEMY_DATABASE_URL = "sqlite:///:memory:"
engine = create_engine(
    SQLALCHEMY_DATABASE_URL,
    connect_args={"check_same_thread": False},
    poolclass=StaticPool,
)
TestingSessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

def override_get_db():
    db = TestingSessionLocal()
    try:
        yield db
    finally:
        db.close()

app.dependency_overrides[get_db] = override_get_db

@pytest.fixture(autouse=True)
def setup_db():
    Base.metadata.create_all(bind=engine)
    yield
    Base.metadata.drop_all(bind=engine)

@pytest.fixture
def client():
    return TestClient(app)

def test_health_check(client):
    res = client.get("/api/health")
    assert res.status_code == 200
    assert res.json()["status"] == "ok"

def test_doctor_and_patient_flow(client):
    # 1. Register Doctor
    doc_res = client.post("/api/auth/register", json={
        "name": "Dr. Test Watson",
        "email": "doctor@test.com",
        "password": "securepassword",
        "role": "doctor"
    })
    assert doc_res.status_code == 200
    doc_data = doc_res.json()
    assert doc_data["user"]["role"] == "doctor"
    doc_code = doc_data["user"]["doctor_code"]
    assert doc_code is not None
    doc_token = doc_data["access_token"]
    doc_headers = {"Authorization": f"Bearer {doc_token}"}

    # 2. Patient register with invalid doctor code -> should fail
    bad_pat_res = client.post("/api/auth/register", json={
        "name": "Patient Bad",
        "email": "patientbad@test.com",
        "password": "securepassword",
        "role": "patient",
        "doctor_code": "INVALID_CODE"
    })
    assert bad_pat_res.status_code == 400

    # 3. Patient register with valid doctor code
    pat_res = client.post("/api/auth/register", json={
        "name": "Alice Patient",
        "email": "alice@test.com",
        "password": "securepassword",
        "role": "patient",
        "doctor_code": doc_code
    })
    assert pat_res.status_code == 200
    pat_data = pat_res.json()
    assert pat_data["user"]["role"] == "patient"
    patient_id = pat_data["patient_id"]
    assert patient_id is not None
    pat_token = pat_data["access_token"]
    pat_headers = {"Authorization": f"Bearer {pat_token}"}

    # 4. Doctor sees patient in patient list
    patients_res = client.get("/api/patients", headers=doc_headers)
    assert patients_res.status_code == 200
    patients_list = patients_res.json()
    assert len(patients_list) == 1
    assert patients_list[0]["name"] == "Alice Patient"
    assert patients_list[0]["has_baseline_data"] is False

    # 5. Doctor sets threshold for patient
    thresh_res = client.post(f"/api/patients/{patient_id}/threshold", json={
        "min_angle": 25.0,
        "max_angle": 105.0,
        "target_reps": 15,
        "motion_limit": 110.0,
        "temp_limit": 39.0,
        "exercise_type": "Bicep Curl",
        "notes": "Post-op Week 3 target protocol"
    }, headers=doc_headers)
    assert thresh_res.status_code == 200
    thresh_data = thresh_res.json()
    assert thresh_data["min_angle"] == 25.0
    assert thresh_data["target_reps"] == 15

    # 6. Patient gets their threshold
    pat_thresh_res = client.get(f"/api/patients/{patient_id}/threshold", headers=pat_headers)
    assert pat_thresh_res.status_code == 200
    assert pat_thresh_res.json()["max_angle"] == 105.0

    # 7. Patient completes an exercise session and records history
    ex_res = client.post("/api/exercise-history", json={
        "patient_id": patient_id,
        "reps_completed": 15,
        "target_reps": 15,
        "max_angle_reached": 102.3,
        "min_angle_reached": 26.1,
        "avg_temp": 36.8,
        "duration_s": 240,
        "status": "completed",
        "exercise_type": "Bicep Curl",
        "notes": "Felt good throughout session"
    }, headers=pat_headers)
    assert ex_res.status_code == 200
    ex_data = ex_res.json()
    assert ex_data["reps_completed"] == 15
    assert ex_data["status"] == "completed"

    # 8. Record a second session (partial)
    client.post("/api/exercise-history", json={
        "patient_id": patient_id,
        "reps_completed": 8,
        "target_reps": 15,
        "max_angle_reached": 95.0,
        "min_angle_reached": 28.0,
        "avg_temp": 37.0,
        "duration_s": 120,
        "status": "partial",
        "exercise_type": "Bicep Curl",
        "notes": "Fatigued"
    }, headers=pat_headers)

    # 9. Doctor views Alice's exercise history
    history_res = client.get(f"/api/patients/{patient_id}/exercise-history", headers=doc_headers)
    assert history_res.status_code == 200
    history_list = history_res.json()
    assert len(history_list) == 2
    assert history_list[0]["reps_completed"] == 8
    assert history_list[1]["reps_completed"] == 15

    # 10. Check patient analytics
    analytics_res = client.get(f"/api/patients/{patient_id}/analytics", headers=doc_headers)
    assert analytics_res.status_code == 200
    analytics = analytics_res.json()
    assert analytics["total_sessions"] == 2
    assert analytics["completed_sessions"] == 1
    assert analytics["compliance_rate"] == 50.0
    assert analytics["total_reps"] == 23

    # 11. Doctor updates patient profile
    update_res = client.put(f"/api/patients/{patient_id}", json={
        "name": "Alice Patient Updated",
        "age": 35,
        "condition": "Post-Op Arm Rehab Phase 2",
        "has_baseline_data": True
    }, headers=doc_headers)
    assert update_res.status_code == 200
    updated_data = update_res.json()
    assert updated_data["age"] == 35
    assert updated_data["condition"] == "Post-Op Arm Rehab Phase 2"

    # 12. Doctor gets threshold history
    client.post(f"/api/patients/{patient_id}/threshold", json={
        "min_angle": 20.0,
        "max_angle": 120.0,
        "target_reps": 20,
        "motion_limit": 120.0,
        "temp_limit": 39.0,
        "exercise_type": "Bicep Curl",
        "notes": "Progression to Week 4"
    }, headers=doc_headers)
    thresh_hist_res = client.get(f"/api/patients/{patient_id}/thresholds", headers=doc_headers)
    assert thresh_hist_res.status_code == 200
    thresh_hist = thresh_hist_res.json()
    assert len(thresh_hist) == 2
    assert thresh_hist[0]["target_reps"] == 20
    assert thresh_hist[1]["target_reps"] == 15

def test_sensor_sessions_and_csv(client):
    # Save a sensor session
    session_res = client.post("/api/sessions", json={
        "patient_name": "Bob Tester",
        "exercise": "Shoulder Abduction",
        "notes": "Testing raw sensor stream capture",
        "duration_s": 5,
        "interval_ms": 50,
        "stats": {"maxAccelMag": 1.25, "maxGyroMag": 45.2, "avgTemp": 36.5, "dataPoints": 2},
        "rows": [
            {"t": 0, "ax": 0.1, "ay": 0.2, "az": 0.98, "gx": 1.0, "gy": 0.5, "gz": 0.2, "tp": 36.5},
            {"t": 50, "ax": 0.15, "ay": 0.22, "az": 0.97, "gx": 2.0, "gy": 0.6, "gz": 0.3, "tp": 36.6},
        ]
    })
    assert session_res.status_code == 200
    session_data = session_res.json()
    session_id = session_data["id"]
    assert session_data["data_points"] == 2

    # Get session detail
    detail_res = client.get(f"/api/sessions/{session_id}")
    assert detail_res.status_code == 200
    detail = detail_res.json()
    assert len(detail["rows"]) == 2

    # Download CSV
    csv_res = client.get(f"/api/sessions/{session_id}/csv")
    assert csv_res.status_code == 200
    assert "timestamp_ms,accel_x_g,accel_y_g" in csv_res.text
    assert "0,0.1,0.2,0.98,1.0,0.5,0.2,36.5" in csv_res.text

    # Delete session
    del_res = client.delete(f"/api/sessions/{session_id}")
    assert del_res.status_code == 200

    # Verify deleted
    get_del = client.get(f"/api/sessions/{session_id}")
    assert get_del.status_code == 404

def test_threshold_video_and_strict_limits(client):
    # 1. Register Doctor & Patient
    doc_res = client.post("/api/auth/register", json={
        "name": "Dr. Video Tester",
        "email": "docvideo@test.com",
        "password": "password123",
        "role": "doctor"
    })
    doc_data = doc_res.json()
    doc_token = doc_data["access_token"]
    doc_code = doc_data["user"]["doctor_code"]
    doc_headers = {"Authorization": f"Bearer {doc_token}"}

    pat_res = client.post("/api/auth/register", json={
        "name": "Charlie Video",
        "email": "charlie@test.com",
        "password": "password123",
        "role": "patient",
        "doctor_code": doc_code
    })
    patient_id = pat_res.json()["patient_id"]

    # 2. Doctor sets threshold with YouTube URL and strict limit
    t_res = client.post(f"/api/patients/{patient_id}/threshold", json={
        "min_angle": 30.0,
        "max_angle": 90.0,
        "target_reps": 10,
        "motion_limit": 100.0,
        "temp_limit": 40.0,
        "exercise_type": "Bicep Curl",
        "video_url": "https://www.youtube.com/watch?v=kYJjT6lYqA8",
        "strict_limit": True,
        "notes": "Follow video form closely. Do not exceed 90 degrees."
    }, headers=doc_headers)
    assert t_res.status_code == 200
    t_data = t_res.json()
    assert t_data["video_url"] == "https://www.youtube.com/watch?v=kYJjT6lYqA8"
    assert t_data["strict_limit"] is True
    assert t_data["max_angle"] == 90.0

    # 3. Patient fetches threshold
    pat_token = pat_res.json()["access_token"]
    pat_headers = {"Authorization": f"Bearer {pat_token}"}
    pat_t_res = client.get(f"/api/patients/{patient_id}/threshold", headers=pat_headers)
    assert pat_t_res.status_code == 200
    assert pat_t_res.json()["video_url"] == "https://www.youtube.com/watch?v=kYJjT6lYqA8"

def test_pain_alerts_flow(client):
    # 1. Register Doctor & Patient
    doc_res = client.post("/api/auth/register", json={
        "name": "Dr. Pain Tester",
        "email": "docpain@test.com",
        "password": "password123",
        "role": "doctor"
    })
    doc_data = doc_res.json()
    doc_headers = {"Authorization": f"Bearer {doc_data['access_token']}"}
    doc_code = doc_data["user"]["doctor_code"]

    pat_res = client.post("/api/auth/register", json={
        "name": "David Discomfort",
        "email": "david@test.com",
        "password": "password123",
        "role": "patient",
        "doctor_code": doc_code
    })
    pat_data = pat_res.json()
    patient_id = pat_data["patient_id"]
    pat_headers = {"Authorization": f"Bearer {pat_data['access_token']}"}

    # 2. Patient creates a Pain Alert
    alert_res = client.post(f"/api/patients/{patient_id}/pain-alert", json={
        "patient_id": patient_id,
        "angle_at_pain": 72.5,
        "reps_at_pain": 6,
        "pain_level": "Moderate",
        "notes": "Sharp pinching feeling in joint above 70 degrees"
    }, headers=pat_headers)
    assert alert_res.status_code == 200
    alert_data = alert_res.json()
    alert_id = alert_data["id"]
    assert alert_data["angle_at_pain"] == 72.5
    assert alert_data["reps_at_pain"] == 6
    assert alert_data["pain_level"] == "Moderate"
    assert alert_data["status"] == "new"

    # 3. Doctor checks pain alerts list
    doc_alerts_res = client.get("/api/doctor/pain-alerts", headers=doc_headers)
    assert doc_alerts_res.status_code == 200
    doc_alerts = doc_alerts_res.json()
    assert len(doc_alerts) == 1
    assert doc_alerts[0]["patient_name"] == "David Discomfort"
    assert doc_alerts[0]["notes"] == "Sharp pinching feeling in joint above 70 degrees"

    # 4. Patient views their own pain alerts
    pat_alerts_res = client.get(f"/api/patients/{patient_id}/pain-alerts", headers=pat_headers)
    assert pat_alerts_res.status_code == 200
    assert len(pat_alerts_res.json()) == 1

    # 5. Doctor resolves the pain alert
    resolve_res = client.put(f"/api/pain-alerts/{alert_id}/resolve", json={
        "status": "reviewed"
    }, headers=doc_headers)
    assert resolve_res.status_code == 200
    assert resolve_res.json()["status"] == "reviewed"

