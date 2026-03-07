---
name: juce-component-ui
description: JUCE Component system for professional DAW/plugin UIs — layout, painting, interaction, accessibility for REVITHION STUDIO.
---
# JUCE Component UI — Audio Interface Development
Every visible element in REVITHION STUDIO inherits `juce::Component`. Override `paint()` to draw, `resized()` to position children. Call `repaint()` to schedule redraws — never invoke `paint()` directly.

## paint() and resized()
```cpp
class ChannelMeter : public juce::Component {
    float level = 0.0f;
    void paint(Graphics& g) override {
        auto b = getLocalBounds().toFloat(); g.fillAll(Colours::black);
        auto filled = b.reduced(2).removeFromBottom(b.getHeight() * level);
        g.setGradientFill({ Colours::green, filled.getBottomLeft(), Colours::red, filled.getTopLeft(), false });
        g.fillRect(filled);
    }
    void resized() override { /* position child labels */ }
public:
    void setLevel(float v) { level = jlimit(0.0f, 1.0f, v); repaint(); }
};
```
## FlexBox Layout — Mixer Strip
```cpp
void MixerStrip::resized() {
    FlexBox fb; fb.flexDirection = FlexBox::Direction::column;
    fb.items.add(FlexItem(panKnob).withHeight(60.0f));
    fb.items.add(FlexItem(vuMeter).withFlex(1.0f));
    fb.items.add(FlexItem(faderSlider).withFlex(2.0f));
    fb.items.add(FlexItem(muteButton).withHeight(28.0f));
    fb.performLayout(getLocalBounds().reduced(4));
}
```
## Grid Layout — Editor Zones
```cpp
void MainEditor::resized() {
    Grid grid; using Tr = Grid::TrackInfo; using Fr = Grid::Fr; using Px = Grid::Px;
    grid.templateRows    = { Tr(Px(40)), Tr(Fr(1)), Tr(Px(200)) };
    grid.templateColumns = { Tr(Px(220)), Tr(Fr(1)), Tr(Px(300)) };
    grid.items = { GridItem(toolbar).withArea(1,1,2,4), GridItem(browser).withArea(2,1,3,2),
                   GridItem(arrangement).withArea(2,2,3,3), GridItem(mixer).withArea(3,1,4,4) };
    grid.performLayout(getLocalBounds());
}
```
## Viewport, Tabs, TreeView, ListBox
```cpp
viewport.setViewedComponent(&content, false);       // Viewport — scrollable areas
viewport.setScrollBarsShown(true, false); addAndMakeVisible(viewport);
addTab("Mix", bg, new MixerPanel(), true);           // TabbedComponent — section tabs
addTab("Edit", bg, new ArrangementPanel(), true);
bool mightContainSubItems() override { return file.isDirectory(); } // TreeView item
void itemOpened() override {
    for (auto c : file.findChildFiles(File::findFilesAndDirectories, false)) addSubItem(new FileTreeItem(c));
}
void paintListBoxItem(int row, Graphics& g, int w, int h, bool sel) override { // ListBoxModel
    if (sel) g.fillAll(Colour(0xFF3A3A5C));
    g.setColour(Colours::white); g.drawText(presets[row], 8, 0, w-8, h, Justification::centredLeft);
}
```
## Custom Waveform Display
```cpp
class WaveformDisplay : public Component, private ChangeListener {
    AudioThumbnail* thumb = nullptr;
    void changeListenerCallback(ChangeBroadcaster*) override { repaint(); }
public:
    void setSource(AudioThumbnail& t) { thumb = &t; t.addChangeListener(this); repaint(); }
    void paint(Graphics& g) override {
        g.fillAll(Colour(0xFF121212));
        if (thumb && thumb->getTotalLength() > 0) thumb->drawChannels(g, getLocalBounds(), 0.0, thumb->getTotalLength(), 1.0f);
    }
};
```
## Mouse, Keyboard, Drag-and-Drop
```cpp
class XYPad : public Component {
    float xVal = 0.5f, yVal = 0.5f;
    void updateXY(const MouseEvent& e) { xVal = (float)e.x / getWidth(); yVal = 1.f - (float)e.y / getHeight(); repaint(); }
    void mouseDown(const MouseEvent& e) override { updateXY(e); }
    void mouseDrag(const MouseEvent& e) override { updateXY(e); }
    void mouseWheelMove(const MouseEvent&, const MouseWheelDetails& w) override {
        yVal = jlimit(0.0f, 1.0f, yVal - w.deltaY * 0.05f); repaint();
    }
    bool keyPressed(const KeyPress& k) override {
        return (k == KeyPress::escapeKey) ? (xVal = yVal = 0.5f, repaint(), true) : false;
    }
};
// DragAndDrop: top-level inherits DragAndDropContainer
class PluginSlot : public Component, public DragAndDropTarget {
    bool isInterestedInDragSource(const SourceDetails& d) override { return d.description.toString().startsWith("plugin:"); }
    void itemDropped(const SourceDetails& d) override { /* load plugin */ }
    void mouseDrag(const MouseEvent&) override {
        if (auto* c = DragAndDropContainer::findParentDragContainerFor(this)) c->startDragging("plugin:" + pluginId, this);
    }
};
```
## Tooltips and Accessibility
```cpp
class ParamKnob : public Slider, public TooltipClient {
    String getTooltip() override { return getName() + ": " + String(getValue(), 2); }
};
// TooltipWindow tooltipWindow { this, 400 };   // top-level
// component.setTitle("Master Volume"); component.setDescription("Controls output level");
```
## Anti-Patterns
- **Painting outside `paint()`** — use `repaint()` to schedule; never call Graphics elsewhere
- **Blocking the message thread** — `paint()`/`resized()` must be fast; offload heavy work
- **Hardcoded pixel positions** — use FlexBox/Grid or proportional bounds
- **Forgetting `addAndMakeVisible()`** — children stay invisible until registered
- **`repaint()` at audio rate** — throttle to ~30 Hz via Timer, not per-sample

## Checklist
- [ ] Every component overrides both `paint()` and `resized()`
- [ ] Layout uses FlexBox or Grid — no magic pixel values
- [ ] `repaint()` throttled via Timer, never from audio thread
- [ ] `addAndMakeVisible()` called for all child components
- [ ] DragAndDropContainer at the top-level window
- [ ] TooltipClient on all interactive controls
- [ ] Accessibility titles on primary navigation components
- [ ] Viewport wraps content exceeding visible area
