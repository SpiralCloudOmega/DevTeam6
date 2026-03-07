;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Macro Definitions for Audio Design Tokens
;; ═══════════════════════════════════════════════════════════════════════
;; These macros provide reusable templates for common audio token patterns.
;; Expanded by generate-tokens.py at build time into C++20 constexpr code.
;;
;; Macro Syntax:
;;   (defmacro name (pos1 pos2 &key kw1 kw2 &rest rest-param)
;;     "Optional docstring."
;;     (body-template ...))
;;
;; Special Forms:
;;   (or param default)    — Use param if bound, else default
;;   (splice list-param)   — Inline list elements into parent expression
;; ═══════════════════════════════════════════════════════════════════════

;; ── Amp Model Templates ─────────────────────────────────────────────

(defmacro british-amp (name gain &key bass mid treble presence master eq-bands gain-stages)
  "British-voiced amplifier template (Marshall, Vox, Orange, Hiwatt).
   Characteristic: mid-focused crunch, harmonically rich overdrive,
   Class A/B power section with EL34/EL84 voicing."
  (amp-model :name name
             :voicing british
             :gain-stages (or gain-stages 3)
             :eq-bands (or eq-bands 3)
             :gain gain
             :bass (or bass 5.0)
             :mid (or mid 7.0)
             :treble (or treble 6.0)
             :presence (or presence 0.7)
             :master (or master 5.0)))

(defmacro american-amp (name gain &key bass mid treble presence master eq-bands gain-stages)
  "American-voiced amplifier template (Fender, Mesa clean channels).
   Characteristic: clean headroom, scooped mids, tight low end,
   Class AB power section with 6L6/6V6 voicing."
  (amp-model :name name
             :voicing american
             :gain-stages (or gain-stages 2)
             :eq-bands (or eq-bands 4)
             :gain gain
             :bass (or bass 6.0)
             :mid (or mid 4.0)
             :treble (or treble 6.5)
             :presence (or presence 0.6)
             :master (or master 5.0)))

(defmacro high-gain-amp (name gain &key bass mid treble presence master eq-bands gain-stages)
  "High-gain amplifier template (Mesa Rectifier, EVH, Soldano, Diezel, Peavey).
   Characteristic: saturated distortion, tight response, cascaded gain stages,
   extended gain range with aggressive EQ shaping."
  (amp-model :name name
             :voicing high-gain
             :gain-stages (or gain-stages 4)
             :eq-bands (or eq-bands 5)
             :gain gain
             :bass (or bass 5.5)
             :mid (or mid 5.0)
             :treble (or treble 6.0)
             :presence (or presence 0.75)
             :master (or master 4.5)))

;; ── Effect Chain Templates ──────────────────────────────────────────

(defmacro standard-chain (name &rest blocks)
  "Standard serial effect chain — blocks processed in sequence.
   Signal flows left-to-right through each block."
  (fx-chain :name name
            :topology serial
            (splice blocks)))

(defmacro parallel-chain (name &rest blocks)
  "Parallel effect chain — blocks processed in parallel paths.
   Signal is split, processed independently, then summed."
  (fx-chain :name name
            :topology parallel
            (splice blocks)))

;; ── Mastering Chain Templates ───────────────────────────────────────

(defmacro mastering-preset (name target-lufs &rest chain)
  "Mastering chain preset targeting a specific LUFS loudness level.
   Includes dithering and high sample rate by default."
  (master-chain :name name
                :target-lufs target-lufs
                :dithering true
                :sample-rate 96000
                :bit-depth 32
                (splice chain)))

(defmacro broadcast-master (name target-lufs)
  "Broadcast/streaming mastering preset with standard broadcast processing.
   Fixed chain: linear-phase EQ → multiband comp → stereo enhance → IRC5 limiter.
   Lower sample rate (48kHz) for broadcast compatibility."
  (master-chain :name name
                :target-lufs target-lufs
                :dithering true
                :sample-rate 48000
                :bit-depth 24
                (eq :type linear-phase :bands 6)
                (comp :type multiband :bands 4 :threshold -18.0 :ratio 2.5)
                (stereo-enhance :width 1.1)
                (limiter :type irc5 :ceiling -1.0 :release 50.0)))

;; ── Routing Templates ───────────────────────────────────────────────

(defmacro simple-route (name input output &rest blocks)
  "Simple single-path routing through the 12x4 grid.
   Blocks placed sequentially across columns in the specified row."
  (route :grid 12x4
         :name name
         :input input
         :output output
         :blocks [(splice blocks)]))

;; ── UI Theme Templates ──────────────────────────────────────────────

(defmacro dark-theme (name accent &key bg fg font border surface)
  "Dark UI theme template. Optimized for low-light studio environments
   with high-contrast accent colors for metering and controls."
  (theme :name name
         :bg (or bg "#1a1a2e")
         :fg (or fg "#e0e0e0")
         :accent accent
         :font (or font "Inter")
         :border (or border "#333344")
         :surface (or surface "#252540")))
