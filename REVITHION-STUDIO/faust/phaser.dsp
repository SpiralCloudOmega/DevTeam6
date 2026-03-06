// ============================================================================
// Phaser — classic analog-style phaser with selectable stages
// ============================================================================
declare name        "Phaser";
declare author      "SpiralCloudOmega";
declare license     "MIT";
declare description "Classic phaser with 2-12 selectable stages and stereo width";

import("stdfaust.lib");

// ---------- parameters ------------------------------------------------------
bypass = checkbox("h:Phaser/[0]Bypass");

stages    = nentry("h:Phaser/h:Main/[0]Stages", 4, 1, 6, 1);  // 1=2stg, 2=4stg, …6=12stg
rate      = hslider("h:Phaser/h:Main/[1]Rate [unit:Hz][style:knob]",
                     0.5, 0.01, 10, 0.01);
depth     = hslider("h:Phaser/h:Main/[2]Depth [style:knob]",
                     70, 0, 100, 0.1) * 0.01;
fb        = hslider("h:Phaser/h:Main/[3]Feedback [style:knob]",
                     30, -100, 100, 0.1) * 0.01;
st_width  = hslider("h:Phaser/h:Main/[4]Stereo Width [style:knob]",
                     50, 0, 100, 0.1) * 0.01;
mix       = hslider("h:Phaser/[9]Mix [style:knob]", 50, 0, 100, 0.1) * 0.01;

// ---------- helpers ---------------------------------------------------------
// Phaser frequency range
F_MIN = 100;
F_MAX = 4000;

// LFO (triangle wave for smoother sweep)
lfo(phase_offset) = os.osc(rate + phase_offset * 0.001) * 0.5 + 0.5;

// Map LFO to frequency range (logarithmic)
lfo2freq(lfo_val) = F_MIN * pow(F_MAX / F_MIN, lfo_val * depth);

// Single first-order allpass stage
allpass1(fc) = fi.iir((a, 1.0), (1.0, a))
with {
    w = 2.0 * ma.PI * fc / ma.SR;
    a = (1.0 - sin(w)) / cos(w);
};

// N allpass stages in series at a given centre frequency
allpass_chain(n, fc) = seq(i, n, allpass1(fc));

// ---------- phaser channel --------------------------------------------------
phaser_channel(phase_offset) = _ <: *(1 - mix), phased_path : +
with {
    // LFO drives the notch frequency
    mod  = lfo(phase_offset);
    freq = lfo2freq(mod);

    // Feedback normalization
    norm = 1.0 / (1.0 + abs(fb));

    // Allpass chain with feedback using ~ operator
    // 12 cascaded allpass stages (max) with feedback loop
    phased_path = (+ : *(norm) : allpass_chain(12, freq)) ~ (*(fb)) : *(mix);
};

// ---------- main process ----------------------------------------------------
stereo_phaser = _, _ :
    (phaser_channel(0), phaser_channel(offset))
with {
    // Phase offset for stereo spread
    offset = st_width * 0.5;
};

process = ba.bypass2(bypass, stereo_phaser);
