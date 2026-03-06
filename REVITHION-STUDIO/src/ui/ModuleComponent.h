#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../rack/AudioModule.h"

namespace revithion::ui {

//==============================================================================
/** Represents a single input/output jack on a module panel.
    Draws a color-coded circular port with hover/connected state indicators. */
class PortComponent : public juce::Component
{
public:
    explicit PortComponent(const revithion::rack::ModulePort& port)
        : portInfo_(port)
    {
        setSize(kSize, kSize);
        setRepaintsOnMouseActivity(true);
    }

    const revithion::rack::ModulePort& getPortInfo() const { return portInfo_; }

    juce::Point<float> getCenterInParent() const
    {
        auto bounds = getBoundsInParent().toFloat();
        return bounds.getCentre();
    }

    void setConnected(bool connected)
    {
        if (isConnected_ != connected)
        {
            isConnected_ = connected;
            repaint();
        }
    }

    bool isConnected() const { return isConnected_; }

    //--------------------------------------------------------------------------
    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(1.0f);
        auto colour = getPortColour();

        // Outer ring / highlight on hover
        if (isHovered_)
        {
            g.setColour(colour.brighter(0.4f).withAlpha(0.5f));
            g.fillEllipse(area);
        }

        // Jack body
        auto jackArea = area.reduced(2.0f);
        g.setColour(juce::Colour(0xff1a1a1a));
        g.fillEllipse(jackArea);

        // Border ring
        g.setColour(colour);
        g.drawEllipse(jackArea, 1.5f);

        // Filled center when connected
        if (isConnected_)
        {
            auto inner = jackArea.reduced(3.0f);
            g.setColour(colour);
            g.fillEllipse(inner);
        }
    }

    void mouseEnter(const juce::MouseEvent&) override
    {
        isHovered_ = true;
        repaint();
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        isHovered_ = false;
        repaint();
    }

    void mouseDown(const juce::MouseEvent&) override;

private:
    static constexpr int kSize = 16;

    juce::Colour getPortColour() const
    {
        using ST = revithion::rack::ModulePort::SignalType;
        switch (portInfo_.signalType)
        {
            case ST::Audio:     return juce::Colour(0xffff8c00); // orange
            case ST::Sidechain: return juce::Colour(0xffffd700); // yellow
            case ST::CV:        return juce::Colour(0xff00c853); // green
            case ST::MIDI:      return juce::Colour(0xff9c27b0); // purple
            default:            return juce::Colours::grey;
        }
    }

    revithion::rack::ModulePort portInfo_;
    bool isHovered_ = false;
    bool isConnected_ = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PortComponent)
};

//==============================================================================
/** Full module panel in the modular rack.
    Displays a colored header bar with name and controls, input/output ports
    along the sides, and the module's custom editor in the body area. */
class ModuleComponent : public juce::Component,
                        public revithion::rack::AudioModule::Listener
{
public:
    //--------------------------------------------------------------------------
    /** Listener interface so the parent rack view can respond to module events. */
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void moduleClicked(ModuleComponent*) {}
        virtual void moduleDragStarted(ModuleComponent*) {}
        virtual void portDragStarted(ModuleComponent*, uint32_t portId, juce::Point<float> pos) {}
        virtual void portDragEnded(ModuleComponent*, uint32_t portId, juce::Point<float> pos) {}
        virtual void removeRequested(ModuleComponent*) {}
    };

    //--------------------------------------------------------------------------
    explicit ModuleComponent(revithion::rack::AudioModule& module)
        : module_(module)
    {
        module_.addListener(this);

        // Header controls
        nameLabel_.setText(module_.getInfo().displayName, juce::dontSendNotification);
        nameLabel_.setColour(juce::Label::textColourId, juce::Colours::white);
        nameLabel_.setFont(juce::FontOptions(13.0f, juce::Font::bold));
        nameLabel_.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(nameLabel_);

        bypassButton_.setButtonText("B");
        bypassButton_.setTooltip("Bypass");
        bypassButton_.setClickingTogglesState(true);
        bypassButton_.setToggleState(module_.isBypassed(), juce::dontSendNotification);
        bypassButton_.onClick = [this]
        {
            module_.setBypassed(bypassButton_.getToggleState());
        };
        addAndMakeVisible(bypassButton_);

        collapseButton_.setButtonText(collapsed_ ? "+" : "-");
        collapseButton_.setTooltip("Collapse / Expand");
        collapseButton_.onClick = [this] { setCollapsed(!collapsed_); };
        addAndMakeVisible(collapseButton_);

        removeButton_.setButtonText(juce::String::charToString(0x00d7)); // multiplication sign as X
        removeButton_.setTooltip("Remove module");
        removeButton_.onClick = [this] { listeners_.call(&Listener::removeRequested, this); };
        addAndMakeVisible(removeButton_);

        cpuLabel_.setText("0%", juce::dontSendNotification);
        cpuLabel_.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.6f));
        cpuLabel_.setFont(juce::FontOptions(10.0f));
        cpuLabel_.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(cpuLabel_);

        // Create the module's custom editor
        moduleEditor_ = module_.createEditor();
        if (moduleEditor_)
            addAndMakeVisible(*moduleEditor_);

        createPortComponents();

        expandedHeight_ = calculateExpandedHeight();
    }

    ~ModuleComponent() override
    {
        module_.removeListener(this);
    }

    //--------------------------------------------------------------------------
    // Accessors

    revithion::rack::AudioModule& getModule() { return module_; }
    const revithion::rack::AudioModule& getModule() const { return module_; }
    const juce::Uuid& getModuleId() const { return module_.getUuid(); }

    //--------------------------------------------------------------------------
    // Visual state

    void setSelected(bool selected)
    {
        if (selected_ != selected)
        {
            selected_ = selected;
            repaint();
        }
    }

    bool isSelected() const { return selected_; }

    void setCollapsed(bool collapsed)
    {
        if (collapsed_ != collapsed)
        {
            collapsed_ = collapsed;
            collapseButton_.setButtonText(collapsed_ ? "+" : "-");

            if (moduleEditor_)
                moduleEditor_->setVisible(!collapsed_);

            for (auto& p : inputPortComponents_)  p->setVisible(!collapsed_);
            for (auto& p : outputPortComponents_) p->setVisible(!collapsed_);

            if (auto* parent = getParentComponent())
                parent->resized();

            resized();
            repaint();
        }
    }

    bool isCollapsed() const { return collapsed_; }

    int getDesiredHeight() const
    {
        return collapsed_ ? kCollapsedHeight : expandedHeight_;
    }

    //--------------------------------------------------------------------------
    // Port access (for cable rendering)

    PortComponent* findInputPort(uint32_t portId)
    {
        for (auto& p : inputPortComponents_)
            if (p->getPortInfo().id == portId)
                return p.get();
        return nullptr;
    }

    PortComponent* findOutputPort(uint32_t portId)
    {
        for (auto& p : outputPortComponents_)
            if (p->getPortInfo().id == portId)
                return p.get();
        return nullptr;
    }

    juce::Point<float> getPortPosition(uint32_t portId) const
    {
        for (auto& p : inputPortComponents_)
            if (p->getPortInfo().id == portId)
                return p->getCenterInParent() + getBounds().getTopLeft().toFloat();

        for (auto& p : outputPortComponents_)
            if (p->getPortInfo().id == portId)
                return p->getCenterInParent() + getBounds().getTopLeft().toFloat();

        return {};
    }

    /** Returns {moduleId, portId} if a port was hit, or a null Uuid otherwise. */
    std::pair<juce::Uuid, uint32_t> findPortAt(juce::Point<float> localPos) const
    {
        for (auto& p : inputPortComponents_)
            if (p->getBounds().toFloat().contains(localPos))
                return { module_.getUuid(), p->getPortInfo().id };

        for (auto& p : outputPortComponents_)
            if (p->getBounds().toFloat().contains(localPos))
                return { module_.getUuid(), p->getPortInfo().id };

        return { {}, 0 };
    }

    //--------------------------------------------------------------------------
    // CPU monitoring

    void updateCpuDisplay()
    {
        auto pct = module_.getCpuUsage();
        cpuLabel_.setText(juce::String(pct, 1) + "%", juce::dontSendNotification);

        auto colour = pct > 80.0 ? juce::Colours::red
                    : pct > 50.0 ? juce::Colours::yellow
                    : juce::Colours::white.withAlpha(0.6f);
        cpuLabel_.setColour(juce::Label::textColourId, colour);
    }

    //--------------------------------------------------------------------------
    // Listener management

    void addListener(Listener* l) { listeners_.add(l); }
    void removeListener(Listener* l) { listeners_.remove(l); }

    //--------------------------------------------------------------------------
    // juce::Component overrides

    void paint(juce::Graphics& g) override
    {
        paintShadow(g);
        paintBody(g);
        paintHeader(g);

        // Selection highlight
        if (selected_)
        {
            g.setColour(juce::Colour(0xffffd700).withAlpha(0.6f));
            g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 4.0f, 2.0f);
        }
        else if (hovered_)
        {
            g.setColour(juce::Colours::white.withAlpha(0.15f));
            g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 4.0f, 1.0f);
        }
    }

    void resized() override
    {
        auto bounds = getLocalBounds();

        // Header layout
        auto header = bounds.removeFromTop(kHeaderHeight);
        auto headerInner = header.reduced(4, 4);

        removeButton_.setBounds(headerInner.removeFromRight(20));
        headerInner.removeFromRight(2);
        collapseButton_.setBounds(headerInner.removeFromRight(20));
        headerInner.removeFromRight(2);
        bypassButton_.setBounds(headerInner.removeFromRight(20));
        headerInner.removeFromRight(4);
        cpuLabel_.setBounds(headerInner.removeFromRight(36));
        nameLabel_.setBounds(headerInner);

        if (!collapsed_)
        {
            layoutPorts();

            // Module editor fills the remaining center area
            if (moduleEditor_)
            {
                auto editorArea = bounds.reduced(kPortSize + 8, 4);
                moduleEditor_->setBounds(editorArea);
            }
        }
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        listeners_.call(&Listener::moduleClicked, this);

        if (e.mods.isLeftButtonDown())
            dragStartPos_ = e.getPosition();
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (e.getDistanceFromDragStart() > 4)
            listeners_.call(&Listener::moduleDragStarted, this);
    }

    void mouseEnter(const juce::MouseEvent&) override
    {
        hovered_ = true;
        repaint();
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        hovered_ = false;
        repaint();
    }

    //--------------------------------------------------------------------------
    // AudioModule::Listener overrides

    void moduleParameterChanged(revithion::rack::AudioModule*, const juce::String&) override
    {
        juce::MessageManager::callAsync([this] { repaint(); });
    }

    void moduleBypassChanged(revithion::rack::AudioModule*) override
    {
        juce::MessageManager::callAsync([this]
        {
            bypassButton_.setToggleState(module_.isBypassed(), juce::dontSendNotification);
            repaint();
        });
    }

    void moduleStateChanged(revithion::rack::AudioModule*) override
    {
        juce::MessageManager::callAsync([this]
        {
            nameLabel_.setText(module_.getInfo().displayName, juce::dontSendNotification);
            updateCpuDisplay();
            repaint();
        });
    }

    //--------------------------------------------------------------------------
    juce::ListenerList<Listener> listeners_;

private:
    //--------------------------------------------------------------------------
    // Layout constants

    static constexpr int kHeaderHeight  = 36;
    static constexpr int kCollapsedHeight = 36;
    static constexpr int kPortSize      = 16;
    static constexpr int kPortSpacing   = 24;

    //--------------------------------------------------------------------------
    /** Returns a header colour based on the module's category. */
    juce::Colour getHeaderColor() const
    {
        using MC = revithion::rack::ModuleCategory;
        switch (module_.getInfo().category)
        {
            case MC::Amp:        return juce::Colour(0xff8b0000); // deep red
            case MC::Cab:        return juce::Colour(0xff6b4226); // brown
            case MC::Drive:      return juce::Colour(0xffe65100); // orange
            case MC::Delay:      return juce::Colour(0xff00796b); // teal
            case MC::Reverb:     return juce::Colour(0xff1565c0); // blue
            case MC::Modulation: return juce::Colour(0xff7b1fa2); // purple
            case MC::EQ:         return juce::Colour(0xff2e7d32); // green
            case MC::Dynamics:   return juce::Colour(0xff546e7a); // slate
            case MC::Pitch:      return juce::Colour(0xffc2185b); // magenta
            case MC::Synth:      return juce::Colour(0xff00acc1); // cyan
            case MC::AI:         return juce::Colour(0xffffc107); // gold
            case MC::Utility:    return juce::Colour(0xff757575); // gray
            case MC::Input:      return juce::Colour(0xff1b5e20); // dark green
            case MC::Output:     return juce::Colour(0xffb71c1c); // dark red
            default:             return juce::Colour(0xff424242);
        }
    }

    //--------------------------------------------------------------------------
    void createPortComponents()
    {
        inputPortComponents_.clear();
        outputPortComponents_.clear();

        for (auto& port : module_.getInputPorts())
        {
            auto comp = std::make_unique<PortComponent>(port);
            addAndMakeVisible(*comp);
            inputPortComponents_.push_back(std::move(comp));
        }

        for (auto& port : module_.getOutputPorts())
        {
            auto comp = std::make_unique<PortComponent>(port);
            addAndMakeVisible(*comp);
            outputPortComponents_.push_back(std::move(comp));
        }
    }

    void layoutPorts()
    {
        auto bounds = getLocalBounds();
        auto bodyTop = bounds.getY() + kHeaderHeight;
        auto bodyHeight = bounds.getHeight() - kHeaderHeight;

        // Input ports along left edge
        {
            auto count = static_cast<int>(inputPortComponents_.size());
            auto totalHeight = count * kPortSpacing;
            auto startY = bodyTop + (bodyHeight - totalHeight) / 2;

            for (int i = 0; i < count; ++i)
            {
                auto y = startY + i * kPortSpacing + (kPortSpacing - kPortSize) / 2;
                inputPortComponents_[static_cast<size_t>(i)]->setBounds(2, y, kPortSize, kPortSize);
            }
        }

        // Output ports along right edge
        {
            auto count = static_cast<int>(outputPortComponents_.size());
            auto totalHeight = count * kPortSpacing;
            auto startY = bodyTop + (bodyHeight - totalHeight) / 2;
            auto x = bounds.getWidth() - kPortSize - 2;

            for (int i = 0; i < count; ++i)
            {
                auto y = startY + i * kPortSpacing + (kPortSpacing - kPortSize) / 2;
                outputPortComponents_[static_cast<size_t>(i)]->setBounds(x, y, kPortSize, kPortSize);
            }
        }
    }

    int calculateExpandedHeight() const
    {
        auto maxPorts = std::max(inputPortComponents_.size(), outputPortComponents_.size());
        auto portsHeight = static_cast<int>(maxPorts) * kPortSpacing + 16;
        auto editorHeight = moduleEditor_ ? moduleEditor_->getHeight() : 80;
        auto bodyHeight = std::max(portsHeight, editorHeight);
        return kHeaderHeight + std::max(bodyHeight, 60);
    }

    //--------------------------------------------------------------------------
    void paintHeader(juce::Graphics& g)
    {
        auto header = getLocalBounds().removeFromTop(kHeaderHeight).toFloat();
        auto colour = getHeaderColor();

        // Header background with rounded top corners
        juce::Path headerPath;
        headerPath.addRoundedRectangle(header.getX(), header.getY(),
                                       header.getWidth(), header.getHeight(),
                                       4.0f, 4.0f, true, true, false, false);

        g.setColour(module_.isBypassed() ? colour.withSaturation(0.2f) : colour);
        g.fillPath(headerPath);

        // Subtle gradient overlay
        g.setGradientFill(juce::ColourGradient(
            juce::Colours::white.withAlpha(0.08f), header.getX(), header.getY(),
            juce::Colours::transparentBlack, header.getX(), header.getBottom(),
            false));
        g.fillPath(headerPath);
    }

    void paintBody(juce::Graphics& g)
    {
        if (collapsed_)
            return;

        auto body = getLocalBounds().withTrimmedTop(kHeaderHeight).toFloat();
        auto bgColour = juce::Colour(0xff2a2a2a);

        if (module_.isBypassed())
            bgColour = bgColour.darker(0.3f);

        juce::Path bodyPath;
        bodyPath.addRoundedRectangle(body.getX(), body.getY(),
                                     body.getWidth(), body.getHeight(),
                                     4.0f, 4.0f, false, false, true, true);

        g.setColour(bgColour);
        g.fillPath(bodyPath);

        // Border
        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 4.0f, 1.0f);
    }

    void paintShadow(juce::Graphics& g)
    {
        auto shadow = juce::DropShadow(juce::Colours::black.withAlpha(0.35f), 6, { 0, 2 });
        shadow.drawForRectangle(g, getLocalBounds());
    }

    //--------------------------------------------------------------------------
    revithion::rack::AudioModule& module_;
    std::vector<std::unique_ptr<PortComponent>> inputPortComponents_;
    std::vector<std::unique_ptr<PortComponent>> outputPortComponents_;
    std::unique_ptr<juce::Component> moduleEditor_;

    bool selected_  = false;
    bool hovered_   = false;
    bool collapsed_ = false;
    int expandedHeight_ = 200;

    juce::Point<int> dragStartPos_;

    // Header components
    juce::Label      nameLabel_;
    juce::TextButton bypassButton_;
    juce::TextButton collapseButton_;
    juce::TextButton removeButton_;
    juce::Label      cpuLabel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleComponent)
};

} // namespace revithion::ui
