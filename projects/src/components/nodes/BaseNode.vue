<script setup lang="ts">
import { Handle, Position } from '@vue-flow/core'
import type { NodeData } from '@/types'

interface Props {
  data: NodeData
  selected?: boolean
}

defineProps<Props>()
</script>

<template>
  <div 
    class="base-node"
    :class="{ selected: selected }"
    :style="{ '--node-color': data.color }"
  >
    <!-- Input Handles -->
    <Handle
      v-for="(input, index) in data.inputs"
      :key="`input-${input.id}`"
      type="target"
      :position="Position.Left"
      :id="input.id"
      :style="{ top: `${30 + index * 25}px` }"
      class="handle input-handle"
    />

    <!-- Node Content -->
    <div class="node-header">
      <span class="node-icon">{{ data.icon }}</span>
      <span class="node-label">{{ data.label }}</span>
    </div>

    <div class="node-body">
      <span class="node-type">{{ data.type }}</span>
    </div>

    <!-- Port Labels -->
    <div v-if="data.inputs?.length" class="port-labels input-labels">
      <div v-for="input in data.inputs" :key="input.id" class="port-label">
        {{ input.label }}
      </div>
    </div>

    <div v-if="data.outputs?.length" class="port-labels output-labels">
      <div v-for="output in data.outputs" :key="output.id" class="port-label">
        {{ output.label }}
      </div>
    </div>

    <!-- Output Handles -->
    <Handle
      v-for="(output, index) in data.outputs"
      :key="`output-${output.id}`"
      type="source"
      :position="Position.Right"
      :id="output.id"
      :style="{ top: `${30 + index * 25}px` }"
      class="handle output-handle"
    />
  </div>
</template>

<style scoped>
.base-node {
  background: linear-gradient(135deg, var(--cyber-dark), var(--cyber-mid));
  border: 2px solid var(--node-color);
  border-radius: 8px;
  min-width: 160px;
  padding: 0;
  transition: all 0.2s;
  position: relative;
}

.base-node:hover {
  box-shadow: 0 0 20px var(--node-color);
}

.base-node.selected {
  box-shadow: 0 0 25px var(--node-color), 0 0 40px var(--node-color);
  border-width: 3px;
}

.node-header {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 10px 12px;
  background: rgba(0, 0, 0, 0.3);
  border-radius: 6px 6px 0 0;
  border-bottom: 1px solid var(--node-color);
}

.node-icon {
  font-size: 18px;
}

.node-label {
  font-size: 13px;
  font-weight: bold;
  color: #fff;
}

.node-body {
  padding: 8px 12px;
}

.node-type {
  font-size: 11px;
  color: var(--node-color);
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.port-labels {
  position: absolute;
  display: flex;
  flex-direction: column;
  gap: 10px;
  top: 22px;
  font-size: 10px;
  color: #888;
}

.input-labels {
  left: 16px;
}

.output-labels {
  right: 16px;
  text-align: right;
}

.handle {
  width: 12px !important;
  height: 12px !important;
  border: 2px solid var(--node-color) !important;
  background: var(--cyber-dark) !important;
  transition: all 0.2s;
}

.handle:hover {
  background: var(--node-color) !important;
  box-shadow: 0 0 10px var(--node-color);
  transform: scale(1.2);
}

.input-handle {
  left: -6px !important;
}

.output-handle {
  right: -6px !important;
}
</style>
