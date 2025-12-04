import { useEffect, useMemo, useState } from 'react'
import { getLoadStatus, sanitizeEmbedUrl } from '../utils/controlDeck'

interface Panel {
  id: string
  title: string
  description: string
  icon: string
}

interface Agent {
  name: string
  role: string
  status: 'online' | 'standby'
  focus: string
}

interface PlaylistItem {
  title: string
  url: string
  chapter: string
}

const panels: Panel[] = [
  { id: 'graph', title: 'Graph Deck', description: 'n8n-style clusters with path sim.', icon: '🛰️' },
  { id: 'video', title: 'Mission Video Hub', description: 'Chapters + embedded briefings.', icon: '🎬' },
  { id: 'agents', title: 'Agent Roster', description: 'Persona macros + background tasks.', icon: '🤖' },
  { id: 'motion', title: 'UX Motion Lab', description: 'Glows, ribbons, and layout anchors.', icon: '🌀' },
  { id: 'docs', title: 'Guidance Rail', description: 'Quick-start cards + shortcuts.', icon: '📜' }
]

const themes = [
  { id: 'tron', label: 'Tron', primary: '#00f0ff', secondary: '#ff00ff' },
  { id: 'quantum', label: 'Quantum', primary: '#7b2fff', secondary: '#00ff88' },
  { id: 'noir', label: 'Noir Grid', primary: '#9ddff6', secondary: '#ffaa44' }
]

const playlist: PlaylistItem[] = [
  { title: 'Control Deck Tour', url: 'https://www.youtube.com/watch?v=tpe_Y6h6BfM', chapter: '00:00 - Overview' },
  { title: 'Graph Automation', url: 'https://www.youtube.com/watch?v=VdQkYJ_Rp-k', chapter: '02:10 - Path routing' },
  { title: 'Repomind Personas', url: 'https://www.youtube.com/watch?v=6Dh-RL__uN4', chapter: '04:35 - Macro builder' }
]

const agentRoster: Agent[] = [
  { name: 'Repomind Prime', role: 'Narrator', status: 'online', focus: 'Story + safety rails' },
  { name: 'Ops Drive', role: 'Automation', status: 'online', focus: 'Path sim + telemetry' },
  { name: 'Design Pulse', role: 'UX motion', status: 'standby', focus: 'Glows, ribbons, low-power' },
  { name: 'Guardian', role: 'Security', status: 'standby', focus: 'Sanitize embeds + ARIA' }
]

const quickStarts = [
  { title: 'Build', detail: 'pnpm install && pnpm dev', cta: 'Open build docs' },
  { title: 'Test', detail: 'npm test -- --watch=false', cta: 'View QA checklist' },
  { title: 'Deploy', detail: 'GitHub Pages /DevTeam6', cta: 'See deployment notes' }
]

const macroPresets = [
  { label: 'Drive-Through', text: 'Summarize repo + surface live pages with shortcuts.' },
  { label: 'Security Sweep', text: 'List hardening steps, 0 secrets, offline defaults.' },
  { label: 'Design Glow', text: 'Suggest neon-friendly spacing + reduced-motion variant.' }
]

const actions = [
  'Open Repomind with preset video',
  'Toggle low-power mode (reduced particles)',
  'Jump to Node Graph Path Playbook',
  'Show keyboard shortcut overlay',
  'Switch theme to Quantum',
  'Open UX Motion Lab presets'
]

const motionTracks = [
  { title: 'Tron Bloom', detail: 'Ribbon sweeps + neon particle halos', density: 'High motion' },
  { title: 'Vector Drift', detail: 'Slow parallax grid with depth haze', density: 'Calm' },
  { title: 'Pulse Trails', detail: 'Glow pulses mapped to macro queue', density: 'Reactive' }
]

const widgetShelf = [
  { title: 'Dock Anchors', detail: 'Pinned nav pills for clusters', status: 'Ready' },
  { title: 'Halo Widgets', detail: 'Status beacons for load + theme', status: 'Live' },
  { title: 'Telemetry Tiles', detail: 'FPS + glow load HUD with tone', status: 'Synced' }
]

const missionBriefs: PlaylistItem[] = [
  { title: 'First-Run Tour', url: 'https://www.youtube.com/watch?v=tpe_Y6h6BfM&t=23s', chapter: '00:23 - Docking intro' },
  { title: 'Automation Preview', url: 'https://www.youtube.com/watch?v=VdQkYJ_Rp-k&t=64s', chapter: '01:04 - Path simulator' },
  { title: 'Repomind Sync', url: 'https://www.youtube.com/watch?v=6Dh-RL__uN4&t=90s', chapter: '01:30 - Macro queues' }
]

export default function ControlDeck() {
  const [activeTheme, setActiveTheme] = useState('tron')
  const [activePanel, setActivePanel] = useState('graph')
  const [commandOpen, setCommandOpen] = useState(false)
  const [lowPower, setLowPower] = useState(false)
  const [activeVideo, setActiveVideo] = useState(playlist[0])
  const [selectedMacro, setSelectedMacro] = useState(macroPresets[0])
  const [focusedAction, setFocusedAction] = useState(actions[0])
  const [activeBriefing, setActiveBriefing] = useState(missionBriefs[0])

  const theme = themes.find(t => t.id === activeTheme) ?? themes[0]
  const embedUrl = useMemo(() => sanitizeEmbedUrl(activeVideo.url), [activeVideo.url])
  const briefingEmbed = useMemo(() => sanitizeEmbedUrl(activeBriefing.url), [activeBriefing.url])
  const fps = lowPower ? 60 : 144
  const glowLoad = lowPower ? 0.32 : 0.68
  const loadStatus = useMemo(() => getLoadStatus(fps, glowLoad), [fps, glowLoad])

  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      if (event.key === 'p' && (event.metaKey || event.ctrlKey || event.shiftKey)) {
        event.preventDefault()
        setCommandOpen(prev => !prev)
      }
      if (event.key === 'Escape' && commandOpen) {
        setCommandOpen(false)
      }
    }
    window.addEventListener('keydown', handleKeyDown)
    return () => window.removeEventListener('keydown', handleKeyDown)
  }, [commandOpen])

  const renderHud = () => (
    <section
      className="control-deck__quick-meta"
      aria-label="Workstation status HUD"
      style={{ marginTop: '10px' }}
    >
      <div>
        <p className="meta-label">Load Status</p>
        <p className="meta-value">{loadStatus.label}</p>
        <p className="panel__note">{loadStatus.description}</p>
      </div>
      <div>
        <p className="meta-label">Theme Blend</p>
        <p className="meta-value">{theme.label}</p>
        <p className="panel__note">Primary {theme.primary}</p>
      </div>
      <div>
        <p className="meta-label">Power Mode</p>
        <p className="meta-value">{lowPower ? 'Low power' : 'Full glow'}</p>
        <p className="panel__note">{lowPower ? 'Particles capped for laptops' : 'Max ribbons and stars'}</p>
      </div>
    </section>
  )

  const renderHeader = () => (
    <header
      className="control-deck__header"
      style={{
        borderColor: theme.primary,
        boxShadow: `0 12px 42px rgba(0,0,0,0.45), 0 0 20px ${theme.primary}44`
      }}
    >
      <div>
        <p className="control-deck__eyebrow">OMEGA TOOL KIT • GITFATHER CONTROL DECK</p>
        <h1>Static Digital Workstation</h1>
        <p className="control-deck__subtitle">
          Docked panels, command palette, and Tron-inspired ribbons to navigate the repo without leaving the page.
        </p>
        <div className="control-deck__theme-toggle" role="group" aria-label="Theme selector">
          {themes.map(option => (
            <button
              key={option.id}
              className={`pill ${option.id === activeTheme ? 'pill--active' : ''}`}
              onClick={() => setActiveTheme(option.id)}
              onKeyDown={e => e.key === 'Enter' && setActiveTheme(option.id)}
              style={{ borderColor: option.primary, color: option.primary }}
            >
              {option.label}
            </button>
          ))}
          <button
            className={`pill ${lowPower ? 'pill--active' : ''}`}
            onClick={() => setLowPower(prev => !prev)}
            onKeyDown={e => e.key === 'Enter' && setLowPower(prev => !prev)}
            aria-pressed={lowPower}
          >
            {lowPower ? 'Low Power: On' : 'Low Power: Off'}
          </button>
        </div>
      </div>
      <div className="control-deck__quick-meta" aria-label="Panel stats">
        <div>
          <p className="meta-label">Panels</p>
          <p className="meta-value">5 docked</p>
        </div>
        <div>
          <p className="meta-label">Command Palette</p>
          <p className="meta-value">⇧⌘P / ⇧Ctrl+P</p>
        </div>
        <div>
          <p className="meta-label">Theme</p>
          <p className="meta-value">{theme.label}</p>
        </div>
      </div>
    </header>
  )

  const renderNav = () => (
    <nav className="control-deck__nav" aria-label="Dock navigation">
      {panels.map(panel => (
        <button
          key={panel.id}
          className={`dock ${panel.id === activePanel ? 'dock--active' : ''}`}
          onClick={() => setActivePanel(panel.id)}
          onKeyDown={e => e.key === 'Enter' && setActivePanel(panel.id)}
          aria-pressed={panel.id === activePanel}
        >
          <span className="dock__icon" aria-hidden>
            {panel.icon}
          </span>
          <div className="dock__content">
            <p className="dock__title">{panel.title}</p>
            <p className="dock__desc">{panel.description}</p>
          </div>
        </button>
      ))}
    </nav>
  )

  const renderGraphPanel = () => (
    <section className="control-deck__panel">
      <div className="panel__header">
        <h2>Graph Deck</h2>
        <p>Mind-map and automation presets with quick jump edges.</p>
      </div>
      <div className="panel__grid">
        <div className="panel__card">
          <p className="panel__label">Presets</p>
          <ul className="panel__list">
            <li>Knowledge Mind Map • clusters + halos</li>
            <li>Automation Brain • signals → delivery</li>
            <li>Path Playbook • start/target highlighting</li>
          </ul>
        </div>
        <div className="panel__card">
          <p className="panel__label">Mini-map</p>
          <div className="minimap" aria-label="Graph mini map">
            {[...Array(9)].map((_, idx) => (
              <span key={idx} className="minimap__node" aria-hidden />
            ))}
            <span className="minimap__path" aria-hidden />
          </div>
          <p className="panel__note">Scaled snapshot to keep orientation.</p>
        </div>
        <div className="panel__card">
          <p className="panel__label">Telemetry</p>
          <div className="telemetry">
            <div>
              <p className="meta-label">FPS</p>
              <p className="meta-value">{lowPower ? '60 capped' : '144 uncapped'}</p>
            </div>
            <div>
              <p className="meta-label">Glow Health</p>
              <p className="meta-value">Stable</p>
            </div>
            <div>
              <p className="meta-label">Hops</p>
              <p className="meta-value">6 max</p>
            </div>
          </div>
        </div>
      </div>
    </section>
  )

  const renderVideoPanel = () => (
    <section className="control-deck__panel">
      <div className="panel__header">
        <h2>Mission Video Hub</h2>
        <p>Embedded playlists with sanitized links and chapter hints.</p>
      </div>
      <div className="panel__grid">
        <div className="panel__card">
          <p className="panel__label">Playlist</p>
          <div className="playlist" role="list">
            {playlist.map(item => (
              <button
                key={item.url}
                role="listitem"
                className={`playlist__item ${item.url === activeVideo.url ? 'playlist__item--active' : ''}`}
                onClick={() => setActiveVideo(item)}
                onKeyDown={e => e.key === 'Enter' && setActiveVideo(item)}
                aria-pressed={item.url === activeVideo.url}
              >
                <div>
                  <p className="playlist__title">{item.title}</p>
                  <p className="playlist__meta">{item.chapter}</p>
                </div>
                <span aria-hidden>▶</span>
              </button>
            ))}
          </div>
        </div>
        <div className="panel__card">
          <p className="panel__label">Embedded Player</p>
          {embedUrl ? (
            <div className="player">
              <iframe
                title={activeVideo.title}
                src={embedUrl}
                allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
                allowFullScreen
              />
            </div>
          ) : (
            <div className="player player--error">Invalid video URL — only YouTube hosts are allowed.</div>
          )}
        </div>
        <div className="panel__card">
          <p className="panel__label">Mission Briefings</p>
          <div className="macros">
            {missionBriefs.map(briefing => (
              <button
                key={briefing.url}
                className={`playlist__item ${briefing.title === activeBriefing.title ? 'playlist__item--active' : ''}`}
                onClick={() => setActiveBriefing(briefing)}
                onKeyDown={e => e.key === 'Enter' && setActiveBriefing(briefing)}
                aria-pressed={briefing.title === activeBriefing.title}
              >
                <div>
                  <p className="playlist__title">{briefing.title}</p>
                  <p className="playlist__meta">{briefing.chapter}</p>
                </div>
              </button>
            ))}
          </div>
          <p className="panel__note">Briefings mirror the YouTube playlist with safe embeds.</p>
        </div>
      </div>
      <div className="panel__grid panel__grid--two">
        <div className="panel__card">
          <p className="panel__label">Briefing Preview</p>
          {briefingEmbed ? (
            <div className="player">
              <iframe
                title={activeBriefing.title}
                src={briefingEmbed}
                allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
                allowFullScreen
              />
            </div>
          ) : (
            <div className="player player--error">Invalid briefing link — only YouTube hosts are allowed.</div>
          )}
          <p className="panel__note">{activeBriefing.chapter}</p>
        </div>
        <div className="panel__card">
          <p className="panel__label">Story cues</p>
          <ul className="panel__list">
            <li>Use the briefing preview for in-repo tours.</li>
            <li>Pair with macros to pre-stage Repomind prompts.</li>
            <li>Low-power mode keeps iframe effects lighter.</li>
          </ul>
        </div>
      </div>
    </section>
  )

  const renderAgentsPanel = () => (
    <section className="control-deck__panel">
      <div className="panel__header">
        <h2>Agent Roster & Macro Builder</h2>
        <p>Mix personas, queue macros, and keep background tasks visible.</p>
      </div>
      <div className="panel__grid">
        <div className="panel__card">
          <p className="panel__label">Roster</p>
          <ul className="panel__list">
            {agentRoster.map(agent => (
              <li key={agent.name} className="agent">
                <div>
                  <p className="agent__name">{agent.name}</p>
                  <p className="agent__role">{agent.role} • {agent.focus}</p>
                </div>
                <span className={`pill ${agent.status === 'online' ? 'pill--active' : ''}`} aria-label={agent.status}>
                  {agent.status}
                </span>
              </li>
            ))}
          </ul>
        </div>
        <div className="panel__card">
          <p className="panel__label">Macro Presets</p>
          <div className="macros">
            {macroPresets.map(macro => (
              <button
                key={macro.label}
                className={`playlist__item ${macro.label === selectedMacro.label ? 'playlist__item--active' : ''}`}
                onClick={() => setSelectedMacro(macro)}
                onKeyDown={e => e.key === 'Enter' && setSelectedMacro(macro)}
                aria-pressed={macro.label === selectedMacro.label}
              >
                <div>
                  <p className="playlist__title">{macro.label}</p>
                  <p className="playlist__meta">{macro.text}</p>
                </div>
              </button>
            ))}
          </div>
          <div className="macro-preview" aria-live="polite">
            <p className="panel__label">Preview</p>
            <p className="macro-preview__text">{selectedMacro.text}</p>
          </div>
        </div>
        <div className="panel__card">
          <p className="panel__label">Background Queue</p>
          <ul className="panel__list">
            <li>Prefetch assets for Repomind ribbons</li>
            <li>Validate header links to avoid 404s</li>
            <li>Cache graph presets for offline mode</li>
          </ul>
        </div>
      </div>
    </section>
  )

  const renderMotionPanel = () => (
    <section className="control-deck__panel">
      <div className="panel__header">
        <h2>UX Motion Lab</h2>
        <p>Ambient glow systems, ribbon presets, and layout anchors tuned for the Gitfather deck.</p>
      </div>
      <div className="panel__grid">
        <div className="panel__card">
          <p className="panel__label">Motion Tracks</p>
          <div className="quickstarts">
            {motionTracks.map(track => (
              <div key={track.title} className="quickstart">
                <p className="quickstart__title">{track.title}</p>
                <p className="quickstart__meta">{track.detail}</p>
                <span className="quickstart__cta">{track.density}</span>
              </div>
            ))}
          </div>
        </div>
        <div className="panel__card">
          <p className="panel__label">Widget Shelf</p>
          <ul className="panel__list">
            {widgetShelf.map(widget => (
              <li key={widget.title} className="agent">
                <div>
                  <p className="agent__name">{widget.title}</p>
                  <p className="agent__role">{widget.detail}</p>
                </div>
                <span className={`pill ${widget.status === 'Live' ? 'pill--active' : ''}`}>{widget.status}</span>
              </li>
            ))}
          </ul>
        </div>
        <div className="panel__card">
          <p className="panel__label">Stage Directions</p>
          <ul className="panel__list">
            <li>Pin the Motion Lab to keep ribbons visible on scroll.</li>
            <li>Use command palette to swap themes mid-automation.</li>
            <li>Toggle low-power mode for meeting demos.</li>
          </ul>
          <p className="panel__note">HUD load tones shift between cool, balanced, and warm.</p>
        </div>
      </div>
    </section>
  )

  const renderDocsPanel = () => (
    <section className="control-deck__panel">
      <div className="panel__header">
        <h2>Guidance Rail</h2>
        <p>Quick-start cards, shortcuts, and helpful actions for first-time visitors.</p>
      </div>
      <div className="panel__grid panel__grid--two">
        <div className="panel__card">
          <p className="panel__label">Quick Start</p>
          <div className="quickstarts">
            {quickStarts.map(tile => (
              <div key={tile.title} className="quickstart">
                <p className="quickstart__title">{tile.title}</p>
                <p className="quickstart__meta">{tile.detail}</p>
                <span className="quickstart__cta">{tile.cta}</span>
              </div>
            ))}
          </div>
        </div>
        <div className="panel__card">
          <p className="panel__label">Actions & Shortcuts</p>
          <ul className="panel__list">
            {actions.map(action => (
              <li key={action} className={action === focusedAction ? 'highlighted' : ''}>
                <button
                  className="linkish"
                  onFocus={() => setFocusedAction(action)}
                  onMouseEnter={() => setFocusedAction(action)}
                  onClick={() => setFocusedAction(action)}
                  onKeyDown={e => e.key === 'Enter' && setFocusedAction(action)}
                >
                  {action}
                </button>
              </li>
            ))}
          </ul>
          <p className="panel__note">Command palette mirrors these entries.</p>
        </div>
      </div>
    </section>
  )

  return (
    <div className={`control-deck theme-${activeTheme}`}>
      <div className="control-deck__background" aria-hidden>
        <div className="control-deck__ribbon control-deck__ribbon--a" />
        <div className="control-deck__ribbon control-deck__ribbon--b" />
        <div className="control-deck__grid" />
      </div>
      {renderHeader()}
      <div className="control-deck__body">
        {renderNav()}
        <main className="control-deck__main" aria-live="polite">
          {renderHud()}
          {activePanel === 'graph' && renderGraphPanel()}
          {activePanel === 'video' && renderVideoPanel()}
          {activePanel === 'agents' && renderAgentsPanel()}
          {activePanel === 'motion' && renderMotionPanel()}
          {activePanel === 'docs' && renderDocsPanel()}
        </main>
      </div>

      {commandOpen && (
        <div className="command" role="dialog" aria-modal="true" aria-label="Command palette" onClick={() => setCommandOpen(false)}>
          <div className="command__content" onClick={e => e.stopPropagation()}>
            <div className="command__header">
              <p>Command Palette</p>
              <p className="command__hint">⇧⌘P / ⇧Ctrl+P</p>
            </div>
            <div className="command__actions">
              {actions.map(action => (
                <button
                  key={action}
                  className={`command__item ${action === focusedAction ? 'command__item--active' : ''}`}
                  onMouseEnter={() => setFocusedAction(action)}
                  onFocus={() => setFocusedAction(action)}
                  onClick={() => {
                    setFocusedAction(action)
                    setCommandOpen(false)
                  }}
                >
                  {action}
                </button>
              ))}
            </div>
            <p className="panel__note">Palette mirrors dock actions and macros. Esc to close.</p>
          </div>
        </div>
      )}
    </div>
  )
}

