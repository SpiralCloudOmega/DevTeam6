"""
DevTeam6 Local AI - Formatting Utilities

Functions for formatting context and sources.
"""

from typing import List, Dict, Any
from core.vector_store import SearchResult


def format_context(
    results: List[SearchResult],
    max_length: int = 4096,
    include_scores: bool = False,
) -> str:
    """
    Format search results as context for RAG.

    Args:
        results: Search results
        max_length: Maximum context length
        include_scores: Include similarity scores

    Returns:
        Formatted context string
    """
    if not results:
        return "No relevant context found."

    parts = []
    current_length = 0

    for i, result in enumerate(results, 1):
        # Format entry
        entry = f"[{i}] {result.content}"
        if include_scores:
            entry += f" (score: {result.score:.2f})"

        # Check length
        if current_length + len(entry) > max_length:
            # Truncate if needed
            remaining = max_length - current_length
            if remaining > 100:
                entry = entry[:remaining] + "..."
                parts.append(entry)
            break

        parts.append(entry)
        current_length += len(entry) + 2

    return "\n\n".join(parts)


def format_sources(
    results: List[SearchResult],
    style: str = "numbered",
) -> str:
    """
    Format sources as citations.

    Args:
        results: Search results
        style: Citation style ("numbered", "bullet", "inline")

    Returns:
        Formatted sources string
    """
    if not results:
        return ""

    lines = ["Sources:"]

    for i, result in enumerate(results, 1):
        title = result.metadata.get("title", result.id)
        source = result.metadata.get("source", "")

        if style == "numbered":
            lines.append(f"[{i}] {title}")
            if source:
                lines.append(f"    Source: {source}")
        elif style == "bullet":
            lines.append(f"• {title}")
            if source:
                lines.append(f"  ({source})")
        elif style == "inline":
            citation = f"({title}"
            if source:
                citation += f", {source}"
            citation += ")"
            lines.append(citation)

    return "\n".join(lines)


def format_answer_with_citations(
    answer: str,
    sources: List[SearchResult],
) -> str:
    """
    Format an answer with inline citations.

    Args:
        answer: Answer text
        sources: Source documents

    Returns:
        Answer with citations appended
    """
    if not sources:
        return answer

    formatted_sources = format_sources(sources, style="numbered")
    return f"{answer}\n\n{formatted_sources}"


def truncate_text(text: str, max_length: int, suffix: str = "...") -> str:
    """
    Truncate text to maximum length.

    Args:
        text: Text to truncate
        max_length: Maximum length
        suffix: Suffix for truncated text

    Returns:
        Truncated text
    """
    if len(text) <= max_length:
        return text

    # Try to truncate at word boundary
    truncated = text[: max_length - len(suffix)]
    last_space = truncated.rfind(" ")

    if last_space > max_length // 2:
        truncated = truncated[:last_space]

    return truncated + suffix
