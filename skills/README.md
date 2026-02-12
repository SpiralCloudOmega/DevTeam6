# DevTeam6 Skills System
# Token-Efficient Heavy Data Storage

This directory contains **skills** - heavy data and code that is stored locally to avoid token costs. Skills are loaded on-demand and cached for reuse.

## 📁 Directory Structure

```
skills/
├── antigravity/          # Antigravity Awesome Skills
│   └── (from https://github.com/sickn33/antigravity-awesome-skills)
├── planning/             # Planning with Files
│   └── (from https://github.com/OthmanAdi/planning-with-files)
├── jules/                # Jules Agent Skills
│   ├── dependencies/     # Cached dependencies
│   ├── environments/     # Virtual environments
│   └── snapshots/        # VM snapshots
├── cache/                # General skill cache
│   ├── code_templates/   # Reusable code templates
│   ├── dependencies/     # npm, pip, cargo caches
│   ├── documentation/    # Cached docs
│   ├── patterns/         # Design patterns
│   └── benchmarks/       # Performance benchmarks
└── custom/               # Custom skills
    └── README.md

```

## 🎯 Purpose: Token Cost Reduction

**Problem**: Sending large context (dependencies, templates, docs) costs tokens  
**Solution**: Store as skills, reference by ID, retrieve only when needed

### Token Savings Example

**Before** (without skills):
```
Prompt: "Use React template with TypeScript, ESLint, Prettier, Testing Library..."
Tokens: ~5,000 (includes full template code)
```

**After** (with skills):
```
Prompt: "Use skill:react-typescript-template"
Tokens: ~50 (just the reference)
Savings: 99% 💰
```

## 📚 Skill Categories

### 1. Code Templates
- React/Vue/Angular starters
- API backends (FastAPI, Express, Spring Boot)
- Database schemas
- CI/CD pipelines

### 2. Dependencies
- node_modules (cached by package.json hash)
- Python venv (cached by requirements.txt)
- Cargo target (cached by Cargo.lock)
- Maven .m2 cache

### 3. Documentation
- API references
- Framework docs
- Best practices
- Architecture patterns

### 4. Patterns
- Design patterns
- Code snippets
- Configuration templates
- Workflow definitions

### 5. Benchmarks
- Performance baselines
- Test results
- Optimization metrics

## 🔧 Usage

### Loading a Skill

```python
from integrations.skills import load_skill

# Load by ID (0 tokens in prompt)
template = load_skill("react-typescript-template")

# Load with parameters
api = load_skill("fastapi-crud", params={"db": "postgresql"})
```

### Creating a Skill

```python
from integrations.skills import create_skill

create_skill(
    id="my-custom-template",
    category="code_templates",
    content=template_code,
    metadata={
        "language": "python",
        "framework": "fastapi",
        "version": "0.1.0"
    }
)
```

### Caching Dependencies

```bash
# Automatically cached by Jules agent
npm install  # → skills/cache/dependencies/npm/<hash>
pip install  # → skills/cache/dependencies/pip/<hash>
cargo build  # → skills/cache/dependencies/cargo/<hash>
```

## 🚀 Integration Points

### With Gemini CLI
- Skills referenced by ID in prompts
- Reduces context size by 70-90%
- Faster responses, lower costs

### With Jules Agent
- Auto-caches dependencies in 20GB VM
- Snapshots for quick restore
- Zero token cost for cached items

### With RAG System
- Skills indexed in ChromaDB
- Semantic search for relevant skills
- Automatic retrieval on demand

### With Conductor
- Reusable workflow templates
- Cached plan structures
- Zero token cost for repeated patterns

## 📦 External Skills

### Antigravity Awesome Skills
- **Repo**: https://github.com/sickn33/antigravity-awesome-skills
- **Location**: `skills/antigravity/`
- **Purpose**: Community-contributed AI skills
- **Auto-update**: Enabled

### Planning with Files
- **Repo**: https://github.com/OthmanAdi/planning-with-files
- **Location**: `skills/planning/`
- **Purpose**: File-based planning methodologies
- **Auto-update**: Enabled

## 🔐 Security

- Skills are sandboxed
- File access restricted to `./` and `/tmp`
- Network access controlled
- Code execution in isolated environments

## 📊 Statistics

Track skill usage and savings:
```bash
# View skill cache statistics
devteam6 skills stats

# Output:
# Total skills: 247
# Cache size: 15.2 GB / 50 GB
# Token savings: 892,441 tokens (estimate)
# Most used: react-typescript-template (142 times)
```

## 🧹 Maintenance

```bash
# Clean old cache entries (>7 days)
devteam6 skills clean

# Rebuild skill index
devteam6 skills reindex

# Validate all skills
devteam6 skills validate
```

## 📖 Learn More

- [Gemini Integration Guide](../docs/GEMINI_INTEGRATION.md)
- [Jules Agent Documentation](../docs/JULES_AGENT.md)
- [Skills Development Guide](../docs/SKILLS_GUIDE.md)
