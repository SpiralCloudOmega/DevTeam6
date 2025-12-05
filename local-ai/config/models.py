"""
DevTeam6 Local AI - Model Configurations

Defines model-specific configurations for Ollama, OpenAI, and other providers.
"""

from typing import Optional, List, Dict, Any
from pydantic import BaseModel, Field


class OllamaConfig(BaseModel):
    """Configuration for Ollama models."""

    host: str = "http://localhost:11434"
    model: str = "llama3.2"
    timeout: int = 120
    
    # Generation parameters
    temperature: float = 0.7
    top_p: float = 0.9
    top_k: int = 40
    num_predict: int = 2048
    stop: Optional[List[str]] = None
    
    # System prompt
    system_prompt: Optional[str] = None


class EmbeddingConfig(BaseModel):
    """Configuration for embedding models."""

    model: str = "nomic-embed-text"
    dimensions: int = 768
    batch_size: int = 32
    normalize: bool = True
    
    # Provider: "ollama", "openai", "local"
    provider: str = "ollama"


class ChromaConfig(BaseModel):
    """Configuration for ChromaDB vector store."""

    persist_directory: str = "./data/chroma"
    collection_name: str = "devteam6"
    distance_function: str = "cosine"  # cosine, l2, ip
    
    # Metadata filtering
    where_filter: Optional[Dict[str, Any]] = None
    where_document: Optional[Dict[str, Any]] = None


class RAGConfig(BaseModel):
    """Configuration for RAG pipeline."""

    # Retrieval settings
    top_k: int = 5
    score_threshold: float = 0.7
    rerank: bool = True
    
    # Generation settings
    max_tokens: int = 2048
    temperature: float = 0.7
    
    # Context formatting
    context_template: str = """Use the following context to answer the question.

Context:
{context}

Question: {question}

Answer:"""

    # Source citation
    cite_sources: bool = True


class ModelConfig(BaseModel):
    """Master configuration for all models."""

    # Model providers
    ollama: OllamaConfig = Field(default_factory=OllamaConfig)
    embedding: EmbeddingConfig = Field(default_factory=EmbeddingConfig)
    chroma: ChromaConfig = Field(default_factory=ChromaConfig)
    rag: RAGConfig = Field(default_factory=RAGConfig)
    
    # Active provider
    llm_provider: str = "ollama"  # ollama, openai, anthropic
    embedding_provider: str = "ollama"  # ollama, openai
    
    # OpenAI settings (if used)
    openai_model: str = "gpt-4"
    openai_embedding_model: str = "text-embedding-3-small"
    
    # Anthropic settings (if used)
    anthropic_model: str = "claude-3-opus-20240229"


# Default configurations for different use cases
CONFIGS = {
    "default": ModelConfig(),
    "fast": ModelConfig(
        ollama=OllamaConfig(model="llama3.2:1b", num_predict=512),
        rag=RAGConfig(top_k=3, max_tokens=1024),
    ),
    "quality": ModelConfig(
        ollama=OllamaConfig(model="llama3.2:70b", temperature=0.5),
        rag=RAGConfig(top_k=10, rerank=True),
    ),
    "local": ModelConfig(
        llm_provider="ollama",
        embedding_provider="ollama",
    ),
    "cloud": ModelConfig(
        llm_provider="openai",
        embedding_provider="openai",
    ),
}


def get_model_config(name: str = "default") -> ModelConfig:
    """Get a predefined model configuration."""
    return CONFIGS.get(name, CONFIGS["default"])
