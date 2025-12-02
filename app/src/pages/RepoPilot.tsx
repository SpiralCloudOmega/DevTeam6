import { useState, useRef, useEffect } from 'react';
import { Link } from 'react-router-dom';

interface Message {
  id: number;
  role: 'user' | 'assistant' | 'system';
  content: string;
  timestamp: Date;
}

const SAMPLE_RESPONSES: Record<string, string> = {
  'default': `I'm RepoMind, your AI co-pilot for DevTeam6! I can help you navigate the repository, explain tools and resources, suggest learning paths, and answer questions about the codebase. What would you like to explore today?`,
  'ai': `DevTeam6 features an extensive AI Tools section including:

**AI IDEs & Coding Assistants:**
- Cursor, Windsurf, GitHub Copilot, Codeium
- Claude Code, Gemini CLI, Amazon Q

**Local LLM Solutions:**
- Ollama, LM Studio, LocalAI, Jan
- GPT4All, Tabby, llama.cpp

**AI Frameworks:**
- LangChain, LlamaIndex, AutoGen
- CrewAI, OpenAI Agents SDK

Would you like more details on any specific category?`,
  'devops': `DevOps resources in DevTeam6 include:

**Container & Orchestration:**
- Docker, Kubernetes, Podman
- Helm, ArgoCD, Flux

**CI/CD:**
- GitHub Actions, GitLab CI
- Jenkins, CircleCI, Drone

**Infrastructure as Code:**
- Terraform, Pulumi, Ansible
- CloudFormation, CDK

Want me to dive deeper into any area?`,
  'music': `Music Production tools curated in DevTeam6:

**DAWs:**
- Ableton Live, FL Studio, Logic Pro X
- Bitwig, Reaper, LMMS, Ardour

**Live Coding:**
- Sonic Pi, TidalCycles, Strudel, Orca

**AI Music:**
- Suno AI, Splash, Omnizart, Magenta

Need recommendations for your workflow?`,
  'cad': `CAD & 3D Design resources:

**Open Source CAD:**
- FreeCAD, OpenSCAD, LibreCAD, KiCad

**3D Modeling:**
- Blender, Wings 3D, MeshLab

**Industrial CAD:**
- Fusion 360, Onshape, SolidWorks

**3D Printing:**
- PrusaSlicer, Cura, OctoPrint

What type of project are you working on?`,
  'help': `Here's what I can help you with:

🔍 **Navigate** - Find tools, resources, and documentation
📚 **Learn** - Get learning paths and tutorials
🛠️ **Build** - Project recommendations and tech stacks
🎮 **Gamify** - Track achievements and progress
💡 **Suggest** - AI-powered recommendations

Try asking about: AI tools, DevOps, Music Production, CAD, Security, or any topic in the repo!`
};

function getAIResponse(message: string): string {
  const lower = message.toLowerCase();
  if (lower.includes('ai') || lower.includes('llm') || lower.includes('copilot') || lower.includes('coding')) {
    return SAMPLE_RESPONSES['ai'];
  }
  if (lower.includes('devops') || lower.includes('docker') || lower.includes('kubernetes') || lower.includes('ci/cd')) {
    return SAMPLE_RESPONSES['devops'];
  }
  if (lower.includes('music') || lower.includes('audio') || lower.includes('daw') || lower.includes('production')) {
    return SAMPLE_RESPONSES['music'];
  }
  if (lower.includes('cad') || lower.includes('3d') || lower.includes('blender') || lower.includes('modeling')) {
    return SAMPLE_RESPONSES['cad'];
  }
  if (lower.includes('help') || lower.includes('what can') || lower.includes('how do')) {
    return SAMPLE_RESPONSES['help'];
  }
  return SAMPLE_RESPONSES['default'];
}

export default function RepoPilot() {
  const [messages, setMessages] = useState<Message[]>([
    {
      id: 0,
      role: 'system',
      content: '🧠 RepoMind AI Co-Pilot initialized. I have context awareness of the entire DevTeam6 repository.',
      timestamp: new Date()
    }
  ]);
  const [input, setInput] = useState('');
  const [isTyping, setIsTyping] = useState(false);
  const messagesEndRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);

  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  };

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!input.trim()) return;

    const userMessage: Message = {
      id: messages.length,
      role: 'user',
      content: input,
      timestamp: new Date()
    };

    setMessages(prev => [...prev, userMessage]);
    setInput('');
    setIsTyping(true);

    // Simulate AI response delay
    setTimeout(() => {
      const response = getAIResponse(input);
      const assistantMessage: Message = {
        id: messages.length + 1,
        role: 'assistant',
        content: response,
        timestamp: new Date()
      };
      setMessages(prev => [...prev, assistantMessage]);
      setIsTyping(false);
    }, 1000 + Math.random() * 1000);
  };

  const quickActions = [
    { label: '🤖 AI Tools', query: 'What AI tools are available?' },
    { label: '🐳 DevOps', query: 'Tell me about DevOps resources' },
    { label: '🎵 Music', query: 'What music production tools are here?' },
    { label: '📐 CAD', query: 'Show me CAD and 3D tools' },
    { label: '❓ Help', query: 'What can you help me with?' }
  ];

  return (
    <div style={{
      minHeight: '100vh',
      background: 'linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0a1a2a 100%)',
      padding: '20px',
      fontFamily: "'JetBrains Mono', 'Fira Code', monospace"
    }}>
      {/* Header */}
      <div style={{
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: '20px',
        padding: '15px 20px',
        background: 'rgba(0, 255, 255, 0.05)',
        borderRadius: '15px',
        border: '1px solid rgba(0, 255, 255, 0.2)'
      }}>
        <div style={{ display: 'flex', alignItems: 'center', gap: '15px' }}>
          <div style={{
            width: '50px',
            height: '50px',
            borderRadius: '50%',
            background: 'linear-gradient(135deg, #00ffff, #ff00ff)',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            fontSize: '24px',
            boxShadow: '0 0 20px rgba(0, 255, 255, 0.5)'
          }}>
            🧠
          </div>
          <div>
            <h1 style={{
              color: '#00ffff',
              margin: 0,
              fontSize: '24px',
              textShadow: '0 0 10px rgba(0, 255, 255, 0.5)'
            }}>
              RepoMind AI Co-Pilot
            </h1>
            <p style={{ color: '#888', margin: 0, fontSize: '12px' }}>
              Your intelligent guide to DevTeam6
            </p>
          </div>
        </div>
        <Link to="/DevTeam6/" style={{
          padding: '10px 20px',
          background: 'rgba(255, 0, 255, 0.2)',
          border: '1px solid #ff00ff',
          borderRadius: '8px',
          color: '#ff00ff',
          textDecoration: 'none',
          fontSize: '14px',
          transition: 'all 0.3s'
        }}>
          ← Back to Home
        </Link>
      </div>

      {/* Main Chat Container */}
      <div style={{
        display: 'grid',
        gridTemplateColumns: '1fr 300px',
        gap: '20px',
        height: 'calc(100vh - 160px)'
      }}>
        {/* Chat Area */}
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          background: 'rgba(0, 0, 0, 0.3)',
          borderRadius: '15px',
          border: '1px solid rgba(0, 255, 255, 0.2)',
          overflow: 'hidden'
        }}>
          {/* Messages */}
          <div style={{
            flex: 1,
            overflowY: 'auto',
            padding: '20px',
            display: 'flex',
            flexDirection: 'column',
            gap: '15px'
          }}>
            {messages.map((msg) => (
              <div
                key={msg.id}
                style={{
                  display: 'flex',
                  justifyContent: msg.role === 'user' ? 'flex-end' : 'flex-start'
                }}
              >
                <div style={{
                  maxWidth: '80%',
                  padding: '15px',
                  borderRadius: '15px',
                  background: msg.role === 'user'
                    ? 'linear-gradient(135deg, rgba(255, 0, 255, 0.3), rgba(255, 0, 255, 0.1))'
                    : msg.role === 'system'
                    ? 'rgba(255, 255, 0, 0.1)'
                    : 'linear-gradient(135deg, rgba(0, 255, 255, 0.2), rgba(0, 255, 255, 0.05))',
                  border: `1px solid ${msg.role === 'user' ? '#ff00ff' : msg.role === 'system' ? '#ffff00' : '#00ffff'}`,
                  color: '#fff',
                  whiteSpace: 'pre-wrap'
                }}>
                  {msg.role === 'assistant' && (
                    <div style={{
                      display: 'flex',
                      alignItems: 'center',
                      gap: '8px',
                      marginBottom: '10px',
                      paddingBottom: '10px',
                      borderBottom: '1px solid rgba(0, 255, 255, 0.2)'
                    }}>
                      <span style={{ fontSize: '20px' }}>🧠</span>
                      <span style={{ color: '#00ffff', fontWeight: 'bold' }}>RepoMind</span>
                    </div>
                  )}
                  {msg.content}
                </div>
              </div>
            ))}
            {isTyping && (
              <div style={{
                display: 'flex',
                gap: '5px',
                padding: '15px',
                background: 'rgba(0, 255, 255, 0.1)',
                borderRadius: '15px',
                width: 'fit-content'
              }}>
                <span style={{ animation: 'pulse 1s infinite', color: '#00ffff' }}>●</span>
                <span style={{ animation: 'pulse 1s infinite 0.2s', color: '#00ffff' }}>●</span>
                <span style={{ animation: 'pulse 1s infinite 0.4s', color: '#00ffff' }}>●</span>
              </div>
            )}
            <div ref={messagesEndRef} />
          </div>

          {/* Input Area */}
          <form onSubmit={handleSubmit} style={{
            padding: '20px',
            borderTop: '1px solid rgba(0, 255, 255, 0.2)',
            display: 'flex',
            gap: '10px'
          }}>
            <input
              ref={inputRef}
              type="text"
              value={input}
              onChange={(e) => setInput(e.target.value)}
              placeholder="Ask RepoMind anything about DevTeam6..."
              style={{
                flex: 1,
                padding: '15px',
                background: 'rgba(0, 0, 0, 0.5)',
                border: '1px solid rgba(0, 255, 255, 0.3)',
                borderRadius: '10px',
                color: '#fff',
                fontSize: '14px',
                outline: 'none'
              }}
            />
            <button
              type="submit"
              style={{
                padding: '15px 30px',
                background: 'linear-gradient(135deg, #00ffff, #ff00ff)',
                border: 'none',
                borderRadius: '10px',
                color: '#000',
                fontWeight: 'bold',
                cursor: 'pointer',
                fontSize: '14px',
                transition: 'all 0.3s'
              }}
            >
              Send →
            </button>
          </form>
        </div>

        {/* Sidebar */}
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          gap: '15px'
        }}>
          {/* Quick Actions */}
          <div style={{
            background: 'rgba(0, 0, 0, 0.3)',
            borderRadius: '15px',
            border: '1px solid rgba(0, 255, 255, 0.2)',
            padding: '20px'
          }}>
            <h3 style={{ color: '#00ffff', margin: '0 0 15px 0', fontSize: '16px' }}>
              ⚡ Quick Actions
            </h3>
            <div style={{ display: 'flex', flexDirection: 'column', gap: '10px' }}>
              {quickActions.map((action, i) => (
                <button
                  key={i}
                  onClick={() => {
                    setInput(action.query);
                    inputRef.current?.focus();
                  }}
                  style={{
                    padding: '12px',
                    background: 'rgba(0, 255, 255, 0.1)',
                    border: '1px solid rgba(0, 255, 255, 0.3)',
                    borderRadius: '8px',
                    color: '#fff',
                    cursor: 'pointer',
                    textAlign: 'left',
                    transition: 'all 0.3s',
                    fontSize: '14px'
                  }}
                >
                  {action.label}
                </button>
              ))}
            </div>
          </div>

          {/* Context Awareness */}
          <div style={{
            background: 'rgba(0, 0, 0, 0.3)',
            borderRadius: '15px',
            border: '1px solid rgba(255, 0, 255, 0.2)',
            padding: '20px'
          }}>
            <h3 style={{ color: '#ff00ff', margin: '0 0 15px 0', fontSize: '16px' }}>
              📊 Repo Context
            </h3>
            <div style={{ color: '#888', fontSize: '12px', lineHeight: '1.8' }}>
              <div>📁 1000+ Resources</div>
              <div>🤖 500+ AI Tools</div>
              <div>📚 300+ Awesome Lists</div>
              <div>🎮 Gamification Active</div>
              <div>🌐 3D Experience Loaded</div>
            </div>
          </div>

          {/* Powered By */}
          <div style={{
            background: 'rgba(0, 0, 0, 0.3)',
            borderRadius: '15px',
            border: '1px solid rgba(255, 255, 0, 0.2)',
            padding: '20px',
            textAlign: 'center'
          }}>
            <div style={{ color: '#ffff00', fontSize: '12px', marginBottom: '10px' }}>
              Powered by
            </div>
            <a
              href="https://github.com/403errors/repomind"
              target="_blank"
              rel="noopener noreferrer"
              style={{
                color: '#00ffff',
                textDecoration: 'none',
                fontWeight: 'bold',
                fontSize: '16px'
              }}
            >
              🧠 Repomind
            </a>
          </div>
        </div>
      </div>
    </div>
  );
}
