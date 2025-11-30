import { useState, useCallback, useMemo } from 'react';

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
}

interface Edge {
  id: string;
  from: string;
  to: string;
  label?: string;
  animated?: boolean;
}

// DevTeam6 Workflow Nodes - representing project architecture
const initialNodes: Node[] = [
  // Layer 1 - Entry Points
  { id: 'start', type: 'start', label: 'DevTeam6', description: 'Repository Entry Point', position: { x: 400, y: 50 }, color: '#00f0ff', icon: '🚀', connections: ['readme', 'pages'], status: 'completed' },
  
  // Layer 2 - Core Components
  { id: 'readme', type: 'data', label: 'README.md', description: 'Visibility Layer - Project Dashboard', position: { x: 200, y: 150 }, color: '#ff00ff', icon: '📄', connections: ['resources', 'navigation', 'badges'], status: 'completed' },
  { id: 'pages', type: 'cloud', label: 'GitHub Pages', description: 'Interaction Layer - Live Applications', position: { x: 600, y: 150 }, color: '#7b2fff', icon: '🌐', connections: ['3d-demo', 'genui', 'dashboard'], status: 'completed' },
  
  // Layer 3 - README Components
  { id: 'resources', type: 'data', label: 'Resources', description: '1000+ Curated Tools & Links', position: { x: 50, y: 280 }, color: '#00ff88', icon: '📚', connections: ['ai-tools', 'dev-tools', 'cloud-tools'], status: 'completed' },
  { id: 'navigation', type: 'action', label: 'Navigation', description: 'Quick Jump Buttons', position: { x: 200, y: 280 }, color: '#ff6600', icon: '🧭', connections: [], status: 'completed' },
  { id: 'badges', type: 'action', label: 'Badges & Stats', description: 'Dynamic Status Indicators', position: { x: 350, y: 280 }, color: '#ffcc00', icon: '🏅', connections: [], status: 'active' },
  
  // Layer 3 - Pages Components
  { id: '3d-demo', type: 'ai', label: '3D Demo', description: 'Three.js Interactive Experience', position: { x: 500, y: 280 }, color: '#00f0ff', icon: '🎮', connections: [], status: 'completed' },
  { id: 'genui', type: 'ai', label: 'GenUI Playground', description: 'AI Component Generator', position: { x: 650, y: 280 }, color: '#ff00ff', icon: '🎨', connections: [], status: 'completed' },
  { id: 'dashboard', type: 'process', label: 'Dashboard', description: 'Gamification & Stats', position: { x: 800, y: 280 }, color: '#00ff88', icon: '📊', connections: ['leaderboard', 'achievements'], status: 'completed' },
  
  // Layer 4 - Resource Categories
  { id: 'ai-tools', type: 'ai', label: 'AI Tools', description: 'GPT-5.1, Claude Opus 4.5, Gemini 3', position: { x: 50, y: 420 }, color: '#00f0ff', icon: '🤖', connections: [], status: 'completed' },
  { id: 'dev-tools', type: 'process', label: 'Dev Tools', description: 'IDEs, CLIs, Extensions', position: { x: 150, y: 420 }, color: '#ff00ff', icon: '⚙️', connections: [], status: 'completed' },
  { id: 'cloud-tools', type: 'cloud', label: 'Cloud', description: 'AWS, GCP, Azure, Vercel', position: { x: 250, y: 420 }, color: '#7b2fff', icon: '☁️', connections: [], status: 'active' },
  
  // Layer 4 - Dashboard Components  
  { id: 'leaderboard', type: 'data', label: 'Leaderboard', description: 'Top Contributors', position: { x: 700, y: 420 }, color: '#ff6600', icon: '🏆', connections: [], status: 'completed' },
  { id: 'achievements', type: 'action', label: 'Achievements', description: 'Collectible Badges', position: { x: 850, y: 420 }, color: '#ffcc00', icon: '🎖️', connections: [], status: 'active' },
  
  // Layer 5 - Automation
  { id: 'automation', type: 'security', label: 'GitHub Actions', description: 'CI/CD & Sync Pipelines', position: { x: 400, y: 520 }, color: '#aa00ff', icon: '⚡', connections: ['deploy', 'cvs-sync', 'roadmap-sync'], status: 'active' },
  
  // Layer 6 - Workflows
  { id: 'deploy', type: 'cloud', label: 'Deploy', description: 'Pages Deployment', position: { x: 250, y: 620 }, color: '#00ff88', icon: '🚀', connections: [], status: 'completed' },
  { id: 'cvs-sync', type: 'process', label: 'CVS Sync', description: 'Hourly Metric Updates', position: { x: 400, y: 620 }, color: '#00f0ff', icon: '🔄', connections: [], status: 'active' },
  { id: 'roadmap-sync', type: 'action', label: 'Roadmap Sync', description: '6-Hour Progress Updates', position: { x: 550, y: 620 }, color: '#ff00ff', icon: '📋', connections: [], status: 'pending' },
];

// Generate edges from node connections
const generateEdges = (nodes: Node[]): Edge[] => {
  const edges: Edge[] = [];
  nodes.forEach(node => {
    node.connections.forEach(targetId => {
      edges.push({
        id: `${node.id}-${targetId}`,
        from: node.id,
        to: targetId,
        animated: node.status === 'active',
      });
    });
  });
  return edges;
};

// Node Component
const NodeComponent = ({ 
  node, 
  isSelected, 
  onClick,
  onDragStart,
  onDrag,
  onDragEnd
}: { 
  node: Node; 
  isSelected: boolean;
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
      
      {/* Node background */}
      <rect
        x="-65"
        y="-28"
        width="130"
        height="56"
        rx="8"
        fill={`url(#grad-${node.id})`}
        stroke={node.color}
        strokeWidth={isSelected ? 2 : 1.5}
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
    </g>
  );
};

// Edge Component with animated flow
const EdgeComponent = ({ edge, nodes }: { edge: Edge; nodes: Node[] }) => {
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
        strokeWidth="2"
        opacity="0.6"
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
  const [nodes, setNodes] = useState<Node[]>(initialNodes);
  const [selectedNode, setSelectedNode] = useState<Node | null>(null);
  const [viewMode, setViewMode] = useState<'graph' | 'list'>('graph');
  const [isDragging, setIsDragging] = useState(false);
  const [dragOffset, setDragOffset] = useState({ x: 0, y: 0 });
  const [zoom, setZoom] = useState(1);
  const [pan, setPan] = useState({ x: 0, y: 0 });
  
  const edges = useMemo(() => generateEdges(nodes), [nodes]);
  
  // Statistics
  const stats = useMemo(() => ({
    total: nodes.length,
    completed: nodes.filter(n => n.status === 'completed').length,
    active: nodes.filter(n => n.status === 'active').length,
    pending: nodes.filter(n => n.status === 'pending').length,
  }), [nodes]);
  
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
      background: 'linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0a0a1a 100%)',
      color: '#ffffff',
      fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
    }}>
      {/* Header */}
      <header style={{
        padding: '20px 40px',
        borderBottom: '1px solid rgba(0, 240, 255, 0.3)',
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center',
        background: 'rgba(10, 10, 26, 0.9)',
        backdropFilter: 'blur(10px)',
      }}>
        <div>
          <h1 style={{
            margin: 0,
            fontSize: '28px',
            background: 'linear-gradient(90deg, #00f0ff, #ff00ff, #00ff88)',
            WebkitBackgroundClip: 'text',
            WebkitTextFillColor: 'transparent',
          }}>
            ⚡ Node Graph Editor
          </h1>
          <p style={{ margin: '5px 0 0', opacity: 0.7, fontSize: '14px' }}>
            Interactive n8n-style workflow visualization
          </p>
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
      </header>
      
      {/* Stats Bar */}
      <div style={{
        display: 'flex',
        justifyContent: 'center',
        gap: '30px',
        padding: '15px',
        background: 'rgba(0, 240, 255, 0.05)',
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
      </div>
      
      <div style={{ display: 'flex', height: 'calc(100vh - 150px)' }}>
        {/* Main Canvas */}
        <div style={{ flex: 1, position: 'relative', overflow: 'hidden' }}>
          {viewMode === 'graph' ? (
            <>
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
                  background: 'radial-gradient(circle at center, rgba(0, 240, 255, 0.05) 0%, transparent 70%)',
                }}
                onMouseMove={handleDrag}
                onMouseUp={handleDragEnd}
                onMouseLeave={handleDragEnd}
              >
                {/* Grid pattern */}
                <defs>
                  <pattern id="grid" width="50" height="50" patternUnits="userSpaceOnUse">
                    <path d="M 50 0 L 0 0 0 50" fill="none" stroke="rgba(0, 240, 255, 0.1)" strokeWidth="0.5"/>
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
                  {/* Edges */}
                  {edges.map(edge => (
                    <EdgeComponent key={edge.id} edge={edge} nodes={nodes}/>
                  ))}
                  
                  {/* Nodes */}
                  {nodes.map(node => (
                    <NodeComponent
                      key={node.id}
                      node={node}
                      isSelected={selectedNode?.id === node.id}
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
                    <th style={{ padding: '10px', textAlign: 'center', color: '#ffcc00' }}>Status</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: '#ff6600' }}>Connections</th>
                  </tr>
                </thead>
                <tbody>
                  {nodes.map(node => (
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
        
        {/* Side Panel - Node Details */}
        {selectedNode && (
          <div style={{
            width: '350px',
            background: 'rgba(10, 10, 26, 0.95)',
            borderLeft: `2px solid ${selectedNode.color}`,
            padding: '20px',
            overflow: 'auto',
          }}>
            <div style={{
              display: 'flex',
              justifyContent: 'space-between',
              alignItems: 'center',
              marginBottom: '20px',
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
              marginBottom: '15px',
            }}>
              <div style={{ fontSize: '12px', opacity: 0.7, marginBottom: '5px' }}>DESCRIPTION</div>
              <div style={{ fontSize: '14px' }}>{selectedNode.description}</div>
            </div>
            
            <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '10px', marginBottom: '15px' }}>
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
              marginBottom: '15px',
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
            
            <div style={{ marginTop: '20px', padding: '15px', background: 'rgba(123, 47, 255, 0.1)', borderRadius: '8px' }}>
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
          </div>
        )}
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
