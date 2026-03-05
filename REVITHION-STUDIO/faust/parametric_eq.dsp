// ============================================================================
// Parametric EQ — 4-band fully parametric equalizer
// ============================================================================
declare name        "Parametric EQ";
declare author      "SpiralCloudOmega";
declare license     "MIT";
declare description "4-band fully parametric EQ (low shelf, 2 peaking, high shelf)";

import("stdfaust.lib");

// ---------- parameters ------------------------------------------------------
bypass = checkbox("h:Parametric EQ/[0]Bypass");

// Band 1 — Low Shelf
ls_freq = hslider("h:Parametric EQ/h:Band 1 Low Shelf/[0]Freq [unit:Hz][style:knob]",
                   100, 20, 500, 0.1);
ls_gain = hslider("h:Parametric EQ/h:Band 1 Low Shelf/[1]Gain [unit:dB][style:knob]",
                   0, -15, 15, 0.1);
ls_q    = hslider("h:Parametric EQ/h:Band 1 Low Shelf/[2]Q [style:knob]",
                   0.707, 0.1, 10, 0.01);

// Band 2 — Peaking
p1_freq = hslider("h:Parametric EQ/h:Band 2 Peak/[0]Freq [unit:Hz][style:knob]",
                   500, 100, 5000, 0.1);
p1_gain = hslider("h:Parametric EQ/h:Band 2 Peak/[1]Gain [unit:dB][style:knob]",
                   0, -15, 15, 0.1);
p1_q    = hslider("h:Parametric EQ/h:Band 2 Peak/[2]Q [style:knob]",
                   1.0, 0.1, 10, 0.01);

// Band 3 — Peaking
p2_freq = hslider("h:Parametric EQ/h:Band 3 Peak/[0]Freq [unit:Hz][style:knob]",
                   2000, 500, 15000, 0.1);
p2_gain = hslider("h:Parametric EQ/h:Band 3 Peak/[1]Gain [unit:dB][style:knob]",
                   0, -15, 15, 0.1);
p2_q    = hslider("h:Parametric EQ/h:Band 3 Peak/[2]Q [style:knob]",
                   1.0, 0.1, 10, 0.01);

// Band 4 — High Shelf
hs_freq = hslider("h:Parametric EQ/h:Band 4 High Shelf/[0]Freq [unit:Hz][style:knob]",
                   8000, 2000, 20000, 0.1);
hs_gain = hslider("h:Parametric EQ/h:Band 4 High Shelf/[1]Gain [unit:dB][style:knob]",
                   0, -15, 15, 0.1);
hs_q    = hslider("h:Parametric EQ/h:Band 4 High Shelf/[2]Q [style:knob]",
                   0.707, 0.1, 10, 0.01);

// ---------- DSP chain -------------------------------------------------------
eq_chain = low_shelf : peak1 : peak2 : high_shelf
with {
    low_shelf  = fi.low_shelf(ls_gain, ls_freq);
    peak1      = fi.peak_eq(p1_gain, p1_freq, p1_freq / p1_q);
    peak2      = fi.peak_eq(p2_gain, p2_freq, p2_freq / p2_q);
    high_shelf = fi.high_shelf(hs_gain, hs_freq);
};

// Stereo processing with bypass
process = ba.bypass2(bypass, (eq_chain, eq_chain));
