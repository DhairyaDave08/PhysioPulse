from sqlalchemy import create_engine
from sqlalchemy.orm import declarative_base, sessionmaker
from app.config import settings

db_url = settings.DATABASE_URL

# Supabase provides postgres:// or postgresql://; ensure postgresql+psycopg2 driver is used
if db_url.startswith("postgres://"):
    db_url = db_url.replace("postgres://", "postgresql+psycopg2://", 1)
elif db_url.startswith("postgresql://") and not db_url.startswith("postgresql+"):
    db_url = db_url.replace("postgresql://", "postgresql+psycopg2://", 1)

# Auto-encode unescaped '@' characters in password (e.g. user:pass@123@host)
if "@" in db_url:
    auth_part, host_part = db_url.rsplit("@", 1)
    if "://" in auth_part:
        scheme, user_pass = auth_part.split("://", 1)
        if ":" in user_pass:
            user, pwd = user_pass.split(":", 1)
            if "@" in pwd:
                pwd = pwd.replace("@", "%40")
                db_url = f"{scheme}://{user}:{pwd}@{host_part}"

# SQLite needs connect_args check_same_thread=False
is_sqlite = db_url.startswith("sqlite")
connect_args = {"check_same_thread": False} if is_sqlite else {}

# For PostgreSQL on serverless/cloud, enable pool_pre_ping to automatically handle dropped connections
engine_kwargs = {"connect_args": connect_args}
if not is_sqlite:
    engine_kwargs.update({
        "pool_pre_ping": True,
        "pool_recycle": 300,
    })

engine = create_engine(db_url, **engine_kwargs)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

Base = declarative_base()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

