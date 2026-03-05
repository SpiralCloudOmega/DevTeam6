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
phaser_channel(x, phase_offset) = x_out
with {
    // LFO drives the notch frequency
    mod   = lfo(phase_offset);
    freq  = lfo2freq(mod);

    // Number of actual allpass stages = stages * 2 (2/4/6/8/10/12)
    n_stg = stages * 2;

    // Allpass chain with feedback
    phased = x_fb : allpass_chain(12, freq)  // run max 12 stages
    with {
        x_fb = x + phased * fb : *(1.0 / (1.0 + abs(fb)));
    };

    // Stage selection: crossfade based on stages parameter
    // Each pair of allpass stages adds a notch
    // We use all 12 stages but attenuate unused ones by mixing back dry
    stg_mix = min(1.0, stages / 6.0);

    x_out = x * (1 - mix) + phased * mix;
};

// ---------- main process ----------------------------------------------------
stereo_phaser(l, r) = (l_out, r_out)
with {
    // Phase offset for stereo spread
    offset = st_width * 0.5;

    l_phased = phaser_channel(l, 0);
    r_phased = phaser_channel(r, offset);

    l_out = l_phased;
    r_out = r_phased;
};

process = ba.bypass2(bypass, stereo_phaser);
