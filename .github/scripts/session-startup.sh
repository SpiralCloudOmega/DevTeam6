#!/usr/bin/env bash
# ============================================================================
# session-startup.sh — WSL-side REVITHION STUDIO session launcher
# ============================================================================
# Called by REVITHION-STARTUP.ps1 (or run directly from WSL).
# Sources environment, checks hardware/services, then runs copilot --resume.
#
# AFTER SESSION RESTORES:
#   1. Select session #1 (most recent) from the session list
#   2. Type: /yolo    — enables autonomous execution (no confirmation prompts)
#   3. Type: /fleet   — enables parallel sub-agent dispatching
# ============================================================================

set -uo pipefail

# --- Colors ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
DIM='\033[2m'
BOLD='\033[1m'
NC='\033[0m'

ok()   { echo -e "  ${GREEN}[+]${NC} $1"; }
warn() { echo -e "  ${YELLOW}[!]${NC} $1"; }
err()  { echo -e "  ${RED}[-]${NC} $1"; }
info() { echo -e "  ${CYAN}[*]${NC} $1"; }

# ============================================================================
# ENVIRONMENT VARIABLES
# ============================================================================
export REVITHION_HOME="/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6/REVITHION-STUDIO"
export REVITHION_SAMPLE_RATE=96000
export REVITHION_GPU=RTX3090
export REVITHION_REPO="/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6"

DEVTEAM6_DIR="/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6"
MEMORY_DB="${DEVTEAM6_DIR}/.github/memory/spiralcloud-memory.db"
LOG_FILE="${DEVTEAM6_DIR}/.github/scripts/startup.log"

# ============================================================================
# BANNER
# ============================================================================
echo ""
echo -e "${CYAN}${BOLD}"
cat << 'BANNER'
    ╔══════════════════════════════════════════════════════════╗
    ║  ██████  ███████ ██    ██ ██ ████████ ██   ██ ██  ██████  ███  ║
    ║  ██   ██ ██      ██    ██ ██    ██    ██   ██ ██ ██    ██ ████ ║
    ║  ██████  █████   ██    ██ ██    ██    ███████ ██ ██    ██ ██ █ ║
    ║  ██   ██ ██       ██  ██  ██    ██    ██   ██ ██ ██    ██ ██   ║
    ║  ██   ██ ███████   ████   ██    ██    ██   ██ ██  ██████  ██   ║
    ║                                                              ║
    ║          S T U D I O  —  S e s s i o n   S t a r t u p       ║
    ╚══════════════════════════════════════════════════════════╝
BANNER
echo -e "${NC}"
echo -e "  ${DIM}$(date '+%Y-%m-%d %H:%M:%S %Z')${NC}"
echo ""

# ============================================================================
# SOURCE ENVIRONMENT
# ============================================================================
echo -e "  ${MAGENTA}=== Environment ===${NC}"

if [ -f "$HOME/projects/devteam6-env.sh" ]; then
    # shellcheck disable=SC1091
    source "$HOME/projects/devteam6-env.sh" 2>/dev/null && ok "Sourced devteam6-env.sh" || warn "devteam6-env.sh had errors"
else
    info "devteam6-env.sh not found at ~/projects/ — skipping"
fi

ok "REVITHION_HOME=${REVITHION_HOME}"
ok "REVITHION_SAMPLE_RATE=${REVITHION_SAMPLE_RATE}"
ok "REVITHION_GPU=${REVITHION_GPU}"

# ============================================================================
# GPU CHECK
# ============================================================================
echo ""
echo -e "  ${MAGENTA}=== GPU ===${NC}"

if command -v nvidia-smi &>/dev/null; then
    gpu_info=$(nvidia-smi --query-gpu=name,memory.used,memory.total,temperature.gpu --format=csv,noheader,nounits 2>/dev/null)
    if [ -n "$gpu_info" ]; then
        IFS=',' read -r gpu_name mem_used mem_total temp <<< "$gpu_info"
        gpu_name=$(echo "$gpu_name" | xargs)
        mem_used=$(echo "$mem_used" | xargs)
        mem_total=$(echo "$mem_total" | xargs)
        temp=$(echo "$temp" | xargs)
        mem_free=$((mem_total - mem_used))
        ok "GPU: ${gpu_name} — ${mem_used}MB / ${mem_total}MB VRAM (${mem_free}MB free)"
        ok "GPU Temp: ${temp}°C"
    else
        warn "nvidia-smi returned no data"
    fi
else
    warn "nvidia-smi not available (GPU passthrough may not be configured)"
fi

# ============================================================================
# SERVICES CHECK
# ============================================================================
echo ""
echo -e "  ${MAGENTA}=== Services ===${NC}"

check_service() {
    local name="$1"
    local port="$2"
    if ss -tlnp 2>/dev/null | grep -q ":${port} " 2>/dev/null; then
        ok "${name}: Running on port ${port}"
        return 0
    elif curl -s --connect-timeout 1 "http://localhost:${port}" &>/dev/null; then
        ok "${name}: Responding on port ${port}"
        return 0
    else
        info "${name}: Not detected on port ${port}"
        return 1
    fi
}

check_service "Ollama"     11434
check_service "PostgreSQL" 5432
check_service "Gitea"      3000
check_service "CyborgDB"   8100

# Check Docker
if command -v docker &>/dev/null && docker info &>/dev/null 2>&1; then
    ok "Docker: Running"
else
    info "Docker: Not running"
fi

# ============================================================================
# DISK SPACE
# ============================================================================
echo ""
echo -e "  ${MAGENTA}=== Disk Space ===${NC}"

df_output=$(df -h "$DEVTEAM6_DIR" 2>/dev/null | tail -1)
if [ -n "$df_output" ]; then
    disk_size=$(echo "$df_output" | awk '{print $2}')
    disk_used=$(echo "$df_output" | awk '{print $3}')
    disk_avail=$(echo "$df_output" | awk '{print $4}')
    disk_pct=$(echo "$df_output" | awk '{print $5}')
    ok "DevTeam6 volume: ${disk_used} / ${disk_size} used (${disk_avail} free, ${disk_pct})"
fi

wsl_root=$(df -h / 2>/dev/null | tail -1)
if [ -n "$wsl_root" ]; then
    root_avail=$(echo "$wsl_root" | awk '{print $4}')
    root_pct=$(echo "$wsl_root" | awk '{print $5}')
    ok "WSL root (/): ${root_avail} free (${root_pct} used)"
fi

# ============================================================================
# MEMORY DATABASE
# ============================================================================
echo ""
echo -e "  ${MAGENTA}=== Memory Database ===${NC}"

if [ -f "$MEMORY_DB" ]; then
    db_size=$(du -h "$MEMORY_DB" | cut -f1)
    ok "Memory DB: ${db_size} — ${MEMORY_DB}"
    if command -v sqlite3 &>/dev/null; then
        session_count=$(sqlite3 "$MEMORY_DB" "SELECT COUNT(*) FROM sessions;" 2>/dev/null || echo "?")
        turn_count=$(sqlite3 "$MEMORY_DB" "SELECT COUNT(*) FROM turns;" 2>/dev/null || echo "?")
        ok "Sessions: ${session_count} | Turns: ${turn_count}"
    fi
else
    info "Memory DB not found (will be created on first session)"
fi

# ============================================================================
# LOG TIMESTAMP
# ============================================================================
mkdir -p "$(dirname "$LOG_FILE")" 2>/dev/null
echo "[$(date '+%Y-%m-%d %H:%M:%S %Z')] session-startup.sh launched" >> "$LOG_FILE" 2>/dev/null

# ============================================================================
# NAVIGATE TO REPO
# ============================================================================
echo ""
echo -e "  ${MAGENTA}=== Launching Copilot CLI ===${NC}"
cd "$DEVTEAM6_DIR" || { err "Cannot cd to DevTeam6 repo"; exit 1; }
ok "Working directory: $(pwd)"

# ============================================================================
# PRE-FLIGHT: Check copilot CLI
# ============================================================================
if ! command -v copilot &>/dev/null; then
    err "Copilot CLI not found!"
    err "Install with: gh extension install github/gh-copilot"
    echo ""
    exit 1
fi
ok "Copilot CLI: $(command -v copilot)"

# ============================================================================
# LAUNCH COPILOT — RESUME LAST SESSION
# ============================================================================
echo ""
echo -e "  ${YELLOW}${BOLD}┌─────────────────────────────────────────────────────────┐${NC}"
echo -e "  ${YELLOW}${BOLD}│  AFTER SESSION RESTORES:                                │${NC}"
echo -e "  ${YELLOW}${BOLD}│    1. Select session #1 (most recent) from the list      │${NC}"
echo -e "  ${YELLOW}${BOLD}│    2. Type: /yolo    — autonomous execution mode         │${NC}"
echo -e "  ${YELLOW}${BOLD}│    3. Type: /fleet   — parallel sub-agent dispatching     │${NC}"
echo -e "  ${YELLOW}${BOLD}└─────────────────────────────────────────────────────────┘${NC}"
echo ""

exec copilot --resume
