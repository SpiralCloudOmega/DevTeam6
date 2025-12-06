<script setup lang="ts">
import { ref } from 'vue'
import { VueFlow } from '@vue-flow/core'
import { Background } from '@vue-flow/background'
import { Controls } from '@vue-flow/controls'

// Mindmap data structure
const mindmapNodes = ref([
  {
    id: 'root',
    type: 'mindmap',
    position: { x: 400, y: 300 },
    data: { label: 'DevTeam6', color: '#00f0ff', level: 0 },
  },
  {
    id: 'agents',
    type: 'mindmap',
    position: { x: 200, y: 150 },
    data: { label: 'Agents', color: '#ff00ff', level: 1 },
  },
  {
    id: 'projects',
    type: 'mindmap',
    position: { x: 600, y: 150 },
    data: { label: 'Projects', color: '#00ff88', level: 1 },
  },
  {
    id: 'docs',
    type: 'mindmap',
    position: { x: 200, y: 450 },
    data: { label: 'Documentation', color: '#7b2fff', level: 1 },
  },
  {
    id: 'tools',
    type: 'mindmap',
    position: { x: 600, y: 450 },
    data: { label: 'Tools', color: '#ff6600', level: 1 },
  },
  // Level 2 nodes
  {
    id: 'master-agent',
    type: 'mindmap',
    position: { x: 50, y: 80 },
    data: { label: '@master', color: '#00f0ff', level: 2 },
  },
  {
    id: 'react-agent',
    type: 'mindmap',
    position: { x: 50, y: 150 },
    data: { label: '@react', color: '#ff00ff', level: 2 },
  },
  {
    id: 'python-agent',
    type: 'mindmap',
    position: { x: 50, y: 220 },
    data: { label: '@python', color: '#00ff88', level: 2 },
  },
  {
    id: 'vue-flow',
    type: 'mindmap',
    position: { x: 750, y: 80 },
    data: { label: 'Vue Flow', color: '#00f0ff', level: 2 },
  },
  {
    id: 'vector-rag',
    type: 'mindmap',
    position: { x: 750, y: 150 },
    data: { label: 'Vector RAG', color: '#ff00ff', level: 2 },
  },
  {
    id: 'ai-nodes',
    type: 'mindmap',
    position: { x: 750, y: 220 },
    data: { label: 'AI Nodes', color: '#00ff88', level: 2 },
  },
])

const mindmapEdges = ref([
  { id: 'e-root-agents', source: 'root', target: 'agents', animated: true },
  { id: 'e-root-projects', source: 'root', target: 'projects', animated: true },
  { id: 'e-root-docs', source: 'root', target: 'docs', animated: true },
  { id: 'e-root-tools', source: 'root', target: 'tools', animated: true },
  { id: 'e-agents-master', source: 'agents', target: 'master-agent' },
  { id: 'e-agents-react', source: 'agents', target: 'react-agent' },
  { id: 'e-agents-python', source: 'agents', target: 'python-agent' },
  { id: 'e-projects-vue', source: 'projects', target: 'vue-flow' },
  { id: 'e-projects-rag', source: 'projects', target: 'vector-rag' },
  { id: 'e-projects-ai', source: 'projects', target: 'ai-nodes' },
])
</script>

<template>
  <div class="mindmap-view">
    <!-- Header -->
    <header class="mindmap-header">
      <RouterLink to="/" class="back-btn">← Home</RouterLink>
      <h1>🧠 DevTeam6 Mindmap</h1>
      <div class="header-actions">
        <RouterLink to="/editor" class="action-btn">
          ⚡ Switch to Editor
        </RouterLink>
      </div>
    </header>

    <!-- Mindmap Canvas -->
    <div class="mindmap-canvas">
      <VueFlow 
        :nodes="mindmapNodes" 
        :edges="mindmapEdges"
        fit-view-on-init
      >
        <template #node-mindmap="{ data }">
          <div 
            class="mindmap-node"
            :class="`level-${data.level}`"
            :style="{ borderColor: data.color, '--node-color': data.color }"
          >
            <span class="node-label">{{ data.label }}</span>
          </div>
        </template>
        
        <Background pattern-color="#1a1a3a" :gap="30" />
        <Controls />
      </VueFlow>
    </div>

    <!-- Legend -->
    <div class="legend">
      <h4>Node Levels</h4>
      <div class="legend-items">
        <div class="legend-item">
          <span class="legend-dot" style="background: #00f0ff"></span>
          <span>Root / Primary</span>
        </div>
        <div class="legend-item">
          <span class="legend-dot" style="background: #ff00ff"></span>
          <span>Category</span>
        </div>
        <div class="legend-item">
          <span class="legend-dot" style="background: #00ff88"></span>
          <span>Component</span>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.mindmap-view {
  height: 100vh;
  display: flex;
  flex-direction: column;
  background: var(--cyber-dark);
}

.mindmap-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 16px 24px;
  background: var(--cyber-mid);
  border-bottom: 1px solid var(--cyber-magenta);
}

.mindmap-header h1 {
  margin: 0;
  font-size: 20px;
  background: linear-gradient(90deg, var(--cyber-cyan), var(--cyber-magenta));
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}

.back-btn {
  color: var(--cyber-cyan);
  text-decoration: none;
  padding: 8px 16px;
  border-radius: 4px;
  transition: background 0.2s;
}

.back-btn:hover {
  background: rgba(0, 240, 255, 0.1);
}

.action-btn {
  padding: 10px 20px;
  background: var(--cyber-magenta);
  border: none;
  border-radius: 6px;
  color: #fff;
  text-decoration: none;
  font-weight: bold;
  transition: all 0.2s;
}

.action-btn:hover {
  box-shadow: 0 0 20px rgba(255, 0, 255, 0.4);
}

.mindmap-canvas {
  flex: 1;
  position: relative;
}

.mindmap-node {
  padding: 12px 24px;
  background: linear-gradient(135deg, var(--cyber-dark), var(--cyber-mid));
  border: 2px solid var(--node-color);
  border-radius: 24px;
  color: #fff;
  font-weight: bold;
  transition: all 0.2s;
  cursor: pointer;
}

.mindmap-node:hover {
  box-shadow: 0 0 20px var(--node-color);
  transform: scale(1.05);
}

.mindmap-node.level-0 {
  padding: 20px 40px;
  font-size: 18px;
  border-width: 3px;
}

.mindmap-node.level-1 {
  padding: 14px 28px;
  font-size: 14px;
}

.mindmap-node.level-2 {
  padding: 10px 20px;
  font-size: 12px;
  border-radius: 16px;
}

.legend {
  position: fixed;
  bottom: 20px;
  right: 20px;
  background: var(--cyber-mid);
  border: 1px solid var(--cyber-cyan);
  border-radius: 8px;
  padding: 16px;
}

.legend h4 {
  margin: 0 0 12px;
  font-size: 12px;
  color: var(--cyber-cyan);
  text-transform: uppercase;
}

.legend-items {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.legend-item {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 12px;
  color: #888;
}

.legend-dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
}

/* Vue Flow Edge Styles */
:deep(.vue-flow__edge-path) {
  stroke: var(--cyber-cyan);
  stroke-width: 2;
}

:deep(.vue-flow__edge.animated .vue-flow__edge-path) {
  stroke-dasharray: 5;
  animation: flow 1s linear infinite;
}

@keyframes flow {
  from {
    stroke-dashoffset: 10;
  }
  to {
    stroke-dashoffset: 0;
  }
}
</style>
