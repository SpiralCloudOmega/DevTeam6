#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# omega-dispatch.sh — Send commands to external CLI terminals via HTTP bridge
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#
# Routes commands to external terminals (Gemini, OpenCode, JULES, OpenClaw,
# Ollama, GitHub CLI) through the Omega HTTP bridge server.
#
# Usage:
#   omega-dispatch <terminal> <command>
#   omega-dispatch gemini "Research the ChrysaLisp macro system"
#   omega-dispatch opencode "Fix the bug in src/audio.cpp"
#   omega-dispatch jules "Set up the test environment"
#   omega-dispatch ollama "Generate embeddings for these files"
#   omega-dispatch --batch commands.json
#   omega-dispatch --health [terminal]
#   omega-dispatch --list
#   omega-dispatch --metrics
#
# Environment:
#   OMEGA_BRIDGE_PORT  — bridge port (default: 18800)
#   OMEGA_BRIDGE_HOST  — bridge host (default: localhost)
#   OMEGA_LOG_LEVEL    — log level (default: info)
#
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

set -euo pipefail

# ── Configuration ────────────────────────────────────────────────────────────
BRIDGE_HOST="${OMEGA_BRIDGE_HOST:-localhost}"
BRIDGE_PORT="${OMEGA_BRIDGE_PORT:-18800}"
BRIDGE_URL="http://${BRIDGE_HOST}:${BRIDGE_PORT}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_DIR="$SCRIPT_DIR/logs"
DISPATCH_LOG="$LOG_DIR/dispatch.log"

mkdir -p "$LOG_DIR"

# ── Helpers ──────────────────────────────────────────────────────────────────
log() {
  echo "[$(date '+%Y-%m-%dT%H:%M:%S')] $*" >> "$DISPATCH_LOG"
}

die() {
  echo "ERROR: $*" >&2
  log "ERROR: $*"
  exit 1
}

check_bridge() {
  if ! curl -sf "${BRIDGE_URL}/health" >/dev/null 2>&1; then
    die "Bridge not reachable at ${BRIDGE_URL} — run omega-bootstrap.sh first"
  fi
}

usage() {
  cat <<EOF
Usage: omega-dispatch <terminal> <command> [--timeout SECONDS] [--session ID]
       omega-dispatch --batch <file.json>
       omega-dispatch --health [terminal]
       omega-dispatch --list
       omega-dispatch --metrics
       omega-dispatch --council <prompt> [--terminals t1,t2,t3]

Terminals: gemini, opencode, jules, openclaw, ollama, gh

Options:
  --timeout SECONDS   Override default timeout for this command
  --session ID        Attach to an existing session
  --batch FILE        Send batch commands from a JSON file
  --health [NAME]     Check health of all or one terminal
  --list              List registered terminals
  --metrics           Show bridge metrics
  --council PROMPT    Run a council vote across terminals
  --terminals LIST    Comma-separated terminal list for council
  --json              Output raw JSON (default: formatted)
EOF
  exit 0
}

# ── Format output ────────────────────────────────────────────────────────────
format_result() {
  local json="$1"
  local raw="${2:-false}"

  if [[ "$raw" == "true" ]]; then
    echo "$json"
    return
  fi

  local status terminal output duration
  status="$(echo "$json" | python3 -c "import sys,json; print(json.load(sys.stdin).get('status','unknown'))" 2>/dev/null || echo "unknown")"
  terminal="$(echo "$json" | python3 -c "import sys,json; print(json.load(sys.stdin).get('terminal',''))" 2>/dev/null || echo "")"
  output="$(echo "$json" | python3 -c "import sys,json; print(json.load(sys.stdin).get('output',''))" 2>/dev/null || echo "")"
  duration="$(echo "$json" | python3 -c "import sys,json; print(json.load(sys.stdin).get('duration_ms',0))" 2>/dev/null || echo "0")"

  local icon="❓"
  case "$status" in
    success) icon="✅" ;;
    error)   icon="❌" ;;
    timeout) icon="⏱️" ;;
  esac

  echo "╭─── ${icon} ${terminal} — ${status} (${duration}ms) ───"
  echo "│"
  echo "$output" | sed 's/^/│  /'
  echo "│"
  echo "╰───────────────────────────────────────────────────────────"
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Commands
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

dispatch_single() {
  local terminal="$1"
  local command="$2"
  local timeout="${3:-}"
  local session="${4:-}"
  local raw_json="${5:-false}"

  check_bridge

  local payload
  payload="{\"terminal\":\"${terminal}\",\"command\":$(python3 -c "import json; print(json.dumps('$command'))" 2>/dev/null || echo "\"$command\"")"

  if [[ -n "$timeout" ]]; then
    payload="${payload},\"timeout\":${timeout}"
  fi
  if [[ -n "$session" ]]; then
    payload="${payload},\"session_id\":\"${session}\""
  fi
  payload="${payload}}"

  log "DISPATCH → $terminal: $command"

  local response
  response="$(curl -sf -X POST "${BRIDGE_URL}/dispatch" \
    -H "Content-Type: application/json" \
    -d "$payload" 2>&1)" || die "Dispatch failed — bridge returned error"

  log "RESULT ← $terminal: $(echo "$response" | head -c 200)"

  format_result "$response" "$raw_json"
}

dispatch_batch() {
  local batch_file="$1"
  local raw_json="${2:-false}"

  if [[ ! -f "$batch_file" ]]; then
    die "Batch file not found: $batch_file"
  fi

  check_bridge

  log "BATCH DISPATCH from $batch_file"

  local payload
  payload="{\"commands\":$(cat "$batch_file")}"

  local response
  response="$(curl -sf -X POST "${BRIDGE_URL}/dispatch/batch" \
    -H "Content-Type: application/json" \
    -d "$payload" 2>&1)" || die "Batch dispatch failed"

  if [[ "$raw_json" == "true" ]]; then
    echo "$response"
  else
    echo "$response" | python3 -c "
import sys, json
data = json.load(sys.stdin)
for r in data.get('results', []):
    icon = '✅' if r.get('status') == 'success' else '❌'
    print(f\"{icon} [{r.get('terminal','')}] {r.get('status','')} ({r.get('duration_ms',0)}ms)\")
    output = r.get('output', '')
    if output:
        for line in output.splitlines()[:5]:
            print(f'   {line}')
        if len(output.splitlines()) > 5:
            print(f'   ... ({len(output.splitlines())} lines total)')
" 2>/dev/null || echo "$response"
  fi
}

show_health() {
  local terminal="${1:-}"
  check_bridge

  local endpoint="/health"
  if [[ -n "$terminal" ]]; then
    endpoint="/health/${terminal}"
  fi

  local response
  response="$(curl -sf "${BRIDGE_URL}${endpoint}" 2>&1)" || die "Health check failed"

  echo "$response" | python3 -c "
import sys, json
data = json.load(sys.stdin)
if 'terminals' in data:
    print('╭─── Terminal Health ─────────────────────────────────────╮')
    for name, info in data['terminals'].items():
        status = info.get('status', 'unknown')
        icon = '🟢' if status == 'healthy' else '🔴' if status in ('offline','not_found') else '🟡'
        latency = info.get('latency_ms', 0)
        print(f'│  {icon} {name:<15} {status:<12} {latency:.0f}ms')
    print('╰────────────────────────────────────────────────────────╯')
    all_ok = data.get('all_healthy', False)
    print(f\"Bridge: {data.get('bridge','?')} | Uptime: {data.get('uptime_s',0):.0f}s | All healthy: {all_ok}\")
else:
    print(json.dumps(data, indent=2))
" 2>/dev/null || echo "$response"
}

list_terminals() {
  check_bridge

  local response
  response="$(curl -sf "${BRIDGE_URL}/terminals" 2>&1)" || die "List terminals failed"

  echo "$response" | python3 -c "
import sys, json
data = json.load(sys.stdin)
print('╭─── Registered Terminals ───────────────────────────────╮')
for name, info in data.get('terminals', {}).items():
    status = info.get('status', 'unknown')
    desc = info.get('description', '')[:40]
    print(f'│  {name:<15} {status:<12} {desc}')
print('╰────────────────────────────────────────────────────────╯')
" 2>/dev/null || echo "$response"
}

show_metrics() {
  check_bridge

  local response
  response="$(curl -sf "${BRIDGE_URL}/metrics" 2>&1)" || die "Metrics fetch failed"

  echo "$response" | python3 -c "
import sys, json
data = json.load(sys.stdin)
bridge = data.pop('_bridge', {})
print('╭─── Bridge Metrics ─────────────────────────────────────╮')
print(f\"│  Uptime: {bridge.get('uptime_s',0):.0f}s  Version: {bridge.get('version','?')}\")
print('├─────────────────────────────────────────────────────────┤')
for name, m in sorted(data.items()):
    reqs = m.get('total_requests', 0)
    errs = m.get('errors', 0)
    avg = m.get('avg_duration_ms', 0)
    rate = m.get('error_rate', 0)
    print(f'│  {name:<15} reqs={reqs:<5} errs={errs:<3} avg={avg:.0f}ms err_rate={rate:.1%}')
print('╰────────────────────────────────────────────────────────╯')
" 2>/dev/null || echo "$response"
}

run_council() {
  local prompt="$1"
  local terminals="${2:-}"
  local raw_json="${3:-false}"

  check_bridge

  local payload="{\"prompt\":$(python3 -c "import json; print(json.dumps('$prompt'))" 2>/dev/null || echo "\"$prompt\"")"
  if [[ -n "$terminals" ]]; then
    local terminals_json
    terminals_json="$(echo "$terminals" | python3 -c "import sys,json; print(json.dumps(sys.stdin.read().strip().split(',')))" 2>/dev/null)"
    payload="${payload},\"terminals\":${terminals_json}"
  fi
  payload="${payload}}"

  log "COUNCIL VOTE: $prompt"

  local response
  response="$(curl -sf -X POST "${BRIDGE_URL}/council/vote" \
    -H "Content-Type: application/json" \
    -d "$payload" 2>&1)" || die "Council vote failed"

  if [[ "$raw_json" == "true" ]]; then
    echo "$response"
  else
    echo "$response" | python3 -c "
import sys, json
data = json.load(sys.stdin)
consensus = data.get('consensus', '?')
tally = data.get('tally', {})
icon = '✅' if consensus == 'APPROVE' else '❌' if consensus == 'REJECT' else '⚖️'
print(f'{icon} Council verdict: {consensus}')
print(f\"   Approve: {tally.get('approve',0)} | Reject: {tally.get('reject',0)} | Abstain: {tally.get('abstain',0)}\")
for v in data.get('votes', []):
    vote_icon = '👍' if v['vote'] == 'APPROVE' else '👎' if v['vote'] == 'REJECT' else '🤷'
    print(f\"   {vote_icon} {v.get('terminal','?')}: {v.get('vote','')} ({v.get('duration_ms',0):.0f}ms)\")
" 2>/dev/null || echo "$response"
  fi
}

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Main
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
main() {
  if (( $# == 0 )); then
    usage
  fi

  local raw_json="false"
  local timeout=""
  local session=""
  local terminals=""

  # Parse leading flags
  case "$1" in
    --help|-h)
      usage
      ;;
    --health)
      shift
      show_health "${1:-}"
      exit 0
      ;;
    --list)
      list_terminals
      exit 0
      ;;
    --metrics)
      show_metrics
      exit 0
      ;;
    --batch)
      shift
      [[ $# -ge 1 ]] || die "Usage: omega-dispatch --batch <file.json>"
      dispatch_batch "$1" "$raw_json"
      exit 0
      ;;
    --council)
      shift
      [[ $# -ge 1 ]] || die "Usage: omega-dispatch --council <prompt> [--terminals t1,t2]"
      local council_prompt="$1"; shift
      while (( $# > 0 )); do
        case "$1" in
          --terminals) shift; terminals="$1"; shift ;;
          --json) raw_json="true"; shift ;;
          *) shift ;;
        esac
      done
      run_council "$council_prompt" "$terminals" "$raw_json"
      exit 0
      ;;
  esac

  # Standard dispatch: omega-dispatch <terminal> <command> [options]
  local terminal="$1"; shift
  [[ $# -ge 1 ]] || die "Usage: omega-dispatch <terminal> <command>"
  local command="$1"; shift

  while (( $# > 0 )); do
    case "$1" in
      --timeout) shift; timeout="$1"; shift ;;
      --session) shift; session="$1"; shift ;;
      --json) raw_json="true"; shift ;;
      *) shift ;;
    esac
  done

  dispatch_single "$terminal" "$command" "$timeout" "$session" "$raw_json"
}

main "$@"
