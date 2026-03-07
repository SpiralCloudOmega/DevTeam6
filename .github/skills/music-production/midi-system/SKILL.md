---
name: midi-system
description: MIDI engine for REVITHION STUDIO — messages, clock, sync, MPE, MIDI 2.0, recording, quantization, humanization via JUCE C++.
---

# MIDI System — DAW Engine Core

Complete MIDI subsystem for a JUCE-based DAW covering the full spec through MIDI 2.0.
## MIDI Messages
```cpp
auto noteOn  = juce::MidiMessage::noteOn(ch, note, (juce::uint8) vel); // note/vel 0-127, ch 1-16
auto noteOff = juce::MidiMessage::noteOff(ch, note, (juce::uint8) 0);
auto chanAT  = juce::MidiMessage::channelPressureChange(ch, pressure); // channel aftertouch
auto polyAT  = juce::MidiMessage::aftertouchChange(ch, note, pressure);// polyphonic aftertouch
auto cc    = juce::MidiMessage::controllerEvent(ch, 1, value); // CC1=mod,7=vol,10=pan,11=expr,64=sus
auto pb    = juce::MidiMessage::pitchWheel(ch, 8192 + bend);  // 14-bit, center=8192
auto pc    = juce::MidiMessage::programChange(ch, program);
auto sysex = juce::MidiMessage::createSysExMessage(data, len); // SysEx (without F0/F7 framing)
```
## NRPN/RPN — Extended Control
```cpp
void sendNRPN(juce::MidiBuffer& buf, int ch, int pMSB, int pLSB, int vMSB, int vLSB, int s) {
    buf.addEvent(juce::MidiMessage::controllerEvent(ch, 99, pMSB), s); // NRPN param MSB
    buf.addEvent(juce::MidiMessage::controllerEvent(ch, 98, pLSB), s); // NRPN param LSB
    buf.addEvent(juce::MidiMessage::controllerEvent(ch, 6,  vMSB), s); // Data Entry MSB
    buf.addEvent(juce::MidiMessage::controllerEvent(ch, 38, vLSB), s); // Data Entry LSB
} // RPN: use CC101/CC100 instead of CC99/CC98
```
## MIDI Clock — 24 PPQ
```cpp
void sendMidiClock(juce::MidiBuffer& buf, double bpm, double sr, int numSamples) {
    double samplesPerTick = sr / ((bpm / 60.0) * 24.0);
    for (double pos = tickAccum; pos < numSamples; pos += samplesPerTick)
        buf.addEvent(juce::MidiMessage::midiClock(), (int) pos);
    tickAccum = std::fmod(tickAccum + numSamples, samplesPerTick);
} // Receive: BPM = 60.0 / (avgTickInterval * 24.0 / sampleRate)
```
## MTC and MMC — Sync and Transport
```cpp
// MTC: quarter-frame msgs encode SMPTE (24/25/29.97/30 fps) across 8 messages
auto qf = juce::MidiMessage::quarterFrame(seqIndex, value); // status 0xF1
int total = (int)(samplePos / sr * 30.0); // sample→SMPTE at 30fps
int fr = total % 30, sec = (total/30)%60, min = (total/1800)%60, hr = (total/108000)%24;
// MMC SysEx: F0 7F <dev> 06 <cmd> F7 — 01=Stop 02=Play 04=FF 05=Rew 06=Rec 44=Locate
auto stop = juce::MidiMessage(0xF0, 0x7F, 0x7F, 0x06, 0x01, 0xF7);
auto play = juce::MidiMessage(0xF0, 0x7F, 0x7F, 0x06, 0x02, 0xF7);
auto rec  = juce::MidiMessage(0xF0, 0x7F, 0x7F, 0x06, 0x06, 0xF7);
```
## MPE — Polyphonic Expression
```cpp
juce::MPEZoneLayout layout;
layout.setLowerZone(15); // channels 2-16, master ch 1
juce::MPEInstrument instrument(layout);
// Per-note expression in a synth voice
void handleMPENote(const juce::MPENote& note) {
    float pitch  = note.noteID + note.totalPitchbendInSemitones;
    float slide  = note.timbre.asUnsignedFloat();         // CC74
    float press  = note.pressure.asUnsignedFloat();       // per-note aftertouch
}
```
## MIDI 2.0 — Universal MIDI Packets
- **32-bit velocity** (4B levels vs 128), **per-note CC/pitch bend** without MPE channel tricks
- **Attribute types** for articulation/tuning in note-on; **bidirectional negotiation** via Property Exchange
- JUCE 8: `juce::universal_midi_packets`. REVITHION uses MIDI 2.0 internally, MIDI 1.0 for I/O.
## MidiBuffer, Recording, and Playback
```cpp
void processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi) {
    for (const auto meta : midi) {
        auto msg = meta.getMessage(); int pos = meta.samplePosition;
        if (msg.isNoteOn()) handleNoteOn(msg, pos);
        else if (msg.isNoteOff()) handleNoteOff(msg, pos);
        else if (msg.isController()) handleCC(msg, pos);
    }
    midi.swapWith(outputBuffer); // swap in generated output
}
// Recording: append timestamped events, link note-on/off pairs
juce::MidiMessageSequence seq;
void record(const juce::MidiMessage& msg, double t) {
    seq.addEvent(juce::MidiMessage(msg, t)); seq.updateMatchedPairs();
}
// Playback: emit events falling within current block's time window
void playback(juce::MidiBuffer& buf, double start, double end, double sr) {
    for (int i = 0; i < seq.getNumEvents(); ++i) {
        double t = seq.getEventPointer(i)->message.getTimeStamp();
        if (t >= start && t < end)
            buf.addEvent(seq.getEventPointer(i)->message, (int)((t - start) * sr));
    }
}
```
## Quantization
```cpp
double quantize(double tick, double grid, double strength = 1.0) {
    double nearest = std::round(tick / grid) * grid;
    return tick + (nearest - tick) * strength;
}
// Grid at 960 PPQ: 1/4=960 1/8=480 1/16=240 1/32=120 | triplets: 640 320 160 80
double swingQuantize(double tick, double grid, double swingPct) {
    bool offbeat = std::fmod(tick, grid * 2.0) >= grid;
    return quantize(tick, grid) + (offbeat ? grid * (swingPct - 0.5) * 2.0 : 0.0);
} // swingPct: 0.5=straight, 0.66=triplet feel
```
## Humanization
```cpp
void humanize(juce::MidiMessageSequence& s, double timeTicks, int velAmt, juce::Random& rng) {
    for (int i = 0; i < s.getNumEvents(); ++i) {
        auto& m = s.getEventPointer(i)->message;  if (!m.isNoteOn()) continue;
        m.setTimeStamp(std::max(0.0, m.getTimeStamp() + (rng.nextDouble()*2-1) * timeTicks));
        int v = juce::jlimit(1, 127, m.getVelocity() + (int)((rng.nextDouble()*2-1) * velAmt));
        m = juce::MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), (juce::uint8)v);
    }
} // Typical: timeTicks=10-30, velAmt=5-15
```
## Anti-Patterns
- **Allocating in audio thread** — no `new`/`push_back` in processBlock; use pre-allocated MidiBuffer
- **Sample position = 0 for all events** — causes timing smear; compute correct offset per block
- **Hardcoding channel 1** — breaks MPE/multi-timbral; propagate original channel
- **Blocking MIDI I/O** — never `sendMessageNow()` on audio thread; queue for dedicated thread
- **Orphaned note-ons** — always track/flush pending note-offs on transport stop
## Checklist
- [ ] All message types: Note, AT, CC, PitchBend, ProgramChange, SysEx, System
- [ ] NRPN/RPN with CC99/98/6/38 sequencing; MIDI clock at 24 PPQ synced to tempo
- [ ] MTC quarter-frame SMPTE (24/25/29.97/30 fps); MMC transport forwarding
- [ ] MPE zones with per-note pitch/slide/pressure; MIDI 2.0 UMP on internal bus
- [ ] MidiBuffer iteration with correct sample offsets in processBlock
- [ ] Recording to MidiMessageSequence with matched pairs; sample-accurate playback
- [ ] Quantize: grid sizes, strength, swing; Humanize: timing + velocity jitter
- [ ] Zero audio-thread allocations for all MIDI operations
