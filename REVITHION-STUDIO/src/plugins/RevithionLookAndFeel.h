#pragma once

#include <JuceHeader.h>

namespace revithion::plugins {

// Custom JUCE LookAndFeel for all exported REVITHION plugins.
// Provides a professional dark theme consistent with the REVITHION Studio UI.
class RevithionLookAndFeel : public juce::LookAndFeel_V4 {
public:
    RevithionLookAndFeel();

    // ── Theme Colors ──────────────────────────────────────────────────────
    static constexpr uint32_t kBackground = 0xFF121220;
    static constexpr uint32_t kSurface    = 0xFF1A1A2E;
    static constexpr uint32_t kPanel      = 0xFF16162A;
    static constexpr uint32_t kAccent     = 0xFF00D4FF;
    static constexpr uint32_t kSecondary  = 0xFFFF6B35;
    static constexpr uint32_t kSuccess    = 0xFF00FF88;
    static constexpr uint32_t kError      = 0xFFFF4466;
    static constexpr uint32_t kText       = 0xFFE0E0F0;
    static constexpr uint32_t kTextDim    = 0xFF8888AA;

    // ── Component Draw Overrides ──────────────────────────────────────────

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override;

    void drawButtonBackground(juce::Graphics&, juce::Button&,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    void drawComboBox(juce::Graphics&, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox&) override;

    void drawLabel(juce::Graphics&, juce::Label&) override;

    void drawGroupComponentOutline(juce::Graphics&, int width, int height,
                                   const juce::String& text,
                                   const juce::Justification&,
                                   juce::GroupComponent&) override;

    // ── Font Overrides ────────────────────────────────────────────────────

    juce::Font getLabelFont(juce::Label&) override;
    juce::Font getComboBoxFont(juce::ComboBox&) override;
};

}  // namespace revithion::plugins
