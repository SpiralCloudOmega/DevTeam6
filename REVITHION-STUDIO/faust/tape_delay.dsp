// ============================================================================
// Tape Delay — stereo delay with modulation, saturation, and filtered feedback
// ============================================================================
declare name        "Tape Delay";
declare author      "SpiralCloudOmega";
declare license     "MIT";
declare description "Stereo tape delay with modulation, saturation, and filters";

import("stdfaust.lib");

// ---------- parameters ------------------------------------------------------
bypass = checkbox("h:Tape Delay/[0]Bypass");

// Delay times
delay_l  = hslider("h:Tape Delay/h:Time/[0]Left [unit:ms][style:knob]",
                    375, 10, 2000, 0.1);
delay_r  = hslider("h:Tape Delay/h:Time/[1]Right [unit:ms][style:knob]",
                    500, 10, 2000, 0.1);
sync_on  = checkbox("h:Tape Delay/h:Time/[2]Tempo Sync");
bpm      = hslider("h:Tape Delay/h:Time/[3]BPM [style:knob]",
                    120, 40, 240, 0.1);
sync_div = nentry("h:Tape Delay/h:Time/[4]Note Division",
                   4, 1, 16, 1);   // 1=whole, 2=half, 4=quarter, 8=eighth …

// Feedback
feedback = hslider("h:Tape Delay/h:Feedback/[0]Feedback [style:knob]",
                    40, 0, 100, 0.1) * 0.01;

// Modulation
mod_rate  = hslider("h:Tape Delay/h:Modulation/[0]Rate [unit:Hz][style:knob]",
                     0.5, 0.01, 10, 0.01);
mod_depth = hslider("h:Tape Delay/h:Modulation/[1]Depth [style:knob]",
                     20, 0, 100, 0.1) * 0.01;

// Feedback filters
fb_locut = hslider("h:Tape Delay/h:Filters/[0]Low Cut [unit:Hz][style:knob]",
                    80, 20, 2000, 1);
fb_hicut = hslider("h:Tape Delay/h:Filters/[1]High Cut [unit:Hz][style:knob]",
                    8000, 1000, 20000, 1);

// Saturation
sat_drive = hslider("h:Tape Delay/h:Saturation/[0]Drive [style:knob]",
                     30, 0, 100, 0.1) * 0.01;

// Mix
mix = hslider("h:Tape Delay/[9]Mix [style:knob]", 30, 0, 100, 0.1) * 0.01;

// ---------- helpers ---------------------------------------------------------
MAX_DELAY = 192001;   // enough for 2 s at 96 kHz

ms2samp(ms) = ms * 0.001 * ma.SR;

// Tempo sync: convert BPM + note division to ms
sync_ms = 60000.0 / bpm * 4.0 / sync_div;

// Effective delay in samples
eff_delay_l = ms2samp(select2(sync_on, delay_l, sync_ms));
eff_delay_r = ms2samp(select2(sync_on, delay_r, sync_ms));

// LFO for wow/flutter modulation (offset per channel for stereo spread)
lfo_l = os.osc(mod_rate) * mod_depth * 20;  // ±20 samples max at full depth
lfo_r = os.osc(mod_rate * 1.07) * mod_depth * 20;

// Tape saturation via tanh waveshaper
saturate(drive, x) = ma.tanh(x * (1 + drive * 4)) / (1 + drive * 4)
with {};

// Feedback filters: high-pass then low-pass
fb_filter = fi.highpass(2, fb_locut) : fi.lowpass(2, fb_hicut);

// ---------- stereo tape delay -----------------------------------------------
tape_delay(l, r) = (l_out, r_out)
with {
    // Delay with modulation and feedback
    l_delayed = l_del_fb
    with {
        d = max(0, eff_delay_l + lfo_l);
        l_del_fb = de.delay(MAX_DELAY, d, l + fb_l)
        with {
            fb_l = l_del_fb * feedback : fb_filter : saturate(sat_drive);
        };
    };

    r_delayed = r_del_fb
    with {
        d = max(0, eff_delay_r + lfo_r);
        r_del_fb = de.delay(MAX_DELAY, d, r + fb_r)
        with {
            fb_r = r_del_fb * feedback : fb_filter : saturate(sat_drive);
        };
    };

    // Dry/wet mixing
    l_out = l * (1 - mix) + l_delayed * mix;
    r_out = r * (1 - mix) + r_delayed * mix;
};

process = ba.bypass2(bypass, tape_delay);
