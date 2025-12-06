"""
DevTeam6 Local AI - API Gateway

Secure API execution with endpoint validation, request signing, and rate limiting.
"""

from typing import Dict, Any, Optional, List, Callable, Awaitable
from dataclasses import dataclass, field
from datetime import datetime
from enum import Enum
import hashlib
import hmac
import time
import asyncio

from .auth_manager import AuthManager, SecurityLevel
from .rate_limiter import RateLimiter


class HTTPMethod(Enum):
    """Supported HTTP methods."""
    GET = "GET"
    POST = "POST"
    PUT = "PUT"
    DELETE = "DELETE"
    PATCH = "PATCH"


@dataclass
class APIEndpoint:
    """Definition of an API endpoint."""
    
    path: str
    method: HTTPMethod
    required_permissions: set = field(default_factory=set)
    security_level: SecurityLevel = SecurityLevel.AUTHENTICATED
    rate_limit: int = 100  # requests per window
    rate_window: int = 60  # window in seconds
    handler: Optional[Callable[..., Awaitable[Any]]] = None
    description: str = ""


@dataclass
class APIRequest:
    """Incoming API request."""
    
    endpoint: str
    method: HTTPMethod
    headers: Dict[str, str]
    body: Optional[Dict[str, Any]] = None
    query_params: Optional[Dict[str, str]] = None
    client_id: Optional[str] = None
    timestamp: datetime = field(default_factory=datetime.utcnow)


@dataclass
class APIResponse:
    """API response."""
    
    success: bool
    status_code: int
    data: Optional[Any] = None
    error: Optional[str] = None
    headers: Dict[str, str] = field(default_factory=dict)


class APIGateway:
    """
    Secure API gateway for request handling.
    
    Features:
    - Endpoint registration and validation
    - Request signing and verification
    - Rate limit tracking
    - Security level enforcement
    """
    
    def __init__(
        self,
        auth_manager: Optional[AuthManager] = None,
        signing_key: Optional[str] = None,
        default_rate_limit: int = 100,
        default_rate_window: int = 60
    ):
        """
        Initialize API gateway.
        
        Args:
            auth_manager: Auth manager instance
            signing_key: Key for request signing
            default_rate_limit: Default rate limit per client
            default_rate_window: Default rate window in seconds
        """
        self.auth_manager = auth_manager or AuthManager()
        self.signing_key = signing_key or "devteam6-default-key"
        self.default_rate_limit = default_rate_limit
        self.default_rate_window = default_rate_window
        
        # Endpoint registry
        self._endpoints: Dict[str, APIEndpoint] = {}
        
        # Rate limiters per endpoint
        self._rate_limiters: Dict[str, RateLimiter] = {}
        
        # Request metrics
        self._metrics: Dict[str, int] = {
            "total_requests": 0,
            "successful_requests": 0,
            "failed_requests": 0,
            "rate_limited_requests": 0,
            "unauthorized_requests": 0
        }
    
    def register_endpoint(
        self,
        path: str,
        method: HTTPMethod,
        handler: Callable[..., Awaitable[Any]],
        required_permissions: Optional[set] = None,
        security_level: SecurityLevel = SecurityLevel.AUTHENTICATED,
        rate_limit: Optional[int] = None,
        rate_window: Optional[int] = None,
        description: str = ""
    ) -> None:
        """
        Register an API endpoint.
        
        Args:
            path: Endpoint path
            method: HTTP method
            handler: Async handler function
            required_permissions: Required permissions
            security_level: Required security level
            rate_limit: Rate limit override
            rate_window: Rate window override
            description: Endpoint description
        """
        endpoint_key = f"{method.value}:{path}"
        
        endpoint = APIEndpoint(
            path=path,
            method=method,
            required_permissions=required_permissions or set(),
            security_level=security_level,
            rate_limit=rate_limit or self.default_rate_limit,
            rate_window=rate_window or self.default_rate_window,
            handler=handler,
            description=description
        )
        
        self._endpoints[endpoint_key] = endpoint
        
        # Create rate limiter for endpoint
        self._rate_limiters[endpoint_key] = RateLimiter(
            max_requests=endpoint.rate_limit,
            window_seconds=endpoint.rate_window
        )
    
    def validate_endpoint(
        self,
        path: str,
        method: HTTPMethod
    ) -> Optional[APIEndpoint]:
        """
        Validate that an endpoint exists.
        
        Args:
            path: Endpoint path
            method: HTTP method
            
        Returns:
            APIEndpoint if valid, None otherwise
        """
        endpoint_key = f"{method.value}:{path}"
        return self._endpoints.get(endpoint_key)
    
    def sign_request(
        self,
        request: APIRequest
    ) -> str:
        """
        Generate signature for a request.
        
        Args:
            request: Request to sign
            
        Returns:
            Request signature
        """
        # Create signing string
        sign_string = f"{request.method.value}:{request.endpoint}:{request.timestamp.isoformat()}"
        
        if request.body:
            import json
            body_str = json.dumps(request.body, sort_keys=True)
            sign_string += f":{body_str}"
        
        # Generate HMAC signature
        signature = hmac.new(
            self.signing_key.encode(),
            sign_string.encode(),
            hashlib.sha256
        ).hexdigest()
        
        return signature
    
    def verify_signature(
        self,
        request: APIRequest,
        provided_signature: str,
        max_age_seconds: int = 300
    ) -> bool:
        """
        Verify a request signature.
        
        Args:
            request: Request to verify
            provided_signature: Signature from request
            max_age_seconds: Maximum age of request
            
        Returns:
            True if signature is valid
        """
        # Check request age
        age = (datetime.utcnow() - request.timestamp).total_seconds()
        if age > max_age_seconds:
            return False
        
        # Generate expected signature
        expected = self.sign_request(request)
        
        # Constant-time comparison
        return hmac.compare_digest(expected, provided_signature)
    
    async def handle_request(
        self,
        request: APIRequest,
        token_id: Optional[str] = None
    ) -> APIResponse:
        """
        Handle an incoming API request.
        
        Args:
            request: Incoming request
            token_id: Optional auth token
            
        Returns:
            API response
        """
        self._metrics["total_requests"] += 1
        
        # Validate endpoint
        endpoint = self.validate_endpoint(request.endpoint, request.method)
        if endpoint is None:
            self._metrics["failed_requests"] += 1
            return APIResponse(
                success=False,
                status_code=404,
                error=f"Endpoint not found: {request.method.value} {request.endpoint}"
            )
        
        endpoint_key = f"{request.method.value}:{request.endpoint}"
        
        # Check rate limit
        client_id = request.client_id or "anonymous"
        rate_limiter = self._rate_limiters.get(endpoint_key)
        
        if rate_limiter and not rate_limiter.allow_request(client_id):
            self._metrics["rate_limited_requests"] += 1
            return APIResponse(
                success=False,
                status_code=429,
                error="Rate limit exceeded",
                headers={
                    "Retry-After": str(rate_limiter.window_seconds),
                    "X-RateLimit-Limit": str(rate_limiter.max_requests),
                    "X-RateLimit-Remaining": "0"
                }
            )
        
        # Check authentication
        if endpoint.security_level != SecurityLevel.PUBLIC:
            if token_id is None:
                self._metrics["unauthorized_requests"] += 1
                return APIResponse(
                    success=False,
                    status_code=401,
                    error="Authentication required"
                )
            
            # Validate token and permissions
            assessment = self.auth_manager.assess_request_security(
                token_id,
                endpoint.required_permissions,
                endpoint.security_level
            )
            
            if not assessment["allowed"]:
                self._metrics["unauthorized_requests"] += 1
                return APIResponse(
                    success=False,
                    status_code=403,
                    error=assessment["reason"]
                )
        
        # Execute handler
        if endpoint.handler:
            try:
                result = await endpoint.handler(request)
                self._metrics["successful_requests"] += 1
                return APIResponse(
                    success=True,
                    status_code=200,
                    data=result
                )
            except Exception as e:
                self._metrics["failed_requests"] += 1
                return APIResponse(
                    success=False,
                    status_code=500,
                    error=str(e)
                )
        
        self._metrics["successful_requests"] += 1
        return APIResponse(
            success=True,
            status_code=200,
            data={"message": "Endpoint reached"}
        )
    
    def list_endpoints(self) -> List[Dict[str, Any]]:
        """List all registered endpoints."""
        return [
            {
                "path": ep.path,
                "method": ep.method.value,
                "security_level": ep.security_level.name,
                "rate_limit": ep.rate_limit,
                "description": ep.description
            }
            for ep in self._endpoints.values()
        ]
    
    def get_metrics(self) -> Dict[str, Any]:
        """Get gateway metrics."""
        return {
            **self._metrics,
            "registered_endpoints": len(self._endpoints)
        }
    
    def reset_metrics(self) -> None:
        """Reset metrics counters."""
        for key in self._metrics:
            self._metrics[key] = 0
