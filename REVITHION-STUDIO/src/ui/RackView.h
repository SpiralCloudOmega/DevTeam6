#pragma once
/*  ═══════════════════════════════════════════════════════════════════════════
    REVITHION STUDIO — RackView (UI)
    ═══════════════════════════════════════════════════════════════════════════
    Main rack UI component — a vertical rack layout inspired by Guitar Rig 7
    Pro, VCV Rack, and Reason's rack interface.

        • Scrollable viewport with zoom (0.5×–2.0×)
        • Drag-and-drop module insertion and reordering
        • Interactive cable patching with visual feedback
        • Metal-textured rack background with screws and rails
        • Selection / hover state for modules and cables
        • Listens to RackProcessor for model→view synchronisation

    ═══════════════════════════════════════════════════════════════════════════ */

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

#include "../rack/RackProcessor.h"
#include "CableRenderer.h"
#include "ModuleComponent.h"

#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace revithion::ui {

// ═══════════════════════════════════════════════════════════════════════════
// RackView
// ═══════════════════════════════════════════════════════════════════════════

class RackView final : public juce::Component,
                       public juce::DragAndDropContainer,
                       public juce::DragAndDropTarget,
                       public revithion::rack::RackProcessor::Listener
{
public:
    //──────────────────────────────────────────────────────────────────────
    // Listener — UI-level events emitted by the rack view
    //──────────────────────────────────────────────────────────────────────
    class Listener
    {
    public:
        virtual ~Listener() = default;

        virtual void moduleSelected  (RackView*, const juce::Uuid& /*moduleId*/) {}
        virtual void cableSelected   (RackView*, const juce::Uuid& /*cableId*/)  {}
        virtual void selectionCleared (RackView*) {}
        virtual void rackLayoutChanged(RackView*) {}
    };

    //──────────────────────────────────────────────────────────────────────
    // Construction / destruction
    //──────────────────────────────────────────────────────────────────────
    explicit RackView(revithion::rack::RackProcessor& processor);
    ~RackView() override;

    //──────────────────────────────────────────────────────────────────────
    // Module management (UI-side)
    //──────────────────────────────────────────────────────────────────────
    void             addModuleAtPosition(const juce::String& moduleTypeId,
                                         int insertIndex = -1);
    void             removeSelectedModule();
    void             moveModule(int fromIndex, int toIndex);
    ModuleComponent* getModuleComponentFor(const juce::Uuid& moduleId);

    //──────────────────────────────────────────────────────────────────────
    // View control
    //──────────────────────────────────────────────────────────────────────
    void  setZoomLevel(float zoom);
    float getZoomLevel() const noexcept { return zoomLevel_; }
    void  scrollToModule(const juce::Uuid& moduleId);
    void  fitToScreen();

    //──────────────────────────────────────────────────────────────────────
    // Selection
    //──────────────────────────────────────────────────────────────────────
    void selectModule(const juce::Uuid& moduleId);
    void selectCable(const juce::Uuid& cableId);
    void clearSelection();

    std::optional<juce::Uuid> getSelectedModuleId() const noexcept
    {
        return selectedModuleId_;
    }
    std::optional<juce::Uuid> getSelectedCableId() const noexcept
    {
        return selectedCableId_;
    }

    //──────────────────────────────────────────────────────────────────────
    // Cable interaction
    //──────────────────────────────────────────────────────────────────────
    void beginCableDrag(const juce::Uuid& moduleId, uint32_t portId,
                        juce::Point<float> startPos);
    void updateCableDrag(juce::Point<float> currentPos);
    void endCableDrag(const juce::Uuid& targetModuleId, uint32_t targetPortId);
    void cancelCableDrag();

    //──────────────────────────────────────────────────────────────────────
    // Listener list management
    //──────────────────────────────────────────────────────────────────────
    void addListener(Listener* l)    { listeners_.add(l); }
    void removeListener(Listener* l) { listeners_.remove(l); }

    //──────────────────────────────────────────────────────────────────────
    // juce::Component overrides
    //──────────────────────────────────────────────────────────────────────
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e,
                        const juce::MouseWheelDetails& w) override;
    bool keyPressed(const juce::KeyPress& key) override;

    //──────────────────────────────────────────────────────────────────────
    // juce::DragAndDropTarget overrides
    //──────────────────────────────────────────────────────────────────────
    bool isInterestedInDragSource(const SourceDetails& details) override;
    void itemDropped(const SourceDetails& details) override;

    //──────────────────────────────────────────────────────────────────────
    // revithion::rack::RackProcessor::Listener overrides
    //──────────────────────────────────────────────────────────────────────
    void moduleAdded(revithion::rack::RackProcessor* rp,
                     revithion::rack::AudioModule* module) override;
    void moduleRemoved(revithion::rack::RackProcessor* rp,
                       const juce::Uuid& moduleId) override;
    void cableAdded(revithion::rack::RackProcessor* rp,
                    revithion::rack::PatchCable* cable) override;
    void cableRemoved(revithion::rack::RackProcessor* rp,
                      const juce::Uuid& cableId) override;
    void rackCleared(revithion::rack::RackProcessor* rp) override;

private:
    //──────────────────────────────────────────────────────────────────────
    // Layout constants
    //──────────────────────────────────────────────────────────────────────
    static constexpr int kRackWidth          = 600;
    static constexpr int kModuleDefaultHeight = 200;
    static constexpr int kModuleSpacing      = 4;
    static constexpr int kRackPadding        = 16;
    static constexpr int kRackScrewSize      = 12;

    //──────────────────────────────────────────────────────────────────────
    // Drag state machine
    //──────────────────────────────────────────────────────────────────────
    struct DragState
    {
        enum class Mode
        {
            None,
            DraggingModule,
            DraggingCable,
            ReorderingModule
        };

        Mode               mode              = Mode::None;
        juce::Point<float> startPos          = {};
        juce::Point<float> currentPos        = {};
        juce::Uuid         draggedModuleId   = {};
        juce::Uuid         sourceModuleId    = {};
        uint32_t           sourcePortId      = 0;
        int                reorderSourceIndex = -1;
        int                reorderTargetIndex = -1;
    };

    //──────────────────────────────────────────────────────────────────────
    // Private helpers
    //──────────────────────────────────────────────────────────────────────
    void             rebuildLayout();
    void             paintRackBackground(juce::Graphics& g);
    void             paintRackScrew(juce::Graphics& g, juce::Point<float> pos);
    ModuleComponent* findModuleAt(juce::Point<float> pos);
    std::pair<juce::Uuid, uint32_t> findPortAt(juce::Point<float> pos);
    void             updateCablePositions();
    int              calculateInsertIndex(float y) const;

    //──────────────────────────────────────────────────────────────────────
    // Members
    //──────────────────────────────────────────────────────────────────────
    revithion::rack::RackProcessor& rackProcessor_;

    std::vector<std::unique_ptr<ModuleComponent>> moduleComponents_;
    std::unique_ptr<CableRenderer>                cableRenderer_;

    // Viewport / scroll
    juce::Viewport                  viewport_;
    std::unique_ptr<juce::Component> rackContent_;
    float                           zoomLevel_ = 1.0f;

    // Interaction state
    DragState                       dragState_;
    std::optional<juce::Uuid>       selectedModuleId_;
    std::optional<juce::Uuid>       selectedCableId_;
    std::optional<juce::Uuid>       hoveredModuleId_;
    std::optional<juce::Uuid>       hoveredCableId_;

    // Listeners
    juce::ListenerList<Listener>    listeners_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RackView)
};

} // namespace revithion::ui
