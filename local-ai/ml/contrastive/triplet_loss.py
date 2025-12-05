"""
DevTeam6 Triplet Loss

Triplet loss implementation for contrastive learning.
"""

from dataclasses import dataclass
from typing import List, Optional, Tuple
import math


@dataclass
class TripletConfig:
    """Configuration for triplet loss."""
    margin: float = 0.5
    mining_strategy: str = "hard"  # hard, semi-hard, random
    squared: bool = False


class TripletLoss:
    """
    Triplet loss for learning embeddings.
    
    Encourages anchor-positive pairs to be closer than
    anchor-negative pairs by at least a margin.
    """
    
    def __init__(self, config: Optional[TripletConfig] = None):
        self.config = config or TripletConfig()
        self.margin = self.config.margin
        self.mining_strategy = self.config.mining_strategy
        
        # History
        self.loss_history: List[float] = []
        self.violation_count = 0
    
    def _euclidean_distance(self, a: List[float], b: List[float]) -> float:
        """Calculate Euclidean distance."""
        dist_sq = sum((ai - bi) ** 2 for ai, bi in zip(a, b))
        if self.config.squared:
            return dist_sq
        return math.sqrt(dist_sq)
    
    def _cosine_distance(self, a: List[float], b: List[float]) -> float:
        """Calculate cosine distance (1 - cosine similarity)."""
        dot = sum(ai * bi for ai, bi in zip(a, b))
        norm_a = math.sqrt(sum(ai ** 2 for ai in a))
        norm_b = math.sqrt(sum(bi ** 2 for bi in b))
        
        if norm_a == 0 or norm_b == 0:
            return 1.0
        
        similarity = dot / (norm_a * norm_b)
        return 1.0 - similarity
    
    def calculate_loss(
        self,
        anchor: List[float],
        positive: List[float],
        negative: List[float],
        distance_fn: str = "euclidean"
    ) -> float:
        """
        Calculate triplet loss.
        
        Args:
            anchor: Anchor embedding
            positive: Positive embedding (same class as anchor)
            negative: Negative embedding (different class)
            distance_fn: Distance function ("euclidean" or "cosine")
            
        Returns:
            Triplet loss value
        """
        if distance_fn == "cosine":
            dist_pos = self._cosine_distance(anchor, positive)
            dist_neg = self._cosine_distance(anchor, negative)
        else:
            dist_pos = self._euclidean_distance(anchor, positive)
            dist_neg = self._euclidean_distance(anchor, negative)
        
        # Triplet loss with margin
        loss = max(0, dist_pos - dist_neg + self.margin)
        
        # Track violations (when loss > 0, constraint is violated)
        if loss > 0:
            self.violation_count += 1
        
        self.loss_history.append(loss)
        return loss
    
    def calculate_batch_loss(
        self,
        anchors: List[List[float]],
        positives: List[List[float]],
        negatives: List[List[float]],
        distance_fn: str = "euclidean"
    ) -> Tuple[float, List[float]]:
        """
        Calculate batch triplet loss.
        
        Args:
            anchors: List of anchor embeddings
            positives: List of positive embeddings
            negatives: List of negative embeddings
            distance_fn: Distance function
            
        Returns:
            Tuple of (mean_loss, individual_losses)
        """
        losses = []
        for a, p, n in zip(anchors, positives, negatives):
            loss = self.calculate_loss(a, p, n, distance_fn)
            losses.append(loss)
        
        mean_loss = sum(losses) / len(losses) if losses else 0
        return mean_loss, losses
    
    def mine_triplets(
        self,
        embeddings: List[List[float]],
        labels: List[int]
    ) -> List[Tuple[int, int, int]]:
        """
        Mine triplets based on strategy.
        
        Args:
            embeddings: All embeddings
            labels: Class labels for each embedding
            
        Returns:
            List of (anchor_idx, positive_idx, negative_idx) tuples
        """
        triplets = []
        n = len(embeddings)
        
        for i in range(n):
            anchor_label = labels[i]
            
            # Find positives (same label)
            positives = [j for j in range(n) if j != i and labels[j] == anchor_label]
            # Find negatives (different label)
            negatives = [j for j in range(n) if labels[j] != anchor_label]
            
            if not positives or not negatives:
                continue
            
            if self.mining_strategy == "hard":
                triplet = self._hard_mining(embeddings, i, positives, negatives)
            elif self.mining_strategy == "semi-hard":
                triplet = self._semi_hard_mining(embeddings, i, positives, negatives)
            else:
                triplet = self._random_mining(i, positives, negatives)
            
            if triplet:
                triplets.append(triplet)
        
        return triplets
    
    def _hard_mining(
        self,
        embeddings: List[List[float]],
        anchor_idx: int,
        positives: List[int],
        negatives: List[int]
    ) -> Optional[Tuple[int, int, int]]:
        """Hard negative mining: furthest positive, closest negative."""
        anchor = embeddings[anchor_idx]
        
        # Furthest positive
        max_pos_dist = -1
        hard_pos = positives[0]
        for p_idx in positives:
            dist = self._euclidean_distance(anchor, embeddings[p_idx])
            if dist > max_pos_dist:
                max_pos_dist = dist
                hard_pos = p_idx
        
        # Closest negative
        min_neg_dist = float('inf')
        hard_neg = negatives[0]
        for n_idx in negatives:
            dist = self._euclidean_distance(anchor, embeddings[n_idx])
            if dist < min_neg_dist:
                min_neg_dist = dist
                hard_neg = n_idx
        
        return (anchor_idx, hard_pos, hard_neg)
    
    def _semi_hard_mining(
        self,
        embeddings: List[List[float]],
        anchor_idx: int,
        positives: List[int],
        negatives: List[int]
    ) -> Optional[Tuple[int, int, int]]:
        """Semi-hard mining: negative that is closer than positive but still outside margin."""
        import random
        anchor = embeddings[anchor_idx]
        pos_idx = random.choice(positives)
        pos_dist = self._euclidean_distance(anchor, embeddings[pos_idx])
        
        # Find semi-hard negatives
        semi_hard = []
        for n_idx in negatives:
            neg_dist = self._euclidean_distance(anchor, embeddings[n_idx])
            if pos_dist < neg_dist < pos_dist + self.margin:
                semi_hard.append(n_idx)
        
        if semi_hard:
            neg_idx = random.choice(semi_hard)
        else:
            neg_idx = random.choice(negatives)
        
        return (anchor_idx, pos_idx, neg_idx)
    
    def _random_mining(
        self,
        anchor_idx: int,
        positives: List[int],
        negatives: List[int]
    ) -> Tuple[int, int, int]:
        """Random triplet selection."""
        import random
        return (anchor_idx, random.choice(positives), random.choice(negatives))
    
    def get_stats(self) -> dict:
        """Get loss statistics."""
        return {
            "total_triplets": len(self.loss_history),
            "mean_loss": sum(self.loss_history) / len(self.loss_history) if self.loss_history else 0,
            "violation_rate": self.violation_count / len(self.loss_history) if self.loss_history else 0,
            "margin": self.margin,
            "mining_strategy": self.mining_strategy
        }
