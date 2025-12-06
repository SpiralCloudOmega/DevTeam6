"""
DevTeam6 Neuromorphic Computing

Spiking neural networks for event-driven processing.
"""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Any, Tuple
import math
import random
from datetime import datetime


@dataclass
class SpikeEvent:
    """A spike event from a neuron."""
    neuron_id: int
    timestamp: float
    strength: float


@dataclass
class NeuronConfig:
    """Configuration for spiking neuron."""
    threshold: float = 0.5
    refractory_period: int = 5
    leak_rate: float = 0.1
    reset_potential: float = 0.0


class SpikingNeuron:
    """
    Leaky Integrate-and-Fire (LIF) spiking neuron.
    
    Implements neuromorphic computing concepts for event-driven
    processing with temporal dynamics.
    """
    
    def __init__(
        self,
        neuron_id: int,
        config: Optional[NeuronConfig] = None
    ):
        self.neuron_id = neuron_id
        self.config = config or NeuronConfig()
        
        # State
        self.membrane_potential: float = 0.0
        self.refractory_counter: int = 0
        self.last_spike_time: Optional[float] = None
        
        # Connections
        self.input_weights: Dict[int, float] = {}  # source_id -> weight
        
        # History
        self.spike_history: List[SpikeEvent] = []
        self.potential_history: List[float] = []
    
    def receive_spike(
        self,
        source_id: int,
        strength: float,
        timestamp: float
    ) -> Optional[SpikeEvent]:
        """
        Receive input spike from another neuron.
        
        Args:
            source_id: ID of source neuron
            strength: Spike strength
            timestamp: Current simulation time
            
        Returns:
            Spike event if neuron fires, None otherwise
        """
        # Check refractory period
        if self.refractory_counter > 0:
            self.refractory_counter -= 1
            return None
        
        # Get weight for this connection
        weight = self.input_weights.get(source_id, 1.0)
        
        # Update membrane potential
        self.membrane_potential += strength * weight
        
        # Apply leak
        self.membrane_potential *= (1 - self.config.leak_rate)
        
        # Record potential
        self.potential_history.append(self.membrane_potential)
        
        # Check for spike
        if self.membrane_potential >= self.config.threshold:
            return self._fire(timestamp)
        
        return None
    
    def _fire(self, timestamp: float) -> SpikeEvent:
        """Generate spike event."""
        # Calculate spike strength based on excess potential
        excess = self.membrane_potential - self.config.threshold
        strength = 1.0 + excess * 0.5  # Proportional to excess
        
        # Create spike event
        spike = SpikeEvent(
            neuron_id=self.neuron_id,
            timestamp=timestamp,
            strength=strength
        )
        
        # Reset neuron
        self.membrane_potential = self.config.reset_potential
        self.refractory_counter = self.config.refractory_period
        self.last_spike_time = timestamp
        
        # Record spike
        self.spike_history.append(spike)
        
        return spike
    
    def add_connection(self, source_id: int, weight: float) -> None:
        """Add input connection from another neuron."""
        self.input_weights[source_id] = weight
    
    def get_spike_rate(self, window: int = 100) -> float:
        """Get firing rate over recent window."""
        if not self.spike_history:
            return 0.0
        
        recent_spikes = len([
            s for s in self.spike_history[-window:]
        ])
        
        return recent_spikes / window
    
    def reset(self) -> None:
        """Reset neuron state."""
        self.membrane_potential = 0.0
        self.refractory_counter = 0
        self.last_spike_time = None
        self.potential_history.clear()


class SpikingNetwork:
    """
    Network of spiking neurons for neuromorphic computing.
    
    Implements a layered spiking neural network with
    spike-timing dependent plasticity (STDP) learning.
    """
    
    def __init__(
        self,
        layer_sizes: List[int],
        neuron_config: Optional[NeuronConfig] = None
    ):
        self.layer_sizes = layer_sizes
        self.neuron_config = neuron_config or NeuronConfig()
        
        # Create neurons
        self.neurons: List[List[SpikingNeuron]] = []
        self._create_network()
        
        # Simulation state
        self.current_time: float = 0.0
        self.spike_queue: List[SpikeEvent] = []
        
        # STDP parameters
        self.stdp_enabled = True
        self.stdp_lr = 0.01
        self.stdp_window = 20.0
    
    def _create_network(self) -> None:
        """Create layered network structure."""
        neuron_id = 0
        
        for layer_idx, size in enumerate(self.layer_sizes):
            layer = []
            for _ in range(size):
                neuron = SpikingNeuron(neuron_id, self.neuron_config)
                layer.append(neuron)
                neuron_id += 1
            self.neurons.append(layer)
        
        # Create connections between layers
        for layer_idx in range(len(self.neurons) - 1):
            for source in self.neurons[layer_idx]:
                for target in self.neurons[layer_idx + 1]:
                    # Random initial weight
                    weight = random.uniform(0.1, 0.5)
                    target.add_connection(source.neuron_id, weight)
    
    def forward(
        self,
        input_spikes: List[float],
        duration: float = 10.0
    ) -> List[float]:
        """
        Process input through the network.
        
        Args:
            input_spikes: Input spike strengths for first layer
            duration: Simulation duration
            
        Returns:
            Output layer firing rates
        """
        # Reset spike queue
        self.spike_queue.clear()
        
        # Inject input spikes
        for i, strength in enumerate(input_spikes):
            if i < len(self.neurons[0]) and strength > 0:
                spike = SpikeEvent(
                    neuron_id=self.neurons[0][i].neuron_id,
                    timestamp=self.current_time,
                    strength=strength
                )
                self.spike_queue.append(spike)
        
        # Simulate for duration
        end_time = self.current_time + duration
        
        while self.current_time < end_time:
            # Process pending spikes
            current_spikes = [s for s in self.spike_queue if s.timestamp <= self.current_time]
            self.spike_queue = [s for s in self.spike_queue if s.timestamp > self.current_time]
            
            for spike in current_spikes:
                self._propagate_spike(spike)
            
            # Advance time
            self.current_time += 1.0
        
        # Get output firing rates
        output_rates = [
            neuron.get_spike_rate(int(duration))
            for neuron in self.neurons[-1]
        ]
        
        return output_rates
    
    def _propagate_spike(self, spike: SpikeEvent) -> None:
        """Propagate spike through network."""
        # Find which layer the spike came from
        source_layer = None
        for layer_idx, layer in enumerate(self.neurons):
            for neuron in layer:
                if neuron.neuron_id == spike.neuron_id:
                    source_layer = layer_idx
                    break
            if source_layer is not None:
                break
        
        if source_layer is None or source_layer >= len(self.neurons) - 1:
            return
        
        # Propagate to next layer
        target_layer = self.neurons[source_layer + 1]
        for target in target_layer:
            result = target.receive_spike(
                spike.neuron_id,
                spike.strength,
                self.current_time
            )
            
            if result:
                self.spike_queue.append(result)
                
                # Apply STDP if enabled
                if self.stdp_enabled:
                    self._apply_stdp(spike, result)
    
    def _apply_stdp(
        self,
        pre_spike: SpikeEvent,
        post_spike: SpikeEvent
    ) -> None:
        """Apply Spike-Timing Dependent Plasticity."""
        # Find post-synaptic neuron
        for layer in self.neurons:
            for neuron in layer:
                if neuron.neuron_id == post_spike.neuron_id:
                    if pre_spike.neuron_id in neuron.input_weights:
                        # Calculate timing difference
                        dt = post_spike.timestamp - pre_spike.timestamp
                        
                        if abs(dt) < self.stdp_window:
                            # LTP (potentiation) if pre before post
                            if dt > 0:
                                dw = self.stdp_lr * math.exp(-dt / self.stdp_window)
                            # LTD (depression) if post before pre
                            else:
                                dw = -self.stdp_lr * math.exp(dt / self.stdp_window)
                            
                            # Update weight
                            current_weight = neuron.input_weights[pre_spike.neuron_id]
                            new_weight = max(0.01, min(1.0, current_weight + dw))
                            neuron.input_weights[pre_spike.neuron_id] = new_weight
                    return
    
    def get_stats(self) -> Dict[str, Any]:
        """Get network statistics."""
        total_spikes = sum(
            len(neuron.spike_history)
            for layer in self.neurons
            for neuron in layer
        )
        
        return {
            "num_layers": len(self.neurons),
            "layer_sizes": self.layer_sizes,
            "total_neurons": sum(len(layer) for layer in self.neurons),
            "total_spikes": total_spikes,
            "current_time": self.current_time,
            "stdp_enabled": self.stdp_enabled
        }
    
    def reset(self) -> None:
        """Reset network state."""
        for layer in self.neurons:
            for neuron in layer:
                neuron.reset()
        self.spike_queue.clear()
        self.current_time = 0.0
