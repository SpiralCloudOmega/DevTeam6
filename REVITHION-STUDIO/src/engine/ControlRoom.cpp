#include "ControlRoom.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace revithion::engine {

static constexpr const char* kMonitorNames[] = {
    "Monitor A", "Monitor B", "Monitor C", "Monitor D"
};

ControlRoom::ControlRoom() {
    for (int i = 0; i < kMaxMonitors; ++i)
        monitors_[i].name = kMonitorNames[i];

    for (int i = 0; i < kMaxCueMixes; ++i)
        cueMixes_[i].name = "Cue " + std::to_string(i + 1);

    for (int i = 0; i < kMaxHeadphones; ++i) {
        headphones_[i].name = "Headphone " + std::to_string(i + 1);
        headphones_[i].sourceIndex = std::min(i, kMaxCueMixes - 1);
    }
}

ControlRoom::~ControlRoom() = default;

// --- Monitor management ---

MonitorOutput& ControlRoom::monitor(int index) {
    assert(index >= 0 && index < kMaxMonitors);
    return monitors_[index];
}

const MonitorOutput& ControlRoom::monitor(int index) const {
    assert(index >= 0 && index < kMaxMonitors);
    return monitors_[index];
}

int ControlRoom::activeMonitor() const { return activeMonitor_; }

void ControlRoom::setActiveMonitor(int index) {
    if (index < 0 || index >= kMaxMonitors)
        throw std::out_of_range("Monitor index out of range");
    activeMonitor_ = index;
}

void ControlRoom::toggleDim() {
    auto& m = monitors_[activeMonitor_];
    m.dimmed = !m.dimmed;
}

void ControlRoom::toggleMono() {
    auto& m = monitors_[activeMonitor_];
    m.mono = !m.mono;
}

void ControlRoom::setMonitorLevel(float db) {
    monitors_[activeMonitor_].level = db;
}

// --- Cue mixes ---

CueMix& ControlRoom::cueMix(int index) {
    assert(index >= 0 && index < kMaxCueMixes);
    return cueMixes_[index];
}

const CueMix& ControlRoom::cueMix(int index) const {
    assert(index >= 0 && index < kMaxCueMixes);
    return cueMixes_[index];
}

void ControlRoom::setCueMixLevel(int index, float db) {
    if (index < 0 || index >= kMaxCueMixes)
        throw std::out_of_range("Cue mix index out of range");
    cueMixes_[index].level = db;
}

// --- Headphones ---

HeadphoneOutput& ControlRoom::headphone(int index) {
    assert(index >= 0 && index < kMaxHeadphones);
    return headphones_[index];
}

void ControlRoom::setHeadphoneSource(int hpIndex, int cueIndex) {
    if (hpIndex < 0 || hpIndex >= kMaxHeadphones)
        throw std::out_of_range("Headphone index out of range");
    if (cueIndex < 0 || cueIndex >= kMaxCueMixes)
        throw std::out_of_range("Cue mix index out of range");
    headphones_[hpIndex].sourceIndex = cueIndex;
}

// --- Talkback ---

Talkback& ControlRoom::talkback() { return talkback_; }

void ControlRoom::talkbackOn() {
    talkback_.active = true;

    // Auto-dim active monitor when talkback engages
    if (talkback_.autoDim)
        monitors_[activeMonitor_].dimmed = true;

    // Route talkback to target cue mixes
    if (talkback_.toAllCues) {
        for (auto& cue : cueMixes_)
            cue.talkback = true;
    } else {
        for (int idx : talkback_.targetCues) {
            if (idx >= 0 && idx < kMaxCueMixes)
                cueMixes_[idx].talkback = true;
        }
    }
}

void ControlRoom::talkbackOff() {
    talkback_.active = false;

    // Restore monitor dim state
    if (talkback_.autoDim)
        monitors_[activeMonitor_].dimmed = false;

    // Clear talkback routing
    for (auto& cue : cueMixes_)
        cue.talkback = false;
}

void ControlRoom::toggleTalkback() {
    if (talkback_.active)
        talkbackOff();
    else
        talkbackOn();
}

// --- Reference tracks ---

void ControlRoom::addReference(const ReferenceTrack& ref) {
    if (static_cast<int>(references_.size()) >= kMaxReferences)
        throw std::runtime_error("Maximum reference tracks reached");
    references_.push_back(ref);
}

void ControlRoom::removeReference(int index) {
    if (index < 0 || index >= static_cast<int>(references_.size()))
        throw std::out_of_range("Reference index out of range");

    references_.erase(references_.begin() + index);

    // Adjust active reference after removal
    if (activeReference_ == index) {
        activeReference_ = -1;
        referenceActive_ = false;
    } else if (activeReference_ > index) {
        --activeReference_;
    }
}

ReferenceTrack& ControlRoom::reference(int index) {
    if (index < 0 || index >= static_cast<int>(references_.size()))
        throw std::out_of_range("Reference index out of range");
    return references_[index];
}

int ControlRoom::referenceCount() const {
    return static_cast<int>(references_.size());
}

void ControlRoom::setActiveReference(int index) {
    if (index < 0 || index >= static_cast<int>(references_.size()))
        throw std::out_of_range("Reference index out of range");

    // Deactivate previous reference
    if (activeReference_ >= 0 && activeReference_ < static_cast<int>(references_.size()))
        references_[activeReference_].active = false;

    activeReference_ = index;
    references_[index].active = referenceActive_;
}

int ControlRoom::activeReference() const { return activeReference_; }

void ControlRoom::toggleReference() {
    if (activeReference_ < 0 || activeReference_ >= static_cast<int>(references_.size()))
        return;

    referenceActive_ = !referenceActive_;
    references_[activeReference_].active = referenceActive_;
}

bool ControlRoom::isReferenceActive() const { return referenceActive_; }

// --- Global controls ---

float ControlRoom::masterLevel() const { return masterLevel_; }
void ControlRoom::setMasterLevel(float db) { masterLevel_ = db; }
bool ControlRoom::isMuted() const { return masterMute_; }
void ControlRoom::setMute(bool mute) { masterMute_ = mute; }

// --- Listen bus ---

float ControlRoom::listenLevel() const { return listenLevel_; }
void ControlRoom::setListenLevel(float db) { listenLevel_ = db; }
bool ControlRoom::listenEnabled() const { return listenEnabled_; }
void ControlRoom::setListenEnabled(bool e) { listenEnabled_ = e; }

} // namespace revithion::engine
