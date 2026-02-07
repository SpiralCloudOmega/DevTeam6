# Contributing to DevTeam6

Thank you for your interest in contributing to DevTeam6! This guide will help you get started.

---

## 🚀 Quick Start

### 1. Fork and Clone

```bash
# Fork the repository on GitHub, then clone your fork
git clone https://github.com/YOUR_USERNAME/DevTeam6.git
cd DevTeam6
```

### 2. Set Up Development Environment

**📋 Complete Environment Setup Guide**: See [DEVELOPMENT_ENVIRONMENT.md](DEVELOPMENT_ENVIRONMENT.md) for comprehensive setup instructions including all tools, dependencies, and platform-specific guides.

**✅ Verify Your Environment**:
```bash
# Linux/macOS
./scripts/verify-environment.sh

# Windows PowerShell
.\scripts\verify-environment.ps1
```

#### Option A: Docker Compose (Recommended)

```bash
# Copy environment files
cp local-ai/.env.example local-ai/.env
cp app/.env.example app/.env
cp projects/.env.example projects/.env

# Start all services
docker-compose up -d

# View logs
docker-compose logs -f
```

#### Option B: Local Development

**Backend (Python)**:
```bash
cd local-ai
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate
pip install -r requirements.txt
uvicorn api.main:app --reload --port 8000
```

**Frontend (Node.js)**:
```bash
# Main App
cd app
npm install
npm run dev

# Projects Dashboard
cd projects
npm install
npm run dev
```

### 3. Create a Feature Branch

```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/your-bug-fix
```

---

## 📝 Development Guidelines

### Code Style

**Python**:
- Use [Black](https://black.readthedocs.io/) for formatting (line length: 88)
- Use [isort](https://pycqa.github.io/isort/) for import sorting
- Use [flake8](https://flake8.pycqa.org/) for linting
- Use type hints where possible
- Follow [PEP 8](https://peps.python.org/pep-0008/)

```bash
# Format code
black local-ai/
isort local-ai/
flake8 local-ai/
```

**TypeScript/JavaScript**:
- Use [Prettier](https://prettier.io/) for formatting
- Use [ESLint](https://eslint.org/) for linting
- Follow [TypeScript guidelines](https://www.typescriptlang.org/docs/)

```bash
# Format code
npm run format
npm run lint
```

**Commit Messages**:
- Use [Conventional Commits](https://www.conventionalcommits.org/)
- Format: `type(scope): description`
- Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

Examples:
```
feat(api): add embedding endpoint
fix(ui): resolve layout issue in node graph
docs(readme): update installation instructions
test(core): add tests for memory system
```

### Documentation

- Update README.md if you add new features
- Add docstrings to all functions and classes
- Update relevant documentation in `docs/`
- Add code examples for new APIs
- Update CHANGELOG.md

### Testing

**Write Tests**:
- Unit tests for all new functions
- Integration tests for API endpoints
- E2E tests for critical user flows
- Maintain >80% code coverage

**Run Tests**:
```bash
# Python tests
cd local-ai
pytest tests/ --cov=. --cov-report=html

# JavaScript tests
cd app
npm test

# Run all tests
npm run test:all
```

---

## 🔧 Project Structure

```
DevTeam6/
├── local-ai/          # Python backend (FastAPI, RAG, MCP)
│   ├── api/           # API endpoints
│   ├── core/          # Core services (memory, embeddings, RAG)
│   ├── mcp/           # MCP servers
│   ├── tests/         # Backend tests
│   └── requirements.txt
│
├── app/               # Main frontend app (Vue/React)
│   ├── src/
│   ├── tests/
│   └── package.json
│
├── projects/          # Projects dashboard
│   ├── src/
│   └── package.json
│
├── docs/              # Documentation
│   ├── architecture/
│   ├── awesome-copilot/
│   └── *.md
│
├── templates/         # Project templates
│   ├── react-starter/
│   ├── vue3-starter/
│   └── ...
│
├── context7/          # Multi-agent system
│   └── agents.md
│
└── .github/           # GitHub configuration
    ├── workflows/     # CI/CD
    └── agents/        # Agent definitions
```

---

## 🎯 How to Contribute

### Reporting Bugs

1. Check existing [issues](https://github.com/SpiralCloudOmega/DevTeam6/issues)
2. Create a new issue using the bug report template
3. Include:
   - Clear description
   - Steps to reproduce
   - Expected vs actual behavior
   - Screenshots if applicable
   - Environment details (OS, browser, versions)

### Suggesting Features

1. Check existing feature requests
2. Create a new issue using the feature request template
3. Describe:
   - The problem you're trying to solve
   - Your proposed solution
   - Alternative solutions considered
   - Additional context

### Submitting Pull Requests

1. **Before You Start**:
   - Comment on the issue you're working on
   - Discuss major changes beforehand
   - Follow the project roadmap

2. **Make Your Changes**:
   - Write clean, documented code
   - Add tests for new functionality
   - Update documentation
   - Run linters and tests locally

3. **Submit PR**:
   - Fill out the PR template completely
   - Link related issues
   - Request review from maintainers
   - Address review feedback promptly

4. **PR Checklist**:
   - [ ] Code follows style guidelines
   - [ ] Tests added/updated and passing
   - [ ] Documentation updated
   - [ ] Commits follow conventional commits
   - [ ] No merge conflicts
   - [ ] CI/CD checks passing

---

## 🧪 Testing Guidelines

### Backend Tests (Python)

```python
# local-ai/tests/test_memory_system.py
import pytest
from core.memory_system import MemorySystem

@pytest.mark.asyncio
async def test_store_memory():
    """Test storing memory in vector store"""
    memory = MemorySystem()
    await memory.store("test content", metadata={"type": "test"})
    
    results = await memory.search("test content")
    assert len(results) > 0
    assert "test content" in results[0]["content"]
```

### Frontend Tests (TypeScript)

```typescript
// app/tests/component.test.ts
import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'
import MyComponent from '../src/components/MyComponent.vue'

describe('MyComponent', () => {
  it('renders correctly', () => {
    const wrapper = mount(MyComponent)
    expect(wrapper.text()).toContain('Expected Text')
  })
})
```

---

## 🔒 Security

- Never commit sensitive data (.env files, API keys, etc.)
- Review code for security vulnerabilities
- Report security issues privately (see SECURITY.md)
- Use environment variables for configuration
- Follow the principle of least privilege

---

## 📦 Dependencies

### Adding Dependencies

**Python**:
```bash
cd local-ai
pip install package-name
pip freeze > requirements.txt
```

**Node.js**:
```bash
cd app  # or projects
npm install package-name
# package.json and package-lock.json auto-updated
```

**Guidelines**:
- Only add necessary dependencies
- Check for security vulnerabilities
- Prefer well-maintained packages
- Document why dependency is needed

---

## 🎓 Learning Resources

### DevTeam6 Documentation
- [System Overview](docs/architecture/SYSTEM_OVERVIEW.md)
- [AI Orchestration](docs/architecture/AI_ORCHESTRATION.md)
- [Deployment Guide](docs/DEPLOYMENT_GUIDE.md)
- [API Reference](docs/API_REFERENCE.md)

### External Resources
- [FastAPI Documentation](https://fastapi.tiangolo.com/)
- [Vue.js Guide](https://vuejs.org/guide/)
- [ChromaDB Docs](https://docs.trychroma.com/)
- [Ollama Documentation](https://ollama.ai/docs/)

---

## 💬 Communication

### Get Help

- **GitHub Discussions**: Ask questions, share ideas
- **GitHub Issues**: Report bugs, request features
- **Discord/Slack**: Real-time chat with the community
- **Documentation**: Check docs/ directory first

### Code Review Process

1. Maintainers review within 24-48 hours
2. Address feedback constructively
3. Reviewers check:
   - Code quality and style
   - Test coverage
   - Documentation
   - Breaking changes
   - Performance impact

---

## 🏆 Recognition

Contributors are recognized in:
- README.md contributors section
- Release notes
- GitHub contributors page
- Special mentions for significant contributions

---

## 📜 License

By contributing, you agree that your contributions will be licensed under the [MIT License](LICENSE).

---

## ❓ Questions?

- Create a GitHub Discussion
- Ask in the community chat
- Email: contributors@devteam6.example.com

Thank you for contributing to DevTeam6! 🚀
