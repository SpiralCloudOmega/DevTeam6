#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# omega-env.sh — Omega RLM Environment Variables
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#
# RLM (Recursive Language Model) Pattern — arXiv:2512.24601
#
# All configuration is environment variables pointing to FILES and PATHS,
# never inline data. The LM treats its own context as a mutable environment
# in a REPL: it can read variables, modify them, and recursively invoke
# itself with the updated environment. This file is the bootstrap — source
# it once and every sub-agent, script, or tool inherits the full topology.
#
# Usage:
#   source .github/omega/omega-env.sh        # from repo root
#   omega_verify                              # check critical paths
#   omega_status                              # print all variables
#
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Note: no set -euo pipefail — this file is sourced, not executed

# ── Resolve workspace root ──────────────────────────────────────────────────
# Walk up from this script's location to the repository root.
_OMEGA_SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export OMEGA_ROOT="$(cd "$_OMEGA_SCRIPT_DIR/../.." && pwd)"

# ── Helper: resolve relative paths against OMEGA_ROOT ───────────────────────
omega_resolve() {
  local rel="$1"
  if [[ "$rel" == /* ]]; then
    echo "$rel"
  else
    echo "${OMEGA_ROOT}/${rel}"
  fi
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 1. Core Omega paths
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_HOME="$(omega_resolve .github/omega)"
export OMEGA_AGENTS="$(omega_resolve .github/omega/agents)"
export OMEGA_SKILLS="$(omega_resolve .github/omega/skills)"
export OMEGA_MACROS="$(omega_resolve .github/omega/macros)"
export OMEGA_BRIDGE="$(omega_resolve .github/omega/bridge)"
export OMEGA_KANBAN="$(omega_resolve .github/omega/kanban)"
export OMEGA_CONFIG="$(omega_resolve .github/omega/omega.config.sexp)"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 2. Existing infrastructure paths
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_GLOBAL_AGENTS="$(omega_resolve .github/agents)"
export OMEGA_GLOBAL_SKILLS="$(omega_resolve .github/skills)"
export OMEGA_REVITHION="$(omega_resolve REVITHION-STUDIO)"
export OMEGA_TOKENS="$(omega_resolve REVITHION-STUDIO/tokens)"
export OMEGA_MEMORY_DB="$(omega_resolve .github/memory/spiralcloud-memory.db)"
export OMEGA_ENV_PROFILE="$(omega_resolve .github/memory/environment-profile.json)"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 3. Component paths (stack references)
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_CHRYSALISP="$(omega_resolve ChrysaLisp-master)"
export OMEGA_RLM="$(omega_resolve rlm)"
export OMEGA_FULCRUM="$(omega_resolve fulcrum-main)"
export OMEGA_CHORUS="$(omega_resolve Chorus-main)"
export OMEGA_COWORK="$(omega_resolve Open-Claude-Cowork-main)"
export OMEGA_LLM_COUNCIL="$(omega_resolve llm-council-master)"
export OMEGA_CONTEXT_MODE="$(omega_resolve context-mode-main)"
export OMEGA_MCP2CLI="$(omega_resolve mcp2cli-main)"
export OMEGA_SKILLKIT="$(omega_resolve skillkit-main)"
export OMEGA_PLANNING="$(omega_resolve planning-with-files-master)"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 4. External CLI terminal endpoints
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_CLI_GEMINI="/usr/local/bin/gemini"
export OMEGA_CLI_OPENCODE="${HOME}/.opencode/bin/opencode"
export OMEGA_CLI_JULES="/usr/bin/jules"
export OMEGA_CLI_OPENCLAW="/usr/bin/openclaw"
export OMEGA_CLI_OLLAMA="/usr/local/bin/ollama"
export OMEGA_CLI_GH="$(which gh 2>/dev/null || echo /usr/bin/gh)"
export OMEGA_CLI_KIMI="http://localhost:5494"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 5. Service ports
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_BRIDGE_PORT=18800
export OMEGA_HEALTH_PORT=18801
export OMEGA_OPENCLAW_PORT=18789
export OMEGA_OLLAMA_PORT=11434
export OMEGA_ACESTEP_PORT=8001
export OMEGA_COPILOTKIT_PORT=3003
export OMEGA_OPENDAW_PORT=8080
export OMEGA_POSTGRES_PORT=5432
export OMEGA_CYBORGDB_PORT=8100
export OMEGA_KIMI_PORT=5494

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 6. Per-agent workspace paths
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_WS_ORCHESTRATOR="$(omega_resolve .planning/omega-orchestrator)"
export OMEGA_WS_CRITIC="$(omega_resolve .planning/omega-critic)"
export OMEGA_WS_HEARTBEAT="$(omega_resolve .planning/omega-heartbeat)"
export OMEGA_WS_AUDIO_LEAD="$(omega_resolve .planning/omega-audio-lead)"
export OMEGA_WS_CODE_LEAD="$(omega_resolve .planning/omega-code-lead)"
export OMEGA_WS_RESEARCH_LEAD="$(omega_resolve .planning/omega-research-lead)"
export OMEGA_WS_BUILDER="$(omega_resolve .planning/omega-builder)"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 7. Heartbeat / health config
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_HEARTBEAT_INTERVAL=30
export OMEGA_HEARTBEAT_TIMEOUT=10
export OMEGA_HEARTBEAT_MAX_RETRIES=3
export OMEGA_HEARTBEAT_RECOVERY_DELAY=60

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 8. Model preferences (RLM pointers — reference the model, don't embed it)
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_MODEL_PRIMARY="claude-opus-4-20250514"
export OMEGA_MODEL_FAST="claude-haiku-4-20250414"
export OMEGA_MODEL_CODE="claude-sonnet-4-20250514"
export OMEGA_MODEL_RESEARCH="gemini-2.5-pro"
export OMEGA_MODEL_LOCAL="qwen3:8b"
export OMEGA_MODEL_LOCAL_LARGE="qwen3-coder:30b"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 9. Context management
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_CONTEXT_MAX_TOKENS=200000
export OMEGA_CONTEXT_REDUCTION_TARGET=0.02
export OMEGA_FTS5_TOKENIZER="porter unicode61"
export OMEGA_CHUNK_MAX_BYTES=4096

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 10. Runtime flags
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_YOLO_MODE=1
export OMEGA_PARALLEL_AGENTS=100
export OMEGA_LOG_LEVEL="info"
export OMEGA_DEBUG=0

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Functions
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# omega_verify — check that all critical paths exist on disk
omega_verify() {
  local rc=0
  local critical_paths=(
    "$OMEGA_ROOT"
    "$OMEGA_HOME"
    "$OMEGA_GLOBAL_AGENTS"
    "$OMEGA_GLOBAL_SKILLS"
    "$OMEGA_REVITHION"
    "$OMEGA_MEMORY_DB"
    "$OMEGA_ENV_PROFILE"
    "$OMEGA_RLM"
    "$OMEGA_CHRYSALISP"
    "$OMEGA_FULCRUM"
    "$OMEGA_CHORUS"
    "$OMEGA_COWORK"
  )

  printf "╭─── omega_verify ───────────────────────────────────────╮\n"
  for p in "${critical_paths[@]}"; do
    if [[ -e "$p" ]]; then
      printf "│  ✓  %-50s │\n" "${p#"$OMEGA_ROOT"/}"
    else
      printf "│  ✗  %-50s │\n" "${p#"$OMEGA_ROOT"/}"
      rc=1
    fi
  done
  printf "╰────────────────────────────────────────────────────────╯\n"

  if (( rc == 0 )); then
    echo "All critical paths verified."
  else
    echo "WARNING: some paths are missing (see ✗ above)."
  fi
  return $rc
}

# omega_status — print every OMEGA_* variable and its value
omega_status() {
  printf "╭─── omega_status ──────────────────────────────────────╮\n"
  printf "│  %-36s  %s\n" "Variable" "Value"
  printf "│  %-36s  %s\n" "────────────────────────────────────" "─────────────────"

  # Collect and sort all OMEGA_ vars
  while IFS='=' read -r name value; do
    # Shorten absolute paths relative to OMEGA_ROOT for readability
    local display="$value"
    if [[ "$value" == "${OMEGA_ROOT}/"* ]]; then
      display="\${OMEGA_ROOT}/${value#"$OMEGA_ROOT"/}"
    fi
    printf "│  %-36s  %s\n" "$name" "$display"
  done < <(env | grep '^OMEGA_' | sort)

  printf "╰────────────────────────────────────────────────────────╯\n"
  echo "Total: $(env | grep -c '^OMEGA_') variables exported."
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 11. Omega system scripts and directories
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_LOGS_DIR="$(omega_resolve .github/omega/logs)"
export OMEGA_WORKSPACE_DIR="$(omega_resolve .planning)"
export OMEGA_BOOTSTRAP="$(omega_resolve .github/omega/omega-bootstrap.sh)"
export OMEGA_DISPATCH="$(omega_resolve .github/omega/omega-dispatch.sh)"

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 12. Fleet and governance settings
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
export OMEGA_FLEET_SIZE=100
export OMEGA_CRITIC_QUORUM=2
export OMEGA_CONTEXT_BUDGET=200000

# Additional per-agent workspace paths
export OMEGA_WS_FLEET_COMMANDER="$(omega_resolve .planning/omega-fleet-commander)"
export OMEGA_WS_RESEARCHER="$(omega_resolve .planning/omega-researcher)"
export OMEGA_WS_SECURITY="$(omega_resolve .planning/omega-security)"

# Guard: warn if sourced from wrong directory
if [[ ! -d "$OMEGA_ROOT/.github" ]]; then
  echo "omega-env.sh: WARNING — OMEGA_ROOT ($OMEGA_ROOT) does not look like the DevTeam6 repo." >&2
fi
