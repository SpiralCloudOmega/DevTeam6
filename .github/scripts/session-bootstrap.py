#!/usr/bin/env python3
"""
session-bootstrap.py — Load ALL prior context for a new Copilot session
SpiralCloudOmega / DevTeam6

Usage:
    python3 session-bootstrap.py              # Full context dump
    python3 session-bootstrap.py --json       # JSON output for agents
    python3 session-bootstrap.py --compact    # Key facts only
"""

import sqlite3
import json
import os
import sys
from pathlib import Path
from datetime import datetime

DEVTEAM6_ROOT = Path("/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6")
MEMORY_DIR = DEVTEAM6_ROOT / ".github" / "memory"
MEMORY_DB = MEMORY_DIR / "spiralcloud-memory.db"
ENV_PROFILE = MEMORY_DIR / "environment-profile.json"
RULES_FILE = MEMORY_DIR / "rules-and-directives.json"
COPILOT_STATE = Path.home() / ".copilot" / "session-state"


def get_db():
    """Connect to memory database."""
    if not MEMORY_DB.exists():
        print(f"WARNING: Memory DB not found at {MEMORY_DB}", file=sys.stderr)
        return None
    return sqlite3.connect(str(MEMORY_DB))


def load_json(path):
    """Load a JSON file."""
    if not path.exists():
        return {}
    with open(path) as f:
        return json.load(f)


def get_session_history(db, limit=10):
    """Get recent session history — FULL, no summarizing."""
    if not db:
        return []
    cursor = db.execute(
        "SELECT id, cwd, repository, branch, model, started_at, ended_at, summary, "
        "total_turns, total_commands, total_file_changes "
        "FROM sessions ORDER BY started_at DESC LIMIT ?",
        (limit,)
    )
    columns = [d[0] for d in cursor.description]
    return [dict(zip(columns, row)) for row in cursor.fetchall()]


def get_user_directives(db):
    """Get ALL active user directives — verbatim."""
    if not db:
        return []
    cursor = db.execute(
        "SELECT category, directive, verbatim_quote, session_id, timestamp "
        "FROM user_directives WHERE still_active = 1 ORDER BY category, timestamp"
    )
    columns = [d[0] for d in cursor.description]
    return [dict(zip(columns, row)) for row in cursor.fetchall()]


def get_recent_turns(db, limit=20):
    """Get most recent conversation turns — FULL TEXT."""
    if not db:
        return []
    cursor = db.execute(
        "SELECT t.session_id, t.turn_index, t.role, t.message, t.timestamp "
        "FROM turns t JOIN sessions s ON t.session_id = s.id "
        "ORDER BY t.timestamp DESC LIMIT ?",
        (limit,)
    )
    columns = [d[0] for d in cursor.description]
    return [dict(zip(columns, row)) for row in cursor.fetchall()]


def get_pending_work(db):
    """Get any pending/in-progress work items."""
    if not db:
        return []
    cursor = db.execute(
        "SELECT key, value FROM kv_store WHERE key LIKE 'pending_%' OR key LIKE 'todo_%'"
    )
    return cursor.fetchall()


def get_stats(db):
    """Get database statistics."""
    if not db:
        return {}
    stats = {}
    tables = ["sessions", "turns", "actions", "decisions", "file_changes",
              "commands", "user_directives", "repos", "agents", "skills", "services", "checkpoints", "plans"]
    for table in tables:
        try:
            cursor = db.execute(f"SELECT COUNT(*) FROM {table}")
            stats[table] = cursor.fetchone()[0]
        except sqlite3.OperationalError:
            stats[table] = 0
    return stats


def check_services():
    """Check which services are running."""
    import subprocess
    services = {
        11434: "Ollama", 18789: "OpenClaw", 3000: "Gitea",
        8001: "ACE-Step API", 3001: "ACE-Step Backend", 3002: "ACE-Step Frontend",
        3003: "CopilotKit", 8080: "OpenDAW", 5432: "PostgreSQL",
        8100: "CyborgDB", 5494: "KimiCode"
    }
    running = {}
    try:
        result = subprocess.run(["ss", "-tlnp"], capture_output=True, text=True, timeout=5)
        for port, name in services.items():
            if f":{port} " in result.stdout:
                running[port] = name
    except Exception:
        pass
    return running


def main():
    output_json = "--json" in sys.argv
    compact = "--compact" in sys.argv

    db = get_db()
    env_profile = load_json(ENV_PROFILE)
    rules = load_json(RULES_FILE)

    context = {
        "timestamp": datetime.utcnow().isoformat() + "Z",
        "memory_db_exists": MEMORY_DB.exists(),
        "memory_db_size": MEMORY_DB.stat().st_size if MEMORY_DB.exists() else 0,
        "environment_profile": env_profile,
        "rules_and_directives": rules,
        "db_stats": get_stats(db),
        "recent_sessions": get_session_history(db),
        "active_directives": get_user_directives(db),
        "running_services": check_services(),
        "agent_count": len(list((DEVTEAM6_ROOT / ".github" / "agents").glob("*.agent.md")))
            if (DEVTEAM6_ROOT / ".github" / "agents").exists() else 0,
    }

    if not compact:
        context["recent_turns"] = get_recent_turns(db)
        context["pending_work"] = get_pending_work(db)

    if db:
        db.close()

    if output_json:
        print(json.dumps(context, indent=2, default=str))
    else:
        print("=" * 60)
        print("  SpiralCloudOmega Session Bootstrap")
        print(f"  {context['timestamp']}")
        print("=" * 60)
        print()

        if context["memory_db_exists"]:
            print(f"📦 Memory Database: {context['memory_db_size']:,} bytes")
            print("   Stats:")
            for table, count in context["db_stats"].items():
                if count > 0:
                    print(f"     {table}: {count:,}")
        else:
            print("⚠️  No memory database found")

        print()
        print(f"🤖 Custom Agents: {context['agent_count']}")
        print()

        if context["running_services"]:
            print("🔧 Running Services:")
            for port, name in sorted(context["running_services"].items()):
                print(f"   ✅ {name} (:{port})")
        else:
            print("🔧 No services detected")

        print()
        if context["recent_sessions"]:
            print("📅 Recent Sessions:")
            for s in context["recent_sessions"][:5]:
                summary = s.get("summary", "No summary") or "No summary"
                print(f"   {s['id'][:8]}... | {s.get('started_at', '?')} | {summary}")

        print()
        print("=" * 60)
        print("  Bootstrap complete.")
        print("=" * 60)


if __name__ == "__main__":
    main()
