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
// Split a mono signal into 4 bands using cascaded crossovers
split4(x) = (b1, b2, b3, b4)
with {
    lo  = fi.lowpass(4, xf1, x);
    hi  = fi.highpass(4, xf1, x);
    mid_lo = fi.lowpass(4, xf2, hi);
    mid_hi = fi.highpass(4, xf2, hi);
    b1 = lo;
    b2 = mid_lo;
    b3 = fi.lowpass(4, xf3, mid_hi);
    b4 = fi.highpass(4, xf3, mid_hi);
};

// ---------- per-band compressor with linked stereo detection -----------------
// Takes left band, right band; returns compressed left, right
comp_band(i, l, r) = (l_out, r_out)
with {
    // Linked detection: average of both channels
    detector = 0.5 * (abs(l) + abs(r));
    env      = co.compression_gain_mono(ratio(i), thresh(i), att(i), rel(i), detector);
    gain_lin = env * ba.db2linear(makeup(i));
    l_out    = l * gain_lin;
    r_out    = r * gain_lin;
};

// ---------- main process ----------------------------------------------------
multiband_comp(l, r) = (l_sum, r_sum)
with {
    // Split each channel into 4 bands
    (l1, l2, l3, l4) = split4(l);
    (r1, r2, r3, r4) = split4(r);

    // Compress each band with linked detection
    (lc1, rc1) = comp_band(1, l1, r1);
    (lc2, rc2) = comp_band(2, l2, r2);
    (lc3, rc3) = comp_band(3, l3, r3);
    (lc4, rc4) = comp_band(4, l4, r4);

    // Sum bands back together
    l_sum = lc1 + lc2 + lc3 + lc4;
    r_sum = rc1 + rc2 + rc3 + rc4;
};

process = ba.bypass2(bypass, multiband_comp);
