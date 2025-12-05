"""
DevTeam6 Auto Tuner

Automatic model tuning and optimization.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable
from datetime import datetime

from .hyperparameter_optimizer import HyperparameterOptimizer, HyperparameterSpace, Trial
from .model_selector import ModelSelector


@dataclass
class TuningConfig:
    """Configuration for auto-tuning."""
    n_trials: int = 50
    timeout_hours: float = 24.0
    early_stopping_rounds: int = 10
    improvement_threshold: float = 0.01


class AutoTuner:
    """
    Automatic model tuning combining hyperparameter optimization
    and model selection.
    """
    
    def __init__(self, config: Optional[TuningConfig] = None):
        self.config = config or TuningConfig()
        self.optimizer: Optional[HyperparameterOptimizer] = None
        self.selector = ModelSelector()
        
        # History
        self.tuning_history: List[Dict[str, Any]] = []
        self.best_config: Optional[Dict[str, Any]] = None
        self.best_score: float = float('-inf')
        
        # State
        self.rounds_without_improvement = 0
        self.start_time: Optional[datetime] = None
    
    def define_search_space(
        self,
        spaces: List[HyperparameterSpace]
    ) -> None:
        """Define the hyperparameter search space."""
        self.optimizer = HyperparameterOptimizer(
            search_spaces=spaces,
            n_initial_points=min(10, self.config.n_trials // 5)
        )
    
    def tune(
        self,
        train_fn: Callable[[Dict[str, Any]], Dict[str, float]],
        n_trials: Optional[int] = None
    ) -> Dict[str, Any]:
        """
        Run automatic tuning.
        
        Args:
            train_fn: Function that takes params and returns metrics dict
            n_trials: Number of trials (overrides config)
            
        Returns:
            Best configuration found
        """
        if not self.optimizer:
            raise ValueError("Search space not defined. Call define_search_space first.")
        
        n_trials = n_trials or self.config.n_trials
        self.start_time = datetime.now()
        self.rounds_without_improvement = 0
        
        for i in range(n_trials):
            # Check timeout
            elapsed = (datetime.now() - self.start_time).total_seconds() / 3600
            if elapsed > self.config.timeout_hours:
                print(f"Timeout reached after {i} trials")
                break
            
            # Check early stopping
            if self.rounds_without_improvement >= self.config.early_stopping_rounds:
                print(f"Early stopping after {i} trials (no improvement)")
                break
            
            # Get next hyperparameters
            params = self.optimizer.suggest()
            
            # Train and evaluate
            start = datetime.now()
            try:
                metrics = train_fn(params)
            except Exception as e:
                print(f"Trial {i+1} failed: {e}")
                metrics = {"accuracy": 0, "error": str(e)}
            
            duration = (datetime.now() - start).total_seconds()
            
            # Get score (primary metric)
            score = metrics.get("accuracy", metrics.get("score", 0))
            
            # Report to optimizer
            self.optimizer.report(params, score, duration)
            
            # Track history
            self.tuning_history.append({
                "trial": i + 1,
                "params": params,
                "metrics": metrics,
                "score": score,
                "duration": duration
            })
            
            # Check for improvement
            if score > self.best_score + self.config.improvement_threshold:
                self.best_score = score
                self.best_config = params.copy()
                self.rounds_without_improvement = 0
            else:
                self.rounds_without_improvement += 1
            
            # Progress update
            if (i + 1) % 10 == 0:
                print(f"Trial {i+1}/{n_trials}: score={score:.4f}, best={self.best_score:.4f}")
        
        return {
            "best_config": self.best_config,
            "best_score": self.best_score,
            "total_trials": len(self.tuning_history)
        }
    
    def get_improvement_curve(self) -> List[float]:
        """Get best score at each trial."""
        curve = []
        best_so_far = float('-inf')
        
        for entry in self.tuning_history:
            if entry["score"] > best_so_far:
                best_so_far = entry["score"]
            curve.append(best_so_far)
        
        return curve
    
    def get_param_importance(self) -> Dict[str, float]:
        """Estimate hyperparameter importance."""
        if len(self.tuning_history) < 10:
            return {}
        
        importance = {}
        
        for param_name in self.best_config or {}:
            # Calculate correlation with score
            values = []
            scores = []
            
            for entry in self.tuning_history:
                if param_name in entry["params"]:
                    val = entry["params"][param_name]
                    if isinstance(val, (int, float)):
                        values.append(val)
                        scores.append(entry["score"])
            
            if len(values) > 5:
                # Calculate correlation
                mean_v = sum(values) / len(values)
                mean_s = sum(scores) / len(scores)
                
                cov = sum((v - mean_v) * (s - mean_s) for v, s in zip(values, scores))
                var_v = sum((v - mean_v) ** 2 for v in values)
                var_s = sum((s - mean_s) ** 2 for s in scores)
                
                if var_v > 0 and var_s > 0:
                    corr = abs(cov / (var_v ** 0.5 * var_s ** 0.5))
                    importance[param_name] = corr
        
        # Normalize
        total = sum(importance.values()) or 1
        importance = {k: v / total for k, v in importance.items()}
        
        return dict(sorted(importance.items(), key=lambda x: x[1], reverse=True))
    
    def get_stats(self) -> Dict[str, Any]:
        """Get tuning statistics."""
        if not self.tuning_history:
            return {"status": "not_started"}
        
        scores = [e["score"] for e in self.tuning_history]
        durations = [e["duration"] for e in self.tuning_history]
        
        return {
            "total_trials": len(self.tuning_history),
            "best_score": self.best_score,
            "mean_score": sum(scores) / len(scores),
            "total_time_hours": sum(durations) / 3600,
            "mean_trial_time": sum(durations) / len(durations),
            "improvement_curve": self.get_improvement_curve()[-10:],
            "param_importance": self.get_param_importance()
        }
