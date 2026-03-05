// ============================================================================
// Tape Delay — C++ Wrapper (Placeholder)
// Generated from Faust DSP — run faust2cpp when Faust is installed
//
// Faust compilation error: Endless evaluation cycle — recursive feedback with delay requires letrec pattern
// Source: faust/tape_delay.dsp
//
// Author: SpiralCloudOmega
// License: MIT
// Description: Stereo tape delay with modulation, saturation, and filters
// ============================================================================
#pragma once

#include <cstring>
#include <cmath>
#include <algorithm>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class TapeDelay {
public:
    // --- Parameter metadata ---
        // Parameter: "Bypass" min=0 max=1 default=0 step=1
        // Parameter: "Left Delay (ms)" min=10 max=2000 default=375 step=0.1
        // Parameter: "Right Delay (ms)" min=10 max=2000 default=500 step=0.1
        // Parameter: "Tempo Sync" min=0 max=1 default=0 step=1
        // Parameter: "BPM" min=40 max=240 default=120 step=0.1
        // Parameter: "Note Division" min=1 max=16 default=4 step=1
        // Parameter: "Feedback (%)" min=0 max=100 default=40 step=0.1
        // Parameter: "Mod Rate (Hz)" min=0.01 max=10 default=0.5 step=0.01
        // Parameter: "Mod Depth (%)" min=0 max=100 default=20 step=0.1
        // Parameter: "Feedback Low Cut (Hz)" min=20 max=2000 default=80 step=1
        // Parameter: "Feedback High Cut (Hz)" min=1000 max=20000 default=8000 step=1
        // Parameter: "Saturation Drive (%)" min=0 max=100 default=30 step=0.1
        // Parameter: "Mix (%)" min=0 max=100 default=30 step=0.1

    // --- Parameter storage ---
    struct Parameters {
        float bypass = 0f;
        float delay_l = 375f;
        float delay_r = 500f;
        float sync_on = 0f;
        float bpm = 120f;
        float sync_div = 4f;
        float feedback = 40f;
        float mod_rate = 0.5f;
        float mod_depth = 20f;
        float fb_locut = 80f;
        float fb_hicut = 8000f;
        float sat_drive = 30f;
        float mix = 30f;
    } params;

    TapeDelay() = default;
    ~TapeDelay() = default;

    /// Initialize DSP with sample rate
    void init(int sample_rate) {
        fSampleRate = sample_rate;
        instanceClear();
    }

    /// Reset internal state
    void instanceClear() {
        // TODO: Clear delay lines, filter states, etc.
    }

    /// Number of audio input channels
    static int getNumInputs() { return 2; }

    /// Number of audio output channels
    static int getNumOutputs() { return 2; }

    /// Process audio block (placeholder — passes through dry signal)
    void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
        // Placeholder: dry pass-through until Faust compilation is fixed
        for (int ch = 0; ch < 2; ++ch) {
            if (ch < 2) {
                std::memcpy(outputs[ch], inputs[ch], count * sizeof(FAUSTFLOAT));
            } else {
                std::memset(outputs[ch], 0, count * sizeof(FAUSTFLOAT));
            }
        }
    }

    /// Get metadata about this DSP
    static const char* getName() { return "Tape Delay"; }
    static const char* getDescription() { return "Stereo tape delay with modulation, saturation, and filters"; }

private:
    int fSampleRate = 44100;
};
