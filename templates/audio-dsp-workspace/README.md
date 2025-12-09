# Audio DSP / JUCE Workspace

Professional audio plugin development environment using the JUCE framework. Build VST3, AU, and AAX plugins for Pro Tools, Logic Pro, Ableton Live, and more.

## 🚀 Quick Start

### GitHub Codespaces
1. Create a new codespace from this template
2. Wait for JUCE to install (included in container)
3. Start building your audio plugin!

### Local Development
1. Install Docker and VS Code with Dev Containers extension
2. Open this folder and click "Reopen in Container"
3. JUCE framework is automatically installed

## 📦 What's Included

### JUCE Framework
- **Version 7.0.12** - Latest stable JUCE
- All JUCE modules included
- VST3, AU, AAX, and Standalone plugin formats
- Cross-platform support

### Audio Libraries
- **ALSA** - Linux audio
- **JACK** - Low-latency audio routing
- **PulseAudio** - Sound server

### Build Tools
- CMake with JUCE integration
- Ninja build system
- C++17 compiler support

## 🔨 Building Your Plugin

### Configure and Build
```bash
# Configure CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build the plugin
cmake --build build

# Plugin outputs will be in:
# build/AudioPlugin_artefacts/
```

### Plugin Formats
The build produces:
- **VST3** - Compatible with most DAWs
- **AU** - macOS Audio Unit (Mac only)
- **AAX** - Pro Tools (requires AAX SDK)
- **Standalone** - Independent application

## 🎹 Plugin Development

### Basic Plugin Structure

```cpp
// PluginProcessor.h - Audio processing
class AudioPluginProcessor : public juce::AudioProcessor {
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    // ... parameter management
};

// PluginEditor.h - GUI
class AudioPluginEditor : public juce::AudioProcessorEditor {
    void paint(juce::Graphics&) override;
    void resized() override;
    // ... UI components
};
```

### Adding Parameters

```cpp
// In constructor
addParameter(gainParameter = new juce::AudioParameterFloat(
    "gain", "Gain", 0.0f, 1.0f, 0.5f));

// In processBlock
float gain = gainParameter->get();
for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* channelData = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        channelData[sample] *= gain;
    }
}
```

### Creating GUI Components

```cpp
// In PluginEditor.h
private:
    juce::Slider gainSlider;
    juce::Label gainLabel;

// In constructor
gainSlider.setSliderStyle(juce::Slider::LinearVertical);
gainSlider.setRange(0.0, 1.0);
addAndMakeVisible(gainSlider);

// In resized()
gainSlider.setBounds(50, 50, 100, 200);
```

## 🎚️ Common DSP Algorithms

### Simple Gain
```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    float gain = 0.5f;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        buffer.applyGain(channel, 0, buffer.getNumSamples(), gain);
    }
}
```

### Low-Pass Filter
```cpp
juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                juce::dsp::IIR::Coefficients<float>> lowPassFilter;

void prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    lowPassFilter.prepare(spec);
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, 1000.0f);
}
```

### Delay Effect
```cpp
juce::dsp::DelayLine<float> delayLine;

void prepareToPlay(double sampleRate, int samplesPerBlock) {
    delayLine.prepare({sampleRate, samplesPerBlock, 2});
    delayLine.setDelay(sampleRate * 0.5); // 500ms delay
}

void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    delayLine.process(context);
}
```

## 🎨 GUI Design

### Using JUCE Look and Feel
```cpp
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
    void drawRotarySlider(...) override {
        // Custom slider rendering
    }
};

// In editor
CustomLookAndFeel customLF;
setLookAndFeel(&customLF);
```

### Adding Visualizers
```cpp
class WaveformDisplay : public juce::Component, public juce::Timer {
    void timerCallback() override {
        repaint(); // Update display
    }
    
    void paint(juce::Graphics& g) override {
        // Draw waveform
    }
};
```

## 🧪 Testing Your Plugin

### Standalone Testing
```bash
# Run standalone version
./build/AudioPlugin_artefacts/Standalone/AudioPlugin
```

### VST3 Testing
1. Copy VST3 to your DAW's plugin folder:
   - Windows: `C:\Program Files\Common Files\VST3\`
   - macOS: `~/Library/Audio/Plug-Ins/VST3/`
   - Linux: `~/.vst3/`

2. Rescan plugins in your DAW
3. Load and test

### Recommended DAWs for Testing
- **Reaper** - Free, cross-platform
- **Ardour** - Open source, Linux-friendly
- **LMMS** - Free, beginner-friendly

## 📚 JUCE Modules Reference

| Module | Purpose |
|--------|---------|
| `juce_audio_basics` | Audio buffers, MIDI, sample rates |
| `juce_audio_devices` | Audio I/O, MIDI I/O |
| `juce_audio_formats` | Audio file reading/writing |
| `juce_audio_processors` | Plugin hosting, parameters |
| `juce_audio_utils` | Audio components, players |
| `juce_dsp` | DSP utilities, filters, effects |
| `juce_graphics` | 2D graphics, images |
| `juce_gui_basics` | GUI components, layout |

## 🎯 Example Projects

### Gain Plugin
Simple volume control with meter

### EQ Plugin
Multi-band equalizer with frequency analyzer

### Compressor
Dynamic range compression with attack/release

### Reverb
Algorithmic reverb effect

### Synthesizer
Virtual instrument with oscillators and filters

## 🔧 Advanced Topics

### Parameter Automation
```cpp
class AutomatableParameter : public juce::AudioProcessorParameter {
    // Implement parameter automation
};
```

### Preset Management
```cpp
void AudioPluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}
```

### MIDI Processing
```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (const auto metadata : midiMessages) {
        auto message = metadata.getMessage();
        if (message.isNoteOn()) {
            // Handle note on
        }
    }
}
```

## 📖 Resources

### JUCE Documentation
- [JUCE Website](https://juce.com/)
- [JUCE Tutorials](https://juce.com/learn/tutorials)
- [JUCE API Documentation](https://docs.juce.com/)
- [JUCE Forum](https://forum.juce.com/)

### Audio DSP Learning
- [The Audio Programmer](https://www.youtube.com/@TheAudioProgrammer) - YouTube channel
- [Will Pirkle Books](http://www.willpirkle.com/) - DSP textbooks
- [Digital Signal Processing](https://ccrma.stanford.edu/~jos/) - Stanford course
- [MusicDSP Archive](https://www.musicdsp.org/) - Algorithm database

### Plugin Development
- [JUCE Plugin Examples](https://github.com/juce-framework/JUCE/tree/master/examples)
- [Audio Plugin Development](https://www.theaudioprogrammer.com/)
- [VST3 SDK Documentation](https://steinbergmedia.github.io/vst3_doc/)

## 🐛 Troubleshooting

### Build Errors
```bash
# Clean and rebuild
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Plugin Not Loading
- Check plugin format compatibility with your DAW
- Verify plugin is in correct folder
- Check DAW's plugin scan settings

### Audio Glitches
- Increase buffer size in `prepareToPlay`
- Use lock-free data structures
- Avoid allocations in `processBlock`

## 📝 License

JUCE is dual-licensed:
- **GPL v3** - Free for open source projects
- **Commercial** - Required for closed-source products

See [JUCE Licensing](https://juce.com/legal/juce-7-licence/) for details.

## 🤝 Contributing

Contributions welcome! Submit issues or pull requests.

---

**Happy Plugin Development! 🎵**
