"""
DevTeam6 Adapter Manager

Manages multiple LoRA/QLoRA adapters.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any
from datetime import datetime
import json

from .lora_adapter import LoRAAdapter, LoRAConfig
from .qlora_adapter import QLoRAAdapter, QLoRAConfig


@dataclass
class AdapterInfo:
    """Information about a registered adapter."""
    name: str
    adapter_type: str
    created_at: str
    num_modules: int
    total_params: int
    is_active: bool = True


class AdapterManager:
    """
    Manages multiple LoRA/QLoRA adapters.
    
    Provides functionality to create, switch between, merge,
    and manage multiple fine-tuned adapters.
    """
    
    def __init__(self):
        self.adapters: Dict[str, LoRAAdapter] = {}
        self.adapter_info: Dict[str, AdapterInfo] = {}
        self.active_adapter: Optional[str] = None
    
    def create_lora(
        self,
        name: str,
        config: Optional[LoRAConfig] = None,
        modules: Optional[List[tuple]] = None
    ) -> LoRAAdapter:
        """
        Create a new LoRA adapter.
        
        Args:
            name: Adapter name
            config: LoRA configuration
            modules: List of (module_name, input_dim, output_dim) tuples
            
        Returns:
            Created adapter
        """
        adapter = LoRAAdapter(config)
        
        if modules:
            for module_name, input_dim, output_dim in modules:
                adapter.create_adapter(module_name, input_dim, output_dim)
        
        self.adapters[name] = adapter
        self.adapter_info[name] = AdapterInfo(
            name=name,
            adapter_type="lora",
            created_at=datetime.now().isoformat(),
            num_modules=len(modules) if modules else 0,
            total_params=adapter.get_stats()["total_parameters"]
        )
        
        return adapter
    
    def create_qlora(
        self,
        name: str,
        config: Optional[QLoRAConfig] = None,
        modules: Optional[List[tuple]] = None
    ) -> QLoRAAdapter:
        """
        Create a new QLoRA adapter.
        
        Args:
            name: Adapter name
            config: QLoRA configuration
            modules: List of (module_name, input_dim, output_dim) tuples
            
        Returns:
            Created adapter
        """
        adapter = QLoRAAdapter(config)
        
        if modules:
            for module_name, input_dim, output_dim in modules:
                adapter.create_adapter(module_name, input_dim, output_dim)
        
        self.adapters[name] = adapter
        self.adapter_info[name] = AdapterInfo(
            name=name,
            adapter_type="qlora",
            created_at=datetime.now().isoformat(),
            num_modules=len(modules) if modules else 0,
            total_params=adapter.get_stats()["total_parameters"]
        )
        
        return adapter
    
    def get_adapter(self, name: str) -> Optional[LoRAAdapter]:
        """Get adapter by name."""
        return self.adapters.get(name)
    
    def set_active(self, name: str) -> bool:
        """Set the active adapter."""
        if name not in self.adapters:
            return False
        
        # Deactivate previous
        if self.active_adapter and self.active_adapter in self.adapter_info:
            self.adapter_info[self.active_adapter].is_active = False
        
        # Activate new
        self.active_adapter = name
        self.adapter_info[name].is_active = True
        return True
    
    def get_active(self) -> Optional[LoRAAdapter]:
        """Get the active adapter."""
        if self.active_adapter:
            return self.adapters.get(self.active_adapter)
        return None
    
    def remove_adapter(self, name: str) -> bool:
        """Remove an adapter."""
        if name not in self.adapters:
            return False
        
        del self.adapters[name]
        del self.adapter_info[name]
        
        if self.active_adapter == name:
            self.active_adapter = None
        
        return True
    
    def list_adapters(self) -> List[AdapterInfo]:
        """List all adapters."""
        return list(self.adapter_info.values())
    
    def merge_adapters(
        self,
        name: str,
        adapter_names: List[str],
        weights: Optional[List[float]] = None
    ) -> Optional[LoRAAdapter]:
        """
        Merge multiple adapters into one.
        
        Args:
            name: Name for merged adapter
            adapter_names: Names of adapters to merge
            weights: Optional weights for each adapter
            
        Returns:
            Merged adapter or None if failed
        """
        if not adapter_names:
            return None
        
        adapters_to_merge = [
            self.adapters[n] for n in adapter_names
            if n in self.adapters
        ]
        
        if not adapters_to_merge:
            return None
        
        if weights is None:
            weights = [1.0 / len(adapters_to_merge)] * len(adapters_to_merge)
        
        # Create new adapter with same config as first
        first = adapters_to_merge[0]
        merged = LoRAAdapter(first.config)
        
        # Merge each module
        all_modules = set()
        for adapter in adapters_to_merge:
            all_modules.update(adapter.adapters.keys())
        
        for module_name in all_modules:
            # Get dimensions from first adapter with this module
            dims = None
            for adapter in adapters_to_merge:
                if module_name in adapter.adapters:
                    dims = (
                        adapter.adapters[module_name]["input_dim"],
                        adapter.adapters[module_name]["output_dim"]
                    )
                    break
            
            if dims:
                merged.create_adapter(module_name, dims[0], dims[1])
                
                # Average the weights
                for i, adapter in enumerate(adapters_to_merge):
                    if module_name in adapter.adapters:
                        w = weights[i]
                        src = adapter.adapters[module_name]
                        dst = merged.adapters[module_name]
                        
                        # Weighted average of A and B
                        for r in range(len(dst["A"])):
                            for c in range(len(dst["A"][r])):
                                if r < len(src["A"]) and c < len(src["A"][r]):
                                    dst["A"][r][c] += w * src["A"][r][c]
                        
                        for r in range(len(dst["B"])):
                            for c in range(len(dst["B"][r])):
                                if r < len(src["B"]) and c < len(src["B"][r]):
                                    dst["B"][r][c] += w * src["B"][r][c]
        
        self.adapters[name] = merged
        self.adapter_info[name] = AdapterInfo(
            name=name,
            adapter_type="merged",
            created_at=datetime.now().isoformat(),
            num_modules=len(all_modules),
            total_params=merged.get_stats()["total_parameters"]
        )
        
        return merged
    
    def save_all(self, directory: str) -> None:
        """Save all adapters to directory."""
        import os
        os.makedirs(directory, exist_ok=True)
        
        # Save each adapter
        for name, adapter in self.adapters.items():
            adapter.save(f"{directory}/{name}.json")
        
        # Save registry
        registry = {
            "adapters": [info.__dict__ for info in self.adapter_info.values()],
            "active": self.active_adapter
        }
        with open(f"{directory}/registry.json", "w") as f:
            json.dump(registry, f)
    
    def load_all(self, directory: str) -> None:
        """Load all adapters from directory."""
        import os
        
        registry_path = f"{directory}/registry.json"
        if not os.path.exists(registry_path):
            return
        
        with open(registry_path, "r") as f:
            registry = json.load(f)
        
        for info_dict in registry["adapters"]:
            name = info_dict["name"]
            adapter_type = info_dict["adapter_type"]
            path = f"{directory}/{name}.json"
            
            if not os.path.exists(path):
                continue
            
            if adapter_type == "qlora":
                adapter = QLoRAAdapter()
            else:
                adapter = LoRAAdapter()
            
            adapter.load(path)
            self.adapters[name] = adapter
            self.adapter_info[name] = AdapterInfo(**info_dict)
        
        self.active_adapter = registry.get("active")
    
    def get_stats(self) -> Dict[str, Any]:
        """Get manager statistics."""
        total_params = sum(
            adapter.get_stats()["total_parameters"]
            for adapter in self.adapters.values()
        )
        
        return {
            "num_adapters": len(self.adapters),
            "active_adapter": self.active_adapter,
            "total_parameters": total_params,
            "adapter_types": {
                info.adapter_type: sum(1 for i in self.adapter_info.values() if i.adapter_type == info.adapter_type)
                for info in self.adapter_info.values()
            }
        }
