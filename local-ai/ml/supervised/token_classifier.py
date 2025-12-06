"""
DevTeam6 Token Classifier

Neural network-based token classification for the Dual Transformer system.
Categorizes tokens into API, Security, Data, Local, Hybrid, Control, IO, and General.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
from enum import Enum
import math
import hashlib
import json
from datetime import datetime


class TokenCategory(Enum):
    """Token category classifications."""
    API = "api"
    SECURITY = "security"
    DATA = "data"
    LOCAL = "local"
    HYBRID = "hybrid"
    CONTROL = "control"
    IO = "io"
    GENERAL = "general"


@dataclass
class TokenFeatures:
    """Features extracted from a token for classification."""
    token: str
    length: int
    has_dot: bool
    has_underscore: bool
    is_camelcase: bool
    is_uppercase: bool
    is_keyword: bool
    char_entropy: float
    embedding: List[float] = field(default_factory=list)


@dataclass
class ClassificationResult:
    """Result of token classification."""
    token: str
    category: TokenCategory
    confidence: float
    probabilities: Dict[str, float]
    features: TokenFeatures
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class NeuralLayer:
    """Simple neural network layer with ReLU activation."""
    
    def __init__(self, input_dim: int, output_dim: int, activation: str = "relu"):
        self.input_dim = input_dim
        self.output_dim = output_dim
        self.activation = activation
        
        # Initialize weights using Xavier initialization
        scale = math.sqrt(2.0 / (input_dim + output_dim))
        self.weights = [[self._pseudo_random(i, j) * scale 
                        for j in range(output_dim)] 
                       for i in range(input_dim)]
        self.biases = [0.0 for _ in range(output_dim)]
    
    def _pseudo_random(self, i: int, j: int) -> float:
        """Generate pseudo-random number for reproducibility."""
        seed = hash(f"{i}_{j}") % 10000
        return (seed / 10000.0) * 2 - 1  # Range [-1, 1]
    
    def forward(self, x: List[float]) -> List[float]:
        """Forward pass through the layer."""
        output = []
        for j in range(self.output_dim):
            val = self.biases[j]
            for i in range(min(len(x), self.input_dim)):
                val += x[i] * self.weights[i][j]
            output.append(val)
        
        # Apply activation
        if self.activation == "relu":
            output = [max(0, v) for v in output]
        elif self.activation == "sigmoid":
            output = [1 / (1 + math.exp(-min(max(v, -500), 500))) for v in output]
        elif self.activation == "softmax":
            max_val = max(output)
            exp_vals = [math.exp(v - max_val) for v in output]
            sum_exp = sum(exp_vals)
            output = [v / sum_exp for v in exp_vals]
        
        return output


class TokenClassifier:
    """
    Neural network-based token classifier for the DevTeam6 Dual Transformer.
    
    Uses a multi-layer perceptron to classify tokens into semantic categories
    based on lexical and structural features.
    """
    
    # Keywords by category for feature extraction
    CATEGORY_KEYWORDS = {
        TokenCategory.API: {
            "http", "https", "api", "rest", "graphql", "endpoint", "request",
            "response", "fetch", "get", "post", "put", "delete", "patch",
            "url", "uri", "webhook", "oauth", "token", "header", "query"
        },
        TokenCategory.SECURITY: {
            "auth", "login", "password", "credential", "encrypt", "decrypt",
            "hash", "salt", "jwt", "session", "permission", "role", "admin",
            "secret", "key", "certificate", "ssl", "tls", "firewall", "sanitize"
        },
        TokenCategory.DATA: {
            "database", "sql", "query", "table", "column", "row", "insert",
            "update", "select", "delete", "join", "index", "schema", "migrate",
            "mongodb", "redis", "postgres", "mysql", "sqlite", "nosql"
        },
        TokenCategory.LOCAL: {
            "file", "path", "directory", "folder", "read", "write", "open",
            "close", "save", "load", "local", "disk", "storage", "cache",
            "temp", "buffer", "stream", "io"
        },
        TokenCategory.CONTROL: {
            "if", "else", "for", "while", "switch", "case", "break", "continue",
            "return", "try", "catch", "throw", "async", "await", "promise",
            "loop", "iterate", "condition", "branch"
        },
        TokenCategory.IO: {
            "print", "console", "log", "input", "output", "stdin", "stdout",
            "stderr", "display", "render", "show", "format", "parse", "serialize"
        }
    }
    
    def __init__(
        self,
        embedding_dim: int = 256,
        hidden_dim: int = 512,
        num_classes: int = 8,
        dropout: float = 0.3
    ):
        self.embedding_dim = embedding_dim
        self.hidden_dim = hidden_dim
        self.num_classes = num_classes
        self.dropout = dropout
        
        # Categories
        self.categories = list(TokenCategory)
        self.category_to_idx = {cat: idx for idx, cat in enumerate(self.categories)}
        
        # Neural network layers
        self.layer1 = NeuralLayer(embedding_dim, hidden_dim, "relu")
        self.layer2 = NeuralLayer(hidden_dim, hidden_dim // 2, "relu")
        self.layer3 = NeuralLayer(hidden_dim // 2, num_classes, "softmax")
        
        # Training state
        self.is_trained = False
        self.training_history: List[Dict[str, float]] = []
    
    def _calculate_entropy(self, token: str) -> float:
        """Calculate character-level entropy of a token."""
        if not token:
            return 0.0
        
        char_counts: Dict[str, int] = {}
        for char in token.lower():
            char_counts[char] = char_counts.get(char, 0) + 1
        
        entropy = 0.0
        total = len(token)
        for count in char_counts.values():
            prob = count / total
            if prob > 0:
                entropy -= prob * math.log2(prob)
        
        return entropy
    
    def _is_camelcase(self, token: str) -> bool:
        """Check if token is in camelCase or PascalCase."""
        if not token or len(token) < 2:
            return False
        
        has_lower = any(c.islower() for c in token)
        has_upper = any(c.isupper() for c in token)
        no_spaces = " " not in token
        
        return has_lower and has_upper and no_spaces
    
    def _extract_features(self, token: str) -> TokenFeatures:
        """Extract features from a token for classification."""
        token_lower = token.lower()
        
        # Check for keywords
        is_keyword = False
        for keywords in self.CATEGORY_KEYWORDS.values():
            if token_lower in keywords or any(kw in token_lower for kw in keywords):
                is_keyword = True
                break
        
        return TokenFeatures(
            token=token,
            length=len(token),
            has_dot="." in token,
            has_underscore="_" in token,
            is_camelcase=self._is_camelcase(token),
            is_uppercase=token.isupper() and len(token) > 1,
            is_keyword=is_keyword,
            char_entropy=self._calculate_entropy(token)
        )
    
    def _features_to_embedding(self, features: TokenFeatures) -> List[float]:
        """Convert features to embedding vector."""
        # Create a deterministic embedding based on features
        embedding = [0.0] * self.embedding_dim
        
        # Hash-based embedding for token
        token_hash = int(hashlib.md5(features.token.encode()).hexdigest(), 16)
        for i in range(min(64, self.embedding_dim)):
            embedding[i] = ((token_hash >> i) & 1) * 2 - 1
        
        # Feature-based components
        embedding[64] = features.length / 50.0  # Normalized length
        embedding[65] = 1.0 if features.has_dot else 0.0
        embedding[66] = 1.0 if features.has_underscore else 0.0
        embedding[67] = 1.0 if features.is_camelcase else 0.0
        embedding[68] = 1.0 if features.is_uppercase else 0.0
        embedding[69] = 1.0 if features.is_keyword else 0.0
        embedding[70] = features.char_entropy / 4.0  # Normalized entropy
        
        # Category keyword presence scores
        token_lower = features.token.lower()
        for idx, (category, keywords) in enumerate(self.CATEGORY_KEYWORDS.items()):
            score = sum(1 for kw in keywords if kw in token_lower) / len(keywords)
            if 71 + idx < self.embedding_dim:
                embedding[71 + idx] = score
        
        features.embedding = embedding
        return embedding
    
    def classify(self, token: str) -> ClassificationResult:
        """
        Classify a single token.
        
        Args:
            token: The token string to classify
            
        Returns:
            ClassificationResult with category, confidence, and probabilities
        """
        # Extract features
        features = self._extract_features(token)
        embedding = self._features_to_embedding(features)
        
        # Forward pass through neural network
        h1 = self.layer1.forward(embedding)
        h2 = self.layer2.forward(h1)
        probabilities = self.layer3.forward(h2)
        
        # Apply rule-based adjustments for better accuracy
        token_lower = token.lower()
        adjustments = [0.0] * self.num_classes
        
        for category, keywords in self.CATEGORY_KEYWORDS.items():
            idx = self.category_to_idx.get(category, -1)
            if idx >= 0 and idx < len(adjustments):
                # Check exact match
                if token_lower in keywords:
                    adjustments[idx] += 0.3
                # Check partial match
                elif any(kw in token_lower for kw in keywords):
                    adjustments[idx] += 0.15
                # Check if token contains keyword
                elif any(token_lower in kw for kw in keywords):
                    adjustments[idx] += 0.1
        
        # Apply adjustments and renormalize
        adjusted = [max(0, p + a) for p, a in zip(probabilities, adjustments)]
        total = sum(adjusted)
        if total > 0:
            probabilities = [p / total for p in adjusted]
        
        # Get predicted category
        max_idx = probabilities.index(max(probabilities))
        category = self.categories[max_idx]
        confidence = probabilities[max_idx]
        
        # Build probability dictionary
        prob_dict = {
            cat.value: probabilities[self.category_to_idx[cat]]
            for cat in self.categories
            if self.category_to_idx[cat] < len(probabilities)
        }
        
        return ClassificationResult(
            token=token,
            category=category,
            confidence=confidence,
            probabilities=prob_dict,
            features=features
        )
    
    def classify_batch(self, tokens: List[str]) -> List[ClassificationResult]:
        """Classify multiple tokens in batch."""
        return [self.classify(token) for token in tokens]
    
    def train(
        self,
        training_data: List[Tuple[str, TokenCategory]],
        epochs: int = 100,
        learning_rate: float = 0.001
    ) -> Dict[str, List[float]]:
        """
        Train the classifier on labeled data.
        
        Args:
            training_data: List of (token, category) tuples
            epochs: Number of training epochs
            learning_rate: Learning rate for optimization
            
        Returns:
            Training history with loss and accuracy per epoch
        """
        history = {"loss": [], "accuracy": []}
        
        for epoch in range(epochs):
            total_loss = 0.0
            correct = 0
            
            for token, true_category in training_data:
                # Forward pass
                result = self.classify(token)
                
                # Calculate loss (cross-entropy approximation)
                true_idx = self.category_to_idx[true_category]
                pred_prob = result.probabilities.get(true_category.value, 0.001)
                loss = -math.log(max(pred_prob, 1e-10))
                total_loss += loss
                
                # Check accuracy
                if result.category == true_category:
                    correct += 1
                
                # Simple gradient update (simplified backprop)
                # In production, use proper autodiff library
                error = 1.0 if result.category != true_category else 0.0
                self._update_weights(learning_rate * error)
            
            avg_loss = total_loss / len(training_data) if training_data else 0
            accuracy = correct / len(training_data) if training_data else 0
            
            history["loss"].append(avg_loss)
            history["accuracy"].append(accuracy)
            
            self.training_history.append({
                "epoch": epoch,
                "loss": avg_loss,
                "accuracy": accuracy
            })
        
        self.is_trained = True
        return history
    
    def _update_weights(self, delta: float) -> None:
        """Simple weight update (placeholder for proper backprop)."""
        # In a real implementation, this would use proper gradient descent
        # This is a simplified version for demonstration
        for layer in [self.layer1, self.layer2, self.layer3]:
            for i in range(len(layer.weights)):
                for j in range(len(layer.weights[i])):
                    # Small random perturbation
                    layer.weights[i][j] += delta * (hash(f"{i}{j}") % 100 - 50) / 10000
    
    def save_model(self, path: str) -> None:
        """Save model weights to file."""
        model_data = {
            "embedding_dim": self.embedding_dim,
            "hidden_dim": self.hidden_dim,
            "num_classes": self.num_classes,
            "is_trained": self.is_trained,
            "training_history": self.training_history,
            "layer1_weights": self.layer1.weights,
            "layer2_weights": self.layer2.weights,
            "layer3_weights": self.layer3.weights,
        }
        with open(path, "w") as f:
            json.dump(model_data, f)
    
    def load_model(self, path: str) -> None:
        """Load model weights from file."""
        with open(path, "r") as f:
            model_data = json.load(f)
        
        self.is_trained = model_data.get("is_trained", False)
        self.training_history = model_data.get("training_history", [])
        
        if "layer1_weights" in model_data:
            self.layer1.weights = model_data["layer1_weights"]
        if "layer2_weights" in model_data:
            self.layer2.weights = model_data["layer2_weights"]
        if "layer3_weights" in model_data:
            self.layer3.weights = model_data["layer3_weights"]
    
    def get_metrics(self) -> Dict[str, Any]:
        """Get classifier metrics."""
        return {
            "is_trained": self.is_trained,
            "embedding_dim": self.embedding_dim,
            "hidden_dim": self.hidden_dim,
            "num_classes": self.num_classes,
            "num_categories": len(self.categories),
            "training_epochs": len(self.training_history),
            "last_accuracy": self.training_history[-1]["accuracy"] if self.training_history else None
        }
