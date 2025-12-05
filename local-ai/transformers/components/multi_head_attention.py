"""
DevTeam6 Local AI - Multi-Head Attention

Simple attention mechanism for context-aware token processing.
"""

from typing import List, Dict, Any, Optional, Tuple
from dataclasses import dataclass
import math
import numpy as np

from .smart_tokenizer import Token, TokenCategory


@dataclass
class AttentionOutput:
    """Output from attention computation."""
    
    weighted_tokens: List[Tuple[Token, float]]  # (token, attention_weight)
    context_vector: np.ndarray
    confidence: float
    dominant_category: TokenCategory


class MultiHeadAttention:
    """
    Multi-head attention mechanism for token context awareness.
    
    Creates embeddings for tokens and applies self-attention
    to understand relationships and calculate confidence scores.
    """
    
    def __init__(
        self,
        embedding_dim: int = 64,
        num_heads: int = 4,
        dropout: float = 0.1
    ):
        """
        Initialize multi-head attention.
        
        Args:
            embedding_dim: Dimension of token embeddings
            num_heads: Number of attention heads
            dropout: Dropout rate (for training)
        """
        self.embedding_dim = embedding_dim
        self.num_heads = num_heads
        self.head_dim = embedding_dim // num_heads
        self.dropout = dropout
        
        # Initialize projection matrices (simplified)
        np.random.seed(42)  # For reproducibility
        self.W_q = np.random.randn(embedding_dim, embedding_dim) * 0.1
        self.W_k = np.random.randn(embedding_dim, embedding_dim) * 0.1
        self.W_v = np.random.randn(embedding_dim, embedding_dim) * 0.1
        self.W_o = np.random.randn(embedding_dim, embedding_dim) * 0.1
        
        # Category embeddings
        self._category_embeddings: Dict[TokenCategory, np.ndarray] = {}
        self._init_category_embeddings()
    
    def _init_category_embeddings(self) -> None:
        """Initialize embeddings for each token category."""
        for i, category in enumerate(TokenCategory):
            # Create distinct embedding for each category
            embedding = np.zeros(self.embedding_dim)
            start = i * (self.embedding_dim // len(TokenCategory))
            end = start + (self.embedding_dim // len(TokenCategory))
            embedding[start:end] = 1.0
            self._category_embeddings[category] = embedding
    
    def embed_token(self, token: Token) -> np.ndarray:
        """
        Create embedding for a single token.
        
        Combines:
        - Category embedding
        - Clarity score
        - Name hash (for uniqueness)
        """
        # Start with category embedding
        embedding = self._category_embeddings[token.category].copy()
        
        # Add clarity score influence
        embedding *= (0.5 + token.clarity_score * 0.5)
        
        # Add name hash for uniqueness
        name_hash = hash(token.value) % 1000 / 1000.0
        embedding[0] = name_hash
        
        # Normalize
        norm = np.linalg.norm(embedding)
        if norm > 0:
            embedding = embedding / norm
        
        return embedding
    
    def embed_tokens(self, tokens: List[Token]) -> np.ndarray:
        """
        Create embeddings for a list of tokens.
        
        Returns:
            Matrix of shape (num_tokens, embedding_dim)
        """
        if not tokens:
            return np.zeros((1, self.embedding_dim))
        
        embeddings = np.array([self.embed_token(t) for t in tokens])
        return embeddings
    
    def attention(
        self,
        query: np.ndarray,
        key: np.ndarray,
        value: np.ndarray,
        mask: Optional[np.ndarray] = None
    ) -> Tuple[np.ndarray, np.ndarray]:
        """
        Compute scaled dot-product attention.
        
        Args:
            query: Query matrix
            key: Key matrix
            value: Value matrix
            mask: Optional attention mask
            
        Returns:
            Tuple of (output, attention_weights)
        """
        # Scaled dot-product attention
        d_k = query.shape[-1]
        scores = np.matmul(query, key.T) / math.sqrt(d_k)
        
        if mask is not None:
            scores = np.where(mask, scores, -1e9)
        
        # Softmax
        attention_weights = self._softmax(scores)
        
        # Apply attention to values
        output = np.matmul(attention_weights, value)
        
        return output, attention_weights
    
    def _softmax(self, x: np.ndarray) -> np.ndarray:
        """Compute softmax."""
        exp_x = np.exp(x - np.max(x, axis=-1, keepdims=True))
        return exp_x / np.sum(exp_x, axis=-1, keepdims=True)
    
    def forward(self, tokens: List[Token]) -> AttentionOutput:
        """
        Process tokens through multi-head attention.
        
        Args:
            tokens: List of tokens to process
            
        Returns:
            AttentionOutput with weighted tokens and confidence
        """
        if not tokens:
            return AttentionOutput(
                weighted_tokens=[],
                context_vector=np.zeros(self.embedding_dim),
                confidence=0.0,
                dominant_category=TokenCategory.GENERAL
            )
        
        # Get embeddings
        embeddings = self.embed_tokens(tokens)
        
        # Project to Q, K, V
        Q = np.matmul(embeddings, self.W_q)
        K = np.matmul(embeddings, self.W_k)
        V = np.matmul(embeddings, self.W_v)
        
        # Multi-head attention (simplified - single head for efficiency)
        output, attention_weights = self.attention(Q, K, V)
        
        # Project output
        output = np.matmul(output, self.W_o)
        
        # Calculate context vector (mean pooling)
        context_vector = np.mean(output, axis=0)
        
        # Get attention weights for each token (average across positions)
        token_weights = np.mean(attention_weights, axis=0)
        
        # Create weighted tokens list
        weighted_tokens = [
            (token, float(weight))
            for token, weight in zip(tokens, token_weights)
        ]
        
        # Sort by weight
        weighted_tokens.sort(key=lambda x: x[1], reverse=True)
        
        # Calculate confidence based on attention distribution
        confidence = self._calculate_confidence(token_weights)
        
        # Determine dominant category
        dominant_category = self._get_dominant_category(tokens, token_weights)
        
        return AttentionOutput(
            weighted_tokens=weighted_tokens,
            context_vector=context_vector,
            confidence=confidence,
            dominant_category=dominant_category
        )
    
    def _calculate_confidence(self, weights: np.ndarray) -> float:
        """
        Calculate confidence based on attention weight distribution.
        
        Higher confidence when attention is focused (low entropy).
        """
        # Avoid log(0)
        weights = np.clip(weights, 1e-10, 1.0)
        
        # Calculate entropy
        entropy = -np.sum(weights * np.log(weights))
        
        # Normalize entropy to [0, 1]
        max_entropy = np.log(len(weights)) if len(weights) > 1 else 1
        normalized_entropy = entropy / max_entropy if max_entropy > 0 else 0
        
        # Convert to confidence (lower entropy = higher confidence)
        confidence = 1.0 - normalized_entropy
        
        return float(confidence)
    
    def _get_dominant_category(
        self,
        tokens: List[Token],
        weights: np.ndarray
    ) -> TokenCategory:
        """Get the dominant category based on weighted tokens."""
        category_weights: Dict[TokenCategory, float] = {}
        
        for token, weight in zip(tokens, weights):
            category_weights[token.category] = (
                category_weights.get(token.category, 0) + weight
            )
        
        if not category_weights:
            return TokenCategory.GENERAL
        
        return max(category_weights, key=category_weights.get)
    
    def get_attention_map(
        self,
        tokens: List[Token]
    ) -> Dict[str, Dict[str, float]]:
        """
        Get attention map showing token-to-token attention.
        
        Returns:
            Dict mapping token pairs to attention weights
        """
        embeddings = self.embed_tokens(tokens)
        Q = np.matmul(embeddings, self.W_q)
        K = np.matmul(embeddings, self.W_k)
        
        _, attention_weights = self.attention(Q, K, embeddings)
        
        attention_map: Dict[str, Dict[str, float]] = {}
        for i, token_i in enumerate(tokens):
            attention_map[token_i.value] = {}
            for j, token_j in enumerate(tokens):
                attention_map[token_i.value][token_j.value] = float(
                    attention_weights[i, j]
                )
        
        return attention_map
