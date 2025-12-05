# 🐍 Python Specialist Rules

> **@python agent guidelines for Python/ML/Data Science development**

---

## 🎯 Expertise Areas

- Python 3.10+
- Machine Learning (PyTorch, TensorFlow, scikit-learn)
- Data Science (Pandas, NumPy, Polars)
- API Development (FastAPI, Flask)
- LLM Integration (LangChain, LlamaIndex)
- Vector Databases (Pinecone, Chroma, Weaviate)
- Testing (pytest, hypothesis)

---

## 📋 Code Standards

### Module Structure
```python
"""
Module docstring describing purpose.

Example:
    >>> from module import function
    >>> function()
"""

# Standard library imports
import os
from pathlib import Path
from typing import Optional, List, Dict

# Third-party imports
import numpy as np
import pandas as pd

# Local imports
from .utils import helper_function

# Constants
MAX_RETRIES = 3
DEFAULT_TIMEOUT = 30

# Module-level code
logger = logging.getLogger(__name__)


def public_function(param: str) -> dict:
    """
    Brief description.
    
    Args:
        param: Description of param.
        
    Returns:
        Description of return value.
        
    Raises:
        ValueError: When param is invalid.
    """
    pass


class PublicClass:
    """Class docstring."""
    
    def __init__(self, value: int) -> None:
        self.value = value
```

### Naming Conventions
| Type | Convention | Example |
|------|------------|---------|
| Modules | snake_case | `my_module.py` |
| Classes | PascalCase | `MyClass` |
| Functions | snake_case | `my_function` |
| Constants | SCREAMING_SNAKE_CASE | `MAX_VALUE` |
| Private | Leading underscore | `_private_func` |

### Type Hints
```python
from typing import Optional, List, Dict, Union, Callable, TypeVar

T = TypeVar('T')

def process_items(
    items: List[str],
    callback: Optional[Callable[[str], None]] = None
) -> Dict[str, int]:
    """Process items with optional callback."""
    pass
```

---

## 🔧 Best Practices

### Error Handling
```python
class CustomError(Exception):
    """Custom exception for specific errors."""
    pass

def safe_operation(value: str) -> Optional[int]:
    """Safely convert string to int."""
    try:
        return int(value)
    except ValueError as e:
        logger.warning(f"Failed to convert {value}: {e}")
        return None
```

### Context Managers
```python
from contextlib import contextmanager

@contextmanager
def managed_resource():
    """Context manager for resource handling."""
    resource = acquire_resource()
    try:
        yield resource
    finally:
        release_resource(resource)
```

### Async Patterns
```python
import asyncio
from typing import List

async def fetch_all(urls: List[str]) -> List[dict]:
    """Fetch multiple URLs concurrently."""
    async with aiohttp.ClientSession() as session:
        tasks = [fetch_one(session, url) for url in urls]
        return await asyncio.gather(*tasks)
```

---

## 🤖 ML/AI Guidelines

### Model Training Pattern
```python
from dataclasses import dataclass
from pathlib import Path

@dataclass
class TrainingConfig:
    epochs: int = 100
    batch_size: int = 32
    learning_rate: float = 1e-4
    checkpoint_dir: Path = Path("checkpoints")

def train_model(config: TrainingConfig) -> dict:
    """Train model with given configuration."""
    # Implementation
    pass
```

### Vector Database Integration
```python
from typing import List
import numpy as np

class VectorStore:
    """Abstract vector store interface."""
    
    def add(self, texts: List[str], embeddings: np.ndarray) -> None:
        """Add vectors to store."""
        raise NotImplementedError
    
    def query(self, embedding: np.ndarray, k: int = 5) -> List[dict]:
        """Query similar vectors."""
        raise NotImplementedError
```

---

## 📊 Data Processing

### Pandas Best Practices
```python
import pandas as pd

# Use method chaining
df = (
    pd.read_csv("data.csv")
    .pipe(clean_data)
    .assign(new_col=lambda x: x['a'] + x['b'])
    .query("value > 0")
    .groupby("category")
    .agg({"value": ["mean", "sum"]})
)
```

### NumPy Optimization
```python
import numpy as np

# Vectorized operations (fast)
result = np.sum(array * weights, axis=1)

# Avoid loops (slow)
# for i in range(len(array)):
#     result[i] = sum(array[i] * weights)
```

---

## 🧪 Testing

### Pytest Patterns
```python
import pytest

@pytest.fixture
def sample_data():
    """Fixture providing sample data."""
    return {"key": "value"}

def test_function(sample_data):
    """Test with fixture."""
    result = process(sample_data)
    assert result is not None

@pytest.mark.parametrize("input,expected", [
    ("a", 1),
    ("b", 2),
])
def test_parametrized(input, expected):
    """Parametrized test."""
    assert convert(input) == expected
```

---

## 🚫 Anti-Patterns to Avoid

1. **Don't** use mutable default arguments
2. **Don't** catch bare exceptions
3. **Don't** use global variables
4. **Don't** ignore type hints
5. **Don't** write untested code

---

*Version: 1.0.0*
*Last Updated: 2025-12-05*
