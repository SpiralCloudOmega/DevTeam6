import { motion } from 'framer-motion';
import { useNavigate } from 'react-router-dom';
import { GlassPanel } from '../components/ui';

const templates = [
  {
    id: 'react',
    icon: '⚛️',
    title: 'React Starter',
    description: 'React 18 + TypeScript + Vite',
    tags: ['React 18', 'TypeScript', 'Vite'],
    port: 3000,
  },
  {
    id: 'vue',
    icon: '💚',
    title: 'Vue 3 Starter',
    description: 'Vue 3 + Pinia + Router',
    tags: ['Vue 3', 'Pinia', 'Vite'],
    port: 5173,
  },
  {
    id: 'dotnet',
    icon: '🟣',
    title: '.NET 8 API',
    description: 'ASP.NET Core + Swagger',
    tags: ['.NET 8', 'Swagger', 'Web API'],
    port: 5000,
  },
  {
    id: 'cpp',
    icon: '⚙️',
    title: 'C++ Workspace',
    description: 'Modern C++20 + CMake + Vulkan/Qt6',
    tags: ['C++20', 'CMake', 'OpenGL', 'Vulkan', 'SDL2', 'Qt6'],
    port: 8080,
  },
  {
    id: 'c',
    icon: '🔧',
    title: 'C Workspace',
    description: 'Pure C17 + Make/CMake + SDL2',
    tags: ['C17', 'Make', 'CMake', 'GDB', 'Valgrind'],
    port: 8080,
  },
  {
    id: 'audio-dsp',
    icon: '🎵',
    title: 'Audio/DSP',
    description: 'JUCE Framework + VST3/AU/AAX',
    tags: ['JUCE', 'VST3', 'Audio', 'DSP', 'Plugins'],
    port: 0,
  },
  {
    id: 'graphics-engine',
    icon: '🎨',
    title: 'Graphics Engine',
    description: 'Vulkan + GLFW + Shaders',
    tags: ['Vulkan', 'OpenGL', 'GLSL', 'SPIR-V', 'Rendering'],
    port: 8080,
  },
];

const tools = [
  { id: 'sandpack', icon: '🧪', label: 'Sandbox' },
  { id: 'stackblitz', icon: '⚡', label: 'StackBlitz' },
  { id: 'vscode', icon: '💻', label: 'VS Code' },
];

export default function Templates() {
  const navigate = useNavigate();

  const openTemplate = (tool: string, template: string) => {
    navigate(`/code?tool=${tool}&template=${template}`);
  };

  return (
    <div className="min-h-screen bg-gray-950 text-white p-8">
      {/* Page Title */}
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        className="text-center mb-12"
      >
        <h1 className="text-5xl font-bold mb-4">📦 Starter Templates</h1>
        <p className="text-xl text-gray-400">
          Production-ready templates for your next project
        </p>
      </motion.div>

      {/* Templates Grid */}
      <div className="max-w-6xl mx-auto">
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {templates.map((template, index) => (
            <motion.div
              key={template.id}
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ delay: index * 0.1 }}
            >
              <GlassPanel hover className="h-full">
                {/* Template Header */}
                <div className="text-center mb-4">
                  <div className="text-5xl mb-3">{template.icon}</div>
                  <h3 className="text-2xl font-semibold mb-2">
                    {template.title}
                  </h3>
                  <p className="text-gray-400 text-sm mb-3">
                    {template.description}
                  </p>
                </div>

                {/* Tags */}
                <div className="flex flex-wrap gap-2 justify-center mb-4">
                  {template.tags.map((tag) => (
                    <span
                      key={tag}
                      className="bg-white/10 rounded-full px-2 py-1 text-xs"
                    >
                      {tag}
                    </span>
                  ))}
                </div>

                {/* Port Badge */}
                {template.port > 0 && (
                  <div className="text-center mb-4">
                    <span className="text-xs text-cyan-400">
                      Port: {template.port}
                    </span>
                  </div>
                )}

                {/* Tool Buttons */}
                <div className="space-y-2">
                  {tools.map((tool) => (
                    <button
                      key={tool.id}
                      onClick={() => openTemplate(tool.id, template.id)}
                      className="w-full px-4 py-2 bg-indigo-500/20 hover:bg-indigo-500/30 rounded-lg transition-colors text-sm flex items-center justify-center gap-2"
                    >
                      <span>{tool.icon}</span>
                      <span>{tool.label}</span>
                    </button>
                  ))}
                </div>
              </GlassPanel>
            </motion.div>
          ))}
        </div>
      </div>
    </div>
  );
}
