import { motion } from 'framer-motion';
import { GlassPanel, ThemeSwitcher } from '../components/ui';
import { useState } from 'react';

export function Settings() {
  const [aiModel, setAiModel] = useState('phi-3');
  const [temperature, setTemperature] = useState(0.7);

  const ports = [
    { name: 'React', port: 3000, color: 'text-blue-400' },
    { name: 'Vue 3', port: 5173, color: 'text-green-400' },
    { name: '.NET 8', port: 5000, color: 'text-purple-400' },
  ];

  return (
    <div className="min-h-screen bg-gray-950 text-gray-100 p-8">
      <motion.div
        className="max-w-5xl mx-auto"
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
      >
        {/* Header */}
        <div className="mb-8">
          <h1 className="text-4xl font-bold mb-2">⚙️ Settings</h1>
          <p className="text-gray-400">Configure your workspace preferences</p>
        </div>

        {/* 2 Column Layout on Desktop */}
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          {/* Section 1 - Appearance */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.1 }}
          >
            <GlassPanel>
              <h2 className="text-xl font-semibold mb-4">Appearance</h2>
              <div className="space-y-2">
                <label className="block text-sm text-gray-400 mb-2">Theme</label>
                <ThemeSwitcher />
              </div>
            </GlassPanel>
          </motion.div>

          {/* Section 2 - AI Configuration */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.2 }}
          >
            <GlassPanel>
              <h2 className="text-xl font-semibold mb-4">AI Configuration</h2>
              
              {/* Model Dropdown */}
              <div className="mb-4">
                <label className="block text-sm text-gray-400 mb-2">Model</label>
                <select
                  value={aiModel}
                  onChange={(e) => setAiModel(e.target.value)}
                  className="w-full bg-white/10 border border-white/20 rounded-lg px-3 py-2 text-gray-100 focus:outline-none focus:ring-2 focus:ring-indigo-500"
                >
                  <option value="phi-3">Phi-3</option>
                  <option value="llama-3">Llama 3</option>
                  <option value="mistral-7b">Mistral 7B</option>
                </select>
              </div>

              {/* Temperature Slider */}
              <div>
                <label className="block text-sm text-gray-400 mb-2">
                  Temperature: {temperature.toFixed(2)}
                </label>
                <input
                  type="range"
                  min="0"
                  max="1"
                  step="0.01"
                  value={temperature}
                  onChange={(e) => setTemperature(parseFloat(e.target.value))}
                  className="w-full h-2 bg-white/10 rounded-lg appearance-none cursor-pointer accent-indigo-500"
                />
                <div className="flex justify-between text-xs text-gray-500 mt-1">
                  <span>0</span>
                  <span>1</span>
                </div>
              </div>
            </GlassPanel>
          </motion.div>

          {/* Section 3 - Developer Info (Full Width) */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.3 }}
            className="lg:col-span-2"
          >
            <GlassPanel>
              <h2 className="text-xl font-semibold mb-4">Developer</h2>
              
              {/* Port Table */}
              <div className="mb-6">
                <h3 className="text-sm text-gray-400 mb-3">Development Ports</h3>
                <div className="space-y-2">
                  {ports.map((item) => (
                    <div
                      key={item.name}
                      className="flex justify-between items-center p-3 bg-white/5 rounded-lg"
                    >
                      <span className={item.color}>{item.name}</span>
                      <span className="font-mono text-cyan-400">:{item.port}</span>
                    </div>
                  ))}
                </div>
              </div>

              {/* GitHub Link & Version */}
              <div className="flex items-center justify-between">
                <a
                  href="https://github.com/SpiralCloudOmega/DevTeam6"
                  target="_blank"
                  rel="noopener noreferrer"
                  className="inline-flex items-center gap-2 px-4 py-2 bg-indigo-600 hover:bg-indigo-700 rounded-lg transition-colors"
                >
                  <span>📦</span>
                  <span>View on GitHub</span>
                </a>
                <span className="text-sm text-gray-400">Version: v3.5.0</span>
              </div>
            </GlassPanel>
          </motion.div>
        </div>
      </motion.div>
    </div>
  );
}

export default Settings;
