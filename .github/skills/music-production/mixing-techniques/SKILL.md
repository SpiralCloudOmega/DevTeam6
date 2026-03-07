---
name: mixing-techniques
description: Professional mixing techniques for REVITHION STUDIO — gain staging, bus routing, parallel compression, send effects, VCA grouping, automation, Scheps Rear Bus, and stem export in JUCE C++.
---

# Mixing Techniques — REVITHION STUDIO

Core mixing techniques implemented as DSP processors and routing patterns in the REVITHION engine (JUCE 7/8, C++17).

## 1. Gain Staging

Record at **-18 dBFS** (analog 0 VU). This keeps every plugin in the chain at its designed operating level, prevents cumulative distortion, and preserves dynamic range across dozens of inserts.

```cpp
class GainUtilityProcessor : public juce::AudioProcessor {
public:
    GainUtilityProcessor()
        : gainParam(new juce::AudioParameterFloat("gain", "Gain (dB)", -48.0f, 24.0f, 0.0f)) {
        addParameter(gainParam);
    }
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {
        float target = juce::Decibels::decibelsToGain(gainParam->get());
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.applyGainRamp(ch, 0, buffer.getNumSamples(), prevGain, target);
        prevGain = target;
    }
private:
    juce::AudioParameterFloat* gainParam;
    float prevGain = 1.0f;
};
```

## 2. Bus Routing

Group related tracks onto sub-mix buses (Drums, Bass, Guitars, Vocals, FX → Master). Pre-fader sends tap before the fader (cue mixes); post-fader sends tap after (reverb/delay that tracks the fader).

```cpp
void buildMixBusGraph(juce::AudioProcessorGraph& graph) {
    auto master = graph.addNode(std::make_unique<MasterBusProcessor>());
    auto output = graph.addNode(std::make_unique<AudioGraphIOProcessor>(
        AudioGraphIOProcessor::audioOutputNode));
    for (auto name : {"Drums", "Bass", "Vocals", "Guitars", "FX"}) {
        auto bus = graph.addNode(std::make_unique<BusProcessor>(name));
        graph.addConnection({{bus->nodeID, 0}, {master->nodeID, 0}});
        graph.addConnection({{bus->nodeID, 1}, {master->nodeID, 1}});
    }
    graph.addConnection({{master->nodeID, 0}, {output->nodeID, 0}});
    graph.addConnection({{master->nodeID, 1}, {output->nodeID, 1}});
}
```

## 3. Parallel Compression (NY Compression)

Blend dry signal with a heavily compressed copy. Settings: attack 0.5–2 ms, release 50–100 ms, ratio 10:1+, threshold –30 dB. Preserves transients while adding sustain.

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {
    juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
    dry.makeCopyOf(buffer);
    compressor.setThreshold(-30.0f); compressor.setRatio(12.0f);
    compressor.setAttack(1.0f);      compressor.setRelease(60.0f);
    compressor.process(juce::dsp::ProcessContextReplacing<float>(
        juce::dsp::AudioBlock<float>(buffer)));
    float mix = mixParam->get(); // 30–50% typical
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* w = buffer.getWritePointer(ch);
        const auto* d = dry.getReadPointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            w[i] = d[i] * (1.0f - mix) + w[i] * mix;
    }
}
```

## 4. Send Effects

Place reverb/delay on **aux/send buses** — all tracks share one instance for a cohesive space and lower CPU cost.

```cpp
void connectPostFaderSend(juce::AudioProcessorGraph& graph,
                          juce::AudioProcessorGraph::NodeID src,
                          juce::AudioProcessorGraph::NodeID reverbSend) {
    // Post-fader: signal tapped after channel fader → shared reverb bus
    graph.addConnection({{src, 0}, {reverbSend, 0}});
    graph.addConnection({{src, 1}, {reverbSend, 1}});
    // Pre-fader: tap before the fader node in the channel strip sub-graph
}
```

## 5. VCA Grouping

VCA groups apply **multiplicative gain** — each member's output is scaled by the VCA master fader. Unlike bus groups (which sum audio), VCA keeps individual inserts, sends, and panning intact.

```cpp
struct VCAGroup {
    float vcaGain = 1.0f; // linear, from VCA master fader
    std::vector<ChannelStrip*> members;
    void apply() {
        for (auto* ch : members)
            ch->setOutputGain(ch->getChannelFaderGain() * vcaGain);
    }
};
```

Drop the VCA 6 dB → every member drops 6 dB, preserving relative balance and individual send levels.

## 6. Automation

Time-stamped parameter events with interpolation between points. Automate in passes: volume → pan → sends → plugin params.

```cpp
struct AutomationPoint {
    double timeInBeats;
    float value; // normalized 0..1
};
class AutomationLane {
public:
    float getValueAt(double t) const {
        if (points.empty()) return 0.5f;
        auto it = std::lower_bound(points.begin(), points.end(), t,
            [](const AutomationPoint& p, double t) { return p.timeInBeats < t; });
        if (it == points.begin()) return it->value;
        if (it == points.end())   return points.back().value;
        const auto& a = *std::prev(it); const auto& b = *it;
        float frac = float((t - a.timeInBeats) / (b.timeInBeats - a.timeInBeats));
        return a.value + frac * (b.value - a.value);
    }
private:
    std::vector<AutomationPoint> points;
};
```

## 7. Scheps Rear Bus Technique

Route every track through **four parallel buses** (compression, distortion, short reverb, delay) plus the main bus. Keep channel inserts minimal — the rear buses shape overall tone.

```cpp
void buildSchepsRearBuses(juce::AudioProcessorGraph& graph,
                          juce::AudioProcessorGraph::NodeID master,
                          juce::AudioProcessorGraph::NodeID output) {
    auto comp   = graph.addNode(std::make_unique<CompressorBus>());
    auto dist   = graph.addNode(std::make_unique<SaturationBus>());
    auto reverb = graph.addNode(std::make_unique<ShortReverbBus>());
    auto delay  = graph.addNode(std::make_unique<SlapDelayBus>());
    for (auto* rear : {comp, dist, reverb, delay}) {
        graph.addConnection({{rear->nodeID, 0}, {master, 0}});
        graph.addConnection({{rear->nodeID, 1}, {master, 1}});
    }
    graph.addConnection({{master, 0}, {output, 0}});
    graph.addConnection({{master, 1}, {output, 1}});
}
```

## 8. Stem Export

Bounce each bus to a separate WAV file (48 kHz / 24-bit) for remix, live playback, or mastering delivery.

```cpp
bool exportStem(juce::AudioProcessorGraph& graph, const juce::File& outFile,
                double sampleRate, int blockSize, int64_t totalSamples) {
    juce::WavAudioFormat fmt;
    auto writer = fmt.createWriterFor(outFile.createOutputStream().release(),
                                      sampleRate, 2, 24, {}, 0);
    if (!writer) return false;
    juce::AudioBuffer<float> buf(2, blockSize); juce::MidiBuffer midi;
    for (int64_t pos = 0; pos < totalSamples; pos += blockSize) {
        int n = std::min((int64_t)blockSize, totalSamples - pos);
        buf.setSize(2, n, false, false, true); buf.clear();
        graph.processBlock(buf, midi);
        writer->writeFromAudioSampleBuffer(buf, 0, n);
    }
    delete writer;
    return true;
}
```

Always add **2–4 seconds** of extra tail to capture reverb/delay decay beyond the last note.

## Anti-Patterns

- **Gain staging after the fact** — Fix levels at the source, not with corrective plugins downstream.
- **Reverb inserted on every channel** — Use shared aux sends; per-channel reverbs waste CPU and sound incoherent.
- **Bus compression before static balance** — Get the dry balance right first.
- **Ignoring PDC on parallel paths** — Uncompensated parallel buses comb-filter against the dry signal.
- **VCA + post-fader sends without compensation** — VCA drop lowers post-fader sends too; use pre-fader sends when independence is needed.
- **Stem export without tail capture** — Always export with extra time for reverb/delay tails.

## Checklist

- [ ] All channels at **-18 dBFS** nominal level
- [ ] Bus structure: Drums, Bass, Guitars, Vocals, FX, Master
- [ ] Reverb/delay on **send buses**, not channel inserts
- [ ] Parallel compression blend at 30–50 %
- [ ] VCA groups for section-level fader control
- [ ] PDC verified across all parallel paths
- [ ] Automation in passes: volume → pan → sends → params
- [ ] Master bus peaking ≤ **-6 dBFS** before limiter
- [ ] Stems exported per bus at 48 kHz / 24-bit with tail capture
- [ ] Mix checked on monitors, headphones, and phone speaker
