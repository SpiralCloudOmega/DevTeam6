/*  ═══════════════════════════════════════════════════════════════════════════
    REVITHION STUDIO — SettingsPanel implementation
    ═══════════════════════════════════════════════════════════════════════════ */

#include "SettingsPanel.h"

using namespace revithion::settings;

namespace revithion::ui {

// ── Theme constants (consistent with RevithionLookAndFeel) ──────────────────
namespace colours {
    static const juce::Colour background   { 0xFF121220 };
    static const juce::Colour surface      { 0xFF1A1A2E };
    static const juce::Colour panel        { 0xFF16162A };
    static const juce::Colour accent       { 0xFF00D4FF };
    static const juce::Colour secondary    { 0xFFFF6B35 };
    static const juce::Colour success      { 0xFF00FF88 };
    static const juce::Colour error        { 0xFFFF4466 };
    static const juce::Colour text         { 0xFFE0E0F0 };
    static const juce::Colour textDim      { 0xFF8888AA };
    static const juce::Colour recommended  { 0xFF00D4FF };  // cyan highlight for ★
}

// ═════════════════════════════════════════════════════════════════════════════
// Construction / Destruction
// ═════════════════════════════════════════════════════════════════════════════

SettingsPanel::SettingsPanel(AudioSettings& settings)
    : settings_(settings),
      stagedRate_(settings.getSampleRate()),
      stagedBits_(settings.getBitDepth()),
      stagedBuf_(settings.getBufferSize())
{
    // ── Title ───────────────────────────────────────────────────────────
    titleLabel_.setFont(juce::FontOptions(22.0f, juce::Font::bold));
    titleLabel_.setColour(juce::Label::textColourId, colours::accent);
    titleLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel_);

    // ── Section labels ──────────────────────────────────────────────────
    for (auto* label : { &sampleRateLabel_, &bitDepthLabel_,
                         &bufferSizeLabel_, &presetLabel_ }) {
        label->setFont(juce::FontOptions(14.0f, juce::Font::bold));
        label->setColour(juce::Label::textColourId, colours::text);
        addAndMakeVisible(label);
    }

    // ── Combo boxes ─────────────────────────────────────────────────────
    for (auto* combo : { &sampleRateCombo_, &bitDepthCombo_,
                         &bufferSizeCombo_, &presetCombo_ }) {
        combo->setColour(juce::ComboBox::backgroundColourId, colours::surface);
        combo->setColour(juce::ComboBox::textColourId, colours::text);
        combo->setColour(juce::ComboBox::outlineColourId, colours::textDim);
        combo->addListener(this);
        addAndMakeVisible(combo);
    }

    populateCombos();

    // ── Latency readout ─────────────────────────────────────────────────
    latencyLabel_.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    latencyLabel_.setColour(juce::Label::textColourId, colours::accent);
    latencyLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(latencyLabel_);

    // ── Warning label (hidden unless latency > 10 ms) ───────────────────
    warningLabel_.setFont(juce::FontOptions(13.0f, juce::Font::bold));
    warningLabel_.setColour(juce::Label::textColourId, colours::error);
    warningLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(warningLabel_);

    // ── Optimal indicator ───────────────────────────────────────────────
    optimalLabel_.setFont(juce::FontOptions(13.0f));
    optimalLabel_.setColour(juce::Label::textColourId, colours::success);
    optimalLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(optimalLabel_);

    // ── Description (multi-line) ────────────────────────────────────────
    descriptionLabel_.setFont(juce::FontOptions(12.0f));
    descriptionLabel_.setColour(juce::Label::textColourId, colours::textDim);
    descriptionLabel_.setJustificationType(juce::Justification::centredLeft);
    descriptionLabel_.setMinimumHorizontalScale(1.0f);
    addAndMakeVisible(descriptionLabel_);

    // ── Buttons ─────────────────────────────────────────────────────────
    applyButton_.setColour(juce::TextButton::buttonColourId, colours::accent);
    applyButton_.setColour(juce::TextButton::textColourOnId, colours::background);
    cancelButton_.setColour(juce::TextButton::buttonColourId, colours::surface);
    cancelButton_.setColour(juce::TextButton::textColourOnId, colours::text);
    resetButton_.setColour(juce::TextButton::buttonColourId, colours::secondary);
    resetButton_.setColour(juce::TextButton::textColourOnId, colours::background);

    for (auto* btn : { &applyButton_, &cancelButton_, &resetButton_ }) {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // ── Initial sync ────────────────────────────────────────────────────
    syncFromSettings();

    settings_.addListener(this);
}

SettingsPanel::~SettingsPanel() {
    settings_.removeListener(this);
}

// ═════════════════════════════════════════════════════════════════════════════
// Combo population
// ═════════════════════════════════════════════════════════════════════════════

void SettingsPanel::populateCombos() {
    // ── Sample rate ─────────────────────────────────────────────────────
    sampleRateCombo_.clear(juce::dontSendNotification);
    int id = 1;
    for (auto r : kAllSampleRates)
        sampleRateCombo_.addItem(juce::String(sampleRateLabel(r)), id++);

    // ── Bit depth ───────────────────────────────────────────────────────
    bitDepthCombo_.clear(juce::dontSendNotification);
    id = 1;
    for (auto b : kAllBitDepths)
        bitDepthCombo_.addItem(juce::String(bitDepthLabel(b)), id++);

    // ── Buffer size ─────────────────────────────────────────────────────
    bufferSizeCombo_.clear(juce::dontSendNotification);
    id = 1;
    for (auto s : kAllBufferSizes)
        bufferSizeCombo_.addItem(juce::String(bufferSizeLabel(s)), id++);

    // ── Presets ─────────────────────────────────────────────────────────
    presetCombo_.clear(juce::dontSendNotification);
    presetCombo_.addItem("(Custom)", 1);
    id = 2;
    for (const auto& p : kAllPresets) {
        juce::String label(p.name);
        if (p.recommended) label += juce::String::fromUTF8(" \xe2\x98\x85");  // ★
        presetCombo_.addItem(label, id++);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// Sync UI ↔ settings
// ═════════════════════════════════════════════════════════════════════════════

void SettingsPanel::syncFromSettings() {
    stagedRate_ = settings_.getSampleRate();
    stagedBits_ = settings_.getBitDepth();
    stagedBuf_  = settings_.getBufferSize();

    // Find combo indices (1-based).
    for (size_t i = 0; i < kAllSampleRates.size(); ++i)
        if (kAllSampleRates[i] == stagedRate_)
            sampleRateCombo_.setSelectedId(static_cast<int>(i) + 1, juce::dontSendNotification);

    for (size_t i = 0; i < kAllBitDepths.size(); ++i)
        if (kAllBitDepths[i] == stagedBits_)
            bitDepthCombo_.setSelectedId(static_cast<int>(i) + 1, juce::dontSendNotification);

    for (size_t i = 0; i < kAllBufferSizes.size(); ++i)
        if (kAllBufferSizes[i] == stagedBuf_)
            bufferSizeCombo_.setSelectedId(static_cast<int>(i) + 1, juce::dontSendNotification);

    // Try to match a preset.
    int presetId = 1;  // "(Custom)" by default
    for (size_t i = 0; i < kAllPresets.size(); ++i) {
        const auto& p = kAllPresets[i];
        if (p.sampleRate == stagedRate_ && p.bitDepth == stagedBits_ && p.bufferSize == stagedBuf_) {
            presetId = static_cast<int>(i) + 2;  // +2 because "(Custom)" is id 1
            break;
        }
    }
    presetCombo_.setSelectedId(presetId, juce::dontSendNotification);

    syncLatencyDisplay();
}

void SettingsPanel::syncLatencyDisplay() {
    float lat = latencyMs(stagedBuf_, stagedRate_);
    float rt  = roundTripLatencyMs(stagedBuf_, stagedRate_);
    float nyq = nyquistHz(stagedRate_);

    latencyLabel_.setText(
        juce::String::formatted("Latency: %.2f ms  |  Round-trip: %.2f ms  |  Nyquist: %.1f kHz",
                                lat, rt, nyq / 1000.0f),
        juce::dontSendNotification);

    // Warning for high latency
    if (lat > 10.0f) {
        warningLabel_.setText(
            juce::String::fromUTF8("\xe2\x9a\xa0 High latency — not recommended for real-time instruments or spatial audio"),
            juce::dontSendNotification);
        warningLabel_.setVisible(true);
    } else {
        warningLabel_.setVisible(false);
    }

    // Optimal indicator
    bool optimal = isOptimalForSpatialAudio(stagedRate_);
    if (optimal) {
        optimalLabel_.setText(
            juce::String::fromUTF8("\xe2\x9c\x93 Optimal for your hardware — 96 kHz is the sweet spot for spatial 3D audio & instruments"),
            juce::dontSendNotification);
        optimalLabel_.setVisible(true);
    } else {
        optimalLabel_.setVisible(false);
    }

    // Description of selected sample rate
    descriptionLabel_.setText(
        juce::String(sampleRateDescription(stagedRate_)),
        juce::dontSendNotification);
}

void SettingsPanel::applyToSettings() {
    settings_.applyAll(stagedRate_, stagedBits_, stagedBuf_);
}

void SettingsPanel::cancelChanges() {
    syncFromSettings();
}

void SettingsPanel::resetToDefaults() {
    settings_.resetToDefaults();
    // syncFromSettings() will be called via the listener callback.
}

// ═════════════════════════════════════════════════════════════════════════════
// Callbacks
// ═════════════════════════════════════════════════════════════════════════════

void SettingsPanel::audioSettingsChanged(const AudioSettings& /*s*/) {
    // Called when the AudioSettings object is modified externally.
    juce::MessageManager::callAsync([this] { syncFromSettings(); });
}

void SettingsPanel::comboBoxChanged(juce::ComboBox* combo) {
    if (combo == &sampleRateCombo_) {
        int idx = combo->getSelectedId() - 1;
        if (idx >= 0 && idx < static_cast<int>(kAllSampleRates.size()))
            stagedRate_ = kAllSampleRates[static_cast<size_t>(idx)];
        // Reset preset to "(Custom)" since user changed manually.
        presetCombo_.setSelectedId(1, juce::dontSendNotification);
    }
    else if (combo == &bitDepthCombo_) {
        int idx = combo->getSelectedId() - 1;
        if (idx >= 0 && idx < static_cast<int>(kAllBitDepths.size()))
            stagedBits_ = kAllBitDepths[static_cast<size_t>(idx)];
        presetCombo_.setSelectedId(1, juce::dontSendNotification);
    }
    else if (combo == &bufferSizeCombo_) {
        int idx = combo->getSelectedId() - 1;
        if (idx >= 0 && idx < static_cast<int>(kAllBufferSizes.size()))
            stagedBuf_ = kAllBufferSizes[static_cast<size_t>(idx)];
        presetCombo_.setSelectedId(1, juce::dontSendNotification);
    }
    else if (combo == &presetCombo_) {
        int idx = combo->getSelectedId() - 2;  // -2 because "(Custom)" is id 1
        if (idx >= 0 && idx < static_cast<int>(kAllPresets.size())) {
            const auto& preset = kAllPresets[static_cast<size_t>(idx)];
            stagedRate_ = preset.sampleRate;
            stagedBits_ = preset.bitDepth;
            stagedBuf_  = preset.bufferSize;

            // Update the individual combos to reflect the preset.
            for (size_t i = 0; i < kAllSampleRates.size(); ++i)
                if (kAllSampleRates[i] == stagedRate_)
                    sampleRateCombo_.setSelectedId(static_cast<int>(i) + 1, juce::dontSendNotification);

            for (size_t i = 0; i < kAllBitDepths.size(); ++i)
                if (kAllBitDepths[i] == stagedBits_)
                    bitDepthCombo_.setSelectedId(static_cast<int>(i) + 1, juce::dontSendNotification);

            for (size_t i = 0; i < kAllBufferSizes.size(); ++i)
                if (kAllBufferSizes[i] == stagedBuf_)
                    bufferSizeCombo_.setSelectedId(static_cast<int>(i) + 1, juce::dontSendNotification);
        }
    }

    syncLatencyDisplay();
}

void SettingsPanel::buttonClicked(juce::Button* button) {
    if (button == &applyButton_)  applyToSettings();
    if (button == &cancelButton_) cancelChanges();
    if (button == &resetButton_)  resetToDefaults();
}

// ═════════════════════════════════════════════════════════════════════════════
// Paint / Layout
// ═════════════════════════════════════════════════════════════════════════════

void SettingsPanel::paint(juce::Graphics& g) {
    g.fillAll(colours::background);

    // Section dividers
    auto bounds = getLocalBounds().reduced(20);
    g.setColour(colours::surface);
    g.fillRoundedRectangle(bounds.toFloat(), 8.0f);

    // Inner panel background
    auto inner = bounds.reduced(12);
    g.setColour(colours::panel);
    g.fillRoundedRectangle(inner.toFloat(), 6.0f);
}

void SettingsPanel::resized() {
    auto area = getLocalBounds().reduced(32);

    // ── Title ───────────────────────────────────────────────────────────
    titleLabel_.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    // ── Row height and label width ──────────────────────────────────────
    const int rowH = 32;
    const int labelW = 110;
    const int gap = 10;

    // ── Preset ──────────────────────────────────────────────────────────
    {
        auto row = area.removeFromTop(rowH);
        presetLabel_.setBounds(row.removeFromLeft(labelW));
        row.removeFromLeft(gap);
        presetCombo_.setBounds(row);
    }
    area.removeFromTop(gap);

    // ── Sample Rate ─────────────────────────────────────────────────────
    {
        auto row = area.removeFromTop(rowH);
        sampleRateLabel_.setBounds(row.removeFromLeft(labelW));
        row.removeFromLeft(gap);
        sampleRateCombo_.setBounds(row);
    }
    area.removeFromTop(gap);

    // ── Bit Depth ───────────────────────────────────────────────────────
    {
        auto row = area.removeFromTop(rowH);
        bitDepthLabel_.setBounds(row.removeFromLeft(labelW));
        row.removeFromLeft(gap);
        bitDepthCombo_.setBounds(row);
    }
    area.removeFromTop(gap);

    // ── Buffer Size ─────────────────────────────────────────────────────
    {
        auto row = area.removeFromTop(rowH);
        bufferSizeLabel_.setBounds(row.removeFromLeft(labelW));
        row.removeFromLeft(gap);
        bufferSizeCombo_.setBounds(row);
    }
    area.removeFromTop(16);

    // ── Latency readout ─────────────────────────────────────────────────
    latencyLabel_.setBounds(area.removeFromTop(28));
    area.removeFromTop(4);

    // ── Warning / Optimal ───────────────────────────────────────────────
    warningLabel_.setBounds(area.removeFromTop(22));
    optimalLabel_.setBounds(area.removeFromTop(22));
    area.removeFromTop(8);

    // ── Description ─────────────────────────────────────────────────────
    descriptionLabel_.setBounds(area.removeFromTop(80));
    area.removeFromTop(16);

    // ── Buttons ─────────────────────────────────────────────────────────
    {
        auto row = area.removeFromTop(36);
        const int btnW = 140;
        const int btnGap = 12;
        auto center = row.withSizeKeepingCentre(btnW * 3 + btnGap * 2, 36);

        applyButton_.setBounds(center.removeFromLeft(btnW));
        center.removeFromLeft(btnGap);
        cancelButton_.setBounds(center.removeFromLeft(btnW));
        center.removeFromLeft(btnGap);
        resetButton_.setBounds(center.removeFromLeft(btnW));
    }
}

} // namespace revithion::ui
