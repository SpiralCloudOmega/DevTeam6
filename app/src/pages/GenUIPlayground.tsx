import { useState, useRef, useEffect } from 'react';
import { Link } from 'react-router-dom';

// Pre-built component templates
const componentTemplates: Record<string, { name: string; jsx: string; css: string }> = {
  'card': {
    name: 'Dark Mode Card',
    jsx: `<div className="generated-card">
  <div className="card-glow"></div>
  <h3>✨ Generated Card</h3>
  <p>A sleek dark-mode card component with neon accents.</p>
  <button className="card-btn">Action</button>
</div>`,
    css: `.generated-card {
  position: relative;
  background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 100%);
  border: 1px solid #00f0ff;
  border-radius: 16px;
  padding: 24px;
  overflow: hidden;
}
.card-glow {
  position: absolute;
  top: -50%;
  left: -50%;
  width: 200%;
  height: 200%;
  background: radial-gradient(circle, rgba(0,240,255,0.1) 0%, transparent 70%);
  animation: rotate 8s linear infinite;
}
@keyframes rotate {
  from { transform: rotate(0deg); }
  to { transform: rotate(360deg); }
}
.generated-card h3 {
  color: #00f0ff;
  margin: 0 0 12px;
  font-size: 1.5rem;
}
.generated-card p {
  color: #a0a0a0;
  margin: 0 0 16px;
}
.card-btn {
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  border: none;
  color: white;
  padding: 10px 24px;
  border-radius: 8px;
  cursor: pointer;
  font-weight: bold;
  transition: transform 0.2s, box-shadow 0.2s;
}
.card-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(0,240,255,0.3);
}`
  },
  'button': {
    name: 'Neon Button',
    jsx: `<button className="neon-button">
  <span className="btn-text">Click Me</span>
  <span className="btn-glow"></span>
</button>`,
    css: `.neon-button {
  position: relative;
  background: transparent;
  border: 2px solid #ff00ff;
  color: #ff00ff;
  padding: 14px 32px;
  font-size: 1rem;
  font-weight: bold;
  text-transform: uppercase;
  letter-spacing: 2px;
  cursor: pointer;
  overflow: hidden;
  transition: all 0.3s ease;
}
.neon-button:hover {
  background: #ff00ff;
  color: #0a0a1a;
  box-shadow: 0 0 30px #ff00ff, 0 0 60px #ff00ff;
}
.btn-glow {
  position: absolute;
  top: 0;
  left: -100%;
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, transparent, rgba(255,255,255,0.3), transparent);
  transition: left 0.5s;
}
.neon-button:hover .btn-glow {
  left: 100%;
}`
  },
  'navbar': {
    name: 'Cyberpunk Navbar',
    jsx: `<nav className="cyber-navbar">
  <div className="nav-logo">⚡ DEVTEAM6</div>
  <div className="nav-links">
    <a href="#" className="nav-link">Home</a>
    <a href="#" className="nav-link">Features</a>
    <a href="#" className="nav-link">Docs</a>
    <a href="#" className="nav-link active">Contact</a>
  </div>
  <div className="nav-glow"></div>
</nav>`,
    css: `.cyber-navbar {
  position: relative;
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: rgba(10, 10, 26, 0.95);
  border-bottom: 2px solid #00f0ff;
  padding: 16px 32px;
  backdrop-filter: blur(10px);
}
.nav-logo {
  color: #00f0ff;
  font-size: 1.5rem;
  font-weight: bold;
  text-shadow: 0 0 10px #00f0ff;
}
.nav-links {
  display: flex;
  gap: 24px;
}
.nav-link {
  color: #a0a0a0;
  text-decoration: none;
  font-weight: 500;
  transition: color 0.3s, text-shadow 0.3s;
}
.nav-link:hover, .nav-link.active {
  color: #ff00ff;
  text-shadow: 0 0 10px #ff00ff;
}
.nav-glow {
  position: absolute;
  bottom: 0;
  left: 0;
  right: 0;
  height: 2px;
  background: linear-gradient(90deg, #00f0ff, #ff00ff, #00ff88, #00f0ff);
  background-size: 300% 100%;
  animation: shimmer 3s linear infinite;
}
@keyframes shimmer {
  0% { background-position: 0% 0%; }
  100% { background-position: 300% 0%; }
}`
  },
  'hero': {
    name: 'Hero Section',
    jsx: `<section className="hero-section">
  <div className="hero-bg"></div>
  <div className="hero-content">
    <h1 className="hero-title">Build the <span>Future</span></h1>
    <p className="hero-subtitle">Next-generation tools for modern developers</p>
    <div className="hero-buttons">
      <button className="hero-btn primary">Get Started</button>
      <button className="hero-btn secondary">Learn More</button>
    </div>
  </div>
  <div className="hero-grid"></div>
</section>`,
    css: `.hero-section {
  position: relative;
  min-height: 400px;
  display: flex;
  align-items: center;
  justify-content: center;
  overflow: hidden;
  background: #0a0a1a;
}
.hero-bg {
  position: absolute;
  inset: 0;
  background: radial-gradient(ellipse at center, rgba(123,47,255,0.2) 0%, transparent 70%);
}
.hero-content {
  position: relative;
  text-align: center;
  z-index: 2;
}
.hero-title {
  font-size: 3rem;
  color: white;
  margin: 0 0 16px;
}
.hero-title span {
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}
.hero-subtitle {
  color: #a0a0a0;
  font-size: 1.25rem;
  margin: 0 0 32px;
}
.hero-buttons {
  display: flex;
  gap: 16px;
  justify-content: center;
}
.hero-btn {
  padding: 14px 32px;
  border-radius: 8px;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s;
}
.hero-btn.primary {
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  border: none;
  color: white;
}
.hero-btn.secondary {
  background: transparent;
  border: 2px solid #00f0ff;
  color: #00f0ff;
}
.hero-btn:hover {
  transform: translateY(-3px);
  box-shadow: 0 10px 30px rgba(0,240,255,0.3);
}
.hero-grid {
  position: absolute;
  inset: 0;
  background-image: 
    linear-gradient(rgba(0,240,255,0.03) 1px, transparent 1px),
    linear-gradient(90deg, rgba(0,240,255,0.03) 1px, transparent 1px);
  background-size: 50px 50px;
  z-index: 1;
}`
  },
  'stats': {
    name: 'Stats Dashboard',
    jsx: `<div className="stats-dashboard">
  <div className="stat-card">
    <div className="stat-icon">📊</div>
    <div className="stat-value">1,234</div>
    <div className="stat-label">Total Users</div>
    <div className="stat-trend up">↑ 12.5%</div>
  </div>
  <div className="stat-card">
    <div className="stat-icon">⚡</div>
    <div className="stat-value">99.9%</div>
    <div className="stat-label">Uptime</div>
    <div className="stat-trend up">↑ 0.2%</div>
  </div>
  <div className="stat-card">
    <div className="stat-icon">🚀</div>
    <div className="stat-value">50ms</div>
    <div className="stat-label">Latency</div>
    <div className="stat-trend down">↓ 5ms</div>
  </div>
</div>`,
    css: `.stats-dashboard {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 20px;
}
.stat-card {
  background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 100%);
  border: 1px solid rgba(0,240,255,0.3);
  border-radius: 16px;
  padding: 24px;
  text-align: center;
  transition: transform 0.3s, border-color 0.3s;
}
.stat-card:hover {
  transform: translateY(-5px);
  border-color: #00f0ff;
}
.stat-icon {
  font-size: 2rem;
  margin-bottom: 12px;
}
.stat-value {
  font-size: 2.5rem;
  font-weight: bold;
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}
.stat-label {
  color: #a0a0a0;
  margin: 8px 0;
}
.stat-trend {
  font-size: 0.875rem;
  font-weight: bold;
}
.stat-trend.up { color: #00ff88; }
.stat-trend.down { color: #ff6600; }`
  },
  'form': {
    name: 'Contact Form',
    jsx: `<form className="cyber-form">
  <h2 className="form-title">Get in Touch</h2>
  <div className="form-group">
    <input type="text" placeholder="Your Name" className="form-input" />
    <div className="input-line"></div>
  </div>
  <div className="form-group">
    <input type="email" placeholder="Your Email" className="form-input" />
    <div className="input-line"></div>
  </div>
  <div className="form-group">
    <textarea placeholder="Your Message" className="form-textarea"></textarea>
    <div className="input-line"></div>
  </div>
  <button type="submit" className="form-submit">Send Message ⚡</button>
</form>`,
    css: `.cyber-form {
  max-width: 400px;
  background: rgba(10, 10, 26, 0.9);
  border: 1px solid rgba(0,240,255,0.3);
  border-radius: 16px;
  padding: 32px;
}
.form-title {
  color: #00f0ff;
  text-align: center;
  margin: 0 0 24px;
}
.form-group {
  position: relative;
  margin-bottom: 24px;
}
.form-input, .form-textarea {
  width: 100%;
  background: transparent;
  border: none;
  border-bottom: 2px solid rgba(0,240,255,0.3);
  color: white;
  padding: 12px 0;
  font-size: 1rem;
  outline: none;
  transition: border-color 0.3s;
}
.form-input:focus, .form-textarea:focus {
  border-color: #ff00ff;
}
.form-textarea {
  min-height: 100px;
  resize: vertical;
}
.input-line {
  position: absolute;
  bottom: 0;
  left: 0;
  width: 0;
  height: 2px;
  background: linear-gradient(90deg, #00f0ff, #ff00ff);
  transition: width 0.3s;
}
.form-input:focus + .input-line,
.form-textarea:focus + .input-line {
  width: 100%;
}
.form-submit {
  width: 100%;
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  border: none;
  color: white;
  padding: 14px;
  border-radius: 8px;
  font-size: 1rem;
  font-weight: bold;
  cursor: pointer;
  transition: transform 0.3s, box-shadow 0.3s;
}
.form-submit:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(255,0,255,0.3);
}`
  },
  'modal': {
    name: 'Modal Dialog',
    jsx: `<div className="modal-overlay">
  <div className="modal-container">
    <div className="modal-header">
      <h3>🎉 Welcome!</h3>
      <button className="modal-close">✕</button>
    </div>
    <div className="modal-body">
      <p>This is a beautifully designed modal dialog with cyberpunk aesthetics.</p>
    </div>
    <div className="modal-footer">
      <button className="modal-btn cancel">Cancel</button>
      <button className="modal-btn confirm">Confirm</button>
    </div>
  </div>
</div>`,
    css: `.modal-overlay {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
  background: rgba(0,0,0,0.8);
  padding: 40px;
  border-radius: 16px;
}
.modal-container {
  background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 100%);
  border: 2px solid #00f0ff;
  border-radius: 16px;
  width: 100%;
  max-width: 400px;
  overflow: hidden;
  box-shadow: 0 0 30px rgba(0,240,255,0.2);
}
.modal-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 20px 24px;
  border-bottom: 1px solid rgba(0,240,255,0.3);
}
.modal-header h3 {
  margin: 0;
  color: #00f0ff;
}
.modal-close {
  background: transparent;
  border: none;
  color: #a0a0a0;
  font-size: 1.25rem;
  cursor: pointer;
  transition: color 0.3s;
}
.modal-close:hover {
  color: #ff00ff;
}
.modal-body {
  padding: 24px;
  color: #a0a0a0;
}
.modal-footer {
  display: flex;
  gap: 12px;
  padding: 20px 24px;
  border-top: 1px solid rgba(0,240,255,0.3);
}
.modal-btn {
  flex: 1;
  padding: 12px;
  border-radius: 8px;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s;
}
.modal-btn.cancel {
  background: transparent;
  border: 1px solid #a0a0a0;
  color: #a0a0a0;
}
.modal-btn.confirm {
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  border: none;
  color: white;
}
.modal-btn:hover {
  transform: translateY(-2px);
}`
  },
  'pricing': {
    name: 'Pricing Card',
    jsx: `<div className="pricing-card">
  <div className="pricing-badge">POPULAR</div>
  <h3 className="pricing-title">Pro Plan</h3>
  <div className="pricing-price">
    <span className="currency">$</span>
    <span className="amount">29</span>
    <span className="period">/mo</span>
  </div>
  <ul className="pricing-features">
    <li>✓ Unlimited Projects</li>
    <li>✓ AI Code Generation</li>
    <li>✓ Priority Support</li>
    <li>✓ Custom Integrations</li>
  </ul>
  <button className="pricing-btn">Get Started</button>
</div>`,
    css: `.pricing-card {
  position: relative;
  background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 100%);
  border: 2px solid #ff00ff;
  border-radius: 24px;
  padding: 32px;
  text-align: center;
  max-width: 300px;
}
.pricing-badge {
  position: absolute;
  top: -12px;
  left: 50%;
  transform: translateX(-50%);
  background: linear-gradient(135deg, #ff00ff, #7b2fff);
  color: white;
  padding: 6px 16px;
  border-radius: 20px;
  font-size: 0.75rem;
  font-weight: bold;
}
.pricing-title {
  color: white;
  margin: 16px 0;
  font-size: 1.5rem;
}
.pricing-price {
  margin: 24px 0;
}
.pricing-price .currency {
  color: #00f0ff;
  font-size: 1.5rem;
  vertical-align: top;
}
.pricing-price .amount {
  font-size: 4rem;
  font-weight: bold;
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}
.pricing-price .period {
  color: #a0a0a0;
}
.pricing-features {
  list-style: none;
  padding: 0;
  margin: 24px 0;
  text-align: left;
}
.pricing-features li {
  color: #a0a0a0;
  padding: 8px 0;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.pricing-btn {
  width: 100%;
  background: linear-gradient(135deg, #ff00ff, #7b2fff);
  border: none;
  color: white;
  padding: 14px;
  border-radius: 12px;
  font-weight: bold;
  cursor: pointer;
  transition: transform 0.3s, box-shadow 0.3s;
}
.pricing-btn:hover {
  transform: translateY(-3px);
  box-shadow: 0 10px 30px rgba(255,0,255,0.4);
}`
  },
  'testimonial': {
    name: 'Testimonial Card',
    jsx: `<div className="testimonial-card">
  <div className="testimonial-quote">"</div>
  <p className="testimonial-text">This tool has completely transformed how our team works. The AI-powered features are incredible!</p>
  <div className="testimonial-author">
    <div className="author-avatar">JD</div>
    <div className="author-info">
      <div className="author-name">Jane Doe</div>
      <div className="author-role">Senior Developer @ Tech Corp</div>
    </div>
  </div>
  <div className="testimonial-stars">★★★★★</div>
</div>`,
    css: `.testimonial-card {
  position: relative;
  background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 100%);
  border: 1px solid rgba(0,240,255,0.3);
  border-radius: 20px;
  padding: 32px;
  max-width: 400px;
}
.testimonial-quote {
  position: absolute;
  top: 20px;
  left: 24px;
  font-size: 4rem;
  color: rgba(0,240,255,0.2);
  line-height: 1;
}
.testimonial-text {
  color: #e0e0e0;
  font-size: 1.1rem;
  line-height: 1.6;
  margin: 24px 0;
  font-style: italic;
}
.testimonial-author {
  display: flex;
  align-items: center;
  gap: 16px;
  margin-top: 24px;
}
.author-avatar {
  width: 50px;
  height: 50px;
  border-radius: 50%;
  background: linear-gradient(135deg, #00f0ff, #ff00ff);
  display: flex;
  align-items: center;
  justify-content: center;
  color: white;
  font-weight: bold;
}
.author-name {
  color: white;
  font-weight: bold;
}
.author-role {
  color: #a0a0a0;
  font-size: 0.875rem;
}
.testimonial-stars {
  position: absolute;
  top: 24px;
  right: 24px;
  color: #ffcc00;
  font-size: 1rem;
}`
  }
};

// Keywords to template mapping
const keywordMap: Record<string, string> = {
  'card': 'card',
  'dark': 'card',
  'button': 'button',
  'btn': 'button',
  'click': 'button',
  'nav': 'navbar',
  'navbar': 'navbar',
  'navigation': 'navbar',
  'menu': 'navbar',
  'header': 'navbar',
  'hero': 'hero',
  'landing': 'hero',
  'banner': 'hero',
  'stats': 'stats',
  'dashboard': 'stats',
  'metrics': 'stats',
  'analytics': 'stats',
  'form': 'form',
  'contact': 'form',
  'input': 'form',
  'modal': 'modal',
  'dialog': 'modal',
  'popup': 'modal',
  'pricing': 'pricing',
  'plan': 'pricing',
  'subscription': 'pricing',
  'testimonial': 'testimonial',
  'review': 'testimonial',
  'quote': 'testimonial',
  'feedback': 'testimonial'
};

function parsePrompt(prompt: string): string {
  const lowerPrompt = prompt.toLowerCase();
  
  for (const [keyword, template] of Object.entries(keywordMap)) {
    if (lowerPrompt.includes(keyword)) {
      return template;
    }
  }
  
  // Default to card if no match
  return 'card';
}

export default function GenUIPlayground() {
  const [prompt, setPrompt] = useState('');
  const [isGenerating, setIsGenerating] = useState(false);
  const [generatedComponent, setGeneratedComponent] = useState<{ name: string; jsx: string; css: string } | null>(null);
  const [activeTab, setActiveTab] = useState<'preview' | 'jsx' | 'css'>('preview');
  const [history, setHistory] = useState<Array<{ prompt: string; template: string }>>([]);
  const previewRef = useRef<HTMLDivElement>(null);

  const handleGenerate = async () => {
    if (!prompt.trim()) return;
    
    setIsGenerating(true);
    
    // Simulate AI processing time
    await new Promise(resolve => setTimeout(resolve, 1500));
    
    const templateKey = parsePrompt(prompt);
    const template = componentTemplates[templateKey];
    
    setGeneratedComponent(template);
    setHistory(prev => [...prev, { prompt, template: templateKey }]);
    setIsGenerating(false);
  };

  useEffect(() => {
    if (generatedComponent && previewRef.current) {
      // Inject CSS
      const styleId = 'genui-dynamic-styles';
      let styleEl = document.getElementById(styleId) as HTMLStyleElement;
      if (!styleEl) {
        styleEl = document.createElement('style');
        styleEl.id = styleId;
        document.head.appendChild(styleEl);
      }
      styleEl.textContent = generatedComponent.css;
    }
  }, [generatedComponent]);

  const examplePrompts = [
    'Generate a dark-mode card component',
    'Create a neon button with hover effects',
    'Build a cyberpunk navigation bar',
    'Design a hero section with gradient',
    'Make a stats dashboard with cards',
    'Create a contact form',
    'Build a modal dialog',
    'Design a pricing card',
    'Generate a testimonial component'
  ];

  const copyToClipboard = (text: string) => {
    navigator.clipboard.writeText(text);
  };

  return (
    <div className="genui-playground">
      {/* Navigation */}
      <nav className="playground-nav">
        <Link to="/" className="nav-back">← Back to 3D Demo</Link>
        <h1>🎨 GenUI Playground</h1>
        <div className="nav-links">
          <Link to="/gamification">🏆 Leaderboard</Link>
          <Link to="/onboarding">🚀 Get Started</Link>
          <Link to="/landing">🌊 Landing</Link>
        </div>
      </nav>

      <div className="playground-container">
        {/* Sidebar */}
        <aside className="playground-sidebar">
          <div className="sidebar-section">
            <h3>💡 Example Prompts</h3>
            <div className="example-prompts">
              {examplePrompts.map((example, i) => (
                <button
                  key={i}
                  className="example-btn"
                  onClick={() => setPrompt(example)}
                >
                  {example}
                </button>
              ))}
            </div>
          </div>

          <div className="sidebar-section">
            <h3>📜 Generation History</h3>
            <div className="history-list">
              {history.length === 0 ? (
                <p className="history-empty">No generations yet</p>
              ) : (
                history.map((item, i) => (
                  <div key={i} className="history-item" onClick={() => {
                    setGeneratedComponent(componentTemplates[item.template]);
                  }}>
                    <span className="history-prompt">{item.prompt.slice(0, 30)}...</span>
                    <span className="history-template">{componentTemplates[item.template].name}</span>
                  </div>
                ))
              )}
            </div>
          </div>
        </aside>

        {/* Main Content */}
        <main className="playground-main">
          {/* Input Area */}
          <div className="input-area">
            <div className="input-wrapper">
              <input
                type="text"
                value={prompt}
                onChange={(e) => setPrompt(e.target.value)}
                placeholder="Describe the component you want to generate..."
                onKeyDown={(e) => e.key === 'Enter' && handleGenerate()}
                disabled={isGenerating}
              />
              <button 
                className="generate-btn" 
                onClick={handleGenerate}
                disabled={isGenerating || !prompt.trim()}
              >
                {isGenerating ? (
                  <span className="generating">
                    <span className="spinner"></span>
                    Generating...
                  </span>
                ) : (
                  '⚡ Generate'
                )}
              </button>
            </div>
            <p className="input-hint">
              Try: "card", "button", "navbar", "hero", "stats", "form", "modal", "pricing", "testimonial"
            </p>
          </div>

          {/* Output Area */}
          {generatedComponent && (
            <div className="output-area">
              {/* Tabs */}
              <div className="output-tabs">
                <button 
                  className={`tab ${activeTab === 'preview' ? 'active' : ''}`}
                  onClick={() => setActiveTab('preview')}
                >
                  👁️ Preview
                </button>
                <button 
                  className={`tab ${activeTab === 'jsx' ? 'active' : ''}`}
                  onClick={() => setActiveTab('jsx')}
                >
                  ⚛️ JSX
                </button>
                <button 
                  className={`tab ${activeTab === 'css' ? 'active' : ''}`}
                  onClick={() => setActiveTab('css')}
                >
                  🎨 CSS
                </button>
              </div>

              {/* Tab Content */}
              <div className="tab-content">
                {activeTab === 'preview' && (
                  <div className="preview-panel" ref={previewRef}>
                    <div className="preview-frame" dangerouslySetInnerHTML={{ __html: generatedComponent.jsx }} />
                  </div>
                )}

                {activeTab === 'jsx' && (
                  <div className="code-panel">
                    <button className="copy-btn" onClick={() => copyToClipboard(generatedComponent.jsx)}>
                      📋 Copy JSX
                    </button>
                    <pre><code>{generatedComponent.jsx}</code></pre>
                  </div>
                )}

                {activeTab === 'css' && (
                  <div className="code-panel">
                    <button className="copy-btn" onClick={() => copyToClipboard(generatedComponent.css)}>
                      📋 Copy CSS
                    </button>
                    <pre><code>{generatedComponent.css}</code></pre>
                  </div>
                )}
              </div>
            </div>
          )}

          {/* Empty State */}
          {!generatedComponent && (
            <div className="empty-state">
              <div className="empty-icon">🎨</div>
              <h2>Generative UI Playground</h2>
              <p>Enter a natural language prompt above to generate beautiful cyberpunk UI components instantly.</p>
              <div className="feature-grid">
                <div className="feature">
                  <span className="feature-icon">⚡</span>
                  <span>Instant Generation</span>
                </div>
                <div className="feature">
                  <span className="feature-icon">🎯</span>
                  <span>10+ Components</span>
                </div>
                <div className="feature">
                  <span className="feature-icon">📋</span>
                  <span>Copy Code</span>
                </div>
                <div className="feature">
                  <span className="feature-icon">🌙</span>
                  <span>Dark Cyberpunk</span>
                </div>
              </div>
            </div>
          )}
        </main>
      </div>

      <style>{`
        .genui-playground {
          min-height: 100vh;
          background: #0a0a1a;
          color: white;
        }

        .playground-nav {
          display: flex;
          justify-content: space-between;
          align-items: center;
          padding: 16px 32px;
          background: rgba(10, 10, 26, 0.95);
          border-bottom: 2px solid #00f0ff;
          position: sticky;
          top: 0;
          z-index: 100;
        }

        .nav-back {
          color: #00f0ff;
          text-decoration: none;
          transition: color 0.3s;
        }

        .nav-back:hover {
          color: #ff00ff;
        }

        .playground-nav h1 {
          margin: 0;
          font-size: 1.5rem;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .nav-links {
          display: flex;
          gap: 24px;
        }

        .nav-links a {
          color: #a0a0a0;
          text-decoration: none;
          transition: color 0.3s;
        }

        .nav-links a:hover {
          color: #00f0ff;
        }

        .playground-container {
          display: flex;
          max-width: 1600px;
          margin: 0 auto;
          padding: 24px;
          gap: 24px;
        }

        .playground-sidebar {
          width: 300px;
          flex-shrink: 0;
        }

        .sidebar-section {
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 16px;
          padding: 20px;
          margin-bottom: 20px;
        }

        .sidebar-section h3 {
          margin: 0 0 16px;
          color: #00f0ff;
          font-size: 1rem;
        }

        .example-prompts {
          display: flex;
          flex-direction: column;
          gap: 8px;
        }

        .example-btn {
          background: rgba(255, 255, 255, 0.05);
          border: 1px solid rgba(255, 255, 255, 0.1);
          color: #a0a0a0;
          padding: 10px 12px;
          border-radius: 8px;
          text-align: left;
          cursor: pointer;
          transition: all 0.3s;
          font-size: 0.875rem;
        }

        .example-btn:hover {
          background: rgba(0, 240, 255, 0.1);
          border-color: #00f0ff;
          color: white;
        }

        .history-list {
          display: flex;
          flex-direction: column;
          gap: 8px;
          max-height: 300px;
          overflow-y: auto;
        }

        .history-empty {
          color: #666;
          font-size: 0.875rem;
        }

        .history-item {
          background: rgba(255, 255, 255, 0.05);
          border-radius: 8px;
          padding: 10px;
          cursor: pointer;
          transition: all 0.3s;
        }

        .history-item:hover {
          background: rgba(255, 0, 255, 0.1);
        }

        .history-prompt {
          display: block;
          color: white;
          font-size: 0.875rem;
        }

        .history-template {
          display: block;
          color: #ff00ff;
          font-size: 0.75rem;
          margin-top: 4px;
        }

        .playground-main {
          flex: 1;
          min-width: 0;
        }

        .input-area {
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 16px;
          padding: 24px;
          margin-bottom: 24px;
        }

        .input-wrapper {
          display: flex;
          gap: 12px;
        }

        .input-wrapper input {
          flex: 1;
          background: rgba(0, 0, 0, 0.3);
          border: 2px solid rgba(0, 240, 255, 0.3);
          border-radius: 12px;
          padding: 16px 20px;
          color: white;
          font-size: 1rem;
          outline: none;
          transition: border-color 0.3s;
        }

        .input-wrapper input:focus {
          border-color: #00f0ff;
        }

        .input-wrapper input::placeholder {
          color: #666;
        }

        .generate-btn {
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          border: none;
          color: white;
          padding: 16px 32px;
          border-radius: 12px;
          font-weight: bold;
          font-size: 1rem;
          cursor: pointer;
          transition: transform 0.3s, box-shadow 0.3s;
          white-space: nowrap;
        }

        .generate-btn:hover:not(:disabled) {
          transform: translateY(-2px);
          box-shadow: 0 8px 25px rgba(0, 240, 255, 0.3);
        }

        .generate-btn:disabled {
          opacity: 0.6;
          cursor: not-allowed;
        }

        .generating {
          display: flex;
          align-items: center;
          gap: 8px;
        }

        .spinner {
          width: 16px;
          height: 16px;
          border: 2px solid rgba(255, 255, 255, 0.3);
          border-top-color: white;
          border-radius: 50%;
          animation: spin 0.8s linear infinite;
        }

        @keyframes spin {
          to { transform: rotate(360deg); }
        }

        .input-hint {
          margin: 12px 0 0;
          color: #666;
          font-size: 0.875rem;
        }

        .output-area {
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 16px;
          overflow: hidden;
        }

        .output-tabs {
          display: flex;
          background: rgba(0, 0, 0, 0.3);
          border-bottom: 1px solid rgba(0, 240, 255, 0.2);
        }

        .tab {
          flex: 1;
          background: transparent;
          border: none;
          color: #a0a0a0;
          padding: 16px;
          cursor: pointer;
          transition: all 0.3s;
          font-size: 1rem;
        }

        .tab:hover {
          background: rgba(0, 240, 255, 0.1);
          color: white;
        }

        .tab.active {
          background: rgba(0, 240, 255, 0.1);
          color: #00f0ff;
          border-bottom: 2px solid #00f0ff;
        }

        .tab-content {
          padding: 24px;
        }

        .preview-panel {
          background: #0a0a1a;
          border-radius: 12px;
          padding: 32px;
          display: flex;
          justify-content: center;
          align-items: center;
          min-height: 300px;
        }

        .preview-frame {
          max-width: 100%;
        }

        .code-panel {
          position: relative;
        }

        .copy-btn {
          position: absolute;
          top: 0;
          right: 0;
          background: rgba(0, 240, 255, 0.2);
          border: 1px solid #00f0ff;
          color: #00f0ff;
          padding: 8px 16px;
          border-radius: 8px;
          cursor: pointer;
          transition: all 0.3s;
        }

        .copy-btn:hover {
          background: #00f0ff;
          color: #0a0a1a;
        }

        .code-panel pre {
          background: rgba(0, 0, 0, 0.5);
          border-radius: 12px;
          padding: 20px;
          overflow-x: auto;
          margin: 0;
        }

        .code-panel code {
          color: #00ff88;
          font-family: 'Fira Code', monospace;
          font-size: 0.875rem;
          line-height: 1.6;
        }

        .empty-state {
          text-align: center;
          padding: 80px 40px;
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 16px;
        }

        .empty-icon {
          font-size: 4rem;
          margin-bottom: 24px;
        }

        .empty-state h2 {
          margin: 0 0 12px;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .empty-state p {
          color: #a0a0a0;
          max-width: 500px;
          margin: 0 auto 32px;
        }

        .feature-grid {
          display: grid;
          grid-template-columns: repeat(4, 1fr);
          gap: 16px;
          max-width: 600px;
          margin: 0 auto;
        }

        .feature {
          display: flex;
          flex-direction: column;
          align-items: center;
          gap: 8px;
          padding: 16px;
          background: rgba(0, 0, 0, 0.3);
          border-radius: 12px;
        }

        .feature-icon {
          font-size: 1.5rem;
        }

        .feature span:last-child {
          color: #a0a0a0;
          font-size: 0.875rem;
        }

        @media (max-width: 1024px) {
          .playground-container {
            flex-direction: column;
          }
          
          .playground-sidebar {
            width: 100%;
          }

          .feature-grid {
            grid-template-columns: repeat(2, 1fr);
          }
        }

        @media (max-width: 640px) {
          .playground-nav {
            flex-direction: column;
            gap: 12px;
          }

          .input-wrapper {
            flex-direction: column;
          }

          .feature-grid {
            grid-template-columns: 1fr 1fr;
          }
        }
      `}</style>
    </div>
  );
}
