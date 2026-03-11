# FULCRUM PROJECT - COMPREHENSIVE TECHNICAL ANALYSIS

## 1. ARCHITECTURE OVERVIEW

### What is Fulcrum?
Fulcrum is an **AI-powered command center for shipping software**, designed for technical solopreneurs and CTOs. It's a "fulcrum" (small lever that moves mountains) for multiplying leverage over AI agents and development workflow.

**Core Mission**: Run multiple AI coding agents (Claude Code, OpenCode) in parallel across isolated tasks, manage them from a unified dashboard, and stay connected through messaging channels (WhatsApp, Slack, Discord, Telegram, Email).

### Tech Stack
- **Frontend**: React 19 + TanStack Router + TanStack Query + shadcn/ui v4
- **Backend**: Hono.js on Bun runtime + SQLite with Drizzle ORM
- **Real-time**: WebSocket (@hono/node-ws) for terminal I/O
- **Terminal**: bun-pty for PTY management + dtach for persistent sessions
- **AI Integration**: 
  - Claude Code via `@anthropic-ai/claude-agent-sdk` (v0.2.72)
  - OpenCode via `@opencode-ai/sdk` (v1.1.34)
- **Model Context Protocol**: `@modelcontextprotocol/sdk` (v1.11.0)
- **Messaging**: Baileys (WhatsApp), Discord.js, Telegram API, Slack Bolt
- **Calendar**: CalDAV (generic), Google Calendar (OAuth2)
- **Config**: fnox (encrypted configuration with age encryption)
- **Build**: Vite + TypeScript 5.9

---

## 2. KANBAN SYSTEM IMPLEMENTATION

### Database Schema (SQLite)

**Core Tables:**
- `tasks` — Task metadata with 3 types:
  - **worktree**: Git-isolated task with `worktreePath`
  - **scratch**: Directory-isolated task (non-git)
  - **manual**: No agent/directory (legacy/tracking-only)
  
- Task Fields: `id`, `title`, `description`, `status`, `position`, `branch`, `worktreePath`, `prUrl`, `agent`, `opencodeModel`, `pinned`, `projectId`, `dueDate`, `timeEstimate`, `priority`, `recurrenceRule`, `recurrenceSourceTaskId`, `type`, `notes`

- `taskRelationships` — M:M dependencies/relationships (replaces old `taskDependencies`)
  - Types: `depends_on`, `relates_to`, `subtask`

- `taskLinks` — Arbitrary URLs (PR, issue, docs, etc.)
- `taskAttachments` — File uploads (50MB max, MIME type validation)
- `tags` + `taskTags` — Reusable labels on tasks

**Status Flow:**
```
TO_DO → IN_PROGRESS → IN_REVIEW → DONE
                   ↘ CANCELED ↙
```

**Recurrence Logic** (`server/services/task-status.ts`):
- Rules: `daily`, `weekly`, `biweekly`, `monthly`, `quarterly`, `yearly`
- On DONE: Auto-creates next occurrence with new due date
- CANCELED: Does NOT spawn new tasks
- Lineage: `recurrenceSourceTaskId` chains to parent task

### Frontend Kanban Board

**Location**: `frontend/routes/tasks/` + `frontend/components/kanban/`

**Components**:
- `kanban-board.tsx` — Main drag-and-drop board (Atlaskit DnD)
- `kanban-column.tsx` — Per-status columns
- `task-card.tsx` — Individual task display with status, priority, due date, pinned state
- `create-task-modal.tsx` — Task creation (57KB - very comprehensive)
- `bulk-actions-toolbar.tsx` — Multi-select operations

**Features**:
- Drag-and-drop between status columns
- Pin tasks to top of columns
- Inline task creation
- Task filtering by project, tags, date range
- Calendar integration with due dates (color-coded: red=overdue, orange=today, yellow=tomorrow)
- Priority levels (high/medium/low) with visual distinction
- Time estimates (hours)
- Quick actions: mark IN_PROGRESS, IN_REVIEW, DONE, CANCELED
- Full-text search across task content
- Dependency visualization (graph view with `@dagrejs/dagre`)

**Board Filtering**:
- By project (or "Inbox" for orphans without projectId)
- By tags (M:M relationship)
- By status
- By date range
- Search box

---

## 3. AGENT WORKSPACES - PER-AGENT ISOLATION

### Three Workspace Types

#### A. Git Worktree Tasks
```
repositories/
└── repo-path/
    └── .git/worktrees/
        ├── task-id-branch-1/
        └── task-id-branch-2/
```
- Each task gets **isolated git worktree** (separate branch, independent code state)
- Spawns PTY with agent shell in `worktreePath`
- Survives server restart (dtach session backing)
- PR tracking with `prUrl` field
- Agent-specific config per task/repo

#### B. Scratch Directory Tasks
```
~/.fulcrum/scratch/
└── task-id/
    └── (free-form file structure, no git)
```
- Non-git isolated directory for scripts, experiments, data processing
- Same terminal isolation as worktrees
- File operations confined to task directory

#### C. Manual Tasks
- No directory, no agent
- Pure task tracking outside Fulcrum's terminal system

### Agent Workspace Storage Structure

**Default Paths**:
- Git repos base: `~/.fulcrum/repos/` (configurable via `FULCRUM_GIT_REPOS_DIR` or settings)
- Scratch base: `~/.fulcrum/scratch/`
- Fulcrum data: `~/.fulcrum/` (configurable via `FULCRUM_DIR` env var)

**Per-Task Terminal State**:
- `terminals` table: `id`, `cwd`, `tmuxSession`, `status`, `exitCode`, `tabId`
- `terminalTabs` table: First-class tab entities with positions
- `terminalViewState` table: Singleton UI state (active tab, focused terminals)

**Task Isolation**:
```typescript
// From server/db/schema.ts
worktreePath: text('worktree_path'),  // Path to git worktree
type: text('type'),                   // 'worktree' | 'scratch' | null
agent: text('agent'),                 // 'claude' | 'opencode'
agentOptions: text('agent_options'),  // JSON: { [flag]: value }
```

### Agent Configuration Hierarchy

**Priority** (highest → lowest):
1. Task-level agent config (`agentOptions` on task)
2. Repository-level agent config (`claudeOptions`/`opencodeOptions` on repository)
3. Project-level agent config (on project)
4. Global agent config (in settings)

**Configuration Includes**:
- Agent type (`claude` vs `opencode`)
- Model selection (OpenCode: `provider/model` format)
- CLI flags (stored as JSON map)
- Startup scripts (bash commands to run before agent invocation)
- Copy files patterns (glob patterns for non-git copy to worktrees)

---

## 4. CLAUDE CODE INTEGRATION

### Plugin Architecture

**Location**: `plugins/fulcrum/`

**Plugin Components**:
1. **Skills** (`skills/fulcrum/SKILL.md`):
   - Provides CLI reference for agents
   - Covers: current-task commands, notifications, config, server management, mcp2cli tool access

2. **Hooks** (`hooks/hooks.json`):
   - `Stop` hook: Runs `fulcrum current-task review` when Claude stops
   - `UserPromptSubmit` hook: Runs `fulcrum current-task in-progress` when user responds

3. **Commands** (`commands/`):
   - `pr.md` — Link PR to current task
   - `notify.md` — Send notifications
   - `task-info.md` — Get current task details

### Claude Code Status Sync

**Flow**:
```
Claude stops (idle)
  → Stop hook fires
  → `fulcrum current-task review`
  → Task moves to IN_REVIEW status in Fulcrum UI
  → Notification sent to user
  ↓
User responds in Claude
  → UserPromptSubmit hook fires
  → `fulcrum current-task in-progress`
  → Task moves back to IN_PROGRESS
```

**Auto-Detection**:
- Hooks detect task ID via git worktree path
- `current-task` CLI command auto-detects current directory's task
- No explicit configuration needed per task

### MCP Server Integration

**Transport**: HTTP + stateless sessions
- Endpoint: `/mcp` (full tool access)
- Endpoint: `/mcp/observer` (restricted tools for message processing)

**Tool Access**:
- Claude connects via MCP server (exposed as CLI: `fulcrum mcp`)
- 100+ tools available across categories

---

## 5. OPENCODE INTEGRATION

### Plugin Structure

**Location**: `plugins/fulcrum-opencode/index.ts`

**Core Features**:
1. **SDK Integration**:
   ```typescript
   import { createOpencode, createOpencodeClient } from '@opencode-ai/sdk'
   ```
   - Auto-starts OpenCode server on port 4096 if not running
   - Maintains client singleton
   - Maps Fulcrum session ID ↔ OpenCode SDK session ID

2. **Status Tracking** (similar to Claude):
   ```
   User input
   → recordActivity("user message")
   → setStatus("in-progress")
   
   OpenCode idle (1.5s debounce)
   → scheduleIdleTransition()
   → setStatus("review")
   → `fulcrum current-task review`
   ```

3. **Activity Monitoring**:
   - Tracks: chat.message, session.created, session.idle, session.status, tool.execute
   - Ignores noisy events: message.part.updated, file.watcher.updated, tui.toast.show, config.updated
   - Debounces status changes (500ms) to avoid thrashing

4. **Subagent Support**:
   - Tracks subagent sessions separately
   - Ignores subagent idle transitions
   - Only main session triggers task status updates

### OpenCode Chat Service

**Location**: `server/services/opencode-chat-service.ts`

**Features**:
- Lazy-loads OpenCode client on first message
- Builds contextual system prompts from current page
- Streams responses via `/api/assistant/sessions/:id/messages`
- Integrates with Fulcrum MCP tools (read-only for observer)
- Supports attachments (images, files)

---

## 6. CLAUDE CODE vs OPENCODE - KEY DIFFERENCES

| Feature | Claude Code | OpenCode |
|---------|-------------|----------|
| **SDK** | `@anthropic-ai/claude-agent-sdk` | `@opencode-ai/sdk` |
| **Status Sync** | Via Claude Code plugin hooks | Via OpenCode plugin SDK |
| **Model** | Claude 3.5 Sonnet (fixed) | Configurable (GPT-4, Claude, etc.) |
| **Cost** | Higher (Anthropic's pricing) | Lower (uses other providers) |
| **Terminal** | Spawned separately | Works with same PTY system |
| **MCP Support** | Full integration | Restricted tool access (observer) |
| **Context Resume** | Claude Agent SDK session ID | OpenCode SDK session tracking |

---

## 7. MCP (Model Context Protocol) IMPLEMENTATION

### Tool Registry & Discovery

**Location**: `cli/src/mcp/registry.ts` + `cli/src/mcp/tools/`

**Tool Categories** (20+):
- `core` — search_tools, list_tasks, get_task, create_task, move_task, etc.
- `tasks` — Task CRUD + dependencies + tags + links + attachments
- `projects` — Project CRUD + links + attachments
- `repositories` — Repository management + scanning
- `apps` — Docker Compose deployment
- `filesystem` — File browsing, read/write
- `exec` — Shell command execution with persistent sessions
- `notifications` — Multi-channel notifications
- `settings` — Configuration read/update
- `backup` — Database snapshot/restore
- `email` — Gmail draft management
- `messaging` — Channel management (WhatsApp, Discord, Telegram, Slack)
- `assistant` — Chat session management + message sending
- `caldav` — Calendar sync + event management
- `gmail` — Google account management
- `memory` — Knowledge store (master file + ephemeral)
- `jobs` — Systemd/launchd timer management
- `search` — Unified FTS5 full-text search

### Tool Deferred Loading

```typescript
// Core tools (always loaded)
defer_loading: false

// Deferred tools (discovered via search_tools)
defer_loading: true
```
- Only ~8 core tools in context by default
- Other tools discovered on-demand via `search_tools`
- Reduces prompt bloat for agents

### MCP Server Entry Points

1. **HTTP Transport** (stateless per-request):
   - Endpoint: `POST /mcp`
   - Transport: `WebStandardStreamableHTTPServerTransport`
   - Used by: Claude Desktop, external agents

2. **Stdio Transport** (persistent):
   - Command: `fulcrum mcp`
   - Used by: CLI tools (mcp2cli), plugins

3. **HTTP Restricted** (observer mode):
   - Endpoint: `/mcp/observer`
   - Limited tools for message processing
   - Used by: Message observer (cost control)

---

## 8. KEY FILES & MODULES

### Entry Points

| File | Purpose |
|------|---------|
| `server/index.ts` | Server startup, daemon checks, service initialization |
| `server/app.ts` | Hono app setup, route mounting |
| `server/routes/mcp.ts` | MCP HTTP transport handler |
| `cli/src/mcp/index.ts` | Stdio MCP server setup |
| `frontend/routes/__root.tsx` | Root layout, navigation |

### Core Services

| Service | Location | Purpose |
|---------|----------|---------|
| **Task Management** | `server/services/task-service.ts` | CRUD, status transitions, recurrence |
| **Task Status** | `server/services/task-status.ts` | Status workflow, recurrence logic |
| **Assistant** | `server/services/assistant-service.ts` | Chat sessions, message storage, Claude SDK integration |
| **OpenCode Chat** | `server/services/opencode-chat-service.ts` | OpenCode client, streaming responses |
| **Terminal** | `server/terminal/pty-manager.ts` | PTY creation, buffer management, terminal lifecycle |
| **Memory** | `server/services/memory-service.ts` | Ephemeral memory (FTS5) |
| **Memory File** | `server/services/memory-file-service.ts` | Master memory file (injected into system prompt) |
| **Search** | `server/services/search-service.ts` | Unified FTS5 search across all entities |
| **Notifications** | `server/services/notification-service.ts` | Multi-channel notifications (Slack, Discord, etc.) |
| **Messaging** | `server/services/channels/` | WhatsApp, Discord, Telegram, Slack, Email |
| **PR Monitor** | `server/services/pr-monitor.ts` | GitHub PR polling, auto-close tasks |
| **CalDAV** | `server/services/caldav/` | Calendar sync with event copy rules |
| **Google** | `server/services/google/` | Google Calendar, Gmail, OAuth2 |
| **Apps** | `server/services/` (docker, cloudflare, traefik) | Docker Compose deployment, DNS, tunnels |

### Database Schema

| File | Purpose |
|------|---------|
| `server/db/schema.ts` | Full Drizzle schema (30+ tables) |
| `drizzle/migrations/` | SQL migrations (16+ versions) |
| `server/db/index.ts` | Database initialization |

### Configuration

| File | Purpose |
|------|---------|
| `server/lib/settings/fnox.ts` | fnox config wrapper + cache |
| `server/lib/settings/types.ts` | Settings schema (80+ settings) |
| `server/lib/settings/migrate-to-fnox.ts` | Migration from settings.json to fnox |
| `.fnox.toml` | Single source of truth for all config |
| `age.txt` | Age private key for secret encryption |

### MCP Tools

| Module | Location |
|--------|----------|
| Core tools | `cli/src/mcp/tools/core.ts` |
| Task tools | `cli/src/mcp/tools/tasks.ts` |
| Project tools | `cli/src/mcp/tools/projects.ts` |
| Repository tools | `cli/src/mcp/tools/repositories.ts` |
| App tools | `cli/src/mcp/tools/apps.ts` |
| Filesystem tools | `cli/src/mcp/tools/filesystem.ts` |
| Execution tools | `cli/src/mcp/tools/exec.ts` |
| Assistant tools | `cli/src/mcp/tools/assistant.ts` |
| Memory tools | `cli/src/mcp/tools/memory.ts` + `memory-file.ts` |
| Search tools | `cli/src/mcp/tools/search.ts` |
| Messaging tools | `cli/src/mcp/tools/messaging.ts` |
| Calendar tools | `cli/src/mcp/tools/caldav.ts` |
| Gmail tools | `cli/src/mcp/tools/gmail.ts` |
| Job tools | `cli/src/mcp/tools/jobs.ts` |
| Notification tools | `cli/src/mcp/tools/notifications.ts` |

---

## 9. EXTENSION POINTS FOR REPLACING CLAUDE CODE

### 1. Agent Interface Layer

**Current**:
- Claude Code via `@anthropic-ai/claude-agent-sdk`
- OpenCode via `@opencode-ai/sdk`

**Extension Point**: Create abstraction layer for new agent backends

```typescript
// Proposed interface
interface AgentBackend {
  name: string
  startSession(options: AgentSessionOptions): Promise<AgentSession>
  sendMessage(sessionId: string, message: string): Promise<void>
  onStatusChange(handler: (status: TaskStatus) => void): void
}

// Plugin implementations
// - ClaudeCodeBackend (existing)
// - OpencodeBackend (existing)
// - OmegaAgentBackend (new)
```

**Files to Extend**:
- `server/services/assistant-service.ts` — Add provider abstraction
- `server/routes/assistant.ts` — Route handler for provider-agnostic messages
- `cli/src/mcp/tools/tasks.ts` — Provider-agnostic task status updates

### 2. Plugin Hook System

**Current** (Claude Code):
```json
// plugins/fulcrum/hooks/hooks.json
{
  "hooks": {
    "Stop": [{ "hooks": [{ "type": "command", "command": "fulcrum current-task review" }] }],
    "UserPromptSubmit": [{ "hooks": [{ "type": "command", "command": "fulcrum current-task in-progress" }] }]
  }
}
```

**Extension Point**: Generalized hook registry

```typescript
// New file: server/lib/agent-hooks.ts
interface AgentHook {
  agent: 'claude' | 'opencode' | 'omega'
  event: 'idle' | 'busy' | 'message' | 'complete'
  action: 'task_status_update' | 'notification' | 'mcp_call'
}

// Then in assistant-service.ts:
// Check hooks on agent idle → trigger actions
```

### 3. Status Sync Mechanism

**Current Flow**:
```
Agent idle event → Hook fires → CLI command → Task status update
```

**Generalized Approach**:
```typescript
// server/services/agent-status-tracker.ts
async function trackAgentStatus(
  agentType: 'claude' | 'opencode' | 'omega',
  sessionId: string,
  taskId: string
) {
  // Subscribe to agent idle/active events
  // Translate to Fulcrum task status changes
  // Send notifications
}
```

### 4. MCP Tool Access Pattern

**Current**:
- Claude: Full MCP tool access via HTTP `/mcp` endpoint
- OpenCode: Restricted tools via `/mcp/observer`
- Proposed (Omega): Could use same HTTP endpoints

**Extension**:
```typescript
// server/routes/mcp.ts - already provider-agnostic
// No changes needed - any agent can call HTTP endpoint
// Just register new agent in status tracking
```

### 5. Terminal & PTY Integration

**Current**: Agent-agnostic terminal spawning
- `server/terminal/pty-manager.ts` — Handles all PTY operations
- Works for Claude Code, OpenCode, custom agents

**Already Extensible**: PTY manager doesn't care which agent runs in terminal
```typescript
const terminal = ptyManager.spawn({
  cwd: worktreePath,
  name: taskId,
  shell: '/bin/bash',
})
// Works for any agent with compatible shell access
```

### 6. New Agent Implementation Template

**Steps to integrate new agent (e.g., Omega)**:

1. **Create Agent Service** (`server/services/omega-chat-service.ts`):
   ```typescript
   import { createOmegaClient } from '@omega/sdk'
   
   export async function streamOmegaMessage(
     sessionId: string,
     message: string,
     // ... other params
   ) {
     const client = getOmegaClient()
     const response = await client.chat.create({ message, /** ... */ })
     // Stream response via SSE
   }
   ```

2. **Extend Assistant Routes** (`server/routes/assistant.ts`):
   ```typescript
   if (session.provider === 'omega') {
     return streamOmegaMessage(sessionId, message, /* ... */)
   }
   ```

3. **Add Agent Config**:
   ```typescript
   // server/db/schema.ts - extend agent selection
   agent: text('agent').notNull().default('claude'), // Add 'omega'
   
   // server/lib/settings/types.ts - extend agent options
   agent: {
     defaultAgent: 'claude' | 'opencode' | 'omega'
     omegaOptions?: OmegaConfig
   }
   ```

4. **Create Plugin** (`plugins/omega/`):
   - Mirrors Claude Code plugin structure
   - Hooks into agent events → task status updates

5. **Register MCP Tools** (existing tools work for any agent):
   - No changes needed
   - New agent can access via same HTTP endpoints

---

## 10. DIRECTORY STRUCTURE

```
fulcrum-main/
├── server/
│   ├── index.ts                    # Entry point, service startup
│   ├── app.ts                      # Hono app setup
│   ├── routes/
│   │   ├── mcp.ts                  # MCP HTTP handler
│   │   ├── mcp-restricted.ts       # Observer-mode MCP
│   │   ├── tasks.ts                # Task CRUD API
│   │   ├── assistant.ts            # Chat sessions API
│   │   ├── assistant-service.ts    # Claude + OpenCode service
│   │   ├── opencode-chat-service.ts # OpenCode streaming
│   │   ├── search.ts               # Unified search API
│   │   ├── memory.ts               # Memory CRUD
│   │   ├── messaging.ts            # Messaging channels
│   │   ├── caldav.ts               # Calendar sync
│   │   ├── google.ts               # Google API
│   │   ├── apps.ts                 # App deployment
│   │   ├── jobs.ts                 # Systemd/launchd timers
│   │   └── ... (20+ more)
│   ├── services/
│   │   ├── task-service.ts         # Task logic
│   │   ├── task-status.ts          # Status transitions, recurrence
│   │   ├── assistant-service.ts    # Session management
│   │   ├── opencode-chat-service.ts # OpenCode SDK wrapper
│   │   ├── memory-service.ts       # Knowledge store
│   │   ├── search-service.ts       # FTS5 search
│   │   ├── notification-service.ts # Multi-channel notifications
│   │   ├── pr-monitor.ts           # GitHub PR polling
│   │   ├── channels/               # Messaging (WhatsApp, Discord, etc.)
│   │   ├── caldav/                 # Calendar integration
│   │   ├── google/                 # Google APIs
│   │   └── ... (15+ more)
│   ├── terminal/
│   │   ├── pty-manager.ts          # PTY lifecycle
│   │   ├── pty-instance.ts         # Singleton + destroy
│   │   ├── terminal-session.ts     # Terminal state
│   │   ├── buffer-manager.ts       # TTY output buffering
│   │   ├── dtach-service.ts        # dtach wrapper
│   │   └── tab-manager.ts          # Terminal tab state
│   ├── websocket/
│   │   └── terminal-ws.ts          # Terminal I/O WebSocket
│   ├── db/
│   │   ├── schema.ts               # Drizzle schema (30+ tables)
│   │   └── index.ts                # DB initialization
│   ├── lib/
│   │   ├── settings/               # Config (fnox + migration)
│   │   ├── logger.ts               # JSONL logging
│   │   ├── git-utils.ts            # git worktree operations
│   │   └── ... (utilities)
│   └── __tests__/
├── frontend/
│   ├── routes/
│   │   ├── tasks/                  # Task kanban pages
│   │   ├── projects/               # Project management
│   │   ├── repositories/           # Repo browser
│   │   ├── assistant/              # Chat UI
│   │   ├── calendar/               # Calendar view
│   │   ├── jobs/                   # Timer management
│   │   ├── apps/                   # App deployment UI
│   │   ├── monitoring/             # System metrics
│   │   └── settings/               # Settings pages
│   └── components/
│       ├── kanban/                 # Task board
│       ├── task/                   # Task detail
│       ├── terminal/               # Terminal emulation
│       ├── chat/                   # Chat UI
│       ├── messaging/              # Messaging channels
│       ├── calendar/               # Calendar widget
│       ├── ui/                     # shadcn components
│       └── ... (20+ more)
├── cli/
│   ├── src/
│   │   ├── mcp/
│   │   │   ├── index.ts            # Stdio MCP server
│   │   │   ├── registry.ts         # Tool metadata
│   │   │   ├── tools/              # 20+ tool modules
│   │   │   └── utils.ts
│   │   ├── commands/               # CLI commands
│   │   ├── client.ts               # HTTP client for API
│   │   └── utils/                  # Utilities
│   ├── server/                     # Bundled server (generated)
│   ├── dist/                       # Frontend assets (generated)
│   └── drizzle/                    # Migrations (generated)
├── plugins/
│   ├── fulcrum/
│   │   ├── skills/                 # Claude skill documentation
│   │   ├── hooks/                  # Claude hooks
│   │   └── commands/               # Slash commands
│   └── fulcrum-opencode/
│       └── index.ts                # OpenCode plugin
├── drizzle/
│   ├── migrations/                 # SQL migrations
│   └── meta/                       # Migration metadata
├── shared/
│   └── types.ts                    # Shared TypeScript types
├── desktop/                        # Neutralino desktop app
├── package.json                    # Dependencies
├── tsconfig.json                   # TypeScript config
├── vite.config.ts                  # Frontend build config
├── drizzle.config.ts               # Drizzle config
└── fnox.toml                       # Configuration (generated)
```

---

## 11. DATA FLOW EXAMPLES

### Task Creation & Execution Flow

```
User creates task in UI
  ↓
POST /api/tasks (frontend → backend)
  ↓
CREATE task (server/routes/tasks.ts)
  ↓
Generate git worktree if type='worktree' (server/lib/git-utils.ts)
  ↓
Spawn PTY session in worktree (server/terminal/pty-manager.ts)
  ↓
Store terminal reference in DB (terminals table)
  ↓
Return task + terminal info to frontend
  ↓
Frontend opens terminal tab
  ↓
WebSocket connection to /ws/terminal
  ↓
Terminal I/O streams via WS (xterm.js renders)
  ↓
User/agent runs Claude Code in terminal
  ↓
Claude hook fires on idle → fulcrum current-task review
  ↓
Task status updated to IN_REVIEW (MCP tool call)
  ↓
Frontend re-renders kanban board
  ↓
Notification sent to user
```

### Agent Status Update Flow

```
Claude Code stops (idle event)
  ↓
Plugin hook: Stop → fulcrum current-task review
  ↓
CLI resolves current task from worktree path
  ↓
HTTP POST /api/tasks/{taskId}/status
  ↓
updateTaskStatus() (server/services/task-status.ts)
  ↓
Task moved to IN_REVIEW in database
  ↓
Broadcast via WebSocket to all connected clients
  ↓
Frontend receives task update
  ↓
Kanban board re-renders (task moved to IN_REVIEW column)
  ↓
Notification service triggered (if configured)
  ↓
Sends to Slack/Discord/Email/etc.
```

### MCP Tool Call Flow

```
Claude sends tool call via MCP
  ↓
HTTP POST /mcp (stateless)
  ↓
Server creates new MCP connection
  ↓
Tool request routed to handler (cli/src/mcp/tools/*)
  ↓
FulcrumClient makes HTTP call back to /api/* endpoint
  ↓
Backend handler processes request
  ↓
Response returned to FulcrumClient
  ↓
FulcrumClient returns to MCP tool handler
  ↓
Tool response sent back via MCP HTTP response
  ↓
Claude processes tool result
```

---

## 12. CRITICAL ARCHITECTURAL DECISIONS

### 1. **Terminal Isolation via dtach**
- Each task gets separate dtach socket + shell
- Survives server restart (persistent sessions)
- Trade-off: dtach is complex but provides true isolation

### 2. **Stateless MCP HTTP Transport**
- Each request creates new MCP connection
- No session state server-side
- Trade-off: Simpler but slightly higher latency vs. persistent connection

### 3. **SQLite with Drizzle ORM**
- Single-file database (portable)
- FTS5 full-text search (good enough for agent memory)
- Trade-off: No distributed setup (but not needed for solopreneur use case)

### 4. **Deferred Tool Loading**
- Only 8 core tools in context by default
- Other tools discovered via `search_tools`
- Trade-off: Extra API call for discovery, but saves token budget

### 5. **Per-Task Agent Configuration**
- Agent type, model, CLI options can be overridden per-task/repo/project/global
- Enables multi-agent workflows in single Fulcrum instance
- Trade-off: Configuration complexity but maximum flexibility

### 6. **fnox for Configuration**
- Single `.fnox.toml` file (replaces multiple JSON files)
- age encryption for secrets
- Trade-off: fnox is less common tool than typical config libs

---

## 13. EXTENSION ROADMAP FOR OMEGA AGENT ORCHESTRATION

### Phase 1: Agent Backend Abstraction (Medium Priority)
- Extract agent-specific logic from `assistant-service.ts`
- Create provider interface: `AgentProvider { startSession, sendMessage, onStatusChange }`
- Implement adapters: `ClaudeProvider`, `OpencodeProvider`, `OmegaProvider`

### Phase 2: Generalized Status Tracking (High Priority)
- Create `server/services/agent-status-tracker.ts`
- Subscribe to agent events (idle, busy, message)
- Automatically map to Fulcrum task statuses
- Works for any agent implementing provider interface

### Phase 3: Unified Plugin Hook System (Medium Priority)
- Generalize Claude/OpenCode hooks into agent-agnostic hook registry
- Hook types: `on_agent_idle`, `on_agent_message`, `on_task_complete`
- Agent plugins register hooks at startup

### Phase 4: Model Context Protocol Enhancement (Low Priority)
- Current MCP tools are already agent-agnostic
- Could add agent-specific discovery (e.g., list agent capabilities)
- Could add agent control tools (pause, restart, etc.)

### Phase 5: Omega-Specific Features (Post-MVP)
- Custom settings for Omega configuration
- Omega-specific MCP tool subsets
- Omega model selection UI (if supporting multiple models)
- Omega-specific memory formats (if different from Claude)

---

## 14. SECURITY & ISOLATION NOTES

### Secret Management
- Encrypted via `age` (modern, auditable)
- Stored in `~/.fulcrum/age.txt` (private key)
- Never committed to git
- Environment variables can override secrets

### Task Isolation
- Git worktrees provide branch isolation (not full process isolation)
- Scratch directories provide filesystem isolation
- PTY sessions run as current user (no containerization)
- Agent code runs in user's shell (inherit permissions)

### Input Validation
- Task attachments: MIME type whitelist, 50MB size limit
- File operations: Path traversal prevented via path normalization
- API inputs: TypeScript types + runtime validation

### MCP Tool Restrictions
- `/mcp/observer` endpoint uses restricted tool set (cost control)
- Messaging channel inputs sanitized
- Admin tools (like backup) require explicit enable

---

## SUMMARY FOR OMEGA DESIGN

Fulcrum is a **fully extensible agent orchestration platform** with:

✅ **Agent-agnostic core**: Terminal, PTY, task management, MCP tools work for any agent
✅ **Clear integration points**: Provider interface, hook system, status tracking
✅ **Existing patterns**: Already supports 2 agents (Claude, OpenCode) - proof of concept
✅ **Strong isolation**: Per-task workspaces, independent terminals, independent config
✅ **Rich data model**: Tasks with dependencies, projects, calendars, messaging, memory
✅ **Production-ready infrastructure**: Database migrations, config management, logging

**To integrate Omega**: Implement `AgentProvider` interface + status tracker + plugin hooks.
**No major architectural changes needed** — Fulcrum was designed for this extensibility.

