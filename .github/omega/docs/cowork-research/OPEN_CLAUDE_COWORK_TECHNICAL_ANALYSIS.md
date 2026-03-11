# Open Claude Cowork: Comprehensive Technical Analysis

## Executive Summary

**Open Claude Cowork** is a cross-platform desktop application (macOS, Linux, Windows) that provides a **GUI-based alternative to Claude Code CLI** with enhanced session management, persistence, and multi-session support. It wraps the Anthropic Claude Agent SDK in an Electron + React interface, enabling visual task management, file operations, code execution, and agent-powered workflows without requiring terminal access or deep SDK knowledge.

Unlike Claude Code (terminal-only), Open Claude Cowork offers:
- **Native desktop UI** with session history and management
- **Per-session workspace isolation** via configurable working directories
- **Persistent session storage** using SQLite
- **Real-time streaming visualization** of agent outputs
- **Tool permission controls** with user confirmation workflows
- **API configuration management** (supports Anthropic or compatible providers)

---

## 1. CORE PURPOSE & DIFFERENTIATION FROM CLAUDE CODE

### What It Is
A **desktop AI agent framework** that serves as a GUI wrapper and enhancement layer over the Claude Agent SDK, enabling:
- **Task-based workflow management** (create, monitor, resume sessions)
- **Code synthesis and execution** (supports Read, Edit, Bash tools)
- **File system operations** (creation, modification, organization)
- **Natural language instruction execution** in isolated workspaces

### Key Differences from Claude Code

| Aspect | Claude Code | Open Claude Cowork |
|--------|-------------|-------------------|
| **Interface** | Terminal/CLI only | Native GUI (Electron) |
| **Persistence** | Session resumable via CLI flag | Full session history + UI recovery |
| **Multi-session** | Sequential via terminal | Concurrent with visual tabs |
| **Workspace** | Current working directory | Per-session configurable CWD |
| **Visual Feedback** | Text streaming | Markdown + partial message rendering |
| **Configuration** | `~/.claude/settings.json` | UI-based + file fallback |
| **Tool Approval** | Binary allow/deny per tool | UI-based AskUserQuestion handling |

### Market Positioning
- **For**: Developers who want visual, persistent, multi-session AI agent management
- **Not for**: Users requiring strict terminal-only execution or server-side orchestration
- **Supports**: Any LLM compatible with Anthropic API (Claude 3.5, MiniMax, etc.)

---

## 2. ARCHITECTURE OVERVIEW

### System Architecture Diagram
```
┌─────────────────────────────────────────────────────────────┐
│                     ELECTRON MAIN PROCESS                   │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  IPC Handler Layer (ipc-handlers.ts)                   │ │
│  │  - Session lifecycle management                        │ │
│  │  - Event routing (client ↔ server)                     │ │
│  │  - Permission resolution                              │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  Runner Layer (runner.ts)                              │ │
│  │  - Claude Agent SDK query execution                    │ │
│  │  - Process spawning (fork-based)                       │ │
│  │  - Stream message handling                             │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  Storage Layer (session-store.ts)                      │ │
│  │  - SQLite persistence (sessions + messages)            │ │
│  │  - In-memory session objects                           │ │
│  │  - Abort controller management                         │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  Config Layer (claude-settings.ts, config-store.ts)    │ │
│  │  - API key / endpoint management                       │ │
│  │  - Fallback to ~/.claude/settings.json                 │ │
│  │  - Environment variable injection                      │ │
│  └────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
                            ↕ IPC Bridge
                  (preload.cts context bridge)
┌─────────────────────────────────────────────────────────────┐
│                   RENDERER PROCESS (React)                  │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  Zustand Store (useAppStore.ts)                        │ │
│  │  - Sessions state (Record<sessionId, SessionView>)     │ │
│  │  - Active session tracking                             │ │
│  │  - Permission request queues                           │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  UI Components                                         │ │
│  │  ├─ Sidebar: session list & navigation                 │ │
│  │  ├─ StartSessionModal: workspace config                │ │
│  │  ├─ PromptInput: message submission                    │ │
│  │  ├─ EventCard: message rendering (with streaming)      │ │
│  │  └─ SettingsModal: API config management               │ │
│  └────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Tech Stack

**Backend (Electron Main)**
- **Electron 39.2.7** - Desktop application framework
- **@anthropic-ai/claude-agent-sdk 0.2.6** - LLM agent execution
- **better-sqlite3 12.6.0** - Persistent message & session storage
- **Node.js 22+** - Runtime (bun or Node)

**Frontend (React)**
- **React 19.2.3** - UI framework
- **Zustand 5.0.10** - State management (lightweight alternative to Redux)
- **Tailwind CSS 4.1.18** - Styling
- **Vite 7.3.1** - Build tooling
- **React Markdown + Highlight.js** - Message rendering with syntax highlighting

**Key Library: Claude Agent SDK**
- Provides `query()` async iterator for streaming agent responses
- Handles tool execution (Read, Edit, Bash, AskUserQuestion)
- Returns `SDKMessage` objects with incremental content

### Entry Points

1. **Electron Main**: `src/electron/main.ts`
   - Creates BrowserWindow
   - Registers IPC handlers
   - Manages window lifecycle
   
2. **React UI**: `src/ui/main.tsx` → `src/ui/App.tsx`
   - Initializes Zustand store
   - Sets up IPC listener (useIPC hook)
   - Renders session sidebar + message panel

3. **IPC Bridge**: `src/electron/preload.cts`
   - Exposes `window.electron.*` API to renderer
   - Isolates main process from renderer

---

## 3. MULTI-AGENT SUPPORT & AGENT COLLABORATION

### Current Implementation: **SINGLE-AGENT PER SESSION** Model

Open Claude Cowork does **NOT** have native multi-agent collaboration features. However, it supports **concurrent independent agents** through session management:

#### Multi-Session Architecture (Not True Multi-Agent)
```javascript
// From ipc-handlers.ts
const runnerHandles = new Map<string, RunnerHandle>();
// Each session gets ONE agent execution handle
// Multiple sessions = multiple sequential/concurrent agent runs
```

**Key Facts:**
- ✅ **Multiple concurrent sessions allowed** - each with independent `runClaude()` execution
- ❌ **No inter-agent communication** - sessions are isolated
- ❌ **No task delegation** - cannot chain work between agents
- ❌ **No shared context** - each agent has separate workspace (configurable CWD)
- ✅ **Concurrent execution** - `runnerHandles` Map supports parallel session processing

#### Session Isolation
```typescript
// Session object (session-store.ts)
type Session = {
  id: string;                    // Unique session ID
  cwd?: string;                  // Per-session working directory
  claudeSessionId?: string;      // Resumable agent session ID
  status: SessionStatus;         // idle | running | completed | error
  pendingPermissions: Map;       // Tool approval queue
};
```

### How Sessions Execute (Roughly Concurrent)
1. User starts Session A at `/path/project-a` with prompt "refactor code"
2. User starts Session B at `/path/project-b` with prompt "write tests"
3. Main thread spawns **two separate `runClaude()` coroutines**
4. Each agent runs in isolated processes via Claude SDK
5. Stream events broadcast to UI in real-time
6. Agents are **independent** - no knowledge of each other

### For True Multi-Agent Collaboration
To add agent-to-agent communication, you would need:
1. **Message broker** (RabbitMQ, Redis Pub/Sub, or simple HTTP)
2. **Shared context store** (shared database or API)
3. **Orchestrator service** (coordinate task delegation)
4. **Result aggregation** (combine outputs from multiple agents)

**Current limitation**: Open Claude Cowork is designed for **user-centric, single-user, multi-task** workflows, not **agent-orchestrated** systems.

---

## 4. WORKSPACE MANAGEMENT (Per-Agent Isolation)

### Session Workspace Model
Each session operates in a **configurable working directory**:

#### Workspace Setup
```typescript
// session-store.ts
createSession(options: {
  cwd?: string;              // User-specified working directory
  title: string;             // Session display name
  prompt: string;            // Initial instruction
  allowedTools?: string;     // Tool whitelist (e.g., "Read,Edit,Bash")
}): Session
```

#### Workspace Isolation
```typescript
// runner.ts - agent execution
query({
  prompt,
  options: {
    cwd: session.cwd ?? DEFAULT_CWD,    // Per-session isolation
    pathToClaudeCodeExecutable: getClaudeCodePath(),
    permissionMode: "bypassPermissions",
    allowDangerouslySkipPermissions: true,
    // Tool execution happens WITHIN this cwd
  }
});
```

### Workspace Features
| Feature | Implementation |
|---------|-----------------|
| **Isolation** | Each session has independent `cwd` (filesystem path) |
| **File Operations** | Scoped to session's `cwd` (Read, Edit, Bash tools) |
| **Persistence** | SQLite stores `cwd` with session metadata |
| **Recency Tracking** | `listRecentCwds()` returns last 8 used directories |
| **UI Support** | StartSessionModal shows recent directories + file browser |

### Database Schema
```sql
CREATE TABLE sessions (
  id TEXT PRIMARY KEY,
  title TEXT,
  cwd TEXT,                  -- Workspace path
  allowed_tools TEXT,        -- Tool whitelist
  claude_session_id TEXT,    -- Resumable agent session
  status TEXT,               -- idle | running | completed | error
  last_prompt TEXT,          -- Last user instruction
  created_at INTEGER,
  updated_at INTEGER
);

CREATE TABLE messages (
  id TEXT PRIMARY KEY,
  session_id TEXT,           -- Foreign key to session
  data TEXT,                 -- JSON-serialized SDKMessage
  created_at INTEGER
);
```

**Note**: No automatic cleanup of session artifacts. Agents must handle file cleanup themselves or users must manually manage filesystem.

---

## 5. CLI INTERFACE & USER INTERACTION

### Does It Have a CLI?
**No native CLI.** Open Claude Cowork is **GUI-first only**.

However, it maintains **CLI compatibility**:
```bash
# Users can still use Claude Code CLI
claude --resume <sessionId>  # Resume in terminal
```

The Sidebar component actually shows this:
```typescript
// Sidebar.tsx
const command = `claude --resume ${resumeSessionId}`;
// Copy-to-clipboard for terminal users
```

### Interaction Model

#### User Flow (Typical Workflow)
```
1. Launch Application
   ↓
2. Check API Config (Settings Modal if needed)
   ↓
3. Click "+ New Task" → StartSessionModal appears
   ├─ Select working directory (browse or recent list)
   ├─ Enter prompt (natural language instruction)
   └─ Click "Start Session"
   ↓
4. Agent begins execution
   ├─ Real-time streaming output in message panel
   ├─ Permission requests appear inline
   └─ View tool calls + results
   ↓
5. Continue conversation or create new session
   ├─ Submit follow-up prompts
   ├─ Stop running session
   └─ Delete session history
   ↓
6. Persistent history maintained in SQLite
   ├─ Switch between sessions via sidebar
   ├─ Hydrate session history from DB
   └─ Resume previous conversations
```

#### IPC Event Flow
```
CLIENT (React)                          SERVER (Electron Main)
    ↓                                         ↓
SendEvent: session.start ─────────────→ handleClientEvent()
    ↑                                         ├─ createSession()
    ├─ ServerEvent: stream.message ←─────── ├─ runClaude()
    ├─ ServerEvent: stream.user_prompt      └─ emit(ServerEvent)
    ├─ ServerEvent: session.status
    ├─ ServerEvent: permission.request
    └─ (broadcast to all windows)
```

### Window.electron API (Preload Bridge)
```typescript
window.electron: {
  // Session management
  sendClientEvent(event: ClientEvent): void;
  onServerEvent(cb: (event: ServerEvent) => void): UnsubscribeFunction;
  
  // Title generation
  generateSessionTitle(userInput: string | null): Promise<string>;
  
  // File management
  getRecentCwds(limit?: number): Promise<string[]>;
  selectDirectory(): Promise<string | null>;
  
  // API configuration
  getApiConfig(): Promise<ApiConfig | null>;
  saveApiConfig(config: ApiConfig): Promise<{ success: boolean }>;
  checkApiConfig(): Promise<{ hasConfig: boolean; config?: ApiConfig }>;
}
```

---

## 6. KEY INTERFACES & EXPOSED APIs

### Server Event Types (Main → Renderer)
```typescript
type ServerEvent =
  | { type: "stream.message"; 
      payload: { sessionId: string; message: SDKMessage | UserPromptMessage } }
  | { type: "stream.user_prompt"; 
      payload: { sessionId: string; prompt: string } }
  | { type: "session.status"; 
      payload: { sessionId: string; status: SessionStatus; title?: string; cwd?: string; error?: string } }
  | { type: "session.list"; 
      payload: { sessions: SessionInfo[] } }
  | { type: "session.history"; 
      payload: { sessionId: string; status: SessionStatus; messages: StreamMessage[] } }
  | { type: "session.deleted"; 
      payload: { sessionId: string } }
  | { type: "permission.request"; 
      payload: { sessionId: string; toolUseId: string; toolName: string; input: unknown } }
  | { type: "runner.error"; 
      payload: { sessionId?: string; message: string } };
```

### Client Event Types (Renderer → Main)
```typescript
type ClientEvent =
  | { type: "session.start"; 
      payload: { title: string; prompt: string; cwd?: string; allowedTools?: string } }
  | { type: "session.continue"; 
      payload: { sessionId: string; prompt: string } }
  | { type: "session.stop"; 
      payload: { sessionId: string } }
  | { type: "session.delete"; 
      payload: { sessionId: string } }
  | { type: "session.list" }
  | { type: "session.history"; 
      payload: { sessionId: string } }
  | { type: "permission.response"; 
      payload: { sessionId: string; toolUseId: string; result: PermissionResult } };
```

### Session Data Model
```typescript
interface SessionInfo {
  id: string;                    // UUID
  title: string;                 // User-visible name
  status: "idle" | "running" | "completed" | "error";
  claudeSessionId?: string;      // Resumable session ID
  cwd?: string;                  // Working directory
  createdAt: number;             // Timestamp
  updatedAt: number;             // Last activity
}

interface SessionView {
  id: string;
  title: string;
  status: SessionStatus;
  cwd?: string;
  messages: StreamMessage[];     // Full conversation history
  permissionRequests: PermissionRequest[];
  lastPrompt?: string;
  createdAt?: number;
  updatedAt?: number;
  hydrated: boolean;             // Whether history loaded from DB
}
```

### Configuration API
```typescript
type ApiConfig = {
  apiKey: string;              // Auth token
  baseURL: string;             // Endpoint (e.g., https://api.anthropic.com)
  model: string;               // Model ID (e.g., claude-opus-4-1)
  apiType?: "anthropic";       // Provider type
};

// Auto-detected from:
// 1. UI config store (app.asar.unpacked/userData/api-config.json)
// 2. ~/.claude/settings.json (fallback)
// 3. Environment variables
```

---

## 7. INTEGRATION WITH FULCRUM-STYLE KANBAN SYSTEM

### Vision: Omega Agent System with Kanban Orchestration

Open Claude Cowork can replace Claude Code in a Fulcrum-style Kanban system by acting as the **task execution backend**. Here's the integration architecture:

#### High-Level Integration Model
```
FULCRUM KANBAN (React UI)
    ↓
OMEGA ORCHESTRATOR (Node.js service)
    ├─ Task scheduling
    ├─ Agent allocation
    ├─ Result aggregation
    └─ Board state updates
    ↓
OPEN CLAUDE COWORK (Electron Desktop App)
    ├─ Session spawning (one per task)
    ├─ Agent execution
    ├─ Progress streaming
    └─ Artifact persistence
```

#### Integration Strategy

**Option 1: IPC Extension (Tightest Integration)**
Extend Open Claude Cowork's IPC with new event types:
```typescript
// Add to ServerEvent type:
| { type: "kanban.task_started"; 
    payload: { sessionId: string; taskId: string; boardId: string } }
| { type: "kanban.task_completed"; 
    payload: { sessionId: string; taskId: string; artifacts: string[] } }
| { type: "kanban.task_failed"; 
    payload: { sessionId: string; taskId: string; error: string } }

// Add to ClientEvent type:
| { type: "kanban.create_task"; 
    payload: { taskId: string; title: string; description: string; boardId: string; cwd?: string } }
```

**Option 2: HTTP API Bridge (Loose Coupling)**
Create a thin HTTP wrapper:
```bash
# Start Cowork as server instead of desktop app
node dist-electron/main.js --server --port 3000

# Omega calls REST API
POST /api/sessions/start
{
  "title": "Task #123",
  "prompt": "Implement feature X",
  "cwd": "/workspace/project",
  "metadata": { "taskId": "task-123", "boardId": "board-1" }
}

# Receives SSE stream of events
GET /api/sessions/:sessionId/stream
```

**Option 3: Database-Driven (Eventually Consistent)**
Shared SQLite database (or PostgreSQL):
```sql
-- Cowork reads/writes sessions
-- Fulcrum reads Cowork results and updates board state
-- Omega orchestrator coordinates

CREATE TABLE tasks (
  id TEXT PRIMARY KEY,
  board_id TEXT,
  session_id TEXT,  -- References Cowork session
  status TEXT,      -- todo | in-progress | done | error
  created_at INTEGER
);
```

#### Mapping Concepts

| Fulcrum Concept | Open Claude Cowork | Implementation |
|-----------------|-------------------|-----------------|
| **Board** | Session list | Sidebar groups sessions by metadata |
| **Card/Task** | Session | One Cowork session per Kanban task |
| **Column** | Session status | idle → running → completed/error |
| **Assignment** | Working directory | Task-specific workspace (cwd) |
| **Activity Feed** | Messages panel | Streaming agent output |
| **Comments** | Permission requests | User confirmation UI |

#### Artifacts & Deliverables
```typescript
// Store outputs in session metadata
interface SessionWithArtifacts extends SessionInfo {
  artifacts: {
    files: string[];           // Created/modified files
    commands: { name: string; output: string }[];
    logs: string[];
    errorLogs?: string[];
  };
}

// Persisted in SQLite as JSON
```

#### Kanban Workflow Example
```
USER ACTION: Drag card from "Todo" → "In Progress"
    ↓
FULCRUM: Updates Kanban board state
    ↓
OMEGA ORCHESTRATOR: Receives board change event
    ↓
OMEGA: Calls Cowork API → session.start
    {
      prompt: "[Kanban card description]",
      cwd: "[Task-specific workspace]",
      metadata: { taskId, boardId, assignee }
    }
    ↓
COWORK: Spawns session, begins agent execution
    ↓
COWORK: Emits stream.message events (real-time feedback)
    ↓
OMEGA: Aggregates output, polls for completion
    ↓
SESSION COMPLETES: status → "completed" or "error"
    ↓
OMEGA: Extracts artifacts, writes to artifacts table
    ↓
FULCRUM: Queries Cowork DB, renders results on card
    ↓
USER: Views agent-generated files/logs directly on Kanban card
```

#### Practical Implementation Example
```typescript
// Omega → Cowork adapter
class CoworkAdapter {
  async executeKanbanTask(task: KanbanTask): Promise<void> {
    // 1. Create Cowork session
    const prompt = `
      [Task Instructions from Kanban]
      Title: ${task.title}
      Description: ${task.description}
      
      Deliverables:
      - Create files in ./output/
      - Log results to ./output/results.json
    `;
    
    const sessionId = await this.startSession({
      title: `[${task.id}] ${task.title}`,
      prompt,
      cwd: `/tasks/${task.id}/workspace`
    });
    
    // 2. Stream progress to Kanban
    this.subscribeToSession(sessionId, (event) => {
      if (event.type === "stream.message") {
        fulcrum.updateCardComment(task.id, event.message);
      }
      if (event.type === "session.status" && event.status === "completed") {
        this.extractArtifacts(sessionId).then(artifacts => {
          fulcrum.moveCard(task.id, "Done", { artifacts });
        });
      }
    });
  }
}
```

### Advantages of This Integration

✅ **Zero modification to Open Claude Cowork** - Use as-is  
✅ **Persistent agent history** - Full conversation available in Kanban card  
✅ **Visual real-time feedback** - Users see agent working on their Kanban board  
✅ **Workspace isolation** - Each Kanban task gets its own agent session & filesystem  
✅ **Resume capability** - Pause/resume tasks without losing context  
✅ **Multi-user friendly** - Each user can spawn independent Cowork instances  
✅ **No agent-to-agent coupling** - Tasks remain independent (can add orchestration later)

### Limitations & Considerations

⚠️ **No native multi-agent coordination** - Omega must implement task scheduling  
⚠️ **Desktop-only constraint** - Each Cowork instance is a single user's application  
⚠️ **Resource management** - May need multiple Cowork instances for concurrent tasks  
⚠️ **Artifact discovery** - Must parse Cowork messages to extract deliverables  
⚠️ **Error propagation** - Failed agents don't auto-retry (Omega must implement)

---

## 8. TECHNICAL DEEP DIVES

### Agent Execution Flow (runClaude Function)
```typescript
export async function runClaude(options: RunnerOptions): Promise<RunnerHandle> {
  const { prompt, session, resumeSessionId, onEvent, onSessionUpdate } = options;
  const abortController = new AbortController();
  
  // Spawn async execution
  (async () => {
    try {
      // 1. Load API config
      const config = getCurrentApiConfig();
      const env = buildEnvForConfig(config);
      
      // 2. Invoke Claude Agent SDK query()
      const q = query({
        prompt,
        options: {
          cwd: session.cwd,
          resume: resumeSessionId,          // Resume previous conversation
          abortController,
          env,
          permissionMode: "bypassPermissions",
          includePartialMessages: true,
          canUseTool: async (toolName, input, { signal }) => {
            if (toolName === "AskUserQuestion") {
              // Wait for user permission before proceeding
              const result = await new Promise<PermissionResult>((resolve) => {
                sendPermissionRequest(toolUseId, toolName, input);
                session.pendingPermissions.set(toolUseId, { resolve });
              });
              return result;
            }
            return { behavior: "allow", updatedInput: input };
          }
        }
      });
      
      // 3. Stream messages from iterator
      for await (const message of q) {
        // Extract session_id from init message
        if (message.type === "system" && message.subtype === "init") {
          session.claudeSessionId = message.session_id;  // Enable resume
        }
        
        // Broadcast to UI
        onEvent({ type: "stream.message", payload: { sessionId: session.id, message } });
        
        // Update session status on completion
        if (message.type === "result") {
          onEvent({
            type: "session.status",
            payload: { sessionId: session.id, status: message.subtype === "success" ? "completed" : "error" }
          });
        }
      }
    } catch (error) {
      if (error.name !== "AbortError") {
        onEvent({ type: "session.status", payload: { status: "error", error: error.message } });
      }
    }
  })();
  
  return { abort: () => abortController.abort() };
}
```

**Key Points:**
- **Non-blocking**: Wrapped in IIFE, runs asynchronously
- **Streaming**: Uses `for await...of` to process incremental messages
- **Resumable**: Captures `session_id` for `--resume` flag
- **Cancellable**: AbortController allows user stop
- **Permission handling**: Blocks on AskUserQuestion, waits for UI response

### Message Streaming Architecture
```
SDK Iterator (query())
    ↓ (for await...of)
SDKMessage | UserPromptMessage
    ↓ (emit via onEvent callback)
ServerEvent (stream.message type)
    ↓ (broadcast via ipcRenderer.send)
React Component (EventCard)
    ↓ (real-time rendering)
Markdown Renderer + Syntax Highlighting
```

### Session Resumption Flow
```typescript
// First interaction:
runClaude({
  prompt: "Write hello world in Python",
  session: newSession(),
  resumeSessionId: undefined  // First time
});
// → Returns session.claudeSessionId = "abc-123-xyz"

// Later interaction:
runClaude({
  prompt: "Add comments to the code",
  session: existingSession,
  resumeSessionId: "abc-123-xyz"  // Resume with context!
});
// → Agent remembers previous code, adds to it
```

### Database Persistence
```typescript
// Session storage in SQLite
this.db.prepare(
  `insert into sessions (...) values (...)`
).run(
  id, title, claudeSessionId, status, cwd, allowedTools, lastPrompt, createdAt, updatedAt
);

// Message storage
this.db.prepare(
  `insert or ignore into messages (id, session_id, data, created_at) values (?, ?, ?, ?)`
).run(id, sessionId, JSON.stringify(message), Date.now());

// Retrieval
const messages = this.db.prepare(
  `select data from messages where session_id = ? order by created_at asc`
).all(sessionId).map(row => JSON.parse(row.data));
```

**Implications:**
- Messages **can be large** (full JSON serialization)
- **No automatic cleanup** - SQLite will grow indefinitely
- **No indexing on message content** - Linear search only
- **Single file** - `userData/sessions.db` is the entire data store

### Electron-Renderer Bridge (IPC)
```
Main Process (Node.js)           Renderer Process (React)
    ↑
    ├─ ipcMain.on("client-event", handleClientEvent)
    └─ win.webContents.send("server-event", JSON.stringify(event))
    
    ↓
Preload Script (preload.cts)
    ├─ Validates frame security
    └─ Exposes window.electron API
    
    ↓
React App
    ├─ window.electron.sendClientEvent(event)
    └─ window.electron.onServerEvent(callback)
```

**Security measures:**
- Preload script runs in isolated context
- IPC events validated against sender frame URL
- All payloads JSON-serialized (no code execution)

---

## 9. LIMITATIONS & CONSTRAINTS

### Current Limitations
1. **No multi-agent orchestration** - Sessions are isolated, no built-in delegation
2. **Desktop-only** - Requires Electron, not suitable for servers
3. **Single-user** - Each instance is per-machine/user
4. **Manual task creation** - No API to programmatically spawn tasks
5. **No authentication** - Relies on OS-level access control + API keys in config
6. **Resource intensive** - Electron + Node.js runtime overhead
7. **Session resumption UI-only** - CLI resume via `claude --resume` works but not discoverable in app
8. **No plugins/extensions** - Architecture is closed, limited customization

### Database Limitations
- **No rotation** - SQLite file grows indefinitely
- **No sharding** - Single process, cannot scale to many simultaneous sessions
- **No query API** - Must direct access SQLite, no HTTP interface

### Tool Limitations
```typescript
// Hard-coded tools:
const DEFAULT_ALLOWED_TOOLS = "Read,Edit,Bash";

// Cannot add custom tools or restrict which ones are allowed per-session
// Tool approval is binary (allow/deny AskUserQuestion only)
```

---

## 10. PACKAGE & DEPLOYMENT

### Build Targets
```bash
npm run dist:mac-arm64    # Universal binary for M1/M2/M3
npm run dist:mac-x64      # Intel macOS
npm run dist:win          # Windows portable .exe
npm run dist:linux        # Linux AppImage
```

### Packaging Details (electron-builder.json)
```json
{
  "appId": "com.devagentforge.agentcowork",
  "productName": "Agent Cowork",
  "files": ["dist-electron", "dist-react"],
  "asarUnpack": ["node_modules/@anthropic-ai/claude-agent-sdk/**/*"],
  "mac": { "target": "dmg" },
  "linux": { "target": "AppImage" },
  "win": { "target": "portable" }
}
```

**Special handling:** Claude Agent SDK is unpacked from ASAR to allow dynamic node_modules loading (see patch file).

### Runtime Requirements
- **macOS**: 10.13+ (x64, arm64)
- **Linux**: glibc 2.28+ (AppImage self-contained)
- **Windows**: 7+ (portable, no installation)
- **Disk**: ~500MB (includes Electron + Node.js)

---

## 11. COMPARISON: OPEN CLAUDE COWORK VS FULCRUM + OMEGA AGENT

| Dimension | Open Claude Cowork | Fulcrum + Omega |
|-----------|-------------------|-----------------|
| **UI** | Desktop Electron | Web-based Kanban |
| **Agent Backend** | One agent per session | Multiple agents per task |
| **Orchestration** | Manual (user-driven) | Automatic (system-driven) |
| **Workspace** | Local filesystem | Hybrid (local + remote) |
| **Scalability** | Single-machine | Distributed (servers) |
| **Persistence** | SQLite (local) | Database (shared) |
| **Multi-user** | No (single instance) | Yes (web-based) |
| **Integration** | Standalone | Integrated system |

---

## 12. RECOMMENDATIONS FOR OMEGA INTEGRATION

### Recommended Approach: **HTTP API Wrapper** (Option 2)

**Why:** Decouples Omega from Electron, enables horizontal scaling

**Implementation:**
1. Fork Open Claude Cowork
2. Add `src/server/express.ts` - HTTP API layer
3. Expose endpoints:
   - `POST /api/sessions/start` - Create session
   - `GET /api/sessions/:id/stream` - SSE stream
   - `POST /api/sessions/:id/continue` - Follow-up prompt
   - `GET /api/sessions/:id/artifacts` - Extract outputs
4. Keep SQLite storage (reuse existing)
5. Deploy as containerized service (Docker)

**Benefits:**
- Cowork remains unchanged (minimal maintenance burden)
- Omega can spawn multiple Cowork instances
- Artifact extraction becomes straightforward (query DB)
- Easy to add load balancing

**Example Omega adapter:**
```typescript
// omega/adapters/cowork-adapter.ts
class CoworkClient {
  async executeTask(task: Task): Promise<ExecutionResult> {
    // POST /api/sessions/start
    const { sessionId } = await this.http.post("/sessions/start", {
      title: task.title,
      prompt: task.description,
      cwd: `/workspace/${task.id}`
    });
    
    // GET /api/sessions/:id/stream (SSE)
    const stream = this.http.stream(`/sessions/${sessionId}/stream`);
    stream.on("message", (event) => {
      fulcrum.updateCard(task.id, event);
    });
    
    // Wait for completion
    await stream.waitForStatus("completed");
    
    // GET /api/sessions/:id/artifacts
    const { files, logs } = await this.http.get(`/sessions/${sessionId}/artifacts`);
    
    return { sessionId, files, logs };
  }
}
```

---

## CONCLUSION

**Open Claude Cowork is a mature, production-ready GUI wrapper for the Claude Agent SDK**, designed for individual developers who need persistent, visual session management. It excels at:
- ✅ Session persistence and recovery
- ✅ Multi-session management
- ✅ Real-time visual feedback
- ✅ API configuration flexibility
- ✅ Workspace isolation

However, it **is not an orchestration platform**. For Omega's Kanban integration, wrap it with a minimal HTTP API adapter and treat each Kanban task as a Cowork session, storing artifacts in a shared database.

The integration enables:
- **Persistent agent history** on every Kanban card
- **Visual real-time feedback** of agent progress
- **Workspace isolation** per task
- **Resume capability** without losing context
- **Minimal changes** to existing Cowork codebase

---

## Appendix: Key Files Reference

| File | Purpose | Size | Key Classes/Functions |
|------|---------|------|----------------------|
| `src/electron/main.ts` | App lifecycle, window creation | 139 lines | app.on("ready"), BrowserWindow |
| `src/electron/ipc-handlers.ts` | Session lifecycle, event routing | 259 lines | handleClientEvent(), emit(), broadcast() |
| `src/electron/libs/runner.ts` | Agent execution, streaming | 154 lines | runClaude(), query() iterator |
| `src/electron/libs/session-store.ts` | SQLite persistence | 269 lines | SessionStore class, sessions/messages tables |
| `src/electron/libs/claude-settings.ts` | API config management | 78 lines | getCurrentApiConfig(), buildEnvForConfig() |
| `src/ui/App.tsx` | Main React component | 300+ lines | useIPC(), message streaming, modal management |
| `src/ui/store/useAppStore.ts` | Zustand state management | 200+ lines | SessionView, AppState, event handlers |
| `src/electron/preload.cts` | IPC bridge, security | 49 lines | window.electron API exposure |

---

**Document Version**: 1.0  
**Last Updated**: 2025-02-13  
**Scope**: Technical analysis for Omega integration planning  
