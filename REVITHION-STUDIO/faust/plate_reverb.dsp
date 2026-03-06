// ============================================================================
// Plate Reverb — Dattorro-style plate reverb
// ============================================================================
declare name        "Plate Reverb";
declare author      "SpiralCloudOmega";
declare license     "MIT";
declare description "Plate reverb based on Dattorro topology with allpass and comb filters";

import("stdfaust.lib");

// ---------- parameters ------------------------------------------------------
bypass = checkbox("h:Plate Reverb/[0]Bypass");

decay     = hslider("h:Plate Reverb/h:Main/[0]Decay [unit:s][style:knob]",
                     2.0, 0.1, 10, 0.01);
predelay  = hslider("h:Plate Reverb/h:Main/[1]Pre-Delay [unit:ms][style:knob]",
                     20, 0, 200, 0.1);
damping   = hslider("h:Plate Reverb/h:Main/[2]Damping [style:knob]",
                     0.5, 0, 1, 0.01);
size      = hslider("h:Plate Reverb/h:Main/[3]Size [style:knob]",
                     0.7, 0.1, 1, 0.01);
diffusion = hslider("h:Plate Reverb/h:Main/[4]Diffusion [style:knob]",
                     0.7, 0, 1, 0.01);

// Filters
locut = hslider("h:Plate Reverb/h:Filters/[0]Low Cut [unit:Hz][style:knob]",
                 80, 20, 1000, 1);
hicut = hslider("h:Plate Reverb/h:Filters/[1]High Cut [unit:Hz][style:knob]",
                 10000, 2000, 20000, 1);

// Mix
mix = hslider("h:Plate Reverb/[9]Mix [style:knob]", 30, 0, 100, 0.1) * 0.01;

// ---------- helpers ---------------------------------------------------------
ms2samp(ms) = ms * 0.001 * ma.SR;

// Scale delay lengths by size parameter
sd(n) = int(n * size);

// One-pole damping filter (low-pass)
damp(g) = _ * (1.0 - g) : + ~ (_ * g);

// ---------- input diffusion (4 cascaded allpass filters) --------------------
input_diffusion = ap1 : ap2 : ap3 : ap4
with {
    coeff = diffusion * 0.75;
    ap1 = fi.allpass_comb(sd(142), sd(142), coeff);
    ap2 = fi.allpass_comb(sd(107), sd(107), coeff);
    ap3 = fi.allpass_comb(sd(379), sd(379), coeff);
    ap4 = fi.allpass_comb(sd(277), sd(277), coeff);
};

// ---------- Dattorro tank (two cross-coupled delay loops) -------------------
// Feedback coefficient derived from decay time
fb_coef = min(0.98, decay / 10.0);

// Left processing chain
left_chain =
    fi.allpass_comb(672, sd(672), -diffusion * 0.6) :
    de.delay(8192, sd(4453)) :
    damp(damping) :
    fi.allpass_comb(1800, sd(1800), diffusion * 0.6) :
    de.delay(8192, sd(3720));

// Right processing chain
right_chain =
    fi.allpass_comb(908, sd(908), -diffusion * 0.6) :
    de.delay(8192, sd(4217)) :
    damp(damping) :
    fi.allpass_comb(2656, sd(2656), diffusion * 0.6) :
    de.delay(8192, sd(3163));

// Cross-coupled feedback: swap channels and scale by fb_coef
cross_fb = route(2, 2, (2,1), (1,2)) : (*(fb_coef), *(fb_coef));

// Tank core: (fb_l, fb_r, x, x) → add feedback to input → process chains
tank_core = route(4, 4, (1,1), (3,2), (2,3), (4,4)) : (+, +) : (left_chain, right_chain);

// Complete tank: mono input → stereo output
tank = _ <: _, _ : tank_core ~ cross_fb;

// ---------- main process ----------------------------------------------------
// Reverb input chain: mono sum → pre-delay → filtering → diffusion → tank
reverb_chain = + : *(0.5)
    : de.delay(19201, ms2samp(predelay))
    : fi.highpass(2, locut) : fi.lowpass(2, hicut)
    : input_diffusion : tank;

// Stereo dry/wet mix: (dry_l, dry_r, wet_l, wet_r) → (l_out, r_out)
plate_reverb = _, _ <:
    (*(1 - mix), *(1 - mix)),
    (reverb_chain : (*(mix), *(mix)))
    : route(4, 4, (1,1), (3,2), (2,3), (4,4)) : (+, +);

process = ba.bypass2(bypass, plate_reverb);
