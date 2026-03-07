---
name: JUCE AudioProcessorValueTreeState (APVTS) Parameters
description: >
  Managing plugin parameters with APVTS — creation, layout, listeners, UI
  attachments, thread-safe access, smoothing, groups, and undo/redo.
---

# JUCE APVTS Parameter Management

## 1. Parameter Creation
```cpp
auto freq = std::make_unique<juce::AudioParameterFloat>(   // skewed for log feel
    juce::ParameterID{"filterFreq", 1}, "Filter Frequency",
    juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 1000.0f);
auto gain = std::make_unique<juce::AudioParameterFloat>(   // with string conversion
    juce::ParameterID{"gain", 1}, "Gain",
    juce::NormalisableRange<float>(-60.0f, 12.0f, 0.1f), 0.0f,
    juce::AudioParameterFloatAttributes()
        .withStringFromValueFunction([](float v, int) { return juce::String(v, 1) + " dB"; }));
auto transpose  = std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"transpose", 1}, "Transpose", -24, 24, 0);
auto bypass     = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"bypass", 1}, "Bypass", false);
auto filterType = std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"filterType", 1},
    "Filter Type", juce::StringArray{"Low Pass", "High Pass", "Band Pass", "Notch"}, 0);
```

## 2. ParameterLayout
```cpp
class RevithionProcessor : public juce::AudioProcessor {
public:
    RevithionProcessor()
        : apvts(*this, &undoManager, "PARAMETERS", createParameterLayout()) {}
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gain", 1},
            "Gain", juce::NormalisableRange<float>(-60.0f, 12.0f, 0.1f), 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"filterFreq", 1},
            "Filter Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 1000.0f));
        return layout;
    }
private:
    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;
};
```

## 3. Listeners
```cpp
class RevithionEditor : public juce::AudioProcessorEditor,
                        public juce::AudioProcessorValueTreeState::Listener {
public:
    RevithionEditor(RevithionProcessor& p) : AudioProcessorEditor(p), proc(p) {
        proc.apvts.addParameterListener("gain", this);
    }
    ~RevithionEditor() override {
        proc.apvts.removeParameterListener("gain", this);  // always clean up
    }
    void parameterChanged(const juce::String& id, float newValue) override {
        if (id == "gain")
            gainLabel.setText(juce::String(newValue, 1) + " dB", juce::dontSendNotification);
    }
private:
    RevithionProcessor& proc;
    juce::Label gainLabel;
};
```

## 4. UI Attachments
Attachments must be **destroyed before** the component — declare components first,
attachments second (C++ reverse destruction order).
```cpp
using SliderAttachment   = juce::AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment   = juce::AudioProcessorValueTreeState::ButtonAttachment;
using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
juce::Slider       gainSlider;                              // component FIRST
juce::ToggleButton bypassButton;
juce::ComboBox     filterTypeBox;
std::unique_ptr<SliderAttachment>   gainAtt;                // attachment SECOND
std::unique_ptr<ButtonAttachment>   bypassAtt;
std::unique_ptr<ComboBoxAttachment> filterAtt;
// In constructor, after addAndMakeVisible:
gainAtt   = std::make_unique<SliderAttachment>(apvts, "gain", gainSlider);
bypassAtt = std::make_unique<ButtonAttachment>(apvts, "bypass", bypassButton);
```

## 5. Thread-Safe Parameter Access
`getRawParameterValue()` returns `std::atomic<float>*` — the **only** safe read path
on the audio thread. Never call `getParameter()->getValue()` in `processBlock`.
```cpp
std::atomic<float>* gainParam = nullptr;
void prepareToPlay(double, int) override {
    gainParam = apvts.getRawParameterValue("gain");
}
void processBlock(juce::AudioBuffer<float>& buf, juce::MidiBuffer&) override {
    buf.applyGain(juce::Decibels::decibelsToGain(gainParam->load()));
}
```

## 6. Parameter Smoothing
Wrap `SmoothedValue` around atomic reads to eliminate zipper noise.
```cpp
juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothGain;
void prepareToPlay(double sr, int) override { smoothGain.reset(sr, 0.02); }
void processBlock(juce::AudioBuffer<float>& buf, juce::MidiBuffer&) override {
    smoothGain.setTargetValue(juce::Decibels::decibelsToGain(gainParam->load()));
    for (int s = 0; s < buf.getNumSamples(); ++s)
        for (int ch = 0; ch < buf.getNumChannels(); ++ch)
            buf.setSample(ch, s, buf.getSample(ch, s) * smoothGain.getNextValue());
}
```

## 7. Parameter Groups
Organize into collapsible hierarchies for large plugins like REVITHION.
```cpp
static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    auto eq = std::make_unique<juce::AudioProcessorParameterGroup>("eq", "EQ", "|",
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"eqLow", 1}, "Low", -12.f, 12.f, 0.f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"eqHigh", 1}, "High", -12.f, 12.f, 0.f));
    auto comp = std::make_unique<juce::AudioProcessorParameterGroup>("comp", "Compressor", "|",
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"compThresh", 1}, "Threshold",
            juce::NormalisableRange<float>(-60.f, 0.f, 0.1f), -20.f));
    return {std::move(eq), std::move(comp)};
}
```

## 8. Undo / Redo
Pass `UndoManager*` to the APVTS constructor; every parameter gesture is recorded.
```cpp
// In processor: apvts(*this, &undoManager, "PARAMETERS", createParameterLayout())
// In editor — wire Cmd+Z / Ctrl+Z:
bool keyPressed(const juce::KeyPress& key) override {
    auto cmd = juce::ModifierKeys::commandModifier;
    if (key == juce::KeyPress('z', cmd, 0))
    { proc.getUndoManager().undo(); return true; }
    if (key == juce::KeyPress('z', cmd | juce::ModifierKeys::shiftModifier, 0))
    { proc.getUndoManager().redo(); return true; }
    return false;
}
```

## Anti-Patterns
| ❌ Don't | ✅ Do Instead |
|----------|--------------|
| `getParameter()->getValue()` on audio thread | `getRawParameterValue()->load()` |
| Declare attachment before its component | Component first, attachment second |
| Forget `removeParameterListener` in destructor | Always clean up listeners |
| Apply raw atomic values without smoothing | Wrap in `SmoothedValue` |
| Scatter string IDs across the codebase | Define `constexpr` ID constants in a header |
| Skip `NormalisableRange` skew for frequency | Use skew 0.2–0.3 for log scaling |

## Checklist
- [ ] All parameters created in `createParameterLayout()` static method
- [ ] `NormalisableRange` skew applied to frequency / logarithmic parameters
- [ ] `UndoManager` passed to APVTS constructor
- [ ] Audio thread reads only via `getRawParameterValue()->load()`
- [ ] `SmoothedValue` wraps every continuously-variable parameter in `processBlock`
- [ ] UI attachments declared after their corresponding components
- [ ] All APVTS listeners removed in the destructor
- [ ] Parameters organized into `AudioProcessorParameterGroup` hierarchies
