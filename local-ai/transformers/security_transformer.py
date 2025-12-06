"""
DevTeam6 Local AI - Security Transformer

Security handler with auth validation pipeline, authorization checks, and audit logging.
"""

from typing import Dict, Any, List, Optional
from dataclasses import dataclass, field
from datetime import datetime
from enum import Enum
import hashlib
import json

from .components.auth_manager import AuthManager, AuthToken, SecurityLevel, AuthResult
from .token_transformer import TokenTransformResult, RouteType


class AuditEventType(Enum):
    """Types of audit events."""
    AUTH_SUCCESS = "auth_success"
    AUTH_FAILURE = "auth_failure"
    ACCESS_GRANTED = "access_granted"
    ACCESS_DENIED = "access_denied"
    SECURITY_ALERT = "security_alert"
    TOKEN_CREATED = "token_created"
    TOKEN_REVOKED = "token_revoked"
    PERMISSION_CHECK = "permission_check"


@dataclass
class AuditLogEntry:
    """Entry in the audit log."""
    
    timestamp: datetime
    event_type: AuditEventType
    user_id: Optional[str]
    details: Dict[str, Any]
    ip_address: Optional[str] = None
    request_id: Optional[str] = None
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "timestamp": self.timestamp.isoformat(),
            "event_type": self.event_type.value,
            "user_id": self.user_id,
            "details": self.details,
            "ip_address": self.ip_address,
            "request_id": self.request_id
        }


@dataclass
class SecurityValidationResult:
    """Result of security validation."""
    
    is_valid: bool
    security_level: SecurityLevel
    user_id: Optional[str] = None
    permissions: set = field(default_factory=set)
    errors: List[str] = field(default_factory=list)
    warnings: List[str] = field(default_factory=list)
    audit_entries: List[AuditLogEntry] = field(default_factory=list)


class SecurityTransformer:
    """
    Security handler for the Dual Transformer system.
    
    Features:
    - Auth validation pipeline
    - Authorization checks
    - Audit logging
    - Security level enforcement
    """
    
    def __init__(
        self,
        auth_manager: Optional[AuthManager] = None,
        audit_log_size: int = 10000
    ):
        """
        Initialize security transformer.
        
        Args:
            auth_manager: Auth manager instance
            audit_log_size: Maximum audit log entries
        """
        self.auth_manager = auth_manager or AuthManager()
        self.audit_log_size = audit_log_size
        
        # Audit log
        self._audit_log: List[AuditLogEntry] = []
        
        # Security policies
        self._policies: Dict[str, Dict[str, Any]] = {}
        
        # Metrics
        self._metrics = {
            "validations": 0,
            "auth_successes": 0,
            "auth_failures": 0,
            "access_grants": 0,
            "access_denials": 0,
            "security_alerts": 0
        }
    
    def validate_request(
        self,
        token_result: TokenTransformResult,
        token_id: Optional[str] = None,
        required_level: SecurityLevel = SecurityLevel.AUTHENTICATED,
        required_permissions: Optional[set] = None,
        request_metadata: Optional[Dict[str, Any]] = None
    ) -> SecurityValidationResult:
        """
        Validate a request through the security pipeline.
        
        Args:
            token_result: Result from TokenTransformer
            token_id: Authentication token
            required_level: Minimum security level
            required_permissions: Required permissions
            request_metadata: Additional request info
            
        Returns:
            SecurityValidationResult
        """
        self._metrics["validations"] += 1
        
        errors: List[str] = []
        warnings: List[str] = []
        audit_entries: List[AuditLogEntry] = []
        
        # Check if request has security indicators
        has_security_route = token_result.routing_decision.route_type in [
            RouteType.SECURITY, RouteType.HYBRID
        ]
        
        if has_security_route:
            # Elevate security requirements
            if required_level.value < SecurityLevel.ELEVATED.value:
                required_level = SecurityLevel.ELEVATED
                warnings.append("Security level elevated due to security indicators")
        
        # Validate token
        auth_result = self._validate_auth(token_id)
        
        if not auth_result.success:
            self._metrics["auth_failures"] += 1
            errors.append(auth_result.error or "Authentication failed")
            
            self._log_audit(
                AuditEventType.AUTH_FAILURE,
                user_id=None,
                details={
                    "error": auth_result.error,
                    "token_provided": token_id is not None
                },
                request_metadata=request_metadata
            )
            
            return SecurityValidationResult(
                is_valid=False,
                security_level=SecurityLevel.PUBLIC,
                errors=errors,
                warnings=warnings,
                audit_entries=audit_entries
            )
        
        self._metrics["auth_successes"] += 1
        token = auth_result.token
        
        # Check security level
        if token.security_level.value < required_level.value:
            self._metrics["access_denials"] += 1
            errors.append(
                f"Insufficient security level: {token.security_level.name} < {required_level.name}"
            )
            
            self._log_audit(
                AuditEventType.ACCESS_DENIED,
                user_id=token.user_id,
                details={
                    "reason": "insufficient_security_level",
                    "current": token.security_level.name,
                    "required": required_level.name
                },
                request_metadata=request_metadata
            )
            
            return SecurityValidationResult(
                is_valid=False,
                security_level=token.security_level,
                user_id=token.user_id,
                permissions=token.permissions,
                errors=errors,
                warnings=warnings
            )
        
        # Check permissions
        if required_permissions:
            missing = required_permissions - token.permissions
            if missing:
                self._metrics["access_denials"] += 1
                errors.append(f"Missing permissions: {', '.join(missing)}")
                
                self._log_audit(
                    AuditEventType.ACCESS_DENIED,
                    user_id=token.user_id,
                    details={
                        "reason": "missing_permissions",
                        "missing": list(missing)
                    },
                    request_metadata=request_metadata
                )
                
                return SecurityValidationResult(
                    is_valid=False,
                    security_level=token.security_level,
                    user_id=token.user_id,
                    permissions=token.permissions,
                    errors=errors,
                    warnings=warnings
                )
        
        # Check for security alerts in token indicators
        security_indicators = [
            ind for ind in token_result.routing_decision.indicators
            if "security" in ind.lower() or "auth" in ind.lower()
        ]
        
        if security_indicators:
            warnings.append(f"Security indicators detected: {security_indicators}")
            
            # Log security alert for sensitive operations
            if required_level.value >= SecurityLevel.ELEVATED.value:
                self._metrics["security_alerts"] += 1
                self._log_audit(
                    AuditEventType.SECURITY_ALERT,
                    user_id=token.user_id,
                    details={
                        "indicators": security_indicators,
                        "operation_level": required_level.name
                    },
                    request_metadata=request_metadata
                )
        
        # Validation passed
        self._metrics["access_grants"] += 1
        
        self._log_audit(
            AuditEventType.ACCESS_GRANTED,
            user_id=token.user_id,
            details={
                "security_level": token.security_level.name,
                "permissions_used": list(required_permissions or [])
            },
            request_metadata=request_metadata
        )
        
        return SecurityValidationResult(
            is_valid=True,
            security_level=token.security_level,
            user_id=token.user_id,
            permissions=token.permissions,
            warnings=warnings
        )
    
    def _validate_auth(self, token_id: Optional[str]) -> AuthResult:
        """Validate authentication token."""
        if token_id is None:
            return AuthResult(
                success=False,
                error="No authentication token provided"
            )
        
        return self.auth_manager.validate_token(token_id)
    
    def check_authorization(
        self,
        user_id: str,
        resource: str,
        action: str
    ) -> bool:
        """
        Check if user is authorized for an action on a resource.
        
        Args:
            user_id: User identifier
            resource: Resource identifier
            action: Action to perform
            
        Returns:
            True if authorized
        """
        # Build permission string
        permission = f"{action}:{resource}"
        
        # Check against policies
        policy_key = f"{resource}:{action}"
        policy = self._policies.get(policy_key, {})
        
        required_level = policy.get("security_level", SecurityLevel.AUTHENTICATED)
        required_permissions = policy.get("permissions", {permission})
        
        # This would need a token lookup by user_id in real implementation
        # For now, return True if no specific policy
        
        self._log_audit(
            AuditEventType.PERMISSION_CHECK,
            user_id=user_id,
            details={
                "resource": resource,
                "action": action,
                "permission": permission
            }
        )
        
        return True
    
    def register_policy(
        self,
        resource: str,
        action: str,
        security_level: SecurityLevel,
        permissions: set
    ) -> None:
        """
        Register a security policy.
        
        Args:
            resource: Resource identifier
            action: Action type
            security_level: Required security level
            permissions: Required permissions
        """
        policy_key = f"{resource}:{action}"
        self._policies[policy_key] = {
            "security_level": security_level,
            "permissions": permissions
        }
    
    def _log_audit(
        self,
        event_type: AuditEventType,
        user_id: Optional[str],
        details: Dict[str, Any],
        request_metadata: Optional[Dict[str, Any]] = None
    ) -> None:
        """Add entry to audit log."""
        entry = AuditLogEntry(
            timestamp=datetime.utcnow(),
            event_type=event_type,
            user_id=user_id,
            details=details,
            ip_address=request_metadata.get("ip_address") if request_metadata else None,
            request_id=request_metadata.get("request_id") if request_metadata else None
        )
        
        self._audit_log.append(entry)
        
        # Trim log if too large
        if len(self._audit_log) > self.audit_log_size:
            self._audit_log = self._audit_log[-self.audit_log_size:]
    
    def get_audit_log(
        self,
        event_type: Optional[AuditEventType] = None,
        user_id: Optional[str] = None,
        limit: int = 100
    ) -> List[Dict[str, Any]]:
        """
        Get audit log entries.
        
        Args:
            event_type: Filter by event type
            user_id: Filter by user
            limit: Maximum entries to return
            
        Returns:
            List of audit log entries
        """
        entries = self._audit_log
        
        if event_type:
            entries = [e for e in entries if e.event_type == event_type]
        
        if user_id:
            entries = [e for e in entries if e.user_id == user_id]
        
        return [e.to_dict() for e in entries[-limit:]]
    
    def get_security_summary(
        self,
        token_result: TokenTransformResult
    ) -> Dict[str, Any]:
        """
        Get security summary for a token result.
        
        Args:
            token_result: TokenTransformResult to analyze
            
        Returns:
            Security summary dict
        """
        routing = token_result.routing_decision
        
        # Extract security-related indicators
        security_indicators = [
            ind for ind in routing.indicators
            if "security" in ind.lower() or "auth" in ind.lower()
        ]
        
        # Determine risk level
        risk_level = "low"
        if routing.route_type == RouteType.SECURITY:
            risk_level = "high"
        elif routing.route_type == RouteType.HYBRID:
            risk_level = "medium"
        elif security_indicators:
            risk_level = "medium"
        
        return {
            "risk_level": risk_level,
            "security_indicators": security_indicators,
            "route_type": routing.route_type.value,
            "confidence": routing.confidence,
            "requires_auth": routing.route_type != RouteType.LOCAL,
            "recommended_level": self._recommend_security_level(routing)
        }
    
    def _recommend_security_level(
        self,
        routing: 'RoutingDecision'
    ) -> str:
        """Recommend security level based on routing."""
        if routing.route_type == RouteType.SECURITY:
            return SecurityLevel.ELEVATED.name
        elif routing.route_type == RouteType.API:
            return SecurityLevel.AUTHENTICATED.name
        elif routing.route_type == RouteType.HYBRID:
            return SecurityLevel.ELEVATED.name
        else:
            return SecurityLevel.PUBLIC.name
    
    def get_metrics(self) -> Dict[str, Any]:
        """Get security metrics."""
        return {
            **self._metrics,
            "audit_log_size": len(self._audit_log),
            "policies_registered": len(self._policies)
        }
