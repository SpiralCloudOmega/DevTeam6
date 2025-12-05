"""
DevTeam6 QLoRA Adapter

Quantized LoRA for memory-efficient fine-tuning.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any
import math
import random

from .lora_adapter import LoRAAdapter, LoRAConfig


@dataclass
class QLoRAConfig(LoRAConfig):
    """Configuration for QLoRA adapter."""
    bits: int = 4  # Quantization bits
    double_quant: bool = True
    quant_type: str = "nf4"  # nf4 or fp4


class QLoRAAdapter(LoRAAdapter):
    """
    Quantized Low-Rank Adaptation (QLoRA) adapter.
    
    Extends LoRA with 4-bit quantization for memory-efficient
    fine-tuning of large language models.
    """
    
    def __init__(self, config: Optional[QLoRAConfig] = None):
        super().__init__(config or QLoRAConfig())
        self.qconfig = config or QLoRAConfig()
        
        # Quantization lookup tables
        self.quant_tables: Dict[str, List[float]] = {}
        self._init_quant_tables()
    
    def _init_quant_tables(self) -> None:
        """Initialize quantization lookup tables."""
        bits = self.qconfig.bits
        num_levels = 2 ** bits
        
        if self.qconfig.quant_type == "nf4":
            # Normal Float 4-bit quantization
            # Use normal distribution quantiles
            self.quant_tables["nf4"] = self._generate_nf4_table(num_levels)
        else:
            # Standard floating point quantization
            self.quant_tables["fp4"] = self._generate_fp4_table(num_levels)
    
    def _generate_nf4_table(self, num_levels: int) -> List[float]:
        """Generate NF4 quantization table."""
        # NF4 uses quantiles of normal distribution
        table = []
        for i in range(num_levels):
            # Map to quantile of standard normal
            p = (i + 0.5) / num_levels
            # Approximate inverse normal CDF
            z = self._approx_inv_norm(p)
            table.append(z)
        return table
    
    def _generate_fp4_table(self, num_levels: int) -> List[float]:
        """Generate FP4 quantization table."""
        # Linear spacing
        table = []
        for i in range(num_levels):
            val = -1 + 2 * i / (num_levels - 1)
            table.append(val)
        return table
    
    def _approx_inv_norm(self, p: float) -> float:
        """Approximate inverse normal CDF."""
        # Ensure p is in valid range
        p = max(0.001, min(0.999, p))
        
        # Simple approximation
        a = 8 * (math.pi - 3) / (3 * math.pi * (4 - math.pi))
        x = 2 * p - 1
        
        ln_term = math.log(1 - x * x)
        term1 = 2 / (math.pi * a) + ln_term / 2
        term2 = ln_term / a
        
        result = math.copysign(1, x) * math.sqrt(math.sqrt(term1 * term1 - term2) - term1)
        return result
    
    def quantize(self, value: float) -> int:
        """Quantize a value to integer."""
        table = self.quant_tables.get(self.qconfig.quant_type, self.quant_tables.get("fp4", []))
        
        if not table:
            return 0
        
        # Find closest value in table
        min_dist = float('inf')
        best_idx = 0
        for i, t in enumerate(table):
            dist = abs(value - t)
            if dist < min_dist:
                min_dist = dist
                best_idx = i
        
        return best_idx
    
    def dequantize(self, index: int) -> float:
        """Dequantize an integer to float."""
        table = self.quant_tables.get(self.qconfig.quant_type, self.quant_tables.get("fp4", []))
        
        if not table or index >= len(table):
            return 0.0
        
        return table[index]
    
    def quantize_matrix(self, matrix: List[List[float]]) -> Dict[str, Any]:
        """Quantize a weight matrix."""
        # Calculate scale and zero point
        flat = [v for row in matrix for v in row]
        if not flat:
            return {"quantized": [], "scale": 1.0, "zero_point": 0.0}
        
        min_val = min(flat)
        max_val = max(flat)
        
        # Scale to quantization range
        scale = (max_val - min_val) / (2 ** self.qconfig.bits - 1) if max_val != min_val else 1.0
        zero_point = min_val
        
        # Quantize each value
        quantized = []
        for row in matrix:
            q_row = []
            for val in row:
                normalized = (val - zero_point) / scale if scale != 0 else 0
                q_val = self.quantize(normalized / (2 ** (self.qconfig.bits - 1)))
                q_row.append(q_val)
            quantized.append(q_row)
        
        return {
            "quantized": quantized,
            "scale": scale,
            "zero_point": zero_point
        }
    
    def dequantize_matrix(self, quantized_data: Dict[str, Any]) -> List[List[float]]:
        """Dequantize a weight matrix."""
        quantized = quantized_data["quantized"]
        scale = quantized_data["scale"]
        zero_point = quantized_data["zero_point"]
        
        dequantized = []
        for row in quantized:
            d_row = []
            for q_val in row:
                normalized = self.dequantize(q_val) * (2 ** (self.qconfig.bits - 1))
                val = normalized * scale + zero_point
                d_row.append(val)
            dequantized.append(d_row)
        
        return dequantized
    
    def get_memory_usage(self) -> Dict[str, int]:
        """Get memory usage statistics."""
        bits = self.qconfig.bits
        full_precision_bits = 32  # float32
        
        total_params = 0
        for adapter in self.adapters.values():
            a_params = len(adapter["A"]) * len(adapter["A"][0]) if adapter["A"] else 0
            b_params = len(adapter["B"]) * len(adapter["B"][0]) if adapter["B"] else 0
            total_params += a_params + b_params
        
        full_memory = total_params * full_precision_bits // 8
        quantized_memory = total_params * bits // 8
        
        return {
            "total_parameters": total_params,
            "full_precision_bytes": full_memory,
            "quantized_bytes": quantized_memory,
            "compression_ratio": full_precision_bits / bits,
            "memory_saved_bytes": full_memory - quantized_memory
        }
    
    def get_stats(self) -> Dict[str, Any]:
        """Get adapter statistics."""
        base_stats = super().get_stats()
        memory_stats = self.get_memory_usage()
        
        return {
            **base_stats,
            "quantization": {
                "bits": self.qconfig.bits,
                "type": self.qconfig.quant_type,
                "double_quant": self.qconfig.double_quant
            },
            "memory": memory_stats
        }
