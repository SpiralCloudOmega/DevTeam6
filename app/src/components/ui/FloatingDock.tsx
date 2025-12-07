import { motion } from 'framer-motion';
import { useNavigate, useLocation } from 'react-router-dom';

const items = [
  { id: 'home', icon: '🏠', label: 'Home', path: '/' },
  { id: 'ai', icon: '🤖', label: 'AI', path: '/ai' },
  { id: 'workstation', icon: '🖥️', label: 'Workstation', path: '/code' },
  { id: 'graph', icon: '🧠', label: 'Graph', path: '/graph-editor' },
  { id: 'workflow', icon: '⚡', label: 'Workflow', path: '/workflow' },
  { id: 'templates', icon: '📦', label: 'Templates', path: '/templates' },
];

export function FloatingDock() {
  const navigate = useNavigate();
  const location = useLocation();

  return (
    <motion.div
      className="fixed bottom-6 left-1/2 -translate-x-1/2 z-40"
      initial={{ y: 100 }}
      animate={{ y: 0 }}
    >
      <div className="flex gap-2 px-4 py-3 bg-gray-900/80 backdrop-blur-xl border border-white/10 rounded-2xl">
        {items.map((item) => (
          <motion.button
            key={item.id}
            className={`p-3 rounded-xl ${location.pathname === item.path ? 'bg-indigo-500/20 text-white' : 'text-gray-400 hover:text-white'}`}
            onClick={() => navigate(item.path)}
            whileHover={{ scale: 1.2, y: -8 }}
            whileTap={{ scale: 0.9 }}
            title={item.label}
          >
            <span className="text-2xl">{item.icon}</span>
          </motion.button>
        ))}
      </div>
    </motion.div>
  );
}

export default FloatingDock;
