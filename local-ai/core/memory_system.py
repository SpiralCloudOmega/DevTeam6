"""
DevTeam6 Local AI - Memory System

Main AI memory manager that coordinates embeddings, vector storage, and retrieval.
"""

from typing import List, Dict, Any, Optional
from dataclasses import dataclass
import asyncio
import json
from datetime import datetime
from pathlib import Path

from config.settings import get_settings
from .embedding_service import EmbeddingService
from .vector_store import VectorStore, SearchResult
from .rag_pipeline import RAGPipeline


@dataclass
class MemoryEntry:
    """A single memory entry."""

    id: str
    content: str
    category: str
    metadata: Dict[str, Any]
    created_at: str
    accessed_at: str
    access_count: int


class MemorySystem:
    """
    Main AI memory manager.

    Coordinates:
    - Embedding generation
    - Vector storage
    - Semantic retrieval
    - Memory consolidation
    """

    def __init__(
        self,
        collection_name: Optional[str] = None,
        persist_directory: Optional[str] = None,
    ):
        """
        Initialize the memory system.

        Args:
            collection_name: ChromaDB collection name
            persist_directory: Directory for persistent storage
        """
        self.settings = get_settings()
        self.embedding_service = EmbeddingService()
        self.vector_store = VectorStore(
            collection_name=collection_name,
            persist_directory=persist_directory,
        )
        self.rag_pipeline = RAGPipeline(
            embedding_service=self.embedding_service,
            vector_store=self.vector_store,
        )
        self._memory_index: Dict[str, MemoryEntry] = {}

    async def close(self) -> None:
        """Close all resources."""
        await self.embedding_service.close()
        await self.rag_pipeline.close()

    @property
    def count(self) -> int:
        """Get total number of memories."""
        return self.vector_store.count

    async def store(
        self,
        content: str,
        category: str = "general",
        metadata: Optional[Dict[str, Any]] = None,
        doc_id: Optional[str] = None,
    ) -> str:
        """
        Store content in memory.

        Args:
            content: Content to store
            category: Category for organization
            metadata: Additional metadata
            doc_id: Optional document ID

        Returns:
            Document ID
        """
        metadata = metadata or {}
        metadata["category"] = category
        metadata["created_at"] = datetime.utcnow().isoformat()
        metadata["accessed_at"] = metadata["created_at"]
        metadata["access_count"] = 0

        # Generate embedding and store
        embedding = await self.embedding_service.embed(content)
        doc_id = self.vector_store.add(
            content=content,
            embedding=embedding,
            metadata=metadata,
            doc_id=doc_id,
        )

        # Update index
        self._memory_index[doc_id] = MemoryEntry(
            id=doc_id,
            content=content,
            category=category,
            metadata=metadata,
            created_at=metadata["created_at"],
            accessed_at=metadata["accessed_at"],
            access_count=0,
        )

        return doc_id

    async def query(
        self,
        query: str,
        top_k: int = 5,
        category: Optional[str] = None,
        score_threshold: float = 0.7,
    ) -> List[SearchResult]:
        """
        Query memory with semantic search.

        Args:
            query: Search query
            top_k: Number of results
            category: Filter by category
            score_threshold: Minimum similarity score

        Returns:
            List of relevant memories
        """
        # Build filter
        where = {"category": category} if category else None

        # Generate query embedding
        query_embedding = await self.embedding_service.embed(query)

        # Search
        results = self.vector_store.query(
            embedding=query_embedding,
            top_k=top_k,
            where=where,
        )

        # Filter by threshold
        results = [r for r in results if r.score >= score_threshold]

        # Update access stats
        for result in results:
            await self._update_access(result.id)

        return results

    async def ask(
        self,
        question: str,
        category: Optional[str] = None,
        top_k: int = 5,
    ) -> str:
        """
        Ask a question using RAG.

        Args:
            question: Question to answer
            category: Filter context by category
            top_k: Number of context documents

        Returns:
            Generated answer
        """
        context_sources = [category] if category else None
        response = await self.rag_pipeline.generate(
            query=question,
            context_sources=context_sources,
            top_k=top_k,
        )
        return response.answer

    async def forget(self, doc_ids: List[str]) -> None:
        """
        Remove memories.

        Args:
            doc_ids: List of document IDs to remove
        """
        self.vector_store.delete(doc_ids)
        for doc_id in doc_ids:
            self._memory_index.pop(doc_id, None)

    async def consolidate(
        self,
        category: Optional[str] = None,
        max_age_days: int = 30,
        min_access_count: int = 0,
    ) -> int:
        """
        Consolidate memories by removing old, unused entries.

        Args:
            category: Filter by category
            max_age_days: Maximum age in days
            min_access_count: Minimum access count to keep

        Returns:
            Number of memories removed
        """
        from datetime import timedelta

        cutoff = datetime.utcnow() - timedelta(days=max_age_days)
        to_remove = []

        for doc_id, entry in self._memory_index.items():
            if category and entry.category != category:
                continue

            created = datetime.fromisoformat(entry.created_at)
            if created < cutoff and entry.access_count <= min_access_count:
                to_remove.append(doc_id)

        await self.forget(to_remove)
        return len(to_remove)

    async def _update_access(self, doc_id: str) -> None:
        """Update access statistics for a memory."""
        if doc_id in self._memory_index:
            entry = self._memory_index[doc_id]
            entry.accessed_at = datetime.utcnow().isoformat()
            entry.access_count += 1

            # Update in vector store
            self.vector_store.update(
                doc_id=doc_id,
                metadata={
                    **entry.metadata,
                    "accessed_at": entry.accessed_at,
                    "access_count": entry.access_count,
                },
            )

    async def export(self, path: str) -> None:
        """
        Export memory index to file.

        Args:
            path: File path for export
        """
        export_data = {
            "exported_at": datetime.utcnow().isoformat(),
            "count": self.count,
            "memories": [
                {
                    "id": entry.id,
                    "category": entry.category,
                    "created_at": entry.created_at,
                    "accessed_at": entry.accessed_at,
                    "access_count": entry.access_count,
                }
                for entry in self._memory_index.values()
            ],
        }

        Path(path).write_text(json.dumps(export_data, indent=2))

    def get_stats(self) -> Dict[str, Any]:
        """Get memory system statistics."""
        categories: Dict[str, int] = {}
        total_accesses = 0

        for entry in self._memory_index.values():
            categories[entry.category] = categories.get(entry.category, 0) + 1
            total_accesses += entry.access_count

        return {
            "total_memories": self.count,
            "categories": categories,
            "total_accesses": total_accesses,
            "index_size": len(self._memory_index),
        }
