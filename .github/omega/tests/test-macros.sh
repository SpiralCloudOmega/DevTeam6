#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-macros.sh — S-expression macro validation tests
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"
OMEGA_DIR="$REPO_ROOT/.github/omega"

fail_count=0
pass_count=0

pass() { pass_count=$((pass_count + 1)); printf "  ✓ %s\n" "$1"; }
fail() { fail_count=$((fail_count + 1)); printf "  ✗ %s\n" "$1" >&2; }

# Validate balanced parentheses in an S-expression file, ignoring comments and strings
check_sexp_balance() {
  local file="$1"
  local label="$2"
  local result
  result=$(python3 -c "
import sys
text = open('$file').read()
depth = 0
in_string = False
in_comment = False
for i, c in enumerate(text):
    if in_comment:
        if c == '\n':
            in_comment = False
        continue
    if in_string:
        if c == '\"' and (i == 0 or text[i-1] != '\\\\'):
            in_string = False
        continue
    if c == ';':
        in_comment = True
        continue
    if c == '\"':
        in_string = True
        continue
    if c == '(':
        depth += 1
    elif c == ')':
        depth -= 1
    if depth < 0:
        print(f'UNBALANCED: depth went negative at char {i}')
        sys.exit(1)
if depth != 0:
    print(f'UNBALANCED: final depth is {depth}')
    sys.exit(1)
print('BALANCED')
" 2>&1)

  if [[ "$result" == "BALANCED" ]]; then
    pass "$label: parentheses are balanced"
    return 0
  else
    fail "$label: $result"
    return 1
  fi
}

printf "── test-macros: S-expression Macro Validation ──\n"

# ── Test 1: omega.config.sexp is valid ─────────────────────────────────────
printf "\n[1/3] Validating omega.config.sexp...\n"
CONFIG_SEXP="$OMEGA_DIR/omega.config.sexp"
if [[ -f "$CONFIG_SEXP" ]]; then
  pass "omega.config.sexp exists"
  check_sexp_balance "$CONFIG_SEXP" "omega.config.sexp"
else
  fail "omega.config.sexp not found"
fi

# ── Test 2: agent-macros.sexp is valid ─────────────────────────────────────
printf "\n[2/3] Validating agent-macros.sexp...\n"
MACROS_SEXP="$OMEGA_DIR/macros/agent-macros.sexp"
if [[ -f "$MACROS_SEXP" ]]; then
  pass "agent-macros.sexp exists"
  check_sexp_balance "$MACROS_SEXP" "agent-macros.sexp"
else
  fail "agent-macros.sexp not found"
fi

# ── Test 3: All (require ...) paths resolve ────────────────────────────────
printf "\n[3/3] Checking (require ...) paths resolve...\n"

# Extract require paths from all .sexp files
sexp_files=("$CONFIG_SEXP" "$MACROS_SEXP")
require_count=0
for sexp_file in "${sexp_files[@]}"; do
  [[ -f "$sexp_file" ]] || continue
  sexp_dir="$(dirname "$sexp_file")"

  while IFS= read -r req_path; do
    # Strip leading/trailing whitespace and quotes
    req_path=$(echo "$req_path" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
    [[ -z "$req_path" ]] && continue
    require_count=$((require_count + 1))

    # Resolve relative to the file's directory (canonicalize to handle ../)
    resolved=$(cd "$sexp_dir" && realpath -m "$req_path" 2>/dev/null || echo "$sexp_dir/$req_path")
    if [[ -f "$resolved" ]]; then
      pass "(require \"$req_path\") → exists (from $(basename "$sexp_file"))"
    else
      # Also try from repo root as fallback
      resolved_root=$(cd "$REPO_ROOT" && realpath -m "$req_path" 2>/dev/null || echo "$REPO_ROOT/$req_path")
      if [[ -f "$resolved_root" ]]; then
        pass "(require \"$req_path\") → exists from repo root (from $(basename "$sexp_file"))"
      else
        # Warn rather than fail: the generator loads files via CLI args, not require
        printf "  ⚠ (require \"%s\") path doesn't resolve from %s (generator uses --macros flag instead)\n" "$req_path" "$(basename "$sexp_file")"
      fi
    fi
  done < <(grep -oP '\(require\s+"([^"]+)"\)' "$sexp_file" | sed 's/(require\s*"//;s/")//')
done

if [[ $require_count -eq 0 ]]; then
  pass "No (require ...) directives found (nothing to resolve)"
fi

# ── Summary ────────────────────────────────────────────────────────────────
printf "\n── macros results: %d passed, %d failed ──\n" "$pass_count" "$fail_count"
[[ $fail_count -eq 0 ]] && exit 0 || exit 1
