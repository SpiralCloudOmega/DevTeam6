import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { Workflow, WorkflowNode, WorkflowEdge, ExecutionContext } from '@/types'

export const useWorkflowStore = defineStore('workflow', () => {
  // State
  const workflows = ref<Workflow[]>([])
  const currentWorkflow = ref<Workflow | null>(null)
  const executionContext = ref<ExecutionContext | null>(null)
  const selectedNodes = ref<string[]>([])

  // Getters
  const nodes = computed(() => currentWorkflow.value?.nodes ?? [])
  const edges = computed(() => currentWorkflow.value?.edges ?? [])
  const isRunning = computed(() => executionContext.value?.status === 'running')

  // Actions
  function createWorkflow(name: string, description?: string): Workflow {
    const workflow: Workflow = {
      id: crypto.randomUUID(),
      name,
      description,
      nodes: [],
      edges: [],
      createdAt: new Date().toISOString(),
      updatedAt: new Date().toISOString(),
      variables: {},
    }
    workflows.value.push(workflow)
    currentWorkflow.value = workflow
    return workflow
  }

  function addNode(node: WorkflowNode): void {
    if (!currentWorkflow.value) return
    currentWorkflow.value.nodes.push(node)
    currentWorkflow.value.updatedAt = new Date().toISOString()
  }

  function updateNode(id: string, updates: Partial<WorkflowNode>): void {
    if (!currentWorkflow.value) return
    const index = currentWorkflow.value.nodes.findIndex(n => n.id === id)
    if (index !== -1) {
      currentWorkflow.value.nodes[index] = {
        ...currentWorkflow.value.nodes[index],
        ...updates,
      }
      currentWorkflow.value.updatedAt = new Date().toISOString()
    }
  }

  function removeNode(id: string): void {
    if (!currentWorkflow.value) return
    currentWorkflow.value.nodes = currentWorkflow.value.nodes.filter(n => n.id !== id)
    currentWorkflow.value.edges = currentWorkflow.value.edges.filter(
      e => e.source !== id && e.target !== id
    )
    currentWorkflow.value.updatedAt = new Date().toISOString()
  }

  function addEdge(edge: WorkflowEdge): void {
    if (!currentWorkflow.value) return
    currentWorkflow.value.edges.push(edge)
    currentWorkflow.value.updatedAt = new Date().toISOString()
  }

  function removeEdge(id: string): void {
    if (!currentWorkflow.value) return
    currentWorkflow.value.edges = currentWorkflow.value.edges.filter(e => e.id !== id)
    currentWorkflow.value.updatedAt = new Date().toISOString()
  }

  function selectNode(id: string, multi = false): void {
    if (multi) {
      if (selectedNodes.value.includes(id)) {
        selectedNodes.value = selectedNodes.value.filter(n => n !== id)
      } else {
        selectedNodes.value.push(id)
      }
    } else {
      selectedNodes.value = [id]
    }
  }

  function clearSelection(): void {
    selectedNodes.value = []
  }

  function startExecution(): void {
    if (!currentWorkflow.value) return
    executionContext.value = {
      workflowId: currentWorkflow.value.id,
      status: 'running',
      results: new Map(),
      errors: [],
      startTime: new Date(),
    }
  }

  function stopExecution(): void {
    if (executionContext.value) {
      executionContext.value.status = 'idle'
      executionContext.value.endTime = new Date()
    }
  }

  return {
    // State
    workflows,
    currentWorkflow,
    executionContext,
    selectedNodes,
    // Getters
    nodes,
    edges,
    isRunning,
    // Actions
    createWorkflow,
    addNode,
    updateNode,
    removeNode,
    addEdge,
    removeEdge,
    selectNode,
    clearSelection,
    startExecution,
    stopExecution,
  }
})
