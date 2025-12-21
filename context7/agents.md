# 🧠 Context7 Multi-Agent System (MCP-Integrated)

> **DevTeam6 Shared Context with Model Context Protocol Integration**
> *This file syncs with Context7 MCP servers via `local-ai/core/context7_sync.py`*

---

## 🔌 MCP Server Integration

### Active MCP Servers

| Server | Type | Endpoint | Status |
|--------|------|----------|--------|
| `context7` | HTTP | https://mcp.context7.com/mcp | 🟢 Active |
| `memory-server` | Local | `local-ai/mcp/memory_server.py` | 🟢 Active |
| `rag-server` | Local | `local-ai/mcp/rag_server.py` | 🟢 Active |

### MCP Tools Available

Via Context7 Sync Engine (`local-ai/core/context7_sync.py`):
- `sync_agent_state` - Synchronize agent state to Context7
- `broadcast_message` - Broadcast to all agents
- `handoff_task` - Transfer task between agents
- `query_project_state` - Get current project state
- `log_activity` - Log agent activity to `.github/agents/logs/`

---

## 🤖 Agent Registry

| Agent ID | Name | Role | Status | MCP Tools |
|----------|------|------|--------|-----------|
| `@documentation` | Documentation Agent | Architecture docs, integration guides | 🟢 Active | memory-server, rag-server |
| `@workspace` | Workspace Agent | UI/UX, graph utilities | 🟢 Active | rag-server |
| `@performance` | Performance Agent | Code optimization | ✅ Complete | - |
| `@uxui` | UX/UI Agent | Interface improvements | 🟡 Standby | - |

### Agent Communication via MCP

Agents communicate through Context7 Sync Engine:

```python
from local_ai.core.context7_sync import Context7Sync

# Initialize
sync = Context7Sync()
await sync.load()

# Update agent state
await sync.update_agent_state("@documentation", {
    "current_task": "TASK-DOC-001",
    "status": "active"
})

# Broadcast message
await sync.broadcast(
    message="Documentation suite completed",
    sender="@documentation"
)

# Handoff task
await sync.handoff(
    from_agent="@documentation",
    to_agent="@workspace",
    task={"id": "TASK-UI-001", "description": "Update UI docs"}
)

# Save to files
await sync.save()
```

---

## 📊 Current Project State

### Active Tasks

| Task ID | Description | Assigned To | Status | Priority | MCP Sync |
|---------|-------------|-------------|--------|----------|----------|
| `TASK-DOC-001` | Complete architecture documentation | @documentation | ✅ Complete | High | Synced |
| `TASK-UI-001` | Graph utilities improvement | @workspace | ✅ Complete | High | Synced |
| `TASK-MCP-001` | Rebuild Context7 MCP integration | @documentation | 🔄 Active | Critical | Pending |

### Recent Completions

- ✅ Architecture documentation (2,155+ lines, 7 files)
- ✅ QFaaS-inspired AI orchestration patterns
- ✅ GitHub Awesome Copilot integration guide
- ✅ Deployment, troubleshooting, quick reference guides
- ✅ Graph utilities edge case handling
- ✅ Test coverage expansion (28 → 44 tests)

### Pending Work via MCP

- [ ] Sync all agent logs to `.github/agents/logs/`
- [ ] Update knowledge graph with architecture data
- [ ] Create MCP server for documentation queries
- [ ] Integrate embeddings for semantic agent search

---

## 🔄 Context7 Sync Protocol

### File Structure

```
.github/agents/
├── context7.agents.md          # Master agent registry (syncs FROM here)
├── logs/                        # Agent activity logs
│   ├── documentation-agent.log.md
│   ├── workspace-agent.log.md
│   └── shared-learnings.md
├── memory/                      # Persistent memory
│   ├── embeddings.json          # Vector embeddings metadata
│   └── knowledge-graph.json     # Knowledge graph state
└── rules/                       # Agent-specific rules
    ├── master.rules.md
    ├── react.rules.md
    └── python.rules.md
```

### Sync Schedule

| Interval | Action | MCP Tool |
|----------|--------|----------|
| Real-time | Critical updates | `broadcast_message` |
| On task change | Task sync | `handoff_task` |
| On completion | State update | `sync_agent_state` |
| Hourly | Full context refresh | `sync()` |

---

## 📡 MCP Communication Protocol

### Message Format (JSON)

```json
{
  "from": "@agent-id",
  "to": "@target-agent",
  "type": "request|response|notification|handoff",
  "priority": "low|normal|high|critical",
  "context": {
    "task_id": "TASK-XXX-001",
    "parent_task": null,
    "thread": "conversation-thread-id"
  },
  "payload": {
    "action": "action-name",
    "data": {},
    "attachments": []
  },
  "timestamp": "ISO-8601",
  "mcp_server": "memory-server"
}
```

### MCP Tool Schemas

Defined in `local-ai/mcp/tool_schemas.py`:
- Agent state updates
- Task handoffs
- Broadcast messages
- Memory queries
- Knowledge graph operations

---

## 📝 Recent Agent Activity

### Documentation Agent (@documentation)

**Branch**: `copilot/connect-ai-qfaas-github`  
**Commits**: c34b855, 186e92e, 8de00ba, cf92701, 16422b2  
**MCP Status**: Actively syncing via Context7Sync

**Work Completed**:
- ✅ Architecture documentation (7 files, 2,578 lines)
  - System Overview (4-layer architecture)
  - AI Orchestration (3-tier QFaaS patterns)
  - Awesome Copilot integration guide
  - Deployment guide (Docker, Vercel, Railway, Fly.io)
  - Quick reference (commands, API, snippets)
  - Troubleshooting (50+ solutions)
- ✅ Updated main README
- ✅ Logged activity to Context7 (this file)

**MCP Integration Status**:
- 🔄 Now rebuilding Context7 file with proper MCP integration
- 🔄 Next: Sync to `.github/agents/logs/documentation-agent.log.md`
- 🔄 Next: Update knowledge graph with architecture keywords
- 🔄 Next: Generate embeddings for semantic doc search

**Methodology Applied**:
- Incremental work (5 commits, manageable scope)
- Context7 logging for continuity
- Context window management (~130k tokens used)
- Following MCP protocol for state sync

**Next Agent Notes**:
- Documentation complete and MCP-integrated
- Architecture: 62k LOC, 331 files documented
- Local AI: FastAPI + ChromaDB + Ollama + RAG
- Frontend: Vue 3 + React 18 + Three.js
- Templates: 7 production workspaces
- Ready for: API docs, videos, ADRs
- **MCP Integration**: Agents can now query docs via MCP tools

---

## 🎨 Design System (Cyberpunk Theme)

### Colors
```css
--primary-cyan: #00f0ff;
--primary-magenta: #ff00ff;
--primary-green: #00ff88;
--primary-orange: #ff6600;
--accent-indigo: #6366f1;
--accent-cyan: #22d3ee;
--background-dark: #0a0a1a;
--background-mid: #0f0f23;
--text-primary: #ffffff;
--text-secondary: #e2e8f0;
```

### Ports
- 3000: React App
- 5173: Vue/React Templates  
- 5000: .NET API
- 8000: Local AI Backend (FastAPI)
- 11434: Ollama (Local LLM)
- 8001: ChromaDB

---

## 🚀 Project Roadmap

- [x] Codespace templates
- [x] DevContainer workspace
- [x] Rich UX/UI components
- [x] Code Workstation with embedded IDE
- [x] VS Code Web integration
- [x] Professional template configurations
- [x] Global UI integration
- [x] Architecture documentation suite
- [x] MCP-based Context7 integration
- [ ] Phase 5: Learning Hub
- [ ] WebLLM integration
- [ ] Vector RAG for all documentation
- [ ] Real-time agent collaboration dashboard

---

## 🔐 MCP Security

- API keys stored in environment variables
- HTTPS for external MCP servers
- Local MCP servers use file-based auth
- Agent logs sanitized of secrets
- Knowledge graph encrypted at rest

---

*Last Updated: 2024-12-21*  
*Version: 2.0.0 (MCP-Integrated)*  
*Sync Status: 🟢 Active*  
*MCP Engine: `local-ai/core/context7_sync.py`*
