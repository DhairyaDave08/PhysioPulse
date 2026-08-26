import os
from pathlib import Path
from pydantic_settings import BaseSettings, SettingsConfigDict

BASE_DIR = Path(__file__).resolve().parent.parent
PROJECT_DIR = BASE_DIR.parent
WEBAPP_DIR = PROJECT_DIR / "webapp"

class Settings(BaseSettings):
    PROJECT_NAME: str = "PhysioPulse"
    SECRET_KEY: str = os.getenv("SECRET_KEY", "physiopulse_super_secret_jwt_key_2026")
    ALGORITHM: str = "HS256"
    ACCESS_TOKEN_EXPIRE_MINUTES: int = 60 * 24 * 7  # 7 days
    DATABASE_URL: str = os.getenv("DATABASE_URL", f"sqlite:///{BASE_DIR / 'physiopulse.db'}")
    CORS_ORIGINS: list[str] = ["*"]
    WEBAPP_DIR: Path = WEBAPP_DIR

    model_config = SettingsConfigDict(case_sensitive=True)

settings = Settings()
