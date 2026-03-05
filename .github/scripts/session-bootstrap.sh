#!/usr/bin/env bash
# session-bootstrap.sh — Load ALL prior context for a new Copilot session
# Usage: source this file at the start of any session
# SpiralCloudOmega / DevTeam6

set -euo pipefail

DEVTEAM6_ROOT="/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6"
MEMORY_DIR="${DEVTEAM6_ROOT}/.github/memory"
MEMORY_DB="${MEMORY_DIR}/spiralcloud-memory.db"
COPILOT_STATE="$HOME/.copilot/session-state"
ENV_PROFILE="${MEMORY_DIR}/environment-profile.json"
RULES_FILE="${MEMORY_DIR}/rules-and-directives.json"

echo "=============================================="
echo "  SpiralCloudOmega Session Bootstrap"
echo "  $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
echo "=============================================="

# 1. Check memory database exists
if [ -f "$MEMORY_DB" ]; then
    echo ""
    echo "📦 Memory Database: $(du -h "$MEMORY_DB" | cut -f1)"
    echo "   Tables:"
    sqlite3 "$MEMORY_DB" ".tables" 2>/dev/null || echo "   (sqlite3 not available)"
    echo ""
    echo "   Stats:"
    sqlite3 "$MEMORY_DB" "SELECT 'Sessions: ' || COUNT(*) FROM sessions;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'Total turns: ' || COUNT(*) FROM turns;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'Actions logged: ' || COUNT(*) FROM actions;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'File changes: ' || COUNT(*) FROM file_changes;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'Commands run: ' || COUNT(*) FROM commands;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'User directives: ' || COUNT(*) FROM user_directives;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'Repos cataloged: ' || COUNT(*) FROM repos;" 2>/dev/null
    sqlite3 "$MEMORY_DB" "SELECT 'Agents cataloged: ' || COUNT(*) FROM agents;" 2>/dev/null
else
    echo "⚠️  Memory database not found at: $MEMORY_DB"
fi

# 2. Show environment profile summary
if [ -f "$ENV_PROFILE" ]; then
    echo ""
    echo "🖥️  Environment Profile:"
    python3 -c "
import json
with open('$ENV_PROFILE') as f:
    p = json.load(f)
print(f'   Hardware: {p[\"hardware\"][\"cpu\"]}, {p[\"hardware\"][\"ram\"]}, {p[\"hardware\"][\"gpu\"]}')
print(f'   Storage: C:{p[\"hardware\"][\"storage\"][\"c_drive\"][\"free\"]} free | D:{p[\"hardware\"][\"storage\"][\"d_drive\"][\"free\"]} free | WSL:{p[\"hardware\"][\"storage\"][\"wsl_vhd\"][\"free\"]} free')
print(f'   Repos: {p[\"repos\"][\"total\"]}')
print(f'   Python: {p[\"languages_and_runtimes\"][\"python\"]} | Node: {p[\"languages_and_runtimes\"][\"node\"]}')
" 2>/dev/null || echo "   (Could not parse profile)"
fi

# 3. Show active user directives
if [ -f "$RULES_FILE" ]; then
    echo ""
    echo "📋 Active Rules:"
    python3 -c "
import json
with open('$RULES_FILE') as f:
    r = json.load(f)
for category, rules in r.items():
    if isinstance(rules, list) and len(rules) > 0:
        print(f'   {category}: {len(rules)} rules')
" 2>/dev/null || echo "   (Could not parse rules)"
fi

# 4. Show recent sessions
echo ""
echo "📅 Recent Sessions:"
if [ -f "$MEMORY_DB" ]; then
    sqlite3 -column -header "$MEMORY_DB" \
        "SELECT id, started_at, summary FROM sessions ORDER BY started_at DESC LIMIT 5;" 2>/dev/null
else
    # Fallback: scan copilot session state
    for dir in "$COPILOT_STATE"/*/; do
        if [ -f "${dir}plan.md" ]; then
            echo "   $(basename "$dir") — has plan.md"
        fi
    done
fi

# 5. Show pending work
echo ""
echo "📝 Pending Work:"
if [ -f "$MEMORY_DB" ]; then
    sqlite3 -column -header "$MEMORY_DB" \
        "SELECT key, value FROM kv_store WHERE key LIKE 'pending_%' ORDER BY key;" 2>/dev/null
fi

# 6. Show running services
echo ""
echo "🔧 Services:"
for port in 11434 18789 3000 8001 3001 3002 3003 8080 5432 8100 5494; do
    if ss -tlnp 2>/dev/null | grep -q ":${port} "; then
        case $port in
            11434) name="Ollama" ;;
            18789) name="OpenClaw" ;;
            3000)  name="Gitea" ;;
            8001)  name="ACE-Step API" ;;
            3001)  name="ACE-Step Backend" ;;
            3002)  name="ACE-Step Frontend" ;;
            3003)  name="CopilotKit" ;;
            8080)  name="OpenDAW" ;;
            5432)  name="PostgreSQL" ;;
            8100)  name="CyborgDB" ;;
            5494)  name="KimiCode" ;;
            *)     name="Unknown" ;;
        esac
        echo "   ✅ ${name} (:${port})"
    fi
done

# 7. Show agent count
AGENT_DIR="${DEVTEAM6_ROOT}/.github/agents"
if [ -d "$AGENT_DIR" ]; then
    echo ""
    echo "🤖 Custom Agents: $(ls "$AGENT_DIR"/*.agent.md 2>/dev/null | wc -l)"
fi

echo ""
echo "=============================================="
echo "  Bootstrap complete. Memory system ready."
echo "=============================================="
