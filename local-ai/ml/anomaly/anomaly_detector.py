"""
DevTeam6 Anomaly Detector

Ensemble anomaly detection for security threats.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Tuple
from enum import Enum
import math
import random
from datetime import datetime


class AnomalyType(Enum):
    """Types of anomalies detected."""
    NORMAL = "normal"
    OUTLIER = "outlier"
    NOVELTY = "novelty"
    ATTACK = "attack"


@dataclass
class AnomalyResult:
    """Result of anomaly detection."""
    input_data: List[float]
    is_anomaly: bool
    anomaly_score: float  # 0.0 to 1.0
    anomaly_type: AnomalyType
    method_scores: Dict[str, float]
    confidence: float
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


@dataclass
class AnomalyConfig:
    """Configuration for anomaly detection."""
    methods: List[str] = field(default_factory=lambda: ["isolation_forest", "autoencoder", "lof"])
    ensemble_strategy: str = "voting"  # voting, average, max
    threshold: float = 0.7
    alert_on_detection: bool = True


class AnomalyDetector:
    """
    Ensemble anomaly detector for the DevTeam6 security system.
    
    Combines multiple anomaly detection methods (Isolation Forest,
    Autoencoder, LOF) for robust threat identification.
    """
    
    def __init__(self, config: Optional[AnomalyConfig] = None):
        self.config = config or AnomalyConfig()
        
        # Detection methods
        self.methods: Dict[str, Any] = {}
        self._initialize_methods()
        
        # History
        self.detection_history: List[AnomalyResult] = []
        self.anomaly_count = 0
        
        # Training data for reference
        self.training_data: List[List[float]] = []
    
    def _initialize_methods(self) -> None:
        """Initialize detection methods."""
        for method in self.config.methods:
            if method == "isolation_forest":
                self.methods[method] = SimplifiedIsolationForest()
            elif method == "autoencoder":
                self.methods[method] = SimplifiedAutoencoder()
            elif method == "lof":
                self.methods[method] = SimplifiedLOF()
    
    def fit(self, data: List[List[float]]) -> None:
        """
        Fit detector on normal data.
        
        Args:
            data: Training data (assumed to be normal)
        """
        self.training_data = data.copy()
        
        for method in self.methods.values():
            method.fit(data)
    
    def detect(self, x: List[float]) -> AnomalyResult:
        """
        Detect if input is anomalous.
        
        Args:
            x: Input feature vector
            
        Returns:
            AnomalyResult with scores and classification
        """
        method_scores = {}
        
        for name, method in self.methods.items():
            score = method.score(x)
            method_scores[name] = score
        
        # Ensemble combination
        if self.config.ensemble_strategy == "voting":
            votes = sum(1 for s in method_scores.values() if s > self.config.threshold)
            final_score = votes / len(method_scores) if method_scores else 0
        elif self.config.ensemble_strategy == "average":
            final_score = sum(method_scores.values()) / len(method_scores) if method_scores else 0
        elif self.config.ensemble_strategy == "max":
            final_score = max(method_scores.values()) if method_scores else 0
        else:
            final_score = sum(method_scores.values()) / len(method_scores) if method_scores else 0
        
        # Determine if anomaly
        is_anomaly = final_score > self.config.threshold
        
        # Determine anomaly type
        if is_anomaly:
            if final_score > 0.9:
                anomaly_type = AnomalyType.ATTACK
            elif final_score > 0.8:
                anomaly_type = AnomalyType.OUTLIER
            else:
                anomaly_type = AnomalyType.NOVELTY
        else:
            anomaly_type = AnomalyType.NORMAL
        
        # Calculate confidence
        if method_scores:
            scores_list = list(method_scores.values())
            mean = sum(scores_list) / len(scores_list)
            variance = sum((s - mean) ** 2 for s in scores_list) / len(scores_list)
            std = math.sqrt(variance)
            confidence = 1.0 - min(std, 0.5) / 0.5  # Higher agreement = higher confidence
        else:
            confidence = 0.5
        
        result = AnomalyResult(
            input_data=x[:50] if len(x) > 50 else x,  # Truncate for storage
            is_anomaly=is_anomaly,
            anomaly_score=final_score,
            anomaly_type=anomaly_type,
            method_scores=method_scores,
            confidence=confidence
        )
        
        self.detection_history.append(result)
        if is_anomaly:
            self.anomaly_count += 1
        
        return result
    
    def detect_batch(self, data: List[List[float]]) -> List[AnomalyResult]:
        """Detect anomalies in batch."""
        return [self.detect(x) for x in data]
    
    def get_stats(self) -> Dict[str, Any]:
        """Get detection statistics."""
        total = len(self.detection_history)
        return {
            "total_detections": total,
            "anomaly_count": self.anomaly_count,
            "anomaly_rate": self.anomaly_count / total if total > 0 else 0,
            "methods": list(self.methods.keys()),
            "threshold": self.config.threshold,
            "type_distribution": self._get_type_distribution()
        }
    
    def _get_type_distribution(self) -> Dict[str, int]:
        """Get distribution of anomaly types."""
        dist: Dict[str, int] = {}
        for result in self.detection_history:
            key = result.anomaly_type.value
            dist[key] = dist.get(key, 0) + 1
        return dist


class SimplifiedIsolationForest:
    """Simplified Isolation Forest implementation."""
    
    def __init__(self, n_estimators: int = 100, contamination: float = 0.1):
        self.n_estimators = n_estimators
        self.contamination = contamination
        self.trees: List[Dict] = []
        self.threshold = 0.5
    
    def fit(self, data: List[List[float]]) -> None:
        """Fit isolation forest."""
        if not data:
            return
        
        n_features = len(data[0])
        
        for _ in range(self.n_estimators):
            tree = self._build_tree(data, n_features, max_depth=10)
            self.trees.append(tree)
        
        # Set threshold based on training scores
        scores = [self._path_length(x) for x in data]
        scores.sort(reverse=True)
        idx = int(len(scores) * self.contamination)
        self.threshold = scores[idx] if idx < len(scores) else 0.5
    
    def _build_tree(self, data: List[List[float]], n_features: int, max_depth: int) -> Dict:
        """Build single isolation tree."""
        if len(data) <= 1 or max_depth == 0:
            return {"size": len(data)}
        
        # Random feature and split
        feature = random.randint(0, n_features - 1)
        values = [x[feature] for x in data if feature < len(x)]
        if not values:
            return {"size": len(data)}
        
        split = random.uniform(min(values), max(values))
        
        left = [x for x in data if x[feature] < split]
        right = [x for x in data if x[feature] >= split]
        
        return {
            "feature": feature,
            "split": split,
            "left": self._build_tree(left, n_features, max_depth - 1),
            "right": self._build_tree(right, n_features, max_depth - 1)
        }
    
    def _path_length(self, x: List[float]) -> float:
        """Calculate average path length across trees."""
        if not self.trees:
            return 0.5
        
        lengths = []
        for tree in self.trees:
            length = self._traverse(tree, x, 0)
            lengths.append(length)
        
        avg_length = sum(lengths) / len(lengths)
        # Normalize by expected path length
        n = max(len(self.trees), 1)
        c = 2 * (math.log(n - 1) + 0.5772156649) - 2 * (n - 1) / n if n > 1 else 1
        
        return 2 ** (-avg_length / max(c, 1))
    
    def _traverse(self, node: Dict, x: List[float], depth: int) -> int:
        """Traverse tree to find path length."""
        if "size" in node:
            return depth
        
        feature = node["feature"]
        if feature >= len(x):
            return depth
        
        if x[feature] < node["split"]:
            return self._traverse(node["left"], x, depth + 1)
        else:
            return self._traverse(node["right"], x, depth + 1)
    
    def score(self, x: List[float]) -> float:
        """Get anomaly score (higher = more anomalous)."""
        return self._path_length(x)


class SimplifiedAutoencoder:
    """Simplified Autoencoder for anomaly detection."""
    
    def __init__(self, latent_dim: int = 32, threshold: float = 0.5):
        self.latent_dim = latent_dim
        self.threshold = threshold
        self.input_dim = 0
        self.encoder_w: List[List[float]] = []
        self.decoder_w: List[List[float]] = []
        self.mean_error = 0.0
        self.std_error = 1.0
    
    def fit(self, data: List[List[float]]) -> None:
        """Fit autoencoder."""
        if not data:
            return
        
        self.input_dim = len(data[0])
        
        # Initialize weights
        scale = math.sqrt(2.0 / (self.input_dim + self.latent_dim))
        self.encoder_w = [
            [(random.random() * 2 - 1) * scale for _ in range(self.latent_dim)]
            for _ in range(self.input_dim)
        ]
        self.decoder_w = [
            [(random.random() * 2 - 1) * scale for _ in range(self.input_dim)]
            for _ in range(self.latent_dim)
        ]
        
        # Simple training
        for _ in range(100):
            for x in data:
                self._train_step(x, 0.01)
        
        # Calculate error statistics
        errors = [self._reconstruction_error(x) for x in data]
        self.mean_error = sum(errors) / len(errors)
        variance = sum((e - self.mean_error) ** 2 for e in errors) / len(errors)
        self.std_error = math.sqrt(variance) + 1e-6
    
    def _train_step(self, x: List[float], lr: float) -> None:
        """Single training step."""
        # Encode
        latent = self._encode(x)
        # Decode
        recon = self._decode(latent)
        
        # Update weights (simplified)
        for i in range(min(len(x), len(self.encoder_w))):
            for j in range(len(self.encoder_w[i])):
                error = (recon[i] - x[i]) if i < len(recon) else 0
                self.encoder_w[i][j] -= lr * error * 0.01
    
    def _encode(self, x: List[float]) -> List[float]:
        """Encode input to latent space."""
        latent = []
        for j in range(self.latent_dim):
            val = 0
            for i in range(min(len(x), len(self.encoder_w))):
                val += x[i] * self.encoder_w[i][j]
            latent.append(math.tanh(val))
        return latent
    
    def _decode(self, latent: List[float]) -> List[float]:
        """Decode from latent space."""
        output = []
        for j in range(self.input_dim):
            val = 0
            for i in range(min(len(latent), len(self.decoder_w))):
                val += latent[i] * self.decoder_w[i][j]
            output.append(val)
        return output
    
    def _reconstruction_error(self, x: List[float]) -> float:
        """Calculate reconstruction error."""
        latent = self._encode(x)
        recon = self._decode(latent)
        
        error = 0
        for i in range(min(len(x), len(recon))):
            error += (x[i] - recon[i]) ** 2
        
        return math.sqrt(error / max(len(x), 1))
    
    def score(self, x: List[float]) -> float:
        """Get anomaly score."""
        error = self._reconstruction_error(x)
        # Normalize to 0-1 range
        z_score = (error - self.mean_error) / self.std_error
        return 1 / (1 + math.exp(-z_score))  # Sigmoid


class SimplifiedLOF:
    """Simplified Local Outlier Factor."""
    
    def __init__(self, n_neighbors: int = 20):
        self.n_neighbors = n_neighbors
        self.training_data: List[List[float]] = []
    
    def fit(self, data: List[List[float]]) -> None:
        """Fit LOF."""
        self.training_data = data.copy()
    
    def _distance(self, a: List[float], b: List[float]) -> float:
        """Euclidean distance."""
        return math.sqrt(sum((ai - bi) ** 2 for ai, bi in zip(a, b)))
    
    def score(self, x: List[float]) -> float:
        """Get anomaly score."""
        if not self.training_data:
            return 0.5
        
        # Find k nearest neighbors
        distances = [
            (i, self._distance(x, point))
            for i, point in enumerate(self.training_data)
        ]
        distances.sort(key=lambda p: p[1])
        
        k = min(self.n_neighbors, len(distances))
        neighbors = distances[:k]
        
        if not neighbors:
            return 0.5
        
        # Calculate reachability distance
        k_dist = neighbors[-1][1] if neighbors else 0
        
        # Calculate local reachability density
        lrd = 0
        for _, dist in neighbors:
            lrd += max(k_dist, dist)
        
        if lrd == 0:
            return 0.5
        
        lrd = k / lrd
        
        # Simplified LOF (compare to average)
        avg_distance = sum(d for _, d in neighbors) / len(neighbors)
        
        # Normalize to 0-1
        if avg_distance > 0:
            score = 1 - math.exp(-lrd / avg_distance)
        else:
            score = 0.5
        
        return score
