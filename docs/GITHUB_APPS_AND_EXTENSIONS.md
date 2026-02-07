# 🚀 GitHub Apps & Copilot Extensions

Complete guide to integrating 16 powerful GitHub Apps and Copilot Extensions into DevTeam6 for enhanced CI/CD, development automation, AI assistance, and productivity.

---

## 📋 Table of Contents

- [Overview](#overview)
- [CI/CD & Build Tools](#cicd--build-tools)
- [Development Automation](#development-automation)
- [AI & Performance Tools](#ai--performance-tools)
- [GitHub Copilot Extensions](#github-copilot-extensions)
- [Installation Guide](#installation-guide)
- [Configuration](#configuration)
- [Integration with DevTeam6](#integration-with-devteam6)

---

## 🎯 Overview

This document covers the integration of 16 essential GitHub Apps and Copilot Extensions that enhance DevTeam6's capabilities:

| Category | Apps | Count |
|----------|------|-------|
| **CI/CD & Build** | Azure Pipelines, Google Cloud Build, Restack AI | 3 |
| **Development Automation** | OctaPHP, Boring Cyborg, Semantic PRs, ChatGPTBot | 4 |
| **AI & Performance** | Product Science, Sentry for Copilot | 2 |
| **Copilot Extensions** | M365 Toolkit, Docker, Mermaid, Perplexity, Models, Agentic Search, Pinecone | 7 |

**Total**: 16 Apps/Extensions

---

## 🏗️ CI/CD & Build Tools

### 1. Azure Pipelines

**Description**: Continuously build, test, and deploy to any platform and cloud

**Status**: ✅ Auto-installed by SpiralCloudOmega

**Features**:
- Multi-platform builds (Linux, macOS, Windows)
- Container support
- Parallel jobs
- YAML-based pipelines
- Integration with Azure DevOps

**Use Cases in DevTeam6**:
- Building all 7 template projects
- Testing Python, Node.js, .NET, C++, Rust code
- Deploying to Azure, AWS, GCP
- Docker image builds

**Configuration**: See [`.github/apps/azure-pipelines.yml`](../.github/apps/azure-pipelines.yml)

**Setup**:
```yaml
# azure-pipelines.yml
trigger:
  - main
  - develop

pool:
  vmImage: 'ubuntu-latest'

jobs:
  - job: BuildAndTest
    steps:
      - task: UsePythonVersion@0
        inputs:
          versionSpec: '3.12'
      - script: |
          pip install -r local-ai/requirements.txt
          pytest local-ai/tests/
        displayName: 'Test Python Backend'
      
      - task: NodeTool@0
        inputs:
          versionSpec: '22.x'
      - script: |
          npm install
          npm run test
        displayName: 'Test Frontend'
```

---

### 2. Google Cloud Build

**Description**: Build, test, and deploy in a fast, consistent, and secure manner

**Status**: ✅ Auto-installed by SpiralCloudOmega

**Features**:
- Fast, secure container builds
- Native Docker support
- Artifact Registry integration
- Vulnerability scanning
- Multi-stage builds

**Use Cases in DevTeam6**:
- Building Docker containers (local-ai, frontend apps)
- Deploying to Google Cloud Run
- Building and pushing to Artifact Registry
- Running integration tests

**Configuration**: See [`.github/apps/cloudbuild.yaml`](../.github/apps/cloudbuild.yaml)

**Setup**:
```yaml
# cloudbuild.yaml
steps:
  # Build backend
  - name: 'gcr.io/cloud-builders/docker'
    args:
      - 'build'
      - '-t'
      - 'gcr.io/$PROJECT_ID/devteam6-backend:$COMMIT_SHA'
      - '-f'
      - 'local-ai/Dockerfile'
      - '.'
  
  # Build frontend
  - name: 'gcr.io/cloud-builders/docker'
    args:
      - 'build'
      - '-t'
      - 'gcr.io/$PROJECT_ID/devteam6-frontend:$COMMIT_SHA'
      - '-f'
      - 'app/Dockerfile'
      - '.'
  
  # Push images
  - name: 'gcr.io/cloud-builders/docker'
    args: ['push', 'gcr.io/$PROJECT_ID/devteam6-backend:$COMMIT_SHA']
  - name: 'gcr.io/cloud-builders/docker'
    args: ['push', 'gcr.io/$PROJECT_ID/devteam6-frontend:$COMMIT_SHA']

images:
  - 'gcr.io/$PROJECT_ID/devteam6-backend:$COMMIT_SHA'
  - 'gcr.io/$PROJECT_ID/devteam6-frontend:$COMMIT_SHA'
```

---

### 3. Restack AI

**Description**: Automatically build, test and deploy your Restack AI applications

**Status**: ✅ Auto-installed by SpiralCloudOmega

**Features**:
- AI application CI/CD
- LLM integration testing
- Model deployment automation
- Vector database migrations
- RAG pipeline testing

**Use Cases in DevTeam6**:
- Testing local-ai RAG pipeline
- Deploying Ollama model updates
- ChromaDB schema migrations
- MCP server deployments
- Automated prompt testing

**Configuration**: See [`.github/apps/restack.yml`](../.github/apps/restack.yml)

**Setup**:
```yaml
# .github/workflows/restack-ai.yml
name: Restack AI Deployment

on:
  push:
    paths:
      - 'local-ai/**'
      - 'local-ai/mcp/**'

jobs:
  deploy-ai:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Deploy RAG Pipeline
        uses: restack-ai/deploy-action@v1
        with:
          service: 'devteam6-rag'
          path: './local-ai'
          env-vars: |
            OLLAMA_BASE_URL=${{ secrets.OLLAMA_URL }}
            CHROMA_HOST=${{ secrets.CHROMA_HOST }}
```

---

## 🤖 Development Automation

### 4. OctaPHP

**Description**: A lightweight PHP framework designed for complex web algorithms, highly customizable and extensible

**Features**:
- Lightweight architecture
- Advanced routing
- Custom algorithm support
- High extensibility

**Use Cases in DevTeam6**:
- Could add PHP template to templates directory
- Backend API alternatives
- Legacy system integration

**Note**: While DevTeam6 primarily uses Python/Node.js, OctaPHP can be added as an additional template option.

---

### 5. Boring Cyborg

**Description**: A GitHub bot to automatically label PRs, issues and perform all the boring operations

**Status**: ✅ Auto-installed by SpiralCloudOmega

**Features**:
- Auto-labeling based on file paths
- PR size labeling
- First-time contributor greeting
- Stale issue management
- Auto-assignment

**Use Cases in DevTeam6**:
- Label PRs touching `local-ai/` as `ai`
- Label frontend changes as `frontend`
- Label docs as `documentation`
- Welcome new contributors
- Mark stale issues

**Configuration**: See [`.github/boring-cyborg.yml`](../.github/boring-cyborg.yml)

**Setup**:
```yaml
# .github/boring-cyborg.yml
labelPRBasedOnFilePath:
  ai:
    - local-ai/**/*
    - local-ai/mcp/**/*
  
  frontend:
    - app/**/*
    - projects/**/*
  
  documentation:
    - docs/**/*
    - '**/*.md'
  
  templates:
    - templates/**/*
  
  docker:
    - '**/Dockerfile'
    - 'docker-compose.yml'
  
  ci-cd:
    - .github/workflows/**/*

# PR size labels
labelPRBasedOnSize:
  small:
    maxLines: 100
  medium:
    maxLines: 500
  large:
    maxLines: 1000

# First-time contributor greeting
firstPRWelcomeComment: >
  🎉 Thanks for opening your first PR in DevTeam6! 
  
  A maintainer will review your contribution soon. While you wait:
  - Check out our [Contributing Guide](../CONTRIBUTING.md)
  - Join our [Discussions](https://github.com/SpiralCloudOmega/DevTeam6/discussions)
  - Explore the [Documentation](../docs/README.md)

# Stale issues
daysUntilStale: 60
daysUntilClose: 14
staleLabel: stale
exemptLabels:
  - pinned
  - security
  - roadmap
```

---

### 6. Semantic Pull Requests

**Description**: Write semantic commit messages and let the robots do the versioning and releasing

**Status**: ✅ Auto-installed

**Features**:
- Enforce conventional commits
- Semantic versioning
- Automated changelog
- Release automation
- PR title validation

**Use Cases in DevTeam6**:
- Enforce commit conventions
- Automated version bumps
- Generate CHANGELOG.md
- Create GitHub releases
- Tag releases semantically

**Configuration**: See [`.github/semantic.yml`](../.github/semantic.yml)

**Setup**:
```yaml
# .github/semantic.yml
# Validate PR titles to follow conventional commits
titleOnly: true

# Valid types
types:
  - feat      # New feature
  - fix       # Bug fix
  - docs      # Documentation
  - style     # Formatting
  - refactor  # Code restructuring
  - perf      # Performance
  - test      # Tests
  - build     # Build system
  - ci        # CI configuration
  - chore     # Maintenance
  - revert    # Revert changes

# Scopes (optional)
scopes:
  - ai        # AI/ML components
  - frontend  # Frontend apps
  - backend   # Backend services
  - docker    # Docker/containers
  - docs      # Documentation
  - templates # Project templates
  - mcp       # MCP servers
  - workflows # CI/CD workflows

# Require scope
requireScope: false

# Example valid titles:
# feat(ai): add new RAG pipeline
# fix(frontend): resolve CORS issue
# docs: update installation guide
# ci(docker): optimize build steps
```

---

### 7. ChatGPTBot

**Description**: ChatGPT GitHub App for AI-assisted development

**Status**: ✅ Auto-installed by SpiralCloudOmega

**Features**:
- Code review assistance
- Bug analysis
- Documentation generation
- Test case suggestions
- PR summaries

**Use Cases in DevTeam6**:
- Automated code reviews
- Generate documentation
- Suggest test cases
- Analyze complex PRs
- Answer technical questions

**Commands**:
```bash
# In PR comments:
@chatgpt review this code
@chatgpt explain this function
@chatgpt suggest tests for this
@chatgpt write documentation for this
@chatgpt summarize this PR
```

---

## 🎯 AI & Performance Tools

### 8. Product Science

**Description**: Get help with performance optimization techniques to make your code faster

**Features**:
- Performance profiling
- Bottleneck detection
- Optimization suggestions
- Benchmark comparisons
- Resource usage analysis

**Use Cases in DevTeam6**:
- Optimize Python RAG pipeline
- Frontend bundle size reduction
- Database query optimization
- API response time improvement
- Memory leak detection

**Integration**:
```yaml
# .github/workflows/performance.yml
name: Performance Analysis

on: [pull_request]

jobs:
  analyze:
    runs-on: ubuntu-latest
    steps:
      - uses: product-science/analyze-action@v1
        with:
          targets:
            - 'local-ai/**/*.py'
            - 'app/**/*.ts'
            - 'app/**/*.tsx'
```

---

### 9. Sentry for GitHub Copilot

**Description**: Fix broken code from the GitHub UI

**Features**:
- Error tracking integration
- Stack trace analysis
- Fix suggestions in Copilot
- Performance monitoring
- Issue linking

**Use Cases in DevTeam6**:
- Track production errors
- Get fix suggestions in IDE
- Link errors to commits
- Monitor API performance
- Catch bugs early

**Setup**:
```bash
# Install Sentry SDK
pip install sentry-sdk

# In local-ai/api/main.py
import sentry_sdk

sentry_sdk.init(
    dsn="your-dsn-here",
    traces_sample_rate=1.0,
    environment="production"
)
```

**Copilot Integration**:
1. Install Sentry extension in GitHub Copilot
2. Errors appear with fix suggestions
3. Click to apply suggested fixes

---

## 🔌 GitHub Copilot Extensions

### 10. Microsoft 365 Agents Toolkit

**Description**: Build applications and agents for Microsoft 365

**Features**:
- M365 API integration
- Teams bot development
- SharePoint integration
- Office add-ins
- Agent orchestration

**Use Cases in DevTeam6**:
- Create Teams integration
- Build M365 agents
- Office automation
- SharePoint connector
- Calendar integration

**Activation**:
```bash
# In GitHub Copilot Chat:
@microsoft365 create teams bot
@microsoft365 integrate with sharepoint
@microsoft365 build office add-in
```

---

### 11. Docker for GitHub Copilot

**Description**: Learn containerization, generate Docker assets, analyze vulnerabilities

**Status**: ✅ Auto-installed by SpiralCloudOmega

**Features**:
- Dockerfile generation
- docker-compose creation
- Vulnerability scanning
- Best practices
- Multi-stage builds

**Use Cases in DevTeam6**:
- Generate Dockerfiles for templates
- Optimize existing containers
- Security scanning
- Compose file improvements
- Build optimization

**Activation**:
```bash
# In GitHub Copilot Chat:
@docker create dockerfile for python fastapi
@docker optimize this dockerfile
@docker scan for vulnerabilities
@docker generate docker-compose
@docker multi-stage build for node app
```

**Example**:
```bash
User: @docker create dockerfile for local-ai service

Copilot: Here's an optimized Dockerfile for your FastAPI service:

FROM python:3.12-slim as builder
WORKDIR /app
COPY requirements.txt .
RUN pip install --user --no-cache-dir -r requirements.txt

FROM python:3.12-slim
WORKDIR /app
COPY --from=builder /root/.local /root/.local
COPY . .
ENV PATH=/root/.local/bin:$PATH
EXPOSE 8000
CMD ["uvicorn", "main:app", "--host", "0.0.0.0"]
```

---

### 12. Mermaid Chart

**Description**: Advanced diagramming and visualization in GitHub Copilot Chat

**Features**:
- Generate Mermaid diagrams
- Flowcharts, sequence diagrams
- Architecture visualization
- State machines
- Gantt charts

**Use Cases in DevTeam6**:
- Architecture diagrams
- Workflow visualization
- Database schemas
- Component relationships
- Process flows

**Activation**:
```bash
# In GitHub Copilot Chat:
@mermaid create architecture diagram
@mermaid sequence diagram for rag pipeline
@mermaid flowchart for user authentication
@mermaid entity relationship diagram
```

**Example**:
```bash
User: @mermaid create diagram for DevTeam6 RAG pipeline

Copilot:
%%{init: {'theme': 'dark'}}%%
graph TB
    User[User Query] --> API[FastAPI]
    API --> Embed[Embedding Model]
    Embed --> Chroma[ChromaDB]
    Chroma --> Context[Retrieved Context]
    Context --> LLM[Ollama LLM]
    LLM --> Response[Generated Response]
    Response --> User
```

---

### 13. PerplexityAI

**Description**: Answer questions as you code by searching the web

**Features**:
- Real-time web search
- Up-to-date information
- Code examples from web
- API documentation lookup
- Stack Overflow integration

**Use Cases in DevTeam6**:
- Latest framework updates
- API documentation
- Best practices
- Error resolution
- Package comparisons

**Activation**:
```bash
# In GitHub Copilot Chat:
@perplexity what's the latest ollama version?
@perplexity best practices for fastapi async
@perplexity chromadb vs pinecone comparison
@perplexity fix this error: [paste error]
```

---

### 14. Models (GitHub)

**Description**: Connect and chat with GitHub Models

**Features**:
- Access to multiple LLMs
- Model comparison
- Prompt testing
- Token usage tracking
- Response comparison

**Available Models**:
- GPT-4, GPT-3.5
- Claude 3 Opus, Sonnet
- Llama 3
- Mistral
- And more

**Use Cases in DevTeam6**:
- Test prompts across models
- Compare model outputs
- Select best model for task
- Cost optimization
- Performance benchmarking

**Activation**:
```bash
# In GitHub Copilot Chat:
@models use gpt-4 to explain this code
@models compare responses from claude and gpt
@models test this prompt: [your prompt]
```

---

### 15. Agentic Search AI

**Description**: Leverage agentic systems to enhance coding experience

**Features**:
- Multi-step reasoning
- Context-aware search
- Task decomposition
- Tool use
- Memory persistence

**Use Cases in DevTeam6**:
- Complex problem solving
- Research assistance
- Code architecture
- Debugging workflows
- Integration planning

**Activation**:
```bash
# In GitHub Copilot Chat:
@agentic plan integration of new feature
@agentic research best approach for [task]
@agentic debug this complex issue
@agentic architect solution for [requirement]
```

---

### 16. Pinecone AI

**Description**: Build AI applications with vector databases

**Features**:
- Vector database integration
- Embedding generation
- Similarity search
- Hybrid search
- Metadata filtering

**Use Cases in DevTeam6**:
- Integrate with RAG pipeline
- Compare with ChromaDB
- Semantic search
- Recommendation systems
- Duplicate detection

**Activation**:
```bash
# In GitHub Copilot Chat:
@pinecone create vector index
@pinecone implement semantic search
@pinecone migrate from chromadb
@pinecone optimize retrieval
```

**Integration Example**:
```python
from pinecone import Pinecone

# Initialize
pc = Pinecone(api_key="your-key")
index = pc.Index("devteam6-docs")

# Upsert vectors
index.upsert(vectors=[
    {"id": "doc1", "values": embedding, "metadata": {"text": "..."}}
])

# Query
results = index.query(
    vector=query_embedding,
    top_k=5,
    include_metadata=True
)
```

---

## 📥 Installation Guide

### Installing GitHub Apps

1. **Navigate to GitHub Marketplace**
   ```
   https://github.com/marketplace
   ```

2. **Search for App**
   - Search by name (e.g., "Azure Pipelines")

3. **Install on Repository**
   - Click "Set up a plan"
   - Select "Free" or appropriate plan
   - Choose repositories to install on
   - Authorize the app

4. **Configure**
   - Add configuration file to `.github/apps/`
   - Follow app-specific setup

### Installing Copilot Extensions

1. **Open GitHub Copilot Chat**
   - In VS Code, open Copilot Chat panel
   - Or use GitHub web interface

2. **Browse Extensions**
   ```
   Settings → GitHub Copilot → Extensions
   ```

3. **Install Extension**
   - Click extension name
   - Click "Install"
   - Authorize permissions

4. **Verify Installation**
   ```bash
   # In Copilot Chat:
   @docker help  # Should respond
   @mermaid help
   @perplexity help
   ```

---

## ⚙️ Configuration

### Directory Structure

```
.github/
├── apps/                      # GitHub Apps configurations
│   ├── azure-pipelines.yml
│   ├── cloudbuild.yaml
│   ├── boring-cyborg.yml
│   ├── semantic.yml
│   └── restack.yml
│
├── workflows/                 # GitHub Actions
│   ├── ci.yml
│   ├── performance.yml
│   └── restack-ai.yml
│
└── copilot/                   # Copilot settings
    └── extensions.json
```

### Global Configuration

Create `.github/copilot/extensions.json`:

```json
{
  "extensions": {
    "docker": {
      "enabled": true,
      "autoSuggest": true
    },
    "mermaid": {
      "enabled": true,
      "theme": "dark"
    },
    "perplexity": {
      "enabled": true,
      "searchDepth": "detailed"
    },
    "models": {
      "enabled": true,
      "defaultModel": "gpt-4"
    },
    "microsoft365": {
      "enabled": true,
      "features": ["teams", "sharepoint"]
    },
    "pinecone": {
      "enabled": true,
      "environment": "production"
    },
    "agentic": {
      "enabled": true,
      "maxSteps": 10
    }
  }
}
```

---

## 🔗 Integration with DevTeam6

### RAG Pipeline Integration

```python
# local-ai/integrations/pinecone_integration.py
from pinecone import Pinecone
from chromadb import Client as ChromaClient

class VectorStoreManager:
    """Manage multiple vector stores"""
    
    def __init__(self):
        self.chroma = ChromaClient()
        self.pinecone = Pinecone(api_key=os.getenv("PINECONE_API_KEY"))
    
    def hybrid_search(self, query: str, top_k: int = 5):
        """Search across both ChromaDB and Pinecone"""
        chroma_results = self.chroma.query(query, top_k)
        pinecone_results = self.pinecone.query(query, top_k)
        
        return self.merge_results(chroma_results, pinecone_results)
```

### CI/CD Integration

```yaml
# .github/workflows/comprehensive-ci.yml
name: Comprehensive CI

on: [push, pull_request]

jobs:
  # Azure Pipelines
  azure-build:
    uses: ./.github/apps/azure-pipelines.yml
  
  # Google Cloud Build
  gcp-build:
    uses: ./.github/apps/cloudbuild.yaml
  
  # Restack AI
  ai-deploy:
    if: github.ref == 'refs/heads/main'
    uses: ./.github/apps/restack.yml
  
  # Performance Analysis
  performance:
    runs-on: ubuntu-latest
    steps:
      - uses: product-science/analyze-action@v1
```

### Automation Integration

All automation apps (Boring Cyborg, Semantic PRs) work automatically once configured. No code changes needed.

---

## 📊 Benefits Summary

| App/Extension | Primary Benefit | DevTeam6 Impact |
|---------------|-----------------|-----------------|
| Azure Pipelines | Multi-platform CI/CD | Test all 7 templates |
| Google Cloud Build | Fast container builds | Docker optimization |
| Restack AI | AI deployment automation | RAG pipeline deployment |
| OctaPHP | PHP framework | Additional template option |
| Boring Cyborg | Auto-labeling | Reduced manual work |
| Semantic PRs | Automated versioning | Clean releases |
| ChatGPTBot | AI code review | Better code quality |
| Product Science | Performance optimization | Faster execution |
| Sentry | Error tracking | Fewer bugs |
| M365 Toolkit | Microsoft integration | Teams/Office support |
| Docker Extension | Container expertise | Better Dockerfiles |
| Mermaid | Visual diagrams | Clear architecture |
| Perplexity | Web knowledge | Up-to-date info |
| GitHub Models | Multi-LLM access | Best model selection |
| Agentic Search | Complex reasoning | Better solutions |
| Pinecone | Vector database | Enhanced RAG |

---

## 🎯 Quick Start Checklist

- [ ] Install Azure Pipelines for CI/CD
- [ ] Install Google Cloud Build for containers
- [ ] Install Boring Cyborg for automation
- [ ] Install Semantic PR for versioning
- [ ] Enable Docker extension in Copilot
- [ ] Enable Mermaid extension in Copilot
- [ ] Enable Perplexity for web search
- [ ] Enable GitHub Models for LLM access
- [ ] Enable Pinecone for vector DB
- [ ] Configure all apps in `.github/apps/`
- [ ] Test each extension
- [ ] Update team documentation

---

## 📚 Additional Resources

- [GitHub Apps Marketplace](https://github.com/marketplace)
- [Copilot Extensions](https://github.com/features/copilot/extensions)
- [Azure Pipelines Docs](https://docs.microsoft.com/azure/devops/pipelines)
- [Google Cloud Build Docs](https://cloud.google.com/build/docs)
- [Sentry Docs](https://docs.sentry.io)
- [Pinecone Docs](https://docs.pinecone.io)

---

## 🤝 Contributing

Found issues with any integration? Want to add more apps?

1. Open an issue
2. Submit a PR with configuration
3. Update this documentation
4. Share your experience

---

*Last Updated: 2026-02-07*
*DevTeam6 - Powered by 16+ GitHub Apps & Extensions*
