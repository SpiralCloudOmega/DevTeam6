"""
DevTeam6 Ensemble Classifier

Combines multiple classifiers for improved accuracy and robustness.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
import math
import json
from datetime import datetime

from .base_classifier import BaseClassifier, TrainingConfig, TrainingMetrics
from .neural_classifier import NeuralClassifier, NeuralConfig


class EnsembleStrategy:
    """Ensemble combination strategies."""
    VOTING = "voting"
    AVERAGE = "average"
    WEIGHTED_AVERAGE = "weighted_average"
    STACKING = "stacking"
    MAX_CONFIDENCE = "max_confidence"


@dataclass
class EnsembleConfig:
    """Configuration for ensemble classifier."""
    num_classifiers: int = 5
    strategy: str = EnsembleStrategy.WEIGHTED_AVERAGE
    diversity_factor: float = 0.3
    use_bootstrap: bool = True


class EnsembleClassifier(BaseClassifier):
    """
    Ensemble classifier combining multiple base classifiers.
    
    Supports voting, averaging, weighted averaging, and stacking strategies
    for combining predictions from multiple models.
    """
    
    def __init__(
        self,
        name: str = "ensemble_classifier",
        num_classes: int = 8,
        config: Optional[EnsembleConfig] = None
    ):
        super().__init__(name, num_classes)
        
        self.config = config or EnsembleConfig()
        self.strategy = self.config.strategy
        
        # Base classifiers
        self.classifiers: List[BaseClassifier] = []
        self.classifier_weights: List[float] = []
        
        # Initialize base classifiers
        self._initialize_classifiers()
    
    def _initialize_classifiers(self) -> None:
        """Initialize base classifiers with diverse configurations."""
        for i in range(self.config.num_classifiers):
            # Create diverse architectures
            hidden_dims = self._get_diverse_architecture(i)
            
            config = NeuralConfig(
                input_dim=256,
                hidden_dims=hidden_dims,
                output_dim=self.num_classes,
                dropout=0.2 + i * 0.05  # Varying dropout
            )
            
            classifier = NeuralClassifier(
                name=f"base_classifier_{i}",
                config=config
            )
            
            self.classifiers.append(classifier)
            self.classifier_weights.append(1.0 / self.config.num_classifiers)
    
    def _get_diverse_architecture(self, index: int) -> List[int]:
        """Get diverse hidden layer configuration."""
        architectures = [
            [512, 256],
            [256, 128],
            [512, 256, 128],
            [384, 192],
            [256, 256]
        ]
        return architectures[index % len(architectures)]
    
    def predict(self, features: List[float]) -> Tuple[int, float]:
        """Make ensemble prediction."""
        proba = self.predict_proba(features)
        class_idx = proba.index(max(proba))
        confidence = proba[class_idx]
        
        self.prediction_count += 1
        return class_idx, confidence
    
    def predict_proba(self, features: List[float]) -> List[float]:
        """Get ensemble probability distribution."""
        if not self.classifiers:
            return [1.0 / self.num_classes] * self.num_classes
        
        all_proba = [clf.predict_proba(features) for clf in self.classifiers]
        
        if self.strategy == EnsembleStrategy.VOTING:
            return self._voting(all_proba)
        elif self.strategy == EnsembleStrategy.AVERAGE:
            return self._average(all_proba)
        elif self.strategy == EnsembleStrategy.WEIGHTED_AVERAGE:
            return self._weighted_average(all_proba)
        elif self.strategy == EnsembleStrategy.MAX_CONFIDENCE:
            return self._max_confidence(all_proba)
        else:
            return self._average(all_proba)
    
    def _voting(self, all_proba: List[List[float]]) -> List[float]:
        """Majority voting strategy."""
        votes = [0] * self.num_classes
        
        for proba in all_proba:
            pred = proba.index(max(proba))
            votes[pred] += 1
        
        total_votes = sum(votes)
        return [v / total_votes for v in votes]
    
    def _average(self, all_proba: List[List[float]]) -> List[float]:
        """Simple averaging strategy."""
        n = len(all_proba)
        result = [0.0] * self.num_classes
        
        for proba in all_proba:
            for i in range(self.num_classes):
                if i < len(proba):
                    result[i] += proba[i]
        
        return [r / n for r in result]
    
    def _weighted_average(self, all_proba: List[List[float]]) -> List[float]:
        """Weighted averaging based on classifier performance."""
        result = [0.0] * self.num_classes
        total_weight = sum(self.classifier_weights)
        
        for proba, weight in zip(all_proba, self.classifier_weights):
            for i in range(self.num_classes):
                if i < len(proba):
                    result[i] += proba[i] * weight
        
        return [r / total_weight for r in result]
    
    def _max_confidence(self, all_proba: List[List[float]]) -> List[float]:
        """Use prediction from most confident classifier."""
        max_conf = 0.0
        best_proba = all_proba[0] if all_proba else [1.0 / self.num_classes] * self.num_classes
        
        for proba in all_proba:
            conf = max(proba)
            if conf > max_conf:
                max_conf = conf
                best_proba = proba
        
        return best_proba
    
    def train(
        self,
        X: List[List[float]],
        y: List[int],
        config: TrainingConfig
    ) -> List[TrainingMetrics]:
        """Train ensemble classifiers."""
        all_history = []
        
        for i, clf in enumerate(self.classifiers):
            print(f"Training classifier {i + 1}/{len(self.classifiers)}...")
            
            # Bootstrap sampling if enabled
            if self.config.use_bootstrap:
                X_train, y_train = self._bootstrap_sample(X, y)
            else:
                X_train, y_train = X, y
            
            history = clf.train(X_train, y_train, config)
            all_history.extend(history)
            
            # Update classifier weight based on validation accuracy
            if history:
                final_acc = history[-1].val_accuracy or history[-1].train_accuracy
                self.classifier_weights[i] = max(final_acc, 0.1)
        
        # Normalize weights
        total = sum(self.classifier_weights)
        self.classifier_weights = [w / total for w in self.classifier_weights]
        
        self.is_trained = True
        self.training_history = all_history
        return all_history
    
    def _bootstrap_sample(
        self,
        X: List[List[float]],
        y: List[int]
    ) -> Tuple[List[List[float]], List[int]]:
        """Create bootstrap sample of training data."""
        import random
        n = len(X)
        indices = [random.randint(0, n - 1) for _ in range(n)]
        X_sample = [X[i] for i in indices]
        y_sample = [y[i] for i in indices]
        return X_sample, y_sample
    
    def add_classifier(self, classifier: BaseClassifier, weight: float = 1.0) -> None:
        """Add a pre-trained classifier to the ensemble."""
        self.classifiers.append(classifier)
        self.classifier_weights.append(weight)
        
        # Renormalize weights
        total = sum(self.classifier_weights)
        self.classifier_weights = [w / total for w in self.classifier_weights]
    
    def remove_classifier(self, index: int) -> None:
        """Remove a classifier from the ensemble."""
        if 0 <= index < len(self.classifiers):
            self.classifiers.pop(index)
            self.classifier_weights.pop(index)
            
            # Renormalize weights
            if self.classifier_weights:
                total = sum(self.classifier_weights)
                self.classifier_weights = [w / total for w in self.classifier_weights]
    
    def get_classifier_stats(self) -> List[Dict[str, Any]]:
        """Get statistics for each base classifier."""
        stats = []
        for i, (clf, weight) in enumerate(zip(self.classifiers, self.classifier_weights)):
            stats.append({
                "index": i,
                "name": clf.name,
                "weight": weight,
                "is_trained": clf.is_trained,
                "predictions": clf.prediction_count
            })
        return stats
    
    def save(self, path: str) -> None:
        """Save ensemble to file."""
        ensemble_data = {
            "name": self.name,
            "num_classes": self.num_classes,
            "strategy": self.strategy,
            "classifier_weights": self.classifier_weights,
            "class_names": self._class_names,
            "is_trained": self.is_trained,
            "config": {
                "num_classifiers": self.config.num_classifiers,
                "strategy": self.config.strategy,
                "diversity_factor": self.config.diversity_factor,
                "use_bootstrap": self.config.use_bootstrap
            }
        }
        
        # Save base classifiers
        for i, clf in enumerate(self.classifiers):
            clf.save(f"{path}.clf_{i}")
        
        with open(path, "w") as f:
            json.dump(ensemble_data, f)
    
    def load(self, path: str) -> None:
        """Load ensemble from file."""
        with open(path, "r") as f:
            ensemble_data = json.load(f)
        
        self.strategy = ensemble_data.get("strategy", EnsembleStrategy.WEIGHTED_AVERAGE)
        self.classifier_weights = ensemble_data.get("classifier_weights", [])
        self._class_names = ensemble_data.get("class_names", [])
        self.is_trained = ensemble_data.get("is_trained", True)
        
        # Load base classifiers
        self.classifiers = []
        for i in range(len(self.classifier_weights)):
            clf = NeuralClassifier(name=f"base_classifier_{i}")
            try:
                clf.load(f"{path}.clf_{i}")
                self.classifiers.append(clf)
            except FileNotFoundError:
                pass
