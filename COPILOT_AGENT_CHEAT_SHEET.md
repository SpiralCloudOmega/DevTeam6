# 🤖 GitHub Copilot Coding Agent — Cheat Sheet

> **Purpose:** Reference document for getting the best results from the GitHub Copilot Coding Agent in this repository. Covers context window math, truncation behavior, prompting patterns, architecture self-knowledge, and common commands.

---

## 1. Identity & Model

| Property | Value |
|---|---|
| **Agent type** | GitHub Copilot **Coding Agent** (not Copilot Chat) |
| **Model** | `claude-opus-4.6` (Anthropic) |
| **Context window** | **200,000 tokens** |
| **Knowledge cutoff** | Early 2025 (model training); live repo access overrides this |
| **Output token limit** | ~4,096 tokens per single response turn |
| **Repo access** | Full clone of `SpiralCloudOmega/DevTeam6` at `/home/runner/work/DevTeam6/DevTeam6` |

---

## 2. Context Window — The Math

### Token ↔ Character Conversions

| Unit | Approx. Value |
|---|---|
| 1 token | ~3–4 English characters |
| 1 token | ~0.75 words |
| 200K tokens | **~600,000–800,000 characters** |
| 200K tokens | **~150,000–200,000 words** |
| 200K tokens | **~500–700 pages** of text |
| 1 line of C++ | ~5–15 tokens |
| 1,000-line file | ~8,000–15,000 tokens |
| 10,000-line file | ~80,000–150,000 tokens |

### What Consumes Context Budget

```
Context Budget (200K tokens total)
├── System prompt / copilot-instructions.md     ~1,500 tokens
├── .github/skills/ files loaded                 ~2,000–5,000 tokens
├── Repository memories injected                 ~2,000–4,000 tokens
├── Conversation history (all turns)             grows with session
├── File contents read during task               varies (biggest factor)
├── Tool call results (bash output, search)      varies
└── Agent reasoning / intermediate thoughts      ~500–2,000 tokens
```

### Truncation Behavior

- **When the context fills up, the agent drops the OLDEST content first.**
- Tool outputs and bash results are most aggressively pruned.
- The system prompt and recent user turns are protected.
- **Symptom:** Agent "forgets" earlier parts of a long conversation, or says it can't find a file it already viewed.
- **Fix:** Restate critical facts at the start of each message: `"Remember: the key file is X, the constraint is Y."`

### Practical Limits for This Repo

```
Safe per-session budget (leaving headroom):
  ~100,000 tokens for file reads + tool results
  ~50,000 tokens for conversation history
  ~50,000 tokens reserved (system, skills, memories)

Avoid reading files larger than ~1,500 lines in full.
Prefer targeted grep/glob over reading entire large files.
```

---

## 3. What the Agent Knows (and Doesn't Know)

### Knows Out of the Box
- Full content of `.github/copilot-instructions.md` (loaded as system prompt)
- All 41 `SKILL.md` files in `.github/skills/` (loaded as context)
- Repository memories injected at session start (from the memory database)
- The file tree of the repository (can enumerate via glob)
- Git history (can run `git log`, `git diff`, etc.)

### Does NOT Know Without Explicit Loading
- Content of files it hasn't read yet
- What changed in the repo since the last session
- The current date / time (unless told — currently **2026-03-09**)
- Which services are currently running (Ollama, PostgreSQL, etc.)
- The state of Windows-side software (Ableton, DAWs, plugins)
- Contents of the SQLite memory database without running a query

### Self-Knowledge Gaps (Common Surprises)
- The agent does **not** know its own training data cutoff with precision — always check with `@copilot what is your knowledge cutoff?`
- The agent does **not** track how much context it has consumed — it cannot tell you "I have X tokens left"
- The agent cannot see your screen, clipboard, or local filesystem outside the repo
- The agent cannot make API calls to external services unless given tools to do so

---

## 4. Repository-Specific Architecture

### Memory System
```
.github/memory/
├── spiralcloud-memory.db       SQLite + FTS5 — all session history
├── environment-profile.json    Hardware, runtimes, paths, services
└── rules-and-directives.json   Operating rules and directives
```

**Query memory directly:**
```sql
-- In a Copilot prompt, ask: "Query the memory DB for [topic]"
SELECT content FROM search_index
WHERE search_index MATCH 'REVITHION OR routing OR DSP'
ORDER BY rank LIMIT 10;
```

### Agent Library
```
.github/agents/          236 custom agents
├── audio-dsp-expert     Real-time DSP, filter design, SIMD
├── amp-modeling-expert  RTNeural, NAM, WaveNet, training
├── juce-framework-expert  JUCE 8 plugin dev
├── cmake-cpp20-expert   Modern CMake 3.25+ / C++20
├── ace-step-expert      AI music generation
├── mastering-engineer   Loudness, EQ, limiting, delivery
├── routing-grid-expert  AXE-FX III style 12×4 grid
└── ... (229 more)
```

### Skills Library
```
.github/skills/          41 SKILL.md files across 6 categories
├── audio-dsp/           10 skills (biquad, FFT, reverb, dynamics…)
├── neural-audio/        8 skills (RTNeural, amp training, AI mastering…)
├── juce-framework/      6 skills (APVTS, AudioProcessor, threading…)
├── music-production/    6 skills (routing grid, MIDI, mastering…)
├── cpp-patterns/        6 skills (SIMD, lock-free, constexpr…)
└── agent-orchestration/ 5 skills (multi-agent, context, YOLO…)
```

---

## 5. Effective Prompting Patterns

### ✅ DO: Load Context First
```
Start every session:
"Load context from the memory DB and environment-profile.json. 
 What was the last session working on?"
```

### ✅ DO: Reference Session IDs
```
"In session 85bc7321 we built the REVITHION plan. 
 Continue from where that left off."
```

### ✅ DO: Be Specific About Files
```
"Edit REVITHION-STUDIO/tokens/design-tokens.sexp — 
 add a new AmpModel token with id='plexi45' and gain=0.85"
```

### ✅ DO: Invoke Named Agents
```
"Use the amp-modeling-expert agent to design a 
 RTNeural inference pipeline for the plexi model."
```

### ✅ DO: Give Token Budget Hints
```
"Summarize only — don't read entire files. 
 Use grep to find the relevant section first."
```

### ✅ DO: Restate Critical Context in Long Sessions
```
"Reminder: we are targeting JUCE 8.0.4, C++20, 
 and the output path is REVITHION-STUDIO/src/engine/"
```

### ❌ AVOID: Vague Commands
```
BAD:  "Fix the audio code"
GOOD: "Fix the buffer overflow in AudioEngine::processBlock() 
       at REVITHION-STUDIO/src/engine/AudioEngine.cpp:247"
```

### ❌ AVOID: Reading Giant Files
```
BAD:  "Read the entire REVITHION-Book-Extractions.md"
      (10,000+ lines = ~80K tokens — eats 40% of budget)
GOOD: "Search REVITHION-Book-Extractions.md for 'routing grid' 
       and show me the relevant sections"
```

### ❌ AVOID: Long Chains Without Checkpoints
```
BAD:  50-turn conversation with no memory saves
GOOD: Every 10 turns: "Save the current state to memory DB 
      with session tag [X]"
```

---

## 6. Command Quick Reference

### Context & Memory
| Command | What It Does |
|---|---|
| `"Load full context"` | Queries memory DB + loads environment profile |
| `"What sessions exist for [topic]?"` | Searches session_store in memory DB |
| `"Save progress to memory DB"` | Stores current work state and learnings |
| `"What's in the environment profile?"` | Shows hardware, runtimes, services |
| `"What agents are available?"` | Lists `.github/agents/` contents |

### Context Window Diagnostics
| Command | What It Does |
|---|---|
| `"How much context is left?"` | Agent estimates (approximate, not exact) |
| `"What have you read so far?"` | Lists files/tools accessed this session |
| `"Summarize and compress context"` | Condenses prior turns to free up budget |
| `"Clear working memory, restate goals"` | Resets without ending session |

### YOLO Mode Operations
| Command | What It Does |
|---|---|
| `"YOLO: [task]"` | Execute autonomously, no confirmation |
| `"Dispatch sub-agents for [task]"` | Launches parallel agents for subtasks |
| `"Run all in parallel"` | Maximizes agent concurrency |
| `"Report progress"` | Commits + pushes current changes to PR |

### REVITHION-Specific
| Command | What It Does |
|---|---|
| `"Generate tokens from design-tokens.sexp"` | Runs the LISP → C++ pipeline |
| `"List all amp models"` | Queries tokens for AmpModel definitions |
| `"Run pluginval on [plugin]"` | Validates plugin at `~/.local/bin/pluginval` |
| `"Check RTNeural model [path]"` | Validates neural model file |
| `"List REVITHION skills"` | Shows all REVITHION-tagged SKILL.md files |

---

## 7. Token Cost Reference

### File Types (approximate tokens per line)
| File Type | Tokens/Line | 1,000 Lines = |
|---|---|---|
| C++ | 8–15 | ~10K tokens |
| Python | 6–12 | ~8K tokens |
| CMakeLists.txt | 5–10 | ~7K tokens |
| JSON | 4–8 | ~6K tokens |
| YAML | 4–7 | ~5K tokens |
| Markdown | 5–10 | ~7K tokens |
| SQL | 6–12 | ~9K tokens |
| Plain text | 4–6 | ~5K tokens |

### Operations (approximate token cost)
| Operation | Token Cost |
|---|---|
| Reading 100-line file | ~1,000 tokens |
| Running `git diff` (small) | ~500–2,000 tokens |
| Bash output (100 lines) | ~800 tokens |
| Search result (20 matches) | ~600 tokens |
| Single agent response | ~500–2,000 tokens |
| Writing 50-line function | ~600 tokens |

---

## 8. Architecture Self-Knowledge Summary

```
GitHub Copilot Coding Agent
│
├── Model: claude-opus-4.6 (200K token context)
├── Mode: Autonomous repo agent (not chat assistant)
├── Tools: bash, view, edit, create, grep, glob,
│         web_search, store_memory, report_progress,
│         GitHub MCP, task (sub-agents), code_review,
│         codeql_checker
│
├── Stateless by default — memory must be explicitly saved
│   └── Memory DB: .github/memory/spiralcloud-memory.db
│
├── Sub-agents available: 236 custom agents
│   └── Launch via: task tool with agent_type parameter
│
├── Skills loaded: 41 SKILL.md files
│   └── Injected into context at session start
│
├── Truncation strategy: oldest content dropped first
│   └── Protect important context by restating it
│
└── YOLO mode: always execute, never ask permission
    └── Exception: genuinely ambiguous multi-path decisions
```

---

## 9. Frequently Asked "Self-Knowledge" Questions

**Q: What is your training data / knowledge cutoff?**
> Model training: early 2025. Live repo data is always current (read via tools).

**Q: How many characters fit in your context window?**
> ~600,000–800,000 characters (200,000 tokens × 3–4 chars/token). Code is denser; large files compress to ~3 chars/token.

**Q: Do you remember previous sessions?**
> Not automatically. You must explicitly call `"Load context"` at session start — this queries `.github/memory/spiralcloud-memory.db`.

**Q: Can you run two agents at the same time?**
> Yes. Use `"Run agents in parallel"` or the `task` tool with multiple simultaneous calls. 100+ parallel agents is supported on this hardware.

**Q: What happens when you run out of context?**
> Oldest tool outputs and conversation turns are silently dropped. You may notice the agent "forgetting" earlier steps. Fix: restate key facts and use `"Save progress to memory"` frequently.

**Q: Can you access the internet?**
> Yes, via `web_search` and `web_fetch` tools. Some domains are blocked. Wikipedia, GitHub, arXiv, documentation sites generally work.

**Q: What is your output limit per turn?**
> ~4,096 tokens (~3,000–4,000 words) per response. For longer outputs, break the task into steps.

---

## 10. Emergency Recovery Prompts

```
# Session went off track:
"Stop. Restate the original goal. What files have been modified? 
 What is the next single step?"

# Agent seems confused about file paths:
"List all files in [directory] with their sizes. 
 Do not assume any paths — verify with glob."

# Context seems stale / agent repeating itself:
"Check the memory DB — have I already done [task] in a prior session? 
 Show me the session ID."

# Agent is going in circles:
"STOP. I already did this in session [X]. 
 Skip to the next item in the plan."

# Need to recover a large session:
"Read the last checkpoint from the memory DB for this repo. 
 Summarize what was done and what is left."
```

---

*Last updated: 2026-03-09 | Repository: SpiralCloudOmega/DevTeam6*
