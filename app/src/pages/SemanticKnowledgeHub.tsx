import { useState, useCallback, useRef, useEffect, useMemo } from 'react'

// Types for the knowledge graph
interface KnowledgeNode {
  id: string
  label: string
  category: string
  description: string
  resources: string[]
  connections: string[]
  x: number
  y: number
  completeness: number
  isExpanded: boolean
}

// GitFather Knowledge Categories
const knowledgeCategories = {
  'ai-creation': { color: '#00f0ff', icon: '🤖', label: 'AI Code Creation' },
  'cicd': { color: '#ff00ff', icon: '⚡', label: 'CI/CD Workflows' },
  'ux-patterns': { color: '#00ff88', icon: '🎨', label: 'UX/UI Patterns' },
  'security': { color: '#ff6600', icon: '🔐', label: 'Security' },
  'cloud': { color: '#7b2fff', icon: '☁️', label: 'Cloud & Infrastructure' },
  'data': { color: '#ffff00', icon: '📊', label: 'Data & Analytics' },
  'automation': { color: '#ff3366', icon: '🔄', label: 'Automation' },
  'learning': { color: '#33ffcc', icon: '📚', label: 'Learning Resources' }
}

// Initial knowledge nodes representing the GitFather architecture
const initialNodes: KnowledgeNode[] = [
  {
    id: 'hub-center',
    label: 'GitFather Hub',
    category: 'ai-creation',
    description: 'Central semantic knowledge hub for AI-first development. Your comprehensive gateway to the developer ecosystem.',
    resources: ['README.md', 'CONTRIBUTING.md', 'LICENSE'],
    connections: ['ai-assistants', 'local-llms', 'cicd-tools', 'cloud-platforms', 'security-tools'],
    x: 400, y: 300, completeness: 95, isExpanded: false
  },
  {
    id: 'ai-assistants',
    label: 'AI Assistants',
    category: 'ai-creation',
    description: 'GPT-5.1, Claude Opus 4.5, Gemini 3, GitHub Copilot, Cursor, Windsurf - Your AI coding companions.',
    resources: ['Copilot', 'Claude Code', 'Cursor', 'Windsurf', 'Codeium'],
    connections: ['hub-center', 'local-llms', 'code-generation'],
    x: 200, y: 150, completeness: 90, isExpanded: false
  },
  {
    id: 'local-llms',
    label: 'Local LLMs',
    category: 'ai-creation',
    description: 'Run AI models locally with Ollama, LM Studio, GPT4All, and Open WebUI for privacy and speed.',
    resources: ['Ollama', 'LM Studio', 'GPT4All', 'Open WebUI', 'LocalAI'],
    connections: ['hub-center', 'ai-assistants', 'privacy-tools'],
    x: 600, y: 150, completeness: 85, isExpanded: false
  },
  {
    id: 'cicd-tools',
    label: 'CI/CD Pipeline',
    category: 'cicd',
    description: 'Automate your workflows with GitHub Actions, Jenkins, CircleCI, and more.',
    resources: ['GitHub Actions', 'Jenkins', 'CircleCI', 'GitLab CI', 'ArgoCD'],
    connections: ['hub-center', 'automation-bots', 'cloud-platforms'],
    x: 150, y: 400, completeness: 88, isExpanded: false
  },
  {
    id: 'cloud-platforms',
    label: 'Cloud Platforms',
    category: 'cloud',
    description: 'AWS, GCP, Azure, Vercel, Cloudflare - Deploy and scale your applications.',
    resources: ['AWS', 'GCP', 'Azure', 'Vercel', 'Cloudflare', 'Netlify'],
    connections: ['hub-center', 'cicd-tools', 'serverless'],
    x: 650, y: 400, completeness: 92, isExpanded: false
  },
  {
    id: 'security-tools',
    label: 'Security Suite',
    category: 'security',
    description: 'Protect your code with CodeQL, Snyk, Dependabot, and security scanning tools.',
    resources: ['CodeQL', 'Snyk', 'Dependabot', 'OWASP', 'SonarQube'],
    connections: ['hub-center', 'cicd-tools', 'privacy-tools'],
    x: 250, y: 550, completeness: 78, isExpanded: false
  },
  {
    id: 'code-generation',
    label: 'Code Generation',
    category: 'ai-creation',
    description: 'AI-powered code scaffolding, refactoring, and generation tools.',
    resources: ['Codex', 'Tabnine', 'Amazon Q', 'Sourcegraph Cody'],
    connections: ['ai-assistants', 'ux-components'],
    x: 100, y: 250, completeness: 82, isExpanded: false
  },
  {
    id: 'automation-bots',
    label: 'Automation Bots',
    category: 'automation',
    description: 'Automate repetitive tasks with bots, scripts, and workflow automation.',
    resources: ['Probot', 'Renovate', 'Semantic Release', 'Auto'],
    connections: ['cicd-tools', 'hub-center'],
    x: 100, y: 500, completeness: 75, isExpanded: false
  },
  {
    id: 'ux-components',
    label: 'UX Components',
    category: 'ux-patterns',
    description: 'Design systems, component libraries, and UX patterns for modern applications.',
    resources: ['Shadcn/ui', 'Radix UI', 'Tailwind', 'Chakra UI', 'MUI'],
    connections: ['code-generation', 'design-tools'],
    x: 50, y: 350, completeness: 88, isExpanded: false
  },
  {
    id: 'design-tools',
    label: 'Design Tools',
    category: 'ux-patterns',
    description: 'Figma, Framer, Storybook - Design and prototype your applications.',
    resources: ['Figma', 'Framer', 'Storybook', 'Chromatic'],
    connections: ['ux-components', 'three-d'],
    x: 50, y: 450, completeness: 80, isExpanded: false
  },
  {
    id: 'three-d',
    label: '3D/WebGL',
    category: 'ux-patterns',
    description: 'Three.js, React Three Fiber, Babylon.js - Create immersive 3D experiences.',
    resources: ['Three.js', 'R3F', 'Babylon.js', 'Spline', 'Theatre.js'],
    connections: ['design-tools', 'motion-animation'],
    x: 150, y: 650, completeness: 70, isExpanded: false
  },
  {
    id: 'motion-animation',
    label: 'Motion & Animation',
    category: 'ux-patterns',
    description: 'GSAP, Framer Motion, Lottie - Bring your interfaces to life.',
    resources: ['GSAP', 'Framer Motion', 'Lottie', 'anime.js'],
    connections: ['three-d', 'ux-components'],
    x: 350, y: 650, completeness: 72, isExpanded: false
  },
  {
    id: 'serverless',
    label: 'Serverless',
    category: 'cloud',
    description: 'AWS Lambda, Cloudflare Workers, Vercel Edge - Scale without servers.',
    resources: ['AWS Lambda', 'Cloudflare Workers', 'Vercel Edge', 'Deno Deploy'],
    connections: ['cloud-platforms', 'data-stores'],
    x: 700, y: 250, completeness: 85, isExpanded: false
  },
  {
    id: 'data-stores',
    label: 'Data & Databases',
    category: 'data',
    description: 'PostgreSQL, MongoDB, Redis, Vector DBs - Store and query your data.',
    resources: ['PostgreSQL', 'MongoDB', 'Redis', 'Pinecone', 'Weaviate'],
    connections: ['serverless', 'ai-assistants'],
    x: 750, y: 350, completeness: 90, isExpanded: false
  },
  {
    id: 'privacy-tools',
    label: 'Privacy Tools',
    category: 'security',
    description: 'Encryption, VPNs, secure communication - Protect your data and privacy.',
    resources: ['GPG', 'Age', 'Vault', 'Bitwarden'],
    connections: ['security-tools', 'local-llms'],
    x: 550, y: 550, completeness: 65, isExpanded: false
  },
  {
    id: 'learning-hub',
    label: 'Learning Hub',
    category: 'learning',
    description: 'Tutorials, documentation, courses - Learn and grow your skills.',
    resources: ['FreeCodeCamp', 'The Odin Project', 'Roadmap.sh', 'Dev.to'],
    connections: ['hub-center', 'ai-assistants'],
    x: 400, y: 500, completeness: 95, isExpanded: false
  }
]

// Semantic Search Component
function SemanticSearch({ onSearch }: { onSearch: (query: string, results: KnowledgeNode[]) => void }) {
  const [query, setQuery] = useState('')
  const [suggestions, setSuggestions] = useState<string[]>([])
  
  const semanticKeywords: Record<string, string[]> = {
    'ai': ['ai-assistants', 'local-llms', 'code-generation'],
    'llm': ['local-llms', 'ai-assistants'],
    'code': ['code-generation', 'ai-assistants', 'cicd-tools'],
    'deploy': ['cloud-platforms', 'serverless', 'cicd-tools'],
    'security': ['security-tools', 'privacy-tools'],
    'design': ['design-tools', 'ux-components', 'three-d'],
    'automation': ['automation-bots', 'cicd-tools'],
    'database': ['data-stores'],
    '3d': ['three-d', 'motion-animation'],
    'animation': ['motion-animation', 'three-d'],
    'learn': ['learning-hub'],
    'cloud': ['cloud-platforms', 'serverless']
  }
  
  const handleSearch = useCallback((searchQuery: string) => {
    setQuery(searchQuery)
    
    if (searchQuery.length > 0) {
      const lowerQuery = searchQuery.toLowerCase()
      
      // Find matching keywords - optimize by early return
      const matchedKeywords: string[] = []
      const keywordList = Object.keys(semanticKeywords)
      for (let i = 0; i < keywordList.length && matchedKeywords.length < 5; i++) {
        const k = keywordList[i]
        if (k.includes(lowerQuery) || lowerQuery.includes(k)) {
          matchedKeywords.push(k)
        }
      }
      setSuggestions(matchedKeywords)
      
      // Find matching nodes - optimize by building set once
      const matchedNodeIds = new Set<string>()
      for (const keyword of matchedKeywords) {
        const nodeIds = semanticKeywords[keyword]
        for (const id of nodeIds) {
          matchedNodeIds.add(id)
        }
      }
      
      // Also search by label and description - single pass
      const results: KnowledgeNode[] = []
      for (const node of initialNodes) {
        if (matchedNodeIds.has(node.id)) {
          results.push(node)
          continue
        }
        
        // Check label and description
        const labelMatch = node.label.toLowerCase().includes(lowerQuery)
        const descMatch = node.description.toLowerCase().includes(lowerQuery)
        
        if (labelMatch || descMatch) {
          results.push(node)
          continue
        }
        
        // Check resources only if not already matched
        for (const resource of node.resources) {
          if (resource.toLowerCase().includes(lowerQuery)) {
            results.push(node)
            break
          }
        }
      }
      
      onSearch(searchQuery, results)
    } else {
      setSuggestions([])
      onSearch('', [])
    }
  }, [onSearch])
  
  return (
    <div className="semantic-search">
      <div className="search-input-wrapper">
        <span className="search-icon">🔍</span>
        <input
          type="text"
          value={query}
          onChange={(e) => handleSearch(e.target.value)}
          placeholder="Search knowledge graph... (try: ai, deploy, security)"
          className="search-input"
        />
        {query && (
          <button className="clear-btn" onClick={() => handleSearch('')}>✕</button>
        )}
      </div>
      {suggestions.length > 0 && (
        <div className="suggestions">
          {suggestions.map(s => (
            <button key={s} className="suggestion-tag" onClick={() => handleSearch(s)}>
              {s}
            </button>
          ))}
        </div>
      )}
    </div>
  )
}

// Completeness Meter Component
function CompletenessMeter({ value, label }: { value: number, label: string }) {
  return (
    <div className="completeness-meter">
      <div className="meter-label">{label}</div>
      <div className="meter-bar">
        <div 
          className="meter-fill" 
          style={{ 
            width: `${value}%`,
            background: value >= 80 ? 'linear-gradient(90deg, #00ff88, #00f0ff)' :
                       value >= 50 ? 'linear-gradient(90deg, #ffff00, #ff6600)' :
                       'linear-gradient(90deg, #ff3366, #ff00ff)'
          }}
        />
      </div>
      <div className="meter-value">{value}%</div>
    </div>
  )
}

// Knowledge Node Detail Panel
function NodeDetailPanel({ node, onClose }: { node: KnowledgeNode | null, onClose: () => void }) {
  if (!node) return null
  
  const category = knowledgeCategories[node.category as keyof typeof knowledgeCategories]
  
  return (
    <div className="node-detail-panel" style={{ borderColor: category.color }}>
      <button className="close-btn" onClick={onClose}>✕</button>
      <div className="panel-header" style={{ background: `linear-gradient(135deg, ${category.color}33, transparent)` }}>
        <span className="category-icon">{category.icon}</span>
        <div>
          <h3>{node.label}</h3>
          <span className="category-label" style={{ color: category.color }}>{category.label}</span>
        </div>
      </div>
      
      <div className="panel-content">
        <p className="description">{node.description}</p>
        
        <CompletenessMeter value={node.completeness} label="Coverage" />
        
        <div className="resources-section">
          <h4>🔗 Resources ({node.resources.length})</h4>
          <div className="resource-tags">
            {node.resources.map(r => (
              <span key={r} className="resource-tag" style={{ borderColor: category.color }}>
                {r}
              </span>
            ))}
          </div>
        </div>
        
        <div className="connections-section">
          <h4>🔄 Connected Nodes ({node.connections.length})</h4>
          <div className="connection-list">
            {node.connections.map(c => {
              const connectedNode = initialNodes.find(n => n.id === c)
              return connectedNode ? (
                <span key={c} className="connection-item">
                  {connectedNode.label}
                </span>
              ) : null
            })}
          </div>
        </div>
      </div>
    </div>
  )
}

// Main Semantic Knowledge Hub Component
export default function SemanticKnowledgeHub() {
  const [nodes, setNodes] = useState(initialNodes)
  const [selectedNode, setSelectedNode] = useState<KnowledgeNode | null>(null)
  const [highlightedNodes, setHighlightedNodes] = useState<Set<string>>(new Set())
  const [searchQuery, setSearchQuery] = useState('')
  const [viewMode, setViewMode] = useState<'graph' | 'list' | 'categories'>('graph')
  const [zoom, setZoom] = useState(1)
  const [pan, setPan] = useState({ x: 0, y: 0 })
  const svgRef = useRef<SVGSVGElement>(null)
  const isDragging = useRef(false)
  const dragStart = useRef({ x: 0, y: 0 })
  
  // Handle search
  const handleSearch = (query: string, results: KnowledgeNode[]) => {
    setSearchQuery(query)
    if (results.length > 0) {
      setHighlightedNodes(new Set(results.map(n => n.id)))
    } else {
      setHighlightedNodes(new Set())
    }
  }
  
  // Handle node click - wrapped in useCallback for stability
  const handleNodeClick = useCallback((node: KnowledgeNode) => {
    setSelectedNode(node)
  }, [])
  
  // Handle pan
  const handleMouseDown = (e: React.MouseEvent) => {
    if (e.target === svgRef.current) {
      isDragging.current = true
      dragStart.current = { x: e.clientX - pan.x, y: e.clientY - pan.y }
    }
  }
  
  const handleMouseMove = (e: React.MouseEvent) => {
    if (isDragging.current) {
      setPan({
        x: e.clientX - dragStart.current.x,
        y: e.clientY - dragStart.current.y
      })
    }
  }
  
  const handleMouseUp = () => {
    isDragging.current = false
  }
  
  // Calculate overall completeness - memoize
  const overallCompleteness = useMemo(() => 
    Math.round(nodes.reduce((sum, n) => sum + n.completeness, 0) / nodes.length),
    [nodes]
  )
  
  // Render connections - memoize to avoid recreating on every render
  const connections = useMemo(() => {
    const result: JSX.Element[] = []
    const hasHighlights = highlightedNodes.size > 0
    
    nodes.forEach(node => {
      node.connections.forEach(targetId => {
        const target = nodes.find(n => n.id === targetId)
        if (target && node.id < targetId) {
          const isHighlighted = !hasHighlights || 
            (highlightedNodes.has(node.id) && highlightedNodes.has(targetId))
          result.push(
            <line
              key={`${node.id}-${targetId}`}
              x1={node.x}
              y1={node.y}
              x2={target.x}
              y2={target.y}
              stroke={isHighlighted ? '#00f0ff' : '#333'}
              strokeWidth={isHighlighted ? 2 : 1}
              strokeDasharray={isHighlighted ? 'none' : '5,5'}
              opacity={isHighlighted ? 0.8 : 0.3}
            />
          )
        }
      })
    })
    return result
  }, [nodes, highlightedNodes])
  
  // Render nodes - memoize
  const nodeElements = useMemo(() => {
    const hasHighlights = highlightedNodes.size > 0
    
    return nodes.map(node => {
      const category = knowledgeCategories[node.category as keyof typeof knowledgeCategories]
      const isHighlighted = !hasHighlights || highlightedNodes.has(node.id)
      const isSelected = selectedNode?.id === node.id
      
      return (
        <g
          key={node.id}
          transform={`translate(${node.x}, ${node.y})`}
          onClick={() => handleNodeClick(node)}
          style={{ cursor: 'pointer' }}
          opacity={isHighlighted ? 1 : 0.4}
        >
          {/* Glow effect */}
          {isSelected && (
            <circle
              r={50}
              fill="none"
              stroke={category.color}
              strokeWidth={3}
              opacity={0.5}
              className="pulse-animation"
            />
          )}
          
          {/* Node circle */}
          <circle
            r={35}
            fill={`${category.color}33`}
            stroke={category.color}
            strokeWidth={isSelected ? 3 : 2}
          />
          
          {/* Completeness ring */}
          <circle
            r={40}
            fill="none"
            stroke={category.color}
            strokeWidth={4}
            strokeDasharray={`${node.completeness * 2.51} 251`}
            strokeLinecap="round"
            transform="rotate(-90)"
            opacity={0.8}
          />
          
          {/* Icon */}
          <text
            y={-5}
            textAnchor="middle"
            fontSize={20}
          >
            {category.icon}
          </text>
          
          {/* Label */}
          <text
            y={15}
            textAnchor="middle"
            fill="white"
            fontSize={10}
            fontWeight="bold"
          >
            {node.label.length > 12 ? node.label.slice(0, 12) + '...' : node.label}
          </text>
          
          {/* Completeness percentage */}
          <text
            y={55}
            textAnchor="middle"
            fill={category.color}
            fontSize={10}
          >
            {node.completeness}%
          </text>
        </g>
      )
    })
  }, [nodes, highlightedNodes, selectedNode, handleNodeClick])
  
  return (
    <div className="semantic-knowledge-hub">
      <style>{`
        .semantic-knowledge-hub {
          min-height: 100vh;
          background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0d0d2a 100%);
          color: white;
          font-family: 'Inter', system-ui, sans-serif;
        }
        
        .hub-header {
          padding: 20px 30px;
          background: rgba(0, 0, 0, 0.5);
          border-bottom: 1px solid rgba(0, 240, 255, 0.3);
          display: flex;
          justify-content: space-between;
          align-items: center;
          flex-wrap: wrap;
          gap: 20px;
        }
        
        .hub-title {
          display: flex;
          align-items: center;
          gap: 15px;
        }
        
        .hub-title h1 {
          margin: 0;
          font-size: 24px;
          background: linear-gradient(90deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }
        
        .hub-title .tagline {
          color: #888;
          font-size: 14px;
        }
        
        .hub-stats {
          display: flex;
          gap: 30px;
        }
        
        .stat-item {
          text-align: center;
        }
        
        .stat-value {
          font-size: 24px;
          font-weight: bold;
          color: #00f0ff;
        }
        
        .stat-label {
          font-size: 12px;
          color: #888;
        }
        
        .hub-controls {
          display: flex;
          gap: 10px;
          align-items: center;
          flex-wrap: wrap;
        }
        
        .view-toggle {
          display: flex;
          background: rgba(0, 0, 0, 0.3);
          border-radius: 8px;
          overflow: hidden;
        }
        
        .view-btn {
          padding: 10px 20px;
          background: transparent;
          border: none;
          color: #888;
          cursor: pointer;
          transition: all 0.3s;
        }
        
        .view-btn.active {
          background: rgba(0, 240, 255, 0.2);
          color: #00f0ff;
        }
        
        .zoom-controls {
          display: flex;
          gap: 5px;
        }
        
        .zoom-btn {
          width: 36px;
          height: 36px;
          background: rgba(0, 0, 0, 0.3);
          border: 1px solid #333;
          border-radius: 8px;
          color: white;
          cursor: pointer;
          transition: all 0.3s;
        }
        
        .zoom-btn:hover {
          background: rgba(0, 240, 255, 0.2);
          border-color: #00f0ff;
        }
        
        .semantic-search {
          width: 100%;
          max-width: 500px;
        }
        
        .search-input-wrapper {
          position: relative;
          display: flex;
          align-items: center;
        }
        
        .search-icon {
          position: absolute;
          left: 15px;
          font-size: 16px;
        }
        
        .search-input {
          width: 100%;
          padding: 12px 40px;
          background: rgba(0, 0, 0, 0.3);
          border: 1px solid #333;
          border-radius: 25px;
          color: white;
          font-size: 14px;
          transition: all 0.3s;
        }
        
        .search-input:focus {
          outline: none;
          border-color: #00f0ff;
          box-shadow: 0 0 20px rgba(0, 240, 255, 0.2);
        }
        
        .clear-btn {
          position: absolute;
          right: 15px;
          background: none;
          border: none;
          color: #888;
          cursor: pointer;
        }
        
        .suggestions {
          display: flex;
          gap: 8px;
          margin-top: 10px;
          flex-wrap: wrap;
        }
        
        .suggestion-tag {
          padding: 5px 12px;
          background: rgba(0, 240, 255, 0.1);
          border: 1px solid #00f0ff;
          border-radius: 15px;
          color: #00f0ff;
          font-size: 12px;
          cursor: pointer;
          transition: all 0.3s;
        }
        
        .suggestion-tag:hover {
          background: rgba(0, 240, 255, 0.2);
        }
        
        .hub-content {
          display: flex;
          height: calc(100vh - 150px);
        }
        
        .graph-container {
          flex: 1;
          position: relative;
          overflow: hidden;
        }
        
        .knowledge-graph {
          width: 100%;
          height: 100%;
          cursor: grab;
        }
        
        .knowledge-graph:active {
          cursor: grabbing;
        }
        
        .pulse-animation {
          animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
          0%, 100% { opacity: 0.5; transform: scale(1); }
          50% { opacity: 0.8; transform: scale(1.1); }
        }
        
        .node-detail-panel {
          width: 350px;
          background: rgba(0, 0, 0, 0.8);
          border-left: 2px solid;
          padding: 20px;
          overflow-y: auto;
        }
        
        .close-btn {
          float: right;
          background: none;
          border: none;
          color: #888;
          font-size: 20px;
          cursor: pointer;
        }
        
        .panel-header {
          display: flex;
          align-items: center;
          gap: 15px;
          padding: 15px;
          border-radius: 12px;
          margin-bottom: 20px;
        }
        
        .category-icon {
          font-size: 40px;
        }
        
        .panel-header h3 {
          margin: 0;
          font-size: 18px;
        }
        
        .category-label {
          font-size: 12px;
        }
        
        .description {
          color: #ccc;
          line-height: 1.6;
          margin-bottom: 20px;
        }
        
        .completeness-meter {
          margin-bottom: 20px;
        }
        
        .meter-label {
          font-size: 12px;
          color: #888;
          margin-bottom: 5px;
        }
        
        .meter-bar {
          height: 8px;
          background: rgba(255, 255, 255, 0.1);
          border-radius: 4px;
          overflow: hidden;
        }
        
        .meter-fill {
          height: 100%;
          border-radius: 4px;
          transition: width 0.5s;
        }
        
        .meter-value {
          font-size: 12px;
          color: #00f0ff;
          text-align: right;
          margin-top: 5px;
        }
        
        .resources-section, .connections-section {
          margin-bottom: 20px;
        }
        
        .resources-section h4, .connections-section h4 {
          font-size: 14px;
          margin-bottom: 10px;
          color: #ccc;
        }
        
        .resource-tags {
          display: flex;
          flex-wrap: wrap;
          gap: 8px;
        }
        
        .resource-tag {
          padding: 5px 12px;
          background: rgba(255, 255, 255, 0.05);
          border: 1px solid;
          border-radius: 15px;
          font-size: 12px;
        }
        
        .connection-list {
          display: flex;
          flex-direction: column;
          gap: 5px;
        }
        
        .connection-item {
          padding: 8px 12px;
          background: rgba(255, 255, 255, 0.05);
          border-radius: 8px;
          font-size: 12px;
        }
        
        .category-legend {
          display: flex;
          flex-wrap: wrap;
          gap: 10px;
          padding: 15px 30px;
          background: rgba(0, 0, 0, 0.3);
          border-top: 1px solid #333;
        }
        
        .legend-item {
          display: flex;
          align-items: center;
          gap: 8px;
          font-size: 12px;
          color: #888;
        }
        
        .legend-dot {
          width: 12px;
          height: 12px;
          border-radius: 50%;
        }
        
        /* List View Styles */
        .list-view {
          padding: 30px;
          overflow-y: auto;
          width: 100%;
        }
        
        .list-item {
          display: flex;
          align-items: center;
          gap: 20px;
          padding: 20px;
          background: rgba(0, 0, 0, 0.3);
          border-radius: 12px;
          margin-bottom: 15px;
          border: 1px solid #333;
          cursor: pointer;
          transition: all 0.3s;
        }
        
        .list-item:hover {
          border-color: #00f0ff;
          background: rgba(0, 240, 255, 0.05);
        }
        
        .list-icon {
          font-size: 30px;
        }
        
        .list-content {
          flex: 1;
        }
        
        .list-content h4 {
          margin: 0 0 5px 0;
        }
        
        .list-content p {
          margin: 0;
          color: #888;
          font-size: 14px;
        }
        
        .list-completeness {
          width: 100px;
        }
        
        /* Categories View Styles */
        .categories-view {
          padding: 30px;
          overflow-y: auto;
          width: 100%;
        }
        
        .category-section {
          margin-bottom: 30px;
        }
        
        .category-header {
          display: flex;
          align-items: center;
          gap: 10px;
          margin-bottom: 15px;
          padding-bottom: 10px;
          border-bottom: 2px solid;
        }
        
        .category-header h3 {
          margin: 0;
        }
        
        .category-nodes {
          display: grid;
          grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
          gap: 15px;
        }
        
        .category-node-card {
          padding: 15px;
          background: rgba(0, 0, 0, 0.3);
          border-radius: 12px;
          border: 1px solid #333;
          cursor: pointer;
          transition: all 0.3s;
        }
        
        .category-node-card:hover {
          border-color: currentColor;
        }
        
        .back-btn {
          display: inline-flex;
          align-items: center;
          gap: 8px;
          padding: 10px 20px;
          background: rgba(0, 0, 0, 0.3);
          border: 1px solid #333;
          border-radius: 8px;
          color: white;
          text-decoration: none;
          margin: 15px;
          transition: all 0.3s;
        }
        
        .back-btn:hover {
          border-color: #00f0ff;
          background: rgba(0, 240, 255, 0.1);
        }
      `}</style>
      
      <a href="/DevTeam6/" className="back-btn">← Back to 3D Demo</a>
      
      <header className="hub-header">
        <div className="hub-title">
          <h1>🧠 GitFather Semantic Knowledge Hub</h1>
          <span className="tagline">AI-First Developer Ecosystem Navigator</span>
        </div>
        
        <div className="hub-stats">
          <div className="stat-item">
            <div className="stat-value">{nodes.length}</div>
            <div className="stat-label">Knowledge Nodes</div>
          </div>
          <div className="stat-item">
            <div className="stat-value">{overallCompleteness}%</div>
            <div className="stat-label">Coverage</div>
          </div>
          <div className="stat-item">
            <div className="stat-value">{nodes.reduce((sum, n) => sum + n.resources.length, 0)}</div>
            <div className="stat-label">Resources</div>
          </div>
        </div>
        
        <div className="hub-controls">
          <SemanticSearch onSearch={handleSearch} />
          
          <div className="view-toggle">
            <button 
              className={`view-btn ${viewMode === 'graph' ? 'active' : ''}`}
              onClick={() => setViewMode('graph')}
            >
              Graph
            </button>
            <button 
              className={`view-btn ${viewMode === 'list' ? 'active' : ''}`}
              onClick={() => setViewMode('list')}
            >
              List
            </button>
            <button 
              className={`view-btn ${viewMode === 'categories' ? 'active' : ''}`}
              onClick={() => setViewMode('categories')}
            >
              Categories
            </button>
          </div>
          
          <div className="zoom-controls">
            <button className="zoom-btn" onClick={() => setZoom(z => Math.min(z + 0.2, 2))}>+</button>
            <button className="zoom-btn" onClick={() => setZoom(z => Math.max(z - 0.2, 0.5))}>−</button>
            <button className="zoom-btn" onClick={() => { setZoom(1); setPan({ x: 0, y: 0 }); }}>⟲</button>
          </div>
        </div>
      </header>
      
      <div className="hub-content">
        {viewMode === 'graph' && (
          <div className="graph-container">
            <svg
              ref={svgRef}
              className="knowledge-graph"
              viewBox={`${-pan.x / zoom - 400} ${-pan.y / zoom - 100} ${900 / zoom} ${700 / zoom}`}
              onMouseDown={handleMouseDown}
              onMouseMove={handleMouseMove}
              onMouseUp={handleMouseUp}
              onMouseLeave={handleMouseUp}
            >
              <defs>
                <filter id="glow">
                  <feGaussianBlur stdDeviation="3" result="coloredBlur"/>
                  <feMerge>
                    <feMergeNode in="coloredBlur"/>
                    <feMergeNode in="SourceGraphic"/>
                  </feMerge>
                </filter>
              </defs>
              
              {/* Grid background */}
              <pattern id="grid" width="50" height="50" patternUnits="userSpaceOnUse">
                <path d="M 50 0 L 0 0 0 50" fill="none" stroke="#222" strokeWidth="0.5"/>
              </pattern>
              <rect x="-500" y="-200" width="1500" height="1000" fill="url(#grid)" />
              
              {/* Connections */}
              <g filter="url(#glow)">
                {connections}
              </g>
              
              {/* Nodes */}
              <g filter="url(#glow)">
                {nodeElements}
              </g>
            </svg>
          </div>
        )}
        
        {viewMode === 'list' && (
          <div className="list-view">
            {nodes.map(node => {
              const category = knowledgeCategories[node.category as keyof typeof knowledgeCategories]
              return (
                <div 
                  key={node.id} 
                  className="list-item"
                  onClick={() => handleNodeClick(node)}
                  style={{ borderColor: selectedNode?.id === node.id ? category.color : undefined }}
                >
                  <span className="list-icon">{category.icon}</span>
                  <div className="list-content">
                    <h4>{node.label}</h4>
                    <p>{node.description.slice(0, 100)}...</p>
                  </div>
                  <div className="list-completeness">
                    <CompletenessMeter value={node.completeness} label="" />
                  </div>
                </div>
              )
            })}
          </div>
        )}
        
        {viewMode === 'categories' && (
          <div className="categories-view">
            {Object.entries(knowledgeCategories).map(([catId, cat]) => {
              const categoryNodes = nodes.filter(n => n.category === catId)
              if (categoryNodes.length === 0) return null
              
              return (
                <div key={catId} className="category-section">
                  <div className="category-header" style={{ borderColor: cat.color }}>
                    <span style={{ fontSize: 24 }}>{cat.icon}</span>
                    <h3 style={{ color: cat.color }}>{cat.label}</h3>
                    <span style={{ color: '#888', marginLeft: 'auto' }}>{categoryNodes.length} nodes</span>
                  </div>
                  <div className="category-nodes">
                    {categoryNodes.map(node => (
                      <div 
                        key={node.id}
                        className="category-node-card"
                        style={{ color: cat.color }}
                        onClick={() => handleNodeClick(node)}
                      >
                        <h4>{node.label}</h4>
                        <p style={{ color: '#888', fontSize: 12 }}>{node.resources.length} resources</p>
                        <CompletenessMeter value={node.completeness} label="" />
                      </div>
                    ))}
                  </div>
                </div>
              )
            })}
          </div>
        )}
        
        <NodeDetailPanel node={selectedNode} onClose={() => setSelectedNode(null)} />
      </div>
      
      <div className="category-legend">
        {Object.entries(knowledgeCategories).map(([id, cat]) => (
          <div key={id} className="legend-item">
            <span className="legend-dot" style={{ background: cat.color }}></span>
            <span>{cat.icon} {cat.label}</span>
          </div>
        ))}
      </div>
    </div>
  )
}
