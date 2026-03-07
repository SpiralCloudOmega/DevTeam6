---
name: wavenet-convnet
description: WaveNet dilated causal convolutions for real-time audio effects and amp modeling
---

# WaveNet & Convolutional Networks for Audio Processing

WaveNet uses stacked dilated causal convolutions to model long-range temporal dependencies without recurrence — ideal for amp modeling and neural audio synthesis in REVITHION STUDIO.
## Dilated Causal Convolutions

Dilation grows exponentially per layer (1, 2, 4, 8, 16 …) so the receptive field expands exponentially while parameters grow linearly. Causal padding ensures output at time `t` depends only on inputs `≤ t`.

```python
import torch.nn as nn

def build_dilated_stack(channels: int, kernel_size: int, num_layers: int) -> nn.ModuleList:
    layers = nn.ModuleList()
    for i in range(num_layers):
        dilation = 2 ** i
        padding = (kernel_size - 1) * dilation  # causal: left-pad only
        layers.append(nn.Conv1d(channels, channels, kernel_size,
                                dilation=dilation, padding=padding))
    return layers
```
## Residual Blocks with Gated Activations

Each block applies `tanh(filter) * sigmoid(gate)` — learning what to pass and how much. A 1×1 conv projects back to residual dimension; the input is added via skip path.

```python
import torch, torch.nn as nn

class WaveNetResidualBlock(nn.Module):
    def __init__(self, res_ch: int, gate_ch: int, skip_ch: int,
                 kernel_size: int, dilation: int):
        super().__init__()
        self.pad = (kernel_size - 1) * dilation
        self.conv = nn.Conv1d(res_ch, 2 * gate_ch, kernel_size, dilation=dilation)
        self.res_1x1 = nn.Conv1d(gate_ch, res_ch, 1)
        self.skip_1x1 = nn.Conv1d(gate_ch, skip_ch, 1)

    def forward(self, x: torch.Tensor) -> tuple[torch.Tensor, torch.Tensor]:
        h = torch.nn.functional.pad(x, (self.pad, 0))
        t, g = self.conv(h).chunk(2, dim=1)
        activated = torch.tanh(t) * torch.sigmoid(g)
        return self.res_1x1(activated) + x, self.skip_1x1(activated)
```
## Skip Connections Aggregation

Skip outputs from every block are summed element-wise, then passed through two 1×1 convolutions with ReLU — deep layers contribute gradients directly, stabilizing training across 30+ layer stacks.
## Receptive Field Calculation

```python
def receptive_field(num_layers: int, kernel_size: int, num_stacks: int = 1) -> int:
    """RF = stacks × (kernel_size - 1) × sum(dilations) + 1"""
    return num_stacks * (kernel_size - 1) * sum(2**i for i in range(num_layers)) + 1

# 10 layers, kernel 3, 1 stack → 2047 samples (~46 ms at 44.1 kHz)
print(receptive_field(10, 3, 1))
```

Stack multiple dilation cycles (`num_stacks > 1`) to widen coverage without deepening.
## Real-Time Cached Inference

Autoregressive generation is too slow. Use **cached inference** with a ring buffer — O(1) per layer instead of O(receptive_field).

```cpp
#include <array>
#include <cstddef>

template <size_t MaxDilation>
struct DilatedConvCache {
    std::array<float, MaxDilation> buffer{};
    size_t write_pos = 0;
    void push(float sample) { buffer[write_pos++ % MaxDilation] = sample; }
    float tap(size_t dilation) const {
        return buffer[(write_pos - dilation) % MaxDilation];
    }
};

inline float dilated_conv_step(DilatedConvCache<512>& cache,
                               const float* w, size_t ks, size_t dil, float bias) {
    float acc = bias;
    for (size_t k = 0; k < ks; ++k)
        acc += w[k] * cache.tap((ks - 1 - k) * dil);
    return acc;
}
```

Process in blocks of 32–128 samples; vectorize with SIMD (SSE/AVX) for 10×+ real-time throughput.
## Pruning and Quantization

- **Structured pruning**: zero entire filters so convolutions shrink. Target 50–70 % sparsity, < 1 dB SNR loss.
- **INT8 quantization**: 8-bit post-training via `torch.quantization.quantize_dynamic` or ONNX Runtime. 2–4× speedup.
- **Knowledge distillation**: train a smaller student WaveNet against full model outputs to recover pruning losses.
## WaveNet vs LSTM Tradeoffs

| Aspect           | WaveNet (CNN)                        | LSTM (RNN)                            |
|------------------|--------------------------------------|---------------------------------------|
| **Quality**      | Excellent; long-range detail         | Good; struggles with long context     |
| **Latency**      | Low with cached inference (~1 ms)    | Low per-sample, but sequential        |
| **Parallelism**  | Fully parallelizable in training     | Sequential; slower to train           |
| **Model size**   | Larger (many conv filters)           | Compact (fewer parameters)            |
| **Best for**     | High-fidelity amp/cab modeling       | Lightweight overdrive/distortion      |

Use WaveNet for high-gain amp channels and LSTM for simpler utility effects in REVITHION STUDIO's 12×4 routing grid.
## Anti-Patterns

- **Non-causal padding**: symmetric padding breaks real-time causality — always left-pad only.
- **Flat dilation**: same dilation every layer wastes depth without expanding receptive field.
- **Ignoring receptive field size**: must be ≥ target impulse response (~20 ms for cabinets).
- **Cross-sample-rate deployment**: dilation patterns are sample-rate-dependent — retrain or resample.
- **Skipping skip connections**: without aggregation, gradients vanish in deep stacks.
- **Unstructured pruning**: random weight zeroing yields no real speedup — use filter-level pruning.
## Checklist

- [ ] Dilation doubles per layer (1, 2, 4, 8, 16 …)
- [ ] Causal left-padding = `(kernel_size - 1) × dilation` per layer
- [ ] Gated activation uses `tanh × sigmoid` split
- [ ] Skip connections summed before output projection
- [ ] Receptive field ≥ target impulse response at deployment sample rate
- [ ] Cached ring-buffer inference (< 3 ms latency budget)
- [ ] INT8 quantization applied; SNR regression < 1 dB
- [ ] Structured pruning 50–70 % verified with A/B listening test
- [ ] Benchmarked at target block size (32–128 samples) on deployment CPU
