"""
DevTeam6 Reward Calculator

Multi-objective reward calculation for routing optimization.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any
from enum import Enum
from datetime import datetime


class RewardComponent(Enum):
    """Components of the reward signal."""
    ACCURACY = "accuracy"
    LATENCY = "latency"
    SECURITY = "security"
    RESOURCE = "resource"
    CONSISTENCY = "consistency"


@dataclass
class RewardConfig:
    """Configuration for reward calculation."""
    accuracy_weight: float = 0.4
    latency_weight: float = 0.2
    security_weight: float = 0.25
    resource_weight: float = 0.1
    consistency_weight: float = 0.05
    
    # Targets
    target_latency_ms: float = 100.0
    max_latency_ms: float = 1000.0
    security_violation_penalty: float = -10.0
    
    # Scaling
    reward_scale: float = 1.0
    clip_min: float = -10.0
    clip_max: float = 10.0


@dataclass
class ActionOutcome:
    """Outcome of a routing action."""
    success: bool
    accuracy: float  # 0.0 to 1.0
    latency_ms: float
    security_violation: bool
    resource_usage: float  # 0.0 to 1.0
    expected_action: Optional[str] = None
    actual_action: Optional[str] = None


class RewardCalculator:
    """
    Multi-objective reward calculator for the routing agent.
    
    Computes rewards based on accuracy, latency, security compliance,
    resource efficiency, and action consistency.
    """
    
    def __init__(self, config: Optional[RewardConfig] = None):
        self.config = config or RewardConfig()
        
        # History for consistency calculation
        self.outcome_history: List[ActionOutcome] = []
        self.reward_history: List[float] = []
        
        # Running statistics
        self.total_rewards = 0.0
        self.reward_count = 0
    
    def calculate_reward(self, outcome: ActionOutcome) -> Dict[str, float]:
        """
        Calculate reward from action outcome.
        
        Args:
            outcome: The outcome of the routing action
            
        Returns:
            Dictionary with total reward and component breakdown
        """
        components = {}
        
        # Accuracy reward
        if outcome.success:
            accuracy_reward = outcome.accuracy * 2 - 1  # Map [0,1] to [-1,1]
        else:
            accuracy_reward = -1.0
        components[RewardComponent.ACCURACY] = accuracy_reward * self.config.accuracy_weight
        
        # Latency reward
        latency_reward = self._calculate_latency_reward(outcome.latency_ms)
        components[RewardComponent.LATENCY] = latency_reward * self.config.latency_weight
        
        # Security reward
        if outcome.security_violation:
            security_reward = self.config.security_violation_penalty
        else:
            security_reward = 1.0
        components[RewardComponent.SECURITY] = security_reward * self.config.security_weight
        
        # Resource efficiency reward
        resource_reward = 1.0 - outcome.resource_usage  # Lower usage = higher reward
        components[RewardComponent.RESOURCE] = resource_reward * self.config.resource_weight
        
        # Consistency reward
        consistency_reward = self._calculate_consistency_reward(outcome)
        components[RewardComponent.CONSISTENCY] = consistency_reward * self.config.consistency_weight
        
        # Total reward
        total = sum(components.values())
        total *= self.config.reward_scale
        total = max(min(total, self.config.clip_max), self.config.clip_min)
        
        # Update history
        self.outcome_history.append(outcome)
        self.reward_history.append(total)
        self.total_rewards += total
        self.reward_count += 1
        
        return {
            "total": total,
            "components": {k.value: v for k, v in components.items()}
        }
    
    def _calculate_latency_reward(self, latency_ms: float) -> float:
        """Calculate latency component of reward."""
        if latency_ms <= self.config.target_latency_ms:
            # Bonus for meeting target
            return 1.0
        elif latency_ms >= self.config.max_latency_ms:
            # Penalty for exceeding max
            return -1.0
        else:
            # Linear interpolation
            range_ms = self.config.max_latency_ms - self.config.target_latency_ms
            excess_ms = latency_ms - self.config.target_latency_ms
            return 1.0 - 2.0 * (excess_ms / range_ms)
    
    def _calculate_consistency_reward(self, outcome: ActionOutcome) -> float:
        """Calculate consistency reward based on action matching."""
        if outcome.expected_action is None:
            return 0.0
        
        if outcome.expected_action == outcome.actual_action:
            return 1.0
        else:
            return -0.5
    
    def get_running_average(self) -> float:
        """Get running average reward."""
        if self.reward_count == 0:
            return 0.0
        return self.total_rewards / self.reward_count
    
    def get_recent_average(self, window: int = 100) -> float:
        """Get average reward over recent window."""
        if not self.reward_history:
            return 0.0
        recent = self.reward_history[-window:]
        return sum(recent) / len(recent)
    
    def get_stats(self) -> Dict[str, Any]:
        """Get reward statistics."""
        return {
            "total_rewards": self.total_rewards,
            "reward_count": self.reward_count,
            "running_average": self.get_running_average(),
            "recent_average": self.get_recent_average(),
            "min_reward": min(self.reward_history) if self.reward_history else 0,
            "max_reward": max(self.reward_history) if self.reward_history else 0,
            "config": {
                "accuracy_weight": self.config.accuracy_weight,
                "latency_weight": self.config.latency_weight,
                "security_weight": self.config.security_weight,
                "resource_weight": self.config.resource_weight
            }
        }
    
    def reset(self) -> None:
        """Reset calculator state."""
        self.outcome_history.clear()
        self.reward_history.clear()
        self.total_rewards = 0.0
        self.reward_count = 0
