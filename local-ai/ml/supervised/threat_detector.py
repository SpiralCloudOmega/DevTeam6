"""
DevTeam6 Threat Detector

Neural network-based security threat detection for the Dual Transformer.
Identifies potentially malicious patterns in token sequences.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
from enum import Enum
import math
import hashlib
import json
from datetime import datetime


class ThreatLevel(Enum):
    """Security threat levels."""
    NONE = "none"
    LOW = "low"
    MEDIUM = "medium"
    HIGH = "high"
    CRITICAL = "critical"


class ThreatType(Enum):
    """Types of security threats."""
    INJECTION = "injection"
    XSS = "xss"
    COMMAND_INJECTION = "command_injection"
    PATH_TRAVERSAL = "path_traversal"
    AUTHENTICATION_BYPASS = "authentication_bypass"
    DATA_EXFILTRATION = "data_exfiltration"
    PRIVILEGE_ESCALATION = "privilege_escalation"
    RATE_ABUSE = "rate_abuse"
    UNKNOWN = "unknown"


@dataclass
class ThreatIndicator:
    """Individual threat indicator detected."""
    pattern: str
    threat_type: ThreatType
    confidence: float
    location: Tuple[int, int]  # (start, end) position
    context: str


@dataclass
class ThreatAnalysis:
    """Complete threat analysis result."""
    input_text: str
    threat_level: ThreatLevel
    overall_score: float  # 0.0 to 1.0
    threats_detected: List[ThreatIndicator]
    risk_factors: Dict[str, float]
    recommendations: List[str]
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class ThreatDetector:
    """
    Neural network-based threat detector for the DevTeam6 Dual Transformer.
    
    Uses pattern matching and neural classification to identify security threats
    in token sequences, including SQL injection, XSS, command injection, and more.
    """
    
    # Threat patterns by category
    THREAT_PATTERNS = {
        ThreatType.INJECTION: [
            "select", "insert", "update", "delete", "drop", "union",
            "or 1=1", "' or '", "'; --", "1; drop", "exec(", "execute(",
            "--", "/*", "*/", "@@", "char(", "nchar(", "varchar("
        ],
        ThreatType.XSS: [
            "<script", "</script>", "javascript:", "onerror=", "onload=",
            "onclick=", "onmouseover=", "onfocus=", "onblur=", "eval(",
            "document.cookie", "document.write", "innerHTML", "outerHTML",
            "alert(", "confirm(", "prompt(", "expression("
        ],
        ThreatType.COMMAND_INJECTION: [
            ";", "|", "&&", "||", "`", "$(", "cmd", "exec", "system(",
            "shell_exec", "passthru", "popen", "proc_open", "eval(",
            "os.system", "subprocess", "spawn", "/bin/sh", "/bin/bash"
        ],
        ThreatType.PATH_TRAVERSAL: [
            "../", "..\\", "%2e%2e", "%252e", "....//", "..;/",
            "/etc/passwd", "/etc/shadow", "c:\\windows", "\\\\",
            "file://", "php://", "data://", "zip://"
        ],
        ThreatType.AUTHENTICATION_BYPASS: [
            "admin'--", "' or 1=1--", "admin'/*", "' or ''='",
            "bypass", "' or true--", "1' or '1'='1", "' or 'x'='x"
        ],
        ThreatType.DATA_EXFILTRATION: [
            "base64_encode", "base64_decode", "serialize", "unserialize",
            "json_encode", "outfile", "into dumpfile", "load_file",
            "pg_sleep", "benchmark", "waitfor delay"
        ],
        ThreatType.PRIVILEGE_ESCALATION: [
            "sudo", "root", "administrator", "admin", "superuser",
            "grant", "revoke", "chmod", "chown", "setuid", "setgid"
        ]
    }
    
    # Risk weight by threat type
    THREAT_WEIGHTS = {
        ThreatType.INJECTION: 0.9,
        ThreatType.XSS: 0.8,
        ThreatType.COMMAND_INJECTION: 0.95,
        ThreatType.PATH_TRAVERSAL: 0.7,
        ThreatType.AUTHENTICATION_BYPASS: 0.85,
        ThreatType.DATA_EXFILTRATION: 0.75,
        ThreatType.PRIVILEGE_ESCALATION: 0.8,
        ThreatType.RATE_ABUSE: 0.5,
        ThreatType.UNKNOWN: 0.3
    }
    
    def __init__(
        self,
        sequence_length: int = 128,
        num_features: int = 64,
        threshold: float = 0.7
    ):
        self.sequence_length = sequence_length
        self.num_features = num_features
        self.threshold = threshold
        
        # Neural network weights (simplified)
        self._initialize_weights()
        
        # Detection history
        self.detection_history: List[ThreatAnalysis] = []
        self.alert_count = 0
    
    def _initialize_weights(self) -> None:
        """Initialize neural network weights."""
        # Feature extraction weights
        self.feature_weights = [
            [self._pseudo_random(i, j) for j in range(self.num_features)]
            for i in range(256)  # ASCII range
        ]
        
        # Classification weights
        self.class_weights = [
            [self._pseudo_random(i + 1000, j) for j in range(len(ThreatType))]
            for i in range(self.num_features)
        ]
    
    def _pseudo_random(self, i: int, j: int) -> float:
        """Generate pseudo-random weight."""
        seed = hash(f"threat_{i}_{j}") % 10000
        return (seed / 10000.0) * 2 - 1
    
    def _extract_features(self, text: str) -> List[float]:
        """Extract feature vector from text."""
        features = [0.0] * self.num_features
        
        # Character-level features
        for char in text[:self.sequence_length]:
            char_idx = ord(char) % 256
            for j in range(self.num_features):
                features[j] += self.feature_weights[char_idx][j]
        
        # Normalize
        max_val = max(abs(f) for f in features) if features else 1
        if max_val > 0:
            features = [f / max_val for f in features]
        
        return features
    
    def _detect_patterns(self, text: str) -> List[ThreatIndicator]:
        """Detect threat patterns in text."""
        indicators = []
        text_lower = text.lower()
        
        for threat_type, patterns in self.THREAT_PATTERNS.items():
            for pattern in patterns:
                pattern_lower = pattern.lower()
                start = 0
                while True:
                    pos = text_lower.find(pattern_lower, start)
                    if pos == -1:
                        break
                    
                    # Calculate confidence based on context
                    confidence = self._calculate_pattern_confidence(
                        text, pos, len(pattern), threat_type
                    )
                    
                    # Get surrounding context
                    context_start = max(0, pos - 20)
                    context_end = min(len(text), pos + len(pattern) + 20)
                    context = text[context_start:context_end]
                    
                    indicators.append(ThreatIndicator(
                        pattern=pattern,
                        threat_type=threat_type,
                        confidence=confidence,
                        location=(pos, pos + len(pattern)),
                        context=context
                    ))
                    
                    start = pos + 1
        
        return indicators
    
    def _calculate_pattern_confidence(
        self,
        text: str,
        position: int,
        length: int,
        threat_type: ThreatType
    ) -> float:
        """Calculate confidence score for a detected pattern."""
        base_confidence = self.THREAT_WEIGHTS.get(threat_type, 0.5)
        
        # Adjust based on context
        context_before = text[max(0, position - 10):position].lower()
        context_after = text[position + length:position + length + 10].lower()
        
        # Higher confidence if pattern appears in suspicious context
        suspicious_before = any(c in context_before for c in ["'", '"', ";", "("])
        suspicious_after = any(c in context_after for c in ["'", '"', ";", ")"])
        
        if suspicious_before:
            base_confidence *= 1.2
        if suspicious_after:
            base_confidence *= 1.2
        
        # Check for encoding (might be evasion attempt)
        if "%" in text[max(0, position - 5):position + length + 5]:
            base_confidence *= 1.3
        
        return min(base_confidence, 1.0)
    
    def _neural_classification(self, features: List[float]) -> Dict[ThreatType, float]:
        """Run neural classification on features."""
        scores = {}
        threat_types = list(ThreatType)
        
        # Simple feedforward
        class_scores = [0.0] * len(threat_types)
        for i, feat in enumerate(features):
            if i < len(self.class_weights):
                for j in range(len(threat_types)):
                    if j < len(self.class_weights[i]):
                        class_scores[j] += feat * self.class_weights[i][j]
        
        # Softmax normalization
        max_score = max(class_scores) if class_scores else 0
        exp_scores = [math.exp(s - max_score) for s in class_scores]
        sum_exp = sum(exp_scores)
        
        for i, threat_type in enumerate(threat_types):
            scores[threat_type] = exp_scores[i] / sum_exp if sum_exp > 0 else 0
        
        return scores
    
    def _determine_threat_level(self, score: float) -> ThreatLevel:
        """Determine threat level from overall score."""
        if score >= 0.9:
            return ThreatLevel.CRITICAL
        elif score >= 0.7:
            return ThreatLevel.HIGH
        elif score >= 0.5:
            return ThreatLevel.MEDIUM
        elif score >= 0.2:
            return ThreatLevel.LOW
        else:
            return ThreatLevel.NONE
    
    def _generate_recommendations(
        self,
        threats: List[ThreatIndicator],
        threat_level: ThreatLevel
    ) -> List[str]:
        """Generate security recommendations based on detected threats."""
        recommendations = []
        
        threat_types_found = set(t.threat_type for t in threats)
        
        if ThreatType.INJECTION in threat_types_found:
            recommendations.append("Use parameterized queries to prevent SQL injection")
            recommendations.append("Implement input validation and sanitization")
        
        if ThreatType.XSS in threat_types_found:
            recommendations.append("Encode output data before rendering")
            recommendations.append("Implement Content Security Policy (CSP)")
        
        if ThreatType.COMMAND_INJECTION in threat_types_found:
            recommendations.append("Avoid executing shell commands with user input")
            recommendations.append("Use allowlists for command arguments")
        
        if ThreatType.PATH_TRAVERSAL in threat_types_found:
            recommendations.append("Validate and normalize file paths")
            recommendations.append("Restrict file access to allowed directories")
        
        if ThreatType.AUTHENTICATION_BYPASS in threat_types_found:
            recommendations.append("Implement proper authentication mechanisms")
            recommendations.append("Use prepared statements for login queries")
        
        if threat_level in [ThreatLevel.HIGH, ThreatLevel.CRITICAL]:
            recommendations.append("Consider blocking this request immediately")
            recommendations.append("Log this incident for security review")
        
        return recommendations
    
    def analyze(self, text: str) -> ThreatAnalysis:
        """
        Analyze text for security threats.
        
        Args:
            text: Input text to analyze
            
        Returns:
            ThreatAnalysis with detected threats and recommendations
        """
        # Pattern-based detection
        threats = self._detect_patterns(text)
        
        # Neural classification
        features = self._extract_features(text)
        neural_scores = self._neural_classification(features)
        
        # Calculate overall risk score
        if threats:
            pattern_score = max(t.confidence for t in threats)
            # Combine pattern and neural scores
            neural_max = max(neural_scores.values())
            overall_score = (pattern_score + neural_max) / 2
        else:
            overall_score = max(neural_scores.values())
        
        # Determine threat level
        threat_level = self._determine_threat_level(overall_score)
        
        # Build risk factors
        risk_factors = {
            threat_type.value: score
            for threat_type, score in neural_scores.items()
        }
        risk_factors["pattern_matches"] = len(threats)
        risk_factors["text_length"] = len(text)
        risk_factors["special_chars"] = sum(1 for c in text if not c.isalnum()) / max(len(text), 1)
        
        # Generate recommendations
        recommendations = self._generate_recommendations(threats, threat_level)
        
        # Create analysis result
        analysis = ThreatAnalysis(
            input_text=text[:500] if len(text) > 500 else text,  # Truncate for storage
            threat_level=threat_level,
            overall_score=overall_score,
            threats_detected=threats,
            risk_factors=risk_factors,
            recommendations=recommendations
        )
        
        # Track history
        self.detection_history.append(analysis)
        if threat_level in [ThreatLevel.HIGH, ThreatLevel.CRITICAL]:
            self.alert_count += 1
        
        return analysis
    
    def analyze_sequence(self, tokens: List[str]) -> ThreatAnalysis:
        """Analyze a sequence of tokens for threats."""
        combined_text = " ".join(tokens)
        return self.analyze(combined_text)
    
    def get_stats(self) -> Dict[str, Any]:
        """Get detection statistics."""
        if not self.detection_history:
            return {
                "total_analyses": 0,
                "alert_count": 0,
                "threat_distribution": {},
                "average_score": 0.0
            }
        
        threat_counts: Dict[str, int] = {}
        total_score = 0.0
        
        for analysis in self.detection_history:
            total_score += analysis.overall_score
            for threat in analysis.threats_detected:
                key = threat.threat_type.value
                threat_counts[key] = threat_counts.get(key, 0) + 1
        
        return {
            "total_analyses": len(self.detection_history),
            "alert_count": self.alert_count,
            "threat_distribution": threat_counts,
            "average_score": total_score / len(self.detection_history),
            "threat_level_distribution": self._get_level_distribution()
        }
    
    def _get_level_distribution(self) -> Dict[str, int]:
        """Get distribution of threat levels."""
        distribution: Dict[str, int] = {}
        for analysis in self.detection_history:
            level = analysis.threat_level.value
            distribution[level] = distribution.get(level, 0) + 1
        return distribution
    
    def clear_history(self) -> None:
        """Clear detection history."""
        self.detection_history.clear()
        self.alert_count = 0
