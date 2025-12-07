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
        
    Performance: O(n + c*k) where n is text length, c is number of chunks,
    and k is average overlap size. Effectively linear for typical use cases.
    """
    if not text:
        return []

    chunks = []
    chunk_start_position = 0
    chunk_index = 0
    text_length = len(text)
    separator_length = len(separator)

    while chunk_start_position < text_length:
        # Find end of chunk
        chunk_end_position = chunk_start_position + chunk_size
        
        # Adjust if we exceed text length
        if chunk_end_position > text_length:
            chunk_end_position = text_length

        # Try to end at a separator
        if chunk_end_position < text_length:
            # Look for separator near end (search backwards from end)
            separator_position = text.rfind(separator, chunk_start_position, chunk_end_position)
            if separator_position > chunk_start_position:
                chunk_end_position = separator_position + separator_length
            else:
                # Fall back to space (search backwards)
                space_position = text.rfind(" ", chunk_start_position, chunk_end_position)
                if space_position > chunk_start_position:
                    chunk_end_position = space_position + 1

        # Get chunk content
        chunk_content = text[chunk_start_position:chunk_end_position].strip()

        if chunk_content:
            chunks.append(
                TextChunk(
                    content=chunk_content,
                    index=chunk_index,
                    start_char=chunk_start_position,
                    end_char=chunk_end_position,
                    metadata={
                        "chunk_index": chunk_index,
                        "total_chunks": -1,  # Set after
                    },
                )
            )
            chunk_index += 1

        # Move start with overlap
        chunk_start_position = chunk_end_position - chunk_overlap
        if chunk_start_position <= 0 or chunk_start_position >= text_length:
            break

    # Update total chunks in single pass
    total_chunks = len(chunks)
    for chunk in chunks:
        chunk.metadata["total_chunks"] = total_chunks

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
    sorted_chunks = sorted(chunks, key=lambda chunk: chunk.index)
    return separator.join(chunk.content for chunk in sorted_chunks)
