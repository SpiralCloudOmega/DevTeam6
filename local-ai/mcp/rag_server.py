"""
DevTeam6 Local AI - RAG MCP Server

MCP server for Retrieval-Augmented Generation operations.
"""

from typing import Dict, Any, List, Optional
from .base_server import BaseMCPServer
from core.rag_pipeline import RAGPipeline


class RAGMCPServer(BaseMCPServer):
    """
    MCP server for RAG operations.

    Provides tools for:
    - Semantic search
    - RAG generation
    - Document storage
    """

    def __init__(self, rag_pipeline: Optional[RAGPipeline] = None):
        """
        Initialize the RAG MCP server.

        Args:
            rag_pipeline: RAG pipeline instance
        """
        super().__init__(name="devteam6-rag", version="1.0.0")
        self.rag = rag_pipeline or RAGPipeline()

        # Register tools
        self._register_tools()
        self._register_resources()

    def _register_tools(self) -> None:
        """Register RAG tools."""

        self.register_tool(
            name="rag_generate",
            description="Generate an answer using RAG (retrieval-augmented generation)",
            input_schema={
                "type": "object",
                "properties": {
                    "query": {
                        "type": "string",
                        "description": "Question or prompt",
                    },
                    "context_sources": {
                        "type": "array",
                        "items": {"type": "string"},
                        "description": "Filter by source categories",
                    },
                    "top_k": {
                        "type": "integer",
                        "description": "Number of context documents",
                        "default": 5,
                    },
                },
                "required": ["query"],
            },
            handler=self._rag_generate,
        )

        self.register_tool(
            name="semantic_search",
            description="Search for relevant documents using semantic similarity",
            input_schema={
                "type": "object",
                "properties": {
                    "query": {
                        "type": "string",
                        "description": "Search query",
                    },
                    "top_k": {
                        "type": "integer",
                        "description": "Number of results",
                        "default": 5,
                    },
                    "score_threshold": {
                        "type": "number",
                        "description": "Minimum similarity score",
                        "default": 0.7,
                    },
                },
                "required": ["query"],
            },
            handler=self._semantic_search,
        )

        self.register_tool(
            name="store_document",
            description="Store a document for later retrieval",
            input_schema={
                "type": "object",
                "properties": {
                    "content": {
                        "type": "string",
                        "description": "Document content",
                    },
                    "metadata": {
                        "type": "object",
                        "description": "Document metadata (title, source, etc.)",
                    },
                },
                "required": ["content"],
            },
            handler=self._store_document,
        )

        self.register_tool(
            name="store_documents_batch",
            description="Store multiple documents at once",
            input_schema={
                "type": "object",
                "properties": {
                    "documents": {
                        "type": "array",
                        "items": {
                            "type": "object",
                            "properties": {
                                "content": {"type": "string"},
                                "metadata": {"type": "object"},
                            },
                            "required": ["content"],
                        },
                        "description": "List of documents to store",
                    },
                },
                "required": ["documents"],
            },
            handler=self._store_batch,
        )

    def _register_resources(self) -> None:
        """Register RAG resources."""

        self.register_resource(
            uri="rag://config",
            name="RAG Configuration",
            description="Current RAG pipeline configuration",
        )

    async def _rag_generate(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Generate using RAG."""
        response = await self.rag.generate(
            query=args["query"],
            context_sources=args.get("context_sources"),
            top_k=args.get("top_k", 5),
        )
        return {
            "answer": response.answer,
            "sources": [
                {
                    "id": s.id,
                    "content": s.content[:200] + "..." if len(s.content) > 200 else s.content,
                    "score": s.score,
                }
                for s in response.sources
            ],
            "model": response.model,
            "tokens_used": response.tokens_used,
        }

    async def _semantic_search(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Search using semantic similarity."""
        results = await self.rag.retrieve(
            query=args["query"],
            top_k=args.get("top_k", 5),
            score_threshold=args.get("score_threshold", 0.7),
        )
        return {
            "results": [
                {
                    "id": r.id,
                    "content": r.content,
                    "score": r.score,
                    "metadata": r.metadata,
                }
                for r in results
            ],
            "count": len(results),
        }

    async def _store_document(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Store a document."""
        doc_id = await self.rag.store(
            content=args["content"],
            metadata=args.get("metadata"),
        )
        return {"doc_id": doc_id, "status": "stored"}

    async def _store_batch(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Store multiple documents."""
        documents = args["documents"]
        contents = [d["content"] for d in documents]
        metadatas = [d.get("metadata") for d in documents]

        doc_ids = await self.rag.store_batch(
            contents=contents,
            metadatas=metadatas,
        )
        return {"doc_ids": doc_ids, "count": len(doc_ids), "status": "stored"}

    async def get_resource(self, uri: str) -> Dict[str, Any]:
        """Get a resource by URI."""
        if uri == "rag://config":
            return {
                "top_k": self.rag.config.top_k,
                "score_threshold": self.rag.config.score_threshold,
                "rerank": self.rag.config.rerank,
                "max_tokens": self.rag.config.max_tokens,
            }
        return {"error": f"Resource not found: {uri}"}
