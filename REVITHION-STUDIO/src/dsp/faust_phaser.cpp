// ============================================================================
// Phaser — C++ Wrapper (Placeholder)
// Generated from Faust DSP — run faust2cpp when Faust is installed
//
// Faust compilation error: Stack overflow in eval — recursive allpass chain with feedback causes infinite expansion
// Source: faust/phaser.dsp
//
// Author: SpiralCloudOmega
// License: MIT
// Description: Classic phaser with 2-12 selectable stages and stereo width
// ============================================================================
#pragma once

#include <cstring>
#include <cmath>
#include <algorithm>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class Phaser {
public:
    // --- Parameter metadata ---
        // Parameter: "Bypass" min=0 max=1 default=0 step=1
        // Parameter: "Stages" min=1 max=6 default=4 step=1
        // Parameter: "Rate (Hz)" min=0.01 max=10 default=0.5 step=0.01
        // Parameter: "Depth (%)" min=0 max=100 default=70 step=0.1
        // Parameter: "Feedback (%)" min=-100 max=100 default=30 step=0.1
        // Parameter: "Stereo Width (%)" min=0 max=100 default=50 step=0.1
        // Parameter: "Mix (%)" min=0 max=100 default=50 step=0.1

    // --- Parameter storage ---
    struct Parameters {
        float bypass = 0f;
        float stages = 4f;
        float rate = 0.5f;
        float depth = 70f;
        float fb = 30f;
        float st_width = 50f;
        float mix = 50f;
    } params;

    Phaser() = default;
    ~Phaser() = default;

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
    static const char* getName() { return "Phaser"; }
    static const char* getDescription() { return "Classic phaser with 2-12 selectable stages and stereo width"; }

private:
    int fSampleRate = 44100;
};
