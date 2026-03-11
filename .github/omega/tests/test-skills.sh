#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-skills.sh — Skill definition validation tests
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"
SKILLS_DIR="$REPO_ROOT/.github/omega/skills"

fail_count=0
pass_count=0

pass() { pass_count=$((pass_count + 1)); printf "  ✓ %s\n" "$1"; }
fail() { fail_count=$((fail_count + 1)); printf "  ✗ %s\n" "$1" >&2; }

printf "── test-skills: Skill Definition Validation ──\n"

# ── Test 1: Skill files exist ──────────────────────────────────────────────
printf "\n[1/3] Checking skill files...\n"
skill_files=()
while IFS= read -r -d '' f; do
  skill_files+=("$f")
done < <(find "$SKILLS_DIR" -name 'SKILL.md' -print0 2>/dev/null | sort -z)

if [[ ${#skill_files[@]} -eq 0 ]]; then
  fail "No SKILL.md files found in $SKILLS_DIR"
  exit 1
fi
pass "Found ${#skill_files[@]} skill files"

# ── Test 2: Valid frontmatter and required sections ────────────────────────
printf "\n[2/3] Validating YAML frontmatter and required sections...\n"
for skill_file in "${skill_files[@]}"; do
  skill_dir="$(basename "$(dirname "$skill_file")")"

  # Check not empty
  if [[ ! -s "$skill_file" ]]; then
    fail "$skill_dir/SKILL.md: file is empty"
    continue
  fi
  pass "$skill_dir/SKILL.md: not empty ($(wc -l < "$skill_file") lines)"

  content=$(cat "$skill_file")

  # Check for YAML frontmatter (optional for skills, but validate if present)
  first_line=$(head -1 "$skill_file")
  if [[ "$first_line" == "---" ]]; then
    # Validate frontmatter closes
    fm_end=$(sed -n '2,$ { /^---$/= }' "$skill_file" | head -1)
    if [[ -n "$fm_end" ]]; then
      pass "$skill_dir/SKILL.md: valid YAML frontmatter (closes at line $fm_end)"
    else
      fail "$skill_dir/SKILL.md: unclosed YAML frontmatter"
    fi
  fi

  # Check for Anti-Patterns section
  if echo "$content" | grep -qiE '(anti.?pattern|anti.?patterns)'; then
    pass "$skill_dir/SKILL.md: has Anti-Patterns section"
  else
    fail "$skill_dir/SKILL.md: missing Anti-Patterns section"
  fi

  # Check for Checklist section
  if echo "$content" | grep -qiE '(checklist|check.?list|verification|acceptance)'; then
    pass "$skill_dir/SKILL.md: has Checklist section"
  else
    fail "$skill_dir/SKILL.md: missing Checklist section"
  fi
done

# ── Test 3: No empty skill files ──────────────────────────────────────────
printf "\n[3/3] Checking for empty skill files...\n"
empty_count=0
for skill_file in "${skill_files[@]}"; do
  if [[ ! -s "$skill_file" ]]; then
    skill_dir="$(basename "$(dirname "$skill_file")")"
    fail "$skill_dir/SKILL.md is empty"
    empty_count=$((empty_count + 1))
  fi
done
if [[ $empty_count -eq 0 ]]; then
  pass "No empty skill files"
fi

# ── Summary ────────────────────────────────────────────────────────────────
printf "\n── skills results: %d passed, %d failed ──\n" "$pass_count" "$fail_count"
[[ $fail_count -eq 0 ]] && exit 0 || exit 1
