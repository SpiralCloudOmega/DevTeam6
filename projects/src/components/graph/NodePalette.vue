<script setup lang="ts">
import { ref } from 'vue'
import type { NodeCategory } from '@/types'

interface NodeTypeItem {
  type: string
  label: string
  icon: string
  description: string
}

interface NodeCategoryGroup {
  id: NodeCategory
  label: string
  color: string
  nodes: NodeTypeItem[]
}

const categories: NodeCategoryGroup[] = [
  {
    id: 'data',
    label: 'Data',
    color: '#00f0ff',
    nodes: [
      { type: 'http', label: 'HTTP Request', icon: '🌐', description: 'Fetch data from APIs' },
      { type: 'file', label: 'File Reader', icon: '📁', description: 'Read local files' },
      { type: 'database', label: 'Database', icon: '🗄️', description: 'Query databases' },
      { type: 'websocket', label: 'WebSocket', icon: '🔌', description: 'Real-time data streams' },
      { type: 'rss', label: 'RSS Feed', icon: '📰', description: 'Parse RSS feeds' },
      { type: 'scraper', label: 'Web Scraper', icon: '🕷️', description: 'Scrape web pages' },
    ],
  },
  {
    id: 'ai',
    label: 'AI',
    color: '#ff00ff',
    nodes: [
      { type: 'openai', label: 'OpenAI GPT', icon: '🤖', description: 'GPT-4, GPT-5.1' },
      { type: 'claude', label: 'Claude', icon: '🧠', description: 'Anthropic Claude' },
      { type: 'gemini', label: 'Gemini', icon: '✨', description: 'Google Gemini' },
      { type: 'ollama', label: 'Ollama', icon: '🦙', description: 'Local LLM inference' },
      { type: 'whisper', label: 'Whisper', icon: '🎤', description: 'Speech to text' },
      { type: 'dalle', label: 'DALL-E', icon: '🎨', description: 'Image generation' },
      { type: 'vision', label: 'Vision', icon: '👁️', description: 'Image analysis' },
      { type: 'embedding', label: 'Embedding', icon: '📊', description: 'Text embeddings' },
    ],
  },
  {
    id: 'vector',
    label: 'Vector',
    color: '#00ff88',
    nodes: [
      { type: 'pinecone', label: 'Pinecone', icon: '🌲', description: 'Vector database' },
      { type: 'chroma', label: 'Chroma', icon: '🎨', description: 'Local vector store' },
      { type: 'weaviate', label: 'Weaviate', icon: '🔷', description: 'Graph + vector DB' },
      { type: 'similarity', label: 'Similarity', icon: '🔍', description: 'Semantic search' },
    ],
  },
  {
    id: 'agent',
    label: 'Agent',
    color: '#7b2fff',
    nodes: [
      { type: 'master-agent', label: 'Master', icon: '🎯', description: 'Orchestrator agent' },
      { type: 'react-agent', label: 'React', icon: '⚛️', description: 'Frontend specialist' },
      { type: 'python-agent', label: 'Python', icon: '🐍', description: 'Backend/ML specialist' },
      { type: 'devops-agent', label: 'DevOps', icon: '🔧', description: 'Infrastructure agent' },
      { type: 'research-agent', label: 'Research', icon: '📚', description: 'Documentation agent' },
    ],
  },
  {
    id: 'transform',
    label: 'Transform',
    color: '#ff6600',
    nodes: [
      { type: 'json', label: 'JSON', icon: '{ }', description: 'Parse/stringify JSON' },
      { type: 'filter', label: 'Filter', icon: '🔎', description: 'Filter data' },
      { type: 'map', label: 'Map', icon: '🗺️', description: 'Transform items' },
      { type: 'merge', label: 'Merge', icon: '🔀', description: 'Combine data' },
      { type: 'split', label: 'Split', icon: '✂️', description: 'Split data' },
    ],
  },
  {
    id: 'output',
    label: 'Output',
    color: '#ffcc00',
    nodes: [
      { type: 'console', label: 'Console', icon: '📤', description: 'Log to console' },
      { type: 'file-write', label: 'File Write', icon: '💾', description: 'Save to file' },
      { type: 'webhook', label: 'Webhook', icon: '🪝', description: 'Send webhook' },
      { type: 'notification', label: 'Notify', icon: '🔔', description: 'Send notification' },
    ],
  },
  {
    id: 'control',
    label: 'Control',
    color: '#00f0ff',
    nodes: [
      { type: 'start', label: 'Start', icon: '▶️', description: 'Workflow entry' },
      { type: 'end', label: 'End', icon: '⏹️', description: 'Workflow exit' },
      { type: 'condition', label: 'If/Else', icon: '🔀', description: 'Conditional branch' },
      { type: 'loop', label: 'Loop', icon: '🔄', description: 'Iterate items' },
      { type: 'delay', label: 'Delay', icon: '⏱️', description: 'Wait interval' },
    ],
  },
]

const expandedCategories = ref<Set<NodeCategory>>(new Set(['data', 'ai']))
const searchQuery = ref('')

function toggleCategory(id: NodeCategory) {
  if (expandedCategories.value.has(id)) {
    expandedCategories.value.delete(id)
  } else {
    expandedCategories.value.add(id)
  }
}

function onDragStart(event: DragEvent, node: NodeTypeItem, category: NodeCategoryGroup) {
  if (event.dataTransfer) {
    event.dataTransfer.setData('application/json', JSON.stringify({
      type: node.type,
      label: node.label,
      icon: node.icon,
      category: category.id,
      color: category.color,
    }))
    event.dataTransfer.effectAllowed = 'move'
  }
}

function filteredNodes(nodes: NodeTypeItem[]) {
  if (!searchQuery.value) return nodes
  const query = searchQuery.value.toLowerCase()
  return nodes.filter(n => 
    n.label.toLowerCase().includes(query) || 
    n.description.toLowerCase().includes(query)
  )
}
</script>

<template>
  <div class="node-palette">
    <div class="palette-header">
      <h3>Node Palette</h3>
      <input 
        v-model="searchQuery"
        type="text" 
        placeholder="Search nodes..."
        class="search-input"
      />
    </div>

    <div class="categories">
      <div 
        v-for="category in categories" 
        :key="category.id"
        class="category"
      >
        <button 
          class="category-header"
          :style="{ '--cat-color': category.color }"
          @click="toggleCategory(category.id)"
        >
          <span class="category-indicator" :class="{ expanded: expandedCategories.has(category.id) }">
            ▶
          </span>
          <span class="category-label">{{ category.label }}</span>
          <span class="category-count">{{ category.nodes.length }}</span>
        </button>

        <div v-if="expandedCategories.has(category.id)" class="node-list">
          <div
            v-for="node in filteredNodes(category.nodes)"
            :key="node.type"
            class="node-item"
            :style="{ '--node-color': category.color }"
            draggable="true"
            @dragstart="onDragStart($event, node, category)"
          >
            <span class="node-icon">{{ node.icon }}</span>
            <div class="node-info">
              <span class="node-label">{{ node.label }}</span>
              <span class="node-description">{{ node.description }}</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.node-palette {
  height: 100%;
  display: flex;
  flex-direction: column;
}

.palette-header {
  padding: 16px;
  border-bottom: 1px solid rgba(0, 240, 255, 0.2);
}

.palette-header h3 {
  margin: 0 0 12px;
  font-size: 14px;
  color: var(--cyber-cyan);
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.search-input {
  width: 100%;
  padding: 10px 12px;
  background: var(--cyber-dark);
  border: 1px solid rgba(0, 240, 255, 0.3);
  border-radius: 6px;
  color: #fff;
  font-family: inherit;
  font-size: 13px;
}

.search-input:focus {
  outline: none;
  border-color: var(--cyber-cyan);
  box-shadow: 0 0 10px rgba(0, 240, 255, 0.2);
}

.search-input::placeholder {
  color: #666;
}

.categories {
  flex: 1;
  overflow-y: auto;
  padding: 8px;
}

.category {
  margin-bottom: 4px;
}

.category-header {
  width: 100%;
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 10px 12px;
  background: transparent;
  border: none;
  border-radius: 6px;
  color: #fff;
  cursor: pointer;
  transition: background 0.2s;
}

.category-header:hover {
  background: rgba(255, 255, 255, 0.05);
}

.category-indicator {
  color: var(--cat-color);
  font-size: 10px;
  transition: transform 0.2s;
}

.category-indicator.expanded {
  transform: rotate(90deg);
}

.category-label {
  flex: 1;
  text-align: left;
  font-weight: bold;
  color: var(--cat-color);
}

.category-count {
  font-size: 11px;
  color: #666;
  background: rgba(255, 255, 255, 0.1);
  padding: 2px 8px;
  border-radius: 10px;
}

.node-list {
  padding: 4px 0 8px 20px;
}

.node-item {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 10px 12px;
  background: var(--cyber-dark);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-left: 3px solid var(--node-color);
  border-radius: 6px;
  margin-bottom: 6px;
  cursor: grab;
  transition: all 0.2s;
}

.node-item:hover {
  background: rgba(0, 240, 255, 0.05);
  border-color: var(--node-color);
  transform: translateX(4px);
}

.node-item:active {
  cursor: grabbing;
}

.node-icon {
  font-size: 18px;
}

.node-info {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 2px;
}

.node-label {
  font-size: 13px;
  font-weight: bold;
}

.node-description {
  font-size: 11px;
  color: #666;
}
</style>
