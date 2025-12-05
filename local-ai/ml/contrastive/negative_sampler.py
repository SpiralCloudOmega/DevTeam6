"""
DevTeam6 Negative Sampler

Intelligent negative sampling for contrastive learning.
"""

from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Tuple, Any
import random
import math


@dataclass
class NegativeSamplerConfig:
    """Configuration for negative sampling."""
    num_negatives: int = 5
    strategy: str = "hard"  # random, hard, curriculum
    use_cache: bool = True
    cache_size: int = 10000


class NegativeSampler:
    """
    Intelligent negative sampler for contrastive learning.
    
    Provides various strategies for selecting informative negatives
    that improve embedding quality.
    """
    
    def __init__(self, config: Optional[NegativeSamplerConfig] = None):
        self.config = config or NegativeSamplerConfig()
        
        # Sample pool
        self.sample_pool: List[List[float]] = []
        self.sample_labels: List[int] = []
        
        # Cache for efficient sampling
        self.cache: Dict[int, List[int]] = {}
        
        # Statistics
        self.sample_count = 0
        self.hard_negative_count = 0
    
    def add_samples(
        self,
        embeddings: List[List[float]],
        labels: List[int]
    ) -> None:
        """
        Add samples to the pool.
        
        Args:
            embeddings: Embedding vectors
            labels: Class labels
        """
        for emb, label in zip(embeddings, labels):
            if len(self.sample_pool) >= self.config.cache_size:
                # Remove oldest samples
                self.sample_pool.pop(0)
                self.sample_labels.pop(0)
            
            self.sample_pool.append(emb)
            self.sample_labels.append(label)
        
        # Invalidate cache
        self.cache.clear()
    
    def sample_negatives(
        self,
        anchor: List[float],
        anchor_label: int,
        num_negatives: Optional[int] = None
    ) -> List[List[float]]:
        """
        Sample negative examples for anchor.
        
        Args:
            anchor: Anchor embedding
            anchor_label: Anchor's class label
            num_negatives: Number of negatives to sample
            
        Returns:
            List of negative embeddings
        """
        n = num_negatives or self.config.num_negatives
        
        if self.config.strategy == "random":
            return self._random_sample(anchor_label, n)
        elif self.config.strategy == "hard":
            return self._hard_sample(anchor, anchor_label, n)
        elif self.config.strategy == "curriculum":
            return self._curriculum_sample(anchor, anchor_label, n)
        else:
            return self._random_sample(anchor_label, n)
    
    def _get_negative_indices(self, anchor_label: int) -> List[int]:
        """Get indices of samples with different labels."""
        if self.config.use_cache and anchor_label in self.cache:
            return self.cache[anchor_label]
        
        neg_indices = [
            i for i, label in enumerate(self.sample_labels)
            if label != anchor_label
        ]
        
        if self.config.use_cache:
            self.cache[anchor_label] = neg_indices
        
        return neg_indices
    
    def _random_sample(
        self,
        anchor_label: int,
        num_negatives: int
    ) -> List[List[float]]:
        """Random negative sampling."""
        neg_indices = self._get_negative_indices(anchor_label)
        
        if not neg_indices:
            return []
        
        selected = random.sample(neg_indices, min(num_negatives, len(neg_indices)))
        self.sample_count += len(selected)
        
        return [self.sample_pool[i] for i in selected]
    
    def _hard_sample(
        self,
        anchor: List[float],
        anchor_label: int,
        num_negatives: int
    ) -> List[List[float]]:
        """Hard negative sampling: select closest negatives."""
        neg_indices = self._get_negative_indices(anchor_label)
        
        if not neg_indices:
            return []
        
        # Calculate distances to all negatives
        distances = []
        for idx in neg_indices:
            dist = self._euclidean_distance(anchor, self.sample_pool[idx])
            distances.append((idx, dist))
        
        # Sort by distance and select closest
        distances.sort(key=lambda x: x[1])
        selected = [idx for idx, _ in distances[:num_negatives]]
        
        self.sample_count += len(selected)
        self.hard_negative_count += len(selected)
        
        return [self.sample_pool[i] for i in selected]
    
    def _curriculum_sample(
        self,
        anchor: List[float],
        anchor_label: int,
        num_negatives: int
    ) -> List[List[float]]:
        """
        Curriculum negative sampling.
        
        Starts with easier negatives and gradually includes harder ones
        as training progresses.
        """
        neg_indices = self._get_negative_indices(anchor_label)
        
        if not neg_indices:
            return []
        
        # Calculate distances
        distances = []
        for idx in neg_indices:
            dist = self._euclidean_distance(anchor, self.sample_pool[idx])
            distances.append((idx, dist))
        
        distances.sort(key=lambda x: x[1])
        
        # Curriculum: mix of easy and hard based on sample count
        difficulty = min(self.sample_count / 10000, 1.0)  # Progress from 0 to 1
        
        # Sample from different difficulty levels
        n_hard = int(num_negatives * difficulty)
        n_easy = num_negatives - n_hard
        
        hard_negatives = [idx for idx, _ in distances[:n_hard]]
        
        # Easy negatives from the far end
        easy_pool = distances[len(distances)//2:]
        if easy_pool:
            easy_negatives = random.sample(
                [idx for idx, _ in easy_pool],
                min(n_easy, len(easy_pool))
            )
        else:
            easy_negatives = []
        
        selected = hard_negatives + easy_negatives
        self.sample_count += len(selected)
        
        return [self.sample_pool[i] for i in selected]
    
    def _euclidean_distance(self, a: List[float], b: List[float]) -> float:
        """Calculate Euclidean distance."""
        return math.sqrt(sum((ai - bi) ** 2 for ai, bi in zip(a, b)))
    
    def sample_batch(
        self,
        anchors: List[List[float]],
        anchor_labels: List[int]
    ) -> List[List[List[float]]]:
        """
        Sample negatives for batch of anchors.
        
        Args:
            anchors: List of anchor embeddings
            anchor_labels: List of anchor labels
            
        Returns:
            List of negative lists for each anchor
        """
        return [
            self.sample_negatives(a, l)
            for a, l in zip(anchors, anchor_labels)
        ]
    
    def get_stats(self) -> Dict[str, Any]:
        """Get sampler statistics."""
        return {
            "pool_size": len(self.sample_pool),
            "unique_labels": len(set(self.sample_labels)),
            "total_samples": self.sample_count,
            "hard_negative_ratio": (
                self.hard_negative_count / self.sample_count
                if self.sample_count > 0 else 0
            ),
            "strategy": self.config.strategy,
            "cache_size": len(self.cache)
        }
    
    def clear(self) -> None:
        """Clear sample pool and cache."""
        self.sample_pool.clear()
        self.sample_labels.clear()
        self.cache.clear()
        self.sample_count = 0
        self.hard_negative_count = 0
