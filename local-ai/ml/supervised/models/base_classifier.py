"""
DevTeam6 Base Classifier

Abstract base class for all classifiers in the ML layer.
"""

from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any, Generic, TypeVar
from datetime import datetime
import json


T = TypeVar('T')  # Type for classification result


@dataclass
class TrainingConfig:
    """Configuration for training a classifier."""
    epochs: int = 100
    batch_size: int = 32
    learning_rate: float = 0.001
    early_stopping_patience: int = 10
    validation_split: float = 0.2
    shuffle: bool = True
    verbose: bool = True


@dataclass
class TrainingMetrics:
    """Metrics from a training run."""
    epoch: int
    train_loss: float
    train_accuracy: float
    val_loss: Optional[float] = None
    val_accuracy: Optional[float] = None
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


@dataclass
class ClassifierMetrics:
    """Overall classifier metrics."""
    total_predictions: int
    correct_predictions: int
    accuracy: float
    precision: Dict[str, float]
    recall: Dict[str, float]
    f1_scores: Dict[str, float]
    confusion_matrix: Dict[str, Dict[str, int]]


class BaseClassifier(ABC):
    """
    Abstract base class for classifiers in the DevTeam6 ML layer.
    
    Provides common interface and utilities for all classifier implementations.
    """
    
    def __init__(self, name: str, num_classes: int):
        self.name = name
        self.num_classes = num_classes
        self.is_trained = False
        self.training_history: List[TrainingMetrics] = []
        self.prediction_count = 0
        self.correct_count = 0
        self._class_names: List[str] = []
    
    @property
    def class_names(self) -> List[str]:
        """Get class names."""
        return self._class_names
    
    @class_names.setter
    def class_names(self, names: List[str]) -> None:
        """Set class names."""
        if len(names) != self.num_classes:
            raise ValueError(f"Expected {self.num_classes} class names, got {len(names)}")
        self._class_names = names
    
    @abstractmethod
    def predict(self, features: List[float]) -> Tuple[int, float]:
        """
        Make a prediction on input features.
        
        Args:
            features: Input feature vector
            
        Returns:
            Tuple of (predicted_class_index, confidence)
        """
        pass
    
    @abstractmethod
    def predict_proba(self, features: List[float]) -> List[float]:
        """
        Get probability distribution over classes.
        
        Args:
            features: Input feature vector
            
        Returns:
            List of probabilities for each class
        """
        pass
    
    @abstractmethod
    def train(
        self,
        X: List[List[float]],
        y: List[int],
        config: TrainingConfig
    ) -> List[TrainingMetrics]:
        """
        Train the classifier.
        
        Args:
            X: Training features (list of feature vectors)
            y: Training labels (list of class indices)
            config: Training configuration
            
        Returns:
            List of training metrics per epoch
        """
        pass
    
    @abstractmethod
    def save(self, path: str) -> None:
        """Save model to file."""
        pass
    
    @abstractmethod
    def load(self, path: str) -> None:
        """Load model from file."""
        pass
    
    def predict_class_name(self, features: List[float]) -> Tuple[str, float]:
        """
        Predict class name instead of index.
        
        Args:
            features: Input feature vector
            
        Returns:
            Tuple of (class_name, confidence)
        """
        class_idx, confidence = self.predict(features)
        class_name = self._class_names[class_idx] if class_idx < len(self._class_names) else f"class_{class_idx}"
        return class_name, confidence
    
    def evaluate(
        self,
        X: List[List[float]],
        y: List[int]
    ) -> ClassifierMetrics:
        """
        Evaluate classifier on test data.
        
        Args:
            X: Test features
            y: Test labels
            
        Returns:
            ClassifierMetrics with accuracy, precision, recall, F1
        """
        predictions = [self.predict(x)[0] for x in X]
        
        total = len(y)
        correct = sum(1 for p, t in zip(predictions, y) if p == t)
        
        # Build confusion matrix
        confusion: Dict[str, Dict[str, int]] = {}
        for true_class in range(self.num_classes):
            class_name = self._get_class_name(true_class)
            confusion[class_name] = {self._get_class_name(i): 0 for i in range(self.num_classes)}
        
        for pred, true in zip(predictions, y):
            pred_name = self._get_class_name(pred)
            true_name = self._get_class_name(true)
            if true_name in confusion and pred_name in confusion[true_name]:
                confusion[true_name][pred_name] += 1
        
        # Calculate precision, recall, F1 per class
        precision = {}
        recall = {}
        f1_scores = {}
        
        for class_idx in range(self.num_classes):
            class_name = self._get_class_name(class_idx)
            
            # True positives
            tp = sum(1 for p, t in zip(predictions, y) if p == class_idx and t == class_idx)
            # False positives
            fp = sum(1 for p, t in zip(predictions, y) if p == class_idx and t != class_idx)
            # False negatives
            fn = sum(1 for p, t in zip(predictions, y) if p != class_idx and t == class_idx)
            
            prec = tp / (tp + fp) if (tp + fp) > 0 else 0.0
            rec = tp / (tp + fn) if (tp + fn) > 0 else 0.0
            f1 = 2 * prec * rec / (prec + rec) if (prec + rec) > 0 else 0.0
            
            precision[class_name] = prec
            recall[class_name] = rec
            f1_scores[class_name] = f1
        
        return ClassifierMetrics(
            total_predictions=total,
            correct_predictions=correct,
            accuracy=correct / total if total > 0 else 0.0,
            precision=precision,
            recall=recall,
            f1_scores=f1_scores,
            confusion_matrix=confusion
        )
    
    def _get_class_name(self, idx: int) -> str:
        """Get class name by index."""
        if idx < len(self._class_names):
            return self._class_names[idx]
        return f"class_{idx}"
    
    def get_summary(self) -> Dict[str, Any]:
        """Get model summary."""
        return {
            "name": self.name,
            "num_classes": self.num_classes,
            "class_names": self._class_names,
            "is_trained": self.is_trained,
            "training_epochs": len(self.training_history),
            "total_predictions": self.prediction_count,
            "accuracy": self.correct_count / self.prediction_count if self.prediction_count > 0 else 0.0
        }
