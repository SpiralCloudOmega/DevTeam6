"""
DevTeam6 Local AI - Smart Tokenizer

AST-based tokenization for deterministic code analysis.
No regex guessing - uses Python's ast module for accurate parsing.
"""

import ast
from typing import List, Dict, Any, Optional, Set
from dataclasses import dataclass, field
from enum import Enum
import re


class TokenCategory(Enum):
    """Categories for token classification."""
    API = "api"
    SECURITY = "security"
    DATA = "data"
    GENERAL = "general"
    CONTROL = "control"
    FUNCTION = "function"
    VARIABLE = "variable"
    IMPORT = "import"


@dataclass
class Token:
    """A meaningful token extracted from code."""
    
    value: str
    category: TokenCategory
    clarity_score: float  # 0.0 - 1.0
    line_number: int
    column: int
    context: str = ""
    metadata: Dict[str, Any] = field(default_factory=dict)


class SmartTokenizer:
    """
    AST-based tokenizer for extracting meaningful tokens from code.
    
    Uses Python's ast module for accurate parsing instead of regex guessing.
    """
    
    # Keywords that indicate API-related code
    API_INDICATORS: Set[str] = {
        "request", "response", "api", "endpoint", "fetch", "get", "post",
        "put", "delete", "patch", "http", "https", "url", "uri", "rest",
        "graphql", "websocket", "socket", "client", "server"
    }
    
    # Keywords that indicate security-related code
    SECURITY_INDICATORS: Set[str] = {
        "auth", "token", "key", "secret", "password", "credential", "login",
        "logout", "session", "jwt", "oauth", "encrypt", "decrypt", "hash",
        "salt", "certificate", "ssl", "tls", "permission", "role", "admin"
    }
    
    # Keywords that indicate data operations
    DATA_INDICATORS: Set[str] = {
        "data", "database", "db", "query", "insert", "update", "delete",
        "select", "table", "column", "row", "record", "model", "schema",
        "json", "xml", "csv", "file", "read", "write", "parse"
    }
    
    def __init__(self):
        """Initialize the smart tokenizer."""
        self._cache: Dict[str, List[Token]] = {}
    
    def tokenize(self, code: str, language: str = "python") -> List[Token]:
        """
        Tokenize code using AST analysis.
        
        Args:
            code: Source code to tokenize
            language: Programming language (currently supports python)
            
        Returns:
            List of Token objects
        """
        cache_key = hash(code)
        if cache_key in self._cache:
            return self._cache[cache_key]
        
        if language == "python":
            tokens = self._tokenize_python(code)
        else:
            # Fallback to simple tokenization for other languages
            tokens = self._tokenize_simple(code)
        
        self._cache[cache_key] = tokens
        return tokens
    
    def _tokenize_python(self, code: str) -> List[Token]:
        """Tokenize Python code using AST."""
        tokens: List[Token] = []
        
        try:
            tree = ast.parse(code)
            tokens.extend(self._extract_from_ast(tree, code))
        except SyntaxError:
            # Fallback to simple tokenization if AST fails
            tokens = self._tokenize_simple(code)
        
        return tokens
    
    def _extract_from_ast(self, tree: ast.AST, source: str) -> List[Token]:
        """Extract tokens from AST nodes."""
        tokens: List[Token] = []
        
        for node in ast.walk(tree):
            token = self._node_to_token(node, source)
            if token:
                tokens.append(token)
        
        return tokens
    
    def _node_to_token(self, node: ast.AST, source: str) -> Optional[Token]:
        """Convert an AST node to a Token."""
        
        # Function definitions
        if isinstance(node, ast.FunctionDef):
            category = self._categorize_name(node.name)
            return Token(
                value=node.name,
                category=category,
                clarity_score=self._calculate_clarity(node.name),
                line_number=node.lineno,
                column=node.col_offset,
                context="function_definition",
                metadata={"args": [arg.arg for arg in node.args.args]}
            )
        
        # Class definitions
        elif isinstance(node, ast.ClassDef):
            category = self._categorize_name(node.name)
            return Token(
                value=node.name,
                category=category,
                clarity_score=self._calculate_clarity(node.name),
                line_number=node.lineno,
                column=node.col_offset,
                context="class_definition",
                metadata={"bases": [self._get_name(b) for b in node.bases]}
            )
        
        # Imports
        elif isinstance(node, ast.Import):
            names = [alias.name for alias in node.names]
            return Token(
                value=", ".join(names),
                category=TokenCategory.IMPORT,
                clarity_score=1.0,
                line_number=node.lineno,
                column=node.col_offset,
                context="import",
                metadata={"modules": names}
            )
        
        # From imports
        elif isinstance(node, ast.ImportFrom):
            module = node.module or ""
            names = [alias.name for alias in node.names]
            return Token(
                value=f"{module}.{', '.join(names)}",
                category=TokenCategory.IMPORT,
                clarity_score=1.0,
                line_number=node.lineno,
                column=node.col_offset,
                context="from_import",
                metadata={"module": module, "names": names}
            )
        
        # Function calls
        elif isinstance(node, ast.Call):
            func_name = self._get_call_name(node)
            if func_name:
                category = self._categorize_name(func_name)
                return Token(
                    value=func_name,
                    category=category,
                    clarity_score=self._calculate_clarity(func_name),
                    line_number=node.lineno if hasattr(node, 'lineno') else 0,
                    column=node.col_offset if hasattr(node, 'col_offset') else 0,
                    context="function_call",
                    metadata={"arg_count": len(node.args)}
                )
        
        # Variable assignments
        elif isinstance(node, ast.Assign):
            for target in node.targets:
                name = self._get_name(target)
                if name:
                    category = self._categorize_name(name)
                    return Token(
                        value=name,
                        category=category,
                        clarity_score=self._calculate_clarity(name),
                        line_number=node.lineno,
                        column=node.col_offset,
                        context="assignment",
                        metadata={}
                    )
        
        return None
    
    def _get_name(self, node: ast.AST) -> str:
        """Get the name from various node types."""
        if isinstance(node, ast.Name):
            return node.id
        elif isinstance(node, ast.Attribute):
            return f"{self._get_name(node.value)}.{node.attr}"
        elif isinstance(node, ast.Constant):
            return str(node.value)
        return ""
    
    def _get_call_name(self, node: ast.Call) -> str:
        """Get the function name from a Call node."""
        if isinstance(node.func, ast.Name):
            return node.func.id
        elif isinstance(node.func, ast.Attribute):
            return node.func.attr
        return ""
    
    def _categorize_name(self, name: str) -> TokenCategory:
        """Categorize a name based on indicators."""
        name_lower = name.lower()
        
        # Check for API indicators
        for indicator in self.API_INDICATORS:
            if indicator in name_lower:
                return TokenCategory.API
        
        # Check for security indicators
        for indicator in self.SECURITY_INDICATORS:
            if indicator in name_lower:
                return TokenCategory.SECURITY
        
        # Check for data indicators
        for indicator in self.DATA_INDICATORS:
            if indicator in name_lower:
                return TokenCategory.DATA
        
        return TokenCategory.GENERAL
    
    def _calculate_clarity(self, name: str) -> float:
        """
        Calculate clarity score for a name.
        
        Higher scores for:
        - Descriptive names (longer, meaningful)
        - CamelCase or snake_case
        - No abbreviations
        """
        if not name:
            return 0.0
        
        score = 0.5  # Base score
        
        # Length bonus (up to 0.2)
        length_score = min(len(name) / 20, 0.2)
        score += length_score
        
        # Naming convention bonus (up to 0.2)
        if "_" in name or (name[0].islower() and any(c.isupper() for c in name)):
            score += 0.2
        
        # No single letters (penalty)
        if len(name) == 1:
            score -= 0.3
        
        # Descriptive words bonus (up to 0.1)
        words = re.findall(r'[A-Z][a-z]+|[a-z]+', name)
        if len(words) >= 2:
            score += 0.1
        
        return min(max(score, 0.0), 1.0)
    
    def _tokenize_simple(self, code: str) -> List[Token]:
        """Simple tokenization fallback for non-Python languages."""
        tokens: List[Token] = []
        
        # Split by whitespace and common delimiters
        words = re.findall(r'\b[a-zA-Z_][a-zA-Z0-9_]*\b', code)
        
        for i, word in enumerate(words):
            category = self._categorize_name(word)
            tokens.append(Token(
                value=word,
                category=category,
                clarity_score=self._calculate_clarity(word),
                line_number=0,
                column=0,
                context="simple_token",
                metadata={"index": i}
            ))
        
        return tokens
    
    def get_category_summary(self, tokens: List[Token]) -> Dict[TokenCategory, int]:
        """Get a summary of token categories."""
        summary: Dict[TokenCategory, int] = {}
        for token in tokens:
            summary[token.category] = summary.get(token.category, 0) + 1
        return summary
    
    def filter_by_category(
        self,
        tokens: List[Token],
        category: TokenCategory
    ) -> List[Token]:
        """Filter tokens by category."""
        return [t for t in tokens if t.category == category]
    
    def get_high_clarity_tokens(
        self,
        tokens: List[Token],
        threshold: float = 0.7
    ) -> List[Token]:
        """Get tokens with clarity score above threshold."""
        return [t for t in tokens if t.clarity_score >= threshold]
