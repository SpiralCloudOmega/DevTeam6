"""
DevTeam6 Local AI - RAG Pipeline

Retrieval-Augmented Generation pipeline for context-aware responses.
"""

from typing import List, Dict, Any, Optional
from dataclasses import dataclass
import httpx

from config.settings import get_settings
from config.models import RAGConfig
from .embedding_service import EmbeddingService
from .vector_store import VectorStore, SearchResult


@dataclass
class RAGResponse:
    """Response from the RAG pipeline."""

    answer: str
    sources: List[SearchResult]
    query: str
    context: str
    model: str
    tokens_used: int


class RAGPipeline:
    """Retrieval-Augmented Generation pipeline."""

    def __init__(
        self,
        embedding_service: Optional[EmbeddingService] = None,
        vector_store: Optional[VectorStore] = None,
        config: Optional[RAGConfig] = None,
    ):
        """
        Initialize the RAG pipeline.

        Args:
            embedding_service: Embedding service for queries
            vector_store: Vector store for retrieval
            config: RAG configuration
        """
        self.embedding_service = embedding_service or EmbeddingService()
        self.vector_store = vector_store or VectorStore()
        self.config = config or RAGConfig()
        self.settings = get_settings()
        self._client: Optional[httpx.AsyncClient] = None

    async def _get_client(self) -> httpx.AsyncClient:
        """Get or create HTTP client."""
        if self._client is None:
            self._client = httpx.AsyncClient(timeout=120.0)
        return self._client

    async def close(self) -> None:
        """Close resources."""
        if self._client:
            await self._client.aclose()
            self._client = None
        await self.embedding_service.close()

    async def retrieve(
        self,
        query: str,
        top_k: Optional[int] = None,
        score_threshold: Optional[float] = None,
        where: Optional[Dict[str, Any]] = None,
    ) -> List[SearchResult]:
        """
        Retrieve relevant documents for a query.

        Args:
            query: Search query
            top_k: Number of results
            score_threshold: Minimum similarity score
            where: Metadata filter

        Returns:
            List of relevant documents
        """
        top_k = top_k or self.config.top_k
        score_threshold = score_threshold or self.config.score_threshold

        # Generate query embedding
        query_embedding = await self.embedding_service.embed(query)

        # Search vector store
        results = self.vector_store.query(
            embedding=query_embedding,
            top_k=top_k,
            where=where,
        )

        # Filter by score threshold
        results = [r for r in results if r.score >= score_threshold]

        return results

    async def generate(
        self,
        query: str,
        context_sources: Optional[List[str]] = None,
        top_k: Optional[int] = None,
        system_prompt: Optional[str] = None,
    ) -> RAGResponse:
        """
        Generate a response using RAG.

        Args:
            query: User query
            context_sources: Optional filter by source category
            top_k: Number of context documents
            system_prompt: Optional system prompt override

        Returns:
            RAGResponse with answer and sources
        """
        # Build metadata filter
        where = None
        if context_sources:
            where = {"source": {"$in": context_sources}}

        # Retrieve relevant documents
        sources = await self.retrieve(query, top_k=top_k, where=where)

        # Build context from sources
        context_parts = []
        for i, source in enumerate(sources, 1):
            context_parts.append(f"[{i}] {source.content}")
        context = "\n\n".join(context_parts)

        # Format prompt
        prompt = self.config.context_template.format(
            context=context or "No relevant context found.",
            question=query,
        )

        # Generate response
        answer, tokens_used = await self._generate_llm(prompt, system_prompt)

        # Add source citations if enabled
        if self.config.cite_sources and sources:
            citations = "\n\nSources:\n" + "\n".join(
                f"[{i}] {s.metadata.get('title', s.id)}"
                for i, s in enumerate(sources, 1)
            )
            answer += citations

        return RAGResponse(
            answer=answer,
            sources=sources,
            query=query,
            context=context,
            model=self.settings.ollama_model,
            tokens_used=tokens_used,
        )

    async def _generate_llm(
        self,
        prompt: str,
        system_prompt: Optional[str] = None,
    ) -> tuple[str, int]:
        """
        Generate response using LLM.

        Args:
            prompt: User prompt
            system_prompt: System prompt

        Returns:
            Tuple of (response text, tokens used)
        """
        client = await self._get_client()

        # Use Ollama by default
        response = await client.post(
            f"{self.settings.ollama_host}/api/generate",
            json={
                "model": self.settings.ollama_model,
                "prompt": prompt,
                "system": system_prompt or "You are a helpful AI assistant for DevTeam6.",
                "stream": False,
                "options": {
                    "temperature": self.config.temperature,
                    "num_predict": self.config.max_tokens,
                },
            },
        )
        response.raise_for_status()
        data = response.json()

        return data["response"], data.get("eval_count", 0)

    async def store(
        self,
        content: str,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> str:
        """
        Store content in the vector store for later retrieval.

        Args:
            content: Content to store
            metadata: Optional metadata

        Returns:
            Document ID
        """
        embedding = await self.embedding_service.embed(content)
        doc_id = self.vector_store.add(
            content=content,
            embedding=embedding,
            metadata=metadata,
        )
        return doc_id

    async def store_batch(
        self,
        contents: List[str],
        metadatas: Optional[List[Dict[str, Any]]] = None,
    ) -> List[str]:
        """
        Store multiple contents in the vector store.

        Args:
            contents: List of contents
            metadatas: Optional list of metadata dicts

        Returns:
            List of document IDs
        """
        embeddings = await self.embedding_service.embed_batch(contents)
        doc_ids = self.vector_store.add_batch(
            contents=contents,
            embeddings=embeddings,
            metadatas=metadatas,
        )
        return doc_ids
