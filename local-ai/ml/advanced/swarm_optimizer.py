"""
DevTeam6 Swarm Optimizer

Particle Swarm Optimization for routing and model optimization.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable, Tuple
import math
import random
from datetime import datetime


@dataclass
class Particle:
    """A particle in the swarm."""
    position: List[float]
    velocity: List[float]
    best_position: List[float]
    best_fitness: float
    fitness: float = 0.0


@dataclass
class SwarmConfig:
    """Configuration for swarm optimization."""
    num_particles: int = 30
    inertia_weight: float = 0.7
    cognitive_coef: float = 1.5
    social_coef: float = 1.5
    max_velocity: float = 0.5
    min_position: float = -1.0
    max_position: float = 1.0


class SwarmOptimizer:
    """
    Particle Swarm Optimization for the DevTeam6 ML layer.
    
    Uses a population of particles to explore the solution space
    and converge on optimal configurations.
    """
    
    def __init__(
        self,
        dimensions: int,
        config: Optional[SwarmConfig] = None,
        fitness_fn: Optional[Callable[[List[float]], float]] = None
    ):
        self.dimensions = dimensions
        self.config = config or SwarmConfig()
        self.fitness_fn = fitness_fn
        
        # Swarm state
        self.particles: List[Particle] = []
        self.global_best_position: List[float] = []
        self.global_best_fitness: float = float('-inf')
        
        # History
        self.fitness_history: List[float] = []
        self.iteration = 0
        
        # Initialize swarm
        self._initialize_swarm()
    
    def _initialize_swarm(self) -> None:
        """Initialize particle positions and velocities."""
        for _ in range(self.config.num_particles):
            # Random initial position
            position = [
                random.uniform(self.config.min_position, self.config.max_position)
                for _ in range(self.dimensions)
            ]
            
            # Random initial velocity
            velocity = [
                random.uniform(-self.config.max_velocity, self.config.max_velocity)
                for _ in range(self.dimensions)
            ]
            
            particle = Particle(
                position=position,
                velocity=velocity,
                best_position=position.copy(),
                best_fitness=float('-inf')
            )
            self.particles.append(particle)
        
        # Initialize global best
        self.global_best_position = self.particles[0].position.copy()
    
    def step(self) -> float:
        """
        Perform one optimization step.
        
        Returns:
            Current global best fitness
        """
        # Evaluate fitness for all particles
        for particle in self.particles:
            if self.fitness_fn:
                particle.fitness = self.fitness_fn(particle.position)
            
            # Update personal best
            if particle.fitness > particle.best_fitness:
                particle.best_fitness = particle.fitness
                particle.best_position = particle.position.copy()
            
            # Update global best
            if particle.fitness > self.global_best_fitness:
                self.global_best_fitness = particle.fitness
                self.global_best_position = particle.position.copy()
        
        # Update velocities and positions
        for particle in self.particles:
            self._update_particle(particle)
        
        self.fitness_history.append(self.global_best_fitness)
        self.iteration += 1
        
        return self.global_best_fitness
    
    def _update_particle(self, particle: Particle) -> None:
        """Update particle velocity and position."""
        for d in range(self.dimensions):
            # Random factors
            r1 = random.random()
            r2 = random.random()
            
            # Cognitive component (personal best)
            cognitive = self.config.cognitive_coef * r1 * (
                particle.best_position[d] - particle.position[d]
            )
            
            # Social component (global best)
            social = self.config.social_coef * r2 * (
                self.global_best_position[d] - particle.position[d]
            )
            
            # Update velocity
            particle.velocity[d] = (
                self.config.inertia_weight * particle.velocity[d] +
                cognitive + social
            )
            
            # Clamp velocity
            particle.velocity[d] = max(
                -self.config.max_velocity,
                min(self.config.max_velocity, particle.velocity[d])
            )
            
            # Update position
            particle.position[d] += particle.velocity[d]
            
            # Clamp position
            particle.position[d] = max(
                self.config.min_position,
                min(self.config.max_position, particle.position[d])
            )
    
    def optimize(self, max_iterations: int = 100) -> Tuple[List[float], float]:
        """
        Run optimization for multiple iterations.
        
        Args:
            max_iterations: Maximum number of iterations
            
        Returns:
            Tuple of (best_position, best_fitness)
        """
        for _ in range(max_iterations):
            self.step()
            
            # Early stopping if converged
            if len(self.fitness_history) > 10:
                recent = self.fitness_history[-10:]
                if max(recent) - min(recent) < 1e-6:
                    break
        
        return self.global_best_position, self.global_best_fitness
    
    def get_swarm_state(self) -> Dict[str, Any]:
        """Get current swarm state."""
        positions = [p.position for p in self.particles]
        fitnesses = [p.fitness for p in self.particles]
        
        return {
            "num_particles": len(self.particles),
            "iteration": self.iteration,
            "global_best_fitness": self.global_best_fitness,
            "global_best_position": self.global_best_position,
            "mean_fitness": sum(fitnesses) / len(fitnesses) if fitnesses else 0,
            "fitness_std": self._std(fitnesses),
            "convergence": self._calculate_convergence()
        }
    
    def _std(self, values: List[float]) -> float:
        """Calculate standard deviation."""
        if not values:
            return 0.0
        mean = sum(values) / len(values)
        variance = sum((v - mean) ** 2 for v in values) / len(values)
        return math.sqrt(variance)
    
    def _calculate_convergence(self) -> float:
        """Calculate convergence metric (0 = not converged, 1 = converged)."""
        if len(self.fitness_history) < 10:
            return 0.0
        
        recent = self.fitness_history[-10:]
        range_val = max(recent) - min(recent)
        max_val = max(abs(v) for v in recent) if recent else 1
        
        return 1.0 - min(range_val / (max_val + 1e-6), 1.0)
    
    def reset(self) -> None:
        """Reset the swarm."""
        self.particles.clear()
        self.global_best_position.clear()
        self.global_best_fitness = float('-inf')
        self.fitness_history.clear()
        self.iteration = 0
        self._initialize_swarm()
