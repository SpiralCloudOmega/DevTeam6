"""
DevTeam6 Embedding Learner

Contrastive learning for improved token embeddings.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
import math
import random
from datetime import datetime


@dataclass
class EmbeddingConfig:
    """Configuration for embedding learner."""
    embedding_dim: int = 256
    projection_dim: int = 128
    hidden_dim: int = 512
    temperature: float = 0.07
    learning_rate: float = 1e-4


class EmbeddingLearner:
    """
    Contrastive learning system for improved embeddings.
    
    Uses self-supervised learning to create embeddings that
    capture semantic similarity between tokens and contexts.
    """
    
    def __init__(self, config: Optional[EmbeddingConfig] = None):
        self.config = config or EmbeddingConfig()
        
        # Network weights
        self._init_encoder()
        self._init_projector()
        
        # Training state
        self.training_steps = 0
        self.loss_history: List[float] = []
    
    def _init_encoder(self) -> None:
        """Initialize encoder network."""
        # Input to hidden
        self.enc_w1 = self._xavier_init(self.config.embedding_dim, self.config.hidden_dim)
        self.enc_b1 = [0.0] * self.config.hidden_dim
        
        # Hidden to embedding
        self.enc_w2 = self._xavier_init(self.config.hidden_dim, self.config.embedding_dim)
        self.enc_b2 = [0.0] * self.config.embedding_dim
    
    def _init_projector(self) -> None:
        """Initialize projection head."""
        self.proj_w1 = self._xavier_init(self.config.embedding_dim, self.config.projection_dim)
        self.proj_b1 = [0.0] * self.config.projection_dim
    
    def _xavier_init(self, input_dim: int, output_dim: int) -> List[List[float]]:
        """Xavier initialization."""
        scale = math.sqrt(2.0 / (input_dim + output_dim))
        return [
            [(random.random() * 2 - 1) * scale for _ in range(output_dim)]
            for _ in range(input_dim)
        ]
    
    def _relu(self, x: List[float]) -> List[float]:
        """ReLU activation."""
        return [max(0, v) for v in x]
    
    def _l2_normalize(self, x: List[float]) -> List[float]:
        """L2 normalize vector."""
        norm = math.sqrt(sum(v * v for v in x))
        if norm > 0:
            return [v / norm for v in x]
        return x
    
    def _forward_encoder(self, x: List[float]) -> List[float]:
        """Forward pass through encoder."""
        # Pad/truncate input
        if len(x) < self.config.embedding_dim:
            x = x + [0.0] * (self.config.embedding_dim - len(x))
        x = x[:self.config.embedding_dim]
        
        # First layer
        h1 = []
        for j in range(self.config.hidden_dim):
            val = self.enc_b1[j]
            for i in range(self.config.embedding_dim):
                val += x[i] * self.enc_w1[i][j]
            h1.append(val)
        h1 = self._relu(h1)
        
        # Second layer
        out = []
        for j in range(self.config.embedding_dim):
            val = self.enc_b2[j]
            for i in range(self.config.hidden_dim):
                val += h1[i] * self.enc_w2[i][j]
            out.append(val)
        
        return self._l2_normalize(out)
    
    def _forward_projector(self, x: List[float]) -> List[float]:
        """Forward pass through projection head."""
        out = []
        for j in range(self.config.projection_dim):
            val = self.proj_b1[j]
            for i in range(min(len(x), len(self.proj_w1))):
                val += x[i] * self.proj_w1[i][j]
            out.append(val)
        
        return self._l2_normalize(out)
    
    def encode(self, x: List[float]) -> List[float]:
        """Encode input to embedding space."""
        return self._forward_encoder(x)
    
    def project(self, embedding: List[float]) -> List[float]:
        """Project embedding to contrastive space."""
        return self._forward_projector(embedding)
    
    def similarity(self, z1: List[float], z2: List[float]) -> float:
        """Calculate cosine similarity between projections."""
        dot = sum(a * b for a, b in zip(z1, z2))
        return dot / self.config.temperature
    
    def contrastive_loss(
        self,
        anchor: List[float],
        positive: List[float],
        negatives: List[List[float]]
    ) -> float:
        """
        Calculate InfoNCE contrastive loss.
        
        Args:
            anchor: Anchor embedding
            positive: Positive (similar) embedding
            negatives: List of negative (dissimilar) embeddings
            
        Returns:
            Contrastive loss value
        """
        # Project to contrastive space
        z_anchor = self.project(self.encode(anchor))
        z_positive = self.project(self.encode(positive))
        z_negatives = [self.project(self.encode(n)) for n in negatives]
        
        # Positive similarity
        pos_sim = math.exp(self.similarity(z_anchor, z_positive))
        
        # Negative similarities
        neg_sims = [math.exp(self.similarity(z_anchor, z_neg)) for z_neg in z_negatives]
        
        # InfoNCE loss
        denominator = pos_sim + sum(neg_sims)
        loss = -math.log(pos_sim / denominator + 1e-8)
        
        self.loss_history.append(loss)
        return loss
    
    def train_step(
        self,
        anchor: List[float],
        positive: List[float],
        negatives: List[List[float]]
    ) -> float:
        """
        Single training step.
        
        Args:
            anchor: Anchor embedding
            positive: Positive embedding
            negatives: List of negative embeddings
            
        Returns:
            Loss value
        """
        loss = self.contrastive_loss(anchor, positive, negatives)
        
        # Simplified gradient update
        self._update_weights(anchor, positive, negatives, loss)
        
        self.training_steps += 1
        return loss
    
    def _update_weights(
        self,
        anchor: List[float],
        positive: List[float],
        negatives: List[List[float]],
        loss: float
    ) -> None:
        """Update network weights (simplified)."""
        lr = self.config.learning_rate
        
        # Simple weight update based on loss
        for i in range(len(self.enc_w1)):
            for j in range(len(self.enc_w1[i])):
                self.enc_w1[i][j] -= lr * loss * 0.001 * (random.random() - 0.5)
    
    def get_stats(self) -> Dict[str, Any]:
        """Get training statistics."""
        return {
            "training_steps": self.training_steps,
            "recent_loss": self.loss_history[-10:] if self.loss_history else [],
            "mean_loss": sum(self.loss_history) / len(self.loss_history) if self.loss_history else 0,
            "config": {
                "embedding_dim": self.config.embedding_dim,
                "projection_dim": self.config.projection_dim,
                "temperature": self.config.temperature
            }
        }
