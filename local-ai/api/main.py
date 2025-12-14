"""
DevTeam6 Local AI - FastAPI Application

Main API server for the Local AI system.
"""

from typing import Dict, Any, List, Optional
from contextlib import asynccontextmanager
from fastapi import FastAPI, HTTPException, Depends
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

from config.settings import get_settings

# Global service instances (singleton pattern)
_embedding_service = None
_memory_system = None
_rag_pipeline = None
_context7_sync = None


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


# Dependency injection functions (return pre-initialized singletons)
async def get_embedding_service():
    """Get embedding service singleton (initialized at startup)."""
    if _embedding_service is None:
        raise RuntimeError("Service not initialized. Application startup may have failed.")
    return _embedding_service


async def get_memory_system():
    """Get memory system singleton (initialized at startup)."""
    if _memory_system is None:
        raise RuntimeError("Service not initialized. Application startup may have failed.")
    return _memory_system


async def get_rag_pipeline():
    """Get RAG pipeline singleton (initialized at startup)."""
    if _rag_pipeline is None:
        raise RuntimeError("Service not initialized. Application startup may have failed.")
    return _rag_pipeline


async def get_context7_sync():
    """Get Context7 sync singleton (initialized at startup)."""
    if _context7_sync is None:
        raise RuntimeError("Service not initialized. Application startup may have failed.")
    return _context7_sync


# Application lifespan
@asynccontextmanager
async def lifespan(app: FastAPI):
    """
    Application lifespan handler.
    
    Initializes all service singletons at startup and cleans them up on shutdown.
    This ensures thread-safe initialization before any requests are processed.
    """
    # Startup
    settings = get_settings()
    print(f"🚀 Starting {settings.app_name} v{settings.app_version}")
    print(f"📊 ChromaDB: {settings.chroma_persist_dir}")
    print(f"🤖 Ollama: {settings.ollama_host}")
    
    # Initialize all singletons (thread-safe, runs before request handling)
    global _embedding_service, _memory_system, _rag_pipeline, _context7_sync
    
    try:
        from core.embedding_service import EmbeddingService
        from core.memory_system import MemorySystem
        from core.rag_pipeline import RAGPipeline
        from core.context7_sync import Context7Sync
        
        _embedding_service = EmbeddingService()
        _memory_system = MemorySystem()
        _rag_pipeline = RAGPipeline()
        _context7_sync = Context7Sync()
        await _context7_sync.load()
        
        print("✅ Services initialized successfully")
    except Exception as e:
        print(f"❌ Service initialization failed: {e}")
        raise

    yield

    # Shutdown - cleanup resources
    print("👋 Shutting down...")
    if _embedding_service:
        await _embedding_service.close()
    if _memory_system:
        await _memory_system.close()
    if _rag_pipeline:
        await _rag_pipeline.close()
    if _context7_sync:
        await _context7_sync.save()
    print("✅ Resources cleaned up")


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
async def generate_embedding(
    request: EmbedRequest,
    service = Depends(get_embedding_service)
):
    """Generate embedding for text."""
    try:
        embedding = await service.embed(request.text)

        return EmbedResponse(
            embedding=embedding,
            dimensions=len(embedding),
            model=request.model or get_settings().embedding_model,
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Store endpoint
@app.post("/store", response_model=StoreResponse)
async def store_content(
    request: StoreRequest,
    memory = Depends(get_memory_system)
):
    """Store content in vector database."""
    try:
        doc_id = await memory.store(
            content=request.content,
            category=request.category,
            metadata=request.metadata,
        )

        return StoreResponse(doc_id=doc_id, status="stored")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Query endpoint
@app.post("/query", response_model=QueryResponse)
async def query_content(
    request: QueryRequest,
    memory = Depends(get_memory_system)
):
    """Query content using semantic search."""
    try:
        results = await memory.query(
            query=request.query,
            top_k=request.top_k,
            category=request.category,
            score_threshold=request.score_threshold,
        )

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
async def rag_generate(
    request: RAGRequest,
    rag = Depends(get_rag_pipeline)
):
    """Generate response using RAG."""
    try:
        response = await rag.generate(
            query=request.query,
            context_sources=request.context_sources,
            top_k=request.top_k,
            system_prompt=request.system_prompt,
        )

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
async def sync_agent_state(
    request: AgentSyncRequest,
    sync = Depends(get_context7_sync)
):
    """Sync agent state with Context7."""
    try:
        await sync.update_agent_state(request.agent_id, request.updates)
        await sync.save()

        return {"status": "synced", "agent_id": request.agent_id}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# Stats endpoint
@app.get("/stats")
async def get_stats(memory = Depends(get_memory_system)):
    """Get system statistics."""
    try:
        from core.knowledge_graph import KnowledgeGraph

        kg = KnowledgeGraph()

        return {
            "memory": memory.get_stats(),
            "knowledge_graph": kg.get_stats(),
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
