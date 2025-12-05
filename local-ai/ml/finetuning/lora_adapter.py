"""
DevTeam6 LoRA Adapter

Low-Rank Adaptation for efficient fine-tuning.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Tuple
import math
import random
import json


@dataclass
class LoRAConfig:
    """Configuration for LoRA adapter."""
    r: int = 16  # Rank
    alpha: int = 32  # Alpha scaling
    dropout: float = 0.05
    target_modules: List[str] = field(default_factory=lambda: ["q_proj", "v_proj", "k_proj", "o_proj"])
    bias: str = "none"  # none, all, lora_only


class LoRAAdapter:
    """
    Low-Rank Adaptation (LoRA) adapter for efficient fine-tuning.
    
    Adds trainable low-rank matrices to frozen pre-trained weights,
    enabling parameter-efficient fine-tuning.
    """
    
    def __init__(self, config: Optional[LoRAConfig] = None):
        self.config = config or LoRAConfig()
        
        # Adapter weights for each target module
        self.adapters: Dict[str, Dict[str, List[List[float]]]] = {}
        
        # Training state
        self.is_trained = False
        self.training_steps = 0
        self.loss_history: List[float] = []
    
    def create_adapter(
        self,
        module_name: str,
        input_dim: int,
        output_dim: int
    ) -> None:
        """
        Create LoRA adapter for a module.
        
        Args:
            module_name: Name of the module to adapt
            input_dim: Input dimension
            output_dim: Output dimension
        """
        r = self.config.r
        
        # Initialize A and B matrices
        # A: input_dim x r (initialized with normal distribution)
        # B: r x output_dim (initialized with zeros)
        
        scale_a = math.sqrt(2.0 / (input_dim + r))
        A = [
            [(random.random() * 2 - 1) * scale_a for _ in range(r)]
            for _ in range(input_dim)
        ]
        
        B = [
            [0.0 for _ in range(output_dim)]
            for _ in range(r)
        ]
        
        self.adapters[module_name] = {
            "A": A,
            "B": B,
            "input_dim": input_dim,
            "output_dim": output_dim
        }
    
    def forward(
        self,
        module_name: str,
        x: List[float],
        base_output: Optional[List[float]] = None
    ) -> List[float]:
        """
        Apply LoRA adapter to input.
        
        Args:
            module_name: Name of the module
            x: Input vector
            base_output: Output from base model (if available)
            
        Returns:
            Adapted output
        """
        if module_name not in self.adapters:
            return base_output if base_output else x
        
        adapter = self.adapters[module_name]
        A = adapter["A"]
        B = adapter["B"]
        r = self.config.r
        
        # Apply dropout during training
        if self.is_trained and self.config.dropout > 0:
            x = [xi if random.random() > self.config.dropout else 0 for xi in x]
        
        # LoRA forward: x @ A @ B * (alpha / r)
        # First: x @ A -> h (input_dim -> r)
        h = [0.0] * r
        for j in range(r):
            for i in range(min(len(x), len(A))):
                h[j] += x[i] * A[i][j]
        
        # Second: h @ B -> out (r -> output_dim)
        output_dim = adapter["output_dim"]
        out = [0.0] * output_dim
        for j in range(output_dim):
            for i in range(r):
                out[j] += h[i] * B[i][j]
        
        # Apply scaling
        scaling = self.config.alpha / self.config.r
        out = [o * scaling for o in out]
        
        # Add to base output if provided
        if base_output is not None:
            out = [o + b for o, b in zip(out, base_output)]
        
        return out
    
    def train_step(
        self,
        module_name: str,
        x: List[float],
        target: List[float],
        learning_rate: float = 0.0002
    ) -> float:
        """
        Single training step for adapter.
        
        Args:
            module_name: Module to train
            x: Input
            target: Target output
            learning_rate: Learning rate
            
        Returns:
            Loss value
        """
        if module_name not in self.adapters:
            return 0.0
        
        # Forward pass
        output = self.forward(module_name, x)
        
        # Calculate loss (MSE)
        loss = sum(
            (o - t) ** 2 for o, t in zip(output, target)
        ) / len(target) if target else 0
        
        # Backward pass (simplified gradient descent)
        adapter = self.adapters[module_name]
        self._update_adapter(adapter, x, output, target, learning_rate)
        
        self.training_steps += 1
        self.loss_history.append(loss)
        
        return loss
    
    def _update_adapter(
        self,
        adapter: Dict,
        x: List[float],
        output: List[float],
        target: List[float],
        lr: float
    ) -> None:
        """Update adapter weights."""
        A = adapter["A"]
        B = adapter["B"]
        r = self.config.r
        output_dim = adapter["output_dim"]
        
        # Calculate output gradient
        d_out = [(o - t) * 2 / len(target) for o, t in zip(output, target)]
        
        # Calculate h (intermediate)
        h = [0.0] * r
        for j in range(r):
            for i in range(min(len(x), len(A))):
                h[j] += x[i] * A[i][j]
        
        # Update B: gradient is h^T @ d_out
        scaling = self.config.alpha / self.config.r
        for i in range(r):
            for j in range(output_dim):
                grad = h[i] * d_out[j] * scaling
                B[i][j] -= lr * grad
        
        # Update A: gradient is x^T @ (d_out @ B^T)
        d_h = [0.0] * r
        for i in range(r):
            for j in range(output_dim):
                d_h[i] += d_out[j] * B[i][j]
        
        for i in range(min(len(x), len(A))):
            for j in range(r):
                grad = x[i] * d_h[j] * scaling
                A[i][j] -= lr * grad
    
    def merge_with_base(
        self,
        base_weights: List[List[float]],
        module_name: str
    ) -> List[List[float]]:
        """
        Merge LoRA adapter with base weights.
        
        Args:
            base_weights: Original weight matrix
            module_name: Module to merge
            
        Returns:
            Merged weight matrix
        """
        if module_name not in self.adapters:
            return base_weights
        
        adapter = self.adapters[module_name]
        A = adapter["A"]
        B = adapter["B"]
        r = self.config.r
        scaling = self.config.alpha / self.config.r
        
        # Calculate A @ B
        lora_weights = []
        for i in range(len(A)):
            row = []
            for j in range(len(B[0]) if B else 0):
                val = 0
                for k in range(r):
                    val += A[i][k] * B[k][j]
                row.append(val * scaling)
            lora_weights.append(row)
        
        # Add to base weights
        merged = []
        for i in range(len(base_weights)):
            row = []
            for j in range(len(base_weights[i])):
                base_val = base_weights[i][j]
                lora_val = lora_weights[i][j] if i < len(lora_weights) and j < len(lora_weights[i]) else 0
                row.append(base_val + lora_val)
            merged.append(row)
        
        return merged
    
    def save(self, path: str) -> None:
        """Save adapter to file."""
        data = {
            "config": {
                "r": self.config.r,
                "alpha": self.config.alpha,
                "dropout": self.config.dropout,
                "target_modules": self.config.target_modules,
                "bias": self.config.bias
            },
            "adapters": self.adapters,
            "is_trained": self.is_trained,
            "training_steps": self.training_steps
        }
        with open(path, "w") as f:
            json.dump(data, f)
    
    def load(self, path: str) -> None:
        """Load adapter from file."""
        with open(path, "r") as f:
            data = json.load(f)
        
        self.config = LoRAConfig(**data["config"])
        self.adapters = data["adapters"]
        self.is_trained = data.get("is_trained", True)
        self.training_steps = data.get("training_steps", 0)
    
    def get_stats(self) -> Dict[str, Any]:
        """Get adapter statistics."""
        total_params = 0
        for adapter in self.adapters.values():
            a_params = len(adapter["A"]) * len(adapter["A"][0]) if adapter["A"] else 0
            b_params = len(adapter["B"]) * len(adapter["B"][0]) if adapter["B"] else 0
            total_params += a_params + b_params
        
        return {
            "num_adapters": len(self.adapters),
            "total_parameters": total_params,
            "rank": self.config.r,
            "alpha": self.config.alpha,
            "training_steps": self.training_steps,
            "recent_loss": self.loss_history[-10:] if self.loss_history else []
        }
