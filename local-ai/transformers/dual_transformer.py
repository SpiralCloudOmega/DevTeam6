"""
DevTeam6 Local AI - Dual Transformer

Orchestration layer that routes between token and security transformers.
"""

from typing import Dict, Any, List, Optional
from dataclasses import dataclass, field
from datetime import datetime
from enum import Enum
import hashlib
import heapq
import time

from .token_transformer import TokenTransformer, TokenTransformResult, RouteType
from .security_transformer import SecurityTransformer, SecurityValidationResult
from .components.auth_manager import AuthManager, SecurityLevel


class ProcessingStatus(Enum):
    """Status of dual transformer processing."""
    SUCCESS = "success"
    PARTIAL = "partial"
    SECURITY_BLOCKED = "security_blocked"
    ERROR = "error"
    CACHED = "cached"


@dataclass
class DualTransformResult:
    """Result from dual transformer processing."""
    
    status: ProcessingStatus
    token_result: TokenTransformResult
    security_result: Optional[SecurityValidationResult]
    final_route: RouteType
    confidence: float
    processing_time_ms: float
    metadata: Dict[str, Any] = field(default_factory=dict)


class DualTransformer:
    """
    Orchestration layer for the Dual Transformer system.
    
    Routes requests between:
    - TokenTransformer: For code analysis and routing decisions
    - SecurityTransformer: For auth validation and security checks
    
    Features:
    - Intelligent routing based on content analysis
    - Security enforcement
    - Metrics tracking
    - Result caching
    """
    
    def __init__(
        self,
        auth_manager: Optional[AuthManager] = None,
        cache_ttl_seconds: int = 300,
        max_cache_size: int = 1000
    ):
        """
        Initialize dual transformer.
        
        Args:
            auth_manager: Shared auth manager
            cache_ttl_seconds: Cache time-to-live
            max_cache_size: Maximum cache entries
        """
        self.auth_manager = auth_manager or AuthManager()
        self.cache_ttl_seconds = cache_ttl_seconds
        self.max_cache_size = max_cache_size
        
        # Initialize sub-transformers
        self.token_transformer = TokenTransformer()
        self.security_transformer = SecurityTransformer(
            auth_manager=self.auth_manager
        )
        
        # Result cache: hash -> (result, timestamp)
        self._cache: Dict[str, tuple] = {}
        
        # Metrics
        self._metrics = {
            "total_requests": 0,
            "cache_hits": 0,
            "token_only": 0,
            "security_validated": 0,
            "security_blocked": 0,
            "errors": 0,
            "total_processing_time_ms": 0
        }
    
    def process(
        self,
        input_text: str,
        token_id: Optional[str] = None,
        language: str = "python",
        bypass_security: bool = False,
        request_metadata: Optional[Dict[str, Any]] = None
    ) -> DualTransformResult:
        """
        Process input through the dual transformer system.
        
        Args:
            input_text: Text to process
            token_id: Authentication token (optional)
            language: Programming language
            bypass_security: Skip security checks (for testing)
            request_metadata: Additional request info
            
        Returns:
            DualTransformResult with processing results
        """
        start_time = time.time()
        self._metrics["total_requests"] += 1
        
        # Check cache
        cache_key = self._make_cache_key(input_text, token_id)
        cached = self._get_cached(cache_key)
        if cached:
            self._metrics["cache_hits"] += 1
            cached.status = ProcessingStatus.CACHED
            return cached
        
        try:
            # Step 1: Token transformation
            token_result = self.token_transformer.process(input_text, language)
            
            # Step 2: Determine if security validation is needed
            needs_security = self._needs_security_validation(token_result)
            
            security_result = None
            final_route = token_result.routing_decision.route_type
            status = ProcessingStatus.SUCCESS
            
            if needs_security and not bypass_security:
                # Step 3: Security validation
                security_result = self.security_transformer.validate_request(
                    token_result=token_result,
                    token_id=token_id,
                    request_metadata=request_metadata
                )
                
                if not security_result.is_valid:
                    self._metrics["security_blocked"] += 1
                    status = ProcessingStatus.SECURITY_BLOCKED
                    final_route = RouteType.SECURITY  # Force security route
                else:
                    self._metrics["security_validated"] += 1
            else:
                self._metrics["token_only"] += 1
            
            # Calculate processing time
            processing_time = (time.time() - start_time) * 1000
            self._metrics["total_processing_time_ms"] += processing_time
            
            # Calculate confidence
            confidence = self._calculate_combined_confidence(
                token_result, security_result
            )
            
            result = DualTransformResult(
                status=status,
                token_result=token_result,
                security_result=security_result,
                final_route=final_route,
                confidence=confidence,
                processing_time_ms=processing_time,
                metadata={
                    "language": language,
                    "needs_security": needs_security,
                    "token_count": len(token_result.tokens)
                }
            )
            
            # Cache result
            self._cache_result(cache_key, result)
            
            return result
            
        except Exception as e:
            self._metrics["errors"] += 1
            processing_time = (time.time() - start_time) * 1000
            
            # Create error result with minimal token processing
            return DualTransformResult(
                status=ProcessingStatus.ERROR,
                token_result=TokenTransformResult(
                    original_input=input_text,
                    tokens=[],
                    attention_output=None,
                    routing_decision=None,
                    confidence=0.0
                ),
                security_result=None,
                final_route=RouteType.LOCAL,
                confidence=0.0,
                processing_time_ms=processing_time,
                metadata={"error": str(e)}
            )
    
    def _needs_security_validation(self, token_result: TokenTransformResult) -> bool:
        """Determine if security validation is needed."""
        routing = token_result.routing_decision
        
        # Always validate for security or hybrid routes
        if routing.route_type in [RouteType.SECURITY, RouteType.HYBRID]:
            return True
        
        # Validate for API routes
        if routing.route_type == RouteType.API:
            return True
        
        # Check for security indicators
        security_indicators = [
            ind for ind in routing.indicators
            if "security" in ind.lower() or "auth" in ind.lower()
        ]
        
        return len(security_indicators) > 0
    
    def _calculate_combined_confidence(
        self,
        token_result: TokenTransformResult,
        security_result: Optional[SecurityValidationResult]
    ) -> float:
        """Calculate combined confidence from both transformers."""
        token_confidence = token_result.confidence
        
        if security_result is None:
            return token_confidence
        
        # If security validation failed, lower confidence
        if not security_result.is_valid:
            return token_confidence * 0.5
        
        # If security passed, boost confidence slightly
        security_boost = 0.1 if security_result.security_level.value >= 2 else 0.05
        
        return min(token_confidence + security_boost, 1.0)
    
    def _make_cache_key(self, input_text: str, token_id: Optional[str]) -> str:
        """Create cache key from input and token."""
        key_material = f"{input_text}:{token_id or 'anon'}"
        return hashlib.sha256(key_material.encode()).hexdigest()[:16]
    
    def _get_cached(self, key: str) -> Optional[DualTransformResult]:
        """Get result from cache if valid."""
        if key not in self._cache:
            return None
        
        result, timestamp = self._cache[key]
        
        # Check TTL
        age = time.time() - timestamp
        if age > self.cache_ttl_seconds:
            del self._cache[key]
            return None
        
        return result
    
    def _cache_result(self, key: str, result: DualTransformResult) -> None:
        """Cache a result."""
        # Evict old entries if cache is full
        if len(self._cache) >= self.max_cache_size:
            self._evict_oldest()
        
        self._cache[key] = (result, time.time())
    
    def _evict_oldest(self) -> None:
        """
        Evict oldest cache entries using efficient sorting.
        
        Performance: Uses a single pass to find and remove oldest 10% of entries.
        """
        if not self._cache:
            return
        
        # Calculate number to remove
        to_remove = max(1, len(self._cache) // 10)
        
        # Use min heap to find oldest entries efficiently
        # Format: (timestamp, key)
        oldest = heapq.nsmallest(
            to_remove,
            ((timestamp, key) for key, (_, timestamp) in self._cache.items()),
            key=lambda x: x[0]
        )
        
        # Remove oldest entries
        for _, key in oldest:
            del self._cache[key]
    
    def route_to_handler(
        self,
        result: DualTransformResult
    ) -> Dict[str, Any]:
        """
        Determine which handler should process the result.
        
        Args:
            result: Dual transform result
            
        Returns:
            Handler routing info
        """
        route = result.final_route
        
        handlers = {
            RouteType.LOCAL: {
                "handler": "ollama_service",
                "method": "generate",
                "requires_auth": False
            },
            RouteType.API: {
                "handler": "api_gateway",
                "method": "forward_request",
                "requires_auth": True
            },
            RouteType.SECURITY: {
                "handler": "security_service",
                "method": "process_secure",
                "requires_auth": True
            },
            RouteType.DATA: {
                "handler": "data_service",
                "method": "query",
                "requires_auth": False
            },
            RouteType.HYBRID: {
                "handler": "orchestrator",
                "method": "multi_route",
                "requires_auth": True
            }
        }
        
        return handlers.get(route, handlers[RouteType.LOCAL])
    
    def get_metrics(self) -> Dict[str, Any]:
        """Get combined metrics from all transformers."""
        token_metrics = self.token_transformer.get_metrics()
        security_metrics = self.security_transformer.get_metrics()
        
        avg_time = 0
        if self._metrics["total_requests"] > 0:
            avg_time = (
                self._metrics["total_processing_time_ms"] /
                self._metrics["total_requests"]
            )
        
        return {
            "dual_transformer": {
                **self._metrics,
                "cache_size": len(self._cache),
                "avg_processing_time_ms": avg_time
            },
            "token_transformer": token_metrics,
            "security_transformer": security_metrics
        }
    
    def clear_cache(self) -> int:
        """Clear all caches."""
        count = len(self._cache)
        self._cache.clear()
        self.token_transformer.clear_cache()
        return count
    
    def create_token(
        self,
        user_id: str,
        security_level: SecurityLevel = SecurityLevel.AUTHENTICATED,
        permissions: Optional[set] = None
    ) -> str:
        """
        Create an authentication token.
        
        Convenience method that delegates to auth manager.
        
        Args:
            user_id: User identifier
            security_level: Token security level
            permissions: Explicit permissions
            
        Returns:
            Token ID
        """
        token = self.auth_manager.generate_token(
            user_id=user_id,
            security_level=security_level,
            permissions=permissions
        )
        return token.token_id
