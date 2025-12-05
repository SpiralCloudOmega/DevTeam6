"""
DevTeam6 Neural Classifier

Multi-layer perceptron classifier with configurable architecture.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
import math
import json
import random
from datetime import datetime

from .base_classifier import BaseClassifier, TrainingConfig, TrainingMetrics


@dataclass
class NeuralConfig:
    """Configuration for neural classifier architecture."""
    input_dim: int = 256
    hidden_dims: List[int] = field(default_factory=lambda: [512, 256])
    output_dim: int = 8
    activation: str = "relu"
    dropout: float = 0.3
    use_batch_norm: bool = True


class NeuralClassifier(BaseClassifier):
    """
    Multi-layer perceptron classifier for the DevTeam6 ML layer.
    
    Implements a feedforward neural network with configurable depth,
    width, activation functions, and regularization.
    """
    
    def __init__(
        self,
        name: str = "neural_classifier",
        config: Optional[NeuralConfig] = None
    ):
        config = config or NeuralConfig()
        super().__init__(name, config.output_dim)
        
        self.config = config
        self.input_dim = config.input_dim
        self.hidden_dims = config.hidden_dims
        self.output_dim = config.output_dim
        self.activation = config.activation
        self.dropout = config.dropout
        
        # Initialize layers
        self.layers: List[Dict[str, List[List[float]]]] = []
        self._initialize_weights()
    
    def _initialize_weights(self) -> None:
        """Initialize network weights using Xavier initialization."""
        dims = [self.input_dim] + self.hidden_dims + [self.output_dim]
        
        for i in range(len(dims) - 1):
            fan_in = dims[i]
            fan_out = dims[i + 1]
            scale = math.sqrt(2.0 / (fan_in + fan_out))
            
            weights = [
                [self._xavier_init(scale) for _ in range(fan_out)]
                for _ in range(fan_in)
            ]
            biases = [0.0 for _ in range(fan_out)]
            
            self.layers.append({
                "weights": weights,
                "biases": biases,
                "fan_in": fan_in,
                "fan_out": fan_out
            })
    
    def _xavier_init(self, scale: float) -> float:
        """Xavier weight initialization."""
        return (random.random() * 2 - 1) * scale
    
    def _activate(self, x: float, activation: str) -> float:
        """Apply activation function."""
        if activation == "relu":
            return max(0, x)
        elif activation == "sigmoid":
            return 1 / (1 + math.exp(-min(max(x, -500), 500)))
        elif activation == "tanh":
            return math.tanh(x)
        elif activation == "leaky_relu":
            return x if x > 0 else 0.01 * x
        else:
            return x
    
    def _softmax(self, x: List[float]) -> List[float]:
        """Apply softmax to convert logits to probabilities."""
        max_x = max(x)
        exp_x = [math.exp(xi - max_x) for xi in x]
        sum_exp = sum(exp_x)
        return [e / sum_exp for e in exp_x]
    
    def _forward(self, x: List[float], training: bool = False) -> Tuple[List[float], List[List[float]]]:
        """
        Forward pass through the network.
        
        Args:
            x: Input features
            training: Whether in training mode (applies dropout)
            
        Returns:
            Tuple of (output, intermediate_activations)
        """
        activations = [x]
        current = x
        
        for i, layer in enumerate(self.layers):
            weights = layer["weights"]
            biases = layer["biases"]
            fan_out = layer["fan_out"]
            
            # Matrix multiplication
            output = [0.0] * fan_out
            for j in range(fan_out):
                for k in range(min(len(current), len(weights))):
                    output[j] += current[k] * weights[k][j]
                output[j] += biases[j]
            
            # Activation (except for last layer)
            if i < len(self.layers) - 1:
                output = [self._activate(o, self.activation) for o in output]
                
                # Dropout during training
                if training and self.dropout > 0:
                    output = [
                        o / (1 - self.dropout) if random.random() > self.dropout else 0
                        for o in output
                    ]
            
            activations.append(output)
            current = output
        
        # Softmax on final layer
        output = self._softmax(current)
        return output, activations
    
    def predict(self, features: List[float]) -> Tuple[int, float]:
        """Make a prediction."""
        proba = self.predict_proba(features)
        class_idx = proba.index(max(proba))
        confidence = proba[class_idx]
        
        self.prediction_count += 1
        return class_idx, confidence
    
    def predict_proba(self, features: List[float]) -> List[float]:
        """Get probability distribution over classes."""
        # Pad or truncate features to match input dimension
        if len(features) < self.input_dim:
            features = features + [0.0] * (self.input_dim - len(features))
        elif len(features) > self.input_dim:
            features = features[:self.input_dim]
        
        output, _ = self._forward(features, training=False)
        return output
    
    def train(
        self,
        X: List[List[float]],
        y: List[int],
        config: TrainingConfig
    ) -> List[TrainingMetrics]:
        """Train the classifier."""
        history = []
        n_samples = len(X)
        
        # Split data for validation
        if config.validation_split > 0:
            split_idx = int(n_samples * (1 - config.validation_split))
            X_train, X_val = X[:split_idx], X[split_idx:]
            y_train, y_val = y[:split_idx], y[split_idx:]
        else:
            X_train, X_val = X, []
            y_train, y_val = y, []
        
        best_val_loss = float('inf')
        patience_counter = 0
        
        for epoch in range(config.epochs):
            # Training
            total_loss = 0.0
            correct = 0
            
            indices = list(range(len(X_train)))
            if config.shuffle:
                random.shuffle(indices)
            
            for idx in indices:
                x = X_train[idx]
                target = y_train[idx]
                
                # Pad features
                if len(x) < self.input_dim:
                    x = x + [0.0] * (self.input_dim - len(x))
                elif len(x) > self.input_dim:
                    x = x[:self.input_dim]
                
                # Forward pass
                proba, activations = self._forward(x, training=True)
                
                # Calculate loss (cross-entropy)
                target_prob = proba[target] if target < len(proba) else 0.001
                loss = -math.log(max(target_prob, 1e-10))
                total_loss += loss
                
                # Check accuracy
                pred = proba.index(max(proba))
                if pred == target:
                    correct += 1
                
                # Backpropagation (simplified gradient descent)
                self._backprop(activations, proba, target, config.learning_rate)
            
            train_loss = total_loss / len(X_train) if X_train else 0
            train_acc = correct / len(X_train) if X_train else 0
            
            # Validation
            val_loss = None
            val_acc = None
            if X_val:
                val_total_loss = 0.0
                val_correct = 0
                
                for x, target in zip(X_val, y_val):
                    if len(x) < self.input_dim:
                        x = x + [0.0] * (self.input_dim - len(x))
                    
                    proba, _ = self._forward(x, training=False)
                    target_prob = proba[target] if target < len(proba) else 0.001
                    val_total_loss += -math.log(max(target_prob, 1e-10))
                    
                    pred = proba.index(max(proba))
                    if pred == target:
                        val_correct += 1
                
                val_loss = val_total_loss / len(X_val)
                val_acc = val_correct / len(X_val)
                
                # Early stopping check
                if val_loss < best_val_loss:
                    best_val_loss = val_loss
                    patience_counter = 0
                else:
                    patience_counter += 1
                    if patience_counter >= config.early_stopping_patience:
                        if config.verbose:
                            print(f"Early stopping at epoch {epoch}")
                        break
            
            metrics = TrainingMetrics(
                epoch=epoch,
                train_loss=train_loss,
                train_accuracy=train_acc,
                val_loss=val_loss,
                val_accuracy=val_acc
            )
            history.append(metrics)
            self.training_history.append(metrics)
            
            if config.verbose and epoch % 10 == 0:
                print(f"Epoch {epoch}: loss={train_loss:.4f}, acc={train_acc:.4f}")
        
        self.is_trained = True
        return history
    
    def _backprop(
        self,
        activations: List[List[float]],
        output: List[float],
        target: int,
        learning_rate: float
    ) -> None:
        """Simplified backpropagation."""
        # Calculate output error
        error = output.copy()
        if target < len(error):
            error[target] -= 1.0
        
        # Propagate error backward and update weights
        for i in range(len(self.layers) - 1, -1, -1):
            layer = self.layers[i]
            prev_activation = activations[i]
            
            # Update weights
            for j in range(len(layer["weights"])):
                for k in range(len(layer["weights"][j])):
                    if j < len(prev_activation) and k < len(error):
                        gradient = prev_activation[j] * error[k]
                        layer["weights"][j][k] -= learning_rate * gradient
            
            # Update biases
            for k in range(len(layer["biases"])):
                if k < len(error):
                    layer["biases"][k] -= learning_rate * error[k]
            
            # Propagate error to previous layer
            if i > 0:
                new_error = [0.0] * layer["fan_in"]
                for j in range(layer["fan_in"]):
                    for k in range(min(len(error), len(layer["weights"][j]))):
                        new_error[j] += error[k] * layer["weights"][j][k]
                    # ReLU derivative
                    if self.activation == "relu" and j < len(prev_activation):
                        new_error[j] *= 1 if prev_activation[j] > 0 else 0
                error = new_error
    
    def save(self, path: str) -> None:
        """Save model to file."""
        model_data = {
            "name": self.name,
            "config": {
                "input_dim": self.config.input_dim,
                "hidden_dims": self.config.hidden_dims,
                "output_dim": self.config.output_dim,
                "activation": self.config.activation,
                "dropout": self.config.dropout
            },
            "layers": self.layers,
            "class_names": self._class_names,
            "is_trained": self.is_trained
        }
        with open(path, "w") as f:
            json.dump(model_data, f)
    
    def load(self, path: str) -> None:
        """Load model from file."""
        with open(path, "r") as f:
            model_data = json.load(f)
        
        self.layers = model_data["layers"]
        self._class_names = model_data.get("class_names", [])
        self.is_trained = model_data.get("is_trained", True)
