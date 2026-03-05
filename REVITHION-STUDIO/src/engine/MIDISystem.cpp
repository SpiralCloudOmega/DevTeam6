#include "MIDISystem.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace revithion::engine {

// ---------------------------------------------------------------------------
// MIDIClip
// ---------------------------------------------------------------------------

MIDIClip::MIDIClip(const std::string& name)
    : name_(name)
{
}

void MIDIClip::addNote(const MIDINote& note) {
    // Insert in sorted order by startTime
    auto it = std::lower_bound(notes_.begin(), notes_.end(), note,
        [](const MIDINote& a, const MIDINote& b) {
            return a.startTime < b.startTime;
        });
    notes_.insert(it, note);
}

void MIDIClip::removeNote(int index) {
    if (index >= 0 && index < static_cast<int>(notes_.size())) {
        notes_.erase(notes_.begin() + index);
    }
}

MIDINote& MIDIClip::note(int index) {
    return notes_.at(index);
}

int MIDIClip::noteCount() const {
    return static_cast<int>(notes_.size());
}

void MIDIClip::addCC(const MIDICC& cc) {
    auto it = std::lower_bound(ccs_.begin(), ccs_.end(), cc,
        [](const MIDICC& a, const MIDICC& b) {
            return a.time < b.time;
        });
    ccs_.insert(it, cc);
}

int MIDIClip::ccCount() const {
    return static_cast<int>(ccs_.size());
}

double MIDIClip::startTime() const {
    if (notes_.empty()) return 0.0;
    return notes_.front().startTime;
}

double MIDIClip::endTime() const {
    if (notes_.empty()) return 0.0;
    double latest = 0.0;
    for (const auto& n : notes_) {
        latest = std::max(latest, n.startTime + n.duration);
    }
    return latest;
}

double MIDIClip::length() const {
    return endTime() - startTime();
}

void MIDIClip::transpose(int semitones) {
    for (auto& n : notes_) {
        n.pitch = std::clamp(n.pitch + semitones, 0, 127);
    }
}

void MIDIClip::quantize(double gridSize) {
    if (gridSize <= 0.0) return;
    for (auto& n : notes_) {
        n.startTime = std::round(n.startTime / gridSize) * gridSize;
    }
    // Re-sort after quantization
    std::sort(notes_.begin(), notes_.end(),
        [](const MIDINote& a, const MIDINote& b) {
            return a.startTime < b.startTime;
        });
}

void MIDIClip::setVelocity(int velocity) {
    int v = std::clamp(velocity, 0, 127);
    for (auto& n : notes_) {
        n.velocity = v;
    }
}

void MIDIClip::scaleVelocity(float factor) {
    for (auto& n : notes_) {
        int v = static_cast<int>(std::round(n.velocity * factor));
        n.velocity = std::clamp(v, 0, 127);
    }
}

void MIDIClip::reverse() {
    if (notes_.size() < 2) return;

    double start = startTime();
    double end = endTime();

    for (auto& n : notes_) {
        // Mirror the note's end position to become the new start
        n.startTime = end - (n.startTime - start) - n.duration;
    }

    // Re-sort by startTime
    std::sort(notes_.begin(), notes_.end(),
        [](const MIDINote& a, const MIDINote& b) {
            return a.startTime < b.startTime;
        });
}

const std::string& MIDIClip::name() const {
    return name_;
}

void MIDIClip::setName(const std::string& n) {
    name_ = n;
}

// ---------------------------------------------------------------------------
// ArpeggiatorEffect
// ---------------------------------------------------------------------------

ArpeggiatorEffect::ArpeggiatorEffect() = default;

const std::string& ArpeggiatorEffect::name() const {
    return name_;
}

void ArpeggiatorEffect::process(std::vector<MIDINote>& notes) {
    if (notes.empty() || rate <= 0.0) return;

    // Collect unique pitches from input
    std::vector<int> pitches;
    pitches.reserve(notes.size());
    for (const auto& n : notes) {
        pitches.push_back(n.pitch);
    }
    std::sort(pitches.begin(), pitches.end());
    pitches.erase(std::unique(pitches.begin(), pitches.end()), pitches.end());

    if (pitches.empty()) return;

    // Build pitch sequence with octave expansion
    std::vector<int> sequence;
    for (int oct = 0; oct < octaves; ++oct) {
        for (int p : pitches) {
            int expanded = p + oct * 12;
            if (expanded <= 127) {
                sequence.push_back(expanded);
            }
        }
    }
    if (sequence.empty()) return;

    // Apply pattern ordering
    std::vector<int> ordered;
    switch (pattern) {
        case Pattern::Up:
            ordered = sequence;
            break;
        case Pattern::Down:
            ordered = sequence;
            std::reverse(ordered.begin(), ordered.end());
            break;
        case Pattern::UpDown:
            ordered = sequence;
            if (sequence.size() > 1) {
                for (int i = static_cast<int>(sequence.size()) - 2; i >= 0; --i) {
                    ordered.push_back(sequence[i]);
                }
            }
            break;
        case Pattern::Random: {
            ordered = sequence;
            std::mt19937 rng(42);  // Deterministic for reproducibility
            std::shuffle(ordered.begin(), ordered.end(), rng);
            break;
        }
        case Pattern::Played:
            // Original order from input
            for (const auto& n : notes) {
                ordered.push_back(n.pitch);
            }
            // Expand with octaves
            if (octaves > 1) {
                auto base = ordered;
                for (int oct = 1; oct < octaves; ++oct) {
                    for (int p : base) {
                        int expanded = p + oct * 12;
                        if (expanded <= 127) ordered.push_back(expanded);
                    }
                }
            }
            break;
    }

    if (ordered.empty()) return;

    // Determine time range from original notes
    double earliest = notes.front().startTime;
    double latest = 0.0;
    for (const auto& n : notes) {
        earliest = std::min(earliest, n.startTime);
        latest = std::max(latest, n.startTime + n.duration);
    }
    double totalDuration = latest - earliest;
    if (totalDuration <= 0.0) totalDuration = rate * ordered.size();

    int baseVelocity = notes.front().velocity;
    int baseCh = notes.front().channel;

    // Generate arpeggio notes
    notes.clear();
    int stepCount = static_cast<int>(totalDuration / rate);
    if (stepCount < 1) stepCount = 1;

    for (int i = 0; i < stepCount; ++i) {
        int idx = i % static_cast<int>(ordered.size());
        double swingOffset = (i % 2 == 1) ? (swing / 100.0) * (rate * 0.5) : 0.0;

        MIDINote arp;
        arp.pitch = std::clamp(ordered[idx], 0, 127);
        arp.velocity = baseVelocity;
        arp.startTime = earliest + i * rate + swingOffset;
        arp.duration = rate * gate;
        arp.channel = baseCh;
        notes.push_back(arp);
    }
}

// ---------------------------------------------------------------------------
// ChordEffect
// ---------------------------------------------------------------------------

ChordEffect::ChordEffect() = default;

const std::string& ChordEffect::name() const {
    return name_;
}

void ChordEffect::process(std::vector<MIDINote>& notes) {
    if (notes.empty()) return;

    // Determine intervals based on chord type
    std::vector<int> intervals;
    switch (chordType) {
        case ChordType::Major:      intervals = {4, 7};         break;
        case ChordType::Minor:      intervals = {3, 7};         break;
        case ChordType::Seventh:    intervals = {4, 7, 10};     break;
        case ChordType::Diminished: intervals = {3, 6};         break;
        case ChordType::Augmented:  intervals = {4, 8};         break;
        case ChordType::Sus2:       intervals = {2, 7};         break;
        case ChordType::Sus4:       intervals = {5, 7};         break;
        case ChordType::Power:      intervals = {7};            break;
        case ChordType::Custom:     intervals = customIntervals; break;
    }

    // Add chord tones for each existing note
    std::vector<MIDINote> chordNotes;
    chordNotes.reserve(notes.size() * (1 + intervals.size()));

    for (const auto& n : notes) {
        chordNotes.push_back(n);  // Keep root
        for (int interval : intervals) {
            int chordPitch = n.pitch + interval;
            if (chordPitch <= 127) {
                MIDINote cn = n;
                cn.pitch = chordPitch;
                chordNotes.push_back(cn);
            }
        }
    }

    notes = std::move(chordNotes);
}

// ---------------------------------------------------------------------------
// VelocityCurveEffect
// ---------------------------------------------------------------------------

VelocityCurveEffect::VelocityCurveEffect() = default;

const std::string& VelocityCurveEffect::name() const {
    return name_;
}

void VelocityCurveEffect::process(std::vector<MIDINote>& notes) {
    static thread_local std::mt19937 rng{std::random_device{}()};

    for (auto& n : notes) {
        // Normalize velocity to 0..1
        float normalized = n.velocity / 127.0f;

        // Apply power curve
        normalized = std::pow(normalized, curve);

        // Map to min..max range
        int mapped = min + static_cast<int>(std::round(normalized * (max - min)));

        // Optional randomization
        if (randomize && randomRange > 0) {
            std::uniform_int_distribution<int> dist(-randomRange, randomRange);
            mapped += dist(rng);
        }

        n.velocity = std::clamp(mapped, 0, 127);
    }
}

// ---------------------------------------------------------------------------
// MIDITrack
// ---------------------------------------------------------------------------

MIDITrack::MIDITrack(const std::string& name)
    : name_(name)
{
}

void MIDITrack::addClip(std::unique_ptr<MIDIClip> clip) {
    clips_.push_back(std::move(clip));
}

MIDIClip* MIDITrack::clip(int index) {
    if (index >= 0 && index < static_cast<int>(clips_.size())) {
        return clips_[index].get();
    }
    return nullptr;
}

int MIDITrack::clipCount() const {
    return static_cast<int>(clips_.size());
}

void MIDITrack::addEffect(std::unique_ptr<MIDIEffect> effect) {
    effects_.push_back(std::move(effect));
}

MIDIEffect* MIDITrack::effect(int index) {
    if (index >= 0 && index < static_cast<int>(effects_.size())) {
        return effects_[index].get();
    }
    return nullptr;
}

int MIDITrack::effectCount() const {
    return static_cast<int>(effects_.size());
}

std::vector<MIDINote> MIDITrack::getProcessedNotes(double startTime, double endTime) {
    // Gather notes from all clips within the time range
    std::vector<MIDINote> result;
    for (const auto& c : clips_) {
        for (int i = 0; i < c->noteCount(); ++i) {
            const auto& n = const_cast<MIDIClip*>(c.get())->note(i);
            double noteEnd = n.startTime + n.duration;
            // Include notes that overlap with the requested range
            if (n.startTime < endTime && noteEnd > startTime) {
                result.push_back(n);
            }
        }
    }

    // Apply effects chain in order
    for (const auto& fx : effects_) {
        if (fx->active) {
            fx->process(result);
        }
    }

    return result;
}

bool MIDITrack::isRecording() const {
    return recording_;
}

void MIDITrack::startRecording(double time) {
    if (recording_) return;
    recording_ = true;
    recordStartTime_ = time;
    recordingClip_ = std::make_unique<MIDIClip>("Recording");
}

void MIDITrack::stopRecording(double time) {
    if (!recording_) return;
    recording_ = false;

    if (recordingClip_ && recordingClip_->noteCount() > 0) {
        // Finalize the clip name with time range
        recordingClip_->setName("Recorded " + std::to_string(static_cast<int>(recordStartTime_))
                                + "-" + std::to_string(static_cast<int>(time)));
        clips_.push_back(std::move(recordingClip_));
    }
    recordingClip_.reset();
}

void MIDITrack::recordNote(const MIDINote& note) {
    if (!recording_ || !recordingClip_) return;
    recordingClip_->addNote(note);
}

const std::string& MIDITrack::name() const {
    return name_;
}

void MIDITrack::setName(const std::string& n) {
    name_ = n;
}

int MIDITrack::channel() const {
    return channel_;
}

void MIDITrack::setChannel(int ch) {
    channel_ = std::clamp(ch, 0, 15);
}

}  // namespace revithion::engine
