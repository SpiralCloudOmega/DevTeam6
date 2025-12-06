"""
DevTeam6 Drift Detector

Concept drift detection for online learning.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any
from enum import Enum
import math
from datetime import datetime


class DriftType(Enum):
    """Types of concept drift."""
    SUDDEN = "sudden"       # Abrupt change
    GRADUAL = "gradual"     # Slow transition
    INCREMENTAL = "incremental"  # Continuous small changes
    RECURRING = "recurring"  # Periodic patterns


class DriftStatus(Enum):
    """Current drift status."""
    NONE = "none"
    WARNING = "warning"
    DETECTED = "detected"


@dataclass
class DriftConfig:
    """Configuration for drift detection."""
    window_size: int = 100
    warning_threshold: float = 2.0
    drift_threshold: float = 3.0
    method: str = "adwin"  # adwin, ddm, eddm, page_hinkley


@dataclass
class DriftEvent:
    """Drift detection event."""
    drift_type: DriftType
    status: DriftStatus
    confidence: float
    timestamp: str
    metrics: Dict[str, float]


class DriftDetector:
    """
    Concept drift detector for online learning.
    
    Monitors data distribution and model performance to detect
    when the underlying patterns change and retraining is needed.
    """
    
    def __init__(self, config: Optional[DriftConfig] = None):
        self.config = config or DriftConfig()
        
        # Statistics windows
        self.error_window: List[float] = []
        self.prediction_window: List[float] = []
        
        # Running statistics
        self.n_samples = 0
        self.running_mean = 0.0
        self.running_variance = 0.0
        self.running_std = 0.0
        
        # Drift detection state
        self.current_status = DriftStatus.NONE
        self.drift_events: List[DriftEvent] = []
        
        # ADWIN specific
        self.adwin_window: List[float] = []
        
        # DDM specific
        self.ddm_min_error = float('inf')
        self.ddm_min_std = float('inf')
        self.ddm_warning_count = 0
    
    def update(self, error: float) -> DriftStatus:
        """
        Update detector with new error observation.
        
        Args:
            error: Prediction error (0 for correct, 1 for incorrect)
            
        Returns:
            Current drift status
        """
        self.n_samples += 1
        
        # Update windows
        self.error_window.append(error)
        if len(self.error_window) > self.config.window_size:
            self.error_window.pop(0)
        
        # Update running statistics
        self._update_statistics(error)
        
        # Detect drift based on method
        if self.config.method == "adwin":
            status = self._adwin_detect(error)
        elif self.config.method == "ddm":
            status = self._ddm_detect(error)
        elif self.config.method == "eddm":
            status = self._eddm_detect(error)
        elif self.config.method == "page_hinkley":
            status = self._page_hinkley_detect(error)
        else:
            status = self._adwin_detect(error)
        
        # Record drift events
        if status == DriftStatus.DETECTED and self.current_status != DriftStatus.DETECTED:
            self._record_drift_event(DriftType.SUDDEN)
        
        self.current_status = status
        return status
    
    def _update_statistics(self, value: float) -> None:
        """Update running mean and variance."""
        if self.n_samples == 1:
            self.running_mean = value
            self.running_variance = 0.0
        else:
            old_mean = self.running_mean
            self.running_mean += (value - self.running_mean) / self.n_samples
            self.running_variance += (value - old_mean) * (value - self.running_mean)
            self.running_std = math.sqrt(self.running_variance / self.n_samples)
    
    def _adwin_detect(self, value: float) -> DriftStatus:
        """ADWIN-style drift detection."""
        self.adwin_window.append(value)
        
        if len(self.adwin_window) < self.config.window_size:
            return DriftStatus.NONE
        
        # Test for significant difference between sub-windows
        window_size = len(self.adwin_window)
        
        for cut in range(10, window_size - 10):
            left = self.adwin_window[:cut]
            right = self.adwin_window[cut:]
            
            left_mean = sum(left) / len(left)
            right_mean = sum(right) / len(right)
            
            # Calculate epsilon bound
            m = 1.0 / (1.0/len(left) + 1.0/len(right))
            delta = 0.1
            epsilon = math.sqrt((1.0/(2*m)) * math.log(4.0/delta))
            
            if abs(left_mean - right_mean) > epsilon * self.config.drift_threshold:
                # Drift detected - shrink window
                self.adwin_window = right
                return DriftStatus.DETECTED
            elif abs(left_mean - right_mean) > epsilon * self.config.warning_threshold:
                return DriftStatus.WARNING
        
        return DriftStatus.NONE
    
    def _ddm_detect(self, error: float) -> DriftStatus:
        """DDM (Drift Detection Method) detection."""
        if len(self.error_window) < 30:
            return DriftStatus.NONE
        
        # Calculate error rate and standard deviation
        p = sum(self.error_window) / len(self.error_window)
        s = math.sqrt(p * (1 - p) / len(self.error_window))
        
        # Track minimum
        if p + s < self.ddm_min_error + self.ddm_min_std:
            self.ddm_min_error = p
            self.ddm_min_std = s
            self.ddm_warning_count = 0
        
        # Check for drift
        if p + s > self.ddm_min_error + self.config.drift_threshold * self.ddm_min_std:
            self.ddm_min_error = float('inf')
            self.ddm_min_std = float('inf')
            return DriftStatus.DETECTED
        elif p + s > self.ddm_min_error + self.config.warning_threshold * self.ddm_min_std:
            self.ddm_warning_count += 1
            return DriftStatus.WARNING
        
        return DriftStatus.NONE
    
    def _eddm_detect(self, error: float) -> DriftStatus:
        """EDDM (Early Drift Detection Method) detection."""
        # Simplified EDDM based on error distances
        if len(self.error_window) < 30:
            return DriftStatus.NONE
        
        recent = self.error_window[-30:]
        older = self.error_window[-60:-30] if len(self.error_window) >= 60 else []
        
        if not older:
            return DriftStatus.NONE
        
        recent_rate = sum(recent) / len(recent)
        older_rate = sum(older) / len(older)
        
        ratio = recent_rate / (older_rate + 1e-10)
        
        if ratio > self.config.drift_threshold:
            return DriftStatus.DETECTED
        elif ratio > self.config.warning_threshold:
            return DriftStatus.WARNING
        
        return DriftStatus.NONE
    
    def _page_hinkley_detect(self, value: float) -> DriftStatus:
        """Page-Hinkley drift detection."""
        if len(self.error_window) < 10:
            return DriftStatus.NONE
        
        mean = sum(self.error_window) / len(self.error_window)
        
        # Calculate cumulative sum
        cumsum = 0
        min_cumsum = 0
        
        for err in self.error_window:
            cumsum += err - mean - 0.005  # Alpha = 0.005
            if cumsum < min_cumsum:
                min_cumsum = cumsum
        
        ph_value = cumsum - min_cumsum
        
        if ph_value > self.config.drift_threshold * 50:
            return DriftStatus.DETECTED
        elif ph_value > self.config.warning_threshold * 50:
            return DriftStatus.WARNING
        
        return DriftStatus.NONE
    
    def _record_drift_event(self, drift_type: DriftType) -> None:
        """Record a drift event."""
        event = DriftEvent(
            drift_type=drift_type,
            status=DriftStatus.DETECTED,
            confidence=0.95,
            timestamp=datetime.now().isoformat(),
            metrics={
                "window_mean": sum(self.error_window) / len(self.error_window) if self.error_window else 0,
                "samples_seen": self.n_samples,
                "running_mean": self.running_mean
            }
        )
        self.drift_events.append(event)
    
    def reset(self) -> None:
        """Reset detector state."""
        self.error_window.clear()
        self.prediction_window.clear()
        self.adwin_window.clear()
        self.n_samples = 0
        self.running_mean = 0.0
        self.running_variance = 0.0
        self.running_std = 0.0
        self.current_status = DriftStatus.NONE
        self.ddm_min_error = float('inf')
        self.ddm_min_std = float('inf')
    
    def get_stats(self) -> Dict[str, Any]:
        """Get detector statistics."""
        return {
            "samples_seen": self.n_samples,
            "current_status": self.current_status.value,
            "drift_count": len(self.drift_events),
            "window_size": len(self.error_window),
            "running_mean": self.running_mean,
            "running_std": self.running_std,
            "method": self.config.method,
            "recent_events": [
                {"type": e.drift_type.value, "timestamp": e.timestamp}
                for e in self.drift_events[-5:]
            ]
        }
