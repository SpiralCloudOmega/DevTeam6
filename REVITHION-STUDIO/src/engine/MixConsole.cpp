#include "MixConsole.h"
#include <algorithm>

namespace revithion::engine {

const char* trackTypeName(TrackType type) {
    switch (type) {
        case TrackType::Audio:      return "Audio";
        case TrackType::Instrument: return "Instrument";
        case TrackType::Sampler:    return "Sampler";
        case TrackType::Drum:       return "Drum";
        case TrackType::MIDI:       return "MIDI";
        case TrackType::Group:      return "Group";
        case TrackType::FX:         return "FX";
        case TrackType::VCA:        return "VCA";
        case TrackType::Ruler:      return "Ruler";
        case TrackType::Marker:     return "Marker";
        case TrackType::Arranger:   return "Arranger";
        case TrackType::Tempo:      return "Tempo";
        case TrackType::Signature:  return "Signature";
        case TrackType::Transpose:  return "Transpose";
        case TrackType::Chord:      return "Chord";
        case TrackType::Folder:     return "Folder";
    }
    return "Unknown";
}

// --- MixChannel ---

MixChannel::MixChannel(const std::string& name, TrackType type, UUID id)
    : id_(id.isNull() ? UUID::generate() : id)
    , name_(name)
    , type_(type)
{
}

// --- MixConsole ---

MixConsole::MixConsole() {
    masterBus_ = std::make_unique<MixChannel>("Stereo Out", TrackType::Group);
}

MixConsole::~MixConsole() = default;

MixChannel* MixConsole::addChannel(const std::string& name, TrackType type) {
    auto ch = std::make_unique<MixChannel>(name, type);
    auto* ptr = ch.get();
    channels_.push_back(std::move(ch));
    return ptr;
}

void MixConsole::removeChannel(const UUID& id) {
    auto it = std::remove_if(channels_.begin(), channels_.end(),
        [&id](const std::unique_ptr<MixChannel>& ch) { return ch->id() == id; });
    channels_.erase(it, channels_.end());
}

MixChannel* MixConsole::channel(const UUID& id) {
    for (auto& ch : channels_) {
        if (ch->id() == id) return ch.get();
    }
    return nullptr;
}

MixChannel* MixConsole::channelByIndex(int index) {
    if (index < 0 || index >= static_cast<int>(channels_.size())) return nullptr;
    return channels_[index].get();
}

int MixConsole::channelCount() const {
    return static_cast<int>(channels_.size());
}

void MixConsole::forEachChannel(std::function<void(MixChannel*)> fn) {
    for (auto& ch : channels_) fn(ch.get());
}

void MixConsole::forEachChannel(std::function<void(const MixChannel*)> fn) const {
    for (const auto& ch : channels_) fn(ch.get());
}

bool MixConsole::hasSoloedChannels() const {
    for (const auto& ch : channels_) {
        if (ch->solo()) return true;
    }
    return false;
}

void MixConsole::clearAllSolos() {
    for (auto& ch : channels_) {
        ch->setSolo(false);
    }
}

} // namespace revithion::engine
