"""
DevTeam6 Local AI - Authentication Manager

Handles token validation, caching, and permission extraction.
"""

from typing import Dict, Any, Optional, List, Set
from dataclasses import dataclass, field
from datetime import datetime, timedelta
from enum import Enum
import hashlib
import secrets
import time


class SecurityLevel(Enum):
    """Security levels for access control."""
    PUBLIC = 0
    AUTHENTICATED = 1
    ELEVATED = 2
    ADMIN = 3
    SYSTEM = 4


@dataclass
class AuthToken:
    """Authentication token with metadata."""
    
    token_id: str
    user_id: str
    permissions: Set[str]
    security_level: SecurityLevel
    created_at: datetime
    expires_at: datetime
    metadata: Dict[str, Any] = field(default_factory=dict)
    
    @property
    def is_expired(self) -> bool:
        """Check if token is expired."""
        return datetime.utcnow() > self.expires_at
    
    @property
    def is_valid(self) -> bool:
        """Check if token is valid (not expired)."""
        return not self.is_expired


@dataclass
class AuthResult:
    """Result of authentication attempt."""
    
    success: bool
    token: Optional[AuthToken] = None
    error: Optional[str] = None
    security_level: SecurityLevel = SecurityLevel.PUBLIC


class AuthManager:
    """
    Authentication manager for handling tokens and permissions.
    
    Features:
    - Token validation and caching
    - Permission extraction
    - Security level assessment
    - Session management
    """
    
    def __init__(
        self,
        token_ttl_hours: int = 24,
        cache_size: int = 1000
    ):
        """
        Initialize auth manager.
        
        Args:
            token_ttl_hours: Default token time-to-live in hours
            cache_size: Maximum number of cached tokens
        """
        self.token_ttl_hours = token_ttl_hours
        self.cache_size = cache_size
        
        # Token cache: token_id -> AuthToken
        self._token_cache: Dict[str, AuthToken] = {}
        
        # User sessions: user_id -> List[token_id]
        self._user_sessions: Dict[str, List[str]] = {}
        
        # Revoked tokens
        self._revoked_tokens: Set[str] = set()
        
        # Permission definitions
        self._permission_hierarchy: Dict[SecurityLevel, Set[str]] = {
            SecurityLevel.PUBLIC: {"read:public"},
            SecurityLevel.AUTHENTICATED: {
                "read:public", "read:user", "write:user"
            },
            SecurityLevel.ELEVATED: {
                "read:public", "read:user", "write:user",
                "read:elevated", "write:elevated"
            },
            SecurityLevel.ADMIN: {
                "read:public", "read:user", "write:user",
                "read:elevated", "write:elevated",
                "read:admin", "write:admin", "manage:users"
            },
            SecurityLevel.SYSTEM: {
                "read:public", "read:user", "write:user",
                "read:elevated", "write:elevated",
                "read:admin", "write:admin", "manage:users",
                "system:all"
            },
        }
    
    def generate_token(
        self,
        user_id: str,
        permissions: Optional[Set[str]] = None,
        security_level: SecurityLevel = SecurityLevel.AUTHENTICATED,
        ttl_hours: Optional[int] = None,
        metadata: Optional[Dict[str, Any]] = None
    ) -> AuthToken:
        """
        Generate a new authentication token.
        
        Args:
            user_id: User identifier
            permissions: Explicit permissions (optional)
            security_level: Security level for the token
            ttl_hours: Token TTL override
            metadata: Additional token metadata
            
        Returns:
            New AuthToken
        """
        # Generate secure token ID
        token_id = secrets.token_urlsafe(32)
        
        # Get permissions from hierarchy or use explicit
        if permissions is None:
            permissions = self._permission_hierarchy.get(
                security_level, set()
            ).copy()
        
        now = datetime.utcnow()
        ttl = ttl_hours or self.token_ttl_hours
        
        token = AuthToken(
            token_id=token_id,
            user_id=user_id,
            permissions=permissions,
            security_level=security_level,
            created_at=now,
            expires_at=now + timedelta(hours=ttl),
            metadata=metadata or {}
        )
        
        # Cache token
        self._cache_token(token)
        
        # Track user session
        if user_id not in self._user_sessions:
            self._user_sessions[user_id] = []
        self._user_sessions[user_id].append(token_id)
        
        return token
    
    def validate_token(self, token_id: str) -> AuthResult:
        """
        Validate a token.
        
        Args:
            token_id: Token to validate
            
        Returns:
            AuthResult with validation status
        """
        # Check if revoked
        if token_id in self._revoked_tokens:
            return AuthResult(
                success=False,
                error="Token has been revoked"
            )
        
        # Check cache
        token = self._token_cache.get(token_id)
        
        if token is None:
            return AuthResult(
                success=False,
                error="Token not found"
            )
        
        if token.is_expired:
            # Remove expired token
            self._remove_token(token_id)
            return AuthResult(
                success=False,
                error="Token has expired"
            )
        
        return AuthResult(
            success=True,
            token=token,
            security_level=token.security_level
        )
    
    def check_permission(
        self,
        token_id: str,
        required_permission: str
    ) -> bool:
        """
        Check if token has a specific permission.
        
        Args:
            token_id: Token to check
            required_permission: Permission to verify
            
        Returns:
            True if permission is granted
        """
        result = self.validate_token(token_id)
        
        if not result.success or result.token is None:
            return False
        
        return required_permission in result.token.permissions
    
    def check_security_level(
        self,
        token_id: str,
        required_level: SecurityLevel
    ) -> bool:
        """
        Check if token meets minimum security level.
        
        Args:
            token_id: Token to check
            required_level: Minimum required level
            
        Returns:
            True if level is sufficient
        """
        result = self.validate_token(token_id)
        
        if not result.success or result.token is None:
            return False
        
        return result.token.security_level.value >= required_level.value
    
    def extract_permissions(self, token_id: str) -> Set[str]:
        """
        Extract all permissions from a token.
        
        Args:
            token_id: Token to extract from
            
        Returns:
            Set of permission strings
        """
        result = self.validate_token(token_id)
        
        if not result.success or result.token is None:
            return set()
        
        return result.token.permissions.copy()
    
    def revoke_token(self, token_id: str) -> bool:
        """
        Revoke a token.
        
        Args:
            token_id: Token to revoke
            
        Returns:
            True if token was revoked
        """
        if token_id in self._token_cache:
            self._revoked_tokens.add(token_id)
            self._remove_token(token_id)
            return True
        return False
    
    def revoke_user_tokens(self, user_id: str) -> int:
        """
        Revoke all tokens for a user.
        
        Args:
            user_id: User whose tokens to revoke
            
        Returns:
            Number of tokens revoked
        """
        token_ids = self._user_sessions.get(user_id, [])
        count = 0
        
        for token_id in token_ids:
            if self.revoke_token(token_id):
                count += 1
        
        self._user_sessions[user_id] = []
        return count
    
    def get_security_level(self, token_id: str) -> SecurityLevel:
        """
        Get the security level of a token.
        
        Args:
            token_id: Token to check
            
        Returns:
            Security level (PUBLIC if invalid)
        """
        result = self.validate_token(token_id)
        return result.security_level
    
    def assess_request_security(
        self,
        token_id: Optional[str],
        required_permissions: Set[str],
        required_level: SecurityLevel = SecurityLevel.AUTHENTICATED
    ) -> Dict[str, Any]:
        """
        Assess security for a request.
        
        Args:
            token_id: Request token (optional)
            required_permissions: Required permissions
            required_level: Required security level
            
        Returns:
            Security assessment dict
        """
        if token_id is None:
            return {
                "allowed": False,
                "reason": "No authentication token provided",
                "current_level": SecurityLevel.PUBLIC.name,
                "required_level": required_level.name,
                "missing_permissions": list(required_permissions)
            }
        
        result = self.validate_token(token_id)
        
        if not result.success:
            return {
                "allowed": False,
                "reason": result.error,
                "current_level": SecurityLevel.PUBLIC.name,
                "required_level": required_level.name,
                "missing_permissions": list(required_permissions)
            }
        
        token = result.token
        
        # Check security level
        if token.security_level.value < required_level.value:
            return {
                "allowed": False,
                "reason": "Insufficient security level",
                "current_level": token.security_level.name,
                "required_level": required_level.name,
                "missing_permissions": []
            }
        
        # Check permissions
        missing = required_permissions - token.permissions
        if missing:
            return {
                "allowed": False,
                "reason": "Missing required permissions",
                "current_level": token.security_level.name,
                "required_level": required_level.name,
                "missing_permissions": list(missing)
            }
        
        return {
            "allowed": True,
            "reason": "Access granted",
            "current_level": token.security_level.name,
            "required_level": required_level.name,
            "missing_permissions": []
        }
    
    def _cache_token(self, token: AuthToken) -> None:
        """Add token to cache, evicting old tokens if needed."""
        # Evict if cache is full
        if len(self._token_cache) >= self.cache_size:
            self._evict_expired_tokens()
        
        self._token_cache[token.token_id] = token
    
    def _remove_token(self, token_id: str) -> None:
        """Remove token from cache."""
        self._token_cache.pop(token_id, None)
    
    def _evict_expired_tokens(self) -> None:
        """Remove expired tokens from cache."""
        now = datetime.utcnow()
        expired = [
            tid for tid, token in self._token_cache.items()
            if token.expires_at < now
        ]
        for tid in expired:
            self._remove_token(tid)
    
    def get_stats(self) -> Dict[str, Any]:
        """Get auth manager statistics."""
        return {
            "cached_tokens": len(self._token_cache),
            "active_users": len(self._user_sessions),
            "revoked_tokens": len(self._revoked_tokens),
            "cache_limit": self.cache_size
        }
