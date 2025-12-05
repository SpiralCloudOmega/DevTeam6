"""
DevTeam6 Online Learning Module

Continuous adaptation with drift detection.
"""

from .drift_detector import DriftDetector, DriftType
from .incremental_learner import IncrementalLearner
from .adaptation_manager import AdaptationManager

__all__ = [
    "DriftDetector",
    "DriftType",
    "IncrementalLearner",
    "AdaptationManager",
]
