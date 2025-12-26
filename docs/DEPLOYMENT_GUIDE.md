# 🚀 DevTeam6 Deployment Guide

## Overview

This guide covers deploying DevTeam6 components for both local development and production environments.

---

## 🏠 Local Development Setup

### Prerequisites

**System Requirements**:
- Node.js 18+ (for frontend apps)
- Python 3.10+ (for local-ai backend)
- Docker (optional, for containerized deployment)
- Git

### Quick Start (5 minutes)

```bash
# 1. Clone repository
git clone https://github.com/SpiralCloudOmega/DevTeam6.git
cd DevTeam6

# 2. Install frontend dependencies
npm install

# 3. Set up Python virtual environment for local-ai
cd local-ai
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate
pip install -r requirements.txt

# 4. Install Ollama (for local LLM)
curl -fsSL https://ollama.com/install.sh | sh
ollama pull llama3.2

# 5. Start services
# Terminal 1: Start local AI backend
cd local-ai
uvicorn api.main:app --reload --port 8000

# Terminal 2: Start frontend
cd ../
npm run dev
```

Visit `http://localhost:5173` to see the application.

---

## 🔧 Component-by-Component Setup

### 1. Local AI System

```bash
cd local-ai

# Create .env file
cat > .env << EOF
# Ollama Configuration
OLLAMA_HOST=http://localhost:11434
OLLAMA_MODEL=llama3.2

# Embedding Configuration
EMBEDDING_MODEL=nomic-embed-text
EMBEDDING_DIMENSIONS=768

# ChromaDB
CHROMA_PERSIST_DIR=./data/chroma
CHROMA_COLLECTION=devteam6

# API Settings
API_HOST=127.0.0.1  # Use 0.0.0.0 only with proper firewall/reverse proxy
API_PORT=8000
EOF

# Initialize database
python -c "from core.memory_system import MemorySystem; import asyncio; asyncio.run(MemorySystem().initialize())"

# Start server
uvicorn api.main:app --reload --port 8000
```

**Verify**: Visit `http://localhost:8000/health`

### 2. Frontend Applications

#### React App (Main Workstation)

```bash
cd app

# Install dependencies
npm install

# Start dev server
npm run dev
# Runs on http://localhost:5173
```

#### Vue Projects (Workflow Editor)

```bash
cd projects

# Install dependencies
npm install

# Start dev server
npm run dev
# Runs on http://localhost:5174
```

### 3. Templates

Each template is self-contained:

```bash
cd templates/react-starter
npm install
npm run dev

# Or for C++ workspace
cd templates/cpp-workspace
mkdir build && cd build
cmake ..
make
```

---

## 🐳 Docker Deployment

### Using Docker Compose (Recommended)

Create `docker-compose.yml`:

```yaml
version: '3.8'

services:
  # Local AI Backend
  local-ai:
    build:
      context: ./local-ai
      dockerfile: Dockerfile
    ports:
      - "8000:8000"
    environment:
      - OLLAMA_HOST=http://ollama:11434
      - CHROMA_PERSIST_DIR=/data/chroma
    volumes:
      - ./local-ai/data:/data
    depends_on:
      - ollama
      - chromadb

  # Ollama (Local LLM)
  ollama:
    image: ollama/ollama:latest
    ports:
      - "11434:11434"
    volumes:
      - ollama-data:/root/.ollama

  # ChromaDB (Vector Database)
  chromadb:
    image: chromadb/chroma:latest
    ports:
      - "8001:8000"
    volumes:
      - chroma-data:/chroma/chroma
    environment:
      - IS_PERSISTENT=TRUE

  # Frontend (Production Build)
  frontend:
    build:
      context: .
      dockerfile: Dockerfile.frontend
    ports:
      - "80:80"
    depends_on:
      - local-ai

volumes:
  ollama-data:
  chroma-data:
```

**Dockerfiles**:

`local-ai/Dockerfile`:
```dockerfile
FROM python:3.11-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

CMD ["uvicorn", "api.main:app", "--host", "0.0.0.0", "--port", "8000"]
```

`Dockerfile.frontend`:
```dockerfile
FROM node:18-alpine AS builder

WORKDIR /app
COPY package*.json ./
RUN npm ci

COPY . .
RUN npm run build

FROM nginx:alpine
COPY --from=builder /app/dist /usr/share/nginx/html
COPY nginx.conf /etc/nginx/nginx.conf
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
```

**Deploy**:
```bash
docker-compose up -d
```

---

## ☁️ Cloud Deployment

### GitHub Pages (Frontend Only)

The repository is already configured for GitHub Pages deployment via `.github/workflows/deploy-pages.yml`.

**Manual Deploy**:
```bash
npm run build
npm run deploy
```

**Automatic**: Pushes to `main` branch automatically deploy to GitHub Pages.

**URL**: `https://spiralcloudomega.github.io/DevTeam6/`

### Vercel (Frontend + Serverless Functions)

```bash
# Install Vercel CLI
npm i -g vercel

# Deploy
vercel

# Production deploy
vercel --prod
```

**Configuration** (`vercel.json`):
```json
{
  "buildCommand": "npm run build",
  "outputDirectory": "dist",
  "framework": "vite",
  "rewrites": [
    { "source": "/(.*)", "destination": "/index.html" }
  ]
}
```

### Railway (Full Stack)

```bash
# Install Railway CLI
npm i -g @railway/cli

# Login
railway login

# Initialize project
railway init

# Deploy
railway up
```

**Configuration**: Add `railway.toml` for each service.

### Fly.io (Docker-based)

```bash
# Install flyctl
curl -L https://fly.io/install.sh | sh

# Login
fly auth login

# Create app
fly launch

# Deploy
fly deploy
```

---

## 🔐 Environment Configuration

### Production Environment Variables

```bash
# Local AI Backend
export OLLAMA_HOST=https://your-ollama-instance.com
export OPENAI_API_KEY=sk-xxx  # Optional: for OpenAI fallback
export CHROMA_PERSIST_DIR=/data/chroma
export API_HOST=0.0.0.0
export API_PORT=8000

# Frontend
export VITE_API_URL=https://api.your-domain.com
export VITE_OLLAMA_URL=https://ollama.your-domain.com
```

### Security Checklist

- [ ] Change default passwords
- [ ] Enable HTTPS/SSL
- [ ] Set up CORS properly
- [ ] Use environment variables (never commit secrets)
- [ ] Enable API rate limiting
- [ ] Set up authentication (if public)
- [ ] Configure firewall rules
- [ ] Regular security updates

---

## 📊 Monitoring & Health Checks

### Health Endpoints

```bash
# Local AI Backend
curl http://localhost:8000/health

# Ollama
curl http://localhost:11434/api/tags

# ChromaDB
curl http://localhost:8001/api/v1/heartbeat
```

### Logging

**Backend logs**:
```bash
# View logs
docker-compose logs -f local-ai

# Or direct
tail -f local-ai/logs/app.log
```

**Frontend logs**: Check browser console

### Performance Monitoring

Add to `local-ai/api/main.py`:
```python
from fastapi import FastAPI
from prometheus_fastapi_instrumentator import Instrumentator

app = FastAPI()

# Add Prometheus metrics
Instrumentator().instrument(app).expose(app)
```

Visit `/metrics` for Prometheus-compatible metrics.

---

## 🔄 CI/CD Pipeline

### GitHub Actions (Included)

The repository includes workflows:
- **deploy-pages.yml**: Deploys to GitHub Pages
- **cvs-sync.yml**: Continuous visual synchronization
- **roadmap-sync.yml**: Updates roadmap
- **auto-update.yml**: Auto-updates dependencies

**Custom workflow** (`.github/workflows/deploy.yml`):
```yaml
name: Deploy DevTeam6

on:
  push:
    branches: [main]

jobs:
  deploy-backend:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Deploy to Railway
        run: |
          npm i -g @railway/cli
          railway up --service local-ai

  deploy-frontend:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Build and Deploy
        run: |
          npm ci
          npm run build
          npx vercel --prod
```

---

## 🧪 Testing Deployment

### Smoke Tests

```bash
# Test backend
curl -X POST http://localhost:8000/embed \
  -H "Content-Type: application/json" \
  -d '{"text": "Hello world"}'

# Test RAG
curl -X POST http://localhost:8000/rag \
  -H "Content-Type: application/json" \
  -d '{"query": "What is DevTeam6?"}'

# Test frontend
curl http://localhost:5173
```

### Load Testing

```bash
# Install k6
brew install k6  # or download from k6.io

# Run load test
k6 run - <<EOF
import http from 'k6/http';
import { check } from 'k6';

export default function () {
  let res = http.get('http://localhost:8000/health');
  check(res, { 'status is 200': (r) => r.status === 200 });
}
EOF
```

---

## 🔧 Troubleshooting

### Common Issues

**1. Ollama not responding**
```bash
# Check if running
ollama list

# Restart
ollama serve
```

**2. ChromaDB connection error**
```bash
# Check persistence directory
ls -la local-ai/data/chroma

# Reset database
rm -rf local-ai/data/chroma
python -c "from core.memory_system import MemorySystem; import asyncio; asyncio.run(MemorySystem().initialize())"
```

**3. Frontend can't connect to backend**
- Check CORS configuration in `local-ai/api/main.py`
- Verify API URL in `.env` or environment variables
- Check network/firewall settings

**4. Out of memory**
- Reduce Ollama model size (use smaller models)
- Increase Docker memory limits
- Use pagination for large queries

---

## 📈 Scaling

### Horizontal Scaling

```yaml
# docker-compose.scale.yml
services:
  local-ai:
    deploy:
      replicas: 3
    # Add load balancer
```

### Vertical Scaling

- Increase RAM for Ollama (requires larger models)
- Add GPU support for faster inference
- Use SSD for ChromaDB storage

### Database Optimization

```python
# Increase batch size for embeddings
BATCH_SIZE = 100

# Use approximate search for speed
collection.query(
    query_embeddings=embeddings,
    n_results=10,
    include=["documents", "distances"]
)
```

---

## 🎯 Production Checklist

- [ ] All environment variables configured
- [ ] Secrets stored securely (not in code)
- [ ] HTTPS/SSL enabled
- [ ] CORS configured properly
- [ ] Rate limiting enabled
- [ ] Monitoring/alerting set up
- [ ] Backups configured
- [ ] Error tracking (Sentry/similar)
- [ ] Log aggregation
- [ ] Load balancer configured (if needed)
- [ ] CDN for static assets
- [ ] Database backups automated

---

*Deployment guide for DevTeam6 - From localhost to production*
