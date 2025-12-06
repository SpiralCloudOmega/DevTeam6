"""
DevTeam6 Ensemble System

Advanced ensemble methods for combining multiple models.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Callable, Tuple
import math
import random
from datetime import datetime


@dataclass
class EnsembleConfig:
    """Configuration for ensemble system."""
    num_models: int = 5
    strategy: str = "weighted_vote"  # vote, average, stacking, weighted_vote
    diversity_threshold: float = 0.3
    min_confidence: float = 0.5


@dataclass
class ModelPrediction:
    """Prediction from a single model."""
    model_id: int
    class_idx: int
    confidence: float
    probabilities: List[float]


class EnsembleSystem:
    """
    Advanced ensemble system for combining multiple models.
    
    Implements voting, averaging, stacking, and dynamic
    weighting strategies for robust predictions.
    """
    
    def __init__(self, config: Optional[EnsembleConfig] = None):
        self.config = config or EnsembleConfig()
        
        # Model registry
        self.models: Dict[int, Callable[[List[float]], List[float]]] = {}
        self.model_weights: Dict[int, float] = {}
        self.model_performance: Dict[int, List[float]] = {}
        
        # Stacking meta-learner
        self.meta_weights: List[List[float]] = []
        self.num_classes = 0
        
        # History
        self.prediction_history: List[Dict[str, Any]] = []
    
    def register_model(
        self,
        model_id: int,
        predict_fn: Callable[[List[float]], List[float]],
        initial_weight: float = 1.0
    ) -> None:
        """
        Register a model in the ensemble.
        
        Args:
            model_id: Unique model identifier
            predict_fn: Function that takes features and returns probabilities
            initial_weight: Initial model weight
        """
        self.models[model_id] = predict_fn
        self.model_weights[model_id] = initial_weight
        self.model_performance[model_id] = []
    
    def predict(self, features: List[float]) -> Tuple[int, float, Dict[str, Any]]:
        """
        Make ensemble prediction.
        
        Args:
            features: Input feature vector
            
        Returns:
            Tuple of (class_idx, confidence, details)
        """
        if not self.models:
            return 0, 0.0, {"error": "No models registered"}
        
        # Get predictions from all models
        predictions = []
        for model_id, predict_fn in self.models.items():
            try:
                probs = predict_fn(features)
                class_idx = probs.index(max(probs))
                confidence = probs[class_idx]
                
                predictions.append(ModelPrediction(
                    model_id=model_id,
                    class_idx=class_idx,
                    confidence=confidence,
                    probabilities=probs
                ))
            except Exception:
                continue
        
        if not predictions:
            return 0, 0.0, {"error": "All models failed"}
        
        # Update num_classes
        if predictions:
            self.num_classes = len(predictions[0].probabilities)
        
        # Combine predictions based on strategy
        if self.config.strategy == "vote":
            result = self._vote_combine(predictions)
        elif self.config.strategy == "average":
            result = self._average_combine(predictions)
        elif self.config.strategy == "stacking":
            result = self._stacking_combine(predictions, features)
        elif self.config.strategy == "weighted_vote":
            result = self._weighted_vote_combine(predictions)
        else:
            result = self._weighted_vote_combine(predictions)
        
        class_idx, confidence, combined_probs = result
        
        # Record prediction
        self.prediction_history.append({
            "features_hash": hash(tuple(features[:10])),
            "class_idx": class_idx,
            "confidence": confidence,
            "num_models_used": len(predictions),
            "timestamp": datetime.now().isoformat()
        })
        
        details = {
            "num_models": len(predictions),
            "combined_probabilities": combined_probs,
            "model_predictions": [
                {"model_id": p.model_id, "class": p.class_idx, "confidence": p.confidence}
                for p in predictions
            ],
            "agreement": self._calculate_agreement(predictions)
        }
        
        return class_idx, confidence, details
    
    def _vote_combine(
        self,
        predictions: List[ModelPrediction]
    ) -> Tuple[int, float, List[float]]:
        """Simple majority voting."""
        votes: Dict[int, int] = {}
        
        for pred in predictions:
            if pred.confidence >= self.config.min_confidence:
                votes[pred.class_idx] = votes.get(pred.class_idx, 0) + 1
        
        if not votes:
            # Fall back to most confident prediction
            best = max(predictions, key=lambda p: p.confidence)
            return best.class_idx, best.confidence, best.probabilities
        
        winner = max(votes.items(), key=lambda x: x[1])
        confidence = winner[1] / len(predictions)
        
        # Construct probability distribution from votes
        probs = [0.0] * self.num_classes
        for class_idx, count in votes.items():
            if class_idx < len(probs):
                probs[class_idx] = count / len(predictions)
        
        return winner[0], confidence, probs
    
    def _average_combine(
        self,
        predictions: List[ModelPrediction]
    ) -> Tuple[int, float, List[float]]:
        """Average probability combining."""
        combined = [0.0] * self.num_classes
        
        for pred in predictions:
            for i, prob in enumerate(pred.probabilities):
                if i < len(combined):
                    combined[i] += prob
        
        # Normalize
        combined = [p / len(predictions) for p in combined]
        
        class_idx = combined.index(max(combined))
        confidence = combined[class_idx]
        
        return class_idx, confidence, combined
    
    def _weighted_vote_combine(
        self,
        predictions: List[ModelPrediction]
    ) -> Tuple[int, float, List[float]]:
        """Weighted averaging based on model performance."""
        combined = [0.0] * self.num_classes
        total_weight = 0.0
        
        for pred in predictions:
            weight = self.model_weights.get(pred.model_id, 1.0)
            
            for i, prob in enumerate(pred.probabilities):
                if i < len(combined):
                    combined[i] += prob * weight
            
            total_weight += weight
        
        # Normalize
        if total_weight > 0:
            combined = [p / total_weight for p in combined]
        
        class_idx = combined.index(max(combined))
        confidence = combined[class_idx]
        
        return class_idx, confidence, combined
    
    def _stacking_combine(
        self,
        predictions: List[ModelPrediction],
        features: List[float]
    ) -> Tuple[int, float, List[float]]:
        """Stacking with meta-learner."""
        # Create meta-features from model predictions
        meta_features = []
        for pred in predictions:
            meta_features.extend(pred.probabilities)
        
        # If meta-learner not trained, fall back to weighted average
        if not self.meta_weights:
            return self._weighted_vote_combine(predictions)
        
        # Apply meta-learner
        combined = [0.0] * self.num_classes
        for i, w in enumerate(self.meta_weights):
            if i < len(meta_features):
                for j, wj in enumerate(w):
                    if j < len(combined):
                        combined[j] += meta_features[i] * wj
        
        # Softmax
        max_c = max(combined)
        exp_c = [math.exp(c - max_c) for c in combined]
        sum_exp = sum(exp_c)
        combined = [e / sum_exp for e in exp_c]
        
        class_idx = combined.index(max(combined))
        confidence = combined[class_idx]
        
        return class_idx, confidence, combined
    
    def _calculate_agreement(self, predictions: List[ModelPrediction]) -> float:
        """Calculate agreement among models."""
        if len(predictions) < 2:
            return 1.0
        
        classes = [p.class_idx for p in predictions]
        most_common = max(set(classes), key=classes.count)
        agreement = classes.count(most_common) / len(classes)
        
        return agreement
    
    def update_weights(
        self,
        true_label: int,
        predictions: List[ModelPrediction]
    ) -> None:
        """Update model weights based on performance."""
        for pred in predictions:
            correct = 1.0 if pred.class_idx == true_label else 0.0
            
            # Update performance history
            if pred.model_id not in self.model_performance:
                self.model_performance[pred.model_id] = []
            self.model_performance[pred.model_id].append(correct)
            
            # Calculate running accuracy
            history = self.model_performance[pred.model_id][-100:]
            accuracy = sum(history) / len(history)
            
            # Update weight (exponential smoothing)
            current_weight = self.model_weights.get(pred.model_id, 1.0)
            new_weight = 0.9 * current_weight + 0.1 * (accuracy + 0.1)
            self.model_weights[pred.model_id] = new_weight
    
    def train_meta_learner(
        self,
        train_data: List[Tuple[List[float], int]]
    ) -> None:
        """Train stacking meta-learner."""
        if not self.models or not train_data:
            return
        
        # Collect meta-features
        meta_features_list = []
        labels = []
        
        for features, label in train_data:
            meta_features = []
            for model_id, predict_fn in self.models.items():
                try:
                    probs = predict_fn(features)
                    meta_features.extend(probs)
                except Exception:
                    meta_features.extend([0.0] * self.num_classes)
            
            meta_features_list.append(meta_features)
            labels.append(label)
        
        # Simple linear meta-learner training
        if meta_features_list:
            input_dim = len(meta_features_list[0])
            self.meta_weights = [
                [random.uniform(-0.1, 0.1) for _ in range(self.num_classes)]
                for _ in range(input_dim)
            ]
            
            # Simple gradient descent
            lr = 0.01
            for _ in range(100):
                for features, label in zip(meta_features_list, labels):
                    # Forward
                    output = [0.0] * self.num_classes
                    for i, w in enumerate(self.meta_weights):
                        if i < len(features):
                            for j in range(self.num_classes):
                                output[j] += features[i] * w[j]
                    
                    # Softmax
                    max_o = max(output)
                    exp_o = [math.exp(o - max_o) for o in output]
                    sum_exp = sum(exp_o)
                    probs = [e / sum_exp for e in exp_o]
                    
                    # Gradient
                    for i in range(len(self.meta_weights)):
                        if i < len(features):
                            for j in range(self.num_classes):
                                target = 1.0 if j == label else 0.0
                                grad = (probs[j] - target) * features[i]
                                self.meta_weights[i][j] -= lr * grad
    
    def get_diversity_score(self) -> float:
        """Calculate diversity among ensemble members."""
        if len(self.prediction_history) < 10:
            return 0.0
        
        # Calculate pairwise disagreement
        disagreements = 0
        comparisons = 0
        
        for record in self.prediction_history[-100:]:
            preds = [p["class"] for p in record.get("model_predictions", [])]
            for i in range(len(preds)):
                for j in range(i + 1, len(preds)):
                    if preds[i] != preds[j]:
                        disagreements += 1
                    comparisons += 1
        
        return disagreements / comparisons if comparisons > 0 else 0
    
    def get_stats(self) -> Dict[str, Any]:
        """Get ensemble statistics."""
        return {
            "num_models": len(self.models),
            "strategy": self.config.strategy,
            "model_weights": dict(self.model_weights),
            "total_predictions": len(self.prediction_history),
            "diversity_score": self.get_diversity_score(),
            "meta_learner_trained": len(self.meta_weights) > 0
        }
