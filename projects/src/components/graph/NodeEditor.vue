<script setup lang="ts">
import { computed } from 'vue'

interface Props {
  node: any | null
}

const props = defineProps<Props>()

const nodeData = computed(() => props.node?.data ?? null)
</script>

<template>
  <div class="node-editor">
    <div v-if="nodeData" class="editor-content">
      <!-- Node Header -->
      <div class="editor-header" :style="{ borderColor: nodeData.color }">
        <span class="node-icon">{{ nodeData.icon }}</span>
        <div class="node-title">
          <h3>{{ nodeData.label }}</h3>
          <span class="node-type">{{ nodeData.type }}</span>
        </div>
      </div>

      <!-- Node Category Badge -->
      <div class="category-badge" :style="{ background: nodeData.color + '20', color: nodeData.color }">
        {{ nodeData.category?.toUpperCase() }}
      </div>

      <!-- Node Configuration -->
      <div class="config-section">
        <h4>Configuration</h4>
        
        <div class="config-field">
          <label>Label</label>
          <input type="text" :value="nodeData.label" class="config-input" />
        </div>

        <div v-if="nodeData.type === 'openai'" class="config-field">
          <label>Model</label>
          <select class="config-select" :value="nodeData.config?.model || 'gpt-4'">
            <option value="gpt-4">GPT-4</option>
            <option value="gpt-4-turbo">GPT-4 Turbo</option>
            <option value="gpt-5.1">GPT-5.1</option>
          </select>
        </div>

        <div v-if="nodeData.type === 'openai'" class="config-field">
          <label>Temperature</label>
          <input 
            type="range" 
            min="0" 
            max="2" 
            step="0.1" 
            :value="nodeData.config?.temperature || 0.7" 
            class="config-slider"
          />
          <span class="slider-value">{{ nodeData.config?.temperature || 0.7 }}</span>
        </div>
      </div>

      <!-- Inputs Section -->
      <div v-if="nodeData.inputs?.length" class="ports-section">
        <h4>Inputs</h4>
        <div v-for="port in nodeData.inputs" :key="port.id" class="port-item">
          <span class="port-dot input-dot"></span>
          <span class="port-label">{{ port.label }}</span>
          <span class="port-type">{{ port.type }}</span>
          <span v-if="port.required" class="port-required">*</span>
        </div>
      </div>

      <!-- Outputs Section -->
      <div v-if="nodeData.outputs?.length" class="ports-section">
        <h4>Outputs</h4>
        <div v-for="port in nodeData.outputs" :key="port.id" class="port-item">
          <span class="port-dot output-dot"></span>
          <span class="port-label">{{ port.label }}</span>
          <span class="port-type">{{ port.type }}</span>
        </div>
      </div>

      <!-- Actions -->
      <div class="actions-section">
        <button class="action-btn btn-test">
          ▶ Test Node
        </button>
        <button class="action-btn btn-delete">
          🗑️ Delete
        </button>
      </div>
    </div>

    <div v-else class="empty-state">
      <span class="empty-icon">📋</span>
      <p>Select a node to edit its properties</p>
    </div>
  </div>
</template>

<style scoped>
.node-editor {
  height: 100%;
  display: flex;
  flex-direction: column;
}

.editor-content {
  padding: 16px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.editor-header {
  display: flex;
  align-items: center;
  gap: 12px;
  padding-bottom: 12px;
  border-bottom: 2px solid;
}

.node-icon {
  font-size: 28px;
}

.node-title {
  flex: 1;
}

.node-title h3 {
  margin: 0;
  font-size: 16px;
}

.node-type {
  font-size: 12px;
  color: #666;
}

.category-badge {
  display: inline-block;
  padding: 4px 12px;
  border-radius: 12px;
  font-size: 11px;
  font-weight: bold;
  letter-spacing: 0.5px;
}

.config-section h4,
.ports-section h4 {
  margin: 0 0 12px;
  font-size: 12px;
  color: var(--cyber-cyan);
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.config-field {
  margin-bottom: 12px;
}

.config-field label {
  display: block;
  margin-bottom: 6px;
  font-size: 12px;
  color: #888;
}

.config-input,
.config-select {
  width: 100%;
  padding: 10px 12px;
  background: var(--cyber-dark);
  border: 1px solid rgba(0, 240, 255, 0.3);
  border-radius: 6px;
  color: #fff;
  font-family: inherit;
  font-size: 13px;
}

.config-input:focus,
.config-select:focus {
  outline: none;
  border-color: var(--cyber-cyan);
}

.config-slider {
  width: calc(100% - 50px);
  -webkit-appearance: none;
  height: 6px;
  background: var(--cyber-dark);
  border-radius: 3px;
}

.config-slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 16px;
  height: 16px;
  background: var(--cyber-cyan);
  border-radius: 50%;
  cursor: pointer;
}

.slider-value {
  display: inline-block;
  width: 40px;
  text-align: right;
  font-size: 12px;
  color: var(--cyber-cyan);
}

.ports-section {
  padding-top: 8px;
  border-top: 1px solid rgba(255, 255, 255, 0.1);
}

.port-item {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 8px 0;
  font-size: 13px;
}

.port-dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
}

.input-dot {
  background: var(--cyber-cyan);
}

.output-dot {
  background: var(--cyber-green);
}

.port-label {
  flex: 1;
}

.port-type {
  font-size: 11px;
  color: #666;
  background: rgba(255, 255, 255, 0.1);
  padding: 2px 8px;
  border-radius: 4px;
}

.port-required {
  color: var(--cyber-orange);
  font-weight: bold;
}

.actions-section {
  display: flex;
  gap: 8px;
  padding-top: 16px;
  border-top: 1px solid rgba(255, 255, 255, 0.1);
}

.action-btn {
  flex: 1;
  padding: 10px;
  border: none;
  border-radius: 6px;
  font-family: inherit;
  font-size: 12px;
  cursor: pointer;
  transition: all 0.2s;
}

.btn-test {
  background: var(--cyber-green);
  color: var(--cyber-dark);
}

.btn-test:hover {
  box-shadow: 0 0 15px rgba(0, 255, 136, 0.4);
}

.btn-delete {
  background: transparent;
  border: 1px solid #ff4444;
  color: #ff4444;
}

.btn-delete:hover {
  background: #ff4444;
  color: #fff;
}

.empty-state {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  color: #666;
  text-align: center;
  padding: 40px;
}

.empty-icon {
  font-size: 48px;
  margin-bottom: 16px;
  opacity: 0.5;
}

.empty-state p {
  margin: 0;
  font-size: 14px;
}
</style>
