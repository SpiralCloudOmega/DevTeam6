# 🧠 Context7 Multi-Agent System

> **DevTeam6 Shared Context File**
> *"One repo. Multiple agents. Infinite possibilities."*

---

## 📋 Repository Overview

| Field | Value |
|-------|-------|
| **Name** | DevTeam6 - The Omega Tool Kit |
| **Description** | Ultimate Software Development Encyclopedia + AI Development Platform |
| **Tech Stack** | React, TypeScript, Three.js, Vite, Node.js |
| **Theme** | Cyberpunk (Cyan `#00f0ff`, Magenta `#ff00ff`, Green `#00ff88`) |
| **Status** | Active Development |

---

## 🤖 Agent Registry

| Agent ID | Name | Role | Status | Rules File |
|----------|------|------|--------|------------|
| `@master` | Master Orchestrator | Coordinates all agents, manages context | 🟢 Active | `rules/master.rules.md` |
| `@react` | React Specialist | React/TypeScript/Three.js development | 🟢 Active | `rules/react.rules.md` |
| `@python` | Python Specialist | Python/ML/Data Science development | 🟢 Active | `rules/python.rules.md` |
| `@devops` | DevOps Engineer | Infrastructure/CI-CD/Deployment | 🟢 Active | `rules/devops.rules.md` |
| `@research` | Research Agent | Documentation/Research/Knowledge | 🟢 Active | `rules/research.rules.md` |

---

## 📡 Inter-Agent Communication Protocol

### Message Format
```yaml
from: "@agent-id"
to: "@target-agent" | "broadcast"
type: "request" | "response" | "notification" | "handoff"
priority: "low" | "normal" | "high" | "critical"
context:
  task_id: "unique-task-identifier"
  parent_task: "parent-task-id" | null
  thread: "conversation-thread-id"
payload:
  action: "action-name"
  data: {}
  attachments: []
timestamp: "ISO-8601"
```

### Communication Channels
1. **Direct** - Agent to agent communication
2. **Broadcast** - All agents receive message
3. **Handoff** - Task transfer between agents
4. **Escalation** - Priority issue routing to @master

### Handoff Protocol
1. Sending agent summarizes current state
2. Sending agent lists pending tasks
3. Receiving agent acknowledges receipt
4. Receiving agent confirms understanding
5. Sending agent marks task as transferred

---

## 📊 Current Project State

### Active Tasks
| Task ID | Description | Assigned To | Status | Priority |
|---------|-------------|-------------|--------|----------|
| `TASK-001` | Graph utilities improvement | @react | ✅ Complete | High |
| `TASK-002` | Test coverage expansion | @react | ✅ Complete | High |
| `TASK-003` | Agents system setup | @master | 🔄 In Progress | High |

### Recent Completions
- ✅ `filterNodesByCluster` edge case handling
- ✅ `computeShortestPath` robustness improvements
- ✅ Test coverage increased from 28 to 44 tests

### Pending Work
- [ ] Vue 3 Projects Branch setup
- [ ] Vector RAG Foundation
- [ ] Local AI Infrastructure integration

---

## 🔌 API Endpoints

### Internal APIs
| Endpoint | Method | Description | Owner |
|----------|--------|-------------|-------|
| `/api/agents/status` | GET | Get all agent statuses | @master |
| `/api/agents/{id}/task` | POST | Assign task to agent | @master |
| `/api/context/sync` | POST | Sync shared context | @master |
| `/api/memory/query` | POST | Query knowledge base | @research |

### External Integrations
| Service | Purpose | Status |
|---------|---------|--------|
| GitHub API | Repository operations | 🟢 Active |
| OpenAI GPT-5.1 | Primary LLM | 🟢 Active |
| Claude Opus | Secondary LLM | 🟢 Active |
| Gemini 3 | Tertiary LLM | 🟢 Active |
| Ollama (Local) | Local inference | 🟡 Standby |

---

## 📝 Logging Protocol

### Log Levels
| Level | Color | Usage |
|-------|-------|-------|
| `DEBUG` | Gray | Detailed debugging information |
| `INFO` | Cyan `#00f0ff` | General operational messages |
| `WARN` | Yellow | Warning conditions |
| `ERROR` | Magenta `#ff00ff` | Error conditions |
| `CRITICAL` | Red | Critical failures |

### Log Format
```
[TIMESTAMP] [AGENT-ID] [LEVEL] [TASK-ID] Message
```

### Log Location
- Agent-specific logs: `logs/{agent-id}.log.md`
- Shared learnings: `logs/shared-learnings.md`

---

## 🔄 Sync Protocol

### Context Sync Schedule
| Interval | Action |
|----------|--------|
| Real-time | Critical updates broadcast |
| 5 min | Task status sync |
| 15 min | Memory consolidation |
| 1 hour | Full context refresh |

### Sync Triggers
1. **Task Completion** - Agent completes a task
2. **Handoff** - Task transferred between agents
3. **Error** - Error requiring attention
4. **Request** - Manual sync request
5. **Schedule** - Scheduled interval reached

### Conflict Resolution
1. Timestamp-based (latest wins)
2. Priority-based (higher priority wins)
3. Master arbitration (if unresolved)

---

## 🎨 Theme Reference

### Colors
```css
--primary-cyan: #00f0ff;
--primary-magenta: #ff00ff;
--primary-green: #00ff88;
--background-dark: #0a0a1a;
--background-mid: #1a0a2e;
--text-primary: #ffffff;
--text-secondary: #888888;
```

### Typography
- **Headers**: JetBrains Mono, Fira Code
- **Body**: System UI, sans-serif
- **Code**: JetBrains Mono, monospace

---

*Last Updated: 2025-12-05*
*Version: 1.0.0*
