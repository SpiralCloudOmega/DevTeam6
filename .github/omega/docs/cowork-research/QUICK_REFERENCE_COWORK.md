# Open Claude Cowork: Quick Reference

## 1-Minute Explainer
**Open Claude Cowork** = GUI wrapper for Claude Agent SDK that lets you:
- Create independent agent sessions in isolated workspaces
- Persist full session history in SQLite
- Stream agent outputs in real-time with visual UI
- Resume conversations with `--resume <sessionId>`
- Use it with Anthropic API or compatible providers

## Answers to Your 7 Questions

### 1. What is it? How does it differ from Claude Code?
- **What**: Desktop app (Electron) wrapping Claude Agent SDK
- **Differs from Claude Code**:
  - Claude Code = CLI-only, terminal-focused
  - Cowork = Native GUI, persistent history, multi-session management
  - Both use same underlying SDK and support resumable sessions

### 2. Architecture?
```
Electron Main (Node.js)
  ├─ IPC handlers → routes client/server events
  ├─ Runner → executes Claude SDK query()
  ├─ Session store → SQLite persistence
  └─ Config store → API management
           ↕ IPC Bridge
React UI
  ├─ Zustand store → session state
  ├─ Components → sidebar, chat, modals
  └─ useIPC hook → event subscription
```

**Tech Stack**: Electron 39 + React 19 + Zustand + Claude Agent SDK + SQLite3

### 3. Multi-agent support?
- ✅ **Multiple concurrent sessions** (each with own agent)
- ❌ **No inter-agent communication** (sessions isolated)
- ❌ **No task delegation** (cannot chain work between agents)
- ✅ **Per-session workspace isolation** (different working directories)

**For Omega**: Treat each Kanban task as one Cowork session (independent agents)

### 4. Workspace management?
Each session gets:
- **cwd**: Configurable working directory
- **Files scoped** to that directory (Read, Edit, Bash tools)
- **Isolation**: Sessions don't share filesystem
- **Recency tracking**: UI shows last 8 used directories

Database schema:
```sql
sessions (id, title, cwd, allowed_tools, claude_session_id, status, ...)
messages (id, session_id, data, created_at)
```

### 5. CLI interface?
- ❌ **No native CLI**
- ✅ **GUI-only interface** (Electron desktop app)
- ✅ **Backward compatible**: Users can still use `claude --resume <sessionId>` in terminal
- Sidebar shows copy-to-clipboard button for resume command

### 6. Key APIs?
**Window.electron (preload bridge)**:
```typescript
sendClientEvent(event: ClientEvent): void
onServerEvent(callback: (event: ServerEvent) => void): UnsubscribeFunction
generateSessionTitle(prompt: string): Promise<string>
getRecentCwds(limit?: number): Promise<string[]>
selectDirectory(): Promise<string | null>
getApiConfig(): Promise<ApiConfig | null>
saveApiConfig(config: ApiConfig): Promise<{ success: boolean }>
checkApiConfig(): Promise<{ hasConfig: boolean; config?: ApiConfig }>
```

**Server Events** (Main → Renderer):
- `stream.message` - Agent output
- `stream.user_prompt` - User input echoed
- `session.status` - Status change (idle/running/completed/error)
- `session.list` - Available sessions
- `session.history` - Messages for session
- `permission.request` - User confirmation needed
- `session.deleted` - Session removed

**Client Events** (Renderer → Main):
- `session.start` - Create + execute
- `session.continue` - Follow-up prompt
- `session.stop` - Pause execution
- `session.delete` - Remove session
- `session.list` - Get all sessions
- `session.history` - Load session messages
- `permission.response` - User permission answer

### 7. Integration with Fulcrum Kanban?

**Recommended: HTTP API Wrapper**

```
Fulcrum Kanban UI
    ↓
Omega Orchestrator
    ↓ (HTTP calls)
Cowork HTTP API Service
    ├─ POST /api/sessions/start
    ├─ GET /api/sessions/:id/stream (SSE)
    ├─ POST /api/sessions/:id/continue
    └─ GET /api/sessions/:id/artifacts
    ↓
SQLite + Agent Execution
```

**Mapping**:
| Fulcrum | Cowork |
|---------|--------|
| Board | Session list |
| Card/Task | Session (one per task) |
| Column | Session status |
| Workspace | Working directory (cwd) |
| Activity | Streaming messages |

**Benefits**:
- ✅ No modification to Cowork needed
- ✅ Persistent history per task
- ✅ Real-time progress feedback
- ✅ Workspace isolation per task
- ✅ Resume capability
- ✅ Artifact extraction from DB

**Example Omega Code**:
```typescript
async executeTask(task: KanbanTask) {
  const sessionId = await cowork.startSession({
    title: `[${task.id}] ${task.title}`,
    prompt: task.description,
    cwd: `/tasks/${task.id}/workspace`
  });
  
  cowork.streamSession(sessionId, (event) => {
    if (event.type === "stream.message") {
      fulcrum.updateCard(task.id, event.message);
    }
    if (event.type === "session.status" && event.status === "completed") {
      const artifacts = await cowork.getArtifacts(sessionId);
      fulcrum.moveCard(task.id, "Done", { artifacts });
    }
  });
}
```

---

## File Structure Cheat Sheet

```
src/electron/
  ├─ main.ts                          # Electron app entry
  ├─ ipc-handlers.ts                  # Event routing + session lifecycle
  ├─ preload.cts                      # IPC bridge (window.electron)
  └─ libs/
      ├─ runner.ts                    # Agent execution (runClaude)
      ├─ session-store.ts             # SQLite persistence
      ├─ claude-settings.ts           # API config
      └─ config-store.ts              # Config file management

src/ui/
  ├─ App.tsx                          # Main component
  ├─ store/useAppStore.ts             # Zustand state
  ├─ hooks/useIPC.ts                  # IPC subscription
  ├─ components/
  │   ├─ Sidebar.tsx                  # Session list
  │   ├─ StartSessionModal.tsx        # Workspace setup
  │   ├─ PromptInput.tsx              # User input
  │   ├─ EventCard.tsx                # Message rendering
  │   └─ SettingsModal.tsx            # API config UI
  └─ render/markdown.tsx              # Markdown + code highlight
```

---

## Key Insights for Omega Design

1. **Sessions are the unit of work** - Each task = one session
2. **Workspaces are per-session** - Each task gets isolated cwd
3. **Real-time streaming** - Full message iterator available
4. **Resumable** - Session IDs allow context preservation
5. **Local-first** - SQLite is the source of truth
6. **No orchestration** - Omega must implement task scheduling
7. **Lightweight wrapper** - Can extend with minimal changes

---

## Limitations to Know

- ❌ Desktop-only (Electron)
- ❌ Single-user per instance
- ❌ No built-in multi-agent coordination
- ❌ SQLite unbounded growth (no rotation)
- ❌ Manual session creation (no programmatic spawning API)
- ⚠️ Tool set hardcoded (Read, Edit, Bash only)
- ⚠️ Permission handling binary (allow/deny AskUserQuestion)

---

## Quick Start for Integration

1. **Use as-is**: Deploy as HTTP-wrapped service
2. **Extend preload**: Add `kanban.*` event types if needed
3. **Create Omega adapter**: Wrap HTTP API, handle SSE streams
4. **Share SQLite**: Fulcrum reads artifacts from Cowork DB
5. **Scale**: Spawn multiple Cowork instances for parallel tasks

