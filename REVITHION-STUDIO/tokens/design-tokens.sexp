;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Master Audio Design Token Definitions
;; ═══════════════════════════════════════════════════════════════════════
;; S-expression format → macro-expanded → C++20 constexpr at build time
;;
;; Run:  python generate-tokens.py
;;
;; Token types:
;;   amp-model     — Amplifier model definitions (voicing, gain staging, EQ)
;;   fx-chain      — Effect chain templates (serial/parallel block routing)
;;   route         — 12×4 routing grid presets (AXE-FX III style)
;;   theme         — UI theme constants (colors, fonts)
;;   master-chain  — Mastering chain presets (target LUFS, processing)
;;
;; Tokens may use macros defined in macros.sexp (british-amp, american-amp,
;; high-gain-amp, standard-chain, mastering-preset, etc.)
;; ═══════════════════════════════════════════════════════════════════════


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  AMP MODELS  (23 definitions)                                    ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; ── British Voicing ─────────────────────────────────────────────────

;; Marshall Plexi 1959 — The gold standard of classic rock tone
(amp-model :name "Plexi 1959"
           :voicing british
           :gain-stages 2
           :eq-bands 3
           :gain 6.5
           :bass 5.0
           :mid 7.0
           :treble 6.5
           :presence 0.7
           :master 5.0)

;; Marshall JCM800 — Definitive 80s rock/metal crunch
(british-amp "JCM800" 7.5
  :bass 5.5
  :mid 6.5
  :treble 7.0
  :presence 0.75
  :master 4.5)

;; Marshall JTM45 — Vintage blues warmth
(british-amp "JTM45" 5.0
  :gain-stages 2
  :bass 6.0
  :mid 6.0
  :treble 5.5
  :presence 0.6
  :master 6.0)

;; Marshall Silver Jubilee — Versatile British crunch with clipping diodes
(british-amp "Silver Jubilee" 7.0
  :bass 5.0
  :mid 7.5
  :treble 6.5
  :presence 0.7
  :master 5.0
  :gain-stages 3
  :eq-bands 3)

;; Vox AC30 Top Boost — Chimey cleans, jangly breakup
(british-amp "AC30 Top Boost" 5.5
  :gain-stages 2
  :eq-bands 3
  :bass 4.5
  :mid 6.0
  :treble 7.5
  :presence 0.8
  :master 5.5)

;; Vox AC15 — Smaller Vox, earlier breakup, studio favorite
(british-amp "AC15" 4.5
  :gain-stages 2
  :eq-bands 2
  :bass 5.0
  :mid 6.0
  :treble 7.0
  :presence 0.75
  :master 6.0)

;; Orange Rockerverb 100 MkIII — Thick, dark, massive
(british-amp "Rockerverb 100" 7.0
  :gain-stages 3
  :eq-bands 4
  :bass 6.5
  :mid 5.5
  :treble 5.0
  :presence 0.65
  :master 4.0)

;; Hiwatt DR103 — Extremely clean and loud, Pete Townshend signature
(british-amp "DR103" 4.0
  :gain-stages 2
  :eq-bands 4
  :bass 5.5
  :mid 5.5
  :treble 6.0
  :presence 0.65
  :master 7.0)

;; ── American Voicing ────────────────────────────────────────────────

;; Fender Twin Reverb — Crystal clean, the jazz/country standard
(american-amp "Twin Reverb" 3.0
  :bass 6.0
  :mid 5.0
  :treble 6.5
  :presence 0.5
  :master 6.0
  :gain-stages 2
  :eq-bands 4)

;; Fender Deluxe Reverb — Warm breakup at lower volumes, recording workhorse
(american-amp "Deluxe Reverb" 4.0
  :bass 5.5
  :mid 5.0
  :treble 6.0
  :presence 0.55
  :master 5.5)

;; Fender Bassman — The amp that started it all, pushed clean to crunch
(american-amp "Bassman" 5.5
  :gain-stages 2
  :eq-bands 4
  :bass 7.0
  :mid 4.5
  :treble 6.0
  :presence 0.6
  :master 5.0)

;; Fender Princeton Reverb — Small-room magic, bedroom-level breakup
(american-amp "Princeton Reverb" 3.5
  :gain-stages 2
  :eq-bands 3
  :bass 5.5
  :mid 4.5
  :treble 5.5
  :presence 0.5
  :master 7.0)

;; ── High-Gain Voicing ───────────────────────────────────────────────

;; Mesa/Boogie Mark IIC+ — The legendary lead tone, Metallica/Dream Theater
(amp-model :name "MESA IIC+"
           :voicing high-gain
           :gain-stages 3
           :eq-bands 5
           :gain 8.5
           :bass 4.5
           :mid 3.0
           :treble 7.5
           :presence 0.7
           :master 4.0)

;; Mesa/Boogie Dual Rectifier — Modern metal standard, thick saturation
(high-gain-amp "Dual Rectifier" 8.0
  :gain-stages 4
  :eq-bands 5
  :bass 6.0
  :mid 4.0
  :treble 6.5
  :presence 0.7
  :master 4.0)

;; Mesa/Boogie Triple Rectifier — Extreme headroom and gain, stadium metal
(high-gain-amp "Triple Rectifier" 9.0
  :gain-stages 5
  :eq-bands 5
  :bass 6.5
  :mid 4.5
  :treble 6.0
  :presence 0.7
  :master 3.5)

;; EVH 5150 III — Eddie Van Halen's vision, tight and aggressive
(high-gain-amp "5150 III" 8.5
  :gain-stages 4
  :eq-bands 4
  :bass 5.0
  :mid 5.5
  :treble 7.0
  :presence 0.8
  :master 4.5)

;; Soldano SLO-100 — The original hot-rodded amp, silky lead tone
(high-gain-amp "SLO-100" 8.0
  :gain-stages 3
  :eq-bands 4
  :bass 5.5
  :mid 6.0
  :treble 6.5
  :presence 0.75
  :master 5.0)

;; Diezel VH4 — Surgical German precision, four independent channels
(high-gain-amp "Diezel VH4" 8.5
  :gain-stages 4
  :eq-bands 5
  :bass 5.0
  :mid 5.5
  :treble 6.5
  :presence 0.8
  :master 4.0)

;; Friedman BE-100 — The "Brown Eye", Dave Friedman's Brown Sound tribute
(amp-model :name "Friedman BE-100"
           :voicing high-gain
           :gain-stages 3
           :eq-bands 4
           :gain 7.5
           :bass 5.5
           :mid 6.5
           :treble 6.5
           :presence 0.75
           :master 5.0)

;; Peavey 6505+ — Thrash/death metal standard, relentless gain
(high-gain-amp "Peavey 6505+" 9.0
  :gain-stages 5
  :eq-bands 5
  :bass 5.5
  :mid 4.0
  :treble 6.5
  :presence 0.7
  :master 3.5)

;; ENGL Powerball II — European metal, surgical mids, crushing low end
(high-gain-amp "ENGL Powerball II" 8.5
  :gain-stages 4
  :eq-bands 5
  :bass 6.0
  :mid 5.0
  :treble 6.0
  :presence 0.75
  :master 4.0)

;; Bogner Ecstasy — Boutique versatility, clean to high-gain in one amp
(high-gain-amp "Bogner Ecstasy" 7.0
  :gain-stages 3
  :eq-bands 4
  :bass 5.5
  :mid 6.0
  :treble 6.0
  :presence 0.7
  :master 5.0)

;; Revv Generator 120 — Modern gain monster, built-in reactive load
(high-gain-amp "Revv Generator 120" 8.0
  :gain-stages 4
  :eq-bands 5
  :bass 5.0
  :mid 5.5
  :treble 6.5
  :presence 0.75
  :master 4.5)


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  EFFECT CHAINS  (10 definitions)                                 ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; 1. Studio Plate Reverb — Classic studio reverb with post-EQ shaping
(fx-chain :name "studio-verb"
  :topology serial
  (reverb :type plate :decay 2.5 :damping 0.7 :pre-delay 20.0 :mix 0.35)
  (eq :type parametric :bands 4 :freq 3000.0 :gain -1.5 :q 1.0))

;; 2. Modern Metal — Tight gate → compression → high-gain → cab → surgical EQ
(standard-chain "modern-metal"
  (gate :threshold -45.0 :ratio 10.0 :attack 0.5 :release 50.0)
  (comp :threshold -12.0 :ratio 4.0 :attack 5.0 :release 80.0)
  (overdrive :drive 6.0 :tone 7.0 :level 5.0)
  (amp :model "5150-iii" :gain 8.5)
  (cab :model "v30-4x12" :mic 0.0 :position 2.0)
  (eq :type graphic :bands 10 :freq 400.0 :gain -3.0))

;; 3. Jazz Clean — Warm compression → clean amp → subtle chorus → spring verb
(standard-chain "jazz-clean"
  (comp :threshold -20.0 :ratio 2.0 :attack 30.0 :release 200.0)
  (amp :model "twin-reverb" :gain 3.0)
  (chorus :rate 0.5 :depth 0.3 :mix 0.2)
  (reverb :type spring :decay 2.5 :damping 0.5 :mix 0.25))

;; 4. Ambient Wash — Shimmer reverb + modulated delay for atmospheric textures
(fx-chain :name "ambient-wash"
  :topology parallel
  (delay :type ping-pong :time 500.0 :feedback 0.6 :mix 0.4)
  (reverb :type shimmer :decay 8.0 :damping 0.3 :pre-delay 40.0 :mix 0.5)
  (tremolo :rate 3.0 :depth 0.4 :mix 0.3))

;; 5. Classic Rock Crunch — Overdrive → British amp → cab → delay → room verb
(standard-chain "classic-rock"
  (overdrive :drive 4.0 :tone 5.5 :level 6.0)
  (amp :model "plexi" :gain 6.5)
  (cab :model "greenback-4x12" :mic 1.0 :position 3.0)
  (delay :type tape :time 375.0 :feedback 0.3 :mix 0.2)
  (reverb :type room :decay 1.5 :damping 0.6 :mix 0.2))

;; 6. Studio Vocal — De-esser → comp → EQ → plate reverb (vocal bus processing)
(standard-chain "studio-vocal"
  (de-esser :threshold -25.0 :freq 6000.0)
  (comp :type opto :threshold -18.0 :ratio 3.0 :attack 10.0 :release 150.0)
  (eq :type parametric :bands 4 :freq 3000.0 :gain 2.0 :q 1.5)
  (reverb :type plate :decay 1.8 :damping 0.6 :pre-delay 30.0 :mix 0.2))

;; 7. Bass Rig — Compression → tube amp → cab → low-end EQ shaping
(standard-chain "bass-rig"
  (comp :type fet :threshold -15.0 :ratio 4.0 :attack 8.0 :release 100.0)
  (amp :model "bassman" :gain 5.5)
  (cab :model "ampeg-8x10" :mic 0.0 :position 1.0)
  (eq :type parametric :bands 3 :freq 80.0 :gain 3.0 :q 0.7))

;; 8. Acoustic Enhancement — Subtle comp → parametric EQ → chorus → hall reverb
(standard-chain "acoustic-enhance"
  (comp :type opto :threshold -22.0 :ratio 2.0 :attack 20.0 :release 250.0)
  (eq :type parametric :bands 5 :freq 200.0 :gain -2.0 :q 1.2)
  (chorus :rate 0.3 :depth 0.2 :mix 0.15)
  (reverb :type hall :decay 2.0 :damping 0.5 :pre-delay 15.0 :mix 0.25))

;; 9. Lead Solo — Comp → OD → amp → delay → reverb (sustaining lead tones)
(standard-chain "lead-solo"
  (comp :threshold -15.0 :ratio 3.0 :attack 5.0 :release 100.0)
  (overdrive :drive 5.0 :tone 6.0 :level 7.0)
  (amp :model "slo-100" :gain 8.0)
  (cab :model "v30-4x12" :mic 0.0 :position 2.0)
  (delay :type analog :time 350.0 :feedback 0.35 :mix 0.25)
  (reverb :type plate :decay 2.0 :damping 0.6 :mix 0.2))

;; 10. Post-Rock — Tremolo → delay layers → huge reverb → shimmer
(parallel-chain "post-rock"
  (tremolo :rate 4.0 :depth 0.6 :mix 0.4)
  (delay :type stereo :time 600.0 :feedback 0.55 :mix 0.35)
  (reverb :type hall :decay 6.0 :damping 0.3 :pre-delay 50.0 :mix 0.45)
  (pitch-shift :semitones 12.0 :mix 0.15))


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  ROUTING PRESETS  (5 definitions — 12×4 grid, AXE-FX III style) ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; 1. Single Amp — Simple serial guitar signal chain through one row
(route :grid 12x4
       :name "single-amp"
       :input 1
       :output 1
       :blocks [(gate :threshold -50.0)
                (comp :threshold -15.0 :ratio 3.0)
                (amp :model "plexi" :gain 6.5)
                (cab :model "greenback-4x12")
                (reverb :type room :decay 1.5 :mix 0.2)
                (limiter :ceiling -0.3)])

;; 2. Dual Amp Stereo — Two amp paths split and panned for stereo width
(route :grid 12x4
       :name "dual-amp-stereo"
       :input 1
       :output 4
       :blocks [(gate :threshold -48.0)
                (comp :threshold -12.0 :ratio 3.0)
                (amp :model "jcm800" :gain 7.0)
                (cab :model "greenback-4x12")
                (amp :model "dual-rec" :gain 7.5)
                (cab :model "v30-4x12")
                (delay :type stereo :time 10.0 :feedback 0.0 :mix 1.0)
                (reverb :type room :decay 1.8 :mix 0.2)])

;; 3. Wet-Dry-Wet — Classic WDW configuration for spatial depth
(route :grid 12x4
       :name "wet-dry-wet"
       :input 1
       :output 3
       :blocks [(comp :threshold -15.0 :ratio 2.5)
                (amp :model "ac30" :gain 5.5)
                (cab :model "blue-2x12")
                (delay :type ping-pong :time 375.0 :feedback 0.4 :mix 1.0)
                (reverb :type hall :decay 3.0 :damping 0.5 :mix 1.0)
                (limiter :ceiling -0.5)])

;; 4. Four Cable Method — Effects loop integration with external amp
(route :grid 12x4
       :name "four-cable-method"
       :input 1
       :output 2
       :blocks [(comp :threshold -18.0 :ratio 3.0)
                (overdrive :drive 5.0 :tone 6.0 :level 5.0)
                (gate :threshold -50.0)
                (eq :type parametric :bands 4 :freq 800.0 :gain -2.0)
                (delay :type tape :time 400.0 :feedback 0.35 :mix 0.25)
                (reverb :type plate :decay 2.0 :mix 0.2)
                (limiter :ceiling -0.3)])

;; 5. Parallel Paths — Multiple parallel effect paths merged at output
(route :grid 12x4
       :name "parallel-paths"
       :input 1
       :output 4
       :blocks [(gate :threshold -48.0)
                (amp :model "5150-iii" :gain 8.0)
                (cab :model "v30-4x12")
                (chorus :rate 0.8 :depth 0.4 :mix 0.3)
                (delay :type ping-pong :time 500.0 :feedback 0.5 :mix 0.3)
                (reverb :type shimmer :decay 5.0 :damping 0.3 :mix 0.35)
                (eq :type parametric :bands 5)
                (limiter :ceiling -0.3)])


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  UI THEMES  (3 definitions)                                      ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; 1. Dark Studio — Default dark theme for low-light studio environments
(theme :name "dark-studio"
       :bg "#1a1a2e"
       :fg "#e0e0e0"
       :accent "#e94560"
       :font "Inter"
       :border "#333344"
       :surface "#252540")

;; 2. Light Mix — Light theme for daytime mixing with high contrast metering
(theme :name "light-mix"
       :bg "#f5f5f5"
       :fg "#1a1a1a"
       :accent "#2563eb"
       :font "Inter"
       :border "#d1d5db"
       :surface "#ffffff")

;; 3. Vintage Warm — Warm retro palette inspired by analog console aesthetics
(theme :name "vintage-warm"
       :bg "#2d1b0e"
       :fg "#f0d9b5"
       :accent "#d4a017"
       :font "IBM Plex Mono"
       :border "#5c3a1e"
       :surface "#3d2817")


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  MASTERING CHAINS  (5 definitions)                               ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; 1. Streaming — Spotify/Apple Music target (-14 LUFS)
(mastering-preset "streaming" -14.0
  (eq :type linear-phase :bands 6 :freq 40.0 :gain 0.0)
  (comp :type multiband :bands 4 :threshold -18.0 :ratio 2.5)
  (stereo-enhance :width 1.15)
  (limiter :type irc5 :ceiling -1.0 :release 50.0))

;; 2. CD Master — Traditional CD loudness (-9 LUFS)
(mastering-preset "cd-master" -9.0
  (eq :type linear-phase :bands 8 :freq 30.0 :gain 0.0)
  (comp :type multiband :bands 5 :threshold -15.0 :ratio 3.0)
  (comp :type vca :threshold -10.0 :ratio 2.0 :attack 10.0 :release 100.0)
  (stereo-enhance :width 1.2)
  (limiter :type irc5 :ceiling -0.3 :release 30.0))

;; 3. Vinyl — Vinyl pressing target with RIAA considerations (-12 LUFS)
(mastering-preset "vinyl" -12.0
  (eq :type linear-phase :bands 6 :freq 35.0 :gain 0.0)
  (comp :type opto :threshold -20.0 :ratio 2.0 :attack 20.0 :release 200.0)
  (eq :type parametric :bands 3 :freq 8000.0 :gain -1.5 :q 0.8)
  (stereo-enhance :width 1.0)
  (limiter :type brickwall :ceiling -0.5 :release 80.0))

;; 4. Broadcast — EBU R128 broadcast standard (-23 LUFS)
(broadcast-master "broadcast" -23.0)

;; 5. Podcast — Spoken word target with voice-optimized processing (-16 LUFS)
(mastering-preset "podcast" -16.0
  (eq :type parametric :bands 4 :freq 200.0 :gain -2.0 :q 1.0)
  (comp :type opto :threshold -22.0 :ratio 3.0 :attack 15.0 :release 150.0)
  (de-esser :threshold -28.0 :freq 6500.0)
  (limiter :type irc5 :ceiling -1.5 :release 60.0))
