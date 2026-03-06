#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <cmath>
#include <cstdint>
#include <memory>

namespace revithion::rack {

// ---------------------------------------------------------------------------
// CableType — signal type carried by the cable
// ---------------------------------------------------------------------------
enum class CableType : uint8_t
{
    Mono,
    Stereo,
    Sidechain,
    CV,
    MIDI
};

// ---------------------------------------------------------------------------
// CableEndpoint — identifies a specific port on a specific module
// ---------------------------------------------------------------------------
struct CableEndpoint
{
    juce::Uuid module_id;
    uint32_t   port_id { 0 };

    bool operator==(const CableEndpoint& other) const
    {
        return module_id == other.module_id && port_id == other.port_id;
    }

    bool operator!=(const CableEndpoint& other) const { return !(*this == other); }
};

// ---------------------------------------------------------------------------
// CableColorScheme — static color palette keyed by CableType
// ---------------------------------------------------------------------------
struct CableColorScheme
{
    static constexpr uint32_t kAudioColor_argb     = 0xFFE8842A; // warm orange
    static constexpr uint32_t kStereoColor_argb    = 0xFF3A8FD6; // blue
    static constexpr uint32_t kSidechainColor_argb = 0xFFE8C72A; // yellow
    static constexpr uint32_t kCVColor_argb        = 0xFF3DD65B; // green
    static constexpr uint32_t kMIDIColor_argb      = 0xFFA54ED6; // purple
    static constexpr uint32_t kHighlightColor_argb = 0xFFFFFFFF; // white

    static inline const juce::Colour kAudioColor     { kAudioColor_argb };
    static inline const juce::Colour kStereoColor    { kStereoColor_argb };
    static inline const juce::Colour kSidechainColor { kSidechainColor_argb };
    static inline const juce::Colour kCVColor        { kCVColor_argb };
    static inline const juce::Colour kMIDIColor      { kMIDIColor_argb };
    static inline const juce::Colour kHighlightColor { kHighlightColor_argb };

    static juce::Colour getColorForType(CableType type)
    {
        switch (type)
        {
            case CableType::Mono:      return kAudioColor;
            case CableType::Stereo:    return kStereoColor;
            case CableType::Sidechain: return kSidechainColor;
            case CableType::CV:        return kCVColor;
            case CableType::MIDI:      return kMIDIColor;
        }
        return kAudioColor;
    }
};

// ---------------------------------------------------------------------------
// CableVisualState — everything the renderer needs to draw one cable
// ---------------------------------------------------------------------------
struct CableVisualState
{
    juce::Point<float> start_pos;
    juce::Point<float> end_pos;
    juce::Point<float> control_point_1;
    juce::Point<float> control_point_2;

    float thickness    { 3.0f };
    float opacity      { 1.0f };
    float signal_level { 0.0f }; // 0.0–1.0 animated signal flow
    bool  highlighted  { false };
    bool  selected     { false };

    // Gravity-droop constants
    static constexpr float kMinDroop       = 30.0f;  // pixels – minimum sag even for short cables
    static constexpr float kDroopPerPixel  = 0.35f;  // additional sag per pixel of horizontal span
    static constexpr float kMaxDroop       = 220.0f; // clamp so very long cables stay reasonable
    static constexpr float kTangentRatio   = 0.4f;   // horizontal share given to the control–point offset

    /// Recalculate bezier control points so the cable droops like a real
    /// hanging cable (catenary approximated with a cubic bezier).
    /// The droop scales with horizontal distance between endpoints.
    void updateBezierPoints()
    {
        const float dx = std::abs(end_pos.x - start_pos.x);
        const float dy = end_pos.y - start_pos.y;

        // Droop increases with horizontal span, clamped to a sane range.
        const float droop = std::clamp(kMinDroop + dx * kDroopPerPixel,
                                       kMinDroop, kMaxDroop);

        // Lowest point sits below the midpoint of the two endpoints,
        // biased slightly toward the lower one for visual realism.
        const float mid_y = (start_pos.y + end_pos.y) * 0.5f + droop;

        // Horizontal offsets push the tangent handles towards the midpoint
        // to produce a smooth catenary-like curve.
        const float offset_x = dx * kTangentRatio;

        control_point_1 = { start_pos.x + offset_x, mid_y };
        control_point_2 = { end_pos.x   - offset_x, mid_y };

        // When the cable is nearly vertical keep the handles centred
        // horizontally to avoid an ugly zig-zag.
        if (dx < 20.0f)
        {
            const float centre_x = (start_pos.x + end_pos.x) * 0.5f;
            control_point_1.x = centre_x;
            control_point_2.x = centre_x;
        }
    }

    /// Build the JUCE Path used by the renderer.
    juce::Path generatePath() const
    {
        juce::Path p;
        p.startNewSubPath(start_pos);
        p.cubicTo(control_point_1, control_point_2, end_pos);
        return p;
    }
};

// ---------------------------------------------------------------------------
// PatchCable — a connection between two module ports in the rack
// ---------------------------------------------------------------------------
class PatchCable
{
public:
    PatchCable(CableEndpoint source, CableEndpoint destination, CableType type)
        : uuid_(juce::Uuid()),
          source_(std::move(source)),
          destination_(std::move(destination)),
          type_(type)
    {
    }

    // --- Identity -----------------------------------------------------------
    const juce::Uuid&      getUuid()        const { return uuid_; }
    const CableEndpoint&   getSource()      const { return source_; }
    const CableEndpoint&   getDestination() const { return destination_; }
    CableType              getType()        const { return type_; }

    // --- Gain / mute --------------------------------------------------------
    void  setGain(float g)       { gain_ = g; }
    float getGain()        const { return gain_; }

    void setMuted(bool m)        { muted_ = m; }
    bool isMuted()         const { return muted_; }

    // --- Latency compensation -----------------------------------------------
    void setLatencyCompensation(int samples) { latency_compensation_ = samples; }
    int  getLatencyCompensation()      const { return latency_compensation_; }

    // --- Visual state -------------------------------------------------------
    CableVisualState&       getVisualState()       { return visual_state_; }
    const CableVisualState& getVisualState() const { return visual_state_; }

    // --- Colour helper ------------------------------------------------------
    juce::Colour getColor() const
    {
        return CableColorScheme::getColorForType(type_);
    }

    // --- Connection queries -------------------------------------------------
    bool connects(const juce::Uuid& module_id) const
    {
        return source_.module_id == module_id || destination_.module_id == module_id;
    }

    bool connectsPort(const juce::Uuid& module_id, uint32_t port_id) const
    {
        return (source_.module_id == module_id      && source_.port_id == port_id)
            || (destination_.module_id == module_id  && destination_.port_id == port_id);
    }

    // --- Serialization ------------------------------------------------------
    juce::ValueTree toValueTree() const
    {
        juce::ValueTree vt { "PatchCable" };

        vt.setProperty("uuid",          uuid_.toString(),                   nullptr);
        vt.setProperty("sourceModule",  source_.module_id.toString(),       nullptr);
        vt.setProperty("sourcePort",    static_cast<int>(source_.port_id),  nullptr);
        vt.setProperty("destModule",    destination_.module_id.toString(),   nullptr);
        vt.setProperty("destPort",      static_cast<int>(destination_.port_id), nullptr);
        vt.setProperty("type",          static_cast<int>(type_),            nullptr);
        vt.setProperty("gain",          static_cast<double>(gain_),         nullptr);
        vt.setProperty("muted",         muted_,                             nullptr);
        vt.setProperty("latencyComp",   latency_compensation_,              nullptr);

        return vt;
    }

    static std::unique_ptr<PatchCable> fromValueTree(const juce::ValueTree& vt)
    {
        if (!vt.isValid() || vt.getType().toString() != "PatchCable")
            return nullptr;

        CableEndpoint src;
        src.module_id = juce::Uuid(vt.getProperty("sourceModule").toString());
        src.port_id   = static_cast<uint32_t>(static_cast<int>(vt.getProperty("sourcePort")));

        CableEndpoint dst;
        dst.module_id = juce::Uuid(vt.getProperty("destModule").toString());
        dst.port_id   = static_cast<uint32_t>(static_cast<int>(vt.getProperty("destPort")));

        auto cable_type = static_cast<CableType>(static_cast<int>(vt.getProperty("type")));

        auto cable = std::make_unique<PatchCable>(src, dst, cable_type);

        // Restore the original UUID rather than the freshly generated one.
        cable->uuid_ = juce::Uuid(vt.getProperty("uuid").toString());

        cable->gain_                 = static_cast<float>(static_cast<double>(vt.getProperty("gain", 1.0)));
        cable->muted_                = static_cast<bool>(vt.getProperty("muted", false));
        cable->latency_compensation_ = static_cast<int>(vt.getProperty("latencyComp", 0));

        return cable;
    }

    // --- Equality (by UUID) -------------------------------------------------
    bool operator==(const PatchCable& other) const { return uuid_ == other.uuid_; }
    bool operator!=(const PatchCable& other) const { return !(*this == other); }

private:
    juce::Uuid       uuid_;
    CableEndpoint    source_;
    CableEndpoint    destination_;
    CableType        type_              { CableType::Mono };
    CableVisualState visual_state_;
    float            gain_              { 1.0f };
    bool             muted_             { false };
    int              latency_compensation_ { 0 };
};

} // namespace revithion::rack
