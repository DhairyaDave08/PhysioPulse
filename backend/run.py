import os
import sys
from pathlib import Path

# Add backend directory to sys.path
backend_dir = Path(__file__).resolve().parent
if str(backend_dir) not in sys.path:
    sys.path.insert(0, str(backend_dir))

import uvicorn
from app.seed import seed_database

if __name__ == "__main__":
    # Seed database with sample data if fresh
    seed_database()

    port = int(os.getenv("PORT", 8000))
    host = os.getenv("HOST", "0.0.0.0")

    print(f"============================================================")
    print(f"  PhysioPulse Backend Server Started")
    print(f"  Web App:       http://localhost:{port}")
    print(f"  API Docs:      http://localhost:{port}/docs")
    print(f"============================================================")

    uvicorn.run("app.main:app", host=host, port=port, reload=True)
