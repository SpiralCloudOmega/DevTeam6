---
name: juce-audio-processor
description: >
  JUCE AudioProcessor lifecycle, state serialization, bus layouts, latency, tail time,
  bypass, and double-precision processing for REVITHION STUDIO plugins.
---

# JUCE AudioProcessor — Core Plugin Base Class

## 1. Core Lifecycle

```cpp
#include <juce_audio_processors/juce_audio_processors.h>

class RevithionProcessor : public juce::AudioProcessor
{
public:
    RevithionProcessor()
        : AudioProcessor(BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock,
                                (juce::uint32)getTotalNumOutputChannels() };
        chain.prepare(spec);
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        juce::ScopedNoDenormals noDenormals;
        for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
            buffer.clear(ch, 0, buffer.getNumSamples());
        dsp::AudioBlock<float> block(buffer);
        chain.process(dsp::ProcessContextReplacing<float>(block));
    }

    void releaseResources() override { chain.reset(); }
private:
    dsp::ProcessorChain<dsp::Gain<float>, dsp::IIR::Filter<float>> chain;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RevithionProcessor)
};
```

## 2. State Serialization

```cpp
void getStateInformation(juce::MemoryBlock& destData) override
{
    auto xml = parameters.copyState().createXml();
    copyXmlToBinary(*xml, destData);
}
void setStateInformation(const void* data, int sizeInBytes) override
{
    auto xml = getXmlFromBinary(data, sizeInBytes);
    if (xml && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}
```

For extra state (IR paths, presets), wrap in a parent `XmlElement`:

```cpp
void getStateInformation(juce::MemoryBlock& destData) override
{
    juce::XmlElement root("RevithionState");
    root.setAttribute("irPath", currentIRPath);
    root.addChildElement(parameters.copyState().createXml().release());
    copyXmlToBinary(root, destData);
}
```

## 3. BusesProperties

```cpp
// Mono→Stereo (amp sim) | Stereo→Stereo (master FX) | Stereo+Sidechain (compressor)
BusesProperties().withInput("Input", AudioChannelSet::mono(), true)
                 .withOutput("Output", AudioChannelSet::stereo(), true)

BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)
                 .withOutput("Output", AudioChannelSet::stereo(), true)

BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)
                 .withInput("Sidechain", AudioChannelSet::mono(), true)
                 .withOutput("Output", AudioChannelSet::stereo(), true)
```

## 4. Channel Layout Negotiation

```cpp
bool isBusesLayoutSupported(const BusesLayout& layouts) const override
{
    const auto& in  = layouts.getMainInputChannelSet();
    const auto& out = layouts.getMainOutputChannelSet();
    if (out.isDisabled()) return false;
    if (in == AudioChannelSet::mono() && out == AudioChannelSet::stereo()) return true;
    return in == out; // accept matched I/O for stereo, surround, etc.
}
```

For sidechain plugins, also validate `layouts.getChannelSet(true, 1)` — accept
`isDisabled()`, mono, or stereo.

## 5. Latency Reporting

Call `setLatencySamples()` in `prepareToPlay` and whenever latency changes. The host
uses this for delay compensation. Rate-dependent: `setLatencySamples(int(0.01 * sr))`.

```cpp
void prepareToPlay(double, int) override { setLatencySamples(512); } // lookahead
```

## 6. Tail Time

Seconds of audio after input stops. Hosts extend offline bounces by this amount.
Return `0.0` for EQ/gain, `infinity()` for freeze reverb.

```cpp
double getTailLengthSeconds() const override { return 10.0; } // reverb
```

## 7. Bypass

Default `processBlockBypassed` silences output — override to pass audio through:

```cpp
void processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());
    updateSmoothedValues(buffer.getNumSamples()); // keep state alive
}
```

## 8. Double Precision

```cpp
bool supportsDoublePrecisionProcessing() const override { return true; }

void processBlock(juce::AudioBuffer<double>& b, juce::MidiBuffer&) override
{ processInternal(b); }

template <typename T>
void processInternal(juce::AudioBuffer<T>& buffer)
{
    juce::ScopedNoDenormals noDenormals;
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());
    dsp::AudioBlock<T> block(buffer);
    dsp::ProcessContextReplacing<T> ctx(block);
    // dispatch to type-appropriate ProcessorChain
}
```

## Anti-Patterns

| Pattern | Problem |
|---|---|
| Allocating in `processBlock` | Audio thread must never allocate — causes glitches |
| Missing `ScopedNoDenormals` | Denorms spike CPU on x86 |
| Not clearing unused channels | Garbage audio in host mixer |
| Hardcoded channel count | Breaks in surround/mono hosts |
| Stale latency reporting | Host delay compensation drifts |
| Zero tail for reverbs/delays | Offline bounces cut off tails |
| Locks or I/O on audio thread | Priority inversion → dropouts |

## Checklist

- [ ] `BusesProperties` declares correct I/O and sidechain buses
- [ ] `prepareToPlay` pre-allocates buffers, resets DSP, calls `setLatencySamples`
- [ ] `processBlock` starts with `ScopedNoDenormals`, clears unused channels
- [ ] `releaseResources` frees non-trivial allocations
- [ ] State round-trips all parameters and custom data via XML
- [ ] `isBusesLayoutSupported` validates accepted channel configs
- [ ] `getTailLengthSeconds` returns accurate value for effect type
- [ ] `processBlockBypassed` passes audio through, maintains internal state
- [ ] Double-precision overrides provided when `supportsDoublePrecisionProcessing` is true
- [ ] No heap allocations, locks, or I/O on the audio thread
- [ ] Tested in at least two hosts (REAPER + Ableton Live)
