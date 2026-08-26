import os
from contextlib import asynccontextmanager
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles

from app.config import settings
from app.database import engine, Base
from app.routers import auth, patients, thresholds, exercise, sessions, alerts

from sqlalchemy import text

def run_migrations():
    Base.metadata.create_all(bind=engine)
    with engine.connect() as conn:
        try:
            res = conn.execute(text("PRAGMA table_info(thresholds)")).fetchall()
            col_names = [r[1] for r in res]
            if "video_url" not in col_names:
                conn.execute(text("ALTER TABLE thresholds ADD COLUMN video_url VARCHAR(500) DEFAULT ''"))
            if "strict_limit" not in col_names:
                conn.execute(text("ALTER TABLE thresholds ADD COLUMN strict_limit BOOLEAN DEFAULT 1"))
            res_users = conn.execute(text("PRAGMA table_info(users)")).fetchall()
            user_cols = [r[1] for r in res_users]
            if "patient_code" not in user_cols:
                conn.execute(text("ALTER TABLE users ADD COLUMN patient_code VARCHAR(50) DEFAULT NULL"))
                conn.execute(text("CREATE UNIQUE INDEX ix_users_patient_code ON users (patient_code)"))

            conn.commit()
        except Exception as e:
            print("[Migration] Note:", e)

# Create DB tables & run migrations
@asynccontextmanager
async def lifespan(app: FastAPI):
    run_migrations()
    yield

app = FastAPI(
    title=settings.PROJECT_NAME,
    description="PhysioPulse Backend API – Patient history, exercise metrics, doctor prescriptions, pain alerts, and sensor sessions",
    version="1.0.0",
    lifespan=lifespan,
)

# CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=settings.CORS_ORIGINS,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Include API routers
app.include_router(auth.router)
app.include_router(patients.router)
app.include_router(thresholds.router)
app.include_router(exercise.router)
app.include_router(sessions.router)
app.include_router(alerts.router)

@app.get("/api/health")
def health_check():
    return {"status": "ok", "app": settings.PROJECT_NAME, "version": "1.0.0"}

# Mount WebApp static frontend if available
if settings.WEBAPP_DIR.exists() and (settings.WEBAPP_DIR / "index.html").exists():
    app.mount("/", StaticFiles(directory=str(settings.WEBAPP_DIR), html=True), name="webapp")
