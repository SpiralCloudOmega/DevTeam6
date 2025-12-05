// Node Categories
export type NodeCategory = 'data' | 'ai' | 'vector' | 'agent' | 'output' | 'transform' | 'control'

// Node Types
export interface NodeData {
  id: string
  type: string
  category: NodeCategory
  label: string
  description?: string
  icon: string
  color: string
  inputs: NodePort[]
  outputs: NodePort[]
  config: Record<string, unknown>
}

export interface NodePort {
  id: string
  label: string
  type: 'string' | 'number' | 'boolean' | 'object' | 'array' | 'any'
  required?: boolean
}

// Workflow Types
export interface Workflow {
  id: string
  name: string
  description?: string
  nodes: WorkflowNode[]
  edges: WorkflowEdge[]
  createdAt: string
  updatedAt: string
  variables: Record<string, unknown>
}

export interface WorkflowNode {
  id: string
  type: string
  position: { x: number; y: number }
  data: NodeData
}

export interface WorkflowEdge {
  id: string
  source: string
  target: string
  sourceHandle?: string
  targetHandle?: string
  animated?: boolean
}

// Execution Types
export interface ExecutionContext {
  workflowId: string
  status: 'idle' | 'running' | 'paused' | 'completed' | 'error'
  currentNode?: string
  results: Map<string, unknown>
  errors: ExecutionError[]
  startTime?: Date
  endTime?: Date
}

export interface ExecutionError {
  nodeId: string
  message: string
  timestamp: Date
}

// Node Definition Registry
export interface NodeDefinition {
  type: string
  category: NodeCategory
  label: string
  description: string
  icon: string
  color: string
  inputs: NodePort[]
  outputs: NodePort[]
  defaultConfig: Record<string, unknown>
  execute: (inputs: Record<string, unknown>, config: Record<string, unknown>) => Promise<Record<string, unknown>>
}
