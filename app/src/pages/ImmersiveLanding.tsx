import { useEffect, useRef, useState } from 'react';
import { Link } from 'react-router-dom';
import gsap from 'gsap';

// Stats data
const stats = [
  { value: '1000+', label: 'Curated Resources', icon: '📚' },
  { value: '500+', label: 'AI Tools', icon: '🤖' },
  { value: '300+', label: 'Awesome Lists', icon: '⭐' },
  { value: '50+', label: '3D/WebGL Tools', icon: '🎮' },
  { value: '10K+', label: 'GitHub Stars', icon: '✨' },
  { value: '100+', label: 'Contributors', icon: '👥' }
];

// Features data
const features = [
  {
    icon: '🤖',
    title: 'AI & Machine Learning',
    description: 'GPT-5.1, Claude Opus 4.5, Gemini 3, and 500+ AI tools curated for developers.',
    color: '#00f0ff'
  },
  {
    icon: '⚡',
    title: 'Developer Tools',
    description: 'CLI tools, IDE extensions, debugging utilities, and productivity boosters.',
    color: '#ff00ff'
  },
  {
    icon: '☁️',
    title: 'Cloud & DevOps',
    description: 'AWS, GCP, Azure, Kubernetes, Docker, and infrastructure automation.',
    color: '#00ff88'
  },
  {
    icon: '🔒',
    title: 'Security',
    description: 'Penetration testing, vulnerability scanning, and security best practices.',
    color: '#ff6600'
  },
  {
    icon: '🎮',
    title: '3D & Interactive',
    description: 'Three.js, React Three Fiber, WebGL, AR/VR, and creative coding tools.',
    color: '#7b2fff'
  },
  {
    icon: '📚',
    title: 'Learning Resources',
    description: 'Books, courses, tutorials, and interactive learning platforms.',
    color: '#ffcc00'
  }
];

// Timeline milestones
const milestones = [
  { year: '2024 Q1', title: 'Foundation', description: 'Initial curated list created' },
  { year: '2024 Q2', title: 'AI Expansion', description: 'Added 500+ AI tools' },
  { year: '2024 Q3', title: 'Interactive 3D', description: 'Three.js demo launched' },
  { year: '2024 Q4', title: 'Gamification', description: 'Leaderboard & achievements' },
  { year: '2025 Q1', title: 'GenUI Playground', description: 'AI component generator' },
  { year: '2025 Q2', title: 'Community Growth', description: '10K+ stars milestone' }
];

export default function ImmersiveLanding() {
  const containerRef = useRef<HTMLDivElement>(null);
  const heroRef = useRef<HTMLDivElement>(null);
  const statsRef = useRef<HTMLDivElement>(null);
  const featuresRef = useRef<HTMLDivElement>(null);
  const timelineRef = useRef<HTMLDivElement>(null);
  const ctaRef = useRef<HTMLDivElement>(null);
  const [scrollProgress, setScrollProgress] = useState(0);

  useEffect(() => {
    // Handle scroll progress
    const handleScroll = () => {
      const scrollTop = window.scrollY;
      const docHeight = document.documentElement.scrollHeight - window.innerHeight;
      const progress = scrollTop / docHeight;
      setScrollProgress(progress);
    };

    window.addEventListener('scroll', handleScroll);

    // GSAP Animations
    const ctx = gsap.context(() => {
      // Hero animation
      gsap.from('.hero-title', {
        opacity: 0,
        y: 100,
        duration: 1.2,
        ease: 'power4.out'
      });

      gsap.from('.hero-subtitle', {
        opacity: 0,
        y: 50,
        duration: 1,
        delay: 0.3,
        ease: 'power3.out'
      });

      gsap.from('.hero-buttons > *', {
        opacity: 0,
        y: 30,
        stagger: 0.15,
        duration: 0.8,
        delay: 0.6,
        ease: 'power2.out'
      });

      // Floating particles animation
      gsap.to('.floating-particle', {
        y: -20,
        duration: 2,
        repeat: -1,
        yoyo: true,
        ease: 'power1.inOut',
        stagger: {
          each: 0.2,
          from: 'random'
        }
      });

      // Feature cards stagger animation with delay
      gsap.from('.feature-card', {
        opacity: 0,
        y: 80,
        stagger: 0.15,
        duration: 0.8,
        delay: 1.2,
        ease: 'power2.out',
      });

      // Timeline items animation
      gsap.from('.timeline-item', {
        opacity: 0,
        x: (i) => i % 2 === 0 ? -50 : 50,
        stagger: 0.2,
        duration: 0.8,
        delay: 1.5,
        ease: 'power2.out',
      });

      // CTA section animation
      gsap.from('.cta-content > *', {
        opacity: 0,
        y: 50,
        stagger: 0.2,
        duration: 0.8,
        delay: 1.8,
        ease: 'power2.out',
      });
    }, containerRef);

    return () => {
      ctx.revert();
      window.removeEventListener('scroll', handleScroll);
    };
  }, []);

  return (
    <div className="immersive-landing" ref={containerRef}>
      {/* Progress Bar */}
      <div className="scroll-progress" style={{ transform: `scaleX(${scrollProgress})` }} />

      {/* Navigation */}
      <nav className="landing-nav">
        <Link to="/" className="nav-logo">⚡ DevTeam6</Link>
        <div className="nav-links">
          <Link to="/">3D Demo</Link>
          <Link to="/genui">GenUI</Link>
          <Link to="/gamification">Leaderboard</Link>
          <Link to="/onboarding">Get Started</Link>
          <a href="https://github.com/SpiralCloudOmega/DevTeam6" target="_blank" rel="noopener noreferrer" className="nav-cta">
            ⭐ Star on GitHub
          </a>
        </div>
      </nav>

      {/* Hero Section */}
      <section className="hero-section" ref={heroRef}>
        {/* Floating Particles */}
        {[...Array(20)].map((_, i) => (
          <div 
            key={i}
            className="floating-particle"
            style={{
              left: `${Math.random() * 100}%`,
              top: `${Math.random() * 100}%`,
              width: `${Math.random() * 6 + 2}px`,
              height: `${Math.random() * 6 + 2}px`,
              background: ['#00f0ff', '#ff00ff', '#00ff88', '#7b2fff'][Math.floor(Math.random() * 4)],
              animationDelay: `${Math.random() * 2}s`
            }}
          />
        ))}

        {/* Grid Background */}
        <div className="hero-grid" />

        {/* Hero Content */}
        <div className="hero-content">
          <h1 className="hero-title">
            <span className="title-line">THE OMEGA</span>
            <span className="title-gradient">TOOL KIT</span>
          </h1>
          <p className="hero-subtitle">
            1000+ Curated Resources for AI, DevOps, Cloud, Security, 3D/WebGL & Beyond
          </p>
          <div className="hero-buttons">
            <Link to="/" className="hero-btn primary">
              🎮 Explore 3D Demo
            </Link>
            <Link to="/genui" className="hero-btn secondary">
              🎨 GenUI Playground
            </Link>
            <a 
              href="https://github.com/SpiralCloudOmega/DevTeam6" 
              target="_blank" 
              rel="noopener noreferrer"
              className="hero-btn outline"
            >
              📚 View Repository
            </a>
          </div>
        </div>

        {/* Scroll Indicator */}
        <div className="scroll-indicator">
          <span>Scroll to Explore</span>
          <div className="scroll-arrow" />
        </div>
      </section>

      {/* Stats Section */}
      <section className="stats-section" ref={statsRef}>
        <div className="stats-container">
          {stats.map((stat, i) => (
            <div key={i} className="stat-item">
              <div className="stat-icon">{stat.icon}</div>
              <div className="stat-number" data-target={stat.value.replace(/\D/g, '')}>
                {stat.value}
              </div>
              <div className="stat-label">{stat.label}</div>
            </div>
          ))}
        </div>
      </section>

      {/* Features Section */}
      <section className="features-section" ref={featuresRef}>
        <div className="section-header">
          <h2>Everything You Need</h2>
          <p>Comprehensive resources across all domains of modern software development</p>
        </div>
        <div className="features-grid">
          {features.map((feature, i) => (
            <div 
              key={i} 
              className="feature-card"
              style={{ '--accent-color': feature.color } as React.CSSProperties}
            >
              <div className="feature-icon">{feature.icon}</div>
              <h3>{feature.title}</h3>
              <p>{feature.description}</p>
              <div className="feature-glow" />
            </div>
          ))}
        </div>
      </section>

      {/* Timeline Section */}
      <section className="timeline-section" ref={timelineRef}>
        <div className="section-header">
          <h2>Our Journey</h2>
          <p>From a simple list to a comprehensive developer ecosystem</p>
        </div>
        <div className="timeline-container">
          <div className="timeline-line" />
          {milestones.map((milestone, i) => (
            <div key={i} className={`timeline-item ${i % 2 === 0 ? 'left' : 'right'}`}>
              <div className="timeline-content">
                <div className="timeline-year">{milestone.year}</div>
                <h3>{milestone.title}</h3>
                <p>{milestone.description}</p>
              </div>
              <div className="timeline-dot" />
            </div>
          ))}
        </div>
      </section>

      {/* Interactive Demos Section */}
      <section className="demos-section">
        <div className="section-header">
          <h2>Interactive Experiences</h2>
          <p>Explore DevTeam6 through immersive demonstrations</p>
        </div>
        <div className="demos-grid">
          <Link to="/" className="demo-card">
            <div className="demo-preview demo-3d">
              <div className="demo-icon">🎮</div>
            </div>
            <div className="demo-info">
              <h3>3D Experience</h3>
              <p>Interactive Three.js visualization</p>
            </div>
          </Link>
          <Link to="/genui" className="demo-card">
            <div className="demo-preview demo-genui">
              <div className="demo-icon">🎨</div>
            </div>
            <div className="demo-info">
              <h3>GenUI Playground</h3>
              <p>AI-powered component generator</p>
            </div>
          </Link>
          <Link to="/gamification" className="demo-card">
            <div className="demo-preview demo-gamification">
              <div className="demo-icon">🏆</div>
            </div>
            <div className="demo-info">
              <h3>Leaderboard</h3>
              <p>Contributor rankings & achievements</p>
            </div>
          </Link>
          <Link to="/onboarding" className="demo-card">
            <div className="demo-preview demo-onboarding">
              <div className="demo-icon">🚀</div>
            </div>
            <div className="demo-info">
              <h3>Onboarding Wizard</h3>
              <p>Step-by-step contributor guide</p>
            </div>
          </Link>
        </div>
      </section>

      {/* CTA Section */}
      <section className="cta-section" ref={ctaRef}>
        <div className="cta-bg" />
        <div className="cta-content">
          <h2>Ready to Level Up?</h2>
          <p>Join thousands of developers using DevTeam6 as their ultimate resource toolkit.</p>
          <div className="cta-buttons">
            <a 
              href="https://github.com/SpiralCloudOmega/DevTeam6" 
              target="_blank" 
              rel="noopener noreferrer"
              className="cta-btn primary"
            >
              ⭐ Star on GitHub
            </a>
            <Link to="/onboarding" className="cta-btn secondary">
              🚀 Get Started
            </Link>
          </div>
          <div className="cta-stats">
            <span>⭐ 10K+ Stars</span>
            <span>👥 100+ Contributors</span>
            <span>📚 1000+ Resources</span>
          </div>
        </div>
      </section>

      {/* Footer */}
      <footer className="landing-footer">
        <div className="footer-content">
          <div className="footer-brand">
            <h3>⚡ DevTeam6</h3>
            <p>The Omega Tool Kit</p>
          </div>
          <div className="footer-links">
            <Link to="/">3D Demo</Link>
            <Link to="/genui">GenUI</Link>
            <Link to="/gamification">Leaderboard</Link>
            <Link to="/onboarding">Get Started</Link>
            <a href="https://github.com/SpiralCloudOmega/DevTeam6" target="_blank" rel="noopener noreferrer">GitHub</a>
          </div>
          <div className="footer-copy">
            © 2025 DevTeam6 • MIT License
          </div>
        </div>
        <div className="footer-gradient" />
      </footer>

      <style>{`
        .immersive-landing {
          min-height: 100vh;
          background: #0a0a1a;
          color: white;
          overflow-x: hidden;
        }

        .scroll-progress {
          position: fixed;
          top: 0;
          left: 0;
          width: 100%;
          height: 3px;
          background: linear-gradient(90deg, #00f0ff, #ff00ff, #00ff88);
          transform-origin: left;
          z-index: 1000;
        }

        /* Navigation */
        .landing-nav {
          position: fixed;
          top: 0;
          left: 0;
          right: 0;
          display: flex;
          justify-content: space-between;
          align-items: center;
          padding: 16px 48px;
          background: rgba(10, 10, 26, 0.9);
          backdrop-filter: blur(10px);
          z-index: 100;
          border-bottom: 1px solid rgba(0, 240, 255, 0.2);
        }

        .nav-logo {
          color: #00f0ff;
          font-size: 1.5rem;
          font-weight: bold;
          text-decoration: none;
          text-shadow: 0 0 10px rgba(0, 240, 255, 0.5);
        }

        .nav-links {
          display: flex;
          align-items: center;
          gap: 32px;
        }

        .nav-links a {
          color: #a0a0a0;
          text-decoration: none;
          transition: color 0.3s;
        }

        .nav-links a:hover {
          color: white;
        }

        .nav-cta {
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          padding: 10px 20px;
          border-radius: 8px;
          color: white !important;
          font-weight: bold;
        }

        /* Hero Section */
        .hero-section {
          min-height: 100vh;
          display: flex;
          align-items: center;
          justify-content: center;
          position: relative;
          overflow: hidden;
          padding-top: 80px;
        }

        .floating-particle {
          position: absolute;
          border-radius: 50%;
          opacity: 0.6;
          pointer-events: none;
        }

        .hero-grid {
          position: absolute;
          inset: 0;
          background-image: 
            linear-gradient(rgba(0, 240, 255, 0.03) 1px, transparent 1px),
            linear-gradient(90deg, rgba(0, 240, 255, 0.03) 1px, transparent 1px);
          background-size: 60px 60px;
        }

        .hero-content {
          text-align: center;
          z-index: 2;
          padding: 0 24px;
        }

        .hero-title {
          font-size: 5rem;
          margin: 0;
          line-height: 1.1;
        }

        .title-line {
          display: block;
          color: white;
          font-weight: 300;
        }

        .title-gradient {
          display: block;
          background: linear-gradient(135deg, #00f0ff, #ff00ff, #00ff88);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
          font-weight: 900;
        }

        .hero-subtitle {
          font-size: 1.5rem;
          color: #a0a0a0;
          margin: 24px 0 48px;
          max-width: 600px;
          margin-left: auto;
          margin-right: auto;
        }

        .hero-buttons {
          display: flex;
          gap: 16px;
          justify-content: center;
          flex-wrap: wrap;
        }

        .hero-btn {
          padding: 16px 32px;
          border-radius: 12px;
          font-weight: bold;
          font-size: 1rem;
          text-decoration: none;
          transition: all 0.3s;
        }

        .hero-btn.primary {
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          color: white;
        }

        .hero-btn.secondary {
          background: rgba(0, 240, 255, 0.1);
          color: #00f0ff;
          border: 2px solid #00f0ff;
        }

        .hero-btn.outline {
          background: transparent;
          color: white;
          border: 2px solid rgba(255, 255, 255, 0.3);
        }

        .hero-btn:hover {
          transform: translateY(-3px);
          box-shadow: 0 10px 30px rgba(0, 240, 255, 0.3);
        }

        .scroll-indicator {
          position: absolute;
          bottom: 40px;
          left: 50%;
          transform: translateX(-50%);
          text-align: center;
        }

        .scroll-indicator span {
          display: block;
          color: #666;
          font-size: 0.875rem;
          margin-bottom: 8px;
        }

        .scroll-arrow {
          width: 24px;
          height: 24px;
          margin: 0 auto;
          border-right: 2px solid #00f0ff;
          border-bottom: 2px solid #00f0ff;
          transform: rotate(45deg);
          animation: bounce 2s infinite;
        }

        @keyframes bounce {
          0%, 20%, 50%, 80%, 100% { transform: translateY(0) rotate(45deg); }
          40% { transform: translateY(10px) rotate(45deg); }
          60% { transform: translateY(5px) rotate(45deg); }
        }

        /* Stats Section */
        .stats-section {
          padding: 100px 24px;
          background: linear-gradient(180deg, #0a0a1a, #1a0a2e);
        }

        .stats-container {
          display: grid;
          grid-template-columns: repeat(6, 1fr);
          gap: 24px;
          max-width: 1200px;
          margin: 0 auto;
        }

        .stat-item {
          text-align: center;
          padding: 24px;
        }

        .stat-icon {
          font-size: 2.5rem;
          margin-bottom: 12px;
        }

        .stat-number {
          font-size: 2.5rem;
          font-weight: bold;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .stat-label {
          color: #a0a0a0;
          margin-top: 8px;
        }

        /* Section Headers */
        .section-header {
          text-align: center;
          margin-bottom: 60px;
        }

        .section-header h2 {
          font-size: 3rem;
          margin: 0;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .section-header p {
          color: #a0a0a0;
          font-size: 1.25rem;
          margin: 16px 0 0;
        }

        /* Features Section */
        .features-section {
          padding: 100px 24px;
          max-width: 1200px;
          margin: 0 auto;
        }

        .features-grid {
          display: grid;
          grid-template-columns: repeat(3, 1fr);
          gap: 24px;
        }

        .feature-card {
          position: relative;
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 20px;
          padding: 32px;
          transition: all 0.3s;
          overflow: hidden;
        }

        .feature-card:hover {
          transform: translateY(-5px);
          border-color: var(--accent-color);
          box-shadow: 0 10px 40px rgba(0, 240, 255, 0.15);
        }

        .feature-icon {
          font-size: 3rem;
          margin-bottom: 16px;
        }

        .feature-card h3 {
          color: white;
          margin: 0 0 12px;
          font-size: 1.5rem;
        }

        .feature-card p {
          color: #a0a0a0;
          margin: 0;
          line-height: 1.6;
        }

        .feature-glow {
          position: absolute;
          top: 0;
          left: 0;
          right: 0;
          height: 3px;
          background: var(--accent-color);
          opacity: 0;
          transition: opacity 0.3s;
        }

        .feature-card:hover .feature-glow {
          opacity: 1;
        }

        /* Timeline Section */
        .timeline-section {
          padding: 100px 24px;
          background: linear-gradient(180deg, #0a0a1a, #1a0a2e, #0a0a1a);
        }

        .timeline-container {
          position: relative;
          max-width: 800px;
          margin: 0 auto;
        }

        .timeline-line {
          position: absolute;
          left: 50%;
          top: 0;
          bottom: 0;
          width: 2px;
          background: linear-gradient(180deg, #00f0ff, #ff00ff, #00ff88);
          transform: translateX(-50%);
        }

        .timeline-item {
          position: relative;
          display: flex;
          margin-bottom: 40px;
        }

        .timeline-item.left {
          justify-content: flex-start;
        }

        .timeline-item.right {
          justify-content: flex-end;
        }

        .timeline-content {
          width: 45%;
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 16px;
          padding: 24px;
        }

        .timeline-year {
          color: #00f0ff;
          font-weight: bold;
          margin-bottom: 8px;
        }

        .timeline-content h3 {
          color: white;
          margin: 0 0 8px;
        }

        .timeline-content p {
          color: #a0a0a0;
          margin: 0;
        }

        .timeline-dot {
          position: absolute;
          left: 50%;
          top: 50%;
          width: 16px;
          height: 16px;
          background: #00f0ff;
          border-radius: 50%;
          transform: translate(-50%, -50%);
          box-shadow: 0 0 20px #00f0ff;
        }

        /* Demos Section */
        .demos-section {
          padding: 100px 24px;
          max-width: 1200px;
          margin: 0 auto;
        }

        .demos-grid {
          display: grid;
          grid-template-columns: repeat(4, 1fr);
          gap: 24px;
        }

        .demo-card {
          text-decoration: none;
          background: rgba(0, 240, 255, 0.05);
          border: 1px solid rgba(0, 240, 255, 0.2);
          border-radius: 20px;
          overflow: hidden;
          transition: all 0.3s;
        }

        .demo-card:hover {
          transform: translateY(-5px);
          border-color: #00f0ff;
        }

        .demo-preview {
          height: 150px;
          display: flex;
          align-items: center;
          justify-content: center;
        }

        .demo-3d { background: linear-gradient(135deg, #00f0ff20, #ff00ff20); }
        .demo-genui { background: linear-gradient(135deg, #ff00ff20, #7b2fff20); }
        .demo-gamification { background: linear-gradient(135deg, #ff660020, #ffcc0020); }
        .demo-onboarding { background: linear-gradient(135deg, #00ff8820, #00f0ff20); }

        .demo-icon {
          font-size: 4rem;
        }

        .demo-info {
          padding: 20px;
        }

        .demo-info h3 {
          color: white;
          margin: 0 0 8px;
        }

        .demo-info p {
          color: #a0a0a0;
          margin: 0;
          font-size: 0.875rem;
        }

        /* CTA Section */
        .cta-section {
          position: relative;
          padding: 100px 24px;
          text-align: center;
          overflow: hidden;
        }

        .cta-bg {
          position: absolute;
          inset: 0;
          background: radial-gradient(ellipse at center, rgba(123, 47, 255, 0.2), transparent 70%);
        }

        .cta-content {
          position: relative;
          z-index: 2;
        }

        .cta-content h2 {
          font-size: 3.5rem;
          margin: 0;
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
        }

        .cta-content p {
          font-size: 1.25rem;
          color: #a0a0a0;
          margin: 24px 0 40px;
        }

        .cta-buttons {
          display: flex;
          gap: 16px;
          justify-content: center;
        }

        .cta-btn {
          padding: 18px 40px;
          border-radius: 12px;
          font-weight: bold;
          font-size: 1.1rem;
          text-decoration: none;
          transition: all 0.3s;
        }

        .cta-btn.primary {
          background: linear-gradient(135deg, #00f0ff, #ff00ff);
          color: white;
        }

        .cta-btn.secondary {
          background: transparent;
          color: #00f0ff;
          border: 2px solid #00f0ff;
        }

        .cta-btn:hover {
          transform: translateY(-3px);
          box-shadow: 0 10px 30px rgba(0, 240, 255, 0.3);
        }

        .cta-stats {
          display: flex;
          gap: 48px;
          justify-content: center;
          margin-top: 48px;
          color: #a0a0a0;
        }

        /* Footer */
        .landing-footer {
          position: relative;
          padding: 60px 24px;
          background: #0a0a1a;
          border-top: 1px solid rgba(0, 240, 255, 0.2);
        }

        .footer-content {
          max-width: 1200px;
          margin: 0 auto;
          text-align: center;
        }

        .footer-brand h3 {
          color: #00f0ff;
          margin: 0 0 8px;
          font-size: 1.5rem;
        }

        .footer-brand p {
          color: #666;
          margin: 0;
        }

        .footer-links {
          display: flex;
          gap: 32px;
          justify-content: center;
          margin: 32px 0;
        }

        .footer-links a {
          color: #a0a0a0;
          text-decoration: none;
          transition: color 0.3s;
        }

        .footer-links a:hover {
          color: #00f0ff;
        }

        .footer-copy {
          color: #666;
          font-size: 0.875rem;
        }

        .footer-gradient {
          position: absolute;
          bottom: 0;
          left: 0;
          right: 0;
          height: 3px;
          background: linear-gradient(90deg, #00f0ff, #ff00ff, #00ff88, #7b2fff, #ff6600, #00f0ff);
          background-size: 300% 100%;
          animation: shimmer 4s linear infinite;
        }

        @keyframes shimmer {
          0% { background-position: 0% 0%; }
          100% { background-position: 300% 0%; }
        }

        /* Responsive */
        @media (max-width: 1024px) {
          .stats-container {
            grid-template-columns: repeat(3, 1fr);
          }
          
          .features-grid {
            grid-template-columns: repeat(2, 1fr);
          }
          
          .demos-grid {
            grid-template-columns: repeat(2, 1fr);
          }
        }

        @media (max-width: 768px) {
          .landing-nav {
            padding: 12px 24px;
          }
          
          .nav-links {
            display: none;
          }
          
          .hero-title {
            font-size: 3rem;
          }
          
          .hero-subtitle {
            font-size: 1.1rem;
          }
          
          .hero-buttons {
            flex-direction: column;
            align-items: center;
          }
          
          .stats-container {
            grid-template-columns: repeat(2, 1fr);
          }
          
          .features-grid,
          .demos-grid {
            grid-template-columns: 1fr;
          }
          
          .timeline-item.left,
          .timeline-item.right {
            justify-content: flex-end;
          }
          
          .timeline-content {
            width: 80%;
          }
          
          .timeline-line {
            left: 20px;
          }
          
          .timeline-dot {
            left: 20px;
          }

          .cta-content h2 {
            font-size: 2rem;
          }
          
          .cta-buttons {
            flex-direction: column;
            align-items: center;
          }
          
          .cta-stats {
            flex-direction: column;
            gap: 16px;
          }
          
          .footer-links {
            flex-wrap: wrap;
          }
        }
      `}</style>
    </div>
  );
}
