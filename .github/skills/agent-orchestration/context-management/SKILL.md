---
name: Context Management for Agents
description: Patterns for persisting state, querying history, storing facts, and maintaining fresh context across AI agent sessions.
---

# Context Management for Agents

Effective context management separates agents that accumulate institutional knowledge from those that start blind every session. This skill covers the full lifecycle: session state, memory databases, checkpoints, environment profiles, cross-session learning, fact storage, and staleness prevention.

## Session State Persistence

Use the per-session SQLite database for tracking work. The `todos` table is pre-built; create custom tables for domain-specific needs.

```sql
INSERT INTO todos (id, title, description, status) VALUES
  ('auth-jwt', 'Implement JWT auth', 'Add login/logout/refresh in src/auth/.', 'pending');
INSERT INTO todo_deps (todo_id, depends_on) VALUES ('auth-tests', 'auth-jwt');

-- Find ready tasks (all dependencies done)
SELECT t.id, t.title FROM todos t WHERE t.status = 'pending'
  AND NOT EXISTS (SELECT 1 FROM todo_deps td JOIN todos dep ON td.depends_on = dep.id
    WHERE td.todo_id = t.id AND dep.status != 'done');

-- Custom domain tracking
CREATE TABLE files_to_migrate (path TEXT PRIMARY KEY, status TEXT DEFAULT 'pending');
```

## Memory Databases (SQLite FTS5)

FTS5 indexes enable fast keyword search. Use OR expansion to act as your own embedder — cast a wide net, then rank results.

```sql
CREATE VIRTUAL TABLE knowledge_index USING fts5(topic, content, source);
INSERT INTO knowledge_index VALUES ('build', 'Run npm run build for production', 'session-42');
SELECT content, rank FROM knowledge_index
WHERE knowledge_index MATCH 'build OR compile OR bundle OR webpack' ORDER BY rank LIMIT 5;
```

## Checkpoint/Restore Patterns

Save progress at milestones so work survives interruption. Use workspace artifacts for prose, session database for structured state.

```sql
-- Resume from last checkpoint in session_store (read-only, all past sessions)
SELECT c.title, c.overview, c.work_done FROM checkpoints c
JOIN sessions s ON c.session_id = s.id WHERE s.repository = 'SpiralCloudOmega/DevTeam6'
ORDER BY s.updated_at DESC, c.checkpoint_number DESC LIMIT 1;
```

Keep `plan.md` in the session workspace (not the repo) for current-phase notes, blocked items, and next steps.

## Environment Profiles

Store machine config in JSON, loaded at session start for platform-specific decisions.

```json
{
  "hardware": { "cpu": "AMD Ryzen 9 5950X", "ram_gb": 64, "gpu": "RTX 3090 24GB" },
  "runtimes": { "python": "3.13", "node": "22", "rust": "1.93" },
  "paths": { "projects": "~/projects", "memory_db": ".github/memory/spiralcloud-memory.db" },
  "services": { "ollama": ":11434", "postgres": ":5432" }
}
```

## Cross-Session Learning

Query `session_store` before starting work — find how a problem was solved before and reuse proven patterns.

```sql
-- Sessions where auth was discussed
SELECT s.id, s.branch, s.summary FROM sessions s
JOIN turns t ON t.session_id = s.id AND t.turn_index = 0
WHERE t.user_message LIKE '%auth%' OR t.user_message LIKE '%login%'
ORDER BY s.updated_at DESC LIMIT 5;

-- Full-text search across all history with query expansion
SELECT content, session_id, source_type FROM search_index
WHERE search_index MATCH 'migration OR schema OR prisma OR drizzle' ORDER BY rank LIMIT 10;

-- Files modified across sessions
SELECT sf.file_path, COUNT(DISTINCT sf.session_id) as sessions
FROM session_files sf JOIN sessions s ON sf.session_id = s.id
WHERE s.repository = 'SpiralCloudOmega/DevTeam6' AND sf.tool_name = 'edit'
GROUP BY sf.file_path ORDER BY sessions DESC LIMIT 10;
```

## Fact Storage (store_memory Pattern)

Good facts are **under 200 characters, actionable, durable, and not obvious from a small code sample**.

```yaml
# Example store_memory invocations
- subject: "build commands"
  fact: "Run 'npm run build && npm run test' to build and verify."
  category: "bootstrap_and_build"
  citations: "package.json:5-8"
  reason: "Future sessions need the build workflow without re-discovering it."

- subject: "error handling"
  fact: "Use ErrKind wrapper for every public API error."
  category: "general"
  citations: "src/errors.ts:12, src/api/users.ts:45"
  reason: "Convention not obvious from a single file; agents reviewing any API file need this."
```

**Categories:** `bootstrap_and_build` (build/test/deploy), `user_preferences` (style, libs), `general` (cross-file conventions), `file_specific` (single-file context).

## Workspace Artifacts

| Artifact | Location | Commit? |
|---|---|---|
| `plan.md`, research notes | Session workspace | No |
| `SKILL.md` (durable skill) | `.github/skills/` | Yes |
| `environment-profile.json` | `.github/memory/` | Yes |

## Avoiding Context Rot

Stale context is worse than none — it causes confident wrong decisions.

- **Detect staleness:** Verify stored paths exist and build commands still work.
- **Refresh profiles:** Re-run discovery periodically; diff against stored environment profile.
- **Prune facts:** Remove facts referencing deleted files or changed conventions.
- **Limit history:** Always use `ORDER BY updated_at DESC LIMIT` — never load all sessions.

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Starting from scratch every session | Query `session_store` and load environment profile first |
| Not searching history before work | Always search for related past sessions before coding |
| Storing trivial or stale facts | Only store actionable, durable, non-obvious facts |
| Keeping `plan.md` in the repo | Use session workspace for ephemeral planning artifacts |
| Loading entire session history | Use targeted FTS5 queries with LIMIT clauses |

## Checklist

- [ ] Searched `session_store` for related past sessions, branches, and file edits
- [ ] Loaded environment profile (hardware, runtimes, paths, services)
- [ ] Reviewed last checkpoint for this repo to resume prior progress
- [ ] Stored new conventions or build commands via `store_memory`
- [ ] Workspace artifacts (`plan.md`, notes) kept in session space, not the repo
- [ ] Verified stored paths exist and stored facts reference current code
