"""
DevTeam6 Routing Agent

PPO-based agent for intelligent routing decisions in the Dual Transformer.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
from enum import Enum
import math
import random
from datetime import datetime


class RoutingAction(Enum):
    """Routing actions the agent can take."""
    LOCAL = 0       # Process locally
    API = 1         # Route to external API
    SECURITY = 2    # Route through security transformer
    DATA = 3        # Route to data pipeline
    HYBRID = 4      # Hybrid processing


@dataclass
class State:
    """Environment state for routing decisions."""
    token_embedding: List[float]
    category_scores: Dict[str, float]
    security_level: float
    urgency: float
    context_length: int
    previous_action: Optional[RoutingAction] = None
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())
    
    def to_vector(self) -> List[float]:
        """Convert state to feature vector."""
        vector = self.token_embedding[:128] if self.token_embedding else [0.0] * 128
        
        # Pad if needed
        while len(vector) < 128:
            vector.append(0.0)
        
        # Add category scores
        for key in ["api", "security", "data", "local", "hybrid"]:
            vector.append(self.category_scores.get(key, 0.0))
        
        # Add other features
        vector.append(self.security_level)
        vector.append(self.urgency)
        vector.append(self.context_length / 1000.0)  # Normalize
        
        # Previous action one-hot
        for action in RoutingAction:
            vector.append(1.0 if self.previous_action == action else 0.0)
        
        return vector


@dataclass 
class ActionResult:
    """Result of taking a routing action."""
    action: RoutingAction
    success: bool
    latency_ms: float
    accuracy: float
    security_violation: bool
    resource_usage: float


class RoutingAgent:
    """
    PPO-based routing agent for the DevTeam6 Dual Transformer.
    
    Learns optimal routing policies through interaction with the environment,
    maximizing reward based on accuracy, latency, and security constraints.
    """
    
    def __init__(
        self,
        state_dim: int = 256,
        action_dim: int = 5,
        hidden_dim: int = 256,
        learning_rate: float = 3e-4,
        gamma: float = 0.99,
        epsilon: float = 0.2
    ):
        self.state_dim = state_dim
        self.action_dim = action_dim
        self.hidden_dim = hidden_dim
        self.learning_rate = learning_rate
        self.gamma = gamma
        self.epsilon = epsilon  # PPO clip range
        
        # Policy network weights
        self._init_policy_network()
        
        # Value network weights
        self._init_value_network()
        
        # Training stats
        self.episode_count = 0
        self.total_steps = 0
        self.cumulative_reward = 0.0
        
        # Action history for analysis
        self.action_history: List[Tuple[RoutingAction, float]] = []
    
    def _init_policy_network(self) -> None:
        """Initialize policy network weights."""
        self.policy_w1 = [
            [self._random_init() for _ in range(self.hidden_dim)]
            for _ in range(self.state_dim)
        ]
        self.policy_b1 = [0.0] * self.hidden_dim
        
        self.policy_w2 = [
            [self._random_init() for _ in range(self.action_dim)]
            for _ in range(self.hidden_dim)
        ]
        self.policy_b2 = [0.0] * self.action_dim
    
    def _init_value_network(self) -> None:
        """Initialize value network weights."""
        self.value_w1 = [
            [self._random_init() for _ in range(self.hidden_dim)]
            for _ in range(self.state_dim)
        ]
        self.value_b1 = [0.0] * self.hidden_dim
        
        self.value_w2 = [
            [self._random_init()]
            for _ in range(self.hidden_dim)
        ]
        self.value_b2 = [0.0]
    
    def _random_init(self) -> float:
        """Xavier-like random initialization."""
        return (random.random() * 2 - 1) * 0.1
    
    def _relu(self, x: float) -> float:
        """ReLU activation."""
        return max(0, x)
    
    def _softmax(self, x: List[float]) -> List[float]:
        """Softmax function."""
        max_x = max(x)
        exp_x = [math.exp(xi - max_x) for xi in x]
        sum_exp = sum(exp_x)
        return [e / sum_exp for e in exp_x]
    
    def _forward_policy(self, state: List[float]) -> List[float]:
        """Forward pass through policy network."""
        # Hidden layer
        hidden = []
        for j in range(self.hidden_dim):
            val = self.policy_b1[j]
            for i in range(min(len(state), self.state_dim)):
                val += state[i] * self.policy_w1[i][j]
            hidden.append(self._relu(val))
        
        # Output layer
        output = []
        for j in range(self.action_dim):
            val = self.policy_b2[j]
            for i in range(self.hidden_dim):
                val += hidden[i] * self.policy_w2[i][j]
            output.append(val)
        
        return self._softmax(output)
    
    def _forward_value(self, state: List[float]) -> float:
        """Forward pass through value network."""
        # Hidden layer
        hidden = []
        for j in range(self.hidden_dim):
            val = self.value_b1[j]
            for i in range(min(len(state), self.state_dim)):
                val += state[i] * self.value_w1[i][j]
            hidden.append(self._relu(val))
        
        # Output layer (single value)
        val = self.value_b2[0]
        for i in range(self.hidden_dim):
            val += hidden[i] * self.value_w2[i][0]
        
        return val
    
    def select_action(self, state: State, explore: bool = True) -> Tuple[RoutingAction, float]:
        """
        Select an action based on the current state.
        
        Args:
            state: Current environment state
            explore: Whether to explore (sample from policy) or exploit (argmax)
            
        Returns:
            Tuple of (action, probability)
        """
        state_vector = state.to_vector()
        probs = self._forward_policy(state_vector)
        
        if explore:
            # Sample from probability distribution
            r = random.random()
            cumsum = 0
            for i, p in enumerate(probs):
                cumsum += p
                if r < cumsum:
                    action = RoutingAction(i)
                    break
            else:
                action = RoutingAction(len(probs) - 1)
        else:
            # Greedy action
            action = RoutingAction(probs.index(max(probs)))
        
        probability = probs[action.value]
        self.action_history.append((action, probability))
        self.total_steps += 1
        
        return action, probability
    
    def get_value(self, state: State) -> float:
        """Get value estimate for a state."""
        state_vector = state.to_vector()
        return self._forward_value(state_vector)
    
    def get_action_probs(self, state: State) -> Dict[RoutingAction, float]:
        """Get probability distribution over actions."""
        state_vector = state.to_vector()
        probs = self._forward_policy(state_vector)
        return {RoutingAction(i): p for i, p in enumerate(probs)}
    
    def update(
        self,
        states: List[State],
        actions: List[RoutingAction],
        rewards: List[float],
        advantages: List[float],
        old_probs: List[float]
    ) -> Dict[str, float]:
        """
        Update policy and value networks using PPO.
        
        Args:
            states: List of states
            actions: List of actions taken
            rewards: List of rewards received
            advantages: Calculated advantages
            old_probs: Action probabilities from old policy
            
        Returns:
            Dictionary with loss metrics
        """
        policy_loss = 0.0
        value_loss = 0.0
        
        for state, action, reward, advantage, old_prob in zip(
            states, actions, rewards, advantages, old_probs
        ):
            state_vector = state.to_vector()
            new_probs = self._forward_policy(state_vector)
            new_prob = new_probs[action.value]
            
            # PPO clipped objective
            ratio = new_prob / (old_prob + 1e-8)
            clipped_ratio = max(min(ratio, 1 + self.epsilon), 1 - self.epsilon)
            
            policy_loss += -min(ratio * advantage, clipped_ratio * advantage)
            
            # Value loss
            value_pred = self._forward_value(state_vector)
            value_target = reward  # Simplified, should use returns
            value_loss += (value_pred - value_target) ** 2
            
            # Update weights (simplified gradient descent)
            self._update_policy_weights(state_vector, action, advantage, new_probs)
            self._update_value_weights(state_vector, value_target, value_pred)
        
        n = len(states) if states else 1
        self.cumulative_reward += sum(rewards)
        
        return {
            "policy_loss": policy_loss / n,
            "value_loss": value_loss / n,
            "mean_advantage": sum(advantages) / n if advantages else 0,
            "mean_reward": sum(rewards) / n if rewards else 0
        }
    
    def _update_policy_weights(
        self,
        state: List[float],
        action: RoutingAction,
        advantage: float,
        probs: List[float]
    ) -> None:
        """Update policy network weights."""
        lr = self.learning_rate * advantage
        
        # Simplified update for demonstration
        for i in range(min(len(state), self.state_dim)):
            for j in range(self.hidden_dim):
                self.policy_w1[i][j] += lr * state[i] * 0.01
    
    def _update_value_weights(
        self,
        state: List[float],
        target: float,
        prediction: float
    ) -> None:
        """Update value network weights."""
        error = target - prediction
        lr = self.learning_rate * error
        
        # Simplified update
        for i in range(min(len(state), self.state_dim)):
            for j in range(self.hidden_dim):
                self.value_w1[i][j] += lr * state[i] * 0.01
    
    def get_stats(self) -> Dict[str, Any]:
        """Get agent statistics."""
        action_counts = {action: 0 for action in RoutingAction}
        for action, _ in self.action_history:
            action_counts[action] += 1
        
        return {
            "episode_count": self.episode_count,
            "total_steps": self.total_steps,
            "cumulative_reward": self.cumulative_reward,
            "action_distribution": {a.name: c for a, c in action_counts.items()},
            "recent_actions": [(a.name, p) for a, p in self.action_history[-10:]]
        }
    
    def reset_episode(self) -> None:
        """Reset for a new episode."""
        self.episode_count += 1
        self.action_history.clear()
