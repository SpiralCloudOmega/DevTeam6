// ============================================================================
// Vocoder — 16-band channel vocoder with built-in carrier
// ============================================================================
declare name        "Vocoder";
declare author      "SpiralCloudOmega";
declare license     "MIT";
declare description "16-band channel vocoder with internal saw carrier and band levels";

import("stdfaust.lib");

// ---------- parameters ------------------------------------------------------
bypass = checkbox("h:Vocoder/[0]Bypass");

// Carrier
carrier_freq = hslider("h:Vocoder/h:Carrier/[0]Frequency [unit:Hz][style:knob]",
                        110, 20, 2000, 0.1);
carrier_fine = hslider("h:Vocoder/h:Carrier/[1]Fine Tune [unit:cents][style:knob]",
                        0, -100, 100, 1);
carrier_mix  = hslider("h:Vocoder/h:Carrier/[2]Ext/Int [style:knob]",
                        0, 0, 100, 0.1) * 0.01;  // 0 = internal, 1 = external

// Envelope
env_attack  = hslider("h:Vocoder/h:Envelope/[0]Attack [unit:ms][style:knob]",
                       5, 0.1, 100, 0.1) * 0.001;
env_release = hslider("h:Vocoder/h:Envelope/[1]Release [unit:ms][style:knob]",
                       50, 5, 500, 1) * 0.001;

// Band levels (16 bands)
band_gain(i) = hslider("h:Vocoder/h:Band Levels/[%2i]Band %2i [style:knob]",
                         0, -20, 20, 0.1) : ba.db2linear
with {};

// Output
out_gain = hslider("h:Vocoder/[8]Output Gain [unit:dB][style:knob]",
                    0, -20, 20, 0.1) : ba.db2linear;
mix      = hslider("h:Vocoder/[9]Mix [style:knob]",
                    100, 0, 100, 0.1) * 0.01;

// ---------- constants -------------------------------------------------------
N_BANDS = 16;
BW_FACTOR = 1.4;   // Bandwidth factor for band filters

// Band center frequencies (roughly 1/3 octave spacing from ~80 Hz to ~12 kHz)
band_freq(0)  = 80;
band_freq(1)  = 120;
band_freq(2)  = 175;
band_freq(3)  = 250;
band_freq(4)  = 360;
band_freq(5)  = 510;
band_freq(6)  = 720;
band_freq(7)  = 1000;
band_freq(8)  = 1400;
band_freq(9)  = 2000;
band_freq(10) = 2800;
band_freq(11) = 4000;
band_freq(12) = 5600;
band_freq(13) = 7500;
band_freq(14) = 9500;
band_freq(15) = 12000;

// Bandwidth for each band (proportional to center frequency)
band_bw(i) = band_freq(i) / BW_FACTOR;

// ---------- helpers ---------------------------------------------------------
// Envelope follower (attack/release)
env_follow(att, rel) = abs : si.smooth(ba.tau2pole(rel))
                       : si.smooth(ba.tau2pole(att));

// Bandpass filter for analysis and synthesis
bp(i) = fi.bandpass(4, band_freq(i) - band_bw(i) * 0.5,
                       band_freq(i) + band_bw(i) * 0.5);

// Internal carrier oscillator (saw wave with detuning for richness)
carrier_osc = saw_main + saw_det
with {
    f = carrier_freq * pow(2.0, carrier_fine / 1200.0);
    saw_main = os.sawtooth(f);
    saw_det  = os.sawtooth(f * 1.005) * 0.5;  // slight detune
};

// ---------- vocoder channel (mono) ------------------------------------------
vocoder_mono(modulator, external_carrier) = output
with {
    // Select carrier source
    carrier = carrier_osc * (1 - carrier_mix)
            + external_carrier * carrier_mix;

    // Process each band: analyse modulator, apply envelope to carrier
    process_band(i) = carrier : bp(i) : *(env_val) : *(band_gain(i))
    with {
        env_val = modulator : bp(i) : env_follow(env_attack, env_release);
    };

    // Sum all bands
    output = sum(i, N_BANDS, process_band(i)) * out_gain;
};

// ---------- main process ----------------------------------------------------
// Input: channels 1-2 = modulator (stereo), channels 3-4 = external carrier (stereo)
// For simple use: modulator on inputs 1-2, internal carrier used by default
vocoder_stereo(ml, mr) = (l_out, r_out)
with {
    // Use modulator as external carrier fallback (for sidechain routing)
    voc_l = vocoder_mono(ml, ml);
    voc_r = vocoder_mono(mr, mr);

    l_out = ml * (1 - mix) + voc_l * mix;
    r_out = mr * (1 - mix) + voc_r * mix;
};

process = ba.bypass2(bypass, vocoder_stereo);
