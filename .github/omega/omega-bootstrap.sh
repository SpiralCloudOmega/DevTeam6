#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# omega-bootstrap.sh — Omega Agent System Startup
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#
# Initializes the full Omega multi-agent system:
#   1. Sources environment variables (omega-env.sh + devteam6-env.sh)
#   2. Verifies external CLI availability
#   3. Starts the HTTP bridge server in background
#   4. Initializes heartbeat monitoring
#   5. Creates per-agent workspace directories
#   6. Runs the SEXP generator for latest configs
#   7. Prints system status table
#
# Usage:
#   bash .github/omega/omega-bootstrap.sh          # full startup
#   bash .github/omega/omega-bootstrap.sh --check   # verify only, no start
#   bash .github/omega/omega-bootstrap.sh --stop     # stop bridge + heartbeat
#
# All output is logged to .github/omega/logs/
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

set -euo pipefail

# ── Resolve paths ────────────────────────────────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
OMEGA_HOME="$SCRIPT_DIR"
LOG_DIR="$OMEGA_HOME/logs"
TIMESTAMP="$(date +%Y%m%dT%H%M%S)"
LOG_FILE="$LOG_DIR/bootstrap-${TIMESTAMP}.log"
BRIDGE_PID_FILE="$LOG_DIR/bridge.pid"
BRIDGE_LOG="$LOG_DIR/bridge.log"

# ── Logging ──────────────────────────────────────────────────────────────────
mkdir -p "$LOG_DIR"

log() {
  local level="$1"; shift
  local msg="[$(date '+%Y-%m-%dT%H:%M:%S')] [$level] $*"
  echo "$msg" | tee -a "$LOG_FILE"
}

log_ok()   { log "OK"   "$@"; }
log_warn() { log "WARN" "$@"; }
log_err()  { log "ERR"  "$@"; }
log_info() { log "INFO" "$@"; }

# ── Banner ───────────────────────────────────────────────────────────────────
print_banner() {
  cat <<'EOF'
╭──────────────────────────────────────────────────────────────╮
│         ⚡ OMEGA AGENT SYSTEM — Bootstrap v1.0.0 ⚡          │
│              REVITHION STUDIO Multi-Agent Core               │
╰──────────────────────────────────────────────────────────────╯
EOF
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 1: Source environment variables
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
source_env() {
  log_info "Sourcing environment variables..."

  # Omega env (required)
  local omega_env="$OMEGA_HOME/omega-env.sh"
  if [[ -f "$omega_env" ]]; then
    # shellcheck source=omega-env.sh
    source "$omega_env"
    log_ok "Sourced omega-env.sh ($(grep -c '^export' "$omega_env") exports)"
  else
    log_err "Missing: $omega_env"
    return 1
  fi

  # DevTeam6 project env (optional)
  local devteam6_env="${HOME}/projects/devteam6-env.sh"
  if [[ -f "$devteam6_env" ]]; then
    # shellcheck source=/dev/null
    source "$devteam6_env"
    log_ok "Sourced devteam6-env.sh"
  else
    log_warn "Optional devteam6-env.sh not found at $devteam6_env (skipping)"
  fi

  local var_count
  var_count="$(env | grep -c '^OMEGA_' || true)"
  log_info "Total OMEGA_* variables: $var_count"
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 2: Verify external CLI tools
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
verify_clis() {
  log_info "Verifying external CLI tools..."

  local -a required_clis=("gh")
  local -a optional_clis=("gemini" "opencode" "jules" "openclaw" "ollama")
  local total_ok=0
  local total_missing=0
  local total_optional_missing=0

  # Required CLIs
  for cli in "${required_clis[@]}"; do
    if command -v "$cli" >/dev/null 2>&1; then
      log_ok "CLI found: $cli ($(command -v "$cli"))"
      (( total_ok++ ))
    else
      log_err "REQUIRED CLI missing: $cli"
      (( total_missing++ ))
    fi
  done

  # Optional CLIs (warn but don't fail)
  for cli in "${optional_clis[@]}"; do
    if command -v "$cli" >/dev/null 2>&1; then
      log_ok "CLI found: $cli ($(command -v "$cli"))"
      (( total_ok++ ))
    else
      log_warn "Optional CLI not found: $cli (external dispatch to this terminal will fail)"
      (( total_optional_missing++ ))
    fi
  done

  log_info "CLI check: $total_ok found, $total_missing required missing, $total_optional_missing optional missing"

  if (( total_missing > 0 )); then
    log_err "Required CLIs are missing — aborting bootstrap"
    return 1
  fi
  return 0
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 3: Start HTTP bridge server
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
start_bridge() {
  log_info "Starting HTTP bridge server..."

  local bridge_script="$OMEGA_HOME/bridge/http-bridge.py"
  local bridge_port="${OMEGA_BRIDGE_PORT:-18800}"

  if [[ ! -f "$bridge_script" ]]; then
    log_err "Bridge script not found: $bridge_script"
    return 1
  fi

  # Check if bridge is already running
  if [[ -f "$BRIDGE_PID_FILE" ]]; then
    local existing_pid
    existing_pid="$(cat "$BRIDGE_PID_FILE")"
    if kill -0 "$existing_pid" 2>/dev/null; then
      log_warn "Bridge already running (PID $existing_pid) — skipping start"
      return 0
    else
      log_info "Stale PID file found, removing"
      rm -f "$BRIDGE_PID_FILE"
    fi
  fi

  # Check if port is already in use
  if command -v ss >/dev/null 2>&1; then
    if ss -tlnp 2>/dev/null | grep -q ":${bridge_port} "; then
      log_warn "Port $bridge_port already in use — bridge may already be running"
      return 0
    fi
  fi

  # Start bridge in background
  local bridge_config="$OMEGA_HOME/bridge/terminal-registry.json"
  local config_flag=""
  if [[ -f "$bridge_config" ]]; then
    config_flag="--config $bridge_config"
  fi

  python3 "$bridge_script" \
    --port "$bridge_port" \
    $config_flag \
    --log-level "${OMEGA_LOG_LEVEL:-info}" \
    >> "$BRIDGE_LOG" 2>&1 &

  local bridge_pid=$!
  echo "$bridge_pid" > "$BRIDGE_PID_FILE"

  # Wait briefly and verify it started
  sleep 1
  if kill -0 "$bridge_pid" 2>/dev/null; then
    log_ok "HTTP bridge started (PID $bridge_pid, port $bridge_port)"
  else
    log_err "Bridge failed to start — check $BRIDGE_LOG"
    return 1
  fi
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 4: Initialize heartbeat monitoring
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
init_heartbeat() {
  log_info "Initializing heartbeat monitoring..."

  local interval="${OMEGA_HEARTBEAT_INTERVAL:-30}"
  local bridge_port="${OMEGA_BRIDGE_PORT:-18800}"
  local bridge_url="http://localhost:${bridge_port}"

  # Verify bridge is reachable before enabling heartbeat
  local retries=3
  local attempt=0
  while (( attempt < retries )); do
    if curl -sf "${bridge_url}/health" >/dev/null 2>&1; then
      log_ok "Bridge health check passed"
      break
    fi
    (( attempt++ ))
    if (( attempt < retries )); then
      log_info "Waiting for bridge... (attempt $attempt/$retries)"
      sleep 2
    fi
  done

  if (( attempt >= retries )); then
    log_warn "Bridge not responding to health checks — heartbeat will start when bridge is available"
  fi

  # Record heartbeat configuration
  log_info "Heartbeat config: interval=${interval}s, timeout=${OMEGA_HEARTBEAT_TIMEOUT:-10}s, max_retries=${OMEGA_HEARTBEAT_MAX_RETRIES:-3}"
  log_ok "Heartbeat monitoring initialized"
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 5: Create per-agent workspace directories
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
create_workspaces() {
  log_info "Creating per-agent workspace directories..."

  local workspace_root="${OMEGA_WORKSPACE_DIR:-$REPO_ROOT/.planning}"
  local -a agents=(
    "omega-orchestrator"
    "omega-critic"
    "omega-heartbeat"
    "omega-fleet-commander"
    "omega-researcher"
    "omega-builder"
    "omega-security"
    "omega-audio-lead"
    "omega-code-lead"
    "omega-research-lead"
  )

  local created=0
  for agent in "${agents[@]}"; do
    local ws_dir="$workspace_root/$agent"
    if [[ ! -d "$ws_dir" ]]; then
      mkdir -p "$ws_dir"
      (( created++ ))
    fi
  done

  log_ok "Agent workspaces ready: ${#agents[@]} agents, $created newly created"
  log_info "Workspace root: $workspace_root"
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 6: Run SEXP generator
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
run_generator() {
  log_info "Running SEXP generator..."

  local generator="$OMEGA_HOME/macros/generate-omega.py"
  local gen_log="$LOG_DIR/generator-${TIMESTAMP}.log"

  if [[ ! -f "$generator" ]]; then
    log_warn "Generator not found: $generator (skipping)"
    return 0
  fi

  if python3 "$generator" --all >> "$gen_log" 2>&1; then
    log_ok "Generator completed — output in $gen_log"
  else
    log_warn "Generator exited with errors — check $gen_log"
  fi
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Step 7: Print system status table
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
print_status() {
  local bridge_status="STOPPED"
  local bridge_pid="—"
  if [[ -f "$BRIDGE_PID_FILE" ]]; then
    bridge_pid="$(cat "$BRIDGE_PID_FILE")"
    if kill -0 "$bridge_pid" 2>/dev/null; then
      bridge_status="RUNNING"
    else
      bridge_status="DEAD"
    fi
  fi

  local agent_count
  agent_count="$(find "$OMEGA_HOME/agents" -name '*.agent.md' 2>/dev/null | wc -l)"

  local skill_count
  skill_count="$(find "$OMEGA_HOME/skills" -name 'SKILL.md' 2>/dev/null | wc -l)"

  local global_agent_count
  global_agent_count="$(find "$REPO_ROOT/.github/agents" -name '*.md' 2>/dev/null | wc -l)"

  local var_count
  var_count="$(env | grep -c '^OMEGA_' || true)"

  cat <<EOF

╭────────────────────────────────────────────────────────────────╮
│                  ⚡ OMEGA SYSTEM STATUS ⚡                     │
├─────────────────────┬──────────────────────────────────────────┤
│ Component           │ Status                                   │
├─────────────────────┼──────────────────────────────────────────┤
│ HTTP Bridge         │ ${bridge_status} (PID: ${bridge_pid}, port ${OMEGA_BRIDGE_PORT:-18800})│
│ Omega Agents        │ ${agent_count} agent definitions loaded              │
│ Omega Skills        │ ${skill_count} skill definitions loaded              │
│ Global Agents       │ ${global_agent_count} agents in .github/agents/             │
│ Environment Vars    │ ${var_count} OMEGA_* variables exported           │
│ Workspace Root      │ ${OMEGA_WORKSPACE_DIR:-$REPO_ROOT/.planning}│
│ Log Directory       │ ${LOG_DIR}│
│ Heartbeat Interval  │ ${OMEGA_HEARTBEAT_INTERVAL:-30}s                                  │
│ Fleet Size          │ ${OMEGA_FLEET_SIZE:-100} max parallel agents                │
│ Context Budget      │ ${OMEGA_CONTEXT_BUDGET:-200000} tokens                          │
│ Critic Quorum       │ ${OMEGA_CRITIC_QUORUM:-2}                                       │
├─────────────────────┼──────────────────────────────────────────┤
│ Bootstrap Time      │ $(date '+%Y-%m-%dT%H:%M:%S')                     │
│ Log File            │ ${LOG_FILE}│
╰─────────────────────┴──────────────────────────────────────────╯
EOF
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Stop command
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stop_omega() {
  log_info "Stopping Omega services..."

  if [[ -f "$BRIDGE_PID_FILE" ]]; then
    local pid
    pid="$(cat "$BRIDGE_PID_FILE")"
    if kill -0 "$pid" 2>/dev/null; then
      kill "$pid" 2>/dev/null || true
      log_ok "Stopped HTTP bridge (PID $pid)"
    else
      log_info "Bridge was not running (stale PID $pid)"
    fi
    rm -f "$BRIDGE_PID_FILE"
  else
    log_info "No bridge PID file found"
  fi

  log_ok "Omega services stopped"
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Main
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
main() {
  print_banner

  case "${1:-}" in
    --stop)
      stop_omega
      exit 0
      ;;
    --check)
      source_env
      verify_clis
      omega_verify
      print_status
      exit 0
      ;;
  esac

  log_info "Bootstrap started at $(date '+%Y-%m-%dT%H:%M:%S')"
  log_info "Repository root: $REPO_ROOT"

  source_env
  verify_clis
  start_bridge
  init_heartbeat
  create_workspaces
  run_generator
  print_status

  log_ok "Omega bootstrap complete"
}

main "$@"
