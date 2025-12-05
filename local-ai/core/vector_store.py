"""
DevTeam6 Local AI - Vector Store

ChromaDB wrapper for persistent vector storage and semantic search.
"""

from typing import List, Dict, Any, Optional
from dataclasses import dataclass
import uuid

try:
    import chromadb
    from chromadb.config import Settings as ChromaSettings
except ImportError:
    chromadb = None

from config.settings import get_settings


@dataclass
class SearchResult:
    """Result from a vector search query."""

    id: str
    content: str
    metadata: Dict[str, Any]
    score: float


class VectorStore:
    """ChromaDB vector store for semantic search."""

    def __init__(
        self,
        collection_name: Optional[str] = None,
        persist_directory: Optional[str] = None,
    ):
        """
        Initialize the vector store.

        Args:
            collection_name: Name of the ChromaDB collection
            persist_directory: Directory for persistent storage
        """
        if chromadb is None:
            raise ImportError("chromadb is required. Install with: pip install chromadb")

        settings = get_settings()
        self.collection_name = collection_name or settings.chroma_collection
        self.persist_directory = persist_directory or settings.chroma_persist_dir

        # Initialize ChromaDB client
        self._client = chromadb.PersistentClient(
            path=self.persist_directory,
            settings=ChromaSettings(
                anonymized_telemetry=False,
                allow_reset=True,
            ),
        )

        # Get or create collection
        self._collection = self._client.get_or_create_collection(
            name=self.collection_name,
            metadata={"hnsw:space": "cosine"},
        )

    @property
    def count(self) -> int:
        """Get the number of items in the collection."""
        return self._collection.count()

    def add(
        self,
        content: str,
        embedding: List[float],
        metadata: Optional[Dict[str, Any]] = None,
        doc_id: Optional[str] = None,
    ) -> str:
        """
        Add a document to the vector store.

        Args:
            content: Document content
            embedding: Embedding vector
            metadata: Optional metadata dict
            doc_id: Optional document ID

        Returns:
            Document ID
        """
        doc_id = doc_id or str(uuid.uuid4())
        metadata = metadata or {}
        metadata["content_length"] = len(content)

        self._collection.add(
            ids=[doc_id],
            embeddings=[embedding],
            documents=[content],
            metadatas=[metadata],
        )

        return doc_id

    def add_batch(
        self,
        contents: List[str],
        embeddings: List[List[float]],
        metadatas: Optional[List[Dict[str, Any]]] = None,
        ids: Optional[List[str]] = None,
    ) -> List[str]:
        """
        Add multiple documents to the vector store.

        Args:
            contents: List of document contents
            embeddings: List of embedding vectors
            metadatas: Optional list of metadata dicts
            ids: Optional list of document IDs

        Returns:
            List of document IDs
        """
        if len(contents) != len(embeddings):
            raise ValueError("Contents and embeddings must have same length")

        ids = ids or [str(uuid.uuid4()) for _ in contents]
        metadatas = metadatas or [{} for _ in contents]

        # Add content length to metadata
        for i, content in enumerate(contents):
            metadatas[i]["content_length"] = len(content)

        self._collection.add(
            ids=ids,
            embeddings=embeddings,
            documents=contents,
            metadatas=metadatas,
        )

        return ids

    def query(
        self,
        embedding: List[float],
        top_k: int = 5,
        where: Optional[Dict[str, Any]] = None,
        where_document: Optional[Dict[str, Any]] = None,
    ) -> List[SearchResult]:
        """
        Query the vector store for similar documents.

        Args:
            embedding: Query embedding vector
            top_k: Number of results to return
            where: Optional metadata filter
            where_document: Optional document content filter

        Returns:
            List of SearchResult objects
        """
        results = self._collection.query(
            query_embeddings=[embedding],
            n_results=top_k,
            where=where,
            where_document=where_document,
            include=["documents", "metadatas", "distances"],
        )

        search_results = []
        if results["ids"] and results["ids"][0]:
            for i, doc_id in enumerate(results["ids"][0]):
                # Convert distance to similarity score (cosine: 1 - distance)
                distance = results["distances"][0][i] if results["distances"] else 0
                score = 1 - distance

                search_results.append(
                    SearchResult(
                        id=doc_id,
                        content=results["documents"][0][i] if results["documents"] else "",
                        metadata=results["metadatas"][0][i] if results["metadatas"] else {},
                        score=score,
                    )
                )

        return search_results

    def delete(self, doc_ids: List[str]) -> None:
        """
        Delete documents from the vector store.

        Args:
            doc_ids: List of document IDs to delete
        """
        self._collection.delete(ids=doc_ids)

    def update(
        self,
        doc_id: str,
        content: Optional[str] = None,
        embedding: Optional[List[float]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> None:
        """
        Update a document in the vector store.

        Args:
            doc_id: Document ID
            content: New content (optional)
            embedding: New embedding (optional)
            metadata: New metadata (optional)
        """
        update_kwargs: Dict[str, Any] = {"ids": [doc_id]}

        if content is not None:
            update_kwargs["documents"] = [content]
        if embedding is not None:
            update_kwargs["embeddings"] = [embedding]
        if metadata is not None:
            update_kwargs["metadatas"] = [metadata]

        self._collection.update(**update_kwargs)

    def reset(self) -> None:
        """Reset the collection (delete all documents)."""
        self._client.delete_collection(self.collection_name)
        self._collection = self._client.create_collection(
            name=self.collection_name,
            metadata={"hnsw:space": "cosine"},
        )

    def get_by_id(self, doc_id: str) -> Optional[SearchResult]:
        """
        Get a document by ID.

        Args:
            doc_id: Document ID

        Returns:
            SearchResult or None if not found
        """
        result = self._collection.get(
            ids=[doc_id],
            include=["documents", "metadatas"],
        )

        if result["ids"]:
            return SearchResult(
                id=result["ids"][0],
                content=result["documents"][0] if result["documents"] else "",
                metadata=result["metadatas"][0] if result["metadatas"] else {},
                score=1.0,
            )

        return None
