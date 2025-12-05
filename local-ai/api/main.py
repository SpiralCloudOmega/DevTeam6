"""
DevTeam6 Local AI - FastAPI Application

Main API server for the Local AI system.
"""

from typing import Dict, Any, List, Optional
from contextlib import asynccontextmanager
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

from config.settings import get_settings


# Request/Response Models
class EmbedRequest(BaseModel):
    """Request for embedding generation."""
    text: str
    model: Optional[str] = None


class EmbedResponse(BaseModel):
    """Response with embedding."""
    embedding: List[float]
    dimensions: int
    model: str


class StoreRequest(BaseModel):
    """Request to store content."""
    content: str
    category: str = "general"
    metadata: Optional[Dict[str, Any]] = None


class StoreResponse(BaseModel):
    """Response after storing."""
    doc_id: str
    status: str


class QueryRequest(BaseModel):
    """Request for semantic search."""
    query: str
    top_k: int = 5
    category: Optional[str] = None
    score_threshold: float = 0.7


class SearchResult(BaseModel):
    """Single search result."""
    id: str
    content: str
    score: float
    metadata: Dict[str, Any]


class QueryResponse(BaseModel):
    """Response with search results."""
    results: List[SearchResult]
    count: int


class RAGRequest(BaseModel):
    """Request for RAG generation."""
    query: str
    context_sources: Optional[List[str]] = None
    top_k: int = 5
    system_prompt: Optional[str] = None


class RAGResponse(BaseModel):
    """Response from RAG."""
    answer: str
    sources: List[SearchResult]
    model: str
    tokens_used: int


class AgentSyncRequest(BaseModel):
    """Request to sync agent state."""
    agent_id: str
    updates: Dict[str, Any]


class HealthResponse(BaseModel):
    """Health check response."""
    status: str
    version: str
    memory_count: int


# Application lifespan
@asynccontextmanager
async def lifespan(app: FastAPI):
    """Application lifespan handler."""
    # Startup
    settings = get_settings()
    print(f"🚀 Starting {settings.app_name} v{settings.app_version}")
    print(f"📊 ChromaDB: {settings.chroma_persist_dir}")
    print(f"🤖 Ollama: {settings.ollama_host}")

    yield

    # Shutdown
    print("👋 Shutting down...")


# Create FastAPI app
app = FastAPI(
    title="DevTeam6 Local AI",
    description="Vector RAG Foundation - Self-hosted AI memory system",
    version="1.0.0",
    lifespan=lifespan,
)

# CORS middleware
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Configure for production
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


# Health endpoint
@app.get("/health", response_model=HealthResponse)
async def health_check():
    """Health check endpoint."""
    settings = get_settings()
    return HealthResponse(
        status="healthy",
        version=settings.app_version,
        memory_count=0,  # Will be updated when memory system is connected
    )


@app.get("/")
async def root():
    """Root endpoint."""
    return {
        "name": "DevTeam6 Local AI",
        "version": "1.0.0",
        "description": "Vector RAG Foundation",
        "endpoints": {
            "health": "/health",
            "embed": "/embed",
            "store": "/store",
            "query": "/query",
            "rag": "/rag",
            "agents_sync": "/agents/sync",
        },
        "theme": {
            "primary": "#00f0ff",
            "secondary": "#ff00ff",
            "accent": "#00ff88",
        },
    }


# Embedding endpoint
@app.post("/embed", response_model=EmbedResponse)
async def generate_embedding(request: EmbedRequest):
    """Generate embedding for text."""
    try:
        # Import here to avoid circular imports
        from core.embedding_service import EmbeddingService

        service = EmbeddingService()
        embedding = await service.embed(request.text)
        await service.close()

        return EmbedResponse(
            embedding=embedding,
            dimensions=len(embedding),
            model=request.model or get_settings().embedding_model,
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Store endpoint
@app.post("/store", response_model=StoreResponse)
async def store_content(request: StoreRequest):
    """Store content in vector database."""
    try:
        from core.memory_system import MemorySystem

        memory = MemorySystem()
        doc_id = await memory.store(
            content=request.content,
            category=request.category,
            metadata=request.metadata,
        )
        await memory.close()

        return StoreResponse(doc_id=doc_id, status="stored")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Query endpoint
@app.post("/query", response_model=QueryResponse)
async def query_content(request: QueryRequest):
    """Query content using semantic search."""
    try:
        from core.memory_system import MemorySystem

        memory = MemorySystem()
        results = await memory.query(
            query=request.query,
            top_k=request.top_k,
            category=request.category,
            score_threshold=request.score_threshold,
        )
        await memory.close()

        return QueryResponse(
            results=[
                SearchResult(
                    id=r.id,
                    content=r.content,
                    score=r.score,
                    metadata=r.metadata,
                )
                for r in results
            ],
            count=len(results),
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# RAG endpoint
@app.post("/rag", response_model=RAGResponse)
async def rag_generate(request: RAGRequest):
    """Generate response using RAG."""
    try:
        from core.rag_pipeline import RAGPipeline

        rag = RAGPipeline()
        response = await rag.generate(
            query=request.query,
            context_sources=request.context_sources,
            top_k=request.top_k,
            system_prompt=request.system_prompt,
        )
        await rag.close()

        return RAGResponse(
            answer=response.answer,
            sources=[
                SearchResult(
                    id=s.id,
                    content=s.content,
                    score=s.score,
                    metadata=s.metadata,
                )
                for s in response.sources
            ],
            model=response.model,
            tokens_used=response.tokens_used,
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Agent sync endpoint
@app.post("/agents/sync")
async def sync_agent_state(request: AgentSyncRequest):
    """Sync agent state with Context7."""
    try:
        from core.context7_sync import Context7Sync

        sync = Context7Sync()
        await sync.load()
        await sync.update_agent_state(request.agent_id, request.updates)
        await sync.save()

        return {"status": "synced", "agent_id": request.agent_id}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Stats endpoint
@app.get("/stats")
async def get_stats():
    """Get system statistics."""
    try:
        from core.memory_system import MemorySystem
        from core.knowledge_graph import KnowledgeGraph

        memory = MemorySystem()
        kg = KnowledgeGraph()

        return {
            "memory": memory.get_stats(),
            "knowledge_graph": kg.get_stats(),
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
