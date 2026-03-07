---
name: routing-grid
description: AXE-FX III-style 12×4 routing grid for REVITHION STUDIO — grid topology, 36+ block types, scenes, channels, X/Y, feedback, and global blocks.
---

# Routing Grid — REVITHION Signal Processing Core

12 columns × 4 rows. Each cell holds one processing block or is empty. Signal flows left-to-right; rows run in parallel and sum at the output.

## Grid Architecture
```cpp
constexpr int kGridCols = 12, kGridRows = 4;
struct BlockSlot {
    int blockId = -1;  BlockType type = BlockType::None;
    bool bypassed = false;  int channel = 0;  bool xyState = false;
    std::unique_ptr<AudioBlock> processor;
};
struct RoutingGrid {
    std::array<std::array<BlockSlot, kGridCols>, kGridRows> cells;
    void process (juce::AudioBuffer<float>& buf, int n) {
        juce::AudioBuffer<float> rows[kGridRows];
        for (auto& r : rows) r.makeCopyOf (buf);
        for (int col = 0; col < kGridCols; ++col)
            for (int row = 0; row < kGridRows; ++row) {
                auto& s = cells[row][col];
                if (s.processor && !s.bypassed) s.processor->process (rows[row], n);
            }
        buf.clear();
        for (int row = 0; row < kGridRows; ++row)
            for (int ch = 0; ch < buf.getNumChannels(); ++ch)
                buf.addFrom (ch, 0, rows[row], ch, 0, n);
    }
};
```

## Serial / Parallel Paths
Row 0 is the primary serial chain. Rows 1–3 create parallel paths that split at a column and recombine via summing.
```cpp
void dryWetParallel (juce::AudioBuffer<float>& buf, AudioBlock& fx, float wet, int n) {
    juce::AudioBuffer<float> dry;  dry.makeCopyOf (buf);
    fx.process (buf, n);
    for (int ch = 0; ch < buf.getNumChannels(); ++ch) {
        buf.applyGain (ch, 0, n, wet);
        buf.addFrom (ch, 0, dry, ch, 0, n, 1.0f - wet);
    }
}
```

## Block Types
```cpp
enum class BlockType {
    None, Input, Output, Amp, Cab, Drive, Delay, Reverb, Compressor, GateExpander,
    GraphicEQ, ParametricEQ, PitchShifter, Chorus, Flanger, Phaser, Wah, TremoloPan,
    Synth, Enhancer, MultibandCompressor, Crossover, Mixer, Multiplexer, Metering,
    FXLoop, ToneMatch, Filter, Formant, Vocoder, VolumePan, Looper, IR_Player,
    NoiseGate, DeEsser, Exciter, Saturator  // 36+
};
std::unique_ptr<AudioBlock> createBlock (BlockType t) {
    switch (t) {
        case BlockType::Amp:   return std::make_unique<AmpBlock>();
        case BlockType::Cab:   return std::make_unique<CabBlock>();
        case BlockType::Delay: return std::make_unique<DelayBlock>();
        /* ... factory covers all 36+ types */
        default:               return nullptr;
    }
}
```

## Scene System — 8 Scenes per Preset
Each scene stores bypass + X/Y state for every block. Grid layout stays fixed.
```cpp
struct SceneState {
    struct PerBlock { bool bypassed; bool xyState; };
    std::unordered_map<int, PerBlock> blocks;
};
struct Preset {
    RoutingGrid grid;  std::array<SceneState, 8> scenes;
    void switchScene (int idx, int xfade = 128) {
        for (auto& [id, st] : scenes[idx].blocks) {
            auto* s = grid.findSlot (id);  if (!s) continue;
            if (s->bypassed != st.bypassed)
                s->processor->setCrossfadeBypass (st.bypassed, xfade);
            s->xyState = st.xyState;
        }
    }
};
```

## Channel System — A/B/C/D per Block
Four independent parameter sets per block. Switching interpolates to prevent zipper noise.
```cpp
class AudioBlock {
protected:
    using Params = std::unordered_map<juce::String, float>;
    std::array<Params, 4> channels;
    int active = 0, target = 0, rampLeft = 0;
public:
    void setChannel (int ch, int ramp = 64) { target = ch; rampLeft = ramp; }
    float param (const juce::String& k) {
        if (rampLeft <= 0) return channels[active][k];
        float t = 1.0f - rampLeft / 64.0f;
        return channels[active][k] * (1 - t) + channels[target][k] * t;
    }
};
```

## X/Y Switching
Two complete parameter snapshots per block for instant A/B comparison, toggled per-scene.
```cpp
struct XYPair { AudioBlock::Params x, y; };
void AudioBlock::applyXY (bool useY) { channels[active] = useY ? xy.y : xy.x; }
```

## Feedback Loops
One-block delay prevents zero-delay recursion. Gain hard-clamped below 1.0 to stop runaway.
```cpp
class FeedbackLoop {
    juce::AudioBuffer<float> buf;
public:
    void prepare (int maxN) { buf.setSize (2, maxN); buf.clear(); }
    void push (const juce::AudioBuffer<float>& src, float gain, int n) {
        float g = juce::jlimit (0.0f, 0.95f, gain);
        for (int ch = 0; ch < src.getNumChannels(); ++ch)
            buf.copyFromWithRamp (ch, 0, src.getReadPointer (ch), n, g, g);
    }
    void mixInto (juce::AudioBuffer<float>& dst, int n) {
        for (int ch = 0; ch < dst.getNumChannels(); ++ch)
            dst.addFrom (ch, 0, buf, ch, 0, n);
        buf.clear();
    }
};
```

## Global Blocks
Settings persist across all presets via a shared registry. Grid slots store a reference ID.
```cpp
class GlobalBlockRegistry {
    std::unordered_map<int, AudioBlock::Params> reg;
public:
    void store (int id, const AudioBlock::Params& p) { reg[id] = p; }
    AudioBlock::Params* get (int id) { auto it = reg.find(id); return it != reg.end() ? &it->second : nullptr; }
};
```

## Input / Output Blocks
Input selects source; output selects destination. Locked to columns 0 and 11 respectively.
```cpp
enum class InputSource  { Instrument, Aux, USB_1_2, USB_3_4 };
enum class OutputTarget { Main_LR, FX_Send, Headphones, USB_Out };
struct IOConfig {
    InputSource source = InputSource::Instrument;  OutputTarget target = OutputTarget::Main_LR;
    float inputGain = 1.0f, outputGain = 1.0f;
};
```

## Anti-Patterns
- **Processing empty slots** — null-check `processor` before `process()`. Most of 48 cells will be empty.
- **Feedback gain ≥ 1.0** — signal explodes within milliseconds. Hard-clamp to 0.95 max.
- **Scene switch without crossfade** — toggling bypass instantly causes clicks. Ramp ≥ 64 samples.
- **Audio-thread allocation** — `createBlock()` on message thread only; pre-allocate during preset load.
- **Summing inactive rows** — skip rows with zero blocks to avoid wasting CPU on silence.

## Checklist
- [ ] Grid is 12×4 with left-to-right flow
- [ ] Empty slots skipped at zero cost
- [ ] Parallel rows split/sum phase-coherently
- [ ] All 36+ block types in factory
- [ ] Scene switching crossfades bypass (≥ 64 samples)
- [ ] Channel switching interpolates params
- [ ] X/Y toggle stored per-scene per-block
- [ ] Feedback gain clamped below 1.0
- [ ] Global blocks use shared registry
- [ ] I/O blocks locked to columns 0 and 11
- [ ] No heap allocation on the audio thread
