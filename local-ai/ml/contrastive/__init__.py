"""
DevTeam6 Contrastive Learning Module

Improved embeddings via triplet loss and contrastive learning.
"""

from .embedding_learner import EmbeddingLearner
from .triplet_loss import TripletLoss
from .negative_sampler import NegativeSampler

__all__ = [
    "EmbeddingLearner",
    "TripletLoss",
    "NegativeSampler",
]
