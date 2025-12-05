"""
DevTeam6 Experience Buffer

Replay buffer for storing and sampling experiences.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
import random
from datetime import datetime


@dataclass
class Experience:
    """Single experience tuple."""
    state: List[float]
    action: int
    reward: float
    next_state: List[float]
    done: bool
    action_prob: float
    value: float
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "state": self.state,
            "action": self.action,
            "reward": self.reward,
            "next_state": self.next_state,
            "done": self.done,
            "action_prob": self.action_prob,
            "value": self.value,
            "timestamp": self.timestamp
        }


class ExperienceBuffer:
    """
    Experience replay buffer with prioritized sampling support.
    
    Stores experiences and provides efficient sampling for
    batch training of reinforcement learning agents.
    """
    
    def __init__(
        self,
        capacity: int = 10000,
        prioritized: bool = True,
        alpha: float = 0.6,
        beta: float = 0.4
    ):
        self.capacity = capacity
        self.prioritized = prioritized
        self.alpha = alpha  # Priority exponent
        self.beta = beta    # Importance sampling exponent
        
        # Storage
        self.buffer: List[Experience] = []
        self.priorities: List[float] = []
        
        # Tracking
        self.position = 0
        self.total_added = 0
    
    def add(self, experience: Experience, priority: Optional[float] = None) -> None:
        """
        Add experience to buffer.
        
        Args:
            experience: Experience to add
            priority: Optional priority (uses max priority if not provided)
        """
        if priority is None:
            priority = max(self.priorities) if self.priorities else 1.0
        
        if len(self.buffer) < self.capacity:
            self.buffer.append(experience)
            self.priorities.append(priority)
        else:
            self.buffer[self.position] = experience
            self.priorities[self.position] = priority
        
        self.position = (self.position + 1) % self.capacity
        self.total_added += 1
    
    def add_batch(
        self,
        states: List[List[float]],
        actions: List[int],
        rewards: List[float],
        next_states: List[List[float]],
        dones: List[bool],
        action_probs: List[float],
        values: List[float]
    ) -> None:
        """Add batch of experiences."""
        for s, a, r, ns, d, p, v in zip(
            states, actions, rewards, next_states, dones, action_probs, values
        ):
            exp = Experience(
                state=s,
                action=a,
                reward=r,
                next_state=ns,
                done=d,
                action_prob=p,
                value=v
            )
            self.add(exp)
    
    def sample(self, batch_size: int) -> Tuple[List[Experience], List[int], List[float]]:
        """
        Sample batch of experiences.
        
        Args:
            batch_size: Number of experiences to sample
            
        Returns:
            Tuple of (experiences, indices, importance_weights)
        """
        if len(self.buffer) < batch_size:
            batch_size = len(self.buffer)
        
        if self.prioritized:
            return self._prioritized_sample(batch_size)
        else:
            return self._uniform_sample(batch_size)
    
    def _uniform_sample(self, batch_size: int) -> Tuple[List[Experience], List[int], List[float]]:
        """Uniform random sampling."""
        indices = random.sample(range(len(self.buffer)), batch_size)
        experiences = [self.buffer[i] for i in indices]
        weights = [1.0] * batch_size
        return experiences, indices, weights
    
    def _prioritized_sample(self, batch_size: int) -> Tuple[List[Experience], List[int], List[float]]:
        """Prioritized experience sampling."""
        # Calculate sampling probabilities
        priorities_alpha = [p ** self.alpha for p in self.priorities]
        total_priority = sum(priorities_alpha)
        probs = [p / total_priority for p in priorities_alpha]
        
        # Sample indices based on priorities
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
        
        experiences = [self.buffer[i] for i in indices]
        
        # Calculate importance sampling weights
        n = len(self.buffer)
        min_prob = min(probs)
        max_weight = (n * min_prob) ** (-self.beta)
        
        weights = []
        for i in indices:
            weight = (n * probs[i]) ** (-self.beta) / max_weight
            weights.append(weight)
        
        return experiences, indices, weights
    
    def update_priorities(self, indices: List[int], priorities: List[float]) -> None:
        """Update priorities for sampled experiences."""
        for idx, priority in zip(indices, priorities):
            if 0 <= idx < len(self.priorities):
                self.priorities[idx] = max(priority, 1e-6)
    
    def compute_returns(
        self,
        gamma: float = 0.99,
        lambda_gae: float = 0.95
    ) -> Tuple[List[float], List[float]]:
        """
        Compute returns and advantages using GAE.
        
        Args:
            gamma: Discount factor
            lambda_gae: GAE lambda
            
        Returns:
            Tuple of (returns, advantages)
        """
        returns = []
        advantages = []
        
        # Process in reverse order
        next_value = 0
        next_advantage = 0
        
        for i in range(len(self.buffer) - 1, -1, -1):
            exp = self.buffer[i]
            
            if exp.done:
                next_value = 0
                next_advantage = 0
            
            # TD error
            delta = exp.reward + gamma * next_value - exp.value
            
            # GAE
            advantage = delta + gamma * lambda_gae * next_advantage
            
            # Return
            ret = advantage + exp.value
            
            advantages.insert(0, advantage)
            returns.insert(0, ret)
            
            next_value = exp.value
            next_advantage = advantage
        
        # Normalize advantages
        if advantages:
            mean_adv = sum(advantages) / len(advantages)
            std_adv = (sum((a - mean_adv) ** 2 for a in advantages) / len(advantages)) ** 0.5
            if std_adv > 0:
                advantages = [(a - mean_adv) / (std_adv + 1e-8) for a in advantages]
        
        return returns, advantages
    
    def get_batch_data(self) -> Dict[str, List]:
        """Get all data as batched lists."""
        return {
            "states": [e.state for e in self.buffer],
            "actions": [e.action for e in self.buffer],
            "rewards": [e.reward for e in self.buffer],
            "next_states": [e.next_state for e in self.buffer],
            "dones": [e.done for e in self.buffer],
            "action_probs": [e.action_prob for e in self.buffer],
            "values": [e.value for e in self.buffer]
        }
    
    def clear(self) -> None:
        """Clear buffer."""
        self.buffer.clear()
        self.priorities.clear()
        self.position = 0
    
    def __len__(self) -> int:
        return len(self.buffer)
    
    def get_stats(self) -> Dict[str, Any]:
        """Get buffer statistics."""
        return {
            "size": len(self.buffer),
            "capacity": self.capacity,
            "total_added": self.total_added,
            "prioritized": self.prioritized,
            "mean_priority": sum(self.priorities) / len(self.priorities) if self.priorities else 0,
            "max_priority": max(self.priorities) if self.priorities else 0,
            "min_priority": min(self.priorities) if self.priorities else 0
        }
