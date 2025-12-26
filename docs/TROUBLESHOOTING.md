# 🔧 DevTeam6 Troubleshooting Guide

## Common Issues & Solutions

---

## 🚨 Local AI Backend Issues

### Issue: Ollama Not Responding

**Symptoms**:
- `Connection refused` on port 11434
- "Failed to connect to Ollama" errors
- Empty model list

**Solutions**:

```bash
# Check if Ollama is running
curl http://localhost:11434/api/tags

# If not running, start Ollama
ollama serve

# Or as background service (Linux/Mac)
sudo systemctl start ollama

# Check process
ps aux | grep ollama

# Restart if needed
pkill ollama && ollama serve
```

**Prevention**: Add Ollama to system startup

---

### Issue: ChromaDB Connection Error

**Symptoms**:
- "Unable to connect to ChromaDB"
- "Collection not found"
- Empty query results

**Solutions**:

```bash
# Check persistence directory
ls -la local-ai/data/chroma

# Reset database
cd local-ai
rm -rf data/chroma

# Reinitialize
python -c "
from core.memory_system import MemorySystem
import asyncio
asyncio.run(MemorySystem().initialize())
"

# Or start fresh with Docker
docker-compose down -v
docker-compose up -d chromadb
```

---

### Issue: Embedding Service Fails

**Symptoms**:
- "Model not found"
- Slow embedding generation
- Out of memory errors

**Solutions**:

```bash
# Ensure embedding model is pulled
ollama pull nomic-embed-text

# Check model exists
ollama list | grep nomic

# Try smaller model if OOM
# Edit local-ai/.env
EMBEDDING_MODEL=all-minilm

# Pull it
ollama pull all-minilm
```

---

### Issue: RAG Queries Return Poor Results

**Symptoms**:
- Irrelevant context retrieved
- Empty responses
- Low quality answers

**Solutions**:

```python
# Check if data is stored
from local_ai.core.memory_system import MemorySystem
memory = MemorySystem()
stats = await memory.get_stats()
print(f"Documents: {stats['count']}")

# If empty, store some data
await memory.store(
    content="Your knowledge base content",
    metadata={"source": "docs"}
)

# Adjust top_k parameter
results = await memory.query(
    query="your query",
    top_k=10  # Try different values
)

# Check similarity scores
for result in results:
    print(f"Score: {result['distance']}")
```

---

## 🎨 Frontend Issues

### Issue: Frontend Won't Start

**Symptoms**:
- `npm run dev` fails
- Port already in use
- Module not found errors

**Solutions**:

```bash
# Clear cache and reinstall
rm -rf node_modules package-lock.json
npm install

# Check for port conflicts
lsof -i :5173
kill -9 <PID>  # if needed

# Try different port
npm run dev -- --port 5174

# Clear Vite cache
rm -rf node_modules/.vite
npm run dev
```

---

### Issue: API Requests Failing (CORS)

**Symptoms**:
- "CORS policy blocked"
- Network errors in console
- 403/401 responses

**Solutions**:

**Backend** (`local-ai/api/main.py`):
```python
from fastapi.middleware.cors import CORSMiddleware

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173", "http://localhost:3000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
```

**Frontend** (`.env`):
```bash
# Ensure correct API URL
VITE_API_URL=http://localhost:8000
```

---

### Issue: Hot Reload Not Working

**Symptoms**:
- Changes not reflecting
- Must manually refresh
- Build seems stuck

**Solutions**:

```bash
# Restart dev server
# Ctrl+C then
npm run dev

# Clear cache
rm -rf node_modules/.vite

# Check file watchers (Linux)
echo fs.inotify.max_user_watches=524288 | sudo tee -a /etc/sysctl.conf
sudo sysctl -p

# Use polling (slower but more reliable)
npm run dev -- --force
```

---

## 🐳 Docker Issues

### Issue: Docker Compose Won't Start

**Symptoms**:
- Services fail to start
- Port conflicts
- Volume permission errors

**Solutions**:

```bash
# Check what's using ports
lsof -i :8000
lsof -i :11434

# Stop conflicting services
docker-compose down

# Remove volumes and restart
docker-compose down -v
docker-compose up -d

# Check logs
docker-compose logs -f

# Rebuild images
docker-compose build --no-cache
docker-compose up -d
```

---

### Issue: Container Out of Memory

**Symptoms**:
- Container restarts frequently
- "Out of memory" in logs
- Slow performance

**Solutions**:

```yaml
# docker-compose.yml
services:
  local-ai:
    mem_limit: 4g
    memswap_limit: 4g
    
  ollama:
    mem_limit: 8g  # Increase for larger models
```

```bash
# Or increase Docker Desktop resources
# Docker Desktop > Settings > Resources > Memory

# Check current usage
docker stats
```

---

## 🔨 Build Issues

### Issue: TypeScript Build Errors

**Symptoms**:
- Type errors in build
- Missing declarations
- Cannot find module

**Solutions**:

```bash
# Regenerate type definitions
npm run build

# Check tsconfig.json
cat tsconfig.json

# Clear TypeScript cache
rm -rf node_modules/.cache
npm run dev

# Update TypeScript
npm install -D typescript@latest
```

---

### Issue: Python Import Errors

**Symptoms**:
- `ModuleNotFoundError`
- Import resolution issues
- Circular imports

**Solutions**:

```bash
# Ensure virtual environment is activated
source venv/bin/activate  # Linux/Mac
# OR
venv\Scripts\activate  # Windows

# Reinstall dependencies
pip install -r requirements.txt

# Check Python path
python -c "import sys; print(sys.path)"

# Add to PYTHONPATH if needed
export PYTHONPATH="${PYTHONPATH}:/path/to/DevTeam6/local-ai"
```

---

## 📦 Dependency Issues

### Issue: Conflicting Package Versions

**Symptoms**:
- Installation warnings
- Runtime errors
- Dependency resolution failures

**Solutions**:

```bash
# Check for conflicts
npm ls

# Update problematic packages
npm update package-name

# Force resolution (package.json)
{
  "overrides": {
    "problematic-package": "^desired-version"
  }
}

# Clean install
rm -rf node_modules package-lock.json
npm install
```

---

### Issue: Python Dependency Conflicts

**Symptoms**:
- pip install fails
- Version conflicts
- ImportError at runtime

**Solutions**:

```bash
# Use pip-tools
pip install pip-tools
pip-compile requirements.txt

# Check for conflicts
pip check

# Create fresh environment
python -m venv venv-new
source venv-new/bin/activate
pip install -r requirements.txt

# Pin specific versions
pip freeze > requirements-lock.txt
```

---

## 🌐 Network Issues

### Issue: Cannot Connect to Services

**Symptoms**:
- Timeouts
- Connection refused
- DNS errors

**Solutions**:

```bash
# Check service status
curl http://localhost:8000/health
curl http://localhost:11434/api/tags

# Check if services are listening
netstat -tuln | grep 8000
netstat -tuln | grep 11434

# Test with different host
curl http://127.0.0.1:8000/health
curl http://0.0.0.0:8000/health

# Check firewall
sudo ufw status  # Linux
# Add rule if needed
sudo ufw allow 8000/tcp
```

---

### Issue: Slow API Responses

**Symptoms**:
- Long wait times
- Timeouts
- UI feels sluggish

**Solutions**:

```python
# Enable caching
from local_ai.core.response_cache import ResponseCache
cache = ResponseCache(ttl_minutes=60)

# Use smaller models
# Change in .env
OLLAMA_MODEL=llama3.2:8b  # Instead of 70b

# Batch requests
# Instead of multiple individual calls
results = await batch_process(queries)

# Use streaming for long responses
async for chunk in generate_stream(prompt):
    yield chunk
```

---

## 💾 Data Issues

### Issue: Lost Data After Restart

**Symptoms**:
- Vector DB empty
- Previous queries not found
- Have to re-index

**Solutions**:

```bash
# Check persistence directory
ls -la local-ai/data/chroma

# Ensure CHROMA_PERSIST_DIR is set
cat local-ai/.env | grep CHROMA

# Verify data directory in Docker
docker-compose exec local-ai ls -la /data/chroma

# Check volume mounts
docker-compose config | grep volumes
```

---

### Issue: Database Corruption

**Symptoms**:
- Queries fail
- Cannot add documents
- Unexpected errors

**Solutions**:

```bash
# Backup existing data
cp -r local-ai/data/chroma local-ai/data/chroma.backup

# Reset database
rm -rf local-ai/data/chroma

# Reinitialize
python -c "
from core.memory_system import MemorySystem
import asyncio
asyncio.run(MemorySystem().initialize())
"

# If backup exists, try recovery
# (manual inspection of .sqlite files)
```

---

## 🔐 Permission Issues

### Issue: File Permission Denied

**Symptoms**:
- Cannot write to directory
- Permission errors in logs
- Docker volume issues

**Solutions**:

```bash
# Fix ownership
sudo chown -R $USER:$USER local-ai/data

# Set proper permissions
chmod -R 755 local-ai/data

# Docker volume permissions
docker-compose down
sudo rm -rf volumes/  # if exists
docker-compose up -d
```

---

## 🎯 Performance Issues

### Issue: High CPU Usage

**Symptoms**:
- System slow
- Fans running high
- Ollama using 100% CPU

**Solutions**:

```bash
# Limit Ollama threads
# Set in environment
export OLLAMA_NUM_THREADS=4

# Use smaller model
ollama pull llama3.2:8b  # Instead of 70b

# Reduce batch size
# In Python code
BATCH_SIZE = 10  # Instead of 100

# Monitor resource usage
htop  # or top
docker stats
```

---

### Issue: High Memory Usage

**Symptoms**:
- System swapping
- Out of memory errors
- Browser crashes

**Solutions**:

```bash
# Reduce model size
ollama pull llama3.2:8b

# Limit context window
# In code
max_tokens = 4096  # Instead of 16384

# Close unused services
docker-compose stop service-name

# Increase swap
sudo fallocate -l 8G /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
```

---

## 🧪 Testing Issues

### Issue: Tests Failing

**Symptoms**:
- Pytest failures
- Jest errors
- Timeout issues

**Solutions**:

```bash
# Python tests
cd local-ai
pytest tests/ -v --tb=short

# With coverage
pytest --cov=core tests/

# Specific test
pytest tests/test_memory.py::test_store -v

# Frontend tests
npm test -- --verbose

# Update snapshots
npm test -- -u
```

---

## 📱 Browser Issues

### Issue: UI Not Displaying Correctly

**Symptoms**:
- Layout broken
- Missing styles
- Components not rendering

**Solutions**:

```bash
# Clear browser cache
# Ctrl+Shift+R (hard refresh)

# Check console for errors
# F12 > Console tab

# Disable browser extensions
# Try incognito mode

# Clear localStorage
localStorage.clear()

# Check browser compatibility
# DevTeam6 requires modern browser
# Chrome 90+, Firefox 88+, Safari 14+
```

---

## 🔍 Debugging Tips

### Enable Debug Mode

**Backend**:
```python
# local-ai/api/main.py
import logging
logging.basicConfig(level=logging.DEBUG)
```

**Frontend**:
```javascript
// In browser console
localStorage.setItem('DEBUG', 'true')
```

### Useful Debug Commands

```bash
# Check all services
curl http://localhost:8000/health && \
curl http://localhost:11434/api/tags && \
echo "All services OK"

# Monitor logs in real-time
tail -f local-ai/logs/*.log

# Check environment
env | grep -E "OLLAMA|CHROMA|VITE"

# Network inspection
tcpdump -i lo0 port 8000

# Process inspection
ps aux | grep -E "ollama|python|node"
```

---

## 📞 Getting Help

If you've tried everything:

1. **Check existing issues**: [GitHub Issues](https://github.com/SpiralCloudOmega/DevTeam6/issues)
2. **Search documentation**: Use Ctrl+F in docs
3. **Ask in discussions**: [GitHub Discussions](https://github.com/SpiralCloudOmega/DevTeam6/discussions)
4. **Check Context7**: See `context7/agents.md` for agent notes
5. **Create new issue**: Include error logs and steps to reproduce

---

## 🔄 Emergency Reset

When all else fails:

```bash
#!/bin/bash
# Nuclear option: complete reset
# USE WITH CAUTION - This will delete all data

set -e  # Exit on error

echo "⚠️  WARNING: This will delete all local data!"
read -p "Continue? (yes/no): " confirm
if [ "$confirm" != "yes" ]; then
    echo "Aborted."
    exit 0
fi

echo "Stopping services..."
docker-compose down -v || true
pkill ollama || true
pkill node || true
pkill python || true

echo "Cleaning caches..."
rm -rf node_modules
rm -rf local-ai/data
rm -rf local-ai/.venv
rm -rf .vite
rm -rf dist

echo "Reinstalling dependencies..."
npm install

echo "Setting up Python environment..."
cd local-ai
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
cd ..

echo "✅ Reset complete!"
echo "Start services manually:"
echo "  1. ollama serve"
echo "  2. cd local-ai && uvicorn api.main:app --reload"
echo "  3. npm run dev"
```

---

*Troubleshooting guide for DevTeam6 - Solutions to common issues*
