#pragma once
#include <memory>
#include <string>
#include <vector>

namespace revithion::engine {

// MIDI note event
struct MIDINote {
    int pitch = 60;          // 0-127
    int velocity = 100;      // 0-127
    double startTime = 0.0;  // PPQN position
    double duration = 480.0; // PPQN length
    int channel = 0;         // 0-15
};

// MIDI CC event
struct MIDICC {
    int controller = 0;  // 0-127
    int value = 0;        // 0-127
    double time = 0.0;    // PPQN position
    int channel = 0;      // 0-15
};

// A MIDI clip (collection of notes + CCs in a time range)
class MIDIClip {
public:
    MIDIClip(const std::string& name = "MIDI Clip");

    // Notes
    void addNote(const MIDINote& note);
    void removeNote(int index);
    MIDINote& note(int index);
    int noteCount() const;

    // CCs
    void addCC(const MIDICC& cc);
    int ccCount() const;

    // Time range
    double startTime() const;
    double endTime() const;
    double length() const;

    // Editing
    void transpose(int semitones);
    void quantize(double gridSize);  // e.g., 480 for quarter note at 480 PPQN
    void setVelocity(int velocity);  // Set all notes to same velocity
    void scaleVelocity(float factor);
    void reverse();

    // Properties
    const std::string& name() const;
    void setName(const std::string& n);

private:
    std::string name_;
    std::vector<MIDINote> notes_;
    std::vector<MIDICC> ccs_;
};

// MIDI effect base class
class MIDIEffect {
public:
    virtual ~MIDIEffect() = default;
    virtual void process(std::vector<MIDINote>& notes) = 0;
    virtual const std::string& name() const = 0;
    bool active = true;
};

// Arpeggiator MIDI effect
class ArpeggiatorEffect : public MIDIEffect {
public:
    enum class Pattern { Up, Down, UpDown, Random, Played };

    ArpeggiatorEffect();
    void process(std::vector<MIDINote>& notes) override;
    const std::string& name() const override;

    Pattern pattern = Pattern::Up;
    double rate = 120.0;  // PPQN per step
    int octaves = 1;      // 1-4 octave range
    float gate = 0.8f;    // Note length as fraction of step
    int swing = 0;        // -100 to +100

private:
    std::string name_{"Arpeggiator"};
};

// Chord MIDI effect (generates chords from single notes)
class ChordEffect : public MIDIEffect {
public:
    enum class ChordType { Major, Minor, Seventh, Diminished, Augmented, Sus2, Sus4, Power, Custom };

    ChordEffect();
    void process(std::vector<MIDINote>& notes) override;
    const std::string& name() const override;

    ChordType chordType = ChordType::Major;
    std::vector<int> customIntervals;  // Semitone offsets for Custom type

private:
    std::string name_{"Chord"};
};

// Velocity curve MIDI effect
class VelocityCurveEffect : public MIDIEffect {
public:
    VelocityCurveEffect();
    void process(std::vector<MIDINote>& notes) override;
    const std::string& name() const override;

    float curve = 1.0f;  // 0.1 = soft, 1.0 = linear, 3.0 = hard
    int min = 0;
    int max = 127;
    bool randomize = false;
    int randomRange = 10;

private:
    std::string name_{"Velocity Curve"};
};

// MIDI track — contains clips and MIDI effects chain
class MIDITrack {
public:
    MIDITrack(const std::string& name = "MIDI Track");

    // Clips
    void addClip(std::unique_ptr<MIDIClip> clip);
    MIDIClip* clip(int index);
    int clipCount() const;

    // MIDI effects chain (processed in order)
    void addEffect(std::unique_ptr<MIDIEffect> effect);
    MIDIEffect* effect(int index);
    int effectCount() const;

    // Get processed notes (apply all active effects)
    std::vector<MIDINote> getProcessedNotes(double startTime, double endTime);

    // Recording
    bool isRecording() const;
    void startRecording(double time);
    void stopRecording(double time);
    void recordNote(const MIDINote& note);

    // Properties
    const std::string& name() const;
    void setName(const std::string& n);
    int channel() const;
    void setChannel(int ch);

private:
    std::string name_;
    int channel_ = 0;
    std::vector<std::unique_ptr<MIDIClip>> clips_;
    std::vector<std::unique_ptr<MIDIEffect>> effects_;
    bool recording_ = false;
    double recordStartTime_ = 0.0;
    std::unique_ptr<MIDIClip> recordingClip_;
};

}  // namespace revithion::engine
