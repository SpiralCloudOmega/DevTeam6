import { useState, useEffect } from 'react'
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  BarElement,
  Title,
  Tooltip,
  Legend,
  ArcElement,
  PointElement,
  LineElement,
} from 'chart.js'
import { Bar, Doughnut, Line } from 'react-chartjs-2'

ChartJS.register(
  CategoryScale,
  LinearScale,
  BarElement,
  Title,
  Tooltip,
  Legend,
  ArcElement,
  PointElement,
  LineElement
)

// Mock contributor data 
// TODO: Replace with actual GitHub API calls using:
// - GET /repos/{owner}/{repo}/contributors for contributor list
// - GET /repos/{owner}/{repo}/stats/contributors for detailed stats
// - Consider using GitHub GraphQL API for more efficient data fetching
const mockContributors = [
  { name: 'SpiralCloudOmega', commits: 156, prs: 42, issues: 28, score: 2840, avatar: 'https://github.com/SpiralCloudOmega.png', badges: ['🏆', '⭐', '🔥', '💎'] },
  { name: 'DevMaster2024', commits: 89, prs: 23, issues: 15, score: 1520, avatar: 'https://ui-avatars.com/api/?name=DM&background=00f0ff&color=0a0a1a', badges: ['⭐', '🔥'] },
  { name: 'CodeNinja42', commits: 67, prs: 18, issues: 12, score: 1140, avatar: 'https://ui-avatars.com/api/?name=CN&background=ff00ff&color=fff', badges: ['⭐'] },
  { name: 'OpenSourceHero', commits: 45, prs: 12, issues: 8, score: 780, avatar: 'https://ui-avatars.com/api/?name=OS&background=00ff88&color=0a0a1a', badges: ['🌟'] },
  { name: 'AIEnthusiast', commits: 34, prs: 9, issues: 6, score: 590, avatar: 'https://ui-avatars.com/api/?name=AI&background=7b2fff&color=fff', badges: [] },
]

const achievements = [
  { id: 1, name: 'First Commit', icon: '🎯', description: 'Make your first commit', unlocked: true, rarity: 'common' },
  { id: 2, name: 'PR Master', icon: '🔀', description: 'Get 10 PRs merged', unlocked: true, rarity: 'rare' },
  { id: 3, name: 'Bug Hunter', icon: '🐛', description: 'Close 20 issues', unlocked: true, rarity: 'rare' },
  { id: 4, name: 'Documentation Hero', icon: '📚', description: 'Improve documentation', unlocked: true, rarity: 'common' },
  { id: 5, name: 'Code Reviewer', icon: '👀', description: 'Review 50 PRs', unlocked: false, rarity: 'epic' },
  { id: 6, name: 'Night Owl', icon: '🦉', description: 'Commit after midnight', unlocked: true, rarity: 'common' },
  { id: 7, name: 'Early Bird', icon: '🌅', description: 'Commit before 6 AM', unlocked: false, rarity: 'rare' },
  { id: 8, name: 'Streak Master', icon: '🔥', description: '30-day commit streak', unlocked: false, rarity: 'legendary' },
  { id: 9, name: 'Diamond Contributor', icon: '💎', description: '1000+ contributions', unlocked: false, rarity: 'legendary' },
  { id: 10, name: 'Community Champion', icon: '👑', description: 'Help 100 contributors', unlocked: false, rarity: 'legendary' },
]

const rarityColors: Record<string, string> = {
  common: '#00ff88',
  rare: '#00f0ff',
  epic: '#aa00ff',
  legendary: '#ff6600',
}

function GamificationDashboard() {
  const [selectedTab, setSelectedTab] = useState<'leaderboard' | 'achievements' | 'stats'>('leaderboard')
  const [animatedScores, setAnimatedScores] = useState<number[]>([])

  useEffect(() => {
    // Animate scores on mount
    const targetScores = mockContributors.map(c => c.score)
    const duration = 1500
    const steps = 60
    const stepDuration = duration / steps
    
    let currentStep = 0
    const interval = setInterval(() => {
      currentStep++
      const progress = currentStep / steps
      const easedProgress = 1 - Math.pow(1 - progress, 3) // Ease out cubic
      
      setAnimatedScores(targetScores.map(score => Math.round(score * easedProgress)))
      
      if (currentStep >= steps) {
        clearInterval(interval)
      }
    }, stepDuration)
    
    return () => clearInterval(interval)
  }, [])

  const barChartData = {
    labels: mockContributors.map(c => c.name),
    datasets: [
      {
        label: 'Commits',
        data: mockContributors.map(c => c.commits),
        backgroundColor: 'rgba(0, 240, 255, 0.8)',
        borderColor: '#00f0ff',
        borderWidth: 2,
      },
      {
        label: 'Pull Requests',
        data: mockContributors.map(c => c.prs),
        backgroundColor: 'rgba(255, 0, 255, 0.8)',
        borderColor: '#ff00ff',
        borderWidth: 2,
      },
      {
        label: 'Issues',
        data: mockContributors.map(c => c.issues),
        backgroundColor: 'rgba(0, 255, 136, 0.8)',
        borderColor: '#00ff88',
        borderWidth: 2,
      },
    ],
  }

  const doughnutData = {
    labels: ['Commits', 'Pull Requests', 'Issues', 'Reviews'],
    datasets: [
      {
        data: [450, 120, 85, 200],
        backgroundColor: ['#00f0ff', '#ff00ff', '#00ff88', '#7b2fff'],
        borderColor: '#0a0a1a',
        borderWidth: 3,
      },
    ],
  }

  const lineChartData = {
    labels: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
    datasets: [
      {
        label: 'Contributions',
        data: [45, 52, 78, 65, 89, 112, 98, 134, 156, 178, 195, 220],
        borderColor: '#00f0ff',
        backgroundColor: 'rgba(0, 240, 255, 0.1)',
        fill: true,
        tension: 0.4,
      },
    ],
  }

  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        labels: {
          color: '#ffffff',
          font: { family: 'JetBrains Mono, monospace' },
        },
      },
    },
    scales: {
      x: {
        ticks: { color: '#888888' },
        grid: { color: 'rgba(255, 255, 255, 0.1)' },
      },
      y: {
        ticks: { color: '#888888' },
        grid: { color: 'rgba(255, 255, 255, 0.1)' },
      },
    },
  }

  return (
    <div className="gamification-dashboard">
      <header className="dashboard-header">
        <h1>🏆 Gamification Dashboard</h1>
        <p>Track contributions, earn achievements, climb the leaderboard!</p>
      </header>

      <nav className="dashboard-tabs">
        <button 
          className={`tab ${selectedTab === 'leaderboard' ? 'active' : ''}`}
          onClick={() => setSelectedTab('leaderboard')}
        >
          🥇 Leaderboard
        </button>
        <button 
          className={`tab ${selectedTab === 'achievements' ? 'active' : ''}`}
          onClick={() => setSelectedTab('achievements')}
        >
          🎖️ Achievements
        </button>
        <button 
          className={`tab ${selectedTab === 'stats' ? 'active' : ''}`}
          onClick={() => setSelectedTab('stats')}
        >
          📊 Statistics
        </button>
      </nav>

      <main className="dashboard-content">
        {selectedTab === 'leaderboard' && (
          <div className="leaderboard-section">
            <div className="leaderboard-podium">
              {mockContributors.slice(0, 3).map((contributor, index) => (
                <div key={contributor.name} className={`podium-spot rank-${index + 1}`}>
                  <div className="rank-badge">{index === 0 ? '🥇' : index === 1 ? '🥈' : '🥉'}</div>
                  <img src={contributor.avatar} alt={contributor.name} className="contributor-avatar" />
                  <h3>{contributor.name}</h3>
                  <div className="score">{animatedScores[index] || 0} pts</div>
                  <div className="badges">{contributor.badges.join(' ')}</div>
                </div>
              ))}
            </div>

            <div className="leaderboard-table">
              <table>
                <thead>
                  <tr>
                    <th>Rank</th>
                    <th>Contributor</th>
                    <th>Commits</th>
                    <th>PRs</th>
                    <th>Issues</th>
                    <th>Score</th>
                    <th>Badges</th>
                  </tr>
                </thead>
                <tbody>
                  {mockContributors.map((contributor, index) => (
                    <tr key={contributor.name} className={index < 3 ? 'top-three' : ''}>
                      <td className="rank">#{index + 1}</td>
                      <td className="contributor">
                        <img src={contributor.avatar} alt={contributor.name} />
                        <span>{contributor.name}</span>
                      </td>
                      <td>{contributor.commits}</td>
                      <td>{contributor.prs}</td>
                      <td>{contributor.issues}</td>
                      <td className="score">{animatedScores[index] || 0}</td>
                      <td className="badges">{contributor.badges.join(' ') || '-'}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        )}

        {selectedTab === 'achievements' && (
          <div className="achievements-section">
            <div className="achievements-summary">
              <div className="summary-card">
                <span className="value">{achievements.filter(a => a.unlocked).length}</span>
                <span className="label">Unlocked</span>
              </div>
              <div className="summary-card">
                <span className="value">{achievements.length}</span>
                <span className="label">Total</span>
              </div>
              <div className="summary-card">
                <span className="value">{Math.round((achievements.filter(a => a.unlocked).length / achievements.length) * 100)}%</span>
                <span className="label">Completion</span>
              </div>
            </div>

            <div className="achievements-grid">
              {achievements.map(achievement => (
                <div 
                  key={achievement.id} 
                  className={`achievement-card ${achievement.unlocked ? 'unlocked' : 'locked'}`}
                  style={{ '--rarity-color': rarityColors[achievement.rarity] } as React.CSSProperties}
                >
                  <div className="achievement-icon">{achievement.icon}</div>
                  <h4>{achievement.name}</h4>
                  <p>{achievement.description}</p>
                  <span className="rarity-badge">{achievement.rarity}</span>
                  {!achievement.unlocked && <div className="lock-overlay">🔒</div>}
                </div>
              ))}
            </div>
          </div>
        )}

        {selectedTab === 'stats' && (
          <div className="stats-section">
            <div className="chart-row">
              <div className="chart-container">
                <h3>Contribution Activity</h3>
                <div className="chart-wrapper">
                  <Bar data={barChartData} options={chartOptions} />
                </div>
              </div>
              <div className="chart-container small">
                <h3>Contribution Types</h3>
                <div className="chart-wrapper">
                  <Doughnut data={doughnutData} options={{ ...chartOptions, maintainAspectRatio: true }} />
                </div>
              </div>
            </div>
            <div className="chart-container full-width">
              <h3>Monthly Trend</h3>
              <div className="chart-wrapper">
                <Line data={lineChartData} options={chartOptions} />
              </div>
            </div>
          </div>
        )}
      </main>

      <footer className="dashboard-footer">
        <a href="/" className="back-link">← Back to 3D Demo</a>
        <a href="/onboarding" className="next-link">Start Onboarding →</a>
      </footer>
    </div>
  )
}

export default GamificationDashboard
