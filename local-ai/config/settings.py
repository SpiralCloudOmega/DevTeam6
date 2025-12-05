"""
DevTeam6 Local AI - Settings Configuration

Manages environment variables and application settings using Pydantic.
"""

from functools import lru_cache
from typing import Optional
from pydantic_settings import BaseSettings
from pydantic import Field


class Settings(BaseSettings):
    """Application settings loaded from environment variables."""

    # Application
    app_name: str = "DevTeam6 Local AI"
    app_version: str = "1.0.0"
    debug: bool = False

    # API Server
    api_host: str = "0.0.0.0"
    api_port: int = 8000
    api_reload: bool = True

    # Ollama Configuration
    ollama_host: str = "http://localhost:11434"
    ollama_model: str = "llama3.2"
    ollama_timeout: int = 120

    # Embedding Configuration
    embedding_model: str = "nomic-embed-text"
    embedding_dimensions: int = 768
    embedding_batch_size: int = 32

    # ChromaDB Configuration
    chroma_persist_dir: str = "./data/chroma"
    chroma_collection: str = "devteam6"
    chroma_distance_fn: str = "cosine"

    # OpenAI Configuration (optional)
    openai_api_key: Optional[str] = None
    openai_model: str = "gpt-4"

    # Anthropic Configuration (optional)
    anthropic_api_key: Optional[str] = None
    anthropic_model: str = "claude-3-opus-20240229"

    # Context7 Configuration
    context7_path: str = "../.github/agents/context7.agents.md"
    context7_sync_interval: int = 60  # seconds

    # Memory Configuration
    memory_max_tokens: int = 4096
    memory_chunk_size: int = 512
    memory_chunk_overlap: int = 50

    # RAG Configuration
    rag_top_k: int = 5
    rag_score_threshold: float = 0.7
    rag_rerank: bool = True

    # Logging
    log_level: str = "INFO"
    log_format: str = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"

    # Theme Colors (Cyberpunk)
    theme_primary: str = "#00f0ff"
    theme_secondary: str = "#ff00ff"
    theme_accent: str = "#00ff88"

    class Config:
        env_file = ".env"
        env_file_encoding = "utf-8"
        case_sensitive = False


@lru_cache()
def get_settings() -> Settings:
    """Get cached settings instance."""
    return Settings()
