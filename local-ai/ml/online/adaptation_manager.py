"""
DevTeam6 Adaptation Manager

Coordinates model adaptation based on drift detection.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable
from datetime import datetime
from enum import Enum

from .drift_detector import DriftDetector, DriftStatus, DriftConfig
from .incremental_learner import IncrementalLearner, IncrementalConfig


class AdaptationStrategy(Enum):
    """Model adaptation strategies."""
    INCREMENTAL = "incremental"  # Small updates only
    PARTIAL_RETRAIN = "partial_retrain"  # Retrain on recent data
    FULL_RETRAIN = "full_retrain"  # Complete retraining
    ENSEMBLE_UPDATE = "ensemble_update"  # Add new model to ensemble


@dataclass
class AdaptationConfig:
    """Configuration for adaptation manager."""
    strategy: str = "incremental"
    adaptation_rate: float = 0.1
    warning_patience: int = 10
    drift_cooldown: int = 100
    auto_adapt: bool = True


@dataclass
class AdaptationEvent:
    """Record of an adaptation action."""
    strategy: AdaptationStrategy
    trigger: str
    samples_used: int
    performance_before: float
    performance_after: float
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class AdaptationManager:
    """
    Manages model adaptation in response to concept drift.
    
    Coordinates drift detection, incremental learning, and
    model retraining to maintain performance over time.
    """
    
    def __init__(
        self,
        drift_config: Optional[DriftConfig] = None,
        learner_config: Optional[IncrementalConfig] = None,
        adapt_config: Optional[AdaptationConfig] = None
    ):
        self.drift_detector = DriftDetector(drift_config)
        self.incremental_learner = IncrementalLearner(learner_config)
        self.config = adapt_config or AdaptationConfig()
        
        # State
        self.warning_count = 0
        self.samples_since_drift = 0
        self.in_cooldown = False
        
        # Callbacks
        self.on_drift_detected: Optional[Callable] = None
        self.on_adaptation_complete: Optional[Callable] = None
        
        # History
        self.adaptation_events: List[AdaptationEvent] = []
        self.performance_history: List[float] = []
    
    def initialize_learner(self, input_dim: int, num_classes: int) -> None:
        """Initialize the incremental learner."""
        self.incremental_learner.initialize(input_dim, num_classes)
    
    def process_sample(
        self,
        x: List[float],
        y_true: int,
        y_pred: int
    ) -> Dict[str, Any]:
        """
        Process a new sample through the adaptation pipeline.
        
        Args:
            x: Input features
            y_true: True label
            y_pred: Predicted label
            
        Returns:
            Status information
        """
        # Update drift detector
        error = 0 if y_pred == y_true else 1
        drift_status = self.drift_detector.update(error)
        
        # Update counters
        self.samples_since_drift += 1
        
        # Handle cooldown
        if self.in_cooldown:
            if self.samples_since_drift >= self.config.drift_cooldown:
                self.in_cooldown = False
        
        # React to drift
        action_taken = None
        if not self.in_cooldown:
            if drift_status == DriftStatus.WARNING:
                self.warning_count += 1
                if self.warning_count >= self.config.warning_patience:
                    action_taken = self._adapt(AdaptationStrategy.INCREMENTAL, "sustained_warning")
            elif drift_status == DriftStatus.DETECTED:
                action_taken = self._handle_drift()
                self.warning_count = 0
        
        # Always do incremental update if enabled
        if self.config.auto_adapt and drift_status != DriftStatus.DETECTED:
            self.incremental_learner.partial_fit(x, y_true)
        
        return {
            "drift_status": drift_status.value,
            "warning_count": self.warning_count,
            "action_taken": action_taken,
            "samples_since_drift": self.samples_since_drift,
            "in_cooldown": self.in_cooldown
        }
    
    def _handle_drift(self) -> str:
        """Handle detected drift."""
        # Trigger callback
        if self.on_drift_detected:
            self.on_drift_detected()
        
        # Choose adaptation strategy
        strategy = AdaptationStrategy(self.config.strategy)
        
        # Execute adaptation
        action = self._adapt(strategy, "drift_detected")
        
        # Enter cooldown
        self.in_cooldown = True
        self.samples_since_drift = 0
        
        return action
    
    def _adapt(self, strategy: AdaptationStrategy, trigger: str) -> str:
        """
        Execute adaptation strategy.
        
        Args:
            strategy: Adaptation strategy to use
            trigger: What triggered the adaptation
            
        Returns:
            Description of action taken
        """
        performance_before = self.incremental_learner.get_accuracy()
        
        if strategy == AdaptationStrategy.INCREMENTAL:
            # Replay from memory
            self.incremental_learner.replay(batch_size=64)
            action = "memory_replay"
            
        elif strategy == AdaptationStrategy.PARTIAL_RETRAIN:
            # Multiple replay passes
            for _ in range(5):
                self.incremental_learner.replay(batch_size=128)
            action = "partial_retrain"
            
        elif strategy == AdaptationStrategy.FULL_RETRAIN:
            # Would trigger full retraining (placeholder)
            self.incremental_learner.replay(batch_size=min(256, len(self.incremental_learner.memory_x)))
            action = "full_retrain_triggered"
            
        else:
            action = "no_action"
        
        performance_after = self.incremental_learner.get_accuracy()
        
        # Record event
        event = AdaptationEvent(
            strategy=strategy,
            trigger=trigger,
            samples_used=len(self.incremental_learner.memory_x),
            performance_before=performance_before,
            performance_after=performance_after
        )
        self.adaptation_events.append(event)
        self.performance_history.append(performance_after)
        
        # Trigger callback
        if self.on_adaptation_complete:
            self.on_adaptation_complete(event)
        
        return action
    
    def force_adaptation(self, strategy: str = "incremental") -> AdaptationEvent:
        """
        Force an adaptation regardless of drift status.
        
        Args:
            strategy: Strategy to use
            
        Returns:
            Adaptation event
        """
        strategy_enum = AdaptationStrategy(strategy)
        self._adapt(strategy_enum, "manual_trigger")
        return self.adaptation_events[-1]
    
    def get_status(self) -> Dict[str, Any]:
        """Get current adaptation status."""
        return {
            "drift_status": self.drift_detector.current_status.value,
            "warning_count": self.warning_count,
            "samples_since_drift": self.samples_since_drift,
            "in_cooldown": self.in_cooldown,
            "total_adaptations": len(self.adaptation_events),
            "current_accuracy": self.incremental_learner.get_accuracy(),
            "drift_stats": self.drift_detector.get_stats(),
            "learner_stats": self.incremental_learner.get_stats()
        }
    
    def get_performance_trend(self, window: int = 10) -> Dict[str, float]:
        """Get performance trend over recent adaptations."""
        if len(self.performance_history) < 2:
            return {"trend": 0.0, "average": self.incremental_learner.get_accuracy()}
        
        recent = self.performance_history[-window:]
        avg = sum(recent) / len(recent)
        
        # Calculate trend (slope)
        if len(recent) >= 2:
            trend = (recent[-1] - recent[0]) / len(recent)
        else:
            trend = 0.0
        
        return {
            "trend": trend,
            "average": avg,
            "min": min(recent),
            "max": max(recent)
        }
    
    def reset(self) -> None:
        """Reset adaptation state."""
        self.drift_detector.reset()
        self.incremental_learner.reset()
        self.warning_count = 0
        self.samples_since_drift = 0
        self.in_cooldown = False
