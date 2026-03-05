#include "RevithionLookAndFeel.h"

namespace revithion::plugins {

RevithionLookAndFeel::RevithionLookAndFeel()
{
    // Configure base colour scheme
    setColour(juce::ResizableWindow::backgroundColourId,
              juce::Colour(kBackground));
    setColour(juce::Label::textColourId,
              juce::Colour(kText));
    setColour(juce::TextEditor::backgroundColourId,
              juce::Colour(kSurface));
    setColour(juce::TextEditor::textColourId,
              juce::Colour(kText));
    setColour(juce::TextEditor::outlineColourId,
              juce::Colour(kAccent).withAlpha(0.3f));
    setColour(juce::ComboBox::backgroundColourId,
              juce::Colour(kSurface));
    setColour(juce::ComboBox::textColourId,
              juce::Colour(kText));
    setColour(juce::ComboBox::outlineColourId,
              juce::Colour(kAccent).withAlpha(0.4f));
    setColour(juce::PopupMenu::backgroundColourId,
              juce::Colour(kPanel));
    setColour(juce::PopupMenu::textColourId,
              juce::Colour(kText));
    setColour(juce::PopupMenu::highlightedBackgroundColourId,
              juce::Colour(kAccent).withAlpha(0.25f));
    setColour(juce::ScrollBar::thumbColourId,
              juce::Colour(kAccent).withAlpha(0.4f));
}

// ── Rotary Slider ─────────────────────────────────────────────────────────
// Dark arc background with cyan accent indicator and value dot.

void RevithionLookAndFeel::drawRotarySlider(
    juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
    juce::Slider& slider)
{
    (void)slider;

    const float radius = static_cast<float>(juce::jmin(width, height)) * 0.5f - 4.0f;
    const float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
    const float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle
                        + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Background arc
    juce::Path bgArc;
    bgArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                         rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(juce::Colour(kSurface));
    g.strokePath(bgArc, juce::PathStrokeType(4.0f,
                 juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Value arc (accent colour)
    juce::Path valueArc;
    valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                            rotaryStartAngle, angle, true);
    g.setColour(juce::Colour(kAccent));
    g.strokePath(valueArc, juce::PathStrokeType(4.0f,
                 juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Centre knob body
    const float innerRadius = radius * 0.6f;
    g.setColour(juce::Colour(kPanel));
    g.fillEllipse(centreX - innerRadius, centreY - innerRadius,
                  innerRadius * 2.0f, innerRadius * 2.0f);
    g.setColour(juce::Colour(kAccent).withAlpha(0.3f));
    g.drawEllipse(centreX - innerRadius, centreY - innerRadius,
                  innerRadius * 2.0f, innerRadius * 2.0f, 1.5f);

    // Indicator dot
    const float dotRadius = 3.0f;
    const float dotDistance = innerRadius * 0.7f;
    const float dotX = centreX + dotDistance * std::cos(angle - juce::MathConstants<float>::halfPi);
    const float dotY = centreY + dotDistance * std::sin(angle - juce::MathConstants<float>::halfPi);
    g.setColour(juce::Colour(kAccent));
    g.fillEllipse(dotX - dotRadius, dotY - dotRadius,
                  dotRadius * 2.0f, dotRadius * 2.0f);
}

// ── Linear Slider ─────────────────────────────────────────────────────────
// Dark track with a gradient accent fill and rounded thumb.

void RevithionLookAndFeel::drawLinearSlider(
    juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    juce::Slider::SliderStyle style, juce::Slider& slider)
{
    (void)minSliderPos;
    (void)maxSliderPos;

    const bool isVertical =
        (style == juce::Slider::LinearVertical
         || style == juce::Slider::LinearBarVertical);

    const float trackWidth = 4.0f;

    if (isVertical) {
        const float trackX = static_cast<float>(x) + static_cast<float>(width) * 0.5f
                             - trackWidth * 0.5f;
        const float trackY = static_cast<float>(y);
        const float trackH = static_cast<float>(height);

        // Background track
        g.setColour(juce::Colour(kSurface));
        g.fillRoundedRectangle(trackX, trackY, trackWidth, trackH, 2.0f);

        // Filled portion
        const float fillHeight = static_cast<float>(y + height) - sliderPos;
        g.setGradientFill(juce::ColourGradient(
            juce::Colour(kAccent), trackX, sliderPos,
            juce::Colour(kAccent).withAlpha(0.4f), trackX,
            static_cast<float>(y + height), false));
        g.fillRoundedRectangle(trackX, sliderPos, trackWidth, fillHeight, 2.0f);

        // Thumb
        const float thumbSize = 12.0f;
        g.setColour(juce::Colour(kAccent));
        g.fillEllipse(static_cast<float>(x) + static_cast<float>(width) * 0.5f
                          - thumbSize * 0.5f,
                      sliderPos - thumbSize * 0.5f, thumbSize, thumbSize);
    } else {
        const float trackY = static_cast<float>(y) + static_cast<float>(height) * 0.5f
                             - trackWidth * 0.5f;
        const float trackX = static_cast<float>(x);
        const float trackW = static_cast<float>(width);

        // Background track
        g.setColour(juce::Colour(kSurface));
        g.fillRoundedRectangle(trackX, trackY, trackW, trackWidth, 2.0f);

        // Filled portion
        const float fillWidth = sliderPos - static_cast<float>(x);
        g.setGradientFill(juce::ColourGradient(
            juce::Colour(kAccent).withAlpha(0.4f), trackX, trackY,
            juce::Colour(kAccent), sliderPos, trackY, false));
        g.fillRoundedRectangle(trackX, trackY, fillWidth, trackWidth, 2.0f);

        // Thumb
        const float thumbSize = 12.0f;
        g.setColour(juce::Colour(kAccent));
        g.fillEllipse(sliderPos - thumbSize * 0.5f,
                      static_cast<float>(y) + static_cast<float>(height) * 0.5f
                          - thumbSize * 0.5f,
                      thumbSize, thumbSize);
    }
}

// ── Button ────────────────────────────────────────────────────────────────
// Rounded rectangle with subtle glow on hover/press.

void RevithionLookAndFeel::drawButtonBackground(
    juce::Graphics& g, juce::Button& button,
    const juce::Colour& /*backgroundColour*/,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    const auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
    const float cornerSize = 6.0f;

    if (shouldDrawButtonAsDown) {
        g.setColour(juce::Colour(kAccent).withAlpha(0.35f));
    } else if (shouldDrawButtonAsHighlighted) {
        g.setColour(juce::Colour(kAccent).withAlpha(0.18f));
    } else {
        g.setColour(juce::Colour(kSurface));
    }

    g.fillRoundedRectangle(bounds, cornerSize);

    // Border
    g.setColour(shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown
                    ? juce::Colour(kAccent).withAlpha(0.6f)
                    : juce::Colour(kAccent).withAlpha(0.2f));
    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
}

// ── ComboBox ──────────────────────────────────────────────────────────────
// Dark dropdown with accent coloured border.

void RevithionLookAndFeel::drawComboBox(
    juce::Graphics& g, int width, int height, bool isButtonDown,
    int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
    juce::ComboBox& box)
{
    const auto bounds = juce::Rectangle<float>(0.0f, 0.0f,
                                               static_cast<float>(width),
                                               static_cast<float>(height));
    const float cornerSize = 4.0f;

    g.setColour(juce::Colour(kSurface));
    g.fillRoundedRectangle(bounds, cornerSize);

    g.setColour(isButtonDown ? juce::Colour(kAccent)
                             : juce::Colour(kAccent).withAlpha(0.4f));
    g.drawRoundedRectangle(bounds.reduced(0.5f), cornerSize, 1.0f);

    // Arrow indicator
    const float arrowSize = 6.0f;
    const float arrowX = static_cast<float>(width) - 18.0f;
    const float arrowY = static_cast<float>(height) * 0.5f;

    juce::Path arrow;
    arrow.addTriangle(arrowX, arrowY - arrowSize * 0.5f,
                      arrowX + arrowSize, arrowY - arrowSize * 0.5f,
                      arrowX + arrowSize * 0.5f, arrowY + arrowSize * 0.5f);
    g.setColour(juce::Colour(kTextDim));
    g.fillPath(arrow);
}

// ── Label ─────────────────────────────────────────────────────────────────
// Clean text with dim colour.

void RevithionLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    if (!label.isBeingEdited()) {
        const auto textArea =
            getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
        const auto alpha = label.isEnabled() ? 1.0f : 0.5f;

        g.setColour(juce::Colour(kText).withAlpha(alpha));
        g.setFont(getLabelFont(label));
        g.drawFittedText(label.getText(), textArea,
                         label.getJustificationType(),
                         juce::jmax(1, static_cast<int>(
                             static_cast<float>(textArea.getHeight())
                             / g.getCurrentFont().getHeight())),
                         label.getMinimumHorizontalScale());
    }
}

// ── Group Component ───────────────────────────────────────────────────────
// Subtle outlined group box with dim text header.

void RevithionLookAndFeel::drawGroupComponentOutline(
    juce::Graphics& g, int width, int height,
    const juce::String& text, const juce::Justification& justification,
    juce::GroupComponent& group)
{
    (void)justification;

    const float textH = 16.0f;
    const float indent = 3.0f;
    const float cornerSize = 5.0f;

    auto bounds = juce::Rectangle<float>(indent, textH,
                                         static_cast<float>(width) - indent * 2.0f,
                                         static_cast<float>(height) - textH - indent);

    g.setColour(juce::Colour(kAccent).withAlpha(0.15f));
    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);

    // Header text
    g.setColour(juce::Colour(kTextDim));
    g.setFont(juce::Font(13.0f));
    g.drawText(text, juce::Rectangle<float>(
                   indent + 8.0f, 0.0f,
                   static_cast<float>(width) - indent * 2.0f - 16.0f, textH),
               juce::Justification::centredLeft, true);
}

// ── Fonts ─────────────────────────────────────────────────────────────────

juce::Font RevithionLookAndFeel::getLabelFont(juce::Label& label)
{
    (void)label;
    return juce::Font(14.0f);
}

juce::Font RevithionLookAndFeel::getComboBoxFont(juce::ComboBox& box)
{
    (void)box;
    return juce::Font(14.0f);
}

}  // namespace revithion::plugins
