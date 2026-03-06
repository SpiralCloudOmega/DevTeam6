#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include "../rack/PatchCable.h"

#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>

namespace revithion::ui
{

struct CableRenderStyle
{
    float baseThickness       = 3.0f;
    float selectedThickness   = 5.0f;
    float highlightThickness  = 4.0f;
    float glowRadius          = 8.0f;
    float signalAnimationSpeed = 2.0f;
    float droopFactor         = 0.5f;
    float opacity             = 0.9f;
    float inactiveOpacity     = 0.4f;
    bool  showSignalAnimation = true;
    bool  showGlow            = true;
    int   bezierSegments      = 48;
    juce::Colour shadowColor  = juce::Colour(0, 0, 0).withAlpha(0.3f);
};

struct CableAnimationState
{
    float signalPhase    = 0.0f;
    float signalLevel    = 0.0f;
    float targetOpacity  = 1.0f;
    float currentOpacity = 1.0f;

    void update(float deltaTime, float level)
    {
        signalLevel = level;

        // Advance phase, wrapping at 1.0
        signalPhase += deltaTime * 2.0f; // base speed multiplier
        signalPhase -= std::floor(signalPhase);

        // Smooth exponential interpolation toward target opacity
        constexpr float kOpacitySmoothingRate = 8.0f;
        float blend = 1.0f - std::exp(-kOpacitySmoothingRate * deltaTime);
        currentOpacity += (targetOpacity - currentOpacity) * blend;
    }
};

class CableRenderer final : public juce::Component,
                             public juce::Timer
{
public:
    CableRenderer()
    {
        setInterceptsMouseClicks(false, false);
        startTimerHz(60);
    }

    ~CableRenderer() override
    {
        stopTimer();
    }

    //==========================================================================
    // Cable management (non-owning references)
    //==========================================================================

    void setCables(const std::vector<revithion::rack::PatchCable*>& cables)
    {
        cables_ = cables;
        pruneAnimationStates();
    }

    void addCable(revithion::rack::PatchCable* cable)
    {
        if (cable != nullptr)
            cables_.push_back(cable);
    }

    void removeCable(const juce::Uuid& cableId)
    {
        cables_.erase(
            std::remove_if(cables_.begin(), cables_.end(),
                [&](const revithion::rack::PatchCable* c) { return c->getUuid() == cableId; }),
            cables_.end());
        animationStates_.erase(cableId);
    }

    void clearCables()
    {
        cables_.clear();
        animationStates_.clear();
    }

    //==========================================================================
    // Signal level (called from audio thread via lock-free mechanism)
    //==========================================================================

    void updateSignalLevel(const juce::Uuid& cableId, float level)
    {
        auto it = animationStates_.find(cableId);
        if (it != animationStates_.end())
            it->second.signalLevel = level;
    }

    //==========================================================================
    // Drag cable (cable currently being created by the user)
    //==========================================================================

    void beginDragCable(juce::Point<float> startPos, revithion::rack::CableType type)
    {
        isDraggingCable_ = true;
        dragCableEnd_    = startPos;
        dragCableType_   = type;
        dragCableColor_  = revithion::rack::CableColorScheme::getColorForType(type);
        repaint();
    }

    void updateDragCable(juce::Point<float> endPos)
    {
        dragCableEnd_ = endPos;
        repaint();
    }

    void endDragCable()
    {
        isDraggingCable_ = false;
        repaint();
    }

    [[nodiscard]] bool isDraggingCable() const noexcept { return isDraggingCable_; }

    //==========================================================================
    // Style
    //==========================================================================

    void setStyle(const CableRenderStyle& style) { style_ = style; }
    [[nodiscard]] const CableRenderStyle& getStyle() const noexcept { return style_; }

    //==========================================================================
    // Hit testing — find cable under a point
    //==========================================================================

    [[nodiscard]] revithion::rack::PatchCable* hitTestCable(juce::Point<float> pos,
                                                            float tolerance = 8.0f) const
    {
        for (auto* cable : cables_)
        {
            if (cable == nullptr) continue;

            auto& vs = cable->getVisualState();
            auto path = createCablePath(vs.start_pos, vs.end_pos);
            if (distanceToPath(path, pos) <= tolerance)
                return cable;
        }
        return nullptr;
    }

    //==========================================================================
    // JUCE overrides
    //==========================================================================

    void paint(juce::Graphics& g) override
    {
        for (auto* cable : cables_)
        {
            if (cable == nullptr) continue;

            auto& anim = animationStates_[cable->getUuid()];
            paintCable(g, *cable, anim);
        }

        if (isDraggingCable_)
            paintDragCable(g);
    }

    void timerCallback() override
    {
        constexpr float kFrameDelta = 1.0f / 60.0f;
        animationTime_ += kFrameDelta;

        for (auto& [id, anim] : animationStates_)
            anim.update(kFrameDelta * style_.signalAnimationSpeed, anim.signalLevel);

        repaint();
    }

private:
    //==========================================================================
    // Rendering helpers
    //==========================================================================

    void paintCable(juce::Graphics& g, const revithion::rack::PatchCable& cable,
                    const CableAnimationState& anim)
    {
        auto& vs   = cable.getVisualState();
        auto  path = createCablePath(vs.start_pos, vs.end_pos);
        auto  color = cable.getColor();

        // Adjust brightness by signal level
        float brightness = 0.6f + 0.4f * anim.signalLevel;
        auto  cableColor = color.withMultipliedBrightness(brightness)
                                .withAlpha(style_.opacity * anim.currentOpacity);

        bool isSelected = vs.selected;

        // Shadow
        {
            juce::Graphics::ScopedSaveState saveState(g);
            auto shadow = path;
            shadow.applyTransform(juce::AffineTransform::translation(1.5f, 2.0f));
            g.setColour(style_.shadowColor);
            g.strokePath(shadow, juce::PathStrokeType(
                isSelected ? style_.selectedThickness + 1.0f : style_.baseThickness + 1.0f));
        }

        // Glow effect
        if (style_.showGlow && (isSelected || anim.signalLevel > 0.01f))
            paintCableGlow(g, path, color, style_.glowRadius * anim.signalLevel);

        // Cable stroke
        float thickness = isSelected ? style_.selectedThickness : style_.baseThickness;
        g.setColour(cableColor);
        g.strokePath(path, juce::PathStrokeType(
            thickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Signal flow dots
        if (style_.showSignalAnimation && anim.signalLevel > 0.05f)
            paintSignalDots(g, path, anim.signalPhase, color);
    }

    void paintCableGlow(juce::Graphics& g, const juce::Path& path,
                        juce::Colour color, float radius)
    {
        if (radius < 0.5f) return;

        constexpr int kGlowPasses = 3;
        for (int i = kGlowPasses; i >= 1; --i)
        {
            float t     = static_cast<float>(i) / static_cast<float>(kGlowPasses);
            float alpha = 0.08f * (1.0f - t);
            float width = radius * t * 2.0f;

            g.setColour(color.withAlpha(alpha));
            g.strokePath(path, juce::PathStrokeType(width));
        }
    }

    void paintSignalDots(juce::Graphics& g, const juce::Path& path,
                         float phase, juce::Colour color)
    {
        constexpr int   kDotCount  = 5;
        constexpr float kDotRadius = 2.5f;

        float pathLength = path.getLength();
        if (pathLength < 1.0f) return;

        g.setColour(color.withAlpha(0.85f));

        for (int i = 0; i < kDotCount; ++i)
        {
            float t = std::fmod(phase + static_cast<float>(i) / static_cast<float>(kDotCount), 1.0f);
            auto  point = path.getPointAlongPath(t * pathLength);
            g.fillEllipse(point.x - kDotRadius, point.y - kDotRadius,
                          kDotRadius * 2.0f, kDotRadius * 2.0f);
        }
    }

    void paintDragCable(juce::Graphics& g)
    {
        if (!isDraggingCable_) return;

        // Use the drag start position from the first cable or fallback
        auto startPos = dragCableEnd_; // will be overridden by caller via begin/update
        auto path     = createCablePath(startPos, dragCableEnd_);

        auto color = dragCableColor_.withAlpha(0.7f);
        g.setColour(style_.shadowColor);
        g.strokePath(path, juce::PathStrokeType(style_.baseThickness + 1.0f));

        g.setColour(color);
        g.strokePath(path, juce::PathStrokeType(
            style_.baseThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
    }

    //==========================================================================
    // Bezier path with gravity droop
    //==========================================================================

    [[nodiscard]] juce::Path createCablePath(juce::Point<float> start,
                                             juce::Point<float> end) const
    {
        juce::Path path;

        float dx = std::abs(end.x - start.x);
        float dy = end.y - start.y;

        // Droop proportional to horizontal distance, with a minimum for nearby modules
        float minDroop    = 20.0f;
        float distDroop   = dx * style_.droopFactor;
        float droopAmount = std::max(minDroop, distDroop);

        // Reduce droop when endpoints are mostly vertical (close horizontally)
        float verticalBias = (dx > 1.0f) ? std::min(1.0f, dx / 200.0f) : 0.15f;
        droopAmount *= verticalBias + (1.0f - verticalBias) * 0.3f;

        // Mid-Y for control points — biased downward (screen coords: +Y is down)
        float midY = (start.y + end.y) * 0.5f + droopAmount;

        // Control points: 1/3 and 2/3 horizontally, drooping below the midpoint
        juce::Point<float> cp1 { start.x + dx * 0.15f, midY };
        juce::Point<float> cp2 { end.x   - dx * 0.15f, midY };

        // If cable goes left-to-right vs right-to-left, keep cp offset direction correct
        if (end.x < start.x)
        {
            cp1.x = start.x - dx * 0.15f;
            cp2.x = end.x   + dx * 0.15f;
        }

        path.startNewSubPath(start);
        path.cubicTo(cp1, cp2, end);

        return path;
    }

    //==========================================================================
    // Distance from point to path (for hit testing)
    //==========================================================================

    [[nodiscard]] float distanceToPath(const juce::Path& path,
                                       juce::Point<float> point) const
    {
        float pathLength = path.getLength();
        if (pathLength < 1.0f)
            return point.getDistanceFrom(path.getPointAlongPath(0.0f));

        float minDist = std::numeric_limits<float>::max();
        int   steps   = std::max(32, style_.bezierSegments);

        for (int i = 0; i <= steps; ++i)
        {
            float t    = static_cast<float>(i) / static_cast<float>(steps);
            auto  p    = path.getPointAlongPath(t * pathLength);
            float dist = point.getDistanceFrom(p);
            minDist    = std::min(minDist, dist);
        }

        return minDist;
    }

    //==========================================================================
    // Internal helpers
    //==========================================================================

    void pruneAnimationStates()
    {
        // Remove animation states for cables no longer in the list
        for (auto it = animationStates_.begin(); it != animationStates_.end();)
        {
            bool found = std::any_of(cables_.begin(), cables_.end(),
                [&](const revithion::rack::PatchCable* c) { return c && c->getUuid() == it->first; });

            if (!found)
                it = animationStates_.erase(it);
            else
                ++it;
        }
    }

    //==========================================================================
    // Members
    //==========================================================================

    struct UuidHash
    {
        std::size_t operator()(const juce::Uuid& uuid) const noexcept
        {
            return uuid.hash();
        }
    };

    std::vector<revithion::rack::PatchCable*>                         cables_;
    std::unordered_map<juce::Uuid, CableAnimationState, UuidHash>    animationStates_;
    CableRenderStyle                                                  style_;

    juce::Point<float>         dragCableEnd_;
    bool                       isDraggingCable_  = false;
    juce::Uuid                 dragSourceModule_;
    uint32_t                   dragSourcePort_   = 0;
    revithion::rack::CableType dragCableType_    = revithion::rack::CableType::Audio;
    juce::Colour               dragCableColor_   { juce::Colours::white };

    float                      animationTime_    = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CableRenderer)
};

} // namespace revithion::ui
