---
name: juce-look-and-feel
description: JUCE LookAndFeel customization for professional audio plugin UIs — knobs, sliders, buttons, themes, OpenGL, and design token integration for REVITHION STUDIO.
---
# JUCE LookAndFeel Customization
Override `LookAndFeel_V4` drawing methods for pixel-perfect audio UIs. All examples target JUCE 7/8.
## drawRotarySlider — Custom Rotary Knobs
```cpp
void drawRotarySlider(Graphics& g, int x, int y, int w, int h,
                      float pos, float startAng, float endAng, Slider&) override {
    auto bounds = Rectangle<int>(x, y, w, h).toFloat().reduced(4.f);
    auto r = jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto c = bounds.getCentre();
    auto angle = startAng + pos * (endAng - startAng);
    g.setColour(Colours::black.withAlpha(0.4f));
    g.fillEllipse(bounds.translated(1.f, 2.f).reduced(2.f));       // shadow
    ColourGradient grad(Colour(0xff3a3a3a), c.x, c.y - r * 0.3f,
                        Colour(0xff1a1a1a), c.x, c.y + r, true);
    g.setGradientFill(grad); g.fillEllipse(bounds.reduced(2.f));    // knob body
    Path arc; arc.addCentredArc(c.x, c.y, r-6, r-6, 0, startAng, angle, true);
    g.setColour(Colour(0xff00b4d8));
    g.strokePath(arc, PathStrokeType(3.f, PathStrokeType::curved)); // value arc
    Path ptr; ptr.addRoundedRectangle(-1.5f, -r*0.55f, 3.f, r*0.55f, 1.f);
    g.setColour(Colours::white);
    g.fillPath(ptr, AffineTransform::rotation(angle).translated(c)); // pointer
}
```
## drawLinearSlider — Track, Fill, Thumb
```cpp
void drawLinearSlider(Graphics& g, int x, int y, int w, int h, float sliderPos,
                      float, float, Slider::SliderStyle style, Slider&) override {
    bool hz = (style == Slider::LinearHorizontal);
    auto track = hz ? Rectangle<float>((float)x, y+h*0.5f-2, (float)w, 4.f)
                    : Rectangle<float>(x+w*0.5f-2, (float)y, 4.f, (float)h);
    g.setColour(Colour(0xff2a2a2a)); g.fillRoundedRectangle(track, 2.f);
    g.setColour(Colour(0xff00b4d8));
    g.fillRoundedRectangle(hz ? track.withWidth(sliderPos-x) : track.withTop(sliderPos), 2.f);
    auto tc = hz ? Point<float>(sliderPos, track.getCentreY())
                 : Point<float>(track.getCentreX(), sliderPos);
    g.setColour(Colour(0xffe0e0e0));
    g.fillEllipse(Rectangle<float>(14.f, 14.f).withCentre(tc));
}
```
## drawButtonBackground — Normal / Hover / Down / Toggled
```cpp
void drawButtonBackground(Graphics& g, Button& b, const Colour&, bool over, bool down) override {
    auto bounds = b.getLocalBounds().toFloat().reduced(1.f);
    bool on = b.getToggleState();
    Colour base = on ? Colour(0xff00b4d8) : Colour(0xff2a2a2a);
    if (down) base = base.brighter(0.15f); else if (over) base = base.brighter(0.08f);
    g.setColour(base); g.fillRoundedRectangle(bounds, tokens.cornerRadius);
    g.setColour(on ? Colour(0xff0090b0) : Colour(0xff444444));
    g.drawRoundedRectangle(bounds, tokens.cornerRadius, 1.f);
}
```
## Custom Fonts & Dark Color Scheme
```cpp
RevithionLookAndFeel() {
    mainFont = Typeface::createSystemTypefaceFor(BinaryData::InterMedium_ttf,
                                                  BinaryData::InterMedium_ttfSize);
    setColour(ResizableWindow::backgroundColourId, Colour(0xff121212));
    setColour(Slider::rotarySliderFillColourId,    Colour(0xff00b4d8));
    setColour(Slider::trackColourId,               Colour(0xff1e1e1e));
    setColour(TextButton::buttonColourId,          Colour(0xff2a2a2a));
    setColour(TextButton::buttonOnColourId,        Colour(0xff00b4d8));
    setColour(Label::textColourId,                 Colour(0xffe0e0e0));
    setColour(PopupMenu::backgroundColourId,       Colour(0xff1a1a1a));
}
Font getLabelFont(Label&) override { return Font(mainFont).withHeight(13.f); }
```
## Scalable UI & OpenGL Rendering
```cpp
// DPI — query display scale, apply with setScaleFactor()
void resized() override {
    setScaleFactor((float)Desktop::getInstance().getDisplays().getPrimaryDisplay()->scale);
}
// OpenGL — attach only to high-refresh components (spectrum analyzers, meters, waveforms)
struct SpectrumPanel : Component, OpenGLRenderer {
    SpectrumPanel()  { gl.setRenderer(this); gl.attachTo(*this); }
    ~SpectrumPanel() { gl.detach(); }
    void newOpenGLContextCreated() override {}
    void openGLContextClosing() override {}
    void renderOpenGL() override { OpenGLHelpers::clear(Colour(0xff121212)); }
    OpenGLContext gl;
};
```
## Procedural Drawing — VU Meter
```cpp
void drawVUMeter(Graphics& g, Rectangle<float> b, float level) {
    g.setColour(Colour(0xff1a1a1a)); g.fillRoundedRectangle(b, 3.f);
    auto fill = b.removeFromBottom(b.getHeight() * jlimit(0.f, 1.f, level));
    ColourGradient gr(Colours::limegreen, fill.getBottomLeft(),
                      Colours::red, fill.getTopLeft(), false);
    gr.addColour(0.6, Colours::yellow);
    g.setGradientFill(gr); g.fillRoundedRectangle(fill, 3.f);
}
```
## Design Tokens & Theme Switching
Central token struct drives all colours — maps to REVITHION's procedural S-expression token system:
```cpp
struct DesignTokens {
    Colour bgPrimary{0xff121212}, bgSecondary{0xff1e1e1e}, accent{0xff00b4d8};
    Colour textPrimary{0xffe0e0e0}, textMuted{0xff888888}, danger{0xffff4444};
    float cornerRadius = 4.f, spacing = 8.f, knobArcWidth = 3.f;
};
void applyTokens(const DesignTokens& t) {
    setColour(ResizableWindow::backgroundColourId, t.bgPrimary);
    setColour(Slider::rotarySliderFillColourId, t.accent);
    setColour(Label::textColourId, t.textPrimary);
}
void setTheme(bool dark) {
    tokens = dark ? DesignTokens{}
                  : DesignTokens{Colour(0xfff0f0f0), Colour(0xffffffff), Colour(0xff007acc),
                                 Colour(0xff1a1a1a), Colour(0xff666666), Colour(0xffcc0000)};
    applyTokens(tokens);
    if (auto* w = TopLevelWindow::getActiveTopLevelWindow()) w->repaint();
}
```
## Anti-Patterns
- **Hardcoded colours in `paint()`** — use `findColour()` or the token struct so themes propagate.
- **Per-component LookAndFeel instances** — share one via `setDefaultLookAndFeel()`.
- **Fixed pixel sizes** — derive dimensions from bounds; use `reduced()` to prevent edge clipping.
- **Blocking in `renderOpenGL()`** — copy audio data via lock-free FIFO, never hold locks.
- **Raw `new` for Typeface** — use `Typeface::Ptr` (ref-counted) to prevent leaks.
## Checklist
- [ ] Single `LookAndFeel` subclass registered with `setDefaultLookAndFeel()`
- [ ] All colours driven by `DesignTokens`, not hardcoded
- [ ] Fonts loaded via `BinaryData` + `Typeface::createSystemTypefaceFor()`
- [ ] Rotary knob draws arc, gradient body, pointer, and shadow
- [ ] Linear slider handles horizontal and vertical orientations
- [ ] Button states: normal / hover / down / toggled
- [ ] DPI scale applied via `setScaleFactor()` in `resized()`
- [ ] OpenGL only on high-refresh components; `renderOpenGL()` is lock-free
- [ ] Runtime theme switch triggers top-level `repaint()`
- [ ] VU meter uses green → yellow → red `ColourGradient`
