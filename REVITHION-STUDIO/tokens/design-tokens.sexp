;; REVITHION STUDIO — Design Tokens
;; S-expression format for procedural token generation
;; Processed by RLM recursive expansion → constexpr C++

(define-palette "dark-steel"
  :bg-primary     (color 0x12 0x12 0x20)
  :bg-secondary   (color 0x1A 0x1A 0x2E)
  :bg-surface     (color 0x22 0x22 0x38)
  :accent-primary (color 0x00 0xD4 0xFF)
  :accent-warm    (color 0xFF 0x6B 0x35)
  :text-primary   (color 0xE0 0xE0 0xE0)
  :text-secondary (color 0x80 0x80 0x90)
  :success        (color 0x00 0xFF 0x88)
  :warning        (color 0xFF 0xCC 0x00)
  :error          (color 0xFF 0x44 0x44)
  :meter-green    (color 0x00 0xCC 0x44)
  :meter-yellow   (color 0xFF 0xCC 0x00)
  :meter-red      (color 0xFF 0x22 0x22))

(define-typography "studio"
  :font-family    "Inter"
  :title-size     32.0
  :heading-size   18.0
  :body-size      14.0
  :label-size     11.0
  :mono-family    "JetBrains Mono"
  :mono-size      12.0)

(define-spacing "compact"
  :unit           4
  :padding-sm     8
  :padding-md     16
  :padding-lg     24
  :margin-sm      8
  :margin-md      16
  :gap-track      2
  :gap-block      4)

(define-dimensions "routing-grid"
  :columns        12
  :rows           4
  :block-width    80
  :block-height   60
  :gap            4
  :header-height  24)
