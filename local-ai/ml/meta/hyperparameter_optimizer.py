"""
DevTeam6 Hyperparameter Optimizer

Bayesian optimization for hyperparameter tuning.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable, Tuple
import math
import random
from datetime import datetime


@dataclass
class HyperparameterSpace:
    """Definition of a hyperparameter search space."""
    name: str
    param_type: str  # float, int, categorical
    low: Optional[float] = None
    high: Optional[float] = None
    choices: Optional[List[Any]] = None
    log_scale: bool = False


@dataclass
class Trial:
    """Result of a hyperparameter trial."""
    trial_id: int
    params: Dict[str, Any]
    score: float
    duration_seconds: float
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class HyperparameterOptimizer:
    """
    Bayesian optimization for hyperparameter tuning.
    
    Uses Gaussian Process surrogate model to efficiently
    search the hyperparameter space.
    """
    
    def __init__(
        self,
        search_spaces: List[HyperparameterSpace],
        n_initial_points: int = 10,
        acquisition_function: str = "ei"  # ei, ucb, pi
    ):
        self.search_spaces = {s.name: s for s in search_spaces}
        self.n_initial_points = n_initial_points
        self.acquisition = acquisition_function
        
        # Trial history
        self.trials: List[Trial] = []
        self.best_trial: Optional[Trial] = None
        self.trial_count = 0
        
        # GP surrogate model (simplified)
        self.observed_x: List[List[float]] = []
        self.observed_y: List[float] = []
    
    def _sample_random(self) -> Dict[str, Any]:
        """Sample random hyperparameters."""
        params = {}
        for name, space in self.search_spaces.items():
            if space.param_type == "float":
                if space.log_scale:
                    val = math.exp(random.uniform(
                        math.log(space.low), math.log(space.high)
                    ))
                else:
                    val = random.uniform(space.low, space.high)
                params[name] = val
            elif space.param_type == "int":
                params[name] = random.randint(int(space.low), int(space.high))
            elif space.param_type == "categorical":
                params[name] = random.choice(space.choices)
        return params
    
    def _params_to_vector(self, params: Dict[str, Any]) -> List[float]:
        """Convert params to normalized vector."""
        vector = []
        for name, space in self.search_spaces.items():
            val = params.get(name)
            if space.param_type in ("float", "int"):
                # Normalize to [0, 1]
                if space.log_scale:
                    norm = (math.log(val) - math.log(space.low)) / (math.log(space.high) - math.log(space.low))
                else:
                    norm = (val - space.low) / (space.high - space.low)
                vector.append(norm)
            elif space.param_type == "categorical":
                # One-hot encode
                for choice in space.choices:
                    vector.append(1.0 if val == choice else 0.0)
        return vector
    
    def _vector_to_params(self, vector: List[float]) -> Dict[str, Any]:
        """Convert normalized vector to params."""
        params = {}
        idx = 0
        for name, space in self.search_spaces.items():
            if space.param_type in ("float", "int"):
                norm = vector[idx]
                if space.log_scale:
                    val = math.exp(math.log(space.low) + norm * (math.log(space.high) - math.log(space.low)))
                else:
                    val = space.low + norm * (space.high - space.low)
                if space.param_type == "int":
                    val = int(round(val))
                params[name] = val
                idx += 1
            elif space.param_type == "categorical":
                # Find max one-hot
                best_idx = 0
                best_val = vector[idx]
                for i, choice in enumerate(space.choices):
                    if vector[idx + i] > best_val:
                        best_val = vector[idx + i]
                        best_idx = i
                params[name] = space.choices[best_idx]
                idx += len(space.choices)
        return params
    
    def _gp_predict(self, x: List[float]) -> Tuple[float, float]:
        """Predict mean and variance using GP surrogate."""
        if not self.observed_x:
            return 0.0, 1.0
        
        # Calculate distances to observed points
        distances = []
        for ox in self.observed_x:
            dist = math.sqrt(sum((xi - oxi) ** 2 for xi, oxi in zip(x, ox)))
            distances.append(dist)
        
        # Simple kernel-based prediction
        length_scale = 0.5
        weights = [math.exp(-d ** 2 / (2 * length_scale ** 2)) for d in distances]
        total_weight = sum(weights) + 1e-6
        
        # Predicted mean
        mean = sum(w * y for w, y in zip(weights, self.observed_y)) / total_weight
        
        # Predicted variance (simplified)
        closest_dist = min(distances) if distances else 1.0
        variance = 1.0 - math.exp(-closest_dist ** 2 / (2 * length_scale ** 2))
        
        return mean, variance
    
    def _expected_improvement(self, x: List[float]) -> float:
        """Calculate Expected Improvement acquisition."""
        mean, var = self._gp_predict(x)
        std = math.sqrt(var)
        
        if std < 1e-6:
            return 0.0
        
        best_y = max(self.observed_y) if self.observed_y else 0
        z = (mean - best_y) / std
        
        # Approximate normal CDF and PDF
        pdf = math.exp(-z ** 2 / 2) / math.sqrt(2 * math.pi)
        cdf = 0.5 * (1 + math.tanh(z * 0.7978845608))  # Approximation
        
        ei = std * (z * cdf + pdf)
        return ei
    
    def _upper_confidence_bound(self, x: List[float], kappa: float = 2.0) -> float:
        """Calculate UCB acquisition."""
        mean, var = self._gp_predict(x)
        return mean + kappa * math.sqrt(var)
    
    def _probability_improvement(self, x: List[float]) -> float:
        """Calculate Probability of Improvement."""
        mean, var = self._gp_predict(x)
        std = math.sqrt(var)
        
        if std < 1e-6:
            return 0.0
        
        best_y = max(self.observed_y) if self.observed_y else 0
        z = (mean - best_y) / std
        
        # Approximate normal CDF
        pi = 0.5 * (1 + math.tanh(z * 0.7978845608))
        return pi
    
    def _optimize_acquisition(self, n_candidates: int = 100) -> Dict[str, Any]:
        """Find params maximizing acquisition function."""
        best_params = None
        best_acq = float('-inf')
        
        for _ in range(n_candidates):
            params = self._sample_random()
            x = self._params_to_vector(params)
            
            if self.acquisition == "ei":
                acq = self._expected_improvement(x)
            elif self.acquisition == "ucb":
                acq = self._upper_confidence_bound(x)
            elif self.acquisition == "pi":
                acq = self._probability_improvement(x)
            else:
                acq = self._expected_improvement(x)
            
            if acq > best_acq:
                best_acq = acq
                best_params = params
        
        return best_params
    
    def suggest(self) -> Dict[str, Any]:
        """
        Suggest next hyperparameters to try.
        
        Returns:
            Dictionary of hyperparameter values
        """
        if len(self.trials) < self.n_initial_points:
            # Random sampling for initial exploration
            return self._sample_random()
        else:
            # Bayesian optimization
            return self._optimize_acquisition()
    
    def report(
        self,
        params: Dict[str, Any],
        score: float,
        duration: float = 0.0
    ) -> Trial:
        """
        Report trial results.
        
        Args:
            params: Hyperparameters used
            score: Evaluation score (higher is better)
            duration: Trial duration in seconds
            
        Returns:
            Trial object
        """
        self.trial_count += 1
        
        trial = Trial(
            trial_id=self.trial_count,
            params=params,
            score=score,
            duration_seconds=duration
        )
        
        self.trials.append(trial)
        
        # Update GP observations
        x = self._params_to_vector(params)
        self.observed_x.append(x)
        self.observed_y.append(score)
        
        # Update best trial
        if self.best_trial is None or score > self.best_trial.score:
            self.best_trial = trial
        
        return trial
    
    def get_best(self) -> Optional[Trial]:
        """Get best trial so far."""
        return self.best_trial
    
    def get_stats(self) -> Dict[str, Any]:
        """Get optimizer statistics."""
        scores = [t.score for t in self.trials]
        return {
            "total_trials": len(self.trials),
            "best_score": self.best_trial.score if self.best_trial else None,
            "best_params": self.best_trial.params if self.best_trial else None,
            "mean_score": sum(scores) / len(scores) if scores else 0,
            "score_std": (
                math.sqrt(sum((s - sum(scores)/len(scores))**2 for s in scores) / len(scores))
                if scores else 0
            ),
            "acquisition_function": self.acquisition
        }
