"""
DevTeam6 Model Selector

Automatic model selection based on performance.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable
from datetime import datetime


@dataclass
class ModelCandidate:
    """A candidate model for selection."""
    name: str
    model_type: str
    score: float
    metrics: Dict[str, float]
    training_time: float
    inference_time: float
    memory_usage: int
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class ModelSelector:
    """
    Automatic model selection based on multi-objective criteria.
    
    Evaluates and ranks models based on accuracy, speed,
    memory usage, and other configurable metrics.
    """
    
    def __init__(
        self,
        criteria_weights: Optional[Dict[str, float]] = None
    ):
        self.criteria_weights = criteria_weights or {
            "accuracy": 0.5,
            "speed": 0.2,
            "memory": 0.15,
            "stability": 0.15
        }
        
        self.candidates: List[ModelCandidate] = []
        self.selected_model: Optional[ModelCandidate] = None
    
    def add_candidate(
        self,
        name: str,
        model_type: str,
        metrics: Dict[str, float],
        training_time: float,
        inference_time: float,
        memory_usage: int
    ) -> ModelCandidate:
        """
        Add a model candidate for evaluation.
        
        Args:
            name: Model name
            model_type: Type of model (e.g., "neural", "ensemble")
            metrics: Performance metrics dict
            training_time: Training time in seconds
            inference_time: Inference time per sample in ms
            memory_usage: Memory usage in bytes
            
        Returns:
            Created candidate
        """
        # Calculate composite score
        score = self._calculate_score(metrics, inference_time, memory_usage)
        
        candidate = ModelCandidate(
            name=name,
            model_type=model_type,
            score=score,
            metrics=metrics,
            training_time=training_time,
            inference_time=inference_time,
            memory_usage=memory_usage
        )
        
        self.candidates.append(candidate)
        return candidate
    
    def _calculate_score(
        self,
        metrics: Dict[str, float],
        inference_time: float,
        memory_usage: int
    ) -> float:
        """Calculate composite score for a model."""
        score = 0.0
        
        # Accuracy component
        accuracy = metrics.get("accuracy", 0.0)
        f1 = metrics.get("f1_score", accuracy)
        acc_score = (accuracy + f1) / 2
        score += self.criteria_weights.get("accuracy", 0.5) * acc_score
        
        # Speed component (lower is better, normalize to 0-1)
        max_time = 100  # ms
        speed_score = max(0, 1 - inference_time / max_time)
        score += self.criteria_weights.get("speed", 0.2) * speed_score
        
        # Memory component (lower is better)
        max_memory = 1024 * 1024 * 1024  # 1GB
        memory_score = max(0, 1 - memory_usage / max_memory)
        score += self.criteria_weights.get("memory", 0.15) * memory_score
        
        # Stability component (from metrics)
        stability = metrics.get("stability", 0.8)
        score += self.criteria_weights.get("stability", 0.15) * stability
        
        return score
    
    def select_best(self) -> Optional[ModelCandidate]:
        """
        Select the best model based on composite score.
        
        Returns:
            Best model candidate or None
        """
        if not self.candidates:
            return None
        
        self.selected_model = max(self.candidates, key=lambda c: c.score)
        return self.selected_model
    
    def get_ranking(self) -> List[ModelCandidate]:
        """Get candidates ranked by score."""
        return sorted(self.candidates, key=lambda c: c.score, reverse=True)
    
    def compare(self, model_a: str, model_b: str) -> Dict[str, Any]:
        """
        Compare two models.
        
        Args:
            model_a: First model name
            model_b: Second model name
            
        Returns:
            Comparison results
        """
        candidate_a = next((c for c in self.candidates if c.name == model_a), None)
        candidate_b = next((c for c in self.candidates if c.name == model_b), None)
        
        if not candidate_a or not candidate_b:
            return {"error": "Model not found"}
        
        return {
            "winner": model_a if candidate_a.score > candidate_b.score else model_b,
            "score_difference": abs(candidate_a.score - candidate_b.score),
            "comparisons": {
                "accuracy": {
                    model_a: candidate_a.metrics.get("accuracy", 0),
                    model_b: candidate_b.metrics.get("accuracy", 0)
                },
                "inference_time": {
                    model_a: candidate_a.inference_time,
                    model_b: candidate_b.inference_time
                },
                "memory_usage": {
                    model_a: candidate_a.memory_usage,
                    model_b: candidate_b.memory_usage
                }
            }
        }
    
    def pareto_frontier(self) -> List[ModelCandidate]:
        """
        Find Pareto-optimal models.
        
        Returns models that are not dominated by any other model
        on all criteria.
        """
        pareto = []
        
        for candidate in self.candidates:
            dominated = False
            
            for other in self.candidates:
                if other == candidate:
                    continue
                
                # Check if other dominates candidate
                all_better = all(
                    other.metrics.get(m, 0) >= candidate.metrics.get(m, 0)
                    for m in ["accuracy", "f1_score"]
                )
                faster = other.inference_time <= candidate.inference_time
                smaller = other.memory_usage <= candidate.memory_usage
                
                strictly_better = (
                    other.metrics.get("accuracy", 0) > candidate.metrics.get("accuracy", 0) or
                    other.inference_time < candidate.inference_time or
                    other.memory_usage < candidate.memory_usage
                )
                
                if all_better and faster and smaller and strictly_better:
                    dominated = True
                    break
            
            if not dominated:
                pareto.append(candidate)
        
        return pareto
    
    def get_stats(self) -> Dict[str, Any]:
        """Get selector statistics."""
        if not self.candidates:
            return {"num_candidates": 0}
        
        scores = [c.score for c in self.candidates]
        return {
            "num_candidates": len(self.candidates),
            "selected_model": self.selected_model.name if self.selected_model else None,
            "best_score": max(scores),
            "mean_score": sum(scores) / len(scores),
            "model_types": list(set(c.model_type for c in self.candidates)),
            "pareto_size": len(self.pareto_frontier())
        }
