"""
DevTeam6 Local AI - Rate Limiter

Window-based rate limiting with client tracking and automatic cleanup.
"""

from typing import Dict, Any, Optional
from dataclasses import dataclass, field
from datetime import datetime, timedelta
from collections import defaultdict
import time
import threading


@dataclass
class RateLimitEntry:
    """Rate limit tracking entry for a client."""
    
    client_id: str
    request_count: int
    window_start: float
    last_request: float


class RateLimiter:
    """
    Window-based rate limiter.
    
    Features:
    - Sliding window rate limiting
    - Per-client tracking
    - Automatic cleanup of expired entries
    - Thread-safe operations
    """
    
    def __init__(
        self,
        max_requests: int = 100,
        window_seconds: int = 60,
        cleanup_interval: int = 300
    ):
        """
        Initialize rate limiter.
        
        Args:
            max_requests: Maximum requests per window
            window_seconds: Window duration in seconds
            cleanup_interval: Interval for cleanup in seconds
        """
        self.max_requests = max_requests
        self.window_seconds = window_seconds
        self.cleanup_interval = cleanup_interval
        
        # Client tracking: client_id -> RateLimitEntry
        self._clients: Dict[str, RateLimitEntry] = {}
        
        # Lock for thread safety
        self._lock = threading.RLock()
        
        # Last cleanup time
        self._last_cleanup = time.time()
    
    def allow_request(self, client_id: str) -> bool:
        """
        Check if a request is allowed for a client.
        
        Args:
            client_id: Client identifier
            
        Returns:
            True if request is allowed
        """
        current_time = time.time()
        
        with self._lock:
            # Run cleanup if needed
            self._maybe_cleanup(current_time)
            
            # Get or create client entry
            entry = self._clients.get(client_id)
            
            if entry is None:
                # New client
                self._clients[client_id] = RateLimitEntry(
                    client_id=client_id,
                    request_count=1,
                    window_start=current_time,
                    last_request=current_time
                )
                return True
            
            # Check if window has expired
            window_age = current_time - entry.window_start
            
            if window_age >= self.window_seconds:
                # Start new window
                entry.request_count = 1
                entry.window_start = current_time
                entry.last_request = current_time
                return True
            
            # Check if within limit
            if entry.request_count < self.max_requests:
                entry.request_count += 1
                entry.last_request = current_time
                return True
            
            # Rate limited
            return False
    
    def get_remaining(self, client_id: str) -> int:
        """
        Get remaining requests for a client.
        
        Args:
            client_id: Client identifier
            
        Returns:
            Number of remaining requests
        """
        current_time = time.time()
        
        with self._lock:
            entry = self._clients.get(client_id)
            
            if entry is None:
                return self.max_requests
            
            # Check if window has expired
            window_age = current_time - entry.window_start
            
            if window_age >= self.window_seconds:
                return self.max_requests
            
            return max(0, self.max_requests - entry.request_count)
    
    def get_reset_time(self, client_id: str) -> float:
        """
        Get time until rate limit resets for a client.
        
        Args:
            client_id: Client identifier
            
        Returns:
            Seconds until reset
        """
        current_time = time.time()
        
        with self._lock:
            entry = self._clients.get(client_id)
            
            if entry is None:
                return 0
            
            window_age = current_time - entry.window_start
            
            if window_age >= self.window_seconds:
                return 0
            
            return self.window_seconds - window_age
    
    def reset_client(self, client_id: str) -> None:
        """
        Reset rate limit for a client.
        
        Args:
            client_id: Client to reset
        """
        with self._lock:
            self._clients.pop(client_id, None)
    
    def _maybe_cleanup(self, current_time: float) -> None:
        """Run cleanup if interval has passed."""
        if current_time - self._last_cleanup >= self.cleanup_interval:
            self._cleanup(current_time)
            self._last_cleanup = current_time
    
    def _cleanup(self, current_time: float) -> int:
        """
        Remove expired entries.
        
        Args:
            current_time: Current timestamp
            
        Returns:
            Number of entries removed
        """
        expired = []
        
        for client_id, entry in self._clients.items():
            window_age = current_time - entry.window_start
            
            # Remove if window is very old (2x window size)
            if window_age >= self.window_seconds * 2:
                expired.append(client_id)
        
        for client_id in expired:
            del self._clients[client_id]
        
        return len(expired)
    
    def force_cleanup(self) -> int:
        """
        Force cleanup of expired entries.
        
        Returns:
            Number of entries removed
        """
        with self._lock:
            return self._cleanup(time.time())
    
    def get_stats(self) -> Dict[str, Any]:
        """Get rate limiter statistics."""
        with self._lock:
            current_time = time.time()
            
            active_clients = 0
            limited_clients = 0
            
            for entry in self._clients.values():
                window_age = current_time - entry.window_start
                
                if window_age < self.window_seconds:
                    active_clients += 1
                    if entry.request_count >= self.max_requests:
                        limited_clients += 1
            
            return {
                "tracked_clients": len(self._clients),
                "active_clients": active_clients,
                "limited_clients": limited_clients,
                "max_requests": self.max_requests,
                "window_seconds": self.window_seconds
            }
    
    def get_client_info(self, client_id: str) -> Optional[Dict[str, Any]]:
        """
        Get detailed info for a client.
        
        Args:
            client_id: Client identifier
            
        Returns:
            Client info dict or None
        """
        with self._lock:
            entry = self._clients.get(client_id)
            
            if entry is None:
                return None
            
            current_time = time.time()
            window_age = current_time - entry.window_start
            
            return {
                "client_id": entry.client_id,
                "request_count": entry.request_count,
                "remaining": max(0, self.max_requests - entry.request_count),
                "window_start": entry.window_start,
                "window_age": window_age,
                "reset_in": max(0, self.window_seconds - window_age),
                "is_limited": entry.request_count >= self.max_requests
            }


class AdaptiveRateLimiter(RateLimiter):
    """
    Adaptive rate limiter that adjusts limits based on behavior.
    
    Features:
    - Increases limits for well-behaved clients
    - Decreases limits for abusive clients
    - Automatic recovery over time
    """
    
    def __init__(
        self,
        base_requests: int = 100,
        window_seconds: int = 60,
        min_requests: int = 10,
        max_requests: int = 1000,
        adjustment_factor: float = 0.1
    ):
        """
        Initialize adaptive rate limiter.
        
        Args:
            base_requests: Base request limit
            window_seconds: Window duration
            min_requests: Minimum allowed limit
            max_requests: Maximum allowed limit
            adjustment_factor: Rate of adjustment
        """
        super().__init__(base_requests, window_seconds)
        self.base_requests = base_requests
        self.min_requests = min_requests
        self._max_requests = max_requests
        self.adjustment_factor = adjustment_factor
        
        # Per-client limits
        self._client_limits: Dict[str, int] = {}
        
        # Violation counts
        self._violations: Dict[str, int] = {}
    
    def allow_request(self, client_id: str) -> bool:
        """Check if request is allowed with adaptive limits."""
        # Use client-specific limit if available
        self.max_requests = self._client_limits.get(
            client_id, self.base_requests
        )
        
        allowed = super().allow_request(client_id)
        
        if not allowed:
            # Track violation
            self._violations[client_id] = self._violations.get(client_id, 0) + 1
            
            # Decrease limit for repeat offenders
            if self._violations[client_id] >= 3:
                current = self._client_limits.get(client_id, self.base_requests)
                new_limit = max(
                    self.min_requests,
                    int(current * (1 - self.adjustment_factor))
                )
                self._client_limits[client_id] = new_limit
        else:
            # Gradually increase limit for good behavior
            if client_id in self._client_limits:
                current = self._client_limits[client_id]
                if current < self.base_requests:
                    new_limit = min(
                        self.base_requests,
                        int(current * (1 + self.adjustment_factor * 0.5))
                    )
                    self._client_limits[client_id] = new_limit
        
        return allowed
    
    def get_client_limit(self, client_id: str) -> int:
        """Get the current limit for a client."""
        return self._client_limits.get(client_id, self.base_requests)
