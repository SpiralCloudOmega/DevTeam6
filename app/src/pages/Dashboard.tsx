import { motion } from 'framer-motion';
import { useNavigate } from 'react-router-dom';
import { GlassPanel } from '../components/ui';

const quickActions = [
  {
    id: 'ai',
    icon: '🤖',
    title: 'AI Playground',
    description: 'Chat with AI assistant',
    path: '/ai',
  },
  {
    id: 'code',
    icon: '🖥️',
    title: 'Code Workstation',
    description: 'In-browser IDE with 3 tools',
    path: '/code',
  },
  {
    id: 'graph',
    icon: '📊',
    title: 'Graph Editor',
    description: 'Visualize node relationships',
    path: '/graph-editor',
  },
  {
    id: 'knowledge',
    icon: '🧠',
    title: 'Knowledge Hub',
    description: 'Semantic knowledge exploration',
    path: '/semantic-hub',
  },
  {
    id: 'templates',
    icon: '📦',
    title: 'Templates',
    description: 'React, Vue 3, .NET 8 starters',
    path: '/templates',
  },
  {
    id: 'settings',
    icon: '⚙️',
    title: 'Settings',
    description: 'Configure your workspace',
    path: '/settings',
  },
];

export default function Dashboard() {
  const navigate = useNavigate();

  return (
    <div className="min-h-screen bg-gray-950 text-white p-8">
      {/* Hero Section */}
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        className="text-center mb-16"
      >
        <h1 className="text-6xl font-bold mb-4 bg-gradient-to-r from-indigo-500 via-purple-500 to-cyan-500 bg-clip-text text-transparent">
          DevTeam6 Workstation
        </h1>
        <p className="text-xl text-gray-400">
          Your full-stack development environment
        </p>
      </motion.div>

      {/* Quick Actions Grid */}
      <div className="max-w-7xl mx-auto mb-16">
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {quickActions.map((action, index) => (
            <motion.div
              key={action.id}
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ delay: index * 0.1 }}
            >
              <GlassPanel
                hover
                className="cursor-pointer h-full"
                onClick={() => navigate(action.path)}
              >
                <div className="text-center">
                  <div className="text-4xl mb-4">{action.icon}</div>
                  <h3 className="text-xl font-semibold mb-2">{action.title}</h3>
                  <p className="text-gray-400 text-sm">{action.description}</p>
                </div>
              </GlassPanel>
            </motion.div>
          ))}
        </div>
      </div>

      {/* Stats Row */}
      <motion.div
        initial={{ opacity: 0 }}
        animate={{ opacity: 1 }}
        transition={{ delay: 0.6 }}
        className="text-center text-gray-400"
      >
        <div className="flex justify-center items-center gap-8">
          <span className="text-lg">3 Templates</span>
          <span className="text-gray-600">|</span>
          <span className="text-lg">6 Tools</span>
          <span className="text-gray-600">|</span>
          <span className="text-lg">∞ Possibilities</span>
        </div>
      </motion.div>
    </div>
  );
}
