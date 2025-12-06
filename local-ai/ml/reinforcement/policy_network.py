"""
DevTeam6 Policy Network

Actor-Critic neural network for PPO.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
import math
import random
import json


@dataclass
class PolicyNetworkConfig:
    """Configuration for policy network."""
    state_dim: int = 256
    action_dim: int = 5
    hidden_dims: List[int] = field(default_factory=lambda: [256, 128])
    activation: str = "relu"
    use_orthogonal_init: bool = True


class PolicyNetwork:
    """
    Actor-Critic neural network for PPO-based routing.
    
    Implements both policy (actor) and value (critic) heads
    sharing a common feature extraction backbone.
    """
    
    def __init__(self, config: Optional[PolicyNetworkConfig] = None):
        self.config = config or PolicyNetworkConfig()
        
        # Network architecture
        self.state_dim = self.config.state_dim
        self.action_dim = self.config.action_dim
        self.hidden_dims = self.config.hidden_dims
        
        # Initialize network layers
        self._initialize_backbone()
        self._initialize_actor()
        self._initialize_critic()
        
        # Training state
        self.training = False
    
    def _initialize_backbone(self) -> None:
        """Initialize shared backbone layers."""
        self.backbone_layers = []
        
        input_dim = self.state_dim
        for hidden_dim in self.hidden_dims:
            weights = self._create_layer_weights(input_dim, hidden_dim)
            biases = [0.0] * hidden_dim
            self.backbone_layers.append({"weights": weights, "biases": biases})
            input_dim = hidden_dim
    
    def _initialize_actor(self) -> None:
        """Initialize actor (policy) head."""
        input_dim = self.hidden_dims[-1] if self.hidden_dims else self.state_dim
        self.actor_weights = self._create_layer_weights(input_dim, self.action_dim)
        self.actor_biases = [0.0] * self.action_dim
    
    def _initialize_critic(self) -> None:
        """Initialize critic (value) head."""
        input_dim = self.hidden_dims[-1] if self.hidden_dims else self.state_dim
        self.critic_weights = self._create_layer_weights(input_dim, 1)
        self.critic_biases = [0.0]
    
    def _create_layer_weights(self, input_dim: int, output_dim: int) -> List[List[float]]:
        """Create weights for a layer."""
        if self.config.use_orthogonal_init:
            return self._orthogonal_init(input_dim, output_dim)
        else:
            return self._xavier_init(input_dim, output_dim)
    
    def _xavier_init(self, input_dim: int, output_dim: int) -> List[List[float]]:
        """Xavier/Glorot initialization."""
        scale = math.sqrt(2.0 / (input_dim + output_dim))
        return [
            [(random.random() * 2 - 1) * scale for _ in range(output_dim)]
            for _ in range(input_dim)
        ]
    
    def _orthogonal_init(self, input_dim: int, output_dim: int) -> List[List[float]]:
        """Simplified orthogonal-like initialization."""
        scale = math.sqrt(2.0 / input_dim)
        weights = []
        for i in range(input_dim):
            row = []
            for j in range(output_dim):
                # Create pseudo-orthogonal pattern
                val = scale * math.cos((i * output_dim + j) * 0.1)
                val += (random.random() - 0.5) * scale * 0.1
                row.append(val)
            weights.append(row)
        return weights
    
    def _relu(self, x: float) -> float:
        """ReLU activation."""
        return max(0, x)
    
    def _tanh(self, x: float) -> float:
        """Tanh activation."""
        return math.tanh(x)
    
    def _softmax(self, x: List[float]) -> List[float]:
        """Softmax activation."""
        max_x = max(x)
        exp_x = [math.exp(xi - max_x) for xi in x]
        sum_exp = sum(exp_x)
        return [e / sum_exp for e in exp_x]
    
    def _forward_backbone(self, state: List[float]) -> List[float]:
        """Forward pass through shared backbone."""
        x = state
        
        for layer in self.backbone_layers:
            weights = layer["weights"]
            biases = layer["biases"]
            output_dim = len(biases)
            
            new_x = []
            for j in range(output_dim):
                val = biases[j]
                for i in range(min(len(x), len(weights))):
                    val += x[i] * weights[i][j]
                
                # Apply activation
                if self.config.activation == "relu":
                    val = self._relu(val)
                elif self.config.activation == "tanh":
                    val = self._tanh(val)
                
                new_x.append(val)
            
            x = new_x
        
        return x
    
    def forward(self, state: List[float]) -> Tuple[List[float], float]:
        """
        Forward pass returning both policy and value.
        
        Args:
            state: State feature vector
            
        Returns:
            Tuple of (action_probs, state_value)
        """
        # Pad/truncate state
        if len(state) < self.state_dim:
            state = state + [0.0] * (self.state_dim - len(state))
        elif len(state) > self.state_dim:
            state = state[:self.state_dim]
        
        # Shared backbone
        features = self._forward_backbone(state)
        
        # Actor head
        action_logits = []
        for j in range(self.action_dim):
            val = self.actor_biases[j]
            for i in range(len(features)):
                if i < len(self.actor_weights):
                    val += features[i] * self.actor_weights[i][j]
            action_logits.append(val)
        action_probs = self._softmax(action_logits)
        
        # Critic head
        value = self.critic_biases[0]
        for i in range(len(features)):
            if i < len(self.critic_weights):
                value += features[i] * self.critic_weights[i][0]
        
        return action_probs, value
    
    def get_action_probs(self, state: List[float]) -> List[float]:
        """Get action probabilities."""
        probs, _ = self.forward(state)
        return probs
    
    def get_value(self, state: List[float]) -> float:
        """Get state value estimate."""
        _, value = self.forward(state)
        return value
    
    def sample_action(self, state: List[float]) -> Tuple[int, float]:
        """Sample action from policy distribution."""
        probs = self.get_action_probs(state)
        
        r = random.random()
        cumsum = 0
        for i, p in enumerate(probs):
            cumsum += p
            if r < cumsum:
                return i, probs[i]
        
        return len(probs) - 1, probs[-1]
    
    def update(
        self,
        states: List[List[float]],
        actions: List[int],
        advantages: List[float],
        returns: List[float],
        old_probs: List[float],
        learning_rate: float = 3e-4,
        clip_epsilon: float = 0.2
    ) -> Dict[str, float]:
        """
        Update network using PPO objective.
        
        Args:
            states: Batch of states
            actions: Batch of actions taken
            advantages: Calculated advantages
            returns: Target returns for value function
            old_probs: Old action probabilities
            learning_rate: Learning rate
            clip_epsilon: PPO clip range
            
        Returns:
            Training metrics
        """
        policy_loss_sum = 0.0
        value_loss_sum = 0.0
        entropy_sum = 0.0
        
        for state, action, adv, ret, old_prob in zip(
            states, actions, advantages, returns, old_probs
        ):
            # Forward pass
            new_probs, value = self.forward(state)
            new_prob = new_probs[action]
            
            # Policy loss (clipped PPO)
            ratio = new_prob / (old_prob + 1e-8)
            clipped = max(min(ratio, 1 + clip_epsilon), 1 - clip_epsilon)
            policy_loss = -min(ratio * adv, clipped * adv)
            policy_loss_sum += policy_loss
            
            # Value loss
            value_loss = (value - ret) ** 2
            value_loss_sum += value_loss
            
            # Entropy bonus
            entropy = -sum(p * math.log(p + 1e-8) for p in new_probs)
            entropy_sum += entropy
            
            # Update weights (simplified)
            self._update_weights(
                state, action, adv, new_probs, value, ret, learning_rate
            )
        
        n = len(states) if states else 1
        return {
            "policy_loss": policy_loss_sum / n,
            "value_loss": value_loss_sum / n,
            "entropy": entropy_sum / n
        }
    
    def _update_weights(
        self,
        state: List[float],
        action: int,
        advantage: float,
        probs: List[float],
        value: float,
        target: float,
        lr: float
    ) -> None:
        """Update network weights (simplified gradient descent)."""
        # Policy gradient
        policy_grad = advantage * (1 if advantage > 0 else 0.1)
        
        # Update actor weights
        for i in range(min(len(state), len(self.actor_weights))):
            self.actor_weights[i][action] += lr * policy_grad * state[i] * 0.1
        
        # Value gradient
        value_grad = target - value
        
        # Update critic weights
        for i in range(min(len(state), len(self.critic_weights))):
            self.critic_weights[i][0] += lr * value_grad * state[i] * 0.1
    
    def save(self, path: str) -> None:
        """Save network to file."""
        data = {
            "config": {
                "state_dim": self.config.state_dim,
                "action_dim": self.config.action_dim,
                "hidden_dims": self.config.hidden_dims,
                "activation": self.config.activation
            },
            "backbone_layers": self.backbone_layers,
            "actor_weights": self.actor_weights,
            "actor_biases": self.actor_biases,
            "critic_weights": self.critic_weights,
            "critic_biases": self.critic_biases
        }
        with open(path, "w") as f:
            json.dump(data, f)
    
    def load(self, path: str) -> None:
        """Load network from file."""
        with open(path, "r") as f:
            data = json.load(f)
        
        self.backbone_layers = data["backbone_layers"]
        self.actor_weights = data["actor_weights"]
        self.actor_biases = data["actor_biases"]
        self.critic_weights = data["critic_weights"]
        self.critic_biases = data["critic_biases"]
