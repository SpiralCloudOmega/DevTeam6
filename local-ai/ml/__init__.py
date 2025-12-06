"""
DevTeam6 Machine Learning Layer

Complete ML infrastructure for intelligent Dual Transformer routing
and self-optimizing AI systems.

Modules:
- supervised: Token classification, threat detection, intent recognition
- reinforcement: PPO-based routing optimization
- contrastive: Improved embeddings via triplet loss
- online: Continuous adaptation with drift detection
- anomaly: Security threat identification
- finetuning: LoRA/QLoRA model adapters
- meta: Self-optimizing hyperparameters
- advanced: Swarm intelligence, neuromorphic concepts
"""

from .config import MLConfig, get_ml_config

__all__ = [
    "MLConfig",
    "get_ml_config",
]

__version__ = "1.0.0"
