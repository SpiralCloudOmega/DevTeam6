"""
Benchmark script to demonstrate performance improvements.

Run with: python benchmark_improvements.py
"""
import time
import sys
from pathlib import Path

# Add local-ai to path
sys.path.insert(0, str(Path(__file__).parent / "local-ai"))

def benchmark_chunking():
    """Benchmark text chunking performance."""
    from utils.chunking import chunk_text
    
    # Generate large text
    large_text = "This is a test sentence. " * 10000  # ~250KB
    
    print("\n=== Text Chunking Benchmark ===")
    print(f"Text size: {len(large_text):,} characters")
    
    # Warm up
    chunk_text(large_text)
    
    # Benchmark
    iterations = 10
    start = time.perf_counter()
    for _ in range(iterations):
        chunks = chunk_text(large_text, chunk_size=512, chunk_overlap=50)
    end = time.perf_counter()
    
    avg_time = (end - start) / iterations * 1000
    print(f"Average time: {avg_time:.2f}ms per chunking")
    print(f"Chunks created: {len(chunks)}")
    print(f"Performance: {len(large_text) / (avg_time / 1000) / 1000:.1f} KB/s")

def benchmark_knowledge_graph():
    """Benchmark knowledge graph operations."""
    from core.knowledge_graph import KnowledgeGraph
    import tempfile
    import os
    
    print("\n=== Knowledge Graph Benchmark ===")
    
    # Create temp file for testing
    with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
        temp_path = f.name
    
    try:
        # Test with auto_save=False (batched)
        print("\n1. Batched mode (auto_save=False):")
        graph = KnowledgeGraph(path=temp_path, auto_save=False)
        
        operations = 100
        start = time.perf_counter()
        for i in range(operations):
            graph.add_node(f"node_{i}", "test", f"Node {i}")
        for i in range(operations - 1):
            graph.add_edge(f"node_{i}", f"node_{i+1}", "connects")
        graph.save()
        end = time.perf_counter()
        
        batched_time = (end - start) * 1000
        print(f"Time for {operations} nodes + {operations-1} edges: {batched_time:.2f}ms")
        print(f"Average per operation: {batched_time / (operations * 2):.3f}ms")
        
        # Test with auto_save=True (immediate)
        os.remove(temp_path)
        print("\n2. Immediate mode (auto_save=True):")
        graph = KnowledgeGraph(path=temp_path, auto_save=True)
        
        # Use fewer operations for immediate mode (would be too slow)
        small_ops = 10
        start = time.perf_counter()
        for i in range(small_ops):
            graph.add_node(f"node_{i}", "test", f"Node {i}")
        for i in range(small_ops - 1):
            graph.add_edge(f"node_{i}", f"node_{i+1}", "connects")
        end = time.perf_counter()
        
        immediate_time = (end - start) * 1000
        print(f"Time for {small_ops} nodes + {small_ops-1} edges: {immediate_time:.2f}ms")
        print(f"Average per operation: {immediate_time / (small_ops * 2):.3f}ms")
        
        # Calculate speedup
        normalized_immediate = (immediate_time / (small_ops * 2)) * (operations * 2)
        speedup = normalized_immediate / batched_time
        print(f"\n💡 Batched mode is {speedup:.1f}x faster!")
        
    finally:
        if os.path.exists(temp_path):
            os.remove(temp_path)

def benchmark_cache_eviction():
    """Benchmark cache eviction strategy."""
    import heapq
    import random
    
    print("\n=== Cache Eviction Benchmark ===")
    
    # Create large cache
    cache_size = 1000
    cache = {
        f"key_{i}": (f"value_{i}", time.time() + random.random())
        for i in range(cache_size)
    }
    
    # Old method: full sort
    print("\n1. Old method (full sort):")
    start = time.perf_counter()
    sorted_entries = sorted(cache.items(), key=lambda x: x[1][1])
    to_remove = len(sorted_entries) // 10
    old_keys = [key for key, _ in sorted_entries[:to_remove]]
    end = time.perf_counter()
    old_time = (end - start) * 1000
    print(f"Time to find {to_remove} oldest: {old_time:.3f}ms")
    
    # New method: heapq
    print("\n2. New method (heapq.nsmallest):")
    start = time.perf_counter()
    to_remove = len(cache) // 10
    oldest = heapq.nsmallest(
        to_remove,
        ((timestamp, key) for key, (_, timestamp) in cache.items()),
        key=lambda x: x[0]
    )
    new_keys = [key for _, key in oldest]
    end = time.perf_counter()
    new_time = (end - start) * 1000
    print(f"Time to find {to_remove} oldest: {new_time:.3f}ms")
    
    speedup = old_time / new_time
    print(f"\n💡 New method is {speedup:.1f}x faster!")

if __name__ == "__main__":
    print("=" * 60)
    print("Performance Improvements Benchmark")
    print("=" * 60)
    
    try:
        benchmark_chunking()
    except Exception as e:
        print(f"Chunking benchmark failed: {e}")
    
    try:
        benchmark_knowledge_graph()
    except Exception as e:
        print(f"Knowledge graph benchmark failed: {e}")
    
    try:
        benchmark_cache_eviction()
    except Exception as e:
        print(f"Cache eviction benchmark failed: {e}")
    
    print("\n" + "=" * 60)
    print("✓ Benchmarks completed!")
    print("=" * 60)
