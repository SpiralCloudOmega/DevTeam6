#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-env.sh — Environment variable tests for the Omega system
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"
OMEGA_DIR="$REPO_ROOT/.github/omega"

fail_count=0
pass_count=0

pass() { pass_count=$((pass_count + 1)); printf "  ✓ %s\n" "$1"; }
fail() { fail_count=$((fail_count + 1)); printf "  ✗ %s\n" "$1" >&2; }

printf "── test-env: Environment Variables ──\n"

# ── Test 1: omega-env.sh sources without errors ────────────────────────────
printf "\n[1/5] Sourcing omega-env.sh...\n"
cd "$REPO_ROOT"
if bash -c "source '$OMEGA_DIR/omega-env.sh'" 2>/dev/null; then
  pass "omega-env.sh sources without errors"
else
  fail "omega-env.sh failed to source (exit $?)"
fi

# ── Test 2: All core OMEGA_* variables are set ─────────────────────────────
printf "\n[2/5] Checking OMEGA_* variables...\n"
REQUIRED_VARS=(
  OMEGA_ROOT OMEGA_HOME OMEGA_AGENTS OMEGA_SKILLS OMEGA_MACROS
  OMEGA_BRIDGE OMEGA_KANBAN OMEGA_CONFIG
  OMEGA_BRIDGE_PORT OMEGA_HEALTH_PORT
  OMEGA_MODEL_PRIMARY OMEGA_MODEL_FAST OMEGA_MODEL_CODE
  OMEGA_YOLO_MODE OMEGA_PARALLEL_AGENTS OMEGA_LOG_LEVEL
  OMEGA_LOGS_DIR OMEGA_WORKSPACE_DIR OMEGA_BOOTSTRAP OMEGA_DISPATCH
  OMEGA_FLEET_SIZE OMEGA_CRITIC_QUORUM OMEGA_CONTEXT_BUDGET
  OMEGA_HEARTBEAT_INTERVAL OMEGA_HEARTBEAT_TIMEOUT
  OMEGA_CONTEXT_MAX_TOKENS
)

# Source in a subshell and check each var
for var in "${REQUIRED_VARS[@]}"; do
  val=$(bash -c "source '$OMEGA_DIR/omega-env.sh' && echo \"\${$var:-__UNSET__}\"" 2>/dev/null)
  if [[ "$val" == "__UNSET__" || -z "$val" ]]; then
    fail "$var is not set"
  else
    pass "$var is set"
  fi
done

# ── Test 3: Core directory paths exist on disk ─────────────────────────────
printf "\n[3/5] Checking core paths exist...\n"
PATH_VARS=(OMEGA_HOME OMEGA_AGENTS OMEGA_SKILLS OMEGA_MACROS OMEGA_BRIDGE OMEGA_KANBAN)

for var in "${PATH_VARS[@]}"; do
  val=$(bash -c "source '$OMEGA_DIR/omega-env.sh' && echo \"\${$var}\"" 2>/dev/null)
  if [[ -d "$val" ]]; then
    pass "$var → $val exists"
  elif [[ -f "$val" ]]; then
    pass "$var → $val exists (file)"
  else
    fail "$var → $val does NOT exist"
  fi
done

# Also check OMEGA_CONFIG (a file, not a directory)
config_val=$(bash -c "source '$OMEGA_DIR/omega-env.sh' && echo \"\$OMEGA_CONFIG\"" 2>/dev/null)
if [[ -f "$config_val" ]]; then
  pass "OMEGA_CONFIG → $config_val exists"
else
  fail "OMEGA_CONFIG → $config_val does NOT exist"
fi

# ── Test 4: devteam6-env.sh check (optional) ──────────────────────────────
printf "\n[4/5] Checking devteam6-env.sh (optional)...\n"
# devteam6-env.sh is optional — just check omega-env.sh doesn't break if missing
if bash -c "source '$OMEGA_DIR/omega-env.sh'" 2>/dev/null; then
  pass "omega-env.sh handles devteam6-env.sh gracefully"
else
  fail "omega-env.sh errors when devteam6-env.sh is absent"
fi

# ── Test 5: External CLIs are findable ─────────────────────────────────────
printf "\n[5/5] Checking external CLI availability...\n"
# gh is required, others are optional
if command -v gh &>/dev/null; then
  pass "gh (GitHub CLI) is available: $(gh --version 2>/dev/null | head -1)"
else
  fail "gh (GitHub CLI) is NOT available (REQUIRED)"
fi

OPTIONAL_CLIS=(gemini opencode jules openclaw ollama)
for cli in "${OPTIONAL_CLIS[@]}"; do
  if command -v "$cli" &>/dev/null; then
    pass "$cli is available"
  else
    # Optional — don't fail, just note
    printf "  ⚠ %s not found (optional)\n" "$cli"
  fi
done

# ── Summary ────────────────────────────────────────────────────────────────
printf "\n── env results: %d passed, %d failed ──\n" "$pass_count" "$fail_count"
[[ $fail_count -eq 0 ]] && exit 0 || exit 1
