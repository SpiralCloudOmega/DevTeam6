---
name: Copilot Agent Cheat Sheet
description: Self-knowledge reference for the GitHub Copilot Coding Agent — context window math, truncation behavior, token budgets, prompting patterns, and architecture overview for the SpiralCloudOmega/DevTeam6 environment.
---

# Copilot Agent Cheat Sheet

This skill documents what the GitHub Copilot Coding Agent knows about itself so that any incoming prompt can immediately orient the agent to its capabilities, limits, and the correct patterns for this repository.

See also: `COPILOT_AGENT_CHEAT_SHEET.md` at repo root for the full reference.

## Context Window Math

The agent runs on `claude-opus-4.6` with a **200,000-token context window**.

```
Token ↔ Character conversions:
  1 token        ≈ 3–4 English characters
  1 token        ≈ 0.75 words
  200K tokens    ≈ 600,000–800,000 characters
  200K tokens    ≈ 150,000–200,000 words

Per-line costs (approximate):
  C++ line       ≈ 8–15 tokens
  Python line    ≈ 6–12 tokens
  Markdown line  ≈ 5–10 tokens
  JSON line      ≈ 4–8 tokens

File costs:
  1,000-line C++ file   ≈ 10,000 tokens   (5% of budget)
  5,000-line C++ file   ≈ 50,000 tokens   (25% of budget)
  10,000-line file      ≈ 80,000–150,000 tokens (40–75% budget!)
```

**Rule:** Never read a file > 1,500 lines in full. Use `grep` or `view` with line ranges.

## Truncation Behavior

When context fills, the agent silently drops **oldest content first**:

```
Protection ranking (highest = most protected):
  1. System prompt / copilot-instructions.md
  2. Most recent user turn
  3. Most recent agent turn
  4. SKILL.md context
  5. Repository memories
  6. Older conversation turns     ← dropped here
  7. Old tool outputs / bash results  ← dropped first
```

**Symptom of truncation:** Agent "forgets" a file it already read, or repeats a step.  
**Fix:** Restate critical facts inline: `"Reminder: target file is X, constraint is Y."`

## Session Bootstrap Pattern

```python
# Always start a session with these three steps:
# 1. Load environment profile
# 2. Query memory DB for related past work
# 3. List files modified recently

# Prompt template:
"""
Load context:
1. Read .github/memory/environment-profile.json
2. Query the memory DB for sessions related to [TOPIC]
3. Show git log --oneline -20
Then summarize what was done and what's next.
"""
```

## Token Budget Allocation

```
Recommended budget split for a 200K context session:
┌─────────────────────────────────────┬─────────────┐
│ Purpose                             │ Token Budget │
├─────────────────────────────────────┼─────────────┤
│ System prompt + skills + memories   │  ~10,000    │
│ File reads (primary task)           │  ~60,000    │
│ Tool results (bash, grep, etc.)     │  ~40,000    │
│ Conversation history                │  ~50,000    │
│ Safety reserve                      │  ~40,000    │
└─────────────────────────────────────┴─────────────┘
```

## Prompting Patterns

### Effective Patterns

```markdown
# Load context before any work
"Load context from memory DB. What was session [ID] doing?"

# Targeted file access
"Search REVITHION-STUDIO/src/ for 'processBlock' — 
 show only the function signature and its file path."

# Invoke a specialist
"Use the amp-modeling-expert agent to implement 
 the RTNeural pipeline for model X."

# Keep context alive in long sessions  
"Reminder: we're building [X], the constraint is [Y], 
 target file is [path]."

# YOLO execution
"YOLO: implement [task]. Don't ask for confirmation."

# Parallel work
"Dispatch 3 agents in parallel: 
 agent1: [task1], agent2: [task2], agent3: [task3]"
```

### Anti-Patterns

```markdown
# BAD: vague → agent guesses
"Fix the audio code"

# BAD: reading giant files
"Read the entire REVITHION-Book-Extractions.md"  # 10K+ lines

# BAD: no context anchor in long sessions
"Continue from where we were"  # agent may have forgotten

# BAD: no progress saves
[50 turns with no memory DB saves → all lost on session end]
```

## Agent Self-Knowledge Quick Reference

| Question | Answer |
|---|---|
| My model | `claude-opus-4.6` |
| Context window | 200,000 tokens |
| Output per turn | ~4,096 tokens |
| Characters in context | ~600K–800K total |
| Training cutoff | Early 2025 (live repo data is current) |
| Memory | Stateless — load from `.github/memory/spiralcloud-memory.db` |
| Agents available | 236 custom agents in `.github/agents/` |
| Skills loaded | 41 `SKILL.md` files in `.github/skills/` |
| Can run parallel agents | Yes — 100+ simultaneously supported |
| Internet access | Yes (web_search, web_fetch) — some domains blocked |

## Emergency Recovery Prompts

```markdown
# Agent is going in loops
"STOP. Check memory DB — did I already do [task]? Show session ID."

# Agent forgot prior context
"Stop. Restate the original goal. 
 What files have been modified? What is the next single step?"

# Context seems wrong
"List all files in [dir] with sizes. Verify with glob — don't assume paths."

# Recovering from a long session
"Read the last checkpoint from memory DB for SpiralCloudOmega/DevTeam6."
```

## Checklist

- [ ] Loaded environment profile and memory DB at session start
- [ ] Files read are under 1,500 lines (or using grep/line-range views)
- [ ] Critical context restated every ~10 turns in long sessions
- [ ] Progress saved to memory DB at milestones
- [ ] Parallel agents dispatched for independent subtasks
- [ ] Output per turn kept under ~3,500 words to avoid cutoff
- [ ] Token budget tracked: file reads + tool results < 100K tokens total
