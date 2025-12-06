"""
DevTeam6 Local AI - Ollama Service

Simple wrapper for local LLM inference via Ollama.
"""

from typing import Dict, Any, List, Optional, AsyncGenerator
from dataclasses import dataclass
import httpx
import asyncio


@dataclass
class ChatMessage:
    """A chat message."""
    role: str  # "system", "user", "assistant"
    content: str


@dataclass
class GenerationResult:
    """Result from generation."""
    response: str
    model: str
    total_duration_ns: int
    prompt_eval_count: int
    eval_count: int
    done: bool


@dataclass
class EmbeddingResult:
    """Result from embedding generation."""
    embedding: List[float]
    model: str


class OllamaService:
    """
    Simple wrapper for Ollama API.
    
    Features:
    - Chat completions
    - Text generation
    - Embedding generation
    - Streaming support
    - Model management
    """
    
    def __init__(
        self,
        host: str = "http://localhost:11434",
        default_model: str = "llama3.2",
        timeout: int = 120
    ):
        """
        Initialize Ollama service.
        
        Args:
            host: Ollama API host
            default_model: Default model to use
            timeout: Request timeout in seconds
        """
        self.host = host.rstrip("/")
        self.default_model = default_model
        self.timeout = timeout
        self._client: Optional[httpx.AsyncClient] = None
    
    async def _get_client(self) -> httpx.AsyncClient:
        """Get or create async HTTP client."""
        if self._client is None or self._client.is_closed:
            self._client = httpx.AsyncClient(
                timeout=httpx.Timeout(self.timeout),
                limits=httpx.Limits(max_connections=10)
            )
        return self._client
    
    async def close(self) -> None:
        """Close the HTTP client."""
        if self._client and not self._client.is_closed:
            await self._client.aclose()
            self._client = None
    
    async def generate(
        self,
        prompt: str,
        model: Optional[str] = None,
        system: Optional[str] = None,
        temperature: float = 0.7,
        max_tokens: int = 2048,
        stream: bool = False
    ) -> GenerationResult:
        """
        Generate text from a prompt.
        
        Args:
            prompt: Input prompt
            model: Model to use
            system: System prompt
            temperature: Generation temperature
            max_tokens: Maximum tokens to generate
            stream: Whether to stream response
            
        Returns:
            GenerationResult
        """
        client = await self._get_client()
        
        payload = {
            "model": model or self.default_model,
            "prompt": prompt,
            "stream": False,  # Non-streaming for simplicity
            "options": {
                "temperature": temperature,
                "num_predict": max_tokens
            }
        }
        
        if system:
            payload["system"] = system
        
        response = await client.post(
            f"{self.host}/api/generate",
            json=payload
        )
        response.raise_for_status()
        data = response.json()
        
        return GenerationResult(
            response=data.get("response", ""),
            model=data.get("model", model or self.default_model),
            total_duration_ns=data.get("total_duration", 0),
            prompt_eval_count=data.get("prompt_eval_count", 0),
            eval_count=data.get("eval_count", 0),
            done=data.get("done", True)
        )
    
    async def chat(
        self,
        messages: List[ChatMessage],
        model: Optional[str] = None,
        temperature: float = 0.7,
        max_tokens: int = 2048
    ) -> GenerationResult:
        """
        Chat completion.
        
        Args:
            messages: List of chat messages
            model: Model to use
            temperature: Generation temperature
            max_tokens: Maximum tokens
            
        Returns:
            GenerationResult
        """
        client = await self._get_client()
        
        payload = {
            "model": model or self.default_model,
            "messages": [
                {"role": m.role, "content": m.content}
                for m in messages
            ],
            "stream": False,
            "options": {
                "temperature": temperature,
                "num_predict": max_tokens
            }
        }
        
        response = await client.post(
            f"{self.host}/api/chat",
            json=payload
        )
        response.raise_for_status()
        data = response.json()
        
        message = data.get("message", {})
        
        return GenerationResult(
            response=message.get("content", ""),
            model=data.get("model", model or self.default_model),
            total_duration_ns=data.get("total_duration", 0),
            prompt_eval_count=data.get("prompt_eval_count", 0),
            eval_count=data.get("eval_count", 0),
            done=data.get("done", True)
        )
    
    async def embed(
        self,
        text: str,
        model: Optional[str] = None
    ) -> EmbeddingResult:
        """
        Generate embedding for text.
        
        Args:
            text: Text to embed
            model: Embedding model
            
        Returns:
            EmbeddingResult
        """
        client = await self._get_client()
        
        # Default to embedding model
        embed_model = model or "nomic-embed-text"
        
        response = await client.post(
            f"{self.host}/api/embeddings",
            json={
                "model": embed_model,
                "prompt": text
            }
        )
        response.raise_for_status()
        data = response.json()
        
        return EmbeddingResult(
            embedding=data.get("embedding", []),
            model=embed_model
        )
    
    async def list_models(self) -> List[Dict[str, Any]]:
        """
        List available models.
        
        Returns:
            List of model info dicts
        """
        client = await self._get_client()
        
        response = await client.get(f"{self.host}/api/tags")
        response.raise_for_status()
        data = response.json()
        
        return data.get("models", [])
    
    async def pull_model(self, model: str) -> bool:
        """
        Pull a model from Ollama registry.
        
        Args:
            model: Model name to pull
            
        Returns:
            True if successful
        """
        client = await self._get_client()
        
        response = await client.post(
            f"{self.host}/api/pull",
            json={"name": model, "stream": False}
        )
        
        return response.status_code == 200
    
    async def model_info(self, model: str) -> Dict[str, Any]:
        """
        Get model information.
        
        Args:
            model: Model name
            
        Returns:
            Model info dict
        """
        client = await self._get_client()
        
        response = await client.post(
            f"{self.host}/api/show",
            json={"name": model}
        )
        response.raise_for_status()
        
        return response.json()
    
    async def health_check(self) -> bool:
        """
        Check if Ollama is running.
        
        Returns:
            True if Ollama is responsive
        """
        try:
            client = await self._get_client()
            response = await client.get(self.host)
            return response.status_code == 200
        except Exception:
            return False
    
    async def generate_with_context(
        self,
        prompt: str,
        context: str,
        model: Optional[str] = None,
        system: Optional[str] = None
    ) -> GenerationResult:
        """
        Generate with additional context (for RAG).
        
        Args:
            prompt: User prompt
            context: Retrieved context
            model: Model to use
            system: System prompt
            
        Returns:
            GenerationResult
        """
        # Build context-aware prompt
        full_prompt = f"""Use the following context to answer the question.

Context:
{context}

Question: {prompt}

Answer:"""
        
        default_system = (
            "You are a helpful AI assistant for DevTeam6. "
            "Answer questions based on the provided context. "
            "If the context doesn't contain relevant information, say so."
        )
        
        return await self.generate(
            prompt=full_prompt,
            model=model,
            system=system or default_system
        )
