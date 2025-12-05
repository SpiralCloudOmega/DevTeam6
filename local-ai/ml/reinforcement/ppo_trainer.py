"""
DevTeam6 PPO Trainer

Training loop for PPO-based routing optimization.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable
import random
from datetime import datetime

from .routing_agent import RoutingAgent, RoutingAction, State
from .reward_calculator import RewardCalculator, ActionOutcome
from .policy_network import PolicyNetwork
from .experience_buffer import ExperienceBuffer, Experience


@dataclass
class PPOConfig:
    """Configuration for PPO training."""
    # Optimization
    learning_rate: float = 3e-4
    gamma: float = 0.99
    lambda_gae: float = 0.95
    clip_epsilon: float = 0.2
    entropy_coef: float = 0.01
    value_coef: float = 0.5
    max_grad_norm: float = 0.5
    
    # Training loop
    num_steps: int = 2048
    num_epochs: int = 10
    mini_batch_size: int = 64
    
    # Environment
    max_episode_length: int = 1000
    num_envs: int = 1
    
    # Logging
    log_interval: int = 10
    save_interval: int = 100


@dataclass
class TrainingMetrics:
    """Metrics from training iteration."""
    iteration: int
    policy_loss: float
    value_loss: float
    entropy: float
    mean_reward: float
    mean_advantage: float
    mean_episode_length: float
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class PPOTrainer:
    """
    PPO trainer for the DevTeam6 routing agent.
    
    Implements the complete PPO training loop with experience collection,
    advantage estimation, and policy updates.
    """
    
    def __init__(
        self,
        policy: PolicyNetwork,
        config: Optional[PPOConfig] = None
    ):
        self.policy = policy
        self.config = config or PPOConfig()
        
        # Experience buffer
        self.buffer = ExperienceBuffer(
            capacity=self.config.num_steps,
            prioritized=False
        )
        
        # Reward calculator
        self.reward_calculator = RewardCalculator()
        
        # Training state
        self.iteration = 0
        self.total_steps = 0
        self.episode_count = 0
        
        # Metrics history
        self.metrics_history: List[TrainingMetrics] = []
        
        # Best model tracking
        self.best_reward = float('-inf')
    
    def collect_rollout(
        self,
        env_step_fn: Callable[[List[float], int], ActionOutcome]
    ) -> None:
        """
        Collect experience rollout from environment.
        
        Args:
            env_step_fn: Function that takes (state, action) and returns ActionOutcome
        """
        self.buffer.clear()
        
        # Initialize state (would come from environment)
        state = self._create_initial_state()
        episode_length = 0
        
        for step in range(self.config.num_steps):
            # Get action from policy
            state_vector = state.to_vector()
            action_probs = self.policy.get_action_probs(state_vector)
            value = self.policy.get_value(state_vector)
            
            # Sample action
            action, prob = self.policy.sample_action(state_vector)
            
            # Execute action
            outcome = env_step_fn(state_vector, action)
            
            # Calculate reward
            action_outcome = ActionOutcome(
                success=outcome.success,
                accuracy=outcome.accuracy,
                latency_ms=outcome.latency_ms,
                security_violation=outcome.security_violation,
                resource_usage=outcome.resource_usage
            )
            reward_info = self.reward_calculator.calculate_reward(action_outcome)
            reward = reward_info["total"]
            
            # Create next state
            next_state = self._create_next_state(state, action, outcome)
            next_state_vector = next_state.to_vector()
            
            # Check if episode done
            done = episode_length >= self.config.max_episode_length
            
            # Store experience
            exp = Experience(
                state=state_vector,
                action=action,
                reward=reward,
                next_state=next_state_vector,
                done=done,
                action_prob=prob,
                value=value
            )
            self.buffer.add(exp)
            
            # Update state
            state = next_state
            episode_length += 1
            self.total_steps += 1
            
            # Handle episode termination
            if done:
                state = self._create_initial_state()
                episode_length = 0
                self.episode_count += 1
    
    def _create_initial_state(self) -> State:
        """Create initial state for episode."""
        return State(
            token_embedding=[random.random() for _ in range(128)],
            category_scores={
                "api": random.random(),
                "security": random.random(),
                "data": random.random(),
                "local": random.random(),
                "hybrid": random.random()
            },
            security_level=random.random(),
            urgency=random.random(),
            context_length=random.randint(1, 100),
            previous_action=None
        )
    
    def _create_next_state(
        self,
        current_state: State,
        action: int,
        outcome: ActionOutcome
    ) -> State:
        """Create next state based on action outcome."""
        # Modify embedding slightly
        new_embedding = [
            e + (random.random() - 0.5) * 0.1
            for e in current_state.token_embedding
        ]
        
        # Update category scores based on action
        new_scores = current_state.category_scores.copy()
        action_name = RoutingAction(action).name.lower()
        if action_name in new_scores:
            new_scores[action_name] = min(1.0, new_scores[action_name] + 0.1)
        
        return State(
            token_embedding=new_embedding,
            category_scores=new_scores,
            security_level=current_state.security_level,
            urgency=max(0, current_state.urgency - 0.1),
            context_length=current_state.context_length + 1,
            previous_action=RoutingAction(action)
        )
    
    def update_policy(self) -> Dict[str, float]:
        """
        Update policy using collected experiences.
        
        Returns:
            Training metrics
        """
        # Compute returns and advantages
        returns, advantages = self.buffer.compute_returns(
            gamma=self.config.gamma,
            lambda_gae=self.config.lambda_gae
        )
        
        # Get batch data
        batch_data = self.buffer.get_batch_data()
        states = batch_data["states"]
        actions = batch_data["actions"]
        old_probs = batch_data["action_probs"]
        
        # Multiple epochs of updates
        total_policy_loss = 0.0
        total_value_loss = 0.0
        total_entropy = 0.0
        update_count = 0
        
        for epoch in range(self.config.num_epochs):
            # Create mini-batches
            indices = list(range(len(states)))
            random.shuffle(indices)
            
            for start in range(0, len(indices), self.config.mini_batch_size):
                end = start + self.config.mini_batch_size
                batch_indices = indices[start:end]
                
                batch_states = [states[i] for i in batch_indices]
                batch_actions = [actions[i] for i in batch_indices]
                batch_advantages = [advantages[i] for i in batch_indices]
                batch_returns = [returns[i] for i in batch_indices]
                batch_old_probs = [old_probs[i] for i in batch_indices]
                
                # Update network
                metrics = self.policy.update(
                    batch_states,
                    batch_actions,
                    batch_advantages,
                    batch_returns,
                    batch_old_probs,
                    learning_rate=self.config.learning_rate,
                    clip_epsilon=self.config.clip_epsilon
                )
                
                total_policy_loss += metrics["policy_loss"]
                total_value_loss += metrics["value_loss"]
                total_entropy += metrics["entropy"]
                update_count += 1
        
        # Average metrics
        return {
            "policy_loss": total_policy_loss / update_count if update_count > 0 else 0,
            "value_loss": total_value_loss / update_count if update_count > 0 else 0,
            "entropy": total_entropy / update_count if update_count > 0 else 0
        }
    
    def train_iteration(
        self,
        env_step_fn: Callable[[List[float], int], ActionOutcome]
    ) -> TrainingMetrics:
        """
        Run single training iteration.
        
        Args:
            env_step_fn: Environment step function
            
        Returns:
            Training metrics
        """
        # Collect rollout
        self.collect_rollout(env_step_fn)
        
        # Update policy
        update_metrics = self.update_policy()
        
        # Calculate additional metrics
        rewards = [e.reward for e in self.buffer.buffer]
        advantages = self.buffer.compute_returns(
            self.config.gamma, self.config.lambda_gae
        )[1]
        
        mean_reward = sum(rewards) / len(rewards) if rewards else 0
        mean_advantage = sum(advantages) / len(advantages) if advantages else 0
        
        # Track best model
        if mean_reward > self.best_reward:
            self.best_reward = mean_reward
        
        # Create metrics
        metrics = TrainingMetrics(
            iteration=self.iteration,
            policy_loss=update_metrics["policy_loss"],
            value_loss=update_metrics["value_loss"],
            entropy=update_metrics["entropy"],
            mean_reward=mean_reward,
            mean_advantage=mean_advantage,
            mean_episode_length=self.config.num_steps / max(self.episode_count, 1)
        )
        
        self.metrics_history.append(metrics)
        self.iteration += 1
        
        return metrics
    
    def train(
        self,
        env_step_fn: Callable[[List[float], int], ActionOutcome],
        num_iterations: int = 100
    ) -> List[TrainingMetrics]:
        """
        Train for multiple iterations.
        
        Args:
            env_step_fn: Environment step function
            num_iterations: Number of training iterations
            
        Returns:
            List of training metrics
        """
        for i in range(num_iterations):
            metrics = self.train_iteration(env_step_fn)
            
            if (i + 1) % self.config.log_interval == 0:
                print(f"Iteration {i+1}/{num_iterations}: "
                      f"reward={metrics.mean_reward:.3f}, "
                      f"policy_loss={metrics.policy_loss:.3f}")
        
        return self.metrics_history
    
    def get_stats(self) -> Dict[str, Any]:
        """Get training statistics."""
        return {
            "iteration": self.iteration,
            "total_steps": self.total_steps,
            "episode_count": self.episode_count,
            "best_reward": self.best_reward,
            "recent_metrics": [m.__dict__ for m in self.metrics_history[-10:]],
            "reward_history": [m.mean_reward for m in self.metrics_history]
        }
