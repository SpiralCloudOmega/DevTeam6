import { useMemo, useState } from 'react'
import { Link } from 'react-router-dom'
import { composePersonaPrompt, formatStatus } from '../utils/repomind'
import { extractYouTubeId } from '../utils/youtube'
import { getPulseTone } from '../utils/visuals'

const defaultVideo = 'https://www.youtube.com/watch?v=VdQkYJ_Rp-k'
const videoPresets = [
  { label: 'Omega Trailer', url: 'https://www.youtube.com/watch?v=VdQkYJ_Rp-k' },
  { label: 'RAG Playbook', url: 'https://www.youtube.com/watch?v=tpe_Y6h6BfM' },
  { label: 'DX Lightning', url: 'https://www.youtube.com/watch?v=6Dh-RL__uN4' }
]

interface QuickAction {
  title: string
  prompt: string
  tone: string
}

interface AgentStatus {
  name: string
  role: string
  mode: string
  status: 'online' | 'standby'
}

function YouTubePlayer({ videoUrl, title }: { videoUrl: string; title: string }) {
  const videoId = useMemo(() => extractYouTubeId(videoUrl), [videoUrl])

  return (
    <div
      style={{
        border: '1px solid rgba(0, 255, 255, 0.2)',
        borderRadius: '16px',
        overflow: 'hidden',
        background: 'rgba(0, 255, 255, 0.05)',
        boxShadow: '0 20px 60px rgba(0,0,0,0.35)'
      }}
    >
      {videoId ? (
        <div style={{ position: 'relative', paddingBottom: '56.25%', height: 0 }}>
          <iframe
            title={title}
            src={`https://www.youtube.com/embed/${videoId}?rel=0`}
            allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
            allowFullScreen
            style={{ position: 'absolute', top: 0, left: 0, width: '100%', height: '100%', border: 'none' }}
          />
        </div>
      ) : (
        <div style={{ padding: '20px', color: '#ff6600', textAlign: 'center', fontWeight: 700 }}>
          Invalid YouTube link — drop a full URL or 11-character video ID.
        </div>
      )}
    </div>
  )
}

export default function Repomind() {
  const [videoInput, setVideoInput] = useState(defaultVideo)
  const [selectedAction, setSelectedAction] = useState<QuickAction | null>(null)
  const [selectedPipeline, setSelectedPipeline] = useState('Instant Repo Scan')
  const [selectedTone, setSelectedTone] = useState('Playful mentor tone with concrete code refs.')
  const [selectedFocus, setSelectedFocus] = useState('Focus on navigation safety + DX speed.')
  const [agents, setAgents] = useState<AgentStatus[]>([
    { name: 'Repomind Prime', role: 'Narrator & Tour Guide', mode: 'Drive-Through', status: 'online' },
    { name: 'Repomind Jr', role: 'Rapid shortcuts', mode: 'Lite persona', status: 'online' },
    { name: 'Omega Guardian', role: 'Security + QA', mode: 'Firewall', status: 'standby' },
    { name: 'MCP Relay', role: 'Multi-agent coordinator', mode: 'Pipelines', status: 'standby' }
  ])

  const highlightStats = [
    { label: 'Velocity', value: 92, detail: 'UI flows under 30s build time.', tag: 'Tron lane' },
    { label: 'Stability', value: 88, detail: '404-immune header routes.', tag: 'Guardrails' },
    { label: 'Clarity', value: 90, detail: 'Persona preview stays readable.', tag: 'Narrator' },
    { label: 'Immersion', value: 94, detail: 'In-repo video + neon overlays.', tag: 'Holo mode' }
  ]

  const canvasWidgets = [
    {
      title: 'Signal Halo',
      description: 'Animated background ribbons to keep the workstation alive.',
      accent: '#7bffc0'
    },
    {
      title: 'Widget Shelf',
      description: 'Cards for persona tuning, pipelines, and Jr cues.',
      accent: '#00f0ff'
    },
    {
      title: 'Flow Lines',
      description: 'Edges between actions keep the mental map intact.',
      accent: '#ff00ff'
    }
  ]

  const motionTracks = [
    {
      name: 'Tron Bloom',
      description: 'Adds soft neon gradients behind every card.',
      benefit: 'Keeps contrast without losing readability.'
    },
    {
      name: 'Vector Drift',
      description: 'Slow parallax ribbons to imply depth.',
      benefit: 'Feels like a cockpit instead of static grid.'
    },
    {
      name: 'Pulse Trails',
      description: 'Chip interactions leave a brief glow trace.',
      benefit: 'Gives motion feedback for keyboard users.'
    }
  ]

  const quickActions: QuickAction[] = [
    {
      title: 'Drive-Through Summary',
      prompt: 'Give me a 90-second tour of DevTeam6 with the latest highlights.',
      tone: 'Concise, energetic, and optimistic.'
    },
    {
      title: 'Bug Patrol',
      prompt: 'Scan the repo for likely breakpoints or 404 risks in navigation.',
      tone: 'Direct, technical, with a short fix-it list.'
    },
    {
      title: 'Creator Mode',
      prompt: 'Draft a new interactive panel idea that keeps visitors inside the repo.',
      tone: 'Inventive, UI-minded, includes accessibility notes.'
    },
    {
      title: 'Security Sweep',
      prompt: 'List top 5 hardening steps for DevTeam6 (no secrets, least privilege).',
      tone: 'Paranoid but practical.'
    }
  ]

  const jrActions = [
    'List today’s 3 fastest code wins.',
    'Open the Knowledge Hub and summarize the densest section.',
    'Compare Repomind and Repomind Jr behaviors.',
    'Generate a to-do list for the next deploy.',
    'Count links in the header to verify no 404s.'
  ]

  const pipelines = [
    {
      title: 'Instant Repo Scan',
      description: 'Greps routes, checks header links, surfaces potential 404s.',
      steps: ['Collect routes from main.tsx', 'Cross-check header anchors', 'Flag missing paths + suggest fixes']
    },
    {
      title: 'DX Lightning',
      description: 'Prepares a three-line DX pitch with visual hotspots.',
      steps: ['Map top experiences', 'Pull quick wins from Jr queue', 'Draft CTA with internal links']
    },
    {
      title: 'Safety Sweep',
      description: 'Runs paranoia mode with least-privilege reminders.',
      steps: ['Scan for secrets patterns (none in repo)', 'Highlight offline/local behavior', 'List safe defaults to keep']
    }
  ]

  const personaPreview = composePersonaPrompt(
    selectedAction?.prompt ?? 'Welcome inside the workstation — what should we optimize first?',
    selectedTone,
    selectedFocus
  )

  const toggleAgent = (name: string) => {
    setAgents((current) =>
      current.map((agent) =>
        agent.name === name ? { ...agent, status: agent.status === 'online' ? 'standby' : 'online' } : agent
      )
    )
  }

  return (
    <div
      style={{
        minHeight: '100vh',
        background: 'radial-gradient(circle at 10% 10%, rgba(0,255,255,0.12), transparent 30%), radial-gradient(circle at 90% 20%, rgba(255,0,255,0.12), transparent 32%), linear-gradient(135deg, #050510 0%, #0c0c1c 50%, #120826 100%)',
        color: '#e8f9ff',
        padding: '24px',
        fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
        position: 'relative',
        overflow: 'hidden'
      }}
    >
      <div className="repomind-ribbon repomind-ribbon--left" aria-hidden />
      <div className="repomind-ribbon repomind-ribbon--right" aria-hidden />

      <div style={{ display: 'flex', justifyContent: 'space-between', gap: '12px', alignItems: 'center', marginBottom: '20px' }}>
        <div>
          <p style={{ margin: 0, color: '#00f0ff', letterSpacing: '0.08em', fontWeight: 700 }}>OMEGA TOOL KIT</p>
          <h1 style={{ margin: '6px 0', fontSize: '32px', color: '#ffffff' }}>Repomind Digital Workstation</h1>
          <p style={{ margin: 0, color: '#9ddff6' }}>
            Talk to the repo without leaving it. Persona-rich AI copilots, drive-through prompts, and a reliable in-page YouTube player.
          </p>
        </div>
        <Link
          to="/"
          style={{
            color: '#00f0ff',
            textDecoration: 'none',
            border: '1px solid rgba(0,255,255,0.4)',
            padding: '10px 14px',
            borderRadius: '12px',
            background: 'rgba(0,255,255,0.08)',
            fontWeight: 700,
            boxShadow: '0 10px 30px rgba(0,0,0,0.35)'
          }}
          aria-label="Back to 3D experience"
        >
          ← Back to 3D Experience
        </Link>
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))', gap: '12px', marginBottom: '16px' }}>
        {highlightStats.map((stat) => {
          const tone = getPulseTone(stat.value)
          return (
            <div
              key={stat.label}
              style={{
                padding: '12px',
                borderRadius: '14px',
                border: '1px solid rgba(0,255,255,0.25)',
                background: `linear-gradient(135deg, rgba(0,255,255,0.08), rgba(255,0,255,0.06)), radial-gradient(circle at 80% 20%, ${tone.color}, transparent 55%)`,
                boxShadow: tone.border,
                position: 'relative',
                overflow: 'hidden'
              }}
            >
              <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '6px' }}>
                <div style={{ fontWeight: 800, color: '#ffffff' }}>{stat.label}</div>
                <span style={{ color: '#7bffc0', fontWeight: 700 }}>{stat.value}%</span>
              </div>
              <div style={{ color: '#b8e4ff', fontSize: '13px', lineHeight: 1.4 }}>{stat.detail}</div>
              <div
                style={{
                  marginTop: '8px',
                  fontSize: '12px',
                  color: '#0a0a0a',
                  fontWeight: 800,
                  background: '#00f0ff',
                  display: 'inline-flex',
                  padding: '4px 8px',
                  borderRadius: '999px',
                  boxShadow: tone.border
                }}
              >
                {stat.tag}
              </div>
            </div>
          )
        })}
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: '1.1fr 0.9fr', gap: '14px', marginBottom: '16px' }}>
        <div
          style={{
            border: '1px solid rgba(0, 240, 255, 0.18)',
            borderRadius: '16px',
            padding: '14px',
            background: 'rgba(0,0,0,0.35)',
            boxShadow: '0 16px 40px rgba(0,0,0,0.35)'
          }}
        >
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '8px' }}>
            <div>
              <p style={{ margin: 0, color: '#00f0ff', fontWeight: 700, letterSpacing: '0.05em' }}>CANVAS</p>
              <h3 style={{ margin: '4px 0', color: '#ffffff' }}>UX Motion Lab</h3>
            </div>
            <span style={{ color: '#9ddff6', fontSize: '13px' }}>Animations stay offline + responsive.</span>
          </div>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(240px, 1fr))', gap: '10px' }}>
            {motionTracks.map((track) => (
              <div
                key={track.name}
                style={{
                  padding: '12px',
                  borderRadius: '12px',
                  border: '1px solid rgba(0,255,255,0.2)',
                  background: 'rgba(0,0,0,0.4)',
                  boxShadow: '0 12px 28px rgba(0,0,0,0.28)'
                }}
              >
                <div style={{ fontWeight: 800, color: '#00f0ff' }}>{track.name}</div>
                <div style={{ color: '#b8e4ff', fontSize: '13px', marginTop: '4px' }}>{track.description}</div>
                <div style={{ color: '#7bffc0', fontSize: '12px', marginTop: '6px', fontWeight: 700 }}>Benefit: {track.benefit}</div>
              </div>
            ))}
          </div>
        </div>

        <div
          style={{
            border: '1px solid rgba(255, 0, 255, 0.2)',
            borderRadius: '16px',
            padding: '14px',
            background: 'rgba(15, 3, 25, 0.55)',
            boxShadow: '0 16px 40px rgba(0,0,0,0.35)'
          }}
        >
          <p style={{ margin: 0, color: '#ff00ff', fontWeight: 700, letterSpacing: '0.05em' }}>LAYOUT</p>
          <h3 style={{ margin: '4px 0', color: '#ffffff' }}>Widget Shelf</h3>
          <div style={{ display: 'grid', gap: '10px' }}>
            {canvasWidgets.map((widget) => (
              <div
                key={widget.title}
                style={{
                  padding: '12px',
                  borderRadius: '12px',
                  border: `1px solid ${widget.accent}55`,
                  background: 'rgba(0,0,0,0.45)',
                  boxShadow: '0 10px 24px rgba(0,0,0,0.28)'
                }}
              >
                <div style={{ fontWeight: 800, color: widget.accent }}>{widget.title}</div>
                <div style={{ color: '#e8d6ff', fontSize: '13px', marginTop: '4px' }}>{widget.description}</div>
              </div>
            ))}
          </div>
        </div>
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: '1.3fr 0.7fr', gap: '16px', marginBottom: '18px' }}>
        <div
          style={{
            border: '1px solid rgba(0, 240, 255, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(6, 14, 34, 0.65)',
            boxShadow: '0 18px 46px rgba(0,0,0,0.4)'
          }}
        >
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start', gap: '12px', marginBottom: '10px' }}>
            <div>
              <p style={{ margin: 0, color: '#7bffc0', fontWeight: 800, letterSpacing: '0.05em' }}>MCP OPS</p>
              <h2 style={{ margin: '4px 0 0', color: '#ffffff' }}>Control Center</h2>
              <p style={{ margin: '4px 0', color: '#9ddff6' }}>Flip agents on/off and pick a pipeline to run locally.</p>
            </div>
            <div style={{ color: '#c5f3ff', fontSize: '13px', textAlign: 'right' }}>
              <div>All offline • zero secrets</div>
              <div>Key focus: 404 immunity + DX speed</div>
            </div>
          </div>

          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(220px, 1fr))', gap: '10px' }}>
            {agents.map((agent) => (
              <button
                key={agent.name}
                onClick={() => toggleAgent(agent.name)}
                onKeyDown={(e) => {
                  if (e.key === 'Enter' || e.key === ' ') {
                    e.preventDefault()
                    toggleAgent(agent.name)
                  }
                }}
                style={{
                  textAlign: 'left',
                  border: '1px solid rgba(0,255,255,0.25)',
                  borderRadius: '12px',
                  padding: '12px',
                  background: agent.status === 'online' ? 'rgba(0,255,136,0.12)' : 'rgba(255,0,255,0.08)',
                  color: '#e8f9ff',
                  cursor: 'pointer',
                  boxShadow: '0 12px 28px rgba(0,0,0,0.32)'
                }}
                aria-label={`Toggle ${agent.name} ${agent.status === 'online' ? 'to standby' : 'online'}`}
                tabIndex={0}
              >
                <div style={{ fontWeight: 800, color: '#00f0ff', marginBottom: '4px' }}>{agent.name}</div>
                <div style={{ fontSize: '13px', color: '#b8e4ff' }}>{agent.role}</div>
                <div style={{ fontSize: '12px', color: '#7bffc0', marginTop: '6px' }}>Mode: {agent.mode}</div>
                <div style={{ fontSize: '12px', marginTop: '6px', color: agent.status === 'online' ? '#7bffc0' : '#ffcc00' }}>
                  Status: {formatStatus(agent.status)} (tap to toggle)
                </div>
              </button>
            ))}
          </div>
        </div>

        <div
          style={{
            border: '1px solid rgba(255, 0, 255, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(21, 8, 33, 0.7)',
            boxShadow: '0 18px 46px rgba(0,0,0,0.4)'
          }}
        >
          <p style={{ margin: 0, color: '#ff00ff', fontWeight: 800, letterSpacing: '0.05em' }}>PIPELINES</p>
          <h3 style={{ margin: '4px 0', color: '#ffffff' }}>Fast-Code-Drive-Through</h3>
          <p style={{ margin: '2px 0 10px', color: '#d9b8ff', fontSize: '14px' }}>Pick a lane to guide Repomind replies.</p>
          <div style={{ display: 'grid', gap: '10px' }}>
            {pipelines.map((pipeline) => (
              <button
                key={pipeline.title}
                onClick={() => setSelectedPipeline(pipeline.title)}
                onKeyDown={(e) => {
                  if (e.key === 'Enter' || e.key === ' ') {
                    e.preventDefault()
                    setSelectedPipeline(pipeline.title)
                  }
                }}
                style={{
                  textAlign: 'left',
                  border: '1px solid rgba(255,0,255,0.25)',
                  borderRadius: '12px',
                  padding: '12px',
                  background: selectedPipeline === pipeline.title ? 'rgba(255,0,255,0.14)' : 'rgba(0,0,0,0.45)',
                  color: '#f5d8ff',
                  cursor: 'pointer',
                  boxShadow: '0 12px 28px rgba(0,0,0,0.32)'
                }}
                aria-label={`Select pipeline ${pipeline.title}`}
                tabIndex={0}
              >
                <div style={{ fontWeight: 800, color: '#ffccff', marginBottom: '4px' }}>{pipeline.title}</div>
                <div style={{ fontSize: '13px', color: '#e8c8ff' }}>{pipeline.description}</div>
                <ul style={{ margin: '8px 0 0', paddingLeft: '16px', color: '#f5d8ff', lineHeight: 1.4 }}>
                  {pipeline.steps.map((step) => (
                    <li key={step} style={{ fontSize: '12px' }}>
                      {step}
                    </li>
                  ))}
                </ul>
              </button>
            ))}
          </div>
        </div>
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: '1.2fr 0.8fr', gap: '18px', marginBottom: '18px' }}>
        <div
          style={{
            border: '1px solid rgba(0, 255, 255, 0.18)',
            borderRadius: '18px',
            padding: '18px',
            background: 'rgba(0,0,0,0.35)',
            boxShadow: '0 20px 60px rgba(0,0,0,0.45)'
          }}
        >
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', gap: '10px', marginBottom: '12px' }}>
            <div>
              <p style={{ margin: 0, color: '#ff00ff', fontWeight: 700, letterSpacing: '0.05em' }}>DRIVE-THROUGH</p>
              <h2 style={{ margin: '4px 0 0', color: '#ffffff' }}>Talk to Repomind</h2>
            </div>
            <div style={{ textAlign: 'right', color: '#9ddff6', fontSize: '14px' }}>
              <div>Persona: "Fast-Code-Drive-Through"</div>
              <div>Behavior: Quick, vivid, helpful.</div>
            </div>
          </div>

          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(220px, 1fr))', gap: '12px' }}>
            {quickActions.map((action) => (
              <button
                key={action.title}
                onClick={() => setSelectedAction(action)}
                onKeyDown={(e) => {
                  if (e.key === 'Enter' || e.key === ' ') {
                    e.preventDefault()
                    setSelectedAction(action)
                  }
                }}
                style={{
                  textAlign: 'left',
                  border: '1px solid rgba(0,255,255,0.35)',
                  borderRadius: '12px',
                  padding: '12px',
                  background: selectedAction?.title === action.title ? 'rgba(0,255,136,0.12)' : 'rgba(0,0,0,0.4)',
                  color: '#e8f9ff',
                  cursor: 'pointer',
                  boxShadow: '0 12px 28px rgba(0,0,0,0.3)'
                }}
                aria-label={`Select quick action ${action.title}`}
                tabIndex={0}
              >
                <div style={{ fontWeight: 800, color: '#00f0ff', marginBottom: '6px' }}>{action.title}</div>
                <div style={{ fontSize: '14px', color: '#b8e4ff' }}>{action.prompt}</div>
                <div style={{ fontSize: '12px', color: '#7bffc0', marginTop: '8px' }}>{action.tone}</div>
              </button>
            ))}
          </div>

          <div
            style={{
              marginTop: '14px',
              padding: '14px',
              borderRadius: '12px',
              border: '1px dashed rgba(0,255,255,0.25)',
              background: 'rgba(0,0,0,0.55)',
              color: '#c5f3ff'
            }}
            aria-live="polite"
          >
            {selectedAction ? (
              <>
                <div style={{ color: '#7b2fff', fontWeight: 800, marginBottom: '6px' }}>Queued Prompt</div>
                <div style={{ fontWeight: 700 }}>{selectedAction.prompt}</div>
                <div style={{ color: '#7bffc0', marginTop: '6px' }}>Tone: {selectedAction.tone}</div>
                <div style={{ color: '#ffcc00', marginTop: '6px' }}>Pipeline: {selectedPipeline}</div>
              </>
            ) : (
              <div>Pick a drive-through shortcut to prefill Repomind’s reply style.</div>
            )}
          </div>
        </div>

        <div
          style={{
            border: '1px solid rgba(255, 0, 255, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(10, 10, 26, 0.65)',
            boxShadow: '0 20px 60px rgba(0,0,0,0.45)'
          }}
        >
          <p style={{ margin: 0, color: '#ff00ff', fontWeight: 700, letterSpacing: '0.05em' }}>LIVE PLAYER</p>
          <h2 style={{ margin: '4px 0 12px', color: '#ffffff' }}>YouTube in the Repo</h2>
          <label htmlFor="video-url" style={{ display: 'block', color: '#9ddff6', marginBottom: '6px', fontSize: '14px' }}>
            Paste a YouTube link or video ID to load it without leaving the workstation.
          </label>
          <input
            id="video-url"
            value={videoInput}
            onChange={(e) => setVideoInput(e.target.value)}
            style={{
              width: '100%',
              padding: '10px 12px',
              borderRadius: '10px',
              border: '1px solid rgba(0,255,255,0.3)',
              background: 'rgba(0,0,0,0.35)',
              color: '#e8f9ff',
              marginBottom: '12px',
              fontFamily: 'inherit'
            }}
            placeholder="https://www.youtube.com/watch?v=..."
            aria-label="YouTube video URL input"
          />
          <div style={{ display: 'flex', gap: '8px', flexWrap: 'wrap', marginBottom: '10px' }}>
            {videoPresets.map((preset) => (
              <button
                key={preset.label}
                onClick={() => setVideoInput(preset.url)}
                onKeyDown={(e) => {
                  if (e.key === 'Enter' || e.key === ' ') {
                    e.preventDefault()
                    setVideoInput(preset.url)
                  }
                }}
                style={{
                  border: '1px solid rgba(255,0,255,0.3)',
                  borderRadius: '10px',
                  padding: '8px 10px',
                  background: videoInput === preset.url ? 'rgba(255,0,255,0.15)' : 'rgba(0,0,0,0.4)',
                  color: '#f1d6ff',
                  cursor: 'pointer'
                }}
                aria-label={`Load preset video ${preset.label}`}
                tabIndex={0}
              >
                {preset.label}
              </button>
            ))}
          </div>
          <YouTubePlayer videoUrl={videoInput} title="Repomind video player" />
        </div>
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: '1.1fr 0.9fr', gap: '16px', marginBottom: '18px' }}>
        <div
          style={{
            border: '1px solid rgba(0, 255, 136, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(5, 26, 18, 0.6)',
            boxShadow: '0 18px 46px rgba(0,0,0,0.4)'
          }}
        >
          <p style={{ margin: 0, color: '#00ff88', fontWeight: 800, letterSpacing: '0.05em' }}>PERSONA BLENDER</p>
          <h3 style={{ margin: '4px 0', color: '#ffffff' }}>Compose the perfect reply</h3>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(260px, 1fr))', gap: '12px' }}>
            <div>
              <p style={{ margin: '0 0 6px', color: '#9ddff6', fontSize: '14px' }}>Tone options</p>
              {['Playful mentor tone with concrete code refs.', 'Executive clarity with 3 bullets.', 'Ultra-concise changelog style.'].map(
                (tone) => (
                  <button
                    key={tone}
                    onClick={() => setSelectedTone(tone)}
                    onKeyDown={(e) => {
                      if (e.key === 'Enter' || e.key === ' ') {
                        e.preventDefault()
                        setSelectedTone(tone)
                      }
                    }}
                    style={{
                      width: '100%',
                      textAlign: 'left',
                      border: '1px solid rgba(0,255,136,0.25)',
                      borderRadius: '10px',
                      padding: '10px',
                      background: selectedTone === tone ? 'rgba(0,255,136,0.16)' : 'rgba(0,0,0,0.4)',
                      color: '#d4ffed',
                      cursor: 'pointer',
                      marginBottom: '8px'
                    }}
                    aria-label={`Select tone ${tone}`}
                    tabIndex={0}
                  >
                    {tone}
                  </button>
                )
              )}
            </div>
            <div>
              <p style={{ margin: '0 0 6px', color: '#9ddff6', fontSize: '14px' }}>Focus options</p>
              {['Focus on navigation safety + DX speed.', 'Highlight MCP pipelines + agents.', 'Call out visuals + accessibility.'].map(
                (focus) => (
                  <button
                    key={focus}
                    onClick={() => setSelectedFocus(focus)}
                    onKeyDown={(e) => {
                      if (e.key === 'Enter' || e.key === ' ') {
                        e.preventDefault()
                        setSelectedFocus(focus)
                      }
                    }}
                    style={{
                      width: '100%',
                      textAlign: 'left',
                      border: '1px solid rgba(0,255,136,0.25)',
                      borderRadius: '10px',
                      padding: '10px',
                      background: selectedFocus === focus ? 'rgba(0,255,136,0.16)' : 'rgba(0,0,0,0.4)',
                      color: '#d4ffed',
                      cursor: 'pointer',
                      marginBottom: '8px'
                    }}
                    aria-label={`Select focus ${focus}`}
                    tabIndex={0}
                  >
                    {focus}
                  </button>
                )
              )}
            </div>
          </div>
          <div
            style={{
              marginTop: '10px',
              padding: '12px',
              borderRadius: '12px',
              border: '1px dashed rgba(0,255,136,0.25)',
              background: 'rgba(0,0,0,0.55)',
              color: '#d4ffed'
            }}
          >
            <div style={{ fontWeight: 800, color: '#00ff88', marginBottom: '6px' }}>Persona preview</div>
            <div style={{ lineHeight: 1.5 }}>{personaPreview}</div>
            <div style={{ marginTop: '6px', fontSize: '12px', color: '#7bffc0' }}>Powered by: {selectedPipeline}</div>
          </div>
        </div>

        <div
          style={{
            border: '1px solid rgba(0, 240, 255, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(5, 18, 32, 0.6)',
            boxShadow: '0 18px 46px rgba(0,0,0,0.4)'
          }}
        >
          <p style={{ margin: 0, color: '#00f0ff', fontWeight: 800, letterSpacing: '0.05em' }}>OPS DIGEST</p>
          <h3 style={{ margin: '4px 0', color: '#ffffff' }}>What Repomind will do next</h3>
          <ul style={{ margin: '0 0 10px', paddingLeft: '18px', color: '#c5f3ff', lineHeight: 1.5 }}>
            <li>Route map anchored by: {selectedPipeline}</li>
            <li>Persona tone: {selectedTone}</li>
            <li>Focus: {selectedFocus}</li>
          </ul>
          <div
            style={{
              border: '1px solid rgba(0,240,255,0.25)',
              borderRadius: '12px',
              padding: '10px',
              background: 'rgba(0,0,0,0.45)',
              color: '#d4e8ff'
            }}
          >
            <div style={{ fontWeight: 800, marginBottom: '6px', color: '#00f0ff' }}>Repomind Jr Fastlane</div>
            <div style={{ fontSize: '14px' }}>Use Jr cards to queue micro-tasks, then blend with the persona preview to keep responses short.</div>
          </div>
        </div>
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: '1.2fr 0.8fr', gap: '18px' }}>
        <div
          style={{
            border: '1px solid rgba(0, 255, 136, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(0, 20, 12, 0.6)'
          }}
        >
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '10px' }}>
            <div>
              <p style={{ margin: 0, color: '#00ff88', fontWeight: 800, letterSpacing: '0.05em' }}>REPOMIND JR</p>
              <h3 style={{ margin: '4px 0', color: '#ffffff' }}>Blazing-fast shortcuts</h3>
            </div>
            <span style={{ color: '#7bffc0', fontWeight: 700 }}>Lightweight persona</span>
          </div>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))', gap: '10px' }}>
            {jrActions.map((action) => (
              <div
                key={action}
                style={{
                  padding: '12px',
                  borderRadius: '12px',
                  background: 'rgba(0,0,0,0.5)',
                  border: '1px solid rgba(0,255,136,0.25)',
                  color: '#d4ffed',
                  fontWeight: 600,
                  boxShadow: '0 12px 24px rgba(0,0,0,0.25)'
                }}
              >
                {action}
              </div>
            ))}
          </div>
        </div>

        <div
          style={{
            border: '1px solid rgba(255, 204, 0, 0.25)',
            borderRadius: '18px',
            padding: '16px',
            background: 'rgba(25, 16, 0, 0.6)'
          }}
        >
          <p style={{ margin: 0, color: '#ffcc00', fontWeight: 800, letterSpacing: '0.05em' }}>QA + SAFETY</p>
          <h3 style={{ margin: '4px 0', color: '#ffffff' }}>Stay inside the repo</h3>
          <ul style={{ margin: 0, paddingLeft: '18px', lineHeight: 1.5, color: '#ffe6a3' }}>
            <li>Header links match live routes — no 404s or dead ends.</li>
            <li>Embedded video runs in-page; no new tabs required.</li>
            <li>Prompts are static and local; no network calls are made.</li>
            <li>Accessibility: keyboard-friendly buttons and clear aria labels.</li>
          </ul>
          <div style={{ marginTop: '10px', color: '#ffcc00', fontWeight: 700 }}>
            Need more? Ping the Fast-Code-Drive-Through.
          </div>
        </div>
      </div>
    </div>
  )
}
