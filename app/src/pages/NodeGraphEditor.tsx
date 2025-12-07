import { useState, useCallback, useMemo, useEffect } from 'react';
import { computeClusterBounds, computeShortestPath, filterNodesByCluster } from '../utils/graph';

// Custom Node Graph implementation (n8n-style interactive workflow)
// Using SVG-based approach for maximum compatibility

interface Node {
  id: string;
  type: 'start' | 'process' | 'decision' | 'action' | 'end' | 'ai' | 'data' | 'cloud' | 'security';
  label: string;
  description?: string;
  position: { x: number; y: number };
  color: string;
  icon: string;
  connections: string[];
  status?: 'completed' | 'active' | 'pending';
  metadata?: Record<string, string>;
  cluster?: string;
}

interface Edge {
  id: string;
  from: string;
  to: string;
  label?: string;
  animated?: boolean;
}

interface Cluster {
  id: string;
  label: string;
  description: string;
  color: string;
  accent: string;
  nodes: string[];
}

// DevTeam6 Workflow Nodes - mind map and automation presets
const mindmapNodes: Node[] = [
  { id: 'start', type: 'start', label: 'DevTeam6', description: 'Repository Entry Point', position: { x: 420, y: 60 }, color: '#00f0ff', icon: '🚀', connections: ['readme', 'pages'], status: 'completed', cluster: 'core' },
  { id: 'readme', type: 'data', label: 'README.md', description: 'Visibility Layer - Project Dashboard', position: { x: 240, y: 160 }, color: '#ff00ff', icon: '📄', connections: ['resources', 'navigation', 'badges'], status: 'completed', cluster: 'core' },
  { id: 'pages', type: 'cloud', label: 'GitHub Pages', description: 'Interaction Layer - Live Applications', position: { x: 640, y: 160 }, color: '#7b2fff', icon: '🌐', connections: ['3d-demo', 'genui', 'dashboard'], status: 'completed', cluster: 'core' },

  { id: 'resources', type: 'data', label: 'Resources', description: '1000+ Curated Tools & Links', position: { x: 90, y: 300 }, color: '#00ff88', icon: '📚', connections: ['ai-tools', 'dev-tools', 'cloud-tools'], status: 'completed', cluster: 'knowledge' },
  { id: 'navigation', type: 'action', label: 'Navigation', description: 'Quick Jump Buttons', position: { x: 240, y: 300 }, color: '#ff6600', icon: '🧭', connections: [], status: 'completed', cluster: 'knowledge' },
  { id: 'badges', type: 'action', label: 'Badges & Stats', description: 'Dynamic Status Indicators', position: { x: 390, y: 300 }, color: '#ffcc00', icon: '🏅', connections: [], status: 'active', cluster: 'knowledge' },

  { id: '3d-demo', type: 'ai', label: '3D Demo', description: 'Three.js Interactive Experience', position: { x: 540, y: 300 }, color: '#00f0ff', icon: '🎮', connections: [], status: 'completed', cluster: 'experiences' },
  { id: 'genui', type: 'ai', label: 'GenUI Playground', description: 'AI Component Generator', position: { x: 700, y: 300 }, color: '#ff00ff', icon: '🎨', connections: [], status: 'completed', cluster: 'experiences' },
  { id: 'dashboard', type: 'process', label: 'Dashboard', description: 'Gamification & Stats', position: { x: 860, y: 300 }, color: '#00ff88', icon: '📊', connections: ['leaderboard', 'achievements'], status: 'completed', cluster: 'experiences' },

  { id: 'ai-tools', type: 'ai', label: 'AI Tools', description: 'GPT-5.1, Claude, Gemini', position: { x: 90, y: 460 }, color: '#00f0ff', icon: '🤖', connections: [], status: 'completed', cluster: 'catalog' },
  { id: 'dev-tools', type: 'process', label: 'Dev Tools', description: 'IDEs, CLIs, Extensions', position: { x: 200, y: 460 }, color: '#ff00ff', icon: '⚙️', connections: [], status: 'completed', cluster: 'catalog' },
  { id: 'cloud-tools', type: 'cloud', label: 'Cloud', description: 'AWS, GCP, Azure, Vercel', position: { x: 310, y: 460 }, color: '#7b2fff', icon: '☁️', connections: [], status: 'active', cluster: 'catalog' },

  { id: 'leaderboard', type: 'data', label: 'Leaderboard', description: 'Top Contributors', position: { x: 760, y: 460 }, color: '#ff6600', icon: '🏆', connections: [], status: 'completed', cluster: 'engagement' },
  { id: 'achievements', type: 'action', label: 'Achievements', description: 'Collectible Badges', position: { x: 900, y: 460 }, color: '#ffcc00', icon: '🎖️', connections: [], status: 'active', cluster: 'engagement' },

  { id: 'automation', type: 'security', label: 'GitHub Actions', description: 'CI/CD & Sync Pipelines', position: { x: 480, y: 560 }, color: '#aa00ff', icon: '⚡', connections: ['deploy', 'cvs-sync', 'roadmap-sync'], status: 'active', cluster: 'automation' },
  { id: 'deploy', type: 'cloud', label: 'Deploy', description: 'Pages Deployment', position: { x: 320, y: 640 }, color: '#00ff88', icon: '🚀', connections: [], status: 'completed', cluster: 'automation' },
  { id: 'cvs-sync', type: 'process', label: 'CVS Sync', description: 'Hourly Metric Updates', position: { x: 480, y: 640 }, color: '#00f0ff', icon: '🔄', connections: [], status: 'active', cluster: 'automation' },
  { id: 'roadmap-sync', type: 'action', label: 'Roadmap Sync', description: '6-Hour Progress Updates', position: { x: 640, y: 640 }, color: '#ff00ff', icon: '📋', connections: [], status: 'pending', cluster: 'automation' },
];

const automationNodes: Node[] = [
  { id: 'trigger', type: 'start', label: 'Webhook Trigger', description: 'n8n / MCP style entry', position: { x: 220, y: 120 }, color: '#00f0ff', icon: '🛰️', connections: ['ingest'], status: 'completed', cluster: 'signals' },
  { id: 'ingest', type: 'process', label: 'Repo Ingest', description: 'Fetch + watch Git events', position: { x: 420, y: 180 }, color: '#00ff88', icon: '📥', connections: ['vectorize', 'classify'], status: 'active', cluster: 'signals' },
  { id: 'classify', type: 'decision', label: 'Intent Classifier', description: 'Security / DX routing', position: { x: 220, y: 260 }, color: '#ffcc00', icon: '🧠', connections: ['alerts'], status: 'pending', cluster: 'brain' },
  { id: 'vectorize', type: 'ai', label: 'Vector Cluster', description: 'Embed nodes -> graph', position: { x: 420, y: 280 }, color: '#7b2fff', icon: '🧬', connections: ['planner', 'widgets'], status: 'active', cluster: 'brain' },
  { id: 'planner', type: 'ai', label: 'Plan & Route', description: 'CrewAI / MCP planner', position: { x: 620, y: 260 }, color: '#ff00ff', icon: '🗺️', connections: ['exec'], status: 'completed', cluster: 'brain' },
  { id: 'exec', type: 'action', label: 'Action Runner', description: 'Runs pipelines & tests', position: { x: 620, y: 360 }, color: '#00ff88', icon: '⚙️', connections: ['deploy-auto'], status: 'active', cluster: 'delivery' },
  { id: 'widgets', type: 'data', label: 'UX Widgets', description: 'Dashboard + overlays', position: { x: 420, y: 380 }, color: '#00f0ff', icon: '🧩', connections: ['exec'], status: 'completed', cluster: 'delivery' },
  { id: 'alerts', type: 'security', label: 'Security Gates', description: 'Guardrails + approvals', position: { x: 220, y: 360 }, color: '#ff6600', icon: '🛡️', connections: ['exec'], status: 'active', cluster: 'delivery' },
  { id: 'deploy-auto', type: 'cloud', label: 'Deploy + Sync', description: 'Ship to Pages + hub', position: { x: 820, y: 320 }, color: '#00ff88', icon: '☁️', connections: ['observability'], status: 'pending', cluster: 'delivery' },
  { id: 'observability', type: 'process', label: 'Observability', description: 'Dashboards + reports', position: { x: 820, y: 420 }, color: '#ffcc00', icon: '📊', connections: ['retro'], status: 'completed', cluster: 'delivery' },
  { id: 'retro', type: 'end', label: 'Retro & Learn', description: 'Feedback into vector mind', position: { x: 620, y: 460 }, color: '#ff00ff', icon: '🔁', connections: ['vectorize'], status: 'active', cluster: 'brain' },
];

const clustersByPreset: Record<string, Cluster[]> = {
  mindmap: [
    { id: 'core', label: 'Core OS', description: 'Entrypoint + GitHub Pages', color: '#00f0ff', accent: 'rgba(0, 240, 255, 0.25)', nodes: ['start', 'readme', 'pages'] },
    { id: 'knowledge', label: 'Knowledge Hub', description: 'Navigation + badges', color: '#ff6600', accent: 'rgba(255, 102, 0, 0.18)', nodes: ['resources', 'navigation', 'badges'] },
    { id: 'experiences', label: 'Experiences', description: '3D + GenUI + Dashboard', color: '#ff00ff', accent: 'rgba(255, 0, 255, 0.18)', nodes: ['3d-demo', 'genui', 'dashboard'] },
    { id: 'catalog', label: 'Tool Catalog', description: 'AI/Dev/Cloud curations', color: '#00ff88', accent: 'rgba(0, 255, 136, 0.18)', nodes: ['ai-tools', 'dev-tools', 'cloud-tools'] },
    { id: 'engagement', label: 'Engagement', description: 'Leaderboard + achievements', color: '#ffcc00', accent: 'rgba(255, 204, 0, 0.18)', nodes: ['leaderboard', 'achievements'] },
    { id: 'automation', label: 'Automation', description: 'CI/CD + roadmap sync', color: '#aa00ff', accent: 'rgba(170, 0, 255, 0.18)', nodes: ['automation', 'deploy', 'cvs-sync', 'roadmap-sync'] },
  ],
  automation: [
    { id: 'signals', label: 'Signals', description: 'Triggers + ingest', color: '#00f0ff', accent: 'rgba(0, 240, 255, 0.2)', nodes: ['trigger', 'ingest'] },
    { id: 'brain', label: 'Vector Brain', description: 'Cluster + plan + retro', color: '#ff00ff', accent: 'rgba(255, 0, 255, 0.16)', nodes: ['classify', 'vectorize', 'planner', 'retro'] },
    { id: 'delivery', label: 'Delivery', description: 'Alerts, exec, deploy', color: '#00ff88', accent: 'rgba(0, 255, 136, 0.16)', nodes: ['alerts', 'widgets', 'exec', 'deploy-auto', 'observability'] },
  ],
};

const presets = [
  { id: 'mindmap', label: 'Knowledge Mind Map', accent: '#00f0ff', nodes: mindmapNodes },
  { id: 'automation', label: 'n8n Automation', accent: '#ff00ff', nodes: automationNodes },
];

const widgetCards = [
  { title: 'Vector Clusters', value: '12 live', detail: 'Embedding + relationship map', color: '#00f0ff', icon: '🧠' },
  { title: 'Automation Pipelines', value: '7 active', detail: 'Drive-through MCP lanes', color: '#ff00ff', icon: '🛠️' },
  { title: 'Security Gates', value: '3 enforced', detail: 'Static analysis + approvals', color: '#ff6600', icon: '🛡️' },
  { title: 'Telemetry', value: 'Realtime', detail: 'Latency 38ms | Uptime 99.99%', color: '#00ff88', icon: '📡' },
];

const controlPresets = [
  { id: 'cinematic', label: 'Cinematic Grid', description: 'Tron lattice with neon flow' },
  { id: 'nebula', label: 'Nebula Glow', description: 'Gradient bloom + halos' },
  { id: 'vector', label: 'Vector Fog', description: 'Clusters framed for n8n' },
];

// Optimize edge generation - use for loop instead of forEach for better performance
const generateEdges = (nodes: Node[]): Edge[] => {
  const generatedEdges: Edge[] = [];
  for (let nodeIndex = 0; nodeIndex < nodes.length; nodeIndex++) {
    const currentNode = nodes[nodeIndex];
    const nodeConnections = currentNode.connections;
    for (let connectionIndex = 0; connectionIndex < nodeConnections.length; connectionIndex++) {
      const targetNodeId = nodeConnections[connectionIndex];
      generatedEdges.push({
        id: `${currentNode.id}-${targetNodeId}`,
        from: currentNode.id,
        to: targetNodeId,
        animated: currentNode.status === 'active',
      });
    }
  }
  return generatedEdges;
};

// Node Component
const NodeComponent = ({ 
  node,
  isSelected,
  isPathFocus,
  onClick,
  onDragStart,
  onDrag,
  onDragEnd
}: {
  node: Node;
  isSelected: boolean;
  isPathFocus: boolean;
  onClick: () => void;
  onDragStart: (e: React.MouseEvent) => void;
  onDrag: (e: React.MouseEvent) => void;
  onDragEnd: () => void;
}) => {
  const statusColors = {
    completed: '#00ff88',
    active: '#ffcc00',
    pending: '#ff6600',
  };

  return (
    <g
      transform={`translate(${node.position.x}, ${node.position.y})`}
      onClick={onClick}
      onMouseDown={onDragStart}
      style={{ cursor: 'grab' }}
    >
      {/* Glow effect */}
      <defs>
        <filter id={`glow-${node.id}`}>
          <feGaussianBlur stdDeviation="3" result="coloredBlur"/>
          <feMerge>
            <feMergeNode in="coloredBlur"/>
            <feMergeNode in="SourceGraphic"/>
          </feMerge>
        </filter>
        <linearGradient id={`grad-${node.id}`} x1="0%" y1="0%" x2="100%" y2="100%">
          <stop offset="0%" stopColor={node.color} stopOpacity="0.8"/>
          <stop offset="100%" stopColor="#0a0a1a" stopOpacity="0.9"/>
        </linearGradient>
      </defs>

      {/* Selection ring */}
      {isSelected && (
        <rect
          x="-75"
          y="-35"
          width="150"
          height="70"
          rx="12"
          fill="none"
          stroke={node.color}
          strokeWidth="3"
          strokeDasharray="5,5"
          filter={`url(#glow-${node.id})`}
        >
          <animate attributeName="stroke-dashoffset" from="0" to="20" dur="1s" repeatCount="indefinite"/>
        </rect>
      )}

      {!isSelected && isPathFocus && (
        <rect
          x="-70"
          y="-32"
          width="140"
          height="64"
          rx="10"
          fill="none"
          stroke={node.color}
          strokeWidth="2"
          opacity="0.6"
          strokeDasharray="3,6"
        />
      )}

      {/* Node background */}
      <rect
        x="-65"
        y="-28"
        width="130"
        height="56"
        rx="8"
        fill={`url(#grad-${node.id})`}
        stroke={node.color}
        strokeWidth={isSelected || isPathFocus ? 2 : 1.5}
        filter={isSelected ? `url(#glow-${node.id})` : undefined}
      />
      
      {/* Status indicator */}
      <circle
        cx="55"
        cy="-18"
        r="6"
        fill={statusColors[node.status || 'pending']}
      >
        {node.status === 'active' && (
          <animate attributeName="opacity" values="1;0.5;1" dur="1.5s" repeatCount="indefinite"/>
        )}
      </circle>
      
      {/* Icon */}
      <text x="-50" y="5" fontSize="20" textAnchor="middle">{node.icon}</text>
      
      {/* Label */}
      <text x="5" y="-5" fontSize="11" fill="#ffffff" fontWeight="bold" textAnchor="middle">{node.label}</text>
      
      {/* Type badge */}
      <text x="5" y="12" fontSize="8" fill={node.color} textAnchor="middle" opacity="0.8">{node.type.toUpperCase()}</text>

      {isPathFocus && (
        <text x="55" y="18" fontSize="8" fill="#00ff88" textAnchor="end" opacity="0.85">PATH</text>
      )}
    </g>
  );
};

// Edge Component with animated flow
const EdgeComponent = ({ edge, nodes, isHighlighted }: { edge: Edge; nodes: Node[]; isHighlighted: boolean }) => {
  const fromNode = nodes.find(n => n.id === edge.from);
  const toNode = nodes.find(n => n.id === edge.to);

  if (!fromNode || !toNode) return null;
  
  // Calculate path
  const startX = fromNode.position.x;
  const startY = fromNode.position.y + 28;
  const endX = toNode.position.x;
  const endY = toNode.position.y - 28;
  
  // Bezier curve control points
  const midY = (startY + endY) / 2;
  const path = `M ${startX} ${startY} C ${startX} ${midY}, ${endX} ${midY}, ${endX} ${endY}`;
  
  return (
    <g>
      {/* Edge path */}
      <path
        d={path}
        fill="none"
        stroke={fromNode.color}
        strokeWidth={isHighlighted ? 3 : 2}
        opacity={isHighlighted ? 0.9 : 0.6}
        markerEnd="url(#arrowhead)"
      />
      
      {/* Animated particles */}
      {edge.animated && (
        <circle r="3" fill={fromNode.color}>
          <animateMotion dur="2s" repeatCount="indefinite" path={path}/>
        </circle>
      )}
    </g>
  );
};

// Main Component
export default function NodeGraphEditor() {
  const [nodes, setNodes] = useState<Node[]>(() => mindmapNodes.map(node => ({ ...node })));
  const [selectedNode, setSelectedNode] = useState<Node | null>(null);
  const [viewMode, setViewMode] = useState<'graph' | 'list'>('graph');
  const [activePresetId, setActivePresetId] = useState<'mindmap' | 'automation'>('mindmap');
  const [activeCluster, setActiveCluster] = useState<string>('all');
  const [backgroundStyle, setBackgroundStyle] = useState<'cinematic' | 'nebula' | 'vector'>('cinematic');
  const [isDragging, setIsDragging] = useState(false);
  const [dragOffset, setDragOffset] = useState({ x: 0, y: 0 });
  const [zoom, setZoom] = useState(1);
  const [pan, setPan] = useState({ x: 0, y: 0 });
  const [pathStart, setPathStart] = useState<string>(mindmapNodes[0]?.id ?? '');
  const [pathTarget, setPathTarget] = useState<string>(mindmapNodes[1]?.id ?? '');

  const activePreset = presets.find(preset => preset.id === activePresetId) || presets[0];
  const clusters = clustersByPreset[activePresetId];
  const displayNodes = useMemo(() => filterNodesByCluster(nodes, activeCluster), [nodes, activeCluster]);
  const edges = useMemo(() => generateEdges(displayNodes), [displayNodes]);
  const visibleNodeIds = useMemo(() => new Set(displayNodes.map(node => node.id)), [displayNodes]);
  const pathEdges = useMemo(() => displayNodes.flatMap(node =>
    node.connections
      .filter(targetId => visibleNodeIds.has(targetId))
      .map(targetId => ({ from: node.id, to: targetId }))
  ), [displayNodes, visibleNodeIds]);
  const pathSequence = useMemo(
    () => computeShortestPath(pathEdges, pathStart, pathTarget),
    [pathEdges, pathStart, pathTarget]
  );
  const pathNodeIds = useMemo(() => new Set(pathSequence), [pathSequence]);
  const pathEdgeIds = useMemo(() => new Set(pathSequence
    .map((nodeId, index) => pathSequence[index + 1] ? `${nodeId}-${pathSequence[index + 1]}` : null)
    .filter(Boolean) as string[]
  ), [pathSequence]);

  // Statistics
  const stats = useMemo(() => ({
    total: nodes.length,
    completed: nodes.filter(n => n.status === 'completed').length,
    active: nodes.filter(n => n.status === 'active').length,
    pending: nodes.filter(n => n.status === 'pending').length,
    visible: displayNodes.length,
  }), [nodes, displayNodes]);

  const backgroundGradient = useMemo(() => {
    if (backgroundStyle === 'nebula') {
      return 'radial-gradient(circle at 20% 20%, rgba(0, 240, 255, 0.18), transparent 32%), radial-gradient(circle at 80% 30%, rgba(255, 0, 255, 0.16), transparent 34%), radial-gradient(circle at 50% 70%, rgba(0, 255, 136, 0.12), transparent 28%), #0a0a1a'
    }
    if (backgroundStyle === 'vector') {
      return 'linear-gradient(135deg, #050710 0%, #0c1124 50%, #050710 100%)'
    }
    return 'linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0a0a1a 100%)'
  }, [backgroundStyle]);

  useEffect(() => {
    if (displayNodes.length === 0) return;
    if (!displayNodes.some(node => node.id === pathStart)) {
      setPathStart(displayNodes[0].id);
    }
    if (!displayNodes.some(node => node.id === pathTarget)) {
      const fallbackTarget = displayNodes[1]?.id ?? displayNodes[0].id;
      setPathTarget(fallbackTarget);
    }
  }, [displayNodes, pathStart, pathTarget]);

  // Optimize cluster bounds - cache computation with proper dependencies
  const clusterBounds = useMemo(() => {
    const boundsPadding = backgroundStyle === 'vector' ? 64 : 48;
    return clusters
      .map(cluster => {
        const clusterBounds = computeClusterBounds(nodes, cluster.id, boundsPadding);
        return clusterBounds ? { cluster, bounds: clusterBounds } : null;
      })
      .filter((item): item is { cluster: Cluster; bounds: ClusterBounds } => item !== null);
  }, [clusters, nodes, backgroundStyle]);

  const handlePresetChange = useCallback((presetId: 'mindmap' | 'automation') => {
    const preset = presets.find(item => item.id === presetId);
    if (!preset) return;
    setNodes(preset.nodes.map(node => ({ ...node })));
    setActivePresetId(presetId);
    setSelectedNode(null);
    setActiveCluster('all');
    setPathStart(preset.nodes[0]?.id ?? '');
    setPathTarget(preset.nodes[1]?.id ?? preset.nodes[0]?.id ?? '');
  }, []);

  const handleNodeClick = useCallback((node: Node) => {
    setSelectedNode(selectedNode?.id === node.id ? null : node);
  }, [selectedNode]);
  
  const handleDragStart = useCallback((node: Node, e: React.MouseEvent) => {
    if (e.button !== 0) return;
    setIsDragging(true);
    setSelectedNode(node);
    setDragOffset({
      x: e.clientX - node.position.x,
      y: e.clientY - node.position.y,
    });
  }, []);
  
  const handleDrag = useCallback((e: React.MouseEvent) => {
    if (!isDragging || !selectedNode) return;
    
    setNodes(prev => prev.map(n => 
      n.id === selectedNode.id 
        ? { ...n, position: { x: e.clientX - dragOffset.x, y: e.clientY - dragOffset.y } }
        : n
    ));
  }, [isDragging, selectedNode, dragOffset]);
  
  const handleDragEnd = useCallback(() => {
    setIsDragging(false);
  }, []);
  
  const handleZoom = useCallback((delta: number) => {
    setZoom(prev => Math.min(Math.max(prev + delta, 0.5), 2));
  }, []);
  
  const handleStatusChange = useCallback((nodeId: string, status: Node['status']) => {
    setNodes(prev => prev.map(n => 
      n.id === nodeId ? { ...n, status } : n
    ));
  }, []);

  return (
    <div style={{
      minHeight: '100vh',
      background: backgroundGradient,
      color: '#ffffff',
      fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
    }}>
      {/* Header */}
      <header style={{
        padding: '20px 32px',
        borderBottom: '1px solid rgba(0, 240, 255, 0.3)',
        display: 'flex',
        justifyContent: 'space-between',
        gap: '20px',
        alignItems: 'center',
        background: 'rgba(10, 10, 26, 0.9)',
        backdropFilter: 'blur(10px)',
      }}>
        <div style={{ display: 'flex', flexDirection: 'column', gap: '6px' }}>
          <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
            <h1 style={{
              margin: 0,
              fontSize: '28px',
              background: 'linear-gradient(90deg, #00f0ff, #ff00ff, #00ff88)',
              WebkitBackgroundClip: 'text',
              WebkitTextFillColor: 'transparent',
            }}>
              ⚡ The Gitfather Flow Grid
            </h1>
            <span style={{
              padding: '6px 10px',
              border: '1px solid rgba(255,255,255,0.15)',
              borderRadius: '999px',
              fontSize: '12px',
              color: '#9ae6ff',
              background: 'rgba(0, 240, 255, 0.08)'
            }}>
              {activePreset.label} · {clusters.length} clusters
            </span>
          </div>
          <p style={{ margin: 0, opacity: 0.78, fontSize: '14px' }}>
            n8n-inspired automation canvas with vector cluster halos, Tron gradients, and MCP drive-through prompts.
          </p>
          <div style={{ display: 'flex', gap: '10px', flexWrap: 'wrap' }}>
            {controlPresets.map(control => (
              <button
                key={control.id}
                onClick={() => setBackgroundStyle(control.id as typeof backgroundStyle)}
                style={{
                  padding: '8px 12px',
                  borderRadius: '999px',
                  border: backgroundStyle === control.id ? '1px solid #ff00ff' : '1px solid rgba(255,255,255,0.15)',
                  background: backgroundStyle === control.id ? 'rgba(255,0,255,0.18)' : 'rgba(255,255,255,0.05)',
                  color: '#ffffff',
                  cursor: 'pointer',
                  fontSize: '12px',
                }}
              >
                {control.label}
              </button>
            ))}
          </div>
        </div>

        <div style={{ display: 'flex', flexDirection: 'column', gap: '10px', alignItems: 'flex-end' }}>
          <div style={{ display: 'flex', gap: '8px', flexWrap: 'wrap', justifyContent: 'flex-end' }}>
            {presets.map(preset => (
              <button
                key={preset.id}
                onClick={() => handlePresetChange(preset.id as 'mindmap' | 'automation')}
                style={{
                  padding: '10px 14px',
                  background: activePresetId === preset.id ? 'rgba(0, 240, 255, 0.15)' : 'rgba(255, 255, 255, 0.04)',
                  border: `1px solid ${activePresetId === preset.id ? preset.accent : 'rgba(255,255,255,0.15)'}`,
                  borderRadius: '10px',
                  color: '#ffffff',
                  cursor: 'pointer',
                  fontSize: '13px',
                }}
              >
                {preset.id === 'automation' ? '🤖 Automation' : '🧭 Mind Map'}
              </button>
            ))}
          </div>
          <div style={{ display: 'flex', gap: '10px', alignItems: 'center' }}>
            <a
              href="/DevTeam6/"
              style={{
                padding: '10px 20px',
                background: 'rgba(0, 240, 255, 0.2)',
                border: '1px solid #00f0ff',
                borderRadius: '8px',
                color: '#00f0ff',
                textDecoration: 'none',
                fontSize: '14px',
              }}
            >
              🏠 Home
            </a>

            <button
              onClick={() => setViewMode(viewMode === 'graph' ? 'list' : 'graph')}
              style={{
                padding: '10px 20px',
                background: viewMode === 'graph' ? 'rgba(255, 0, 255, 0.2)' : 'rgba(0, 255, 136, 0.2)',
                border: `1px solid ${viewMode === 'graph' ? '#ff00ff' : '#00ff88'}`,
                borderRadius: '8px',
                color: viewMode === 'graph' ? '#ff00ff' : '#00ff88',
                cursor: 'pointer',
                fontSize: '14px',
              }}
            >
              {viewMode === 'graph' ? '📋 List View' : '🔀 Graph View'}
            </button>
          </div>
        </div>
      </header>
      
      {/* Stats Bar */}
      <div style={{
        display: 'flex',
        justifyContent: 'center',
        gap: '30px',
        padding: '16px 20px',
        background: 'linear-gradient(90deg, rgba(0,240,255,0.08), rgba(255,0,255,0.05))',
        borderBottom: '1px solid rgba(0, 240, 255, 0.2)',
      }}>
        <div style={{ textAlign: 'center' }}>
          <div style={{ fontSize: '24px', fontWeight: 'bold', color: '#00f0ff' }}>{stats.total}</div>
          <div style={{ fontSize: '12px', opacity: 0.7 }}>Total Nodes</div>
        </div>
        <div style={{ textAlign: 'center' }}>
          <div style={{ fontSize: '24px', fontWeight: 'bold', color: '#00ff88' }}>{stats.completed}</div>
          <div style={{ fontSize: '12px', opacity: 0.7 }}>Completed</div>
        </div>
        <div style={{ textAlign: 'center' }}>
          <div style={{ fontSize: '24px', fontWeight: 'bold', color: '#ffcc00' }}>{stats.active}</div>
          <div style={{ fontSize: '12px', opacity: 0.7 }}>Active</div>
        </div>
        <div style={{ textAlign: 'center' }}>
          <div style={{ fontSize: '24px', fontWeight: 'bold', color: '#ff6600' }}>{stats.pending}</div>
          <div style={{ fontSize: '12px', opacity: 0.7 }}>Pending</div>
        </div>
        <div style={{ textAlign: 'center' }}>
          <div style={{ fontSize: '24px', fontWeight: 'bold', color: '#7b2fff' }}>{stats.visible}</div>
          <div style={{ fontSize: '12px', opacity: 0.7 }}>Visible in View</div>
        </div>
      </div>
      
      <div style={{ display: 'flex', height: 'calc(100vh - 150px)' }}>
        {/* Main Canvas */}
        <div style={{ flex: 1, position: 'relative', overflow: 'hidden' }}>
          {viewMode === 'graph' ? (
            <>
              {/* Cluster Filters */}
              <div style={{
                position: 'absolute',
                top: '20px',
                left: '20px',
                display: 'flex',
                gap: '8px',
                flexWrap: 'wrap',
                zIndex: 10,
              }}>
                <button
                  onClick={() => setActiveCluster('all')}
                  style={{
                    padding: '8px 12px',
                    borderRadius: '10px',
                    border: activeCluster === 'all' ? '1px solid #00f0ff' : '1px solid rgba(255,255,255,0.2)',
                    background: activeCluster === 'all' ? 'rgba(0, 240, 255, 0.2)' : 'rgba(255,255,255,0.05)',
                    color: '#ffffff',
                    cursor: 'pointer',
                    fontSize: '12px',
                  }}
                >
                  🌐 All clusters
                </button>
                {clusters.map(cluster => (
                  <button
                    key={cluster.id}
                    onClick={() => setActiveCluster(cluster.id === activeCluster ? 'all' : cluster.id)}
                    style={{
                      padding: '8px 12px',
                      borderRadius: '10px',
                      border: `1px solid ${cluster.color}`,
                      background: activeCluster === cluster.id ? `${cluster.accent}` : 'rgba(255,255,255,0.05)',
                      color: '#ffffff',
                      cursor: 'pointer',
                      fontSize: '12px',
                    }}
                  >
                    {cluster.label}
                  </button>
                ))}
              </div>

              {/* Zoom Controls */}
              <div style={{
                position: 'absolute',
                top: '20px',
                right: '20px',
                display: 'flex',
                flexDirection: 'column',
                gap: '5px',
                zIndex: 10,
              }}>
                <button
                  onClick={() => handleZoom(0.1)}
                  style={{
                    width: '40px',
                    height: '40px',
                    background: 'rgba(0, 240, 255, 0.2)',
                    border: '1px solid #00f0ff',
                    borderRadius: '8px',
                    color: '#00f0ff',
                    cursor: 'pointer',
                    fontSize: '20px',
                  }}
                >+</button>
                <div style={{
                  textAlign: 'center',
                  padding: '5px',
                  background: 'rgba(10, 10, 26, 0.8)',
                  borderRadius: '4px',
                  fontSize: '12px',
                }}>{Math.round(zoom * 100)}%</div>
                <button
                  onClick={() => handleZoom(-0.1)}
                  style={{
                    width: '40px',
                    height: '40px',
                    background: 'rgba(0, 240, 255, 0.2)',
                    border: '1px solid #00f0ff',
                    borderRadius: '8px',
                    color: '#00f0ff',
                    cursor: 'pointer',
                    fontSize: '20px',
                  }}
                >−</button>
                <button
                  onClick={() => { setZoom(1); setPan({ x: 0, y: 0 }); }}
                  style={{
                    width: '40px',
                    height: '40px',
                    background: 'rgba(255, 0, 255, 0.2)',
                    border: '1px solid #ff00ff',
                    borderRadius: '8px',
                    color: '#ff00ff',
                    cursor: 'pointer',
                    fontSize: '14px',
                  }}
                >⟲</button>
              </div>
              
              {/* SVG Canvas */}
              <svg
                width="100%"
                height="100%"
                style={{
                  background: 'radial-gradient(circle at center, rgba(0, 240, 255, 0.08) 0%, transparent 70%)',
                }}
                onMouseMove={handleDrag}
                onMouseUp={handleDragEnd}
                onMouseLeave={handleDragEnd}
              >
                {/* Grid pattern */}
                <defs>
                  <pattern id="grid" width="50" height="50" patternUnits="userSpaceOnUse">
                    <path d="M 50 0 L 0 0 0 50" fill="none" stroke={backgroundStyle === 'cinematic' ? 'rgba(0, 240, 255, 0.12)' : 'rgba(255, 0, 255, 0.08)'} strokeWidth="0.6"/>
                  </pattern>
                  <marker
                    id="arrowhead"
                    markerWidth="10"
                    markerHeight="7"
                    refX="9"
                    refY="3.5"
                    orient="auto"
                  >
                    <polygon points="0 0, 10 3.5, 0 7" fill="#00f0ff" opacity="0.6"/>
                  </marker>
                </defs>
                <rect width="100%" height="100%" fill="url(#grid)"/>

                <g transform={`translate(${pan.x}, ${pan.y}) scale(${zoom})`}>
                  {/* Cluster halos */}
                  {clusterBounds.map(item => item.bounds && (
                    <g key={item.cluster.id}>
                      <rect
                        x={item.bounds.minX}
                        y={item.bounds.minY}
                        width={item.bounds.width}
                        height={item.bounds.height}
                        rx="18"
                        fill={item.cluster.accent}
                        stroke={activeCluster === item.cluster.id ? '#ffffff' : item.cluster.color}
                        strokeWidth={activeCluster === item.cluster.id ? 2.4 : 1.5}
                        opacity={activeCluster === 'all' || activeCluster === item.cluster.id ? 0.8 : 0.35}
                      />
                      <text
                        x={item.bounds.centerX}
                        y={item.bounds.minY + 18}
                        fontSize="12"
                        textAnchor="middle"
                        fill="#ffffff"
                        opacity="0.9"
                      >
                        {item.cluster.label}
                      </text>
                    </g>
                  ))}

                  {/* Edges */}
                  {edges.map(edge => (
                    <EdgeComponent
                      key={edge.id}
                      edge={edge}
                      nodes={displayNodes}
                      isHighlighted={pathEdgeIds.has(edge.id)}
                    />
                  ))}

                  {/* Nodes */}
                  {displayNodes.map(node => (
                    <NodeComponent
                      key={node.id}
                      node={node}
                      isSelected={selectedNode?.id === node.id}
                      isPathFocus={pathNodeIds.has(node.id)}
                      onClick={() => handleNodeClick(node)}
                      onDragStart={(e) => handleDragStart(node, e)}
                      onDrag={handleDrag}
                      onDragEnd={handleDragEnd}
                    />
                  ))}
                </g>
              </svg>
            </>
          ) : (
            /* List View */
            <div style={{ padding: '20px', overflow: 'auto', height: '100%' }}>
              <table style={{ width: '100%', borderCollapse: 'collapse' }}>
                <thead>
                  <tr style={{ borderBottom: '2px solid #00f0ff' }}>
                    <th style={{ padding: '10px', textAlign: 'left', color: '#00f0ff' }}>Node</th>
                    <th style={{ padding: '10px', textAlign: 'left', color: '#ff00ff' }}>Type</th>
                    <th style={{ padding: '10px', textAlign: 'left', color: '#00ff88' }}>Description</th>
                    <th style={{ padding: '10px', textAlign: 'left', color: '#7b2fff' }}>Cluster</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: '#ffcc00' }}>Status</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: '#ff6600' }}>Connections</th>
                  </tr>
                </thead>
                <tbody>
                  {displayNodes.map(node => (
                    <tr
                      key={node.id}
                      onClick={() => handleNodeClick(node)}
                      style={{
                        borderBottom: '1px solid rgba(255,255,255,0.1)',
                        cursor: 'pointer',
                        background: selectedNode?.id === node.id ? 'rgba(0, 240, 255, 0.1)' : 'transparent',
                      }}
                    >
                      <td style={{ padding: '12px' }}>
                        <span style={{ marginRight: '8px' }}>{node.icon}</span>
                        <span style={{ color: node.color, fontWeight: 'bold' }}>{node.label}</span>
                      </td>
                      <td style={{ padding: '12px', opacity: 0.7 }}>{node.type}</td>
                      <td style={{ padding: '12px', opacity: 0.7, fontSize: '13px' }}>{node.description}</td>
                      <td style={{ padding: '12px', color: '#9ae6ff', fontSize: '12px' }}>{node.cluster || '—'}</td>
                      <td style={{ padding: '12px', textAlign: 'center' }}>
                        <select
                          value={node.status}
                          onChange={(e) => handleStatusChange(node.id, e.target.value as Node['status'])}
                          onClick={(e) => e.stopPropagation()}
                          style={{
                            background: 'rgba(10, 10, 26, 0.8)',
                            border: `1px solid ${node.status === 'completed' ? '#00ff88' : node.status === 'active' ? '#ffcc00' : '#ff6600'}`,
                            borderRadius: '4px',
                            color: node.status === 'completed' ? '#00ff88' : node.status === 'active' ? '#ffcc00' : '#ff6600',
                            padding: '5px 10px',
                            cursor: 'pointer',
                          }}
                        >
                          <option value="completed">✓ Completed</option>
                          <option value="active">● Active</option>
                          <option value="pending">○ Pending</option>
                        </select>
                      </td>
                      <td style={{ padding: '12px', textAlign: 'center' }}>
                        {node.connections.length > 0 ? (
                          <span style={{ 
                            background: 'rgba(0, 240, 255, 0.2)',
                            padding: '3px 8px',
                            borderRadius: '10px',
                            fontSize: '12px',
                          }}>
                            {node.connections.length} →
                          </span>
                        ) : '—'}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          )}
        </div>
        
        {/* Side Panel - Node Details + Widgets */}
        <div style={{
          width: '380px',
          background: 'rgba(10, 10, 26, 0.95)',
          borderLeft: '1px solid rgba(0, 240, 255, 0.25)',
          padding: '20px',
          overflow: 'auto',
          display: 'flex',
          flexDirection: 'column',
          gap: '16px',
        }}>
          <div style={{
            border: '1px solid rgba(0, 240, 255, 0.35)',
            background: 'linear-gradient(135deg, rgba(0,240,255,0.08), rgba(255,0,255,0.06))',
            borderRadius: '12px',
            padding: '12px',
          }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '10px' }}>
              <div>
                <div style={{ fontSize: '12px', letterSpacing: '0.08em', color: '#9ae6ff' }}>PATH PLAYBOOK</div>
                <div style={{ fontSize: '14px', fontWeight: 'bold', color: '#ffffff' }}>Shortest Tron route between visible nodes</div>
              </div>
              <div style={{
                padding: '6px 10px',
                borderRadius: '10px',
                background: pathSequence.length > 0 ? 'rgba(0,255,136,0.18)' : 'rgba(255,102,0,0.18)',
                color: pathSequence.length > 0 ? '#00ff88' : '#ff9966',
                fontSize: '12px',
                border: '1px solid rgba(255,255,255,0.12)'
              }}>
                {pathSequence.length > 0 ? `${pathSequence.length - 1} hops` : 'No route'}
              </div>
            </div>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr auto', gap: '8px', alignItems: 'center' }}>
              <select
                aria-label="Path start node"
                value={pathStart}
                onChange={(e) => setPathStart(e.target.value)}
                style={{
                  background: 'rgba(0,0,0,0.35)',
                  border: '1px solid rgba(0,255,255,0.4)',
                  color: '#e8f9ff',
                  borderRadius: '8px',
                  padding: '8px',
                  fontSize: '12px',
                }}
              >
                {displayNodes.length === 0 ? <option value="">No nodes</option> : displayNodes.map(node => (
                  <option key={node.id} value={node.id}>{node.icon} {node.label}</option>
                ))}
              </select>

              <select
                aria-label="Path target node"
                value={pathTarget}
                onChange={(e) => setPathTarget(e.target.value)}
                style={{
                  background: 'rgba(0,0,0,0.35)',
                  border: '1px solid rgba(255,0,255,0.4)',
                  color: '#e8f9ff',
                  borderRadius: '8px',
                  padding: '8px',
                  fontSize: '12px',
                }}
              >
                {displayNodes.length === 0 ? <option value="">No nodes</option> : displayNodes.map(node => (
                  <option key={node.id} value={node.id}>{node.icon} {node.label}</option>
                ))}
              </select>

              <button
                aria-label="Swap path start and target"
                onClick={() => { setPathStart(pathTarget); setPathTarget(pathStart); }}
                style={{
                  padding: '8px 10px',
                  background: 'rgba(0, 240, 255, 0.15)',
                  border: '1px solid rgba(0, 240, 255, 0.5)',
                  color: '#00f0ff',
                  borderRadius: '8px',
                  cursor: 'pointer',
                  fontSize: '12px',
                  fontWeight: 'bold',
                }}
              >⇄</button>
            </div>

            <div style={{ marginTop: '10px', display: 'flex', flexWrap: 'wrap', gap: '6px' }}>
              {pathSequence.length > 0 ? (
                pathSequence.map((nodeId, idx) => {
                  const node = displayNodes.find(item => item.id === nodeId);
                  return node ? (
                    <div key={nodeId} style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
                      <span style={{
                        padding: '6px 10px',
                        borderRadius: '10px',
                        background: `${node.color}22`,
                        border: `1px solid ${node.color}77`,
                        color: node.color,
                        fontSize: '12px',
                      }}>
                        {node.icon} {node.label}
                      </span>
                      {idx < pathSequence.length - 1 && <span style={{ color: '#9ae6ff' }}>→</span>}
                    </div>
                  ) : null;
                })
              ) : (
                <div style={{ color: '#ffae7a', fontSize: '12px' }}>
                  Pick two visible nodes to trace a neon-fast route.
                </div>
              )}
            </div>
          </div>

          <div>
            <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '8px' }}>MCP WIDGETS</div>
            <div style={{ display: 'grid', gridTemplateColumns: 'repeat(2, minmax(0, 1fr))', gap: '10px' }}>
              {widgetCards.map(card => (
                <div
                  key={card.title}
                  style={{
                    border: `1px solid ${card.color}55`,
                    background: `${card.color}10`,
                    borderRadius: '10px',
                    padding: '10px',
                  }}
                >
                  <div style={{ display: 'flex', alignItems: 'center', gap: '8px', marginBottom: '6px' }}>
                    <span>{card.icon}</span>
                    <span style={{ fontWeight: 'bold' }}>{card.title}</span>
                  </div>
                  <div style={{ color: card.color, fontSize: '14px', fontWeight: 'bold' }}>{card.value}</div>
                  <div style={{ fontSize: '12px', opacity: 0.8 }}>{card.detail}</div>
                </div>
              ))}
            </div>
          </div>

          {selectedNode ? (
            <>
              <div style={{
                display: 'flex',
                justifyContent: 'space-between',
                alignItems: 'center',
              }}>
                <h2 style={{
                  margin: 0,
                  color: selectedNode.color,
                  display: 'flex',
                  alignItems: 'center',
                  gap: '10px',
                }}>
                  <span style={{ fontSize: '30px' }}>{selectedNode.icon}</span>
                  {selectedNode.label}
                </h2>
                <button
                  onClick={() => setSelectedNode(null)}
                  style={{
                    background: 'transparent',
                    border: 'none',
                    color: '#ffffff',
                    fontSize: '24px',
                    cursor: 'pointer',
                    opacity: 0.7,
                  }}
                >×</button>
              </div>

              <div style={{
                background: `linear-gradient(135deg, ${selectedNode.color}20, transparent)`,
                padding: '15px',
                borderRadius: '8px',
              }}>
                <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '5px' }}>DESCRIPTION</div>
                <div style={{ fontSize: '14px' }}>{selectedNode.description}</div>
              </div>

              <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '10px' }}>
                <div style={{
                  background: 'rgba(0, 240, 255, 0.1)',
                  padding: '12px',
                  borderRadius: '8px',
                  textAlign: 'center',
                }}>
                  <div style={{ fontSize: '12px', opacity: 0.7 }}>TYPE</div>
                  <div style={{ fontSize: '14px', fontWeight: 'bold', color: selectedNode.color }}>
                    {selectedNode.type.toUpperCase()}
                  </div>
                </div>
                <div style={{
                  background: 'rgba(255, 0, 255, 0.1)',
                  padding: '12px',
                  borderRadius: '8px',
                  textAlign: 'center',
                }}>
                  <div style={{ fontSize: '12px', opacity: 0.7 }}>STATUS</div>
                  <div style={{
                    fontSize: '14px',
                    fontWeight: 'bold',
                    color: selectedNode.status === 'completed' ? '#00ff88' : selectedNode.status === 'active' ? '#ffcc00' : '#ff6600',
                  }}>
                    {selectedNode.status?.toUpperCase()}
                  </div>
                </div>
              </div>

              <div style={{
                background: 'rgba(0, 255, 136, 0.1)',
                padding: '12px',
                borderRadius: '8px',
              }}>
                <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '8px' }}>POSITION</div>
                <div style={{ display: 'flex', gap: '15px' }}>
                  <div>
                    <span style={{ opacity: 0.7 }}>X:</span>
                    <span style={{ color: '#00f0ff', marginLeft: '5px' }}>{Math.round(selectedNode.position.x)}</span>
                  </div>
                  <div>
                    <span style={{ opacity: 0.7 }}>Y:</span>
                    <span style={{ color: '#ff00ff', marginLeft: '5px' }}>{Math.round(selectedNode.position.y)}</span>
                  </div>
                </div>
              </div>

              {selectedNode.connections.length > 0 && (
                <div style={{
                  background: 'rgba(255, 102, 0, 0.1)',
                  padding: '12px',
                  borderRadius: '8px',
                }}>
                  <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '8px' }}>CONNECTIONS ({selectedNode.connections.length})</div>
                  <div style={{ display: 'flex', flexWrap: 'wrap', gap: '8px' }}>
                    {selectedNode.connections.map(connId => {
                      const connNode = nodes.find(n => n.id === connId);
                      return connNode ? (
                        <button
                          key={connId}
                          onClick={() => setSelectedNode(connNode)}
                          style={{
                            background: `${connNode.color}30`,
                            border: `1px solid ${connNode.color}`,
                            borderRadius: '15px',
                            padding: '5px 12px',
                            color: connNode.color,
                            cursor: 'pointer',
                            fontSize: '12px',
                          }}
                        >
                          {connNode.icon} {connNode.label}
                        </button>
                      ) : null;
                    })}
                  </div>
                </div>
              )}

              <div style={{ marginTop: '10px', padding: '15px', background: 'rgba(123, 47, 255, 0.12)', borderRadius: '8px' }}>
                <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '10px' }}>CHANGE STATUS</div>
                <div style={{ display: 'flex', gap: '8px' }}>
                  {(['completed', 'active', 'pending'] as const).map(status => (
                    <button
                      key={status}
                      onClick={() => handleStatusChange(selectedNode.id, status)}
                      style={{
                        flex: 1,
                        padding: '8px',
                        background: selectedNode.status === status
                          ? (status === 'completed' ? '#00ff88' : status === 'active' ? '#ffcc00' : '#ff6600')
                          : 'transparent',
                        border: `1px solid ${status === 'completed' ? '#00ff88' : status === 'active' ? '#ffcc00' : '#ff6600'}`,
                        borderRadius: '6px',
                        color: selectedNode.status === status ? '#0a0a1a' : (status === 'completed' ? '#00ff88' : status === 'active' ? '#ffcc00' : '#ff6600'),
                        cursor: 'pointer',
                        fontSize: '11px',
                        fontWeight: 'bold',
                      }}
                    >
                      {status === 'completed' ? '✓' : status === 'active' ? '●' : '○'} {status.charAt(0).toUpperCase() + status.slice(1)}
                    </button>
                  ))}
                </div>
              </div>
            </>
          ) : (
            <div style={{
              background: 'rgba(255, 255, 255, 0.04)',
              border: '1px dashed rgba(255,255,255,0.2)',
              borderRadius: '10px',
              padding: '20px',
              textAlign: 'center',
              color: '#9ae6ff',
              fontSize: '13px',
            }}>
              Select a node to view its Tron-grade dossier.
            </div>
          )}
        </div>
      </div>
      
      {/* Legend */}
      <div style={{
        position: 'fixed',
        bottom: '20px',
        left: '20px',
        background: 'rgba(10, 10, 26, 0.95)',
        border: '1px solid rgba(0, 240, 255, 0.3)',
        borderRadius: '12px',
        padding: '15px',
        zIndex: 100,
      }}>
        <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '10px' }}>NODE TYPES</div>
        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(3, 1fr)', gap: '8px', fontSize: '11px' }}>
          {[
            { type: 'start', color: '#00f0ff', icon: '🚀' },
            { type: 'data', color: '#ff00ff', icon: '📄' },
            { type: 'process', color: '#00ff88', icon: '⚙️' },
            { type: 'action', color: '#ff6600', icon: '🎯' },
            { type: 'ai', color: '#7b2fff', icon: '🤖' },
            { type: 'cloud', color: '#ffcc00', icon: '☁️' },
            { type: 'security', color: '#aa00ff', icon: '🔒' },
          ].map(item => (
            <div key={item.type} style={{ display: 'flex', alignItems: 'center', gap: '5px' }}>
              <span style={{ 
                width: '12px', 
                height: '12px', 
                background: item.color, 
                borderRadius: '3px',
                display: 'inline-block',
              }}/>
              <span>{item.icon} {item.type}</span>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
}
