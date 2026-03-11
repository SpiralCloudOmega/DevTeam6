#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-agents.sh — Agent definition validation tests
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"
AGENTS_DIR="$REPO_ROOT/.github/omega/agents"

fail_count=0
pass_count=0

pass() { pass_count=$((pass_count + 1)); printf "  ✓ %s\n" "$1"; }
fail() { fail_count=$((fail_count + 1)); printf "  ✗ %s\n" "$1" >&2; }

printf "── test-agents: Agent Definition Validation ──\n"

# ── Test 1: Agent files exist ──────────────────────────────────────────────
printf "\n[1/3] Checking agent files...\n"
agent_files=()
while IFS= read -r -d '' f; do
  agent_files+=("$f")
done < <(find "$AGENTS_DIR" -name '*.agent.md' -print0 2>/dev/null | sort -z)

if [[ ${#agent_files[@]} -eq 0 ]]; then
  fail "No .agent.md files found in $AGENTS_DIR"
  exit 1
fi
pass "Found ${#agent_files[@]} agent files"

# ── Test 2: Valid YAML frontmatter and required sections ───────────────────
printf "\n[2/3] Validating YAML frontmatter and required sections...\n"
for agent_file in "${agent_files[@]}"; do
  basename_f="$(basename "$agent_file")"

  # Check YAML frontmatter exists (starts with ---)
  first_line=$(head -1 "$agent_file")
  if [[ "$first_line" != "---" ]]; then
    fail "$basename_f: missing YAML frontmatter (first line is not '---')"
    continue
  fi

  # Extract frontmatter (between first and second ---)
  frontmatter=$(sed -n '1,/^---$/p' "$agent_file" | tail -n +2 | head -n -1)
  if [[ -z "$frontmatter" ]]; then
    fail "$basename_f: empty YAML frontmatter"
    continue
  fi

  # Check for 'name:' in frontmatter
  if echo "$frontmatter" | grep -q '^name:'; then
    pass "$basename_f: has 'name' in frontmatter"
  else
    fail "$basename_f: missing 'name' in frontmatter"
  fi

  # Check for 'description:' in frontmatter
  if echo "$frontmatter" | grep -q '^description:'; then
    pass "$basename_f: has 'description' in frontmatter"
  else
    fail "$basename_f: missing 'description' in frontmatter"
  fi

  # Check for system prompt section (any heading containing "system" or "prompt" or "role")
  content=$(sed -n '/^---$/,$ p' "$agent_file" | tail -n +2)
  if echo "$content" | grep -qiE '^#{1,3}\s.*(system|prompt|role|expertise|purpose)'; then
    pass "$basename_f: has system prompt / role section"
  else
    fail "$basename_f: missing system prompt / role section"
  fi
done

# ── Test 3: No duplicate agent names ──────────────────────────────────────
printf "\n[3/3] Checking for duplicate agent names...\n"
declare -A seen_names
has_dupes=0
for agent_file in "${agent_files[@]}"; do
  basename_f="$(basename "$agent_file")"
  agent_name=$(sed -n '/^---$/,/^---$/{ /^name:/{ s/^name:\s*//; s/^["'"'"']//; s/["'"'"']$//; p; } }' "$agent_file" | head -1)
  if [[ -z "$agent_name" ]]; then
    continue
  fi
  if [[ -n "${seen_names[$agent_name]:-}" ]]; then
    fail "Duplicate agent name '$agent_name' in $basename_f and ${seen_names[$agent_name]}"
    has_dupes=1
  else
    seen_names[$agent_name]="$basename_f"
  fi
done
if [[ $has_dupes -eq 0 ]]; then
  pass "No duplicate agent names (${#seen_names[@]} unique agents)"
fi

# ── Summary ────────────────────────────────────────────────────────────────
printf "\n── agents results: %d passed, %d failed ──\n" "$pass_count" "$fail_count"
[[ $fail_count -eq 0 ]] && exit 0 || exit 1
