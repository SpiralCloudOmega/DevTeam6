// ============================================================================
// Multiband Compressor — 4-band with crossover network
// ============================================================================
declare name        "Multiband Compressor";
declare author      "SpiralCloudOmega";
declare license     "MIT";
declare description "4-band multiband compressor with linked stereo detection";

import("stdfaust.lib");

// ---------- global parameters -----------------------------------------------
bypass = checkbox("h:Multiband Comp/[0]Bypass");

// Crossover frequencies
xf1 = hslider("h:Multiband Comp/h:Crossover/[0]Low-LowMid [unit:Hz][style:knob]",
               200, 20, 1000, 1);
xf2 = hslider("h:Multiband Comp/h:Crossover/[1]LowMid-HiMid [unit:Hz][style:knob]",
               1500, 200, 5000, 1);
xf3 = hslider("h:Multiband Comp/h:Crossover/[2]HiMid-High [unit:Hz][style:knob]",
               5000, 2000, 18000, 1);

// Per-band compressor parameters
thresh(i) = hslider("h:Multiband Comp/h:Band %i/[0]Threshold [unit:dB][style:knob]",
                     -20, -60, 0, 0.1);
ratio(i)  = hslider("h:Multiband Comp/h:Band %i/[1]Ratio [style:knob]",
                     4, 1, 20, 0.1);
att(i)    = hslider("h:Multiband Comp/h:Band %i/[2]Attack [unit:ms][style:knob]",
                     10, 0.1, 100, 0.1) * 0.001;
rel(i)    = hslider("h:Multiband Comp/h:Band %i/[3]Release [unit:ms][style:knob]",
                     100, 10, 1000, 1) * 0.001;
makeup(i) = hslider("h:Multiband Comp/h:Band %i/[4]Makeup [unit:dB][style:knob]",
                     0, -20, 20, 0.1);

// ---------- crossover (Linkwitz-Riley 4th order) ----------------------------
// 1 input → 4 band outputs via cascaded crossover filters
split4 = _ <:
    fi.lowpass(4, xf1),
    (fi.highpass(4, xf1) : fi.lowpass(4, xf2)),
    (fi.highpass(4, xf1) : fi.highpass(4, xf2) : fi.lowpass(4, xf3)),
    (fi.highpass(4, xf1) : fi.highpass(4, xf2) : fi.highpass(4, xf3));

// ---------- per-band compressor (mono) with makeup gain ---------------------
comp_mono(i) = _ <: _, co.compression_gain_mono(ratio(i), thresh(i), att(i), rel(i))
               : * : *(ba.db2linear(makeup(i)));

// ---------- main process ----------------------------------------------------
// Stereo: split each channel into 4 bands, compress each, sum back
multiband_comp = par(c, 2, split4 : par(i, 4, comp_mono(i+1)) :> _);

process = ba.bypass2(bypass, multiband_comp);
