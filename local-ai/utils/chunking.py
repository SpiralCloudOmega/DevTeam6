"""
DevTeam6 Local AI - Text Chunking Utilities

Functions for splitting text into chunks for embedding.
"""

from typing import List, Dict, Any, Optional
from dataclasses import dataclass


@dataclass
class TextChunk:
    """A chunk of text with metadata."""

    content: str
    index: int
    start_char: int
    end_char: int
    metadata: Dict[str, Any]


def chunk_text(
    text: str,
    chunk_size: int = 512,
    chunk_overlap: int = 50,
    separator: str = "\n\n",
) -> List[TextChunk]:
    """
    Split text into overlapping chunks.

    Args:
        text: Text to split
        chunk_size: Maximum characters per chunk
        chunk_overlap: Characters to overlap between chunks
        separator: Preferred split point

    Returns:
        List of TextChunk objects
        
    Performance: O(n) where n is text length. Avoids redundant string operations.
    """
    if not text:
        return []

    chunks = []
    start = 0
    index = 0
    text_len = len(text)
    sep_len = len(separator)

    while start < text_len:
        # Find end of chunk (min ensures we don't exceed text bounds)
        end = min(start + chunk_size, text_len)

        # Try to end at a separator
        if end < text_len:
            # Look for separator near end (search backwards from end)
            sep_pos = text.rfind(separator, start, end)
            if sep_pos > start:
                end = sep_pos + sep_len
            else:
                # Fall back to space (search backwards)
                space_pos = text.rfind(" ", start, end)
                if space_pos > start:
                    end = space_pos + 1

        # Get chunk content
        content = text[start:end].strip()

        if content:
            chunks.append(
                TextChunk(
                    content=content,
                    index=index,
                    start_char=start,
                    end_char=end,
                    metadata={
                        "chunk_index": index,
                        "total_chunks": -1,  # Set after
                    },
                )
            )
            index += 1

        # Move start with overlap
        start = end - chunk_overlap
        if start <= 0 or start >= text_len:
            break

    # Update total chunks in single pass
    total = len(chunks)
    for chunk in chunks:
        chunk.metadata["total_chunks"] = total

    return chunks


def chunk_document(
    content: str,
    title: Optional[str] = None,
    source: Optional[str] = None,
    chunk_size: int = 512,
    chunk_overlap: int = 50,
) -> List[Dict[str, Any]]:
    """
    Chunk a document with metadata.

    Args:
        content: Document content
        title: Document title
        source: Document source
        chunk_size: Characters per chunk
        chunk_overlap: Overlap between chunks

    Returns:
        List of chunk dictionaries ready for storage
    """
    chunks = chunk_text(content, chunk_size, chunk_overlap)

    return [
        {
            "content": chunk.content,
            "metadata": {
                **chunk.metadata,
                "title": title,
                "source": source,
                "start_char": chunk.start_char,
                "end_char": chunk.end_char,
            },
        }
        for chunk in chunks
    ]


def merge_chunks(chunks: List[TextChunk], separator: str = "\n\n") -> str:
    """
    Merge chunks back into a single text.

    Args:
        chunks: List of chunks
        separator: Separator between chunks

    Returns:
        Merged text
    """
    # Sort by index
    sorted_chunks = sorted(chunks, key=lambda c: c.index)
    return separator.join(c.content for c in sorted_chunks)
