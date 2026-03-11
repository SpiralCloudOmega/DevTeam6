#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-omega.sh — Master test runner for the Omega Agent System
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Runs all Omega integration tests and reports results.
# Exit 0 if all pass, exit 1 if any fail.
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

passed=0
failed=0
skipped=0
total=0
declare -a results=()

run_suite() {
  local name="$1"
  local script="$2"
  total=$((total + 1))

  if [[ ! -f "$script" ]]; then
    results+=("${YELLOW}SKIP${RESET}  $name  (file not found: $script)")
    skipped=$((skipped + 1))
    return
  fi

  printf "${CYAN}▶ Running:${RESET} %s\n" "$name"
  local start_time
  start_time=$(date +%s%N 2>/dev/null || date +%s)

  local output
  local exit_code=0
  output=$(bash "$script" 2>&1) || exit_code=$?

  local end_time
  end_time=$(date +%s%N 2>/dev/null || date +%s)
  local duration_ms=$(( (end_time - start_time) / 1000000 ))

  if [[ $exit_code -eq 0 ]]; then
    results+=("${GREEN}PASS${RESET}  $name  (${duration_ms}ms)")
    passed=$((passed + 1))
  else
    results+=("${RED}FAIL${RESET}  $name  (exit code: $exit_code)")
    failed=$((failed + 1))
    # Show failure output indented
    while IFS= read -r line; do
      printf "       %s\n" "$line"
    done <<< "$output"
  fi
}

printf "\n${BOLD}━━━ Omega Agent System — Integration Test Suite ━━━${RESET}\n\n"

cd "$REPO_ROOT"

run_suite "Environment Variables"   "$TESTS_DIR/test-env.sh"
run_suite "Agent Definitions"       "$TESTS_DIR/test-agents.sh"
run_suite "Skill Definitions"       "$TESTS_DIR/test-skills.sh"
run_suite "Macro Validation"        "$TESTS_DIR/test-macros.sh"
run_suite "Generator Pipeline"      "$TESTS_DIR/test-generator.sh"
run_suite "HTTP Bridge"             "$TESTS_DIR/test-bridge.sh"

printf "\n${BOLD}━━━ Results ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}\n\n"

for r in "${results[@]}"; do
  printf "  %b\n" "$r"
done

printf "\n${BOLD}━━━ Summary ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}\n"
printf "  Total: %d  |  ${GREEN}Passed: %d${RESET}  |  ${RED}Failed: %d${RESET}  |  ${YELLOW}Skipped: %d${RESET}\n\n" \
  "$total" "$passed" "$failed" "$skipped"

if [[ $failed -gt 0 ]]; then
  printf "${RED}${BOLD}✗ Some tests failed.${RESET}\n\n"
  exit 1
else
  printf "${GREEN}${BOLD}✓ All tests passed.${RESET}\n\n"
  exit 0
fi
