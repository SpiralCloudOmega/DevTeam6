import { useState, useEffect } from 'react';
import { motion } from 'framer-motion';
import { useSearchParams } from 'react-router-dom';
import { Sandpack } from '@codesandbox/sandpack-react';
import { GlassPanel } from '../components/ui';
import { StackBlitzEmbed, VSCodeEmbed } from '../components/ide';
import { reactTemplate, vueTemplate, vanillaTemplate } from '../config/sandpackTemplates';

type Tool = 'sandpack' | 'vscode' | 'stackblitz';
type Template = 'react' | 'vue' | 'dotnet';

const templates = { 
  react: reactTemplate, 
  vue: vueTemplate, 
  dotnet: vanillaTemplate 
};

export default function CodeWorkstation() {
  const [activeTool, setActiveTool] = useState<Tool>('sandpack');
  const [activeTemplate, setActiveTemplate] = useState<Template>('react');
  const [searchParams] = useSearchParams();

  useEffect(() => {
    const toolParam = searchParams.get('tool');
    const templateParam = searchParams.get('template');
    
    if (toolParam && ['sandpack', 'stackblitz', 'vscode'].includes(toolParam)) {
      setActiveTool(toolParam as typeof activeTool);
    }
    if (templateParam && ['react', 'vue', 'dotnet'].includes(templateParam)) {
      setActiveTemplate(templateParam as typeof activeTemplate);
    }
  }, [searchParams]);

  const loadTemplate = (template: Template) => {
    setActiveTemplate(template);
    setActiveTool('sandpack');
  };

  return (
    <motion.div
      className="flex h-[calc(100vh-80px)] bg-gray-950 text-gray-100"
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
    >
      {/* Left Sidebar */}
      <GlassPanel className="w-64 flex-shrink-0 m-4 overflow-y-auto">
        <div className="space-y-6">
          {/* Templates Section */}
          <div>
            <h3 className="text-sm font-semibold text-gray-400 mb-3">Templates</h3>
            <div className="space-y-2">
              <button
                onClick={() => loadTemplate('react')}
                className={`w-full flex items-center gap-2 px-3 py-2 rounded-lg transition-colors ${
                  activeTemplate === 'react' && activeTool === 'sandpack'
                    ? 'bg-indigo-500/20 text-white'
                    : 'text-gray-300 hover:bg-white/5'
                }`}
              >
                <span className="text-xl">⚛️</span>
                <span>React</span>
              </button>
              <button
                onClick={() => loadTemplate('vue')}
                className={`w-full flex items-center gap-2 px-3 py-2 rounded-lg transition-colors ${
                  activeTemplate === 'vue' && activeTool === 'sandpack'
                    ? 'bg-indigo-500/20 text-white'
                    : 'text-gray-300 hover:bg-white/5'
                }`}
              >
                <span className="text-xl">💚</span>
                <span>Vue</span>
              </button>
              <button
                onClick={() => loadTemplate('dotnet')}
                className={`w-full flex items-center gap-2 px-3 py-2 rounded-lg transition-colors ${
                  activeTemplate === 'dotnet' && activeTool === 'sandpack'
                    ? 'bg-indigo-500/20 text-white'
                    : 'text-gray-300 hover:bg-white/5'
                }`}
              >
                <span className="text-xl">🟣</span>
                <span>.NET</span>
              </button>
            </div>
          </div>

          {/* Tools Section */}
          <div>
            <h3 className="text-sm font-semibold text-gray-400 mb-3">Tools</h3>
            <div className="space-y-2">
              <button
                onClick={() => setActiveTool('vscode')}
                className={`w-full flex items-center gap-2 px-3 py-2 rounded-lg transition-colors ${
                  activeTool === 'vscode'
                    ? 'bg-indigo-500/20 text-white'
                    : 'text-gray-300 hover:bg-white/5'
                }`}
              >
                <span className="text-xl">💻</span>
                <span>VS Code</span>
              </button>
              <button
                onClick={() => setActiveTool('stackblitz')}
                className={`w-full flex items-center gap-2 px-3 py-2 rounded-lg transition-colors ${
                  activeTool === 'stackblitz'
                    ? 'bg-indigo-500/20 text-white'
                    : 'text-gray-300 hover:bg-white/5'
                }`}
              >
                <span className="text-xl">⚡</span>
                <span>StackBlitz</span>
              </button>
              <button
                onClick={() => setActiveTool('sandpack')}
                className={`w-full flex items-center gap-2 px-3 py-2 rounded-lg transition-colors ${
                  activeTool === 'sandpack'
                    ? 'bg-indigo-500/20 text-white'
                    : 'text-gray-300 hover:bg-white/5'
                }`}
              >
                <span className="text-xl">📦</span>
                <span>Sandpack</span>
              </button>
            </div>
          </div>
        </div>
      </GlassPanel>

      {/* Main Area */}
      <div className="flex-1 m-4 ml-0">
        {activeTool === 'vscode' && (
          <VSCodeEmbed 
            path={
              activeTemplate === 'react' 
                ? 'templates/react-starter' 
                : activeTemplate === 'vue' 
                ? 'templates/vue3-starter' 
                : 'templates/dotnet8-api'
            }
            height="100%"
          />
        )}

        {activeTool === 'stackblitz' && (
          <StackBlitzEmbed template={activeTemplate} height="100%" />
        )}

        {activeTool === 'sandpack' && (
          <div className="w-full h-full">
            <Sandpack {...templates[activeTemplate]} theme="dark" />
          </div>
        )}
      </div>
    </motion.div>
  );
}
