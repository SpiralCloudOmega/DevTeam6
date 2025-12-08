<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { VueFlow, useVueFlow } from '@vue-flow/core'
import type { NodeMouseEvent } from '@vue-flow/core'
import { Background } from '@vue-flow/background'
import { Controls } from '@vue-flow/controls'
import { MiniMap } from '@vue-flow/minimap'
import NodePalette from '@/components/graph/NodePalette.vue'
import NodeEditor from '@/components/graph/NodeEditor.vue'
import BaseNode from '@/components/nodes/BaseNode.vue'
import { useWorkflowStore } from '@/stores/workflow'
import type { WorkflowNode, NodeCategory } from '@/types'

const workflowStore = useWorkflowStore()
const { onConnect, addEdges, onNodesChange, onEdgesChange, applyNodeChanges, applyEdgeChanges } = useVueFlow()

const showPalette = ref(true)
const showEditor = ref(true)
const selectedNode = ref<any>(null)

// Initialize with sample workflow
onMounted(() => {
  workflowStore.createWorkflow('My First Workflow', 'A sample AI pipeline')
  
  // Add sample nodes
  const startNode: WorkflowNode = {
    id: 'start-1',
    type: 'custom',
    position: { x: 100, y: 200 },
    data: {
      id: 'start-1',
      type: 'start',
      category: 'control' as NodeCategory,
      label: 'Start',
      icon: '▶️',
      color: '#00f0ff',
      inputs: [],
      outputs: [{ id: 'out', label: 'Output', type: 'any' as const }],
      config: {},
    },
  }
  
  const aiNode: WorkflowNode = {
    id: 'ai-1',
    type: 'custom',
    position: { x: 350, y: 200 },
    data: {
      id: 'ai-1',
      type: 'openai',
      category: 'ai' as NodeCategory,
      label: 'GPT-4',
      icon: '🤖',
      color: '#ff00ff',
      inputs: [{ id: 'in', label: 'Prompt', type: 'string' as const, required: true }],
      outputs: [{ id: 'out', label: 'Response', type: 'string' as const }],
      config: { model: 'gpt-4', temperature: 0.7 },
    },
  }
  
  const outputNode: WorkflowNode = {
    id: 'output-1',
    type: 'custom',
    position: { x: 600, y: 200 },
    data: {
      id: 'output-1',
      type: 'console',
      category: 'output' as NodeCategory,
      label: 'Console Output',
      icon: '📤',
      color: '#00ff88',
      inputs: [{ id: 'in', label: 'Data', type: 'any' as const, required: true }],
      outputs: [],
      config: {},
    },
  }
  
  workflowStore.addNode(startNode)
  workflowStore.addNode(aiNode)
  workflowStore.addNode(outputNode)
  
  // Add edges
  workflowStore.addEdge({
    id: 'e1-2',
    source: 'start-1',
    target: 'ai-1',
    sourceHandle: 'out',
    targetHandle: 'in',
    animated: true,
  })
  workflowStore.addEdge({
    id: 'e2-3',
    source: 'ai-1',
    target: 'output-1',
    sourceHandle: 'out',
    targetHandle: 'in',
    animated: true,
  })
})

// Handle connections
onConnect((params) => {
  addEdges([{ ...params, animated: true }])
})

// Handle node changes
onNodesChange((changes) => {
  applyNodeChanges(changes)
})

// Handle edge changes
onEdgesChange((changes) => {
  applyEdgeChanges(changes)
})

function onNodeClick(event: NodeMouseEvent) {
  selectedNode.value = event.node
}

function togglePalette() {
  showPalette.value = !showPalette.value
}

function toggleEditor() {
  showEditor.value = !showEditor.value
}

function runWorkflow() {
  workflowStore.startExecution()
  // Simulate execution
  setTimeout(() => {
    workflowStore.stopExecution()
  }, 2000)
}
</script>

<template>
  <div class="editor-view">
    <!-- Top Bar -->
    <header class="toolbar">
      <div class="toolbar-left">
        <RouterLink to="/" class="back-btn">← Home</RouterLink>
        <span class="separator">|</span>
        <h2>{{ workflowStore.currentWorkflow?.name || 'Untitled Workflow' }}</h2>
      </div>
      <div class="toolbar-center">
        <button class="tool-btn" @click="togglePalette">
          {{ showPalette ? '◀' : '▶' }} Palette
        </button>
        <button class="tool-btn" @click="toggleEditor">
          Editor {{ showEditor ? '▶' : '◀' }}
        </button>
      </div>
      <div class="toolbar-right">
        <button 
          class="run-btn" 
          :class="{ running: workflowStore.isRunning }"
          @click="runWorkflow"
          :disabled="workflowStore.isRunning"
        >
          {{ workflowStore.isRunning ? '⏳ Running...' : '▶ Run' }}
        </button>
      </div>
    </header>

    <!-- Main Content -->
    <div class="editor-content">
      <!-- Left Sidebar - Node Palette -->
      <aside v-if="showPalette" class="sidebar sidebar-left">
        <NodePalette />
      </aside>

      <!-- Graph Canvas -->
      <main class="canvas-container">
        <VueFlow 
          :nodes="workflowStore.nodes" 
          :edges="workflowStore.edges"
          fit-view-on-init
          @node-click="onNodeClick"
        >
          <template #node-custom="nodeProps">
            <BaseNode :data="nodeProps.data" :selected="nodeProps.selected" />
          </template>
          
          <Background pattern-color="#1a1a3a" :gap="20" />
          <Controls />
          <MiniMap />
        </VueFlow>
      </main>

      <!-- Right Sidebar - Node Editor -->
      <aside v-if="showEditor" class="sidebar sidebar-right">
        <NodeEditor :node="selectedNode" />
      </aside>
    </div>
  </div>
</template>

<style scoped>
.editor-view {
  height: 100vh;
  display: flex;
  flex-direction: column;
  background: var(--cyber-dark);
}

.toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 12px 20px;
  background: var(--cyber-mid);
  border-bottom: 1px solid var(--cyber-cyan);
}

.toolbar-left {
  display: flex;
  align-items: center;
  gap: 12px;
}

.back-btn {
  color: var(--cyber-cyan);
  text-decoration: none;
  padding: 6px 12px;
  border-radius: 4px;
  transition: background 0.2s;
}

.back-btn:hover {
  background: rgba(0, 240, 255, 0.1);
}

.separator {
  color: #444;
}

.toolbar h2 {
  margin: 0;
  font-size: 16px;
  color: #fff;
}

.toolbar-center {
  display: flex;
  gap: 8px;
}

.tool-btn {
  padding: 8px 16px;
  background: transparent;
  border: 1px solid #444;
  border-radius: 4px;
  color: #888;
  cursor: pointer;
  transition: all 0.2s;
}

.tool-btn:hover {
  border-color: var(--cyber-cyan);
  color: var(--cyber-cyan);
}

.run-btn {
  padding: 10px 24px;
  background: var(--cyber-green);
  border: none;
  border-radius: 6px;
  color: var(--cyber-dark);
  font-weight: bold;
  cursor: pointer;
  transition: all 0.2s;
}

.run-btn:hover:not(:disabled) {
  box-shadow: 0 0 20px rgba(0, 255, 136, 0.4);
}

.run-btn.running {
  background: var(--cyber-orange);
}

.run-btn:disabled {
  cursor: not-allowed;
  opacity: 0.7;
}

.editor-content {
  flex: 1;
  display: flex;
  overflow: hidden;
}

.sidebar {
  width: 280px;
  background: var(--cyber-mid);
  border: 1px solid rgba(0, 240, 255, 0.2);
  overflow-y: auto;
}

.sidebar-left {
  border-right: 1px solid rgba(0, 240, 255, 0.2);
}

.sidebar-right {
  border-left: 1px solid rgba(0, 240, 255, 0.2);
}

.canvas-container {
  flex: 1;
  position: relative;
}

/* Vue Flow Overrides */
:deep(.vue-flow__minimap) {
  background: var(--cyber-dark);
  border: 1px solid var(--cyber-cyan);
}

:deep(.vue-flow__controls) {
  border: 1px solid var(--cyber-cyan);
  background: var(--cyber-mid);
}

:deep(.vue-flow__controls-button) {
  background: var(--cyber-dark);
  border-bottom: 1px solid var(--cyber-cyan);
  color: var(--cyber-cyan);
}

:deep(.vue-flow__controls-button:hover) {
  background: rgba(0, 240, 255, 0.1);
}
</style>
