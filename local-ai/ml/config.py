"""
DevTeam6 ML Configuration

Comprehensive configuration for all ML components including
supervised learning, reinforcement learning, contrastive learning,
online learning, anomaly detection, fine-tuning, and meta-learning.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any
from enum import Enum
import os


class ModelType(Enum):
    """Supported model types."""
    TRANSFORMER = "transformer"
    CNN = "cnn"
    RNN = "rnn"
    ENSEMBLE = "ensemble"
    NEURAL = "neural"


class LearningMode(Enum):
    """Learning paradigm modes."""
    SUPERVISED = "supervised"
    REINFORCEMENT = "reinforcement"
    CONTRASTIVE = "contrastive"
    ONLINE = "online"
    META = "meta"


@dataclass
class SupervisedConfig:
    """Configuration for supervised learning components."""
    # Token Classifier
    token_embedding_dim: int = 256
    token_hidden_dim: int = 512
    token_num_classes: int = 8  # API, SECURITY, DATA, LOCAL, HYBRID, CONTROL, IO, GENERAL
    token_dropout: float = 0.3
    
    # Threat Detector
    threat_sequence_length: int = 128
    threat_num_features: int = 64
    threat_threshold: float = 0.7
    
    # Intent Recognizer
    intent_embedding_dim: int = 128
    intent_num_intents: int = 32
    intent_confidence_threshold: float = 0.6
    
    # Training
    batch_size: int = 32
    learning_rate: float = 0.001
    epochs: int = 100
    early_stopping_patience: int = 10


@dataclass
class ReinforcementConfig:
    """Configuration for reinforcement learning (PPO)."""
    # PPO Hyperparameters
    gamma: float = 0.99  # Discount factor
    lambda_gae: float = 0.95  # GAE lambda
    clip_epsilon: float = 0.2  # PPO clip range
    entropy_coef: float = 0.01  # Entropy bonus coefficient
    value_coef: float = 0.5  # Value loss coefficient
    max_grad_norm: float = 0.5  # Gradient clipping
    
    # Network Architecture
    state_dim: int = 256
    action_dim: int = 5  # LOCAL, API, SECURITY, DATA, HYBRID
    hidden_dim: int = 256
    num_hidden_layers: int = 2
    
    # Training
    num_steps: int = 2048  # Steps per update
    num_epochs: int = 10  # PPO epochs per update
    mini_batch_size: int = 64
    learning_rate: float = 3e-4
    
    # Experience Buffer
    buffer_size: int = 10000
    prioritized_replay: bool = True
    priority_alpha: float = 0.6
    priority_beta: float = 0.4


@dataclass
class ContrastiveConfig:
    """Configuration for contrastive learning."""
    # Embedding Dimensions
    embedding_dim: int = 256
    projection_dim: int = 128
    
    # Triplet Loss
    margin: float = 0.5
    mining_strategy: str = "hard"  # hard, semi-hard, random
    
    # Negative Sampling
    num_negatives: int = 5
    temperature: float = 0.07
    
    # Training
    batch_size: int = 128
    learning_rate: float = 1e-4
    warmup_steps: int = 1000


@dataclass
class OnlineConfig:
    """Configuration for online learning."""
    # Drift Detection
    drift_window_size: int = 100
    drift_threshold: float = 0.1
    drift_method: str = "adwin"  # adwin, ddm, eddm, page_hinkley
    
    # Incremental Learning
    update_frequency: int = 10  # Update after N samples
    forgetting_factor: float = 0.99
    memory_size: int = 1000  # Maximum samples to retain
    
    # Adaptation
    adaptation_rate: float = 0.1
    min_samples_before_update: int = 50


@dataclass
class AnomalyConfig:
    """Configuration for anomaly detection."""
    # Detection Methods
    methods: List[str] = field(default_factory=lambda: ["isolation_forest", "autoencoder", "lof"])
    ensemble_strategy: str = "voting"  # voting, average, max
    
    # Isolation Forest
    if_n_estimators: int = 100
    if_contamination: float = 0.1
    
    # Autoencoder
    ae_latent_dim: int = 32
    ae_reconstruction_threshold: float = 0.5
    
    # LOF (Local Outlier Factor)
    lof_n_neighbors: int = 20
    
    # General
    anomaly_threshold: float = 0.7
    alert_on_detection: bool = True


@dataclass
class FinetuningConfig:
    """Configuration for model fine-tuning (LoRA/QLoRA)."""
    # LoRA Parameters
    lora_r: int = 16  # Rank
    lora_alpha: int = 32  # Alpha scaling
    lora_dropout: float = 0.05
    target_modules: List[str] = field(default_factory=lambda: ["q_proj", "v_proj", "k_proj", "o_proj"])
    
    # QLoRA (Quantization)
    use_quantization: bool = True
    bits: int = 4  # 4-bit or 8-bit quantization
    double_quant: bool = True
    quant_type: str = "nf4"  # nf4 or fp4
    
    # Training
    batch_size: int = 4
    gradient_accumulation_steps: int = 4
    learning_rate: float = 2e-4
    max_steps: int = 1000
    warmup_ratio: float = 0.03


@dataclass
class MetaConfig:
    """Configuration for meta-learning and hyperparameter optimization."""
    # Hyperparameter Search
    search_method: str = "bayesian"  # bayesian, random, grid, evolutionary
    n_trials: int = 50
    timeout_hours: float = 24.0
    
    # Bayesian Optimization
    acquisition_function: str = "ei"  # ei, ucb, pi
    n_initial_points: int = 10
    
    # Self-Optimization
    auto_tune_frequency: int = 1000  # Re-tune every N iterations
    performance_window: int = 100
    improvement_threshold: float = 0.01


@dataclass
class AdvancedConfig:
    """Configuration for advanced ML features."""
    # Swarm Intelligence
    swarm_enabled: bool = True
    num_particles: int = 30
    inertia_weight: float = 0.7
    cognitive_coef: float = 1.5
    social_coef: float = 1.5
    
    # Neuromorphic Concepts
    spike_threshold: float = 0.5
    refractory_period: int = 5
    leak_rate: float = 0.1
    
    # Ensemble
    ensemble_size: int = 5
    ensemble_strategy: str = "weighted_vote"  # vote, average, stacking, weighted_vote


@dataclass
class MLConfig:
    """Master ML configuration containing all component configs."""
    # Component Configurations
    supervised: SupervisedConfig = field(default_factory=SupervisedConfig)
    reinforcement: ReinforcementConfig = field(default_factory=ReinforcementConfig)
    contrastive: ContrastiveConfig = field(default_factory=ContrastiveConfig)
    online: OnlineConfig = field(default_factory=OnlineConfig)
    anomaly: AnomalyConfig = field(default_factory=AnomalyConfig)
    finetuning: FinetuningConfig = field(default_factory=FinetuningConfig)
    meta: MetaConfig = field(default_factory=MetaConfig)
    advanced: AdvancedConfig = field(default_factory=AdvancedConfig)
    
    # Global Settings
    device: str = "cuda"  # cuda, cpu, mps
    seed: int = 42
    log_level: str = "INFO"
    checkpoint_dir: str = "./checkpoints"
    metrics_dir: str = "./metrics"
    
    # Cyberpunk Theme Colors (for visualization)
    theme_colors: Dict[str, str] = field(default_factory=lambda: {
        "primary": "#00f0ff",   # Cyan
        "secondary": "#ff00ff", # Magenta
        "accent": "#00ff88",    # Green
        "background": "#0a0a0f",
        "surface": "#1a1a2e"
    })
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert config to dictionary."""
        import dataclasses
        return dataclasses.asdict(self)
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "MLConfig":
        """Create config from dictionary."""
        return cls(
            supervised=SupervisedConfig(**data.get("supervised", {})),
            reinforcement=ReinforcementConfig(**data.get("reinforcement", {})),
            contrastive=ContrastiveConfig(**data.get("contrastive", {})),
            online=OnlineConfig(**data.get("online", {})),
            anomaly=AnomalyConfig(**data.get("anomaly", {})),
            finetuning=FinetuningConfig(**data.get("finetuning", {})),
            meta=MetaConfig(**data.get("meta", {})),
            advanced=AdvancedConfig(**data.get("advanced", {})),
            device=data.get("device", "cuda"),
            seed=data.get("seed", 42),
            log_level=data.get("log_level", "INFO"),
            checkpoint_dir=data.get("checkpoint_dir", "./checkpoints"),
            metrics_dir=data.get("metrics_dir", "./metrics"),
        )


# Global config instance
_ml_config: Optional[MLConfig] = None


def get_ml_config() -> MLConfig:
    """Get or create the global ML configuration."""
    global _ml_config
    if _ml_config is None:
        _ml_config = MLConfig()
    return _ml_config


def set_ml_config(config: MLConfig) -> None:
    """Set the global ML configuration."""
    global _ml_config
    _ml_config = config
