"""
DevTeam6 Local AI - Unit Tests

Tests for core functionality.
"""

import pytest
from utils.chunking import chunk_text, chunk_document, TextChunk
from utils.formatting import truncate_text, format_sources


class TestChunking:
    """Tests for text chunking utilities."""

    def test_chunk_text_empty(self):
        """Test chunking empty text."""
        result = chunk_text("")
        assert result == []

    def test_chunk_text_short(self):
        """Test chunking text shorter than chunk size."""
        text = "Hello, world!"
        result = chunk_text(text, chunk_size=100)
        assert len(result) == 1
        assert result[0].content == text

    def test_chunk_text_basic(self):
        """Test basic text chunking."""
        text = "First paragraph.\n\nSecond paragraph.\n\nThird paragraph."
        result = chunk_text(text, chunk_size=30, chunk_overlap=5)
        assert len(result) >= 2
        assert all(isinstance(c, TextChunk) for c in result)

    def test_chunk_text_metadata(self):
        """Test chunk metadata."""
        text = "A" * 100
        result = chunk_text(text, chunk_size=30, chunk_overlap=5)
        for chunk in result:
            assert "chunk_index" in chunk.metadata
            assert "total_chunks" in chunk.metadata
            assert chunk.metadata["total_chunks"] == len(result)

    def test_chunk_document(self):
        """Test document chunking with metadata."""
        content = "This is test content that will be chunked."
        result = chunk_document(
            content=content,
            title="Test Doc",
            source="test.txt",
            chunk_size=20,
        )
        assert len(result) >= 1
        assert all("content" in c for c in result)
        assert all("metadata" in c for c in result)
        assert result[0]["metadata"]["title"] == "Test Doc"


class TestFormatting:
    """Tests for formatting utilities."""

    def test_truncate_text_short(self):
        """Test truncating short text."""
        text = "Hello"
        result = truncate_text(text, 10)
        assert result == "Hello"

    def test_truncate_text_long(self):
        """Test truncating long text."""
        text = "This is a long sentence that needs to be truncated."
        result = truncate_text(text, 20)
        assert len(result) <= 20
        assert result.endswith("...")

    def test_truncate_text_word_boundary(self):
        """Test truncation at word boundary."""
        text = "Hello beautiful world"
        result = truncate_text(text, 15)
        # Should truncate at word boundary
        assert "..." in result


class TestAgents:
    """Tests for agent system."""

    def test_agent_registry_import(self):
        """Test agent registry can be imported."""
        from agents.agent_registry import AgentRegistry, get_registry
        registry = get_registry()
        assert registry is not None
        assert isinstance(registry, AgentRegistry)

    def test_agent_registry_stats(self):
        """Test registry statistics."""
        from agents.agent_registry import AgentRegistry
        registry = AgentRegistry()
        stats = registry.get_stats()
        assert "total_agents" in stats
        assert stats["total_agents"] == 0


class TestConfig:
    """Tests for configuration."""

    def test_settings_load(self):
        """Test settings can be loaded."""
        from config.settings import get_settings
        settings = get_settings()
        assert settings.app_name == "DevTeam6 Local AI"
        assert settings.api_port == 8000

    def test_model_config_defaults(self):
        """Test model config defaults."""
        from config.models import get_model_config, ModelConfig
        config = get_model_config("default")
        assert isinstance(config, ModelConfig)
        assert config.llm_provider == "ollama"
