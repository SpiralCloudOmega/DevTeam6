"""
DevTeam6 Intent Recognizer

Neural network-based intent recognition for understanding
user/system intent from token sequences.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
from enum import Enum
import math
import hashlib
import json
from datetime import datetime


class Intent(Enum):
    """Recognized intent categories."""
    QUERY = "query"
    COMMAND = "command"
    QUESTION = "question"
    STATEMENT = "statement"
    REQUEST = "request"
    NAVIGATION = "navigation"
    CREATION = "creation"
    MODIFICATION = "modification"
    DELETION = "deletion"
    AUTHENTICATION = "authentication"
    CONFIGURATION = "configuration"
    ANALYSIS = "analysis"
    EXPORT = "export"
    IMPORT = "import"
    SEARCH = "search"
    HELP = "help"
    UNKNOWN = "unknown"


class IntentContext(Enum):
    """Context in which intent occurs."""
    USER_INPUT = "user_input"
    API_CALL = "api_call"
    SYSTEM_EVENT = "system_event"
    SCHEDULED_TASK = "scheduled_task"
    WEBHOOK = "webhook"
    INTERNAL = "internal"


@dataclass
class IntentSlot:
    """Extracted slot/entity from intent."""
    name: str
    value: str
    type: str
    confidence: float
    position: Tuple[int, int]


@dataclass
class IntentResult:
    """Result of intent recognition."""
    text: str
    primary_intent: Intent
    confidence: float
    secondary_intents: List[Tuple[Intent, float]]
    slots: List[IntentSlot]
    context: IntentContext
    sentiment: float  # -1.0 to 1.0
    urgency: float  # 0.0 to 1.0
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())


class IntentRecognizer:
    """
    Neural network-based intent recognizer for the DevTeam6 Dual Transformer.
    
    Recognizes user/system intent from natural language and code patterns,
    extracting slots and determining context, sentiment, and urgency.
    """
    
    # Intent keywords and patterns
    INTENT_PATTERNS = {
        Intent.QUERY: {
            "keywords": ["get", "fetch", "retrieve", "read", "show", "display", "find", "list"],
            "patterns": ["get *", "show me *", "what is *", "find *"]
        },
        Intent.COMMAND: {
            "keywords": ["run", "execute", "start", "stop", "restart", "deploy", "build"],
            "patterns": ["run *", "execute *", "start *", "stop *"]
        },
        Intent.QUESTION: {
            "keywords": ["what", "why", "how", "when", "where", "who", "which", "can", "could", "would"],
            "patterns": ["what *?", "how do *?", "why *?", "can you *?"]
        },
        Intent.REQUEST: {
            "keywords": ["please", "could you", "would you", "can you", "help me"],
            "patterns": ["please *", "could you *", "help me *"]
        },
        Intent.CREATION: {
            "keywords": ["create", "make", "new", "add", "generate", "build", "compose"],
            "patterns": ["create *", "make a *", "add *", "generate *"]
        },
        Intent.MODIFICATION: {
            "keywords": ["update", "modify", "change", "edit", "alter", "set", "configure"],
            "patterns": ["update *", "change *", "edit *", "modify *"]
        },
        Intent.DELETION: {
            "keywords": ["delete", "remove", "drop", "clear", "reset", "destroy"],
            "patterns": ["delete *", "remove *", "clear *"]
        },
        Intent.AUTHENTICATION: {
            "keywords": ["login", "logout", "signin", "signout", "authenticate", "authorize"],
            "patterns": ["login *", "sign in *", "authenticate *"]
        },
        Intent.SEARCH: {
            "keywords": ["search", "find", "lookup", "query", "filter", "browse"],
            "patterns": ["search for *", "find *", "look up *"]
        },
        Intent.HELP: {
            "keywords": ["help", "assist", "support", "guide", "explain", "docs", "documentation"],
            "patterns": ["help *", "how to *", "explain *"]
        },
        Intent.NAVIGATION: {
            "keywords": ["go", "navigate", "open", "view", "visit", "redirect"],
            "patterns": ["go to *", "navigate to *", "open *"]
        },
        Intent.ANALYSIS: {
            "keywords": ["analyze", "examine", "inspect", "review", "evaluate", "assess"],
            "patterns": ["analyze *", "review *", "evaluate *"]
        },
        Intent.EXPORT: {
            "keywords": ["export", "download", "save", "backup", "dump"],
            "patterns": ["export *", "download *", "save *"]
        },
        Intent.IMPORT: {
            "keywords": ["import", "upload", "load", "restore", "ingest"],
            "patterns": ["import *", "upload *", "load *"]
        },
        Intent.CONFIGURATION: {
            "keywords": ["config", "configure", "setup", "settings", "preferences", "options"],
            "patterns": ["configure *", "set up *", "settings for *"]
        }
    }
    
    # Sentiment words
    POSITIVE_WORDS = {"good", "great", "excellent", "awesome", "nice", "thanks", "thank", "please", "love", "best"}
    NEGATIVE_WORDS = {"bad", "wrong", "error", "fail", "issue", "problem", "bug", "broken", "hate", "worst"}
    
    # Urgency indicators
    URGENCY_WORDS = {"urgent", "asap", "immediately", "now", "critical", "emergency", "quick", "fast", "hurry"}
    
    def __init__(
        self,
        embedding_dim: int = 128,
        num_intents: int = 17,
        confidence_threshold: float = 0.6
    ):
        self.embedding_dim = embedding_dim
        self.num_intents = num_intents
        self.confidence_threshold = confidence_threshold
        
        # Intent list for indexing
        self.intents = list(Intent)
        self.intent_to_idx = {intent: idx for idx, intent in enumerate(self.intents)}
        
        # Initialize neural weights
        self._initialize_weights()
        
        # Recognition history
        self.history: List[IntentResult] = []
    
    def _initialize_weights(self) -> None:
        """Initialize neural network weights."""
        # Embedding weights
        self.embedding_weights = [
            [self._pseudo_random(i, j) for j in range(self.embedding_dim)]
            for i in range(256)
        ]
        
        # Classification weights
        self.class_weights = [
            [self._pseudo_random(i + 500, j) for j in range(len(self.intents))]
            for i in range(self.embedding_dim)
        ]
    
    def _pseudo_random(self, i: int, j: int) -> float:
        """Generate pseudo-random weight."""
        seed = hash(f"intent_{i}_{j}") % 10000
        return (seed / 10000.0) * 2 - 1
    
    def _text_to_embedding(self, text: str) -> List[float]:
        """Convert text to embedding vector."""
        embedding = [0.0] * self.embedding_dim
        
        for char in text.lower():
            char_idx = ord(char) % 256
            for j in range(self.embedding_dim):
                embedding[j] += self.embedding_weights[char_idx][j]
        
        # Normalize
        magnitude = math.sqrt(sum(e * e for e in embedding))
        if magnitude > 0:
            embedding = [e / magnitude for e in embedding]
        
        return embedding
    
    def _keyword_matching(self, text: str) -> Dict[Intent, float]:
        """Match keywords to calculate intent scores."""
        scores = {intent: 0.0 for intent in self.intents}
        text_lower = text.lower()
        words = set(text_lower.split())
        
        for intent, patterns in self.INTENT_PATTERNS.items():
            # Keyword matching
            keywords = set(patterns["keywords"])
            matches = words.intersection(keywords)
            if matches:
                scores[intent] += len(matches) / len(keywords) * 0.5
            
            # Pattern matching (simplified)
            for pattern in patterns["patterns"]:
                pattern_start = pattern.split(" ")[0].lower()
                if text_lower.startswith(pattern_start):
                    scores[intent] += 0.3
        
        return scores
    
    def _neural_classification(self, embedding: List[float]) -> Dict[Intent, float]:
        """Neural network classification."""
        # Forward pass
        logits = [0.0] * len(self.intents)
        for i, emb in enumerate(embedding):
            if i < len(self.class_weights):
                for j in range(len(self.intents)):
                    if j < len(self.class_weights[i]):
                        logits[j] += emb * self.class_weights[i][j]
        
        # Softmax
        max_logit = max(logits) if logits else 0
        exp_logits = [math.exp(l - max_logit) for l in logits]
        sum_exp = sum(exp_logits)
        
        return {
            intent: exp_logits[idx] / sum_exp if sum_exp > 0 else 0
            for intent, idx in self.intent_to_idx.items()
        }
    
    def _calculate_sentiment(self, text: str) -> float:
        """Calculate sentiment score from -1.0 (negative) to 1.0 (positive)."""
        words = set(text.lower().split())
        
        positive_count = len(words.intersection(self.POSITIVE_WORDS))
        negative_count = len(words.intersection(self.NEGATIVE_WORDS))
        
        total = positive_count + negative_count
        if total == 0:
            return 0.0
        
        return (positive_count - negative_count) / total
    
    def _calculate_urgency(self, text: str) -> float:
        """Calculate urgency score from 0.0 to 1.0."""
        words = set(text.lower().split())
        urgent_matches = len(words.intersection(self.URGENCY_WORDS))
        
        # Check for urgency patterns
        text_lower = text.lower()
        if "!" in text:
            urgent_matches += 1
        if "asap" in text_lower or "urgent" in text_lower:
            urgent_matches += 2
        
        return min(urgent_matches / 3.0, 1.0)
    
    def _extract_slots(self, text: str, intent: Intent) -> List[IntentSlot]:
        """Extract slots/entities from text based on intent."""
        slots = []
        words = text.split()
        
        # Simple slot extraction based on patterns
        for i, word in enumerate(words):
            # Numeric slots
            if word.isdigit():
                slots.append(IntentSlot(
                    name="number",
                    value=word,
                    type="numeric",
                    confidence=0.9,
                    position=(sum(len(w) + 1 for w in words[:i]), sum(len(w) + 1 for w in words[:i]) + len(word))
                ))
            
            # Email-like patterns
            if "@" in word and "." in word:
                slots.append(IntentSlot(
                    name="email",
                    value=word,
                    type="email",
                    confidence=0.85,
                    position=(sum(len(w) + 1 for w in words[:i]), sum(len(w) + 1 for w in words[:i]) + len(word))
                ))
            
            # URL-like patterns
            if word.startswith("http://") or word.startswith("https://"):
                slots.append(IntentSlot(
                    name="url",
                    value=word,
                    type="url",
                    confidence=0.95,
                    position=(sum(len(w) + 1 for w in words[:i]), sum(len(w) + 1 for w in words[:i]) + len(word))
                ))
            
            # File paths
            if "/" in word or "\\" in word:
                if any(ext in word.lower() for ext in [".py", ".js", ".ts", ".json", ".md", ".txt"]):
                    slots.append(IntentSlot(
                        name="file_path",
                        value=word,
                        type="path",
                        confidence=0.8,
                        position=(sum(len(w) + 1 for w in words[:i]), sum(len(w) + 1 for w in words[:i]) + len(word))
                    ))
        
        return slots
    
    def _determine_context(self, text: str) -> IntentContext:
        """Determine the context of the intent."""
        text_lower = text.lower()
        
        if any(w in text_lower for w in ["api", "endpoint", "request", "response"]):
            return IntentContext.API_CALL
        elif any(w in text_lower for w in ["webhook", "callback", "hook"]):
            return IntentContext.WEBHOOK
        elif any(w in text_lower for w in ["schedule", "cron", "timer", "interval"]):
            return IntentContext.SCHEDULED_TASK
        elif any(w in text_lower for w in ["system", "service", "daemon"]):
            return IntentContext.SYSTEM_EVENT
        elif any(w in text_lower for w in ["internal", "private", "self"]):
            return IntentContext.INTERNAL
        else:
            return IntentContext.USER_INPUT
    
    def recognize(self, text: str) -> IntentResult:
        """
        Recognize intent from text.
        
        Args:
            text: Input text to analyze
            
        Returns:
            IntentResult with primary intent, confidence, slots, and metadata
        """
        # Get embedding
        embedding = self._text_to_embedding(text)
        
        # Neural classification
        neural_scores = self._neural_classification(embedding)
        
        # Keyword matching
        keyword_scores = self._keyword_matching(text)
        
        # Combine scores (weighted average)
        combined_scores = {}
        for intent in self.intents:
            combined_scores[intent] = (
                neural_scores.get(intent, 0) * 0.4 +
                keyword_scores.get(intent, 0) * 0.6
            )
        
        # Sort by score
        sorted_intents = sorted(
            combined_scores.items(),
            key=lambda x: x[1],
            reverse=True
        )
        
        primary_intent = sorted_intents[0][0]
        confidence = sorted_intents[0][1]
        
        # If confidence is too low, mark as unknown
        if confidence < self.confidence_threshold:
            primary_intent = Intent.UNKNOWN
        
        # Get secondary intents
        secondary = [(intent, score) for intent, score in sorted_intents[1:4] if score > 0.1]
        
        # Extract additional information
        slots = self._extract_slots(text, primary_intent)
        context = self._determine_context(text)
        sentiment = self._calculate_sentiment(text)
        urgency = self._calculate_urgency(text)
        
        result = IntentResult(
            text=text,
            primary_intent=primary_intent,
            confidence=confidence,
            secondary_intents=secondary,
            slots=slots,
            context=context,
            sentiment=sentiment,
            urgency=urgency
        )
        
        self.history.append(result)
        return result
    
    def recognize_batch(self, texts: List[str]) -> List[IntentResult]:
        """Recognize intents for multiple texts."""
        return [self.recognize(text) for text in texts]
    
    def get_stats(self) -> Dict[str, Any]:
        """Get recognition statistics."""
        if not self.history:
            return {
                "total_recognitions": 0,
                "intent_distribution": {},
                "average_confidence": 0.0
            }
        
        intent_counts: Dict[str, int] = {}
        total_confidence = 0.0
        
        for result in self.history:
            intent_key = result.primary_intent.value
            intent_counts[intent_key] = intent_counts.get(intent_key, 0) + 1
            total_confidence += result.confidence
        
        return {
            "total_recognitions": len(self.history),
            "intent_distribution": intent_counts,
            "average_confidence": total_confidence / len(self.history),
            "context_distribution": self._get_context_distribution(),
            "average_sentiment": sum(r.sentiment for r in self.history) / len(self.history),
            "average_urgency": sum(r.urgency for r in self.history) / len(self.history)
        }
    
    def _get_context_distribution(self) -> Dict[str, int]:
        """Get distribution of contexts."""
        distribution: Dict[str, int] = {}
        for result in self.history:
            context = result.context.value
            distribution[context] = distribution.get(context, 0) + 1
        return distribution
    
    def clear_history(self) -> None:
        """Clear recognition history."""
        self.history.clear()
