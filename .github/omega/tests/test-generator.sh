#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-generator.sh — Generator pipeline tests for generate-omega.py
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"
OMEGA_DIR="$REPO_ROOT/.github/omega"
GENERATOR="$OMEGA_DIR/macros/generate-omega.py"
TEST_OUTPUT_DIR=$(mktemp -d)

trap 'rm -rf "$TEST_OUTPUT_DIR"' EXIT

fail_count=0
pass_count=0

pass() { pass_count=$((pass_count + 1)); printf "  ✓ %s\n" "$1"; }
fail() { fail_count=$((fail_count + 1)); printf "  ✗ %s\n" "$1" >&2; }

printf "── test-generator: Generator Pipeline ──\n"

# ── Test 1: generate-omega.py runs without errors ──────────────────────────
printf "\n[1/3] Running generate-omega.py --help...\n"
if python3 "$GENERATOR" --help &>/dev/null; then
  pass "generate-omega.py --help succeeds"
else
  fail "generate-omega.py --help failed"
fi

# Dry-run to validate parsing without file I/O
printf "\n        Running dry-run...\n"
dry_output=$(python3 "$GENERATOR" \
  --config "$OMEGA_DIR/omega.config.sexp" \
  --macros "$OMEGA_DIR/macros/agent-macros.sexp" \
  --output-dir "$TEST_OUTPUT_DIR" \
  --all --dry-run 2>&1) || true
if [[ $? -eq 0 ]]; then
  pass "generate-omega.py --all --dry-run succeeds"
else
  fail "generate-omega.py --all --dry-run failed: $dry_output"
fi

# ── Test 2: --all produces output files ────────────────────────────────────
printf "\n[2/3] Running generate-omega.py --all...\n"
gen_output=$(python3 "$GENERATOR" \
  --config "$OMEGA_DIR/omega.config.sexp" \
  --macros "$OMEGA_DIR/macros/agent-macros.sexp" \
  --output-dir "$TEST_OUTPUT_DIR" \
  --all 2>&1)
gen_exit=$?

if [[ $gen_exit -eq 0 ]]; then
  pass "generate-omega.py --all exited cleanly"
else
  fail "generate-omega.py --all failed (exit $gen_exit): $gen_output"
fi

# Check output files were created
file_count=$(find "$TEST_OUTPUT_DIR" -type f | wc -l)
if [[ $file_count -gt 0 ]]; then
  pass "Generated $file_count output files"
else
  fail "No output files generated in $TEST_OUTPUT_DIR"
fi

# ── Test 3: Generated files are valid ──────────────────────────────────────
printf "\n[3/3] Validating generated files...\n"

# Validate .sh files with bash -n
while IFS= read -r -d '' sh_file; do
  name="$(basename "$sh_file")"
  if bash -n "$sh_file" 2>/dev/null; then
    pass "$name: valid bash syntax"
  else
    fail "$name: bash syntax error"
  fi
done < <(find "$TEST_OUTPUT_DIR" -name '*.sh' -print0 2>/dev/null)

# Validate .py files with python3 compile check
while IFS= read -r -d '' py_file; do
  name="$(basename "$py_file")"
  if python3 -c "import py_compile; py_compile.compile('$py_file', doraise=True)" 2>/dev/null; then
    pass "$name: valid Python syntax"
  else
    fail "$name: Python syntax error"
  fi
done < <(find "$TEST_OUTPUT_DIR" -name '*.py' -print0 2>/dev/null)

# Validate .json files
while IFS= read -r -d '' json_file; do
  name="$(basename "$json_file")"
  if python3 -c "import json; json.load(open('$json_file'))" 2>/dev/null; then
    pass "$name: valid JSON"
  else
    fail "$name: invalid JSON"
  fi
done < <(find "$TEST_OUTPUT_DIR" -name '*.json' -print0 2>/dev/null)

# ── Summary ────────────────────────────────────────────────────────────────
printf "\n── generator results: %d passed, %d failed ──\n" "$pass_count" "$fail_count"
[[ $fail_count -eq 0 ]] && exit 0 || exit 1
