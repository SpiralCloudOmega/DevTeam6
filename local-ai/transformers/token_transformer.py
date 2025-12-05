"""
DevTeam6 Local AI - Token Transformer

Main token processor with deterministic routing logic.
"""

from typing import Dict, Any, List, Optional, Tuple
from dataclasses import dataclass, field
from enum import Enum

from .components.smart_tokenizer import SmartTokenizer, Token, TokenCategory
from .components.multi_head_attention import MultiHeadAttention, AttentionOutput


class RouteType(Enum):
    """Types of routes for token processing."""
    LOCAL = "local"      # Process locally with Ollama
    API = "api"          # External API call required
    SECURITY = "security"  # Security validation required
    DATA = "data"        # Data operation
    HYBRID = "hybrid"    # Multiple routes needed


@dataclass
class RoutingDecision:
    """Decision about how to route a token/request."""
    
    route_type: RouteType
    confidence: float
    indicators: List[str]
    metadata: Dict[str, Any] = field(default_factory=dict)


@dataclass
class TokenTransformResult:
    """Result of token transformation."""
    
    original_input: str
    tokens: List[Token]
    attention_output: AttentionOutput
    routing_decision: RoutingDecision
    processed_output: Optional[str] = None
    confidence: float = 0.0


class TokenTransformer:
    """
    Main token processor with deterministic routing.
    
    Features:
    - AST-based tokenization
    - Attention-based context analysis
    - Deterministic routing logic
    - Confidence calculation
    """
    
    # Thresholds for routing decisions
    API_THRESHOLD = 0.6
    SECURITY_THRESHOLD = 0.5
    LOCAL_THRESHOLD = 0.7
    
    def __init__(
        self,
        embedding_dim: int = 64,
        num_attention_heads: int = 4
    ):
        """
        Initialize token transformer.
        
        Args:
            embedding_dim: Dimension for token embeddings
            num_attention_heads: Number of attention heads
        """
        self.tokenizer = SmartTokenizer()
        self.attention = MultiHeadAttention(
            embedding_dim=embedding_dim,
            num_heads=num_attention_heads
        )
        
        # Cache for processed results
        self._cache: Dict[int, TokenTransformResult] = {}
        
        # Metrics
        self._metrics = {
            "total_processed": 0,
            "api_routes": 0,
            "security_routes": 0,
            "local_routes": 0,
            "hybrid_routes": 0
        }
    
    def process(self, input_text: str, language: str = "python") -> TokenTransformResult:
        """
        Process input text through the token transformer.
        
        Args:
            input_text: Text to process
            language: Programming language for tokenization
            
        Returns:
            TokenTransformResult with routing decision
        """
        # Check cache
        cache_key = hash(input_text)
        if cache_key in self._cache:
            return self._cache[cache_key]
        
        # Tokenize
        tokens = self.tokenizer.tokenize(input_text, language)
        
        # Apply attention
        attention_output = self.attention.forward(tokens)
        
        # Determine routing
        routing_decision = self._determine_routing(tokens, attention_output)
        
        # Calculate overall confidence
        confidence = self._calculate_confidence(tokens, attention_output, routing_decision)
        
        # Create result
        result = TokenTransformResult(
            original_input=input_text,
            tokens=tokens,
            attention_output=attention_output,
            routing_decision=routing_decision,
            confidence=confidence
        )
        
        # Update metrics
        self._update_metrics(routing_decision)
        
        # Cache result
        self._cache[cache_key] = result
        
        return result
    
    def _determine_routing(
        self,
        tokens: List[Token],
        attention: AttentionOutput
    ) -> RoutingDecision:
        """
        Determine routing based on token analysis.
        
        Uses deterministic rules based on token categories and attention patterns.
        """
        indicators: List[str] = []
        scores: Dict[RouteType, float] = {
            RouteType.LOCAL: 0.0,
            RouteType.API: 0.0,
            RouteType.SECURITY: 0.0,
            RouteType.DATA: 0.0
        }
        
        # Analyze token categories
        category_summary = self.tokenizer.get_category_summary(tokens)
        total_tokens = len(tokens) if tokens else 1
        
        # API indicators
        api_count = category_summary.get(TokenCategory.API, 0)
        if api_count > 0:
            api_ratio = api_count / total_tokens
            scores[RouteType.API] += api_ratio * 0.5
            indicators.append(f"api_tokens:{api_count}")
            
            # Check for HTTP-related patterns
            http_tokens = [t for t in tokens if any(
                kw in t.value.lower() for kw in ["http", "request", "fetch", "api"]
            )]
            if http_tokens:
                scores[RouteType.API] += 0.3
                indicators.append("http_patterns_found")
        
        # Security indicators
        security_count = category_summary.get(TokenCategory.SECURITY, 0)
        if security_count > 0:
            security_ratio = security_count / total_tokens
            scores[RouteType.SECURITY] += security_ratio * 0.5
            indicators.append(f"security_tokens:{security_count}")
            
            # Check for auth-related patterns
            auth_tokens = [t for t in tokens if any(
                kw in t.value.lower() for kw in ["auth", "token", "password", "key"]
            )]
            if auth_tokens:
                scores[RouteType.SECURITY] += 0.3
                indicators.append("auth_patterns_found")
        
        # Data indicators
        data_count = category_summary.get(TokenCategory.DATA, 0)
        if data_count > 0:
            data_ratio = data_count / total_tokens
            scores[RouteType.DATA] += data_ratio * 0.4
            indicators.append(f"data_tokens:{data_count}")
        
        # Local processing indicators (general code)
        general_count = category_summary.get(TokenCategory.GENERAL, 0)
        function_count = category_summary.get(TokenCategory.FUNCTION, 0)
        
        local_count = general_count + function_count
        if local_count > 0:
            local_ratio = local_count / total_tokens
            scores[RouteType.LOCAL] += local_ratio * 0.4
            indicators.append(f"local_tokens:{local_count}")
        
        # Attention-based adjustments
        if attention.dominant_category == TokenCategory.API:
            scores[RouteType.API] += 0.2
            indicators.append("attention_dominant:api")
        elif attention.dominant_category == TokenCategory.SECURITY:
            scores[RouteType.SECURITY] += 0.2
            indicators.append("attention_dominant:security")
        
        # Confidence-based adjustment
        scores[RouteType.LOCAL] += attention.confidence * 0.2
        
        # Determine primary route
        primary_route = max(scores, key=scores.get)
        primary_score = scores[primary_route]
        
        # Check for hybrid routing (multiple high scores)
        high_scores = [rt for rt, score in scores.items() if score >= 0.3]
        if len(high_scores) > 1 and primary_score < 0.6:
            primary_route = RouteType.HYBRID
            indicators.append(f"multiple_routes:{','.join(rt.value for rt in high_scores)}")
        
        return RoutingDecision(
            route_type=primary_route,
            confidence=primary_score,
            indicators=indicators,
            metadata={
                "scores": {rt.value: score for rt, score in scores.items()},
                "category_summary": {cat.value: count for cat, count in category_summary.items()},
                "total_tokens": total_tokens
            }
        )
    
    def _calculate_confidence(
        self,
        tokens: List[Token],
        attention: AttentionOutput,
        routing: RoutingDecision
    ) -> float:
        """Calculate overall processing confidence."""
        if not tokens:
            return 0.0
        
        # Component weights
        token_clarity_weight = 0.3
        attention_confidence_weight = 0.3
        routing_confidence_weight = 0.4
        
        # Average token clarity
        avg_clarity = sum(t.clarity_score for t in tokens) / len(tokens)
        
        # Weighted confidence
        confidence = (
            avg_clarity * token_clarity_weight +
            attention.confidence * attention_confidence_weight +
            routing.confidence * routing_confidence_weight
        )
        
        return min(max(confidence, 0.0), 1.0)
    
    def _update_metrics(self, routing: RoutingDecision) -> None:
        """Update processing metrics."""
        self._metrics["total_processed"] += 1
        
        route_metric_map = {
            RouteType.API: "api_routes",
            RouteType.SECURITY: "security_routes",
            RouteType.LOCAL: "local_routes",
            RouteType.HYBRID: "hybrid_routes",
            RouteType.DATA: "local_routes"  # Data counted as local
        }
        
        metric_key = route_metric_map.get(routing.route_type, "local_routes")
        self._metrics[metric_key] += 1
    
    def get_api_indicators(self, result: TokenTransformResult) -> List[str]:
        """Get API-specific indicators from a result."""
        api_tokens = self.tokenizer.filter_by_category(
            result.tokens, TokenCategory.API
        )
        return [t.value for t in api_tokens]
    
    def get_security_indicators(self, result: TokenTransformResult) -> List[str]:
        """Get security-specific indicators from a result."""
        security_tokens = self.tokenizer.filter_by_category(
            result.tokens, TokenCategory.SECURITY
        )
        return [t.value for t in security_tokens]
    
    def get_local_indicators(self, result: TokenTransformResult) -> List[str]:
        """Get local processing indicators from a result."""
        general_tokens = self.tokenizer.filter_by_category(
            result.tokens, TokenCategory.GENERAL
        )
        function_tokens = self.tokenizer.filter_by_category(
            result.tokens, TokenCategory.FUNCTION
        )
        return [t.value for t in general_tokens + function_tokens]
    
    def get_metrics(self) -> Dict[str, Any]:
        """Get transformer metrics."""
        return {
            **self._metrics,
            "cache_size": len(self._cache)
        }
    
    def clear_cache(self) -> None:
        """Clear the processing cache."""
        self._cache.clear()
