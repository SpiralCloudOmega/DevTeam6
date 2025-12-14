import { useState, useMemo } from 'react';
import { Link } from 'react-router-dom';

// Project phases with tasks
const projectPhases = [
  {
    id: 'foundation',
    name: 'Foundation',
    status: 'completed',
    color: '#00f0ff',
    startDate: '2024-01',
    endDate: '2024-03',
    description: 'Core repository structure and initial content',
    tasks: [
      { id: 1, title: 'Repository Setup', status: 'done', priority: 'high' },
      { id: 2, title: 'Initial Resource Curation', status: 'done', priority: 'high' },
      { id: 3, title: 'README Structure', status: 'done', priority: 'medium' },
      { id: 4, title: 'Category Organization', status: 'done', priority: 'medium' },
      { id: 5, title: 'License & Guidelines', status: 'done', priority: 'low' }
    ]
  },
  {
    id: 'expansion',
    name: 'Content Expansion',
    status: 'completed',
    color: '#ff00ff',
    startDate: '2024-04',
    endDate: '2024-06',
    description: 'Expanding coverage across all development domains',
    tasks: [
      { id: 6, title: 'AI/ML Resources (500+)', status: 'done', priority: 'high' },
      { id: 7, title: 'DevOps & Cloud Tools', status: 'done', priority: 'high' },
      { id: 8, title: 'Security Resources', status: 'done', priority: 'medium' },
      { id: 9, title: 'Awesome Lists Integration', status: 'done', priority: 'medium' },
      { id: 10, title: 'Learning Resources', status: 'done', priority: 'medium' }
    ]
  },
  {
    id: 'visual',
    name: 'Visual Enhancement',
    status: 'completed',
    color: '#00ff88',
    startDate: '2024-07',
    endDate: '2024-09',
    description: 'Dark cyberpunk theme and visual improvements',
    tasks: [
      { id: 11, title: 'Cyberpunk Theme Design', status: 'done', priority: 'high' },
      { id: 12, title: 'Mermaid Diagrams', status: 'done', priority: 'high' },
      { id: 13, title: 'Animated Headers', status: 'done', priority: 'medium' },
      { id: 14, title: 'Badge System', status: 'done', priority: 'medium' },
      { id: 15, title: 'Navigation Enhancements', status: 'done', priority: 'low' }
    ]
  },
  {
    id: 'interactive',
    name: 'Interactive Features',
    status: 'completed',
    color: '#7b2fff',
    startDate: '2024-10',
    endDate: '2024-12',
    description: '3D demos and interactive experiences',
    tasks: [
      { id: 16, title: 'Three.js 3D Demo', status: 'done', priority: 'high' },
      { id: 17, title: 'GitHub Pages Deployment', status: 'done', priority: 'high' },
      { id: 18, title: 'Gamification Dashboard', status: 'done', priority: 'high' },
      { id: 19, title: 'Onboarding Wizard', status: 'done', priority: 'medium' },
      { id: 20, title: 'React Router Navigation', status: 'done', priority: 'medium' }
    ]
  },
  {
    id: 'genui',
    name: 'Generative UI',
    status: 'in-progress',
    color: '#ff6600',
    startDate: '2025-01',
    endDate: '2025-03',
    description: 'AI-powered component generation and immersive landing',
    tasks: [
      { id: 21, title: 'GenUI Playground', status: 'done', priority: 'high' },
      { id: 22, title: 'Immersive Landing Page', status: 'done', priority: 'high' },
      { id: 23, title: 'Project Roadmap Board', status: 'done', priority: 'medium' },
      { id: 24, title: 'CVS Pipeline Enhancement', status: 'in-progress', priority: 'medium' },
      { id: 25, title: 'MCP API Integration', status: 'pending', priority: 'low' }
    ]
  },
  {
    id: 'community',
    name: 'Community Growth',
    status: 'planned',
    color: '#ffcc00',
    startDate: '2025-04',
    endDate: '2025-06',
    description: 'Scaling community engagement and contributions',
    tasks: [
      { id: 26, title: 'Contributor Rewards System', status: 'pending', priority: 'high' },
      { id: 27, title: 'Community Guidelines', status: 'pending', priority: 'high' },
      { id: 28, title: 'Automated PR Reviews', status: 'pending', priority: 'medium' },
      { id: 29, title: 'Newsletter Integration', status: 'pending', priority: 'medium' },
      { id: 30, title: 'Discord Community', status: 'pending', priority: 'low' }
    ]
  },
  {
    id: 'ai-integration',
    name: 'AI Agent Integration',
    status: 'planned',
    color: '#ff0066',
    startDate: '2025-07',
    endDate: '2025-09',
    description: 'Advanced AI agent features and automation',
    tasks: [
      { id: 31, title: 'GPT-5.1 Deep Integration', status: 'pending', priority: 'high' },
      { id: 32, title: 'Claude Opus 4.5 Features', status: 'pending', priority: 'high' },
      { id: 33, title: 'Automated Content Updates', status: 'pending', priority: 'medium' },
      { id: 34, title: 'AI-Powered Search', status: 'pending', priority: 'medium' },
      { id: 35, title: 'Smart Recommendations', status: 'pending', priority: 'low' }
    ]
  }
];

// Team members (simulated)
const teamMembers = [
  { id: 1, name: 'SpiralCloudOmega', role: 'Creator', avatar: 'SC', color: '#00f0ff' },
  { id: 2, name: 'Copilot Agent', role: 'AI Developer', avatar: 'CP', color: '#ff00ff' },
  { id: 3, name: 'Community', role: 'Contributors', avatar: '👥', color: '#00ff88' }
];

type ViewMode = 'board' | 'timeline' | 'list';
type FilterStatus = 'all' | 'done' | 'in-progress' | 'pending';

export default function ProjectRoadmap() {
  const [viewMode, setViewMode] = useState<ViewMode>('board');
  const [filterStatus, setFilterStatus] = useState<FilterStatus>('all');
  const [selectedPhase, setSelectedPhase] = useState<string | null>(null);

  const getStatusColor = (status: string) => {
    switch (status) {
      case 'done': return '#00ff88';
      case 'in-progress': return '#ffcc00';
      case 'pending': return '#666';
      default: return '#666';
    }
  };

  const getStatusIcon = (status: string) => {
    switch (status) {
      case 'done': return '✓';
      case 'in-progress': return '◐';
      case 'pending': return '○';
      default: return '○';
    }
  };

  const getPriorityBadge = (priority: string) => {
    switch (priority) {
      case 'high': return { label: 'HIGH', color: '#ff0066' };
      case 'medium': return { label: 'MED', color: '#ffcc00' };
      case 'low': return { label: 'LOW', color: '#00ff88' };
      default: return { label: 'LOW', color: '#666' };
    }
  };

  const filteredPhases = projectPhases.map(phase => ({
    ...phase,
    tasks: phase.tasks.filter(task => 
      filterStatus === 'all' || task.status === filterStatus
    )
  }));

  // Memoize stats calculation - single pass through all tasks
  const stats = useMemo(() => {
    let totalTasks = 0;
    let completedTasks = 0;
    let inProgressTasks = 0;

    for (const phase of projectPhases) {
      for (const task of phase.tasks) {
        totalTasks++;
        if (task.status === 'done') {
          completedTasks++;
        } else if (task.status === 'in-progress') {
          inProgressTasks++;
        }
      }
    }

    const progress = totalTasks > 0 ? Math.round((completedTasks / totalTasks) * 100) : 0;

    return { totalTasks, completedTasks, inProgressTasks, progress };
  }, [projectPhases]);

  const { totalTasks, completedTasks, inProgressTasks, progress } = stats;

  return (
    <div className="project-roadmap">
      {/* Navigation */}
      <nav className="roadmap-nav">
        <Link to="/" className="nav-back">← Back to 3D Demo</Link>
        <h1>📋 Project Roadmap</h1>
        <div className="nav-links">
          <Link to="/landing">🌊 Landing</Link>
          <Link to="/genui">🎨 GenUI</Link>
          <Link to="/gamification">🏆 Leaderboard</Link>
        </div>
      </nav>

      {/* Header Stats */}
      <div className="roadmap-header">
        <div className="header-stats">
          <div className="stat-card">
            <div className="stat-value">{progress}%</div>
            <div className="stat-label">Overall Progress</div>
            <div className="progress-bar">
              <div className="progress-fill" style={{ width: `${progress}%` }} />
            </div>
          </div>
          <div className="stat-card">
            <div className="stat-value">{completedTasks}</div>
            <div className="stat-label">Completed Tasks</div>
          </div>
          <div className="stat-card">
            <div className="stat-value">{inProgressTasks}</div>
            <div className="stat-label">In Progress</div>
          </div>
          <div className="stat-card">
            <div className="stat-value">{totalTasks - completedTasks - inProgressTasks}</div>
            <div className="stat-label">Pending Tasks</div>
          </div>
        </div>

        {/* View Controls */}
        <div className="view-controls">
          <div className="view-modes">
            <button 
              className={`mode-btn ${viewMode === 'board' ? 'active' : ''}`}
              onClick={() => setViewMode('board')}
            >
              📊 Board
            </button>
            <button 
              className={`mode-btn ${viewMode === 'timeline' ? 'active' : ''}`}
              onClick={() => setViewMode('timeline')}
            >
              📅 Timeline
            </button>
            <button 
              className={`mode-btn ${viewMode === 'list' ? 'active' : ''}`}
              onClick={() => setViewMode('list')}
            >
              📝 List
            </button>
          </div>
          <div className="filter-controls">
            <select 
              value={filterStatus} 
              onChange={(e) => setFilterStatus(e.target.value as FilterStatus)}
              className="filter-select"
            >
              <option value="all">All Tasks</option>
              <option value="done">Completed</option>
              <option value="in-progress">In Progress</option>
              <option value="pending">Pending</option>
            </select>
          </div>
        </div>
      </div>

      {/* Board View */}
      {viewMode === 'board' && (
        <div className="board-view">
          {filteredPhases.map((phase) => (
            <div 
              key={phase.id} 
              className={`phase-column ${phase.status}`}
              style={{ '--phase-color': phase.color } as React.CSSProperties}
            >
              <div className="phase-header">
                <div className="phase-title">
                  <span className="phase-dot" />
                  <span>{phase.name}</span>
                </div>
                <span className="phase-count">{phase.tasks.length}</span>
              </div>
              <div className="phase-meta">
                <span className="phase-dates">{phase.startDate} → {phase.endDate}</span>
                <span className={`phase-status ${phase.status}`}>
                  {phase.status.replace('-', ' ')}
                </span>
              </div>
              <div className="phase-tasks">
                {phase.tasks.map((task) => (
                  <div 
                    key={task.id} 
                    className={`task-card ${task.status}`}
                  >
                    <div className="task-header">
                      <span 
                        className="task-status-icon"
                        style={{ color: getStatusColor(task.status) }}
                      >
                        {getStatusIcon(task.status)}
                      </span>
                      <span className="task-title">{task.title}</span>
                    </div>
                    <div className="task-footer">
                      <span 
                        className="priority-badge"
                        style={{ 
                          background: `${getPriorityBadge(task.priority).color}20`,
                          color: getPriorityBadge(task.priority).color
                        }}
                      >
                        {getPriorityBadge(task.priority).label}
                      </span>
                      <span className="task-id">#{task.id}</span>
                    </div>
                  </div>
                ))}
              </div>
            </div>
          ))}
        </div>
      )}

      {/* Timeline View */}
      {viewMode === 'timeline' && (
        <div className="timeline-view">
          <div className="timeline-header">
            <div className="timeline-labels">
              {['Q1 2024', 'Q2 2024', 'Q3 2024', 'Q4 2024', 'Q1 2025', 'Q2 2025', 'Q3 2025'].map((q, i) => (
                <div key={i} className="quarter-label">{q}</div>
              ))}
            </div>
          </div>
          <div className="timeline-body">
            {projectPhases.map((phase, i) => (
              <div 
                key={phase.id}
                className="timeline-phase"
                style={{ '--phase-color': phase.color } as React.CSSProperties}
              >
                <div className="phase-name">{phase.name}</div>
                <div className="phase-bar">
                  <div 
                    className={`bar-fill ${phase.status}`}
                    style={{
                      left: `${(i / 7) * 100}%`,
                      width: `${100 / 7}%`
                    }}
                  >
                    <span className="bar-label">{phase.tasks.filter(t => t.status === 'done').length}/{phase.tasks.length}</span>
                  </div>
                </div>
              </div>
            ))}
          </div>
          <div className="timeline-legend">
            <div className="legend-item">
              <span className="legend-dot completed" />
              <span>Completed</span>
            </div>
            <div className="legend-item">
              <span className="legend-dot in-progress" />
              <span>In Progress</span>
            </div>
            <div className="legend-item">
              <span className="legend-dot planned" />
              <span>Planned</span>
            </div>
          </div>
        </div>
      )}

      {/* List View */}
      {viewMode === 'list' && (
        <div className="list-view">
          <table className="tasks-table">
            <thead>
              <tr>
                <th>ID</th>
                <th>Task</th>
                <th>Phase</th>
                <th>Status</th>
                <th>Priority</th>
              </tr>
            </thead>
            <tbody>
              {filteredPhases.flatMap(phase => 
                phase.tasks.map(task => (
                  <tr key={task.id} className={task.status}>
                    <td className="task-id">#{task.id}</td>
                    <td className="task-title">{task.title}</td>
                    <td>
                      <span 
                        className="phase-badge"
                        style={{ 
                          background: `${phase.color}20`,
                          color: phase.color
                        }}
                      >
                        {phase.name}
                      </span>
                    </td>
                    <td>
                      <span 
                        className="status-badge"
                        style={{ color: getStatusColor(task.status) }}
                      >
                        {getStatusIcon(task.status)} {task.status.replace('-', ' ')}
                      </span>
                    </td>
                    <td>
                      <span 
                        className="priority-badge"
                        style={{ 
                          background: `${getPriorityBadge(task.priority).color}20`,
                          color: getPriorityBadge(task.priority).color
                        }}
                      >
                        {getPriorityBadge(task.priority).label}
                      </span>
                    </td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      )}

      {/* Team Section */}
      <div className="team-section">
        <h3>Project Team</h3>
        <div className="team-grid">
          {teamMembers.map(member => (
            <div key={member.id} className="team-member">
              <div 
                className="member-avatar"
                style={{ background: member.color }}
              >
                {member.avatar}
              </div>
              <div className="member-info">
                <div className="member-name">{member.name}</div>
                <div className="member-role">{member.role}</div>
              </div>
            </div>
          ))}
        </div>
      </div>

      <style>{`
        .project-roadmap {
          min-height: 100vh;
          background: #0a0a1a;
          color: white;
          padding-bottom: 60px;
        }

        .roadmap-nav {
          display: flex;
          justify-content: space-between;
          align-items: center;
          padding: 16px 32px;
          background: rgba(10, 10, 26, 0.95);
          border-bottom: 2px solid #00f0ff;
          position: sticky;
          top: 0;
          z-index: 100;
        }

        .nav-back {
          color: #00f0ff;
          text-decoration: none;
          transition: color 0.3s;
        }

        .nav-back:hover {
          color: #ff00ff;
        }

        .roadmap-nav h1 {
          margin: 0;
          font-size: 1.5rem;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .nav-links {
          display: flex;
          gap: 24px;
        }

        .nav-links a {
          color: #a0a0a0;
          text-decoration: none;
          transition: color 0.3s;
        }

        .nav-links a:hover {
          color: #00f0ff;
        }

        /* Header Stats */
        .roadmap-header {
          padding: 24px 32px;
          border-bottom: 1px solid rgba(0, 240, 255, 0.2);
        }

        .header-stats {
          display: grid;
          grid-template-columns: repeat(4, 1fr);
          gap: 20px;
          margin-bottom: 24px;
        }

        .stat-card {
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 12px;
          padding: 20px;
          text-align: center;
        }

        .stat-card .stat-value {
          font-size: 2.5rem;
          font-weight: bold;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .stat-card .stat-label {
          color: #a0a0a0;
          margin-top: 8px;
        }

        .progress-bar {
          margin-top: 12px;
          height: 6px;
          background: rgba(0, 240, 255, 0.1);
          border-radius: 3px;
          overflow: hidden;
        }

        .progress-fill {
          height: 100%;
          background: linear-gradient(90deg, #00f0ff, #ff00ff);
          transition: width 0.5s ease;
        }

        /* View Controls */
        .view-controls {
          display: flex;
          justify-content: space-between;
          align-items: center;
        }

        .view-modes {
          display: flex;
          gap: 8px;
        }

        .mode-btn {
          background: rgba(255, 255, 255, 0.05);
          border: 1px solid rgba(255, 255, 255, 0.1);
          color: #a0a0a0;
          padding: 10px 20px;
          border-radius: 8px;
          cursor: pointer;
          transition: all 0.3s;
        }

        .mode-btn:hover {
          background: rgba(0, 240, 255, 0.1);
          color: white;
        }

        .mode-btn.active {
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          color: white;
          border-color: transparent;
        }

        .filter-select {
          background: rgba(0, 0, 0, 0.3);
          border: 1px solid rgba(0, 240, 255, 0.3);
          color: white;
          padding: 10px 20px;
          border-radius: 8px;
          cursor: pointer;
        }

        /* Board View */
        .board-view {
          display: flex;
          gap: 20px;
          padding: 24px 32px;
          overflow-x: auto;
        }

        .phase-column {
          flex: 0 0 280px;
          background: rgba(0, 240, 255, 0.03);
          border: 1px solid rgba(0, 240, 255, 0.1);
          border-radius: 16px;
          padding: 16px;
        }

        .phase-header {
          display: flex;
          justify-content: space-between;
          align-items: center;
          margin-bottom: 8px;
        }

        .phase-title {
          display: flex;
          align-items: center;
          gap: 8px;
          font-weight: bold;
        }

        .phase-dot {
          width: 8px;
          height: 8px;
          border-radius: 50%;
          background: var(--phase-color);
        }

        .phase-count {
          background: rgba(255, 255, 255, 0.1);
          padding: 2px 8px;
          border-radius: 10px;
          font-size: 0.75rem;
        }

        .phase-meta {
          display: flex;
          justify-content: space-between;
          font-size: 0.75rem;
          color: #666;
          margin-bottom: 16px;
          padding-bottom: 12px;
          border-bottom: 1px solid rgba(255, 255, 255, 0.1);
        }

        .phase-status {
          text-transform: capitalize;
        }

        .phase-status.completed { color: #00ff88; }
        .phase-status.in-progress { color: #ffcc00; }
        .phase-status.planned { color: #666; }

        .phase-tasks {
          display: flex;
          flex-direction: column;
          gap: 10px;
        }

        .task-card {
          background: rgba(0, 0, 0, 0.3);
          border: 1px solid rgba(255, 255, 255, 0.1);
          border-radius: 10px;
          padding: 12px;
          transition: all 0.3s;
        }

        .task-card:hover {
          border-color: rgba(0, 240, 255, 0.5);
          transform: translateY(-2px);
        }

        .task-card.done {
          opacity: 0.7;
        }

        .task-header {
          display: flex;
          align-items: flex-start;
          gap: 8px;
        }

        .task-status-icon {
          font-weight: bold;
        }

        .task-title {
          font-size: 0.9rem;
          line-height: 1.4;
        }

        .task-footer {
          display: flex;
          justify-content: space-between;
          align-items: center;
          margin-top: 10px;
        }

        .priority-badge {
          font-size: 0.65rem;
          padding: 2px 6px;
          border-radius: 4px;
          font-weight: bold;
        }

        .task-id {
          font-size: 0.75rem;
          color: #666;
        }

        /* Timeline View */
        .timeline-view {
          padding: 24px 32px;
        }

        .timeline-header {
          margin-bottom: 24px;
        }

        .timeline-labels {
          display: flex;
        }

        .quarter-label {
          flex: 1;
          text-align: center;
          color: #666;
          font-size: 0.875rem;
          padding: 8px;
          border-left: 1px solid rgba(255, 255, 255, 0.1);
        }

        .quarter-label:first-child {
          border-left: none;
        }

        .timeline-body {
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 12px;
          overflow: hidden;
        }

        .timeline-phase {
          display: flex;
          align-items: center;
          padding: 16px;
          border-bottom: 1px solid rgba(255, 255, 255, 0.1);
        }

        .timeline-phase:last-child {
          border-bottom: none;
        }

        .timeline-phase .phase-name {
          width: 150px;
          font-weight: bold;
          color: var(--phase-color);
        }

        .phase-bar {
          flex: 1;
          height: 30px;
          background: rgba(0, 0, 0, 0.3);
          border-radius: 6px;
          position: relative;
          overflow: hidden;
        }

        .bar-fill {
          position: absolute;
          top: 0;
          height: 100%;
          border-radius: 6px;
          display: flex;
          align-items: center;
          justify-content: center;
          font-size: 0.75rem;
          font-weight: bold;
        }

        .bar-fill.completed {
          background: linear-gradient(90deg, var(--phase-color), var(--phase-color)80);
        }

        .bar-fill.in-progress {
          background: linear-gradient(90deg, var(--phase-color), transparent);
          animation: pulse 2s infinite;
        }

        .bar-fill.planned {
          background: rgba(255, 255, 255, 0.1);
          border: 1px dashed rgba(255, 255, 255, 0.3);
        }

        @keyframes pulse {
          0%, 100% { opacity: 1; }
          50% { opacity: 0.7; }
        }

        .timeline-legend {
          display: flex;
          gap: 24px;
          justify-content: center;
          margin-top: 24px;
        }

        .legend-item {
          display: flex;
          align-items: center;
          gap: 8px;
          font-size: 0.875rem;
          color: #a0a0a0;
        }

        .legend-dot {
          width: 12px;
          height: 12px;
          border-radius: 3px;
        }

        .legend-dot.completed { background: #00ff88; }
        .legend-dot.in-progress { background: #ffcc00; }
        .legend-dot.planned { background: rgba(255, 255, 255, 0.2); border: 1px dashed #666; }

        /* List View */
        .list-view {
          padding: 24px 32px;
        }

        .tasks-table {
          width: 100%;
          border-collapse: collapse;
          background: rgba(0, 240, 255, 0.03);
          border: 1px solid rgba(0, 240, 255, 0.1);
          border-radius: 12px;
          overflow: hidden;
        }

        .tasks-table th {
          text-align: left;
          padding: 16px;
          background: rgba(0, 0, 0, 0.3);
          color: #00f0ff;
          font-weight: bold;
          border-bottom: 1px solid rgba(0, 240, 255, 0.2);
        }

        .tasks-table td {
          padding: 14px 16px;
          border-bottom: 1px solid rgba(255, 255, 255, 0.05);
        }

        .tasks-table tr:last-child td {
          border-bottom: none;
        }

        .tasks-table tr:hover {
          background: rgba(0, 240, 255, 0.05);
        }

        .tasks-table .task-id {
          color: #666;
          font-family: monospace;
        }

        .tasks-table .task-title {
          color: white;
        }

        .phase-badge {
          padding: 4px 10px;
          border-radius: 6px;
          font-size: 0.75rem;
          font-weight: bold;
        }

        .status-badge {
          display: flex;
          align-items: center;
          gap: 6px;
          font-size: 0.875rem;
          text-transform: capitalize;
        }

        /* Team Section */
        .team-section {
          padding: 24px 32px;
          border-top: 1px solid rgba(0, 240, 255, 0.2);
          margin-top: 24px;
        }

        .team-section h3 {
          margin: 0 0 20px;
          color: #00f0ff;
        }

        .team-grid {
          display: flex;
          gap: 20px;
        }

        .team-member {
          display: flex;
          align-items: center;
          gap: 12px;
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 12px;
          padding: 16px 24px;
        }

        .member-avatar {
          width: 48px;
          height: 48px;
          border-radius: 50%;
          display: flex;
          align-items: center;
          justify-content: center;
          font-weight: bold;
          color: white;
          font-size: 1.1rem;
        }

        .member-name {
          font-weight: bold;
        }

        .member-role {
          color: #a0a0a0;
          font-size: 0.875rem;
        }

        /* Responsive */
        @media (max-width: 1024px) {
          .header-stats {
            grid-template-columns: repeat(2, 1fr);
          }
        }

        @media (max-width: 768px) {
          .roadmap-nav {
            flex-direction: column;
            gap: 12px;
          }

          .nav-links {
            display: none;
          }

          .header-stats {
            grid-template-columns: 1fr;
          }

          .view-controls {
            flex-direction: column;
            gap: 12px;
          }

          .board-view {
            padding: 16px;
          }

          .phase-column {
            flex: 0 0 260px;
          }

          .timeline-phase .phase-name {
            width: 100px;
            font-size: 0.875rem;
          }

          .team-grid {
            flex-direction: column;
          }
        }
      `}</style>
    </div>
  );
}
