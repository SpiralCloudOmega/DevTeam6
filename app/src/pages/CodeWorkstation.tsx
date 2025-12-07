import { useState } from 'react';
import { motion } from 'framer-motion';
import { Sandpack } from '@codesandbox/sandpack-react';
import { GlassPanel } from '../components/ui';

type Tool = 'sandpack' | 'vscode' | 'stackblitz';
type Template = 'react' | 'vue' | 'dotnet';

const templates = {
  react: {
    files: {
      '/App.tsx': `import { useState } from 'react';

export default function App() {
  const [count, setCount] = useState(0);

  return (
    <div className="app">
      <h1>React Template</h1>
      <button onClick={() => setCount(count + 1)}>
        Count: {count}
      </button>
      <style>{\`
        .app { padding: 2rem; font-family: sans-serif; }
        button { padding: 0.5rem 1rem; font-size: 1rem; cursor: pointer; }
      \`}</style>
    </div>
  );
}`,
      '/index.tsx': `import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import App from './App';

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <App />
  </StrictMode>
);`,
    },
  },
  vue: {
    files: {
      '/App.vue': `<template>
  <div class="app">
    <h1>Vue 3 Template</h1>
    <button @click="count++">Count: {{ count }}</button>
  </div>
</template>

<script setup>
import { ref } from 'vue';
const count = ref(0);
</script>

<style scoped>
.app { padding: 2rem; font-family: sans-serif; }
button { padding: 0.5rem 1rem; font-size: 1rem; cursor: pointer; }
</style>`,
      '/main.js': `import { createApp } from 'vue';
import App from './App.vue';

createApp(App).mount('#app');`,
    },
  },
  dotnet: {
    files: {
      '/Program.cs': `using System;

class Program
{
    static void Main()
    {
        Console.WriteLine(".NET 8 Template");
        Console.WriteLine("Hello from C#!");
        
        var numbers = new[] { 1, 2, 3, 4, 5 };
        var sum = numbers.Sum();
        Console.WriteLine($"Sum: {sum}");
    }
}`,
    },
  },
};

export default function CodeWorkstation() {
  const [activeTool, setActiveTool] = useState<Tool>('sandpack');
  const [activeTemplate, setActiveTemplate] = useState<Template>('react');

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
          <iframe
            src="https://vscode.dev/github/SpiralCloudOmega/DevTeam6"
            className="w-full h-full rounded-xl border border-white/10"
            title="VS Code Web"
          />
        )}

        {activeTool === 'stackblitz' && (
          <iframe
            src="https://stackblitz.com/github/SpiralCloudOmega/DevTeam6/tree/main/templates/react-starter?embed=1&theme=dark&file=src/App.tsx"
            className="w-full h-full rounded-xl border border-white/10"
            title="StackBlitz"
          />
        )}

        {activeTool === 'sandpack' && (
          <div className="w-full h-full">
            <Sandpack
              template={activeTemplate === 'vue' ? 'vue' : 'react'}
              files={templates[activeTemplate].files}
              theme="dark"
              options={{
                showNavigator: true,
                showTabs: true,
                showLineNumbers: true,
                editorHeight: '100%',
              }}
              customSetup={{
                dependencies: {
                  react: activeTemplate === 'react' ? '^18.2.0' : undefined,
                  'react-dom': activeTemplate === 'react' ? '^18.2.0' : undefined,
                  vue: activeTemplate === 'vue' ? '^3.3.0' : undefined,
                },
              }}
            />
          </div>
        )}
      </div>
    </motion.div>
  );
}
