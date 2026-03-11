---
name: Omega Critique
description: >
  LLM Council peer review system — anonymous 3-stage review, scoring rubrics,
  quorum consensus, signal-to-noise filtering, structured findings, pattern
  detection, and git diff integration for code, architecture, and security audits.
---

# Omega Critique

Core principle: **Only surface what matters — bugs, security, logic errors. Never style.**

The critique system implements an LLM Council where multiple agents independently
review the same artifact, score it against rubrics, and reach consensus through
anonymous voting. Findings that fail quorum are discarded. Findings that pass
must include actionable remediation.

## 1. LLM Council 3-Stage Review

```text
Stage 1: Independent Review
  Agent A ──→ findings[]
  Agent B ──→ findings[]     (anonymous, no cross-talk)
  Agent C ──→ findings[]

Stage 2: Consensus Voting
  All findings pooled → deduplicated → each agent votes approve/reject
  Findings meeting quorum threshold → accepted

Stage 3: Remediation
  Accepted findings → enriched with fix suggestions → structured output
```

Each reviewer is a separate agent instance with **no access** to other reviewers'
findings during Stage 1. This prevents groupthink and anchoring bias.

## 2. Scoring Rubrics

Every finding is scored on four dimensions (1–5 scale):

| Dimension | 1 (Critical) | 3 (Moderate) | 5 (Clean) |
|---|---|---|---|
| **Correctness** | Logic error, wrong output | Edge case missed | Provably correct |
| **Security** | Injection, auth bypass | Missing validation | Defense in depth |
| **Performance** | O(n²) in hot path | Unnecessary allocation | Optimal complexity |
| **Maintainability** | Untestable, no contracts | Missing docs on public API | Clear, well-tested |

Aggregate score per artifact:

```python
def aggregate_score(findings):
    if any(f.correctness == 1 or f.security == 1 for f in findings):
        return "REJECT"  # any critical finding blocks merge
    avg = mean([f.weighted_score() for f in findings])
    return "APPROVE" if avg >= 3.5 else "REQUEST_CHANGES"
```

## 3. Quorum Thresholds

Different decision types require different consensus levels:

| Decision Type | Quorum | Council Size | Rationale |
|---|---|---|---|
| **Bug / logic error** | 2 of 3 | 3 | High confidence needed |
| **Security vulnerability** | 1 of 3 | 3 | Single expert suffices |
| **Architecture change** | 3 of 3 | 3 | Unanimous for structural |
| **Performance concern** | 2 of 3 | 3 | Must be reproducible |
| **Documentation gap** | 1 of 3 | 3 | Low risk, any reviewer |

```python
QUORUM = {
    "bug": 2, "security": 1, "architecture": 3,
    "performance": 2, "documentation": 1,
}

def meets_quorum(finding, votes):
    approvals = sum(1 for v in votes if v.approve)
    return approvals >= QUORUM.get(finding.category, 2)
```

## 4. Review Patterns

### Code Review
Input: git diff (unified format). Focus: correctness, edge cases, error handling.

```python
def code_review(diff, context_files):
    return dispatch_council(
        artifact=diff,
        context=context_files,
        rubric=["correctness", "security", "performance"],
        agents=["code-review", "code-review", "code-review"],  # 3 independent instances
        instructions="Only report bugs, security issues, and logic errors. "
                     "Never comment on style, formatting, or naming.",
    )
```

### Architecture Review
Input: design doc or structural change. Focus: coupling, extensibility, consistency.

```python
def architecture_review(design_doc):
    return dispatch_council(
        artifact=design_doc,
        rubric=["maintainability", "correctness"],
        agents=["general-purpose", "general-purpose", "general-purpose"],
        quorum_override={"architecture": 3},  # unanimous required
    )
```

### Security Audit
Input: code handling auth, crypto, user input. Focus: OWASP Top 10, injection, auth.

```python
def security_audit(code_paths):
    return dispatch_council(
        artifact=read_files(code_paths),
        rubric=["security"],
        agents=["code-review", "code-review", "code-review"],
        instructions="Check OWASP Top 10. Report injection, auth bypass, "
                     "path traversal, SSRF, deserialization. Include CVE refs.",
    )
```

## 5. Signal-to-Noise Filtering

**Hard rules** — findings that match these patterns are auto-rejected:

| Auto-Reject Category | Examples |
|---|---|
| Style / formatting | "Use camelCase", "Add blank line", "Reorder imports" |
| Naming preferences | "Rename `x` to `count`", "Variable name too short" |
| Comment suggestions | "Add a comment explaining...", "Document this function" |
| Subjective opinions | "I would prefer...", "Consider using..." |
| Already-linted issues | Anything a linter/formatter catches automatically |

**Hard requirements** — every accepted finding MUST include:

1. **Location** — file path + line range
2. **Category** — bug / security / performance / architecture
3. **Severity** — critical (1) / major (2–3) / minor (4)
4. **Evidence** — what's wrong and why
5. **Remediation** — specific fix, not vague advice

## 6. Structured Output Format

```json
{
  "review_id": "omega-review-2025-0712-001",
  "artifact": "src/audio/engine.cpp",
  "verdict": "REQUEST_CHANGES",
  "aggregate_score": 2.8,
  "findings": [
    {
      "id": "F1",
      "category": "bug",
      "severity": 1,
      "location": {"file": "src/audio/engine.cpp", "lines": [142, 148]},
      "title": "Buffer overflow in processBlock",
      "evidence": "numSamples is not bounds-checked against buffer.getNumSamples()",
      "remediation": "Add `jassert(numSamples <= buffer.getNumSamples())` at line 143",
      "quorum": {"votes": 3, "approvals": 3, "threshold": 2}
    }
  ],
  "council": {
    "size": 3,
    "stage1_findings": 7,
    "post_quorum_findings": 2,
    "noise_filtered": 5
  }
}
```

## 7. Consensus Failure Escalation

When the council cannot reach quorum on a finding:

```python
def handle_consensus_failure(finding, votes):
    if not meets_quorum(finding, votes):
        if finding.severity == 1:
            escalate_to_human(finding, reason="Critical finding without quorum")
        elif any(v.approve for v in votes):
            tag_for_next_review(finding, reason="Minority concern")
        else:
            discard(finding, reason="No reviewer approved")
```

Escalation path: **Council → Senior agent (opus) → Human reviewer**.

## 8. Pattern Detection Across Review History

Track findings over time to detect systemic issues:

```sql
CREATE TABLE IF NOT EXISTS review_findings (
  id TEXT PRIMARY KEY,
  review_id TEXT NOT NULL,
  file_path TEXT NOT NULL,
  category TEXT NOT NULL,
  severity INTEGER NOT NULL,
  title TEXT NOT NULL,
  remediation TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Find recurring issues in the same file
SELECT file_path, category, COUNT(*) as occurrences
FROM review_findings
GROUP BY file_path, category
HAVING COUNT(*) >= 3
ORDER BY occurrences DESC;

-- Find hot files (most findings per file)
SELECT file_path, COUNT(*) as total_findings,
  SUM(CASE WHEN severity = 1 THEN 1 ELSE 0 END) as critical
FROM review_findings
GROUP BY file_path
ORDER BY critical DESC, total_findings DESC
LIMIT 10;
```

## 9. Git Diff Integration

Extract review targets directly from git:

```bash
# Get changed files for review scope
git diff --name-only HEAD~1..HEAD

# Get unified diff for the council
git diff --unified=5 HEAD~1..HEAD -- '*.cpp' '*.h'

# Get diff stats for triage
git diff --stat HEAD~1..HEAD
```

```python
def review_pr(base_branch="main"):
    diff = run(f"git diff --unified=5 {base_branch}..HEAD")
    changed_files = run(f"git diff --name-only {base_branch}..HEAD").splitlines()
    # Partition by domain for specialist review
    cpp_files = [f for f in changed_files if f.endswith(('.cpp', '.h'))]
    py_files = [f for f in changed_files if f.endswith('.py')]
    if cpp_files: code_review(diff, cpp_files)
    if py_files:  code_review(diff, py_files)
```

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Reviewers see each other's findings in Stage 1 | Enforce anonymous isolation per agent |
| Commenting on style or formatting | Auto-reject filter (§5) + explicit instructions |
| Findings without remediation | Reject any finding missing a concrete fix |
| Single-reviewer decisions on architecture | Require unanimous quorum for structural changes (§3) |
| Ignoring review history patterns | Track in SQL, alert on recurring issues (§8) |
| Vague findings ("this looks wrong") | Require location + evidence + remediation (§5) |
| Reviewing generated/vendored code | Exclude from diff scope via .gitattributes |
| No escalation path for deadlocked council | Escalate critical → senior agent → human (§7) |

## Checklist

- [ ] **Council size set** — minimum 3 reviewers for meaningful quorum
- [ ] **Anonymous isolation** — no cross-talk between reviewers in Stage 1
- [ ] **Scoring rubric applied** — correctness, security, performance, maintainability
- [ ] **Quorum thresholds configured** — per decision type (§3)
- [ ] **Signal-to-noise filter active** — style/formatting auto-rejected (§5)
- [ ] **Remediation required** — every finding includes a specific fix
- [ ] **Escalation path defined** — council → senior agent → human
- [ ] **Review history tracked** — findings stored in SQL for pattern detection
- [ ] **Git diff scoped** — only review changed files, exclude generated code
- [ ] **Structured output** — JSON format with verdict, scores, and quorum metadata
