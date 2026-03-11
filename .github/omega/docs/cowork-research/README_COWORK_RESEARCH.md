# Open Claude Cowork Research & Integration Documentation

## 📋 Overview

This directory contains comprehensive technical research on **Open Claude Cowork** and how to integrate it with the Fulcrum Kanban + Omega Agent system.

---

## 📚 Documents

### 1. **OPEN_CLAUDE_COWORK_TECHNICAL_ANALYSIS.md** (934 lines)
**Most comprehensive document** - Deep technical analysis covering:

- **Executive Summary**: What Cowork is and how it differs from Claude Code
- **Architecture**: Full system design with diagrams, tech stack, and entry points
- **Multi-agent Support**: Current capabilities and limitations
- **Workspace Management**: Per-session isolation and database schema
- **CLI Interface**: Interaction model and event flows
- **Key APIs**: Server/Client events, session data models
- **Fulcrum Integration**: Vision, mapping, benefits, and limitations
- **Technical Deep Dives**: Agent execution flow, streaming, resumption, persistence
- **Deployment**: Build targets and packaging details
- **Appendix**: File reference and code organization

**Best for**: Understanding every aspect of Cowork, architecture decisions, and integration possibilities.

---

### 2. **QUICK_REFERENCE_COWORK.md** (211 lines)
**Quick answers to your 7 questions** - Condensed version covering:

- 1-minute explainer
- Answers to all 7 research questions with bullet points
- File structure cheat sheet
- Key insights for Omega design
- Limitations checklist
- Quick start for integration

**Best for**: Rapid lookup, sharing with team, decision-making meetings.

---

### 3. **COWORK_OMEGA_INTEGRATION_GUIDE.md** (266 lines)
**Practical implementation guide** - Step-by-step integration:

- **Phase 1**: HTTP API wrapper for Cowork
  - Express server with endpoints: `/api/sessions/start`, `/stream`, `/artifacts`
  - Server-Sent Events for real-time updates
  
- **Phase 2**: Omega integration layer
  - CoworkClient adapter (TypeScript)
  - TaskExecutor service
  - Orchestrator integration
  
- **Phase 3**: Fulcrum integration
  - Card rendering with Cowork data
  
- **Deployment**: Docker Compose setup
- **Testing**: Test scenario with code

**Best for**: Implementing the integration, copy-paste code samples, deployment setup.

---

## 🎯 Quick Navigation

### "I need to understand Cowork quickly"
→ Read **QUICK_REFERENCE_COWORK.md** (5 min read)

### "I need complete technical details"
→ Read **OPEN_CLAUDE_COWORK_TECHNICAL_ANALYSIS.md** (20 min read)

### "I need to implement the integration"
→ Read **COWORK_OMEGA_INTEGRATION_GUIDE.md** (15 min read)

### "I need to present to stakeholders"
→ Use QUICK_REFERENCE_COWORK.md + OPEN_CLAUDE_COWORK_TECHNICAL_ANALYSIS.md Executive Summary

---

## 🔑 Key Findings

### What is Open Claude Cowork?
**Desktop GUI wrapper** (Electron) around Claude Agent SDK that provides:
- Multi-session management with persistent SQLite storage
- Real-time streaming visualization of agent outputs
- Per-session workspace isolation
- API configuration management
- Session resumption support

### How Does it Differ from Claude Code?
| Aspect | Claude Code | Cowork |
|--------|-------------|--------|
| Interface | CLI (terminal) | GUI (Electron) |
| Persistence | Resumable only | Full history |
| Multi-session | Sequential | Concurrent visual tabs |
| Workspace | Current directory | Per-session configurable |

### Multi-Agent Support?
- ✅ **Multiple concurrent sessions** (each with independent agent)
- ❌ **No inter-agent communication** (isolated)
- ❌ **No task delegation** (no orchestration)

**For Omega**: Treat each Kanban task as one Cowork session (independent agents)

### Recommended Omega Integration Pattern
```
Fulcrum Kanban (React)
    ↓
Omega Orchestrator (Node.js)
    ↓ (HTTP calls)
Cowork HTTP API Service
    ├─ POST /api/sessions/start (create task)
    ├─ GET /api/sessions/:id/stream (real-time progress)
    └─ GET /api/sessions/:id/artifacts (extract outputs)
    ↓
SQLite DB + Agent Execution
```

**Benefits**:
- ✅ No Cowork modification needed
- ✅ Persistent history per task
- ✅ Real-time progress feedback
- ✅ Workspace isolation
- ✅ Artifact extraction

---

## 📊 Architecture at a Glance

```
OPEN CLAUDE COWORK

Electron Main (Node.js)
├── IPC Handlers (route events)
├── Runner (execute Claude SDK)
├── Session Store (SQLite persistence)
└── Config Store (API management)
        ↕ IPC Bridge
React UI
├── Zustand Store (state)
├── Components (sidebar, chat, modals)
└── Hooks (IPC subscription)
```

**Tech Stack**: 
- Backend: Electron 39, Claude Agent SDK 0.2.6, SQLite3
- Frontend: React 19, Zustand 5, Tailwind 4
- Build: Vite, electron-builder

---

## 🚀 Integration Roadmap

### Phase 1: HTTP Wrapper (Week 1)
- [ ] Create Express server in Cowork fork
- [ ] Implement `/api/sessions/*` endpoints
- [ ] Test with curl/Postman
- [ ] Containerize with Docker

### Phase 2: Omega Adapter (Week 2)
- [ ] Create CoworkClient TypeScript class
- [ ] Implement TaskExecutor service
- [ ] Connect to Omega orchestrator
- [ ] Handle SSE streams + artifacts

### Phase 3: Fulcrum Display (Week 2-3)
- [ ] Update card components
- [ ] Display session/artifacts on cards
- [ ] Link back to Cowork for details
- [ ] Test end-to-end workflow

### Phase 4: Production (Week 3-4)
- [ ] Docker Compose setup
- [ ] Error handling + retry logic
- [ ] Logging + monitoring
- [ ] Load testing

---

## 🔗 Key Concepts Mapped

| Cowork Concept | Omega Concept | Fulcrum Concept |
|---|---|---|
| Session | Task Execution | Card in Progress |
| Working Directory (cwd) | Task Workspace | Task Scope |
| Messages | Logs | Card Comments |
| Artifacts (files) | Deliverables | Card Attachments |
| session_id | execution_id | card_id |
| Status (idle/running/completed) | Status | Card Column |

---

## ⚠️ Known Limitations

1. **No multi-agent coordination** - Omega must handle orchestration
2. **Desktop-only** - Cowork uses Electron (requires modification for server mode)
3. **Single-user** - Each Cowork instance is per-machine
4. **No programmatic API** - Manual session creation needed
5. **Tool set hardcoded** - Only Read, Edit, Bash tools available
6. **SQLite unbounded** - No automatic DB rotation/cleanup

---

## 📝 Implementation Notes

### For the HTTP Wrapper
```typescript
// Key modifications to existing code:
// 1. Add express routes (don't remove Electron UI)
// 2. Modify runner.ts to support remote streams
// 3. Create /api/sessions/:id/artifacts endpoint
// 4. Use Server-Sent Events for real-time updates
```

### For Omega Integration
```typescript
// 1. Create CoworkClient adapter
// 2. Treat tasks as Cowork sessions (1:1 mapping)
// 3. Stream events back to Fulcrum
// 4. Extract artifacts on completion
// 5. Handle failures gracefully
```

### For Deployment
```yaml
# Use Docker Compose to orchestrate:
# - cowork (HTTP API service)
# - omega (orchestrator)
# - fulcrum (Kanban UI)
# - db (shared PostgreSQL)
```

---

## 🎓 Learning Path

If you're new to this project:

1. **Start**: QUICK_REFERENCE_COWORK.md (5 min)
2. **Deep dive**: OPEN_CLAUDE_COWORK_TECHNICAL_ANALYSIS.md sections 1-3 (15 min)
3. **Architecture**: Section 2 + diagrams (10 min)
4. **Implementation**: COWORK_OMEGA_INTEGRATION_GUIDE.md (15 min)
5. **Deployment**: Integration guide Phase 3-4 (10 min)

**Total time**: ~55 minutes for complete understanding

---

## 📞 Questions & Answers

### Q: Can Cowork handle multiple agents on the same task?
**A**: No. Each task = one session = one agent. For true multi-agent collaboration, Omega must implement the orchestration logic.

### Q: Is Cowork suitable for production?
**A**: Yes, for single-user/desktop scenarios. For Omega integration, wrap it with HTTP API (no changes to Cowork needed).

### Q: Can we use Cowork without modification?
**A**: Yes! The HTTP wrapper approach doesn't modify Cowork - just wraps it with Express. Cowork remains unchanged.

### Q: What about real-time updates to Fulcrum?
**A**: Use Server-Sent Events (SSE) from the HTTP API wrapper. Omega listens to SSE streams and pushes updates to Fulcrum via WebSocket/REST.

### Q: How do we handle task failures?
**A**: Cowork returns `session.status: "error"` on failure. Omega catches this and either retries or moves card back to backlog.

---

## 🔍 References

### In the Codebase
- **Session lifecycle**: `/src/electron/ipc-handlers.ts` (259 lines)
- **Agent execution**: `/src/electron/libs/runner.ts` (154 lines)
- **Storage**: `/src/electron/libs/session-store.ts` (269 lines)
- **UI state**: `/src/ui/store/useAppStore.ts` (200+ lines)
- **IPC bridge**: `/src/electron/preload.cts` (49 lines)

### Key Event Types
See **OPEN_CLAUDE_COWORK_TECHNICAL_ANALYSIS.md Section 6** for full event definitions:
- `stream.message` - Agent output
- `stream.user_prompt` - User input
- `session.status` - Status changes
- `permission.request` - User confirmation
- `session.deleted` - Cleanup

---

## 📦 Deliverables

All documents are in `/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6/`:

1. ✅ OPEN_CLAUDE_COWORK_TECHNICAL_ANALYSIS.md
2. ✅ QUICK_REFERENCE_COWORK.md
3. ✅ COWORK_OMEGA_INTEGRATION_GUIDE.md
4. ✅ README_COWORK_RESEARCH.md (this file)

---

**Last Updated**: 2025-02-13  
**Document Version**: 1.0  
**Scope**: Technical research for Omega + Fulcrum integration planning

