import sys
from pathlib import Path

# Add backend directory to sys.path so app imports resolve properly
root_dir = Path(__file__).resolve().parent.parent
backend_dir = root_dir / "backend"

if str(backend_dir) not in sys.path:
    sys.path.insert(0, str(backend_dir))
if str(root_dir) not in sys.path:
    sys.path.insert(0, str(root_dir))

# pyrefly: ignore[missing-import]
from app.main import app

# Export ASGI app instance for Vercel Serverless
handler = app
