<script setup lang="ts">
import { RouterLink } from 'vue-router'
</script>

<template>
  <div class="home-view">
    <!-- Header -->
    <header class="header">
      <div class="logo">
        <span class="logo-icon">⚡</span>
        <h1 class="gradient-text">DevTeam6 Projects</h1>
      </div>
      <p class="tagline">Visual Workflow Automation Builder</p>
    </header>

    <!-- Hero Section -->
    <section class="hero">
      <div class="hero-content">
        <h2>Build AI Pipelines <span class="text-cyber-cyan">Visually</span></h2>
        <p>
          Create powerful automation workflows by dragging and connecting nodes.
          Integrate AI models, data sources, and custom logic with ease.
        </p>
        <div class="hero-buttons">
          <RouterLink to="/editor" class="btn btn-primary">
            🚀 Start Building
          </RouterLink>
          <RouterLink to="/mindmap" class="btn btn-secondary">
            🧠 Mindmap Mode
          </RouterLink>
        </div>
      </div>
    </section>

    <!-- Features Grid -->
    <section class="features">
      <h3>Powerful Features</h3>
      <div class="features-grid">
        <div class="feature-card" style="--card-color: var(--cyber-cyan)">
          <span class="feature-icon">🔗</span>
          <h4>n8n-Style Canvas</h4>
          <p>Drag, drop, and connect nodes with an intuitive graph editor</p>
        </div>
        <div class="feature-card" style="--card-color: var(--cyber-magenta)">
          <span class="feature-icon">🤖</span>
          <h4>AI Integration</h4>
          <p>Built-in nodes for GPT, Claude, Gemini, and local Ollama models</p>
        </div>
        <div class="feature-card" style="--card-color: var(--cyber-green)">
          <span class="feature-icon">🧬</span>
          <h4>Vector RAG</h4>
          <p>Semantic search and retrieval with embeddings and knowledge graphs</p>
        </div>
        <div class="feature-card" style="--card-color: var(--cyber-purple)">
          <span class="feature-icon">⚙️</span>
          <h4>30+ Node Types</h4>
          <p>Data, Transform, AI, Agent, Vector, and Output categories</p>
        </div>
        <div class="feature-card" style="--card-color: var(--cyber-orange)">
          <span class="feature-icon">▶️</span>
          <h4>Live Execution</h4>
          <p>Run workflows in real-time with visual progress tracking</p>
        </div>
        <div class="feature-card" style="--card-color: var(--cyber-yellow)">
          <span class="feature-icon">🌐</span>
          <h4>Internet-First</h4>
          <p>Fetch real data from APIs before generating content</p>
        </div>
      </div>
    </section>

    <!-- Node Categories Preview -->
    <section class="categories">
      <h3>Node Categories</h3>
      <div class="category-list">
        <div class="category-item">
          <span class="category-color" style="background: var(--cyber-cyan)"></span>
          <span class="category-label">Data</span>
          <span class="category-count">6 nodes</span>
        </div>
        <div class="category-item">
          <span class="category-color" style="background: var(--cyber-magenta)"></span>
          <span class="category-label">AI</span>
          <span class="category-count">8 nodes</span>
        </div>
        <div class="category-item">
          <span class="category-color" style="background: var(--cyber-green)"></span>
          <span class="category-label">Vector</span>
          <span class="category-count">4 nodes</span>
        </div>
        <div class="category-item">
          <span class="category-color" style="background: var(--cyber-purple)"></span>
          <span class="category-label">Agent</span>
          <span class="category-count">5 nodes</span>
        </div>
        <div class="category-item">
          <span class="category-color" style="background: var(--cyber-orange)"></span>
          <span class="category-label">Transform</span>
          <span class="category-count">5 nodes</span>
        </div>
        <div class="category-item">
          <span class="category-color" style="background: var(--cyber-yellow)"></span>
          <span class="category-label">Output</span>
          <span class="category-count">4 nodes</span>
        </div>
      </div>
    </section>

    <!-- Footer -->
    <footer class="footer">
      <p>DevTeam6 - The Omega Tool Kit</p>
      <a href="/DevTeam6/" class="back-link">← Back to Main</a>
    </footer>
  </div>
</template>

<style scoped>
.home-view {
  min-height: 100vh;
  padding: 20px;
  max-width: 1200px;
  margin: 0 auto;
}

.header {
  text-align: center;
  padding: 40px 0;
}

.logo {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 12px;
  margin-bottom: 10px;
}

.logo-icon {
  font-size: 40px;
}

.logo h1 {
  font-size: 36px;
  margin: 0;
}

.tagline {
  color: #888;
  font-size: 16px;
}

.hero {
  text-align: center;
  padding: 60px 20px;
  background: linear-gradient(135deg, var(--cyber-dark), var(--cyber-mid));
  border-radius: 16px;
  border: 1px solid var(--cyber-cyan);
  margin-bottom: 40px;
}

.hero h2 {
  font-size: 32px;
  margin-bottom: 16px;
}

.hero p {
  color: #aaa;
  max-width: 600px;
  margin: 0 auto 30px;
  line-height: 1.6;
}

.hero-buttons {
  display: flex;
  gap: 16px;
  justify-content: center;
  flex-wrap: wrap;
}

.btn {
  padding: 14px 28px;
  border-radius: 8px;
  text-decoration: none;
  font-weight: bold;
  transition: all 0.2s;
}

.btn-primary {
  background: var(--cyber-cyan);
  color: var(--cyber-dark);
  border: 2px solid var(--cyber-cyan);
}

.btn-primary:hover {
  background: transparent;
  color: var(--cyber-cyan);
  box-shadow: 0 0 20px rgba(0, 240, 255, 0.4);
}

.btn-secondary {
  background: transparent;
  color: var(--cyber-magenta);
  border: 2px solid var(--cyber-magenta);
}

.btn-secondary:hover {
  background: var(--cyber-magenta);
  color: var(--cyber-dark);
  box-shadow: 0 0 20px rgba(255, 0, 255, 0.4);
}

.features {
  margin-bottom: 40px;
}

.features h3 {
  text-align: center;
  margin-bottom: 30px;
  font-size: 24px;
}

.features-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 20px;
}

.feature-card {
  background: linear-gradient(135deg, var(--cyber-dark), var(--cyber-mid));
  border: 1px solid var(--card-color);
  border-radius: 12px;
  padding: 24px;
  transition: all 0.2s;
}

.feature-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 8px 30px rgba(0, 240, 255, 0.2);
}

.feature-icon {
  font-size: 32px;
  display: block;
  margin-bottom: 12px;
}

.feature-card h4 {
  color: var(--card-color);
  margin: 0 0 8px;
}

.feature-card p {
  color: #888;
  margin: 0;
  font-size: 14px;
  line-height: 1.5;
}

.categories {
  background: var(--cyber-mid);
  border-radius: 12px;
  padding: 30px;
  margin-bottom: 40px;
}

.categories h3 {
  text-align: center;
  margin-bottom: 24px;
}

.category-list {
  display: flex;
  flex-wrap: wrap;
  gap: 16px;
  justify-content: center;
}

.category-item {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 12px 20px;
  background: var(--cyber-dark);
  border-radius: 8px;
}

.category-color {
  width: 12px;
  height: 12px;
  border-radius: 50%;
}

.category-label {
  font-weight: bold;
}

.category-count {
  color: #666;
  font-size: 12px;
}

.footer {
  text-align: center;
  padding: 30px;
  color: #666;
}

.back-link {
  color: var(--cyber-cyan);
  text-decoration: none;
}

.back-link:hover {
  text-decoration: underline;
}

.text-cyber-cyan {
  color: var(--cyber-cyan);
}
</style>
