"""
DevTeam6 Local AI - Embedding Service

Generates text embeddings using Ollama or OpenAI models.
"""

import asyncio
from typing import List, Optional, Union
import httpx
from tenacity import retry, stop_after_attempt, wait_exponential

from config.settings import get_settings


class EmbeddingService:
    """Service for generating text embeddings."""

    def __init__(
        self,
        model: Optional[str] = None,
        provider: str = "ollama",
        dimensions: Optional[int] = None,
    ):
        """
        Initialize the embedding service.

        Args:
            model: Model name for embeddings
            provider: Provider to use ("ollama" or "openai")
            dimensions: Expected embedding dimensions
        """
        settings = get_settings()
        self.model = model or settings.embedding_model
        self.provider = provider
        self.dimensions = dimensions or settings.embedding_dimensions
        self.ollama_host = settings.ollama_host
        self.openai_key = settings.openai_api_key
        self._client: Optional[httpx.AsyncClient] = None

    async def _get_client(self) -> httpx.AsyncClient:
        """Get or create HTTP client."""
        if self._client is None:
            self._client = httpx.AsyncClient(timeout=60.0)
        return self._client

    async def close(self) -> None:
        """Close the HTTP client."""
        if self._client:
            await self._client.aclose()
            self._client = None

    @retry(stop=stop_after_attempt(3), wait=wait_exponential(multiplier=1, min=1, max=10))
    async def embed(self, text: str) -> List[float]:
        """
        Generate embedding for a single text.

        Args:
            text: Text to embed

        Returns:
            List of floats representing the embedding
        """
        if self.provider == "ollama":
            return await self._embed_ollama(text)
        elif self.provider == "openai":
            return await self._embed_openai(text)
        else:
            raise ValueError(f"Unknown provider: {self.provider}")

    async def embed_batch(self, texts: List[str]) -> List[List[float]]:
        """
        Generate embeddings for multiple texts.

        Args:
            texts: List of texts to embed

        Returns:
            List of embedding vectors
        """
        settings = get_settings()
        batch_size = settings.embedding_batch_size

        embeddings = []
        for i in range(0, len(texts), batch_size):
            batch = texts[i : i + batch_size]
            batch_embeddings = await asyncio.gather(
                *[self.embed(text) for text in batch]
            )
            embeddings.extend(batch_embeddings)

        return embeddings

    async def _embed_ollama(self, text: str) -> List[float]:
        """Generate embedding using Ollama."""
        client = await self._get_client()

        response = await client.post(
            f"{self.ollama_host}/api/embeddings",
            json={
                "model": self.model,
                "prompt": text,
            },
        )
        response.raise_for_status()
        data = response.json()

        return data["embedding"]

    async def _embed_openai(self, text: str) -> List[float]:
        """Generate embedding using OpenAI."""
        if not self.openai_key:
            raise ValueError("OpenAI API key not configured")

        client = await self._get_client()

        response = await client.post(
            "https://api.openai.com/v1/embeddings",
            headers={"Authorization": f"Bearer {self.openai_key}"},
            json={
                "model": "text-embedding-3-small",
                "input": text,
            },
        )
        response.raise_for_status()
        data = response.json()

        return data["data"][0]["embedding"]

    def similarity(self, embedding1: List[float], embedding2: List[float]) -> float:
        """
        Calculate cosine similarity between two embeddings.

        Args:
            embedding1: First embedding vector
            embedding2: Second embedding vector

        Returns:
            Cosine similarity score (0-1)
        """
        import numpy as np

        vec1 = np.array(embedding1)
        vec2 = np.array(embedding2)

        dot_product = np.dot(vec1, vec2)
        norm1 = np.linalg.norm(vec1)
        norm2 = np.linalg.norm(vec2)

        if norm1 == 0 or norm2 == 0:
            return 0.0

        return float(dot_product / (norm1 * norm2))
