# Open Claude Cowork + Omega: Integration Implementation Guide

## Overview
This guide shows how to integrate Open Claude Cowork with Omega's Kanban orchestrator. The recommended approach is an **HTTP API wrapper** that treats each Kanban task as one Cowork session.

---

## Phase 1: HTTP API Wrapper for Cowork

### Step 1.1: Create HTTP Service Wrapper

**File**: `cowork-server/src/server.ts`
```typescript
import express from 'express';
import { handleClientEvent, sessions } from './ipc-handlers';
import { runClaude } from './libs/runner';
import type { ServerEvent } from './types';

const app = express();
app.use(express.json());

const sessionStreams = new Map<string, express.Response>();

// POST /api/sessions/start
app.post('/api/sessions/start', async (req, res) => {
  const { title, prompt, cwd, allowedTools } = req.body;
  
  try {
    const session = sessions.createSession({ title, prompt, cwd, allowedTools });
    sessions.updateSession(session.id, { status: "running" });
    
    runClaude({
      prompt,
      session,
      onEvent: (event: ServerEvent) => {
        const stream = sessionStreams.get(session.id);
        if (stream) {
          stream.write(`data: ${JSON.stringify(event)}\n\n`);
        }
      }
    }).catch((error) => console.error(error));
    
    res.json({ sessionId: session.id });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

// GET /api/sessions/:sessionId/stream (Server-Sent Events)
app.get('/api/sessions/:sessionId/stream', (req, res) => {
  const { sessionId } = req.params;
  const session = sessions.getSession(sessionId);
  
  if (!session) {
    return res.status(404).json({ error: 'Not found' });
  }
  
  res.setHeader('Content-Type', 'text/event-stream');
  res.setHeader('Cache-Control', 'no-cache');
  res.setHeader('Connection', 'keep-alive');
  
  sessionStreams.set(sessionId, res);
  
  // Send history
  const history = sessions.getSessionHistory(sessionId);
  if (history) {
    for (const message of history.messages) {
      res.write(`data: ${JSON.stringify({
        type: 'stream.message',
        payload: { sessionId, message }
      })}\n\n`);
    }
  }
  
  req.on('close', () => sessionStreams.delete(sessionId));
});

// GET /api/sessions/:sessionId/artifacts
app.get('/api/sessions/:sessionId/artifacts', (req, res) => {
  const { sessionId } = req.params;
  const history = sessions.getSessionHistory(sessionId);
  
  if (!history) return res.status(404).json({ error: 'Not found' });
  
  const files: string[] = [];
  const commands: { name: string; output: string }[] = [];
  
  for (const message of history.messages) {
    if (message.type === 'stream_event') {
      const event = (message as any).event;
      if (event.type === 'tool_result') {
        if (event.tool_name === 'Edit') {
          files.push(event.input.path);
        } else if (event.tool_name === 'Bash') {
          commands.push({ name: event.input.command, output: event.content });
        }
      }
    }
  }
  
  res.json({ sessionId, files, commands });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Cowork API on port ${PORT}`));
```

---

## Phase 2: Omega Integration

### Step 2.1: Cowork Client Adapter

**File**: `omega/adapters/cowork-adapter.ts`
```typescript
import EventSource from 'eventsource';

export class CoworkClient {
  constructor(private apiUrl: string = 'http://localhost:3000') {}
  
  async startSession(config: any): Promise<string> {
    const response = await fetch(`${this.apiUrl}/api/sessions/start`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(config)
    });
    const { sessionId } = await response.json();
    return sessionId;
  }
  
  streamSession(sessionId: string, onEvent: (event: any) => void): Promise<void> {
    return new Promise((resolve, reject) => {
      const es = new EventSource(`${this.apiUrl}/api/sessions/${sessionId}/stream`);
      
      es.addEventListener('message', (event: any) => {
        const serverEvent = JSON.parse(event.data);
        onEvent(serverEvent);
        
        if (serverEvent.type === 'session.status' && 
            ['completed', 'error'].includes(serverEvent.payload.status)) {
          es.close();
          resolve();
        }
      });
      
      es.addEventListener('error', () => {
        es.close();
        reject(new Error('Stream error'));
      });
    });
  }
  
  async getArtifacts(sessionId: string): Promise<any> {
    const response = await fetch(`${this.apiUrl}/api/sessions/${sessionId}/artifacts`);
    return response.json();
  }
}
```

### Step 2.2: Task Executor

**File**: `omega/services/task-executor.ts`
```typescript
import { CoworkClient } from '../adapters/cowork-adapter';

export class TaskExecutor {
  private cowork: CoworkClient;
  
  constructor(private db: Database) {
    this.cowork = new CoworkClient(process.env.COWORK_URL);
  }
  
  async executeTask(board: KanbanBoard, card: KanbanCard): Promise<void> {
    console.log(`Executing: ${card.id}`);
    
    try {
      const sessionId = await this.cowork.startSession({
        title: `[${card.id}] ${card.title}`,
        prompt: card.description,
        cwd: `/workspaces/${board.id}/${card.id}`
      });
      
      await this.db.cards.update(card.id, { coworkSessionId: sessionId });
      
      await this.cowork.streamSession(sessionId, async (event) => {
        if (event.type === 'stream.message') {
          await this.db.cardLogs.create({
            cardId: card.id,
            content: JSON.stringify(event.payload.message)
          });
        }
      });
      
      const artifacts = await this.cowork.getArtifacts(sessionId);
      
      await this.db.artifacts.create({
        cardId: card.id,
        sessionId,
        files: artifacts.files,
        commands: artifacts.commands
      });
      
      console.log(`Completed: ${card.id}`);
    } catch (error) {
      console.error(`Failed: ${card.id}`, error);
      await this.db.cards.update(card.id, { status: 'error' });
    }
  }
}
```

---

## Deployment

### Docker Compose

```yaml
version: '3.9'

services:
  cowork:
    build: ./open-claude-cowork
    ports:
      - "3000:3000"
    environment:
      ANTHROPIC_API_KEY: ${ANTHROPIC_API_KEY}

  omega:
    build: ./omega
    ports:
      - "3001:3001"
    environment:
      COWORK_URL: http://cowork:3000

  fulcrum:
    build: ./fulcrum
    ports:
      - "3002:3002"
    environment:
      OMEGA_URL: http://omega:3001
```

---

## Testing

```typescript
async function test() {
  const client = new CoworkClient('http://localhost:3000');
  
  const sessionId = await client.startSession({
    title: 'Test',
    prompt: 'Write hello.txt',
    cwd: '/tmp/test'
  });
  
  await client.streamSession(sessionId, (event) => {
    console.log(event.type, event.payload);
  });
  
  const artifacts = await client.getArtifacts(sessionId);
  console.log('Files:', artifacts.files);
}
```

