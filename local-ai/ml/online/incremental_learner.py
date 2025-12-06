"""
DevTeam6 Incremental Learner

Online model updates without full retraining.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Tuple
import math
import random
from datetime import datetime


@dataclass
class IncrementalConfig:
    """Configuration for incremental learning."""
    update_frequency: int = 10
    forgetting_factor: float = 0.99
    memory_size: int = 1000
    learning_rate: float = 0.01
    min_samples_before_update: int = 50


class IncrementalLearner:
    """
    Incremental learner for online model updates.
    
    Maintains a sliding window of recent samples and performs
    small updates to adapt to changing patterns without
    catastrophic forgetting.
    """
    
    def __init__(self, config: Optional[IncrementalConfig] = None):
        self.config = config or IncrementalConfig()
        
        # Memory buffer
        self.memory_x: List[List[float]] = []
        self.memory_y: List[int] = []
        self.memory_weights: List[float] = []
        
        # Simple classifier weights
        self.weights: List[List[float]] = []
        self.biases: List[float] = []
        self.num_classes = 0
        self.input_dim = 0
        
        # Statistics
        self.samples_seen = 0
        self.updates_performed = 0
        self.correct_predictions = 0
    
    def initialize(self, input_dim: int, num_classes: int) -> None:
        """
        Initialize learner with dimensions.
        
        Args:
            input_dim: Input feature dimension
            num_classes: Number of output classes
        """
        self.input_dim = input_dim
        self.num_classes = num_classes
        
        # Initialize weights with small random values
        scale = math.sqrt(2.0 / (input_dim + num_classes))
        self.weights = [
            [(random.random() * 2 - 1) * scale for _ in range(num_classes)]
            for _ in range(input_dim)
        ]
        self.biases = [0.0] * num_classes
    
    def _softmax(self, x: List[float]) -> List[float]:
        """Softmax activation."""
        max_x = max(x)
        exp_x = [math.exp(xi - max_x) for xi in x]
        sum_exp = sum(exp_x)
        return [e / sum_exp for e in exp_x]
    
    def _forward(self, x: List[float]) -> List[float]:
        """Forward pass through classifier."""
        if len(x) < self.input_dim:
            x = x + [0.0] * (self.input_dim - len(x))
        x = x[:self.input_dim]
        
        logits = []
        for j in range(self.num_classes):
            val = self.biases[j]
            for i in range(self.input_dim):
                val += x[i] * self.weights[i][j]
            logits.append(val)
        
        return self._softmax(logits)
    
    def predict(self, x: List[float]) -> Tuple[int, float]:
        """
        Make prediction.
        
        Args:
            x: Input features
            
        Returns:
            Tuple of (predicted_class, confidence)
        """
        probs = self._forward(x)
        pred = probs.index(max(probs))
        return pred, probs[pred]
    
    def partial_fit(self, x: List[float], y: int) -> float:
        """
        Update model with single sample.
        
        Args:
            x: Input features
            y: True label
            
        Returns:
            Loss value
        """
        # Add to memory
        self._add_to_memory(x, y)
        self.samples_seen += 1
        
        # Make prediction
        probs = self._forward(x)
        pred = probs.index(max(probs))
        
        if pred == y:
            self.correct_predictions += 1
        
        # Calculate loss
        loss = -math.log(probs[y] + 1e-10)
        
        # Update if enough samples
        if self.samples_seen >= self.config.min_samples_before_update:
            if self.samples_seen % self.config.update_frequency == 0:
                self._update_weights(x, y, probs)
                self.updates_performed += 1
        
        return loss
    
    def _add_to_memory(self, x: List[float], y: int) -> None:
        """Add sample to memory buffer."""
        if len(self.memory_x) >= self.config.memory_size:
            # Remove oldest or lowest weight sample
            min_idx = self.memory_weights.index(min(self.memory_weights))
            self.memory_x.pop(min_idx)
            self.memory_y.pop(min_idx)
            self.memory_weights.pop(min_idx)
        
        self.memory_x.append(x)
        self.memory_y.append(y)
        self.memory_weights.append(1.0)
        
        # Apply forgetting factor to older samples
        for i in range(len(self.memory_weights) - 1):
            self.memory_weights[i] *= self.config.forgetting_factor
    
    def _update_weights(
        self,
        x: List[float],
        y: int,
        probs: List[float]
    ) -> None:
        """Update weights using gradient descent."""
        if len(x) < self.input_dim:
            x = x + [0.0] * (self.input_dim - len(x))
        x = x[:self.input_dim]
        
        # Calculate gradient (cross-entropy loss)
        lr = self.config.learning_rate
        
        for j in range(self.num_classes):
            # Gradient of cross-entropy with softmax
            grad = probs[j] - (1.0 if j == y else 0.0)
            
            # Update bias
            self.biases[j] -= lr * grad
            
            # Update weights
            for i in range(self.input_dim):
                self.weights[i][j] -= lr * grad * x[i]
    
    def replay(self, batch_size: int = 32) -> float:
        """
        Replay samples from memory.
        
        Args:
            batch_size: Number of samples to replay
            
        Returns:
            Average loss
        """
        if len(self.memory_x) < batch_size:
            batch_size = len(self.memory_x)
        
        if batch_size == 0:
            return 0.0
        
        # Sample with weights
        total_weight = sum(self.memory_weights)
        probs = [w / total_weight for w in self.memory_weights]
        
        indices = []
        for _ in range(batch_size):
            r = random.random()
            cumsum = 0
            for i, p in enumerate(probs):
                cumsum += p
                if r < cumsum:
                    indices.append(i)
                    break
            else:
                indices.append(len(probs) - 1)
        
        # Update on sampled batch
        total_loss = 0.0
        for idx in indices:
            x = self.memory_x[idx]
            y = self.memory_y[idx]
            probs = self._forward(x)
            loss = -math.log(probs[y] + 1e-10)
            total_loss += loss
            self._update_weights(x, y, probs)
        
        return total_loss / batch_size
    
    def get_accuracy(self) -> float:
        """Get running accuracy."""
        if self.samples_seen == 0:
            return 0.0
        return self.correct_predictions / self.samples_seen
    
    def get_stats(self) -> Dict[str, Any]:
        """Get learner statistics."""
        return {
            "samples_seen": self.samples_seen,
            "updates_performed": self.updates_performed,
            "accuracy": self.get_accuracy(),
            "memory_size": len(self.memory_x),
            "memory_capacity": self.config.memory_size,
            "input_dim": self.input_dim,
            "num_classes": self.num_classes
        }
    
    def reset(self) -> None:
        """Reset learner state (keep weights)."""
        self.memory_x.clear()
        self.memory_y.clear()
        self.memory_weights.clear()
        self.samples_seen = 0
        self.correct_predictions = 0
