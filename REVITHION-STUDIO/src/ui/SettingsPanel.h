#pragma once
/*  ═══════════════════════════════════════════════════════════════════════════
    REVITHION STUDIO — SettingsPanel (UI)
    ═══════════════════════════════════════════════════════════════════════════
    JUCE component that exposes audio-engine configuration to the user:

        • Sample rate selector (ComboBox)
        • Bit depth selector (ComboBox)
        • Buffer size selector (ComboBox)
        • Preset dropdown (ComboBox)
        • Real-time latency readout (updates on every change)
        • Warning label when latency exceeds 10 ms
        • "Optimal for your hardware" indicator
        • Apply / Cancel / Reset to Defaults buttons

    The panel observes AudioSettings via its Listener interface so external
    changes (e.g. from scripts or the engine) are immediately reflected.
    ═══════════════════════════════════════════════════════════════════════════ */

#include <JuceHeader.h>
#include "../settings/AudioSettings.h"
#include "../settings/AudioPresets.h"

namespace revithion::ui {

class SettingsPanel final : public juce::Component,
                            private revithion::settings::AudioSettings::Listener,
                            private juce::ComboBox::Listener,
                            private juce::Button::Listener {
public:
    explicit SettingsPanel(revithion::settings::AudioSettings& settings);
    ~SettingsPanel() override;

    // juce::Component
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // ── Internal helpers ────────────────────────────────────────────────
    void populateCombos();
    void syncFromSettings();
    void syncLatencyDisplay();
    void applyToSettings();
    void cancelChanges();
    void resetToDefaults();

    // ── AudioSettings::Listener ─────────────────────────────────────────
    void audioSettingsChanged(const revithion::settings::AudioSettings& s) override;

    // ── ComboBox::Listener ──────────────────────────────────────────────
    void comboBoxChanged(juce::ComboBox* combo) override;

    // ── Button::Listener ────────────────────────────────────────────────
    void buttonClicked(juce::Button* button) override;

    // ── Data ────────────────────────────────────────────────────────────
    revithion::settings::AudioSettings& settings_;

    // Staged values — written to settings_ only on "Apply".
    revithion::settings::SampleRate stagedRate_;
    revithion::settings::BitDepth   stagedBits_;
    revithion::settings::BufferSize stagedBuf_;

    // ── UI components ───────────────────────────────────────────────────
    juce::Label          titleLabel_        { {}, "Audio Engine Settings" };
    juce::Label          sampleRateLabel_   { {}, "Sample Rate" };
    juce::Label          bitDepthLabel_     { {}, "Bit Depth" };
    juce::Label          bufferSizeLabel_   { {}, "Buffer Size" };
    juce::Label          presetLabel_       { {}, "Preset" };
    juce::Label          latencyLabel_      { {}, "" };
    juce::Label          warningLabel_      { {}, "" };
    juce::Label          optimalLabel_      { {}, "" };
    juce::Label          descriptionLabel_  { {}, "" };

    juce::ComboBox       sampleRateCombo_;
    juce::ComboBox       bitDepthCombo_;
    juce::ComboBox       bufferSizeCombo_;
    juce::ComboBox       presetCombo_;

    juce::TextButton     applyButton_  { "Apply" };
    juce::TextButton     cancelButton_ { "Cancel" };
    juce::TextButton     resetButton_  { "Reset to Defaults" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPanel)
};

} // namespace revithion::ui
