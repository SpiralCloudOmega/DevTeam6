# 🧠 DevTeam6 Local AI - Vector RAG Foundation

> **Self-hosted AI memory system with vector search and retrieval-augmented generation**

---

## 🎯 Overview

The Local AI module provides the backend infrastructure for DevTeam6's AI capabilities:

- **Vector Storage**: ChromaDB for persistent embeddings
- **Embedding Service**: Generate embeddings via Ollama or OpenAI
- **RAG Pipeline**: Retrieval-augmented generation for context-aware responses
- **Context7 Sync**: Integration with the multi-agent system
- **MCP Servers**: Model Context Protocol tools for agent integration

---

## 📁 Structure

```
local-ai/
├── config/          # Configuration management
├── core/            # Core services (memory, embeddings, RAG)
├── agents/          # Agent integration layer
├── mcp/             # Model Context Protocol servers
├── api/             # FastAPI endpoints
├── utils/           # Utility functions
└── tests/           # Test suite
```

---

## 🚀 Quick Start

### Installation

```bash
cd local-ai
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
pip install -r requirements.txt
```

### Configuration

Create a `.env` file:

```env
# Ollama Configuration
OLLAMA_HOST=http://localhost:11434
OLLAMA_MODEL=llama3.2

# Embedding Model
EMBEDDING_MODEL=nomic-embed-text
EMBEDDING_DIMENSIONS=768

# ChromaDB
CHROMA_PERSIST_DIR=./data/chroma
CHROMA_COLLECTION=devteam6

# OpenAI (optional)
OPENAI_API_KEY=your-key-here

# API
API_HOST=0.0.0.0
API_PORT=8000
```

### Running the Server

```bash
uvicorn api.main:app --reload --port 8000
```

---

## 🔧 Core Services

### Memory System

```python
from core.memory_system import MemorySystem

memory = MemorySystem()

# Store knowledge
await memory.store("React hooks guide", metadata={"category": "react"})

# Query with semantic search
results = await memory.query("How to use useState?", top_k=5)
```

### Embedding Service

```python
from core.embedding_service import EmbeddingService

embedder = EmbeddingService()
embedding = await embedder.embed("Your text here")
```

### RAG Pipeline

```python
from core.rag_pipeline import RAGPipeline

rag = RAGPipeline()
response = await rag.generate(
    query="Explain React context",
    context_sources=["documentation", "examples"]
)
```

---

## 📡 API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/health` | GET | Health check |
| `/embed` | POST | Generate embeddings |
| `/store` | POST | Store content in vector DB |
| `/query` | POST | Semantic search |
| `/rag` | POST | RAG generation |
| `/agents/sync` | POST | Sync agent state |

---

## 🎨 Cyberpunk Theme

Colors used in responses and logging:
- Primary: `#00f0ff` (Cyan)
- Secondary: `#ff00ff` (Magenta)
- Accent: `#00ff88` (Green)

---

## 🔗 Integration

### With Vue 3 Projects

The API is designed to be called from the Vue 3 workflow builder:

```typescript
// From projects/src/utils/api.ts
const response = await fetch('http://localhost:8000/rag', {
  method: 'POST',
  body: JSON.stringify({ query: 'How to...' })
})
```

### With Context7 Agents

The sync engine keeps agent state consistent:

```python
from core.context7_sync import Context7Sync

sync = Context7Sync()
await sync.update_agent_state("@react", {"current_task": "..."})
```

---

## 🧪 Testing

```bash
pytest tests/ -v --cov=core
```

---

## 📚 Dependencies

- **FastAPI**: Modern async web framework
- **ChromaDB**: Vector database with persistence
- **Ollama**: Local LLM inference
- **Pydantic**: Data validation

---

*Part of the DevTeam6 Omega Tool Kit*
