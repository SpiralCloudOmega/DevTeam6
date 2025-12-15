# ⚡ DevTeam6 Quick Reference

> Fast access to commands, endpoints, and patterns for daily development

---

## 🚀 Common Commands

### Local Development

```bash
# Start everything (from root)
npm run dev              # Frontend (port 5173)
npm run dev:projects     # Vue projects (port 5174)
cd local-ai && uvicorn api.main:app --reload  # Backend (port 8000)
```

### Ollama Operations

```bash
# Pull models
ollama pull llama3.2
ollama pull nomic-embed-text

# List models
ollama list

# Run model
ollama run llama3.2 "Your prompt here"

# Check service
curl http://localhost:11434/api/tags
```

### Local AI API

```bash
# Health check
curl http://localhost:8000/health

# Generate embedding
curl -X POST http://localhost:8000/embed \
  -H "Content-Type: application/json" \
  -d '{"text": "Your text here"}'

# Store in vector DB
curl -X POST http://localhost:8000/store \
  -H "Content-Type: application/json" \
  -d '{"content": "Your content", "metadata": {"category": "docs"}}'

# Query with RAG
curl -X POST http://localhost:8000/rag \
  -H "Content-Type: application/json" \
  -d '{"query": "How to use DevTeam6?"}'

# Semantic search
curl -X POST http://localhost:8000/query \
  -H "Content-Type: application/json" \
  -d '{"query": "React components", "top_k": 5}'
```

---

## 📁 Project Structure

```
DevTeam6/
├── app/                  # React 18 + Three.js workstation (port 3000/5173)
│   ├── src/pages/       # Dashboard, NodeGraph, Repomind, etc.
│   └── src/components/  # Reusable UI components
│
├── projects/            # Vue 3 workflow editor (port 5174)
│   └── src/            # Vue components and composables
│
├── local-ai/           # Python FastAPI backend (port 8000)
│   ├── api/            # REST endpoints
│   ├── core/           # RAG pipeline, embeddings, memory
│   ├── mcp/            # Model Context Protocol servers
│   └── services/       # Ollama, ChromaDB services
│
├── templates/          # Dev workspace starters
│   ├── react-starter/  # React 18 + TypeScript + Vite
│   ├── vue3-starter/   # Vue 3 + Composition API
│   ├── cpp-workspace/  # C++20 + CMake + OpenGL
│   └── ...            # 7 total templates
│
├── context7/           # Agent context sharing
│   └── agents.md       # Agent registry and notes
│
└── docs/              # Documentation
    ├── architecture/   # System design docs
    ├── awesome-copilot/ # Integration guides
    └── *.md           # Various guides
```

---

## 🎨 Design System

### Colors (Cyberpunk Theme)

```css
/* Primary Colors */
--primary-cyan: #00f0ff;
--primary-magenta: #ff00ff;
--primary-green: #00ff88;
--primary-orange: #ff6600;
--primary-yellow: #ffcc00;
--primary-purple: #aa00ff;

/* Background */
--bg-primary: #0a0a1a;
--bg-secondary: #0d0d2a;
--bg-accent: #1a0a2e;

/* Text */
--text-primary: #ffffff;
--text-secondary: #e2e8f0;
```

### Typography

```css
/* Headings */
font-family: 'Fira Code', 'JetBrains Mono', monospace;

/* Body */
font-family: 'Segoe UI', 'Inter', sans-serif;
```

---

## 🔌 API Endpoints

### Local AI Backend (http://localhost:8000)

| Endpoint | Method | Purpose |
|----------|--------|---------|
| `/health` | GET | Health check |
| `/embed` | POST | Generate text embeddings |
| `/store` | POST | Store content in vector DB |
| `/query` | POST | Semantic search |
| `/rag` | POST | RAG-powered generation |
| `/agents/sync` | POST | Sync agent state |
| `/mcp/*` | * | MCP server endpoints |

### Frontend Routes

| Route | Component | Purpose |
|-------|-----------|---------|
| `/` | Dashboard | Main landing page |
| `/node-graph` | NodeGraphEditor | Workflow builder |
| `/knowledge-hub` | SemanticKnowledgeHub | Knowledge graph |
| `/genui` | GenUIPlayground | AI component generator |
| `/repomind` | Repomind | Repository intelligence |
| `/control-deck` | ControlDeck | Mission control |
| `/templates` | Templates | Workspace starters |
| `/roadmap` | ProjectRoadmap | Project planning |

---

## 💻 Code Snippets

### Using Local AI from TypeScript

```typescript
// services/aiClient.ts
export class AIClient {
  private baseUrl = 'http://localhost:8000';
  
  async embed(text: string): Promise<number[]> {
    const res = await fetch(`${this.baseUrl}/embed`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ text })
    });
    const data = await res.json();
    return data.embedding;
  }
  
  async query(query: string, topK = 5): Promise<any[]> {
    const res = await fetch(`${this.baseUrl}/query`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ query, top_k: topK })
    });
    return res.json();
  }
  
  async rag(query: string): Promise<string> {
    const res = await fetch(`${this.baseUrl}/rag`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ query })
    });
    const data = await res.json();
    return data.response;
  }
}
```

### Using RAG Pipeline from Python

```python
from local_ai.core.rag_pipeline import RAGPipeline

# Initialize
rag = RAGPipeline()

# Generate with context
response = await rag.generate(
    query="How do I use React hooks?",
    context_sources=["react-docs"],
    top_k=5
)

print(response)
```

### Memory System

```python
from local_ai.core.memory_system import MemorySystem

# Initialize
memory = MemorySystem()

# Store knowledge
await memory.store(
    content="React hooks allow functional components to use state",
    metadata={
        "category": "react",
        "type": "documentation",
        "source": "react-docs"
    }
)

# Query
results = await memory.query(
    query="React state management",
    top_k=5,
    filter={"category": "react"}
)
```

---

## 🧪 Testing

```bash
# Python tests
cd local-ai
pytest tests/ -v

# Frontend tests
npm test

# E2E tests (if configured)
npm run test:e2e
```

---

## 🐛 Debugging

### Enable Debug Logging

**Backend**:
```python
# local-ai/api/main.py
import logging
logging.basicConfig(level=logging.DEBUG)
```

**Frontend**:
```typescript
// Set in .env or localStorage
localStorage.setItem('DEBUG', 'true');
```

### Check Logs

```bash
# Backend logs
cd local-ai
tail -f logs/app.log

# Ollama logs
journalctl -u ollama -f

# Docker logs
docker-compose logs -f local-ai
```

---

## 📦 Package Management

```bash
# Update frontend dependencies
npm update

# Update Python dependencies
cd local-ai
pip install -U -r requirements.txt

# Check for outdated packages
npm outdated
pip list --outdated
```

---

## 🔐 Environment Setup

### Required Environment Variables

**Local AI** (`.env`):
```bash
OLLAMA_HOST=http://localhost:11434
OLLAMA_MODEL=llama3.2
EMBEDDING_MODEL=nomic-embed-text
EMBEDDING_DIMENSIONS=768
CHROMA_PERSIST_DIR=./data/chroma
CHROMA_COLLECTION=devteam6
API_HOST=0.0.0.0
API_PORT=8000
```

**Frontend** (`.env`):
```bash
VITE_API_URL=http://localhost:8000
VITE_OLLAMA_URL=http://localhost:11434
```

---

## 🚨 Troubleshooting Quick Fixes

### Port Already in Use

```bash
# Find process using port
lsof -i :8000
# Kill process
kill -9 <PID>
```

### ChromaDB Connection Error

```bash
# Reset ChromaDB
rm -rf local-ai/data/chroma
cd local-ai
python -c "from core.memory_system import MemorySystem; import asyncio; asyncio.run(MemorySystem().initialize())"
```

### Ollama Not Responding

```bash
# Restart Ollama
pkill ollama
ollama serve
```

### Frontend Build Errors

```bash
# Clear cache and reinstall
rm -rf node_modules package-lock.json
npm install
```

---

## 📚 Quick Links

- [Architecture Overview](architecture/SYSTEM_OVERVIEW.md)
- [AI Orchestration](architecture/AI_ORCHESTRATION.md)
- [Awesome Copilot Integration](awesome-copilot/INTEGRATION_GUIDE.md)
- [Deployment Guide](DEPLOYMENT_GUIDE.md)
- [Local AI README](../local-ai/README.md)
- [Main README](../README.md)

---

## 🎯 Keyboard Shortcuts (Frontend)

| Shortcut | Action |
|----------|--------|
| `Ctrl+K` | Command palette |
| `Ctrl+/` | Toggle sidebar |
| `Ctrl+B` | Toggle navigation |
| `Esc` | Close modals |

---

## 🔄 Git Workflow

```bash
# Create feature branch
git checkout -b feature/your-feature

# Make changes
git add .
git commit -m "feat: your feature"

# Push
git push origin feature/your-feature

# Update Context7
# Edit context7/agents.md with your changes
```

---

*Quick reference for DevTeam6 development - Keep this handy!*
