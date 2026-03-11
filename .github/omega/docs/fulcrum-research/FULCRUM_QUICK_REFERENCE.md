# FULCRUM - QUICK REFERENCE FOR OMEGA DESIGN

## What is Fulcrum?
AI-powered command center for orchestrating multiple agents (Claude Code, OpenCode) across isolated tasks. Think of it as a "fulcrum" (lever) that amplifies a single person's productivity by multiplying agent coordination.

## Core Stack
- **Frontend**: React 19 + TanStack Router + Drizzle ORM + xterm.js
- **Backend**: Hono.js on Bun + SQLite
- **Real-time**: WebSocket for terminal I/O
- **Agents**: Claude Code SDK + OpenCode SDK
- **Protocol**: Model Context Protocol (MCP) with 100+ tools
- **Config**: fnox (encrypted configuration)

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────┐
│                   FULCRUM DASHBOARD                 │
│  (React 19 + TanStack Router + shadcn/ui)          │
│                                                     │
│  ┌────────────────────────────────────────────┐    │
│  │  KANBAN BOARD (Tasks → 5 Status Columns)   │    │
│  │  TO_DO | IN_PROGRESS | IN_REVIEW | DONE   │    │
│  │  CANCELED | PINNED                        │    │
│  └────────────────────────────────────────────┘    │
│                                                     │
│  ┌────────────────────────────────────────────┐    │
│  │  TERMINALS (xterm.js + dtach sessions)    │    │
│  │  ┌────────┬────────┬────────────────────┐ │    │
│  │  │Tab 1  │ Tab 2  │ Task Worktrees...  │ │    │
│  │  └────────┴────────┴────────────────────┘ │    │
│  └────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────┘
           ↕ WebSocket /ws/terminal
           ↕ HTTP REST API /api/*
           ↕ MCP HTTP POST /mcp
┌─────────────────────────────────────────────────────┐
│         BACKEND (Hono on Bun + SQLite)             │
│                                                     │
│  ┌──────────────────────────────────────────────┐  │
│  │  TASK MANAGEMENT                            │  │
│  │  - CRUD (tasks table)                        │  │
│  │  - Status transitions (TO_DO→DONE)          │  │
│  │  - Recurrence (daily/weekly/monthly/etc)    │  │
│  │  - Dependencies (taskRelationships)         │  │
│  │  - Attachments & links                      │  │
│  └──────────────────────────────────────────────┘  │
│                                                     │
│  ┌──────────────────────────────────────────────┐  │
│  │  AGENT SERVICES                             │  │
│  │  ┌─────────────────────────────────────┐   │  │
│  │  │ Claude Code Integration             │   │  │
│  │  │ - Plugin hooks (Stop, Submit)       │   │  │
│  │  │ - Automatic status sync             │   │  │
│  │  └─────────────────────────────────────┘   │  │
│  │  ┌─────────────────────────────────────┐   │  │
│  │  │ OpenCode Integration                │   │  │
│  │  │ - SDK client (auto-start server)   │   │  │
│  │  │ - Event tracking (idle, busy, etc) │   │  │
│  │  └─────────────────────────────────────┘   │  │
│  │  ┌─────────────────────────────────────┐   │  │
│  │  │ [OMEGA INTEGRATION HERE]            │   │  │
│  │  │ - Provider interface               │   │  │
│  │  │ - Status tracker                   │   │  │
│  │  │ - Hook system                      │   │  │
│  │  └─────────────────────────────────────┘   │  │
│  └──────────────────────────────────────────────┘  │
│                                                     │
│  ┌──────────────────────────────────────────────┐  │
│  │  TERMINAL MANAGEMENT (dtach + PTY)          │  │
│  │  - Per-task isolation (worktree/scratch)   │  │
│  │  - Persistent sessions (survive restart)   │  │
│  │  - Buffer management & TTY output          │  │
│  └──────────────────────────────────────────────┘  │
│                                                     │
│  ┌──────────────────────────────────────────────┐  │
│  │  MCP TOOLS (100+ capabilities)              │  │
│  │  - Task CRUD + dependencies                 │  │
│  │  - Project management                       │  │
│  │  - Filesystem operations                    │  │
│  │  - Notifications (Slack/Discord/etc)       │  │
│  │  - Memory (knowledge store + FTS5)         │  │
│  │  - Search (unified across all entities)    │  │
│  │  - Calendar (CalDAV + Google)              │  │
│  │  - Shell execution                         │  │
│  │  - And 80+ more...                         │  │
│  └──────────────────────────────────────────────┘  │
│                                                     │
│  ┌──────────────────────────────────────────────┐  │
│  │  SUPPORT SERVICES                           │  │
│  │  - Messaging (WhatsApp, Discord, Telegram) │  │
│  │  - Notifications (multi-channel)            │  │
│  │  - Memory (ephemeral + master file)        │  │
│  │  - Search (FTS5 full-text)                 │  │
│  │  - Calendar (multi-account sync)           │  │
│  │  - GitHub monitoring (PR auto-close)       │  │
│  │  - App deployment (Docker Compose)         │  │
│  │  - System monitoring (CPU/mem/disk)        │  │
│  └──────────────────────────────────────────────┘  │
│                                                     │
│  ┌──────────────────────────────────────────────┐  │
│  │  DATABASE (SQLite)                          │  │
│  │  - tasks (5 statuses, recurrence)          │  │
│  │  - taskRelationships (dependencies)        │  │
│  │  - terminals (dtach sessions)              │  │
│  │  - chatSessions + chatMessages             │  │
│  │  - projects + repositories                 │  │
│  │  - memories + artifacts                    │  │
│  │  - calendars + messaging_connections       │  │
│  │  - And 10+ more...                         │  │
│  └──────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘
```

---

## Key Features Mapped

| Feature | Implementation | Extension Point |
|---------|-----------------|-----------------|
| **Kanban Board** | 5 status columns, drag-and-drop, priorities, pins, recurrence | Frontend only - no changes needed |
| **Task Types** | Worktree (git), Scratch (no-git), Manual (tracking) | Database schema - add new types in `type` field |
| **Agent Workspace** | Isolated git worktrees + scratch dirs + dtach sessions | Terminal manager - already generic |
| **Claude Integration** | Plugin hooks + status sync + MCP tools | Use as reference for Omega |
| **OpenCode Integration** | SDK client + event tracking + status sync | Use as reference for Omega |
| **Status Tracking** | Task status workflow + agent status updates | Create agent provider interface |
| **Task Dependencies** | Directed graph visualization + blocking logic | Already in schema - visualization works |
| **MCP Tools** | 100+ tools across 20 categories | All agent-agnostic - Omega can use same tools |
| **Messaging** | WhatsApp, Discord, Telegram, Slack, Email | All agent-agnostic - Omega can send messages |
| **Memory** | Master file (system prompt) + FTS5 search | All agent-agnostic - Omega can store/retrieve |

---

## Integration Pattern: How Claude Code Works (Reference for Omega)

### 1. Plugin Hook System (Claude Code)
```
Claude stops working (idle)
    ↓ Plugin Stop hook fires
    ↓ Runs: fulcrum current-task review
    ↓ CLI auto-detects task from worktree
    ↓ HTTP POST /api/tasks/{id}/status → IN_REVIEW
    ↓ Task moves to IN_REVIEW column
    ↓ Notification sent to user
```

### 2. MCP Tool Integration
```
Claude generates code + wants to update task
    ↓ Claude calls MCP tool (e.g., move_task)
    ↓ HTTP POST /mcp (MCP request)
    ↓ Server processes tool call
    ↓ Task updated in database
    ↓ WebSocket broadcasts to UI
    ↓ Kanban updates in real-time
```

### 3. Key Files for Reference
- **Plugin**: `plugins/fulcrum-opencode/index.ts` (event tracking)
- **Service**: `server/services/opencode-chat-service.ts` (SDK wrapper)
- **Routes**: `server/routes/assistant.ts` (provider-agnostic)
- **CLI**: `cli/src/mcp/tools/tasks.ts` (status update tool)

---

## Per-Task Workspace Structure

```
Fulcrum Server (~/.fulcrum/)
├── repos/
│   └── my-repo/
│       └── .git/worktrees/
│           ├── task-123-feature-branch/
│           │   ├── .git (worktree-specific)
│           │   ├── src/
│           │   ├── package.json
│           │   └── [agent runs here]
│           └── task-456-bugfix-branch/
│               └── [separate workspace]
├── scratch/
│   ├── task-789/
│   │   ├── analysis.md
│   │   ├── script.py
│   │   └── [non-git work]
│   └── task-999/
│       └── [another scratch]
├── uploads/
│   └── tasks/
│       ├── task-123/
│       │   ├── design.pdf
│       │   └── spec.md
│       └── task-456/
└── MEMORY.md (system prompt injection)
```

---

## Configuration Hierarchy (Settings Precedence)

```
Command-line argument
    ↓ (if provided)
Environment variable (e.g., FULCRUM_GIT_REPOS_DIR)
    ↓ (if set)
Task-level override (agent, agentOptions)
    ↓
Repository-level override (defaultAgent, claudeOptions)
    ↓
Project-level override (defaultAgent, claudeOptions)
    ↓
Global settings (.fnox.toml)
    ↓
Built-in default
```

---

## Database Schema Essentials

### Core Tables
- **tasks** — Task CRUD + status + agent + type + recurrence
- **terminals** — PTY sessions (tmuxSession, cwd, status)
- **terminalTabs** — Tab organization
- **chatSessions** — Agent conversations (provider='claude'|'opencode'|'omega')
- **chatMessages** — Conversation history
- **taskRelationships** — Dependency graph

### Agent Context
- **memories** — FTS5 searchable knowledge store
- **chatSessions.provider** — 'claude' | 'opencode' | 'omega' (extensible)
- **chatSessions.claudeSessionId** — Resume state for Claude
- (Can add `opencodeSessionId`, `omegaSessionId`, etc.)

---

## MCP Tools Organization

### Always-Loaded Core Tools (~8 tools)
- `search_tools` — Discover other tools
- `list_tasks`, `get_task`, `create_task`, `move_task`, etc.
- `execute_command`
- `send_notification`

### Deferred Tools (Discovered via search_tools)
- **Tasks**: Dependencies, links, attachments, tags
- **Projects**: CRUD, links, attachments, tags
- **Repositories**: Management, scanning
- **Apps**: Deployment, monitoring
- **Filesystem**: Browse, read, write
- **Memory**: Store, search, update master file
- **Calendar**: Events, sync, copy rules
- **Messaging**: Channel management
- **Search**: Unified FTS5 search
- **... 80+ more**

### Key Extension Point
All tools are already agent-agnostic:
- No hardcoded Claude references
- FulcrumClient makes HTTP calls back to `/api/*`
- Works for any agent that can call HTTP MCP endpoint

---

## Extending Fulcrum for Omega Agent

### Step 1: Create Agent Service
```typescript
// server/services/omega-chat-service.ts
import { createOmegaClient } from '@omega-ai/sdk'

export async function streamOmegaMessage(
  sessionId: string,
  message: string,
  context?: PageContext
): Promise<void> {
  const client = getOmegaClient()
  const response = await client.chat.create({
    message,
    tools: getMcpTools(),
    systemPrompt: buildSystemPrompt(context)
  })
  // Stream response via SSE
}
```

### Step 2: Extend Assistant Routes
```typescript
// server/routes/assistant.ts
if (session.provider === 'omega') {
  return streamOmegaMessage(sessionId, message, context)
}
```

### Step 3: Add to Settings
```typescript
// server/lib/settings/types.ts
agent: {
  defaultAgent: 'claude' | 'opencode' | 'omega'
  omegaOptions?: {
    model?: string
    apiKey?: string
    [other options]
  }
}
```

### Step 4: Create Plugin (Optional)
```typescript
// plugins/omega/index.ts
// Hooks for idle detection + status tracking
// Mirror Claude/OpenCode pattern
```

### Step 5: Register in DB
```typescript
// server/db/schema.ts
agent: text('agent').default('claude') // Add 'omega' to enum
provider: text('provider').default('claude') // Add 'omega' to enum
```

**Done!** MCP tools, messaging, memory, calendar, and all other features work automatically.

---

## Performance & Scaling Notes

- **Single-file SQLite**: Fine for solopreneur use (one person, one Fulcrum instance)
- **Deferred MCP tools**: Reduces token budget (only load on discovery)
- **Terminal dtach**: Persistent but complex (requires dtach binary)
- **WebSocket multiplexing**: Single connection handles multiple terminals
- **Async message processing**: Notifications/messaging don't block API responses
- **FTS5 search**: Good enough for ~10K tasks (full-text + recency weighting)

---

## Security Considerations

- **Secret Encryption**: age keys in `~/.fulcrum/age.txt` (never committed)
- **Terminal Isolation**: dtach provides session isolation (not process-level)
- **Path Validation**: Prevent directory traversal in file operations
- **MCP Tool Restrictions**: `/mcp/observer` endpoint uses limited tool set (cost control)
- **Input Sanitization**: MIME types, file sizes, SQL injection prevention

---

## Testing & Development

- **Test Files**: Live alongside source (`*.test.ts`)
- **Test Isolation**: Use `mise run test` (not `bun test` directly) - sets HOME + FULCRUM_DIR
- **Database Migrations**: Always use `mise run db:generate` (not db:push)
- **Type Checking**: `mise run build` (includes tsc type check)
- **Dev Server**: `mise run dev` (frontend + backend)

---

## Quick Commands Reference

```bash
# Start Fulcrum
mise run dev               # Dev servers
mise run up               # Production daemon
mise run down             # Stop daemon

# Database
mise run db:generate      # Create migration from schema
mise run db:migrate       # Apply migrations
mise run db:studio        # Drizzle Studio GUI

# Testing
mise run test             # Run tests (quiet mode)
mise run test -- -v       # Verbose output
mise run test:watch       # Watch mode

# CLI / MCP
fulcrum up               # Start server
fulcrum mcp              # Run MCP server (stdio)
fulcrum current-task info      # Get current task
fulcrum current-task review    # Mark IN_REVIEW
fulcrum api tasks list   # List tasks via API

# Agent Integration
fulcrum claude install   # Install Claude plugin
fulcrum opencode install # Install OpenCode plugin
```

---

## One-Liner Summary

**Fulcrum = Kanban + Terminal + MCP Server + Agent Orchestration for shipping software faster**

The architecture is **agent-agnostic** — you can plug in any AI agent (including Omega) that:
1. Can receive HTTP MCP calls
2. Can run in a shell/PTY
3. Can sync status back to Fulcrum

Everything else (terminal isolation, task management, dependencies, memory, notifications, calendar) works automatically.
