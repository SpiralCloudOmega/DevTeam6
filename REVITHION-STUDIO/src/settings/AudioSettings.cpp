/*  ═══════════════════════════════════════════════════════════════════════════
    REVITHION STUDIO — AudioSettings implementation
    ═══════════════════════════════════════════════════════════════════════════ */

#include "AudioSettings.h"

namespace revithion::settings {

// ── Free-function implementations ───────────────────────────────────────────

std::string sampleRateLabel(SampleRate r) {
    switch (r) {
        case SampleRate::Rate_44100:  return "44.1 kHz";
        case SampleRate::Rate_48000:  return "48.0 kHz";
        case SampleRate::Rate_88200:  return "88.2 kHz";
        case SampleRate::Rate_96000:  return "96.0 kHz \u2605";   // ★ star marks the sweet spot
        case SampleRate::Rate_176400: return "176.4 kHz";
        case SampleRate::Rate_192000: return "192.0 kHz";
        case SampleRate::Rate_352800: return "352.8 kHz";
        case SampleRate::Rate_384000: return "384.0 kHz";
    }
    return "Unknown";
}

std::string sampleRateDescription(SampleRate r) {
    switch (r) {
        case SampleRate::Rate_44100:
            return "CD quality (44.1 kHz) — Standard for streaming platforms "
                   "(Spotify, Apple Music, Tidal HiFi). Use for final bounce-to-disk "
                   "when targeting consumer distribution.";

        case SampleRate::Rate_48000:
            return "Video standard (48 kHz) — The universal rate for film, television, "
                   "and broadcast. Required by most NLEs (DaVinci Resolve, Premiere Pro). "
                   "Use for any audio destined for video post-production.";

        case SampleRate::Rate_88200:
            return "Double CD (88.2 kHz) — Niche rate primarily used for vinyl pre-mastering "
                   "and DDP image creation. Downsamples cleanly to 44.1 kHz by simple "
                   "2:1 decimation without complex SRC.";

        case SampleRate::Rate_96000:
            return "\u2605 SWEET SPOT (96 kHz) — The optimal rate for REVITHION STUDIO.\n\n"
                   "At 96 kHz a 128-sample buffer yields only 1.33 ms one-way latency — "
                   "perceptually instant. This is the rate where real-time instrument "
                   "monitoring, spatial 3D audio (Atmos, Ambisonics), and high-quality "
                   "DSP processing converge without excessive CPU or storage cost.\n\n"
                   "Recommended for: recording, mixing, spatial audio, live performance, "
                   "and any session where low latency matters.";

        case SampleRate::Rate_176400:
            return "Quad CD (176.4 kHz) — Used in high-end mastering houses. "
                   "Provides extreme headroom for analog-modeled processing. "
                   "Downsamples cleanly to 44.1 kHz by 4:1 decimation.";

        case SampleRate::Rate_192000:
            return "Ultra HD Audio (192 kHz) — Archival-grade rate for DXD source masters "
                   "and critical classical / acoustic recordings. Significant storage and "
                   "CPU overhead; use only when the project demands it.";

        case SampleRate::Rate_352800:
            return "DSD-level (352.8 kHz) — Approaches DSD64 fidelity in PCM domain. "
                   "Primarily academic or for extreme mastering workflows. "
                   "Very few interfaces support native 352.8 kHz I/O.";

        case SampleRate::Rate_384000:
            return "Maximum (384 kHz) — Scientific measurement and long-term digital "
                   "preservation. Captures ultrasonic content well beyond human hearing. "
                   "Requires specialized hardware and enormous storage.";
    }
    return {};
}

std::string bitDepthLabel(BitDepth b) {
    switch (b) {
        case BitDepth::Bits_16: return "16-bit";
        case BitDepth::Bits_24: return "24-bit \u2605";  // ★ professional default
        case BitDepth::Bits_32: return "32-bit float";
        case BitDepth::Bits_64: return "64-bit double";
    }
    return "Unknown";
}

std::string bufferSizeLabel(BufferSize s) {
    // Append approximate latency at 96 kHz for quick reference.
    auto ms = latencyMs(s, SampleRate::Rate_96000);
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%d samples (~%.2f ms @ 96 kHz)", toInt(s), ms);
    return buf;
}

// ── Enum-from-int look-ups ──────────────────────────────────────────────────

std::optional<SampleRate> sampleRateFromInt(int value) noexcept {
    for (auto r : kAllSampleRates)
        if (toInt(r) == value) return r;
    return std::nullopt;
}

std::optional<BitDepth> bitDepthFromInt(int value) noexcept {
    for (auto b : kAllBitDepths)
        if (toInt(b) == value) return b;
    return std::nullopt;
}

std::optional<BufferSize> bufferSizeFromInt(int value) noexcept {
    for (auto s : kAllBufferSizes)
        if (toInt(s) == value) return s;
    return std::nullopt;
}

// ═════════════════════════════════════════════════════════════════════════════
// AudioSettings class
// ═════════════════════════════════════════════════════════════════════════════

AudioSettings::AudioSettings()  = default;
AudioSettings::~AudioSettings() = default;

// ── Getters (lock-free atomics — safe from the audio thread) ────────────────

SampleRate AudioSettings::getSampleRate() const noexcept {
    return static_cast<SampleRate>(sampleRate_.load(std::memory_order_relaxed));
}

BitDepth AudioSettings::getBitDepth() const noexcept {
    return static_cast<BitDepth>(bitDepth_.load(std::memory_order_relaxed));
}

BufferSize AudioSettings::getBufferSize() const noexcept {
    return static_cast<BufferSize>(bufferSize_.load(std::memory_order_relaxed));
}

// ── Computed properties ─────────────────────────────────────────────────────

float AudioSettings::getLatencyMs() const noexcept {
    return latencyMs(getBufferSize(), getSampleRate());
}

float AudioSettings::getRoundTripLatencyMs() const noexcept {
    return roundTripLatencyMs(getBufferSize(), getSampleRate());
}

float AudioSettings::getNyquistHz() const noexcept {
    return nyquistHz(getSampleRate());
}

bool AudioSettings::isOptimalForSpatialAudio() const noexcept {
    return settings::isOptimalForSpatialAudio(getSampleRate());
}

bool AudioSettings::isOptimalForInstruments() const noexcept {
    return settings::isOptimalForInstruments(getSampleRate());
}

bool AudioSettings::isHighLatency() const noexcept {
    return getLatencyMs() > 10.0f;
}

// ── Setters (message-thread only) ───────────────────────────────────────────

void AudioSettings::setSampleRate(SampleRate rate) {
    sampleRate_.store(toInt(rate), std::memory_order_release);
    notifyListeners();
}

void AudioSettings::setBitDepth(BitDepth depth) {
    bitDepth_.store(toInt(depth), std::memory_order_release);
    notifyListeners();
}

void AudioSettings::setBufferSize(BufferSize size) {
    bufferSize_.store(toInt(size), std::memory_order_release);
    notifyListeners();
}

void AudioSettings::applyAll(SampleRate rate, BitDepth depth, BufferSize size) {
    sampleRate_.store(toInt(rate), std::memory_order_release);
    bitDepth_.store(toInt(depth), std::memory_order_release);
    bufferSize_.store(toInt(size), std::memory_order_release);
    notifyListeners();
}

void AudioSettings::resetToDefaults() {
    applyAll(kDefaultSampleRate, kDefaultBitDepth, kDefaultBufferSize);
}

// ── JUCE ValueTree serialisation ────────────────────────────────────────────

static const juce::Identifier kTreeType     { "AudioSettings" };
static const juce::Identifier kPropRate     { "sampleRate" };
static const juce::Identifier kPropBitDepth { "bitDepth" };
static const juce::Identifier kPropBuffer   { "bufferSize" };

juce::ValueTree AudioSettings::toValueTree() const {
    juce::ValueTree tree(kTreeType);
    tree.setProperty(kPropRate,     toInt(getSampleRate()), nullptr);
    tree.setProperty(kPropBitDepth, toInt(getBitDepth()),   nullptr);
    tree.setProperty(kPropBuffer,   toInt(getBufferSize()), nullptr);
    return tree;
}

void AudioSettings::fromValueTree(const juce::ValueTree& tree) {
    if (!tree.isValid() || tree.getType() != kTreeType) return;

    auto rate = sampleRateFromInt(static_cast<int>(tree.getProperty(kPropRate, toInt(kDefaultSampleRate))));
    auto bits = bitDepthFromInt(static_cast<int>(tree.getProperty(kPropBitDepth, toInt(kDefaultBitDepth))));
    auto buf  = bufferSizeFromInt(static_cast<int>(tree.getProperty(kPropBuffer, toInt(kDefaultBufferSize))));

    applyAll(rate.value_or(kDefaultSampleRate),
             bits.value_or(kDefaultBitDepth),
             buf.value_or(kDefaultBufferSize));
}

void AudioSettings::saveToFile(const juce::File& file) const {
    auto tree = toValueTree();
    auto xml  = tree.createXml();
    if (xml != nullptr)
        xml->writeTo(file);
}

bool AudioSettings::loadFromFile(const juce::File& file) {
    if (!file.existsAsFile()) return false;

    auto xml = juce::XmlDocument::parse(file);
    if (xml == nullptr) return false;

    auto tree = juce::ValueTree::fromXml(*xml);
    if (!tree.isValid()) return false;

    fromValueTree(tree);
    return true;
}

juce::File AudioSettings::getDefaultSettingsFile() {
    return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
               .getChildFile("REVITHION")
               .getChildFile("AudioSettings.xml");
}

// ── Listener management ─────────────────────────────────────────────────────

void AudioSettings::addListener(Listener* listener) {
    listeners_.add(listener);
}

void AudioSettings::removeListener(Listener* listener) {
    listeners_.remove(listener);
}

void AudioSettings::notifyListeners() {
    listeners_.call([this](Listener& l) { l.audioSettingsChanged(*this); });
}

} // namespace revithion::settings
