#pragma once
/*  ═══════════════════════════════════════════════════════════════════════════
    REVITHION STUDIO — AudioPresets
    ═══════════════════════════════════════════════════════════════════════════
    Curated preset configurations for common production workflows.
    Each preset encodes a (SampleRate, BitDepth, BufferSize) triple together
    with a human-readable name, description, and "recommended" flag.
    ═══════════════════════════════════════════════════════════════════════════ */

#include "AudioSettings.h"
#include <array>
#include <string>
#include <string_view>

namespace revithion::settings {

// ── Preset descriptor (value type, constexpr-friendly) ──────────────────────
struct AudioPreset {
    std::string_view name;
    std::string_view description;
    SampleRate       sampleRate;
    BitDepth         bitDepth;
    BufferSize       bufferSize;
    bool             recommended;  // shown with ★ in the UI

    /// Apply this preset to a live AudioSettings instance.
    void applyTo(AudioSettings& settings) const {
        settings.applyAll(sampleRate, bitDepth, bufferSize);
    }

    /// Computed one-way latency for display.
    constexpr float latencyMs() const noexcept {
        return settings::latencyMs(bufferSize, sampleRate);
    }
};

// ── Built-in presets ────────────────────────────────────────────────────────
// The order here determines the order in the UI preset dropdown.

inline constexpr AudioPreset kPresetSpatial3D {
    "Spatial 3D Audio",
    "96 kHz / 24-bit / 128 samples — 1.33 ms latency.\n"
    "The REVITHION sweet spot: imperceptible latency for Dolby Atmos, "
    "Ambisonics, binaural, and object-based spatial audio workflows.",
    SampleRate::Rate_96000,
    BitDepth::Bits_24,
    BufferSize::Samples_128,
    true   // ★ recommended
};

inline constexpr AudioPreset kPresetLivePerformance {
    "Live Performance",
    "96 kHz / 24-bit / 64 samples — 0.67 ms latency.\n"
    "Ultra-responsive for live guitar, keys, and vocals with real-time "
    "monitoring through neural amp models and effects.",
    SampleRate::Rate_96000,
    BitDepth::Bits_24,
    BufferSize::Samples_64,
    true   // ★ recommended
};

inline constexpr AudioPreset kPresetMixingMastering {
    "Mixing & Mastering",
    "96 kHz / 24-bit / 512 samples — 5.33 ms latency.\n"
    "Generous buffer for heavy plugin loads during mixing and mastering "
    "while keeping the 96 kHz quality ceiling.",
    SampleRate::Rate_96000,
    BitDepth::Bits_24,
    BufferSize::Samples_512,
    false
};

inline constexpr AudioPreset kPresetFilmVideoPost {
    "Film & Video Post",
    "48 kHz / 24-bit / 256 samples — 5.33 ms latency.\n"
    "Standard broadcast rate for DaVinci Resolve, Premiere Pro, and "
    "Nuendo workflows. Avoids SRC when exporting to video.",
    SampleRate::Rate_48000,
    BitDepth::Bits_24,
    BufferSize::Samples_256,
    false
};

inline constexpr AudioPreset kPresetStreamingExport {
    "Streaming Export",
    "44.1 kHz / 16-bit / 1024 samples — 23.22 ms latency.\n"
    "Final bounce settings for Spotify, Apple Music, YouTube, and SoundCloud. "
    "Maximum stability during offline render.",
    SampleRate::Rate_44100,
    BitDepth::Bits_16,
    BufferSize::Samples_1024,
    false
};

inline constexpr AudioPreset kPresetUltraHDMastering {
    "Ultra HD Mastering",
    "192 kHz / 32-bit float / 256 samples — 1.33 ms latency.\n"
    "Maximum fidelity for archival masters and DXD workflows. "
    "Requires a capable interface and significant CPU headroom.",
    SampleRate::Rate_192000,
    BitDepth::Bits_32,
    BufferSize::Samples_256,
    false
};

inline constexpr AudioPreset kPresetArchivalPreservation {
    "Archival Preservation",
    "384 kHz / 32-bit float / 512 samples — 1.33 ms latency.\n"
    "Absolute maximum sample rate for scientific measurement and "
    "long-term digital preservation of irreplaceable recordings.",
    SampleRate::Rate_384000,
    BitDepth::Bits_32,
    BufferSize::Samples_512,
    false
};

inline constexpr AudioPreset kPresetLowLatencyInstruments {
    "Low Latency Instruments",
    "96 kHz / 24-bit / 32 samples — 0.33 ms latency.\n"
    "Absolute minimum buffer for latency-critical instrument tracking. "
    "Requires a high-performance audio interface and low CPU load.",
    SampleRate::Rate_96000,
    BitDepth::Bits_24,
    BufferSize::Samples_32,
    true   // ★ recommended
};

// ── All presets in display order ────────────────────────────────────────────
inline constexpr std::array kAllPresets = {
    kPresetSpatial3D,
    kPresetLivePerformance,
    kPresetLowLatencyInstruments,
    kPresetMixingMastering,
    kPresetFilmVideoPost,
    kPresetStreamingExport,
    kPresetUltraHDMastering,
    kPresetArchivalPreservation
};

inline constexpr int kDefaultPresetIndex = 0;  // Spatial 3D Audio

} // namespace revithion::settings
