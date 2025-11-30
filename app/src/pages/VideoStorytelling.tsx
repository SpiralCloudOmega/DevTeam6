import { useState, useRef, useEffect } from 'react';

interface VideoChapter {
  id: string;
  title: string;
  description: string;
  timestamp: number; // in seconds
  duration: number;
  icon: string;
  color: string;
  tags: string[];
}

interface VideoContent {
  id: string;
  title: string;
  description: string;
  thumbnail: string;
  videoUrl?: string;
  embedType: 'youtube' | 'vimeo' | 'direct' | 'demo';
  embedId?: string;
  duration: string;
  chapters: VideoChapter[];
  category: 'tutorial' | 'demo' | 'overview' | 'deep-dive';
}

// DevTeam6 Video Content Library
const videoLibrary: VideoContent[] = [
  {
    id: 'intro',
    title: 'Welcome to DevTeam6',
    description: 'An overview of the ultimate software development encyclopedia - featuring 1000+ curated resources, AI tools, and developer essentials.',
    thumbnail: 'https://images.unsplash.com/photo-1550439062-609e1531270e?w=800&h=450&fit=crop',
    embedType: 'demo',
    duration: '5:30',
    category: 'overview',
    chapters: [
      { id: 'intro-1', title: 'What is DevTeam6?', description: 'Introduction to the Omega Tool Kit', timestamp: 0, duration: 60, icon: '🚀', color: '#00f0ff', tags: ['intro', 'overview'] },
      { id: 'intro-2', title: 'Resource Categories', description: 'AI, DevOps, Cloud, Security & more', timestamp: 60, duration: 90, icon: '📚', color: '#ff00ff', tags: ['resources', 'categories'] },
      { id: 'intro-3', title: 'Interactive Features', description: '3D Demo, GenUI, Gamification', timestamp: 150, duration: 80, icon: '🎮', color: '#00ff88', tags: ['interactive', 'features'] },
      { id: 'intro-4', title: 'Getting Started', description: 'How to use this repository', timestamp: 230, duration: 100, icon: '🛠️', color: '#ff6600', tags: ['start', 'guide'] },
    ]
  },
  {
    id: 'ai-tools',
    title: 'AI Tools Deep Dive',
    description: 'Explore the latest AI coding assistants including GPT-5.1, Claude Opus 4.5, Gemini 3, and local LLM solutions.',
    thumbnail: 'https://images.unsplash.com/photo-1677442136019-21780ecad995?w=800&h=450&fit=crop',
    embedType: 'demo',
    duration: '12:45',
    category: 'deep-dive',
    chapters: [
      { id: 'ai-1', title: 'AI Coding Assistants', description: 'Cursor, Copilot, Claude Code, Windsurf', timestamp: 0, duration: 180, icon: '🤖', color: '#00f0ff', tags: ['ai', 'coding'] },
      { id: 'ai-2', title: 'Local LLMs', description: 'Ollama, LM Studio, GPT4All', timestamp: 180, duration: 200, icon: '💻', color: '#ff00ff', tags: ['local', 'llm'] },
      { id: 'ai-3', title: 'API Integration', description: 'OpenAI, Anthropic, Google APIs', timestamp: 380, duration: 185, icon: '🔗', color: '#7b2fff', tags: ['api', 'integration'] },
    ]
  },
  {
    id: '3d-demo',
    title: 'Interactive 3D Experience',
    description: 'Behind the scenes of the Three.js powered 3D demo with React Three Fiber integration.',
    thumbnail: 'https://images.unsplash.com/photo-1633356122102-3fe601e05bd2?w=800&h=450&fit=crop',
    embedType: 'demo',
    duration: '8:20',
    category: 'tutorial',
    chapters: [
      { id: '3d-1', title: 'Three.js Basics', description: 'Scene, Camera, Renderer', timestamp: 0, duration: 120, icon: '🎮', color: '#00f0ff', tags: ['threejs', 'basics'] },
      { id: '3d-2', title: 'React Three Fiber', description: 'Declarative 3D in React', timestamp: 120, duration: 150, icon: '⚛️', color: '#00ff88', tags: ['r3f', 'react'] },
      { id: '3d-3', title: 'Cyberpunk Effects', description: 'Glow, particles, animations', timestamp: 270, duration: 130, icon: '✨', color: '#ff00ff', tags: ['effects', 'cyberpunk'] },
      { id: '3d-4', title: 'Deployment', description: 'GitHub Pages CI/CD', timestamp: 400, duration: 100, icon: '🚀', color: '#ff6600', tags: ['deploy', 'cicd'] },
    ]
  },
  {
    id: 'genui',
    title: 'GenUI Playground Tutorial',
    description: 'Learn how to generate React components using natural language prompts in the Generative UI Playground.',
    thumbnail: 'https://images.unsplash.com/photo-1555066931-4365d14bab8c?w=800&h=450&fit=crop',
    embedType: 'demo',
    duration: '6:15',
    category: 'tutorial',
    chapters: [
      { id: 'genui-1', title: 'Natural Language Input', description: 'Describe your component', timestamp: 0, duration: 90, icon: '💬', color: '#7b2fff', tags: ['nlp', 'input'] },
      { id: 'genui-2', title: 'Template Selection', description: 'Choose from 10+ templates', timestamp: 90, duration: 100, icon: '📋', color: '#00f0ff', tags: ['templates', 'select'] },
      { id: 'genui-3', title: 'Customization', description: 'Colors, frameworks, styles', timestamp: 190, duration: 95, icon: '🎨', color: '#ff00ff', tags: ['custom', 'style'] },
      { id: 'genui-4', title: 'Export Code', description: 'Copy and use in your project', timestamp: 285, duration: 90, icon: '📦', color: '#00ff88', tags: ['export', 'code'] },
    ]
  },
  {
    id: 'gamification',
    title: 'Gamification System',
    description: 'Discover the contributor leaderboard, achievements, and how to earn XP in the DevTeam6 community.',
    thumbnail: 'https://images.unsplash.com/photo-1511882150382-421056c89033?w=800&h=450&fit=crop',
    embedType: 'demo',
    duration: '4:50',
    category: 'demo',
    chapters: [
      { id: 'game-1', title: 'Leaderboard', description: 'Track top contributors', timestamp: 0, duration: 80, icon: '🏆', color: '#ffcc00', tags: ['leaderboard', 'ranking'] },
      { id: 'game-2', title: 'Achievements', description: 'Collect badges and rewards', timestamp: 80, duration: 90, icon: '🎖️', color: '#ff6600', tags: ['badges', 'rewards'] },
      { id: 'game-3', title: 'XP System', description: 'Earn points for contributions', timestamp: 170, duration: 70, icon: '⭐', color: '#aa00ff', tags: ['xp', 'points'] },
      { id: 'game-4', title: 'Community', description: 'Join the contributor network', timestamp: 240, duration: 50, icon: '👥', color: '#00ff88', tags: ['community', 'network'] },
    ]
  },
];

// Story Timeline Card Component
const StoryCard = ({ chapter, isActive, onClick, index }: { 
  chapter: VideoChapter; 
  isActive: boolean; 
  onClick: () => void;
  index: number;
}) => (
  <div
    onClick={onClick}
    style={{
      display: 'flex',
      gap: '15px',
      padding: '15px',
      background: isActive 
        ? `linear-gradient(135deg, ${chapter.color}30, transparent)` 
        : 'rgba(10, 10, 26, 0.5)',
      border: `1px solid ${isActive ? chapter.color : 'rgba(255,255,255,0.1)'}`,
      borderRadius: '12px',
      cursor: 'pointer',
      transition: 'all 0.3s ease',
      transform: isActive ? 'scale(1.02)' : 'scale(1)',
      boxShadow: isActive ? `0 0 20px ${chapter.color}40` : 'none',
    }}
  >
    {/* Timeline marker */}
    <div style={{
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      gap: '5px',
    }}>
      <div style={{
        width: '40px',
        height: '40px',
        borderRadius: '50%',
        background: isActive ? chapter.color : 'rgba(255,255,255,0.1)',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        fontSize: '20px',
        transition: 'all 0.3s ease',
      }}>
        {chapter.icon}
      </div>
      <div style={{
        fontSize: '11px',
        color: chapter.color,
        fontWeight: 'bold',
      }}>
        {Math.floor(chapter.timestamp / 60)}:{(chapter.timestamp % 60).toString().padStart(2, '0')}
      </div>
    </div>
    
    {/* Content */}
    <div style={{ flex: 1 }}>
      <h4 style={{
        margin: '0 0 5px 0',
        color: isActive ? chapter.color : '#ffffff',
        fontSize: '14px',
      }}>
        {index + 1}. {chapter.title}
      </h4>
      <p style={{
        margin: '0 0 8px 0',
        fontSize: '12px',
        opacity: 0.7,
        lineHeight: 1.4,
      }}>
        {chapter.description}
      </p>
      <div style={{ display: 'flex', gap: '5px', flexWrap: 'wrap' }}>
        {chapter.tags.map(tag => (
          <span
            key={tag}
            style={{
              padding: '2px 8px',
              background: `${chapter.color}20`,
              border: `1px solid ${chapter.color}50`,
              borderRadius: '10px',
              fontSize: '10px',
              color: chapter.color,
            }}
          >
            #{tag}
          </span>
        ))}
      </div>
    </div>
    
    {/* Duration */}
    <div style={{
      fontSize: '11px',
      opacity: 0.7,
      whiteSpace: 'nowrap',
    }}>
      {Math.floor(chapter.duration / 60)}:{(chapter.duration % 60).toString().padStart(2, '0')}
    </div>
  </div>
);

// Demo Video Player (Animated Placeholder)
const DemoVideoPlayer = ({ video, currentChapter, onChapterChange }: {
  video: VideoContent;
  currentChapter: number;
  onChapterChange: (index: number) => void;
}) => {
  const [isPlaying, setIsPlaying] = useState(false);
  const [progress, setProgress] = useState(0);
  const intervalRef = useRef<number | null>(null);
  
  const chapter = video.chapters[currentChapter];
  const totalDuration = video.chapters.reduce((sum, ch) => sum + ch.duration, 0);
  const currentTime = video.chapters.slice(0, currentChapter).reduce((sum, ch) => sum + ch.duration, 0) + 
    (progress / 100) * chapter.duration;
  
  useEffect(() => {
    if (isPlaying) {
      intervalRef.current = window.setInterval(() => {
        setProgress(prev => {
          if (prev >= 100) {
            // Move to next chapter
            if (currentChapter < video.chapters.length - 1) {
              onChapterChange(currentChapter + 1);
              return 0;
            } else {
              setIsPlaying(false);
              return 100;
            }
          }
          return prev + (100 / chapter.duration) * 0.1; // Update every 100ms
        });
      }, 100);
    } else if (intervalRef.current) {
      clearInterval(intervalRef.current);
    }
    
    return () => {
      if (intervalRef.current) clearInterval(intervalRef.current);
    };
  }, [isPlaying, currentChapter, chapter.duration, video.chapters.length, onChapterChange]);
  
  return (
    <div style={{
      position: 'relative',
      borderRadius: '16px',
      overflow: 'hidden',
      background: '#0a0a1a',
      border: '2px solid rgba(0, 240, 255, 0.3)',
    }}>
      {/* Video Area */}
      <div style={{
        aspectRatio: '16/9',
        background: `linear-gradient(135deg, ${chapter.color}10, #0a0a1a)`,
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        position: 'relative',
        overflow: 'hidden',
      }}>
        {/* Animated Background */}
        <div style={{
          position: 'absolute',
          inset: 0,
          background: `url(${video.thumbnail})`,
          backgroundSize: 'cover',
          backgroundPosition: 'center',
          opacity: 0.3,
          filter: 'blur(2px)',
        }}/>
        
        {/* Floating particles animation */}
        {isPlaying && [...Array(15)].map((_, i) => (
          <div
            key={i}
            style={{
              position: 'absolute',
              width: '6px',
              height: '6px',
              borderRadius: '50%',
              background: chapter.color,
              left: `${Math.random() * 100}%`,
              top: `${Math.random() * 100}%`,
              animation: `float ${2 + Math.random() * 3}s ease-in-out infinite`,
              animationDelay: `${Math.random() * 2}s`,
              opacity: 0.6,
            }}
          />
        ))}
        
        {/* Chapter Info */}
        <div style={{
          position: 'relative',
          zIndex: 10,
          textAlign: 'center',
          padding: '20px',
        }}>
          <div style={{
            fontSize: '60px',
            marginBottom: '15px',
            animation: isPlaying ? 'pulse 2s ease-in-out infinite' : 'none',
          }}>
            {chapter.icon}
          </div>
          <h2 style={{
            margin: '0 0 10px 0',
            fontSize: '28px',
            color: chapter.color,
            textShadow: `0 0 20px ${chapter.color}`,
          }}>
            {chapter.title}
          </h2>
          <p style={{
            margin: 0,
            fontSize: '16px',
            opacity: 0.8,
            maxWidth: '400px',
          }}>
            {chapter.description}
          </p>
        </div>
        
        {/* Play button overlay */}
        {!isPlaying && (
          <button
            onClick={() => setIsPlaying(true)}
            style={{
              position: 'absolute',
              width: '80px',
              height: '80px',
              borderRadius: '50%',
              background: `${chapter.color}`,
              border: 'none',
              cursor: 'pointer',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              fontSize: '30px',
              color: '#0a0a1a',
              boxShadow: `0 0 30px ${chapter.color}80`,
              transition: 'transform 0.3s ease',
            }}
          >
            ▶
          </button>
        )}
      </div>
      
      {/* Controls */}
      <div style={{
        padding: '15px 20px',
        background: 'rgba(10, 10, 26, 0.95)',
        borderTop: '1px solid rgba(255,255,255,0.1)',
      }}>
        {/* Progress bar */}
        <div style={{
          height: '6px',
          background: 'rgba(255,255,255,0.1)',
          borderRadius: '3px',
          marginBottom: '15px',
          overflow: 'hidden',
          position: 'relative',
        }}>
          {/* Chapter markers */}
          {video.chapters.map((ch, idx) => {
            const chapterStart = video.chapters.slice(0, idx).reduce((sum, c) => sum + c.duration, 0);
            return (
              <div
                key={ch.id}
                style={{
                  position: 'absolute',
                  left: `${(chapterStart / totalDuration) * 100}%`,
                  top: 0,
                  bottom: 0,
                  width: '2px',
                  background: ch.color,
                  opacity: 0.5,
                }}
              />
            );
          })}
          
          {/* Progress fill */}
          <div style={{
            height: '100%',
            background: `linear-gradient(90deg, ${chapter.color}, ${chapter.color}80)`,
            width: `${(currentTime / totalDuration) * 100}%`,
            transition: 'width 0.1s linear',
            borderRadius: '3px',
          }}/>
        </div>
        
        {/* Control buttons */}
        <div style={{
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'space-between',
        }}>
          <div style={{ display: 'flex', gap: '10px', alignItems: 'center' }}>
            <button
              onClick={() => {
                if (currentChapter > 0) {
                  onChapterChange(currentChapter - 1);
                  setProgress(0);
                }
              }}
              style={{
                width: '40px',
                height: '40px',
                borderRadius: '50%',
                background: 'rgba(0, 240, 255, 0.2)',
                border: '1px solid #00f0ff',
                color: '#00f0ff',
                cursor: 'pointer',
                fontSize: '16px',
              }}
            >
              ⏮
            </button>
            
            <button
              onClick={() => setIsPlaying(!isPlaying)}
              style={{
                width: '50px',
                height: '50px',
                borderRadius: '50%',
                background: chapter.color,
                border: 'none',
                color: '#0a0a1a',
                cursor: 'pointer',
                fontSize: '20px',
                fontWeight: 'bold',
              }}
            >
              {isPlaying ? '⏸' : '▶'}
            </button>
            
            <button
              onClick={() => {
                if (currentChapter < video.chapters.length - 1) {
                  onChapterChange(currentChapter + 1);
                  setProgress(0);
                }
              }}
              style={{
                width: '40px',
                height: '40px',
                borderRadius: '50%',
                background: 'rgba(255, 0, 255, 0.2)',
                border: '1px solid #ff00ff',
                color: '#ff00ff',
                cursor: 'pointer',
                fontSize: '16px',
              }}
            >
              ⏭
            </button>
          </div>
          
          <div style={{
            fontSize: '14px',
            fontFamily: 'monospace',
            color: chapter.color,
          }}>
            {Math.floor(currentTime / 60)}:{Math.floor(currentTime % 60).toString().padStart(2, '0')} / {Math.floor(totalDuration / 60)}:{(totalDuration % 60).toString().padStart(2, '0')}
          </div>
          
          <div style={{ display: 'flex', gap: '10px', alignItems: 'center' }}>
            <span style={{
              padding: '5px 12px',
              background: `${chapter.color}30`,
              border: `1px solid ${chapter.color}`,
              borderRadius: '15px',
              fontSize: '12px',
              color: chapter.color,
            }}>
              Chapter {currentChapter + 1}/{video.chapters.length}
            </span>
          </div>
        </div>
      </div>
      
      {/* CSS Animations */}
      <style>{`
        @keyframes float {
          0%, 100% { transform: translateY(0) scale(1); opacity: 0.6; }
          50% { transform: translateY(-20px) scale(1.2); opacity: 1; }
        }
        @keyframes pulse {
          0%, 100% { transform: scale(1); }
          50% { transform: scale(1.1); }
        }
      `}</style>
    </div>
  );
};

// Main Component
export default function VideoStorytelling() {
  const [selectedVideo, setSelectedVideo] = useState<VideoContent>(videoLibrary[0]);
  const [currentChapter, setCurrentChapter] = useState(0);
  const [viewMode, setViewMode] = useState<'player' | 'library'>('player');
  
  const categoryColors: Record<string, string> = {
    tutorial: '#00f0ff',
    demo: '#ff00ff',
    overview: '#00ff88',
    'deep-dive': '#7b2fff',
  };
  
  return (
    <div style={{
      minHeight: '100vh',
      background: 'linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0a0a1a 100%)',
      color: '#ffffff',
      fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
    }}>
      {/* Header */}
      <header style={{
        padding: '20px 40px',
        borderBottom: '1px solid rgba(0, 240, 255, 0.3)',
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center',
        background: 'rgba(10, 10, 26, 0.9)',
        backdropFilter: 'blur(10px)',
      }}>
        <div>
          <h1 style={{
            margin: 0,
            fontSize: '28px',
            background: 'linear-gradient(90deg, #00f0ff, #ff00ff, #00ff88)',
            WebkitBackgroundClip: 'text',
            WebkitTextFillColor: 'transparent',
          }}>
            🎬 Video Storytelling
          </h1>
          <p style={{ margin: '5px 0 0', opacity: 0.7, fontSize: '14px' }}>
            Interactive video guides with chapter navigation
          </p>
        </div>
        
        <div style={{ display: 'flex', gap: '10px' }}>
          <a 
            href="/DevTeam6/" 
            style={{
              padding: '10px 20px',
              background: 'rgba(0, 240, 255, 0.2)',
              border: '1px solid #00f0ff',
              borderRadius: '8px',
              color: '#00f0ff',
              textDecoration: 'none',
              fontSize: '14px',
            }}
          >
            🏠 Home
          </a>
          
          <button
            onClick={() => setViewMode(viewMode === 'player' ? 'library' : 'player')}
            style={{
              padding: '10px 20px',
              background: viewMode === 'player' ? 'rgba(255, 0, 255, 0.2)' : 'rgba(0, 255, 136, 0.2)',
              border: `1px solid ${viewMode === 'player' ? '#ff00ff' : '#00ff88'}`,
              borderRadius: '8px',
              color: viewMode === 'player' ? '#ff00ff' : '#00ff88',
              cursor: 'pointer',
              fontSize: '14px',
            }}
          >
            {viewMode === 'player' ? '📚 Library' : '▶️ Player'}
          </button>
        </div>
      </header>
      
      {viewMode === 'player' ? (
        <div style={{
          display: 'grid',
          gridTemplateColumns: '1fr 400px',
          gap: '30px',
          padding: '30px 40px',
          maxWidth: '1600px',
          margin: '0 auto',
        }}>
          {/* Main Video Area */}
          <div>
            <DemoVideoPlayer 
              video={selectedVideo}
              currentChapter={currentChapter}
              onChapterChange={setCurrentChapter}
            />
            
            {/* Video Info */}
            <div style={{
              marginTop: '20px',
              padding: '20px',
              background: 'rgba(10, 10, 26, 0.8)',
              borderRadius: '12px',
              border: '1px solid rgba(0, 240, 255, 0.2)',
            }}>
              <div style={{
                display: 'flex',
                justifyContent: 'space-between',
                alignItems: 'flex-start',
                marginBottom: '15px',
              }}>
                <div>
                  <span style={{
                    padding: '4px 12px',
                    background: `${categoryColors[selectedVideo.category]}30`,
                    border: `1px solid ${categoryColors[selectedVideo.category]}`,
                    borderRadius: '15px',
                    fontSize: '11px',
                    color: categoryColors[selectedVideo.category],
                    textTransform: 'uppercase',
                    marginBottom: '10px',
                    display: 'inline-block',
                  }}>
                    {selectedVideo.category}
                  </span>
                  <h2 style={{ margin: '10px 0 5px', color: '#00f0ff' }}>{selectedVideo.title}</h2>
                  <p style={{ margin: 0, opacity: 0.7, fontSize: '14px', lineHeight: 1.6 }}>
                    {selectedVideo.description}
                  </p>
                </div>
                <div style={{
                  padding: '10px 15px',
                  background: 'rgba(255, 0, 255, 0.2)',
                  borderRadius: '8px',
                  textAlign: 'center',
                }}>
                  <div style={{ fontSize: '12px', opacity: 0.7 }}>Duration</div>
                  <div style={{ fontSize: '18px', fontWeight: 'bold', color: '#ff00ff' }}>{selectedVideo.duration}</div>
                </div>
              </div>
              
              {/* Chapter quick jump */}
              <div style={{
                display: 'flex',
                gap: '8px',
                flexWrap: 'wrap',
              }}>
                {selectedVideo.chapters.map((ch, idx) => (
                  <button
                    key={ch.id}
                    onClick={() => setCurrentChapter(idx)}
                    style={{
                      padding: '8px 15px',
                      background: currentChapter === idx ? ch.color : 'rgba(255,255,255,0.05)',
                      border: `1px solid ${ch.color}`,
                      borderRadius: '20px',
                      color: currentChapter === idx ? '#0a0a1a' : ch.color,
                      cursor: 'pointer',
                      fontSize: '12px',
                      display: 'flex',
                      alignItems: 'center',
                      gap: '5px',
                      transition: 'all 0.2s ease',
                    }}
                  >
                    {ch.icon} {ch.title}
                  </button>
                ))}
              </div>
            </div>
          </div>
          
          {/* Chapter Timeline Sidebar */}
          <div style={{
            background: 'rgba(10, 10, 26, 0.8)',
            borderRadius: '16px',
            border: '1px solid rgba(0, 240, 255, 0.2)',
            padding: '20px',
            height: 'fit-content',
            maxHeight: 'calc(100vh - 200px)',
            overflow: 'auto',
          }}>
            <h3 style={{
              margin: '0 0 20px 0',
              color: '#00f0ff',
              fontSize: '16px',
              display: 'flex',
              alignItems: 'center',
              gap: '10px',
            }}>
              📖 Chapter Timeline
              <span style={{
                padding: '3px 8px',
                background: 'rgba(255, 0, 255, 0.2)',
                borderRadius: '10px',
                fontSize: '11px',
                color: '#ff00ff',
              }}>
                {selectedVideo.chapters.length} chapters
              </span>
            </h3>
            
            <div style={{ display: 'flex', flexDirection: 'column', gap: '12px' }}>
              {selectedVideo.chapters.map((chapter, idx) => (
                <StoryCard
                  key={chapter.id}
                  chapter={chapter}
                  isActive={currentChapter === idx}
                  onClick={() => setCurrentChapter(idx)}
                  index={idx}
                />
              ))}
            </div>
          </div>
        </div>
      ) : (
        /* Library View */
        <div style={{ padding: '30px 40px', maxWidth: '1400px', margin: '0 auto' }}>
          <h2 style={{
            margin: '0 0 30px 0',
            fontSize: '24px',
            color: '#00f0ff',
          }}>
            📚 Video Library
            <span style={{
              marginLeft: '15px',
              padding: '5px 12px',
              background: 'rgba(255, 0, 255, 0.2)',
              borderRadius: '15px',
              fontSize: '14px',
              color: '#ff00ff',
            }}>
              {videoLibrary.length} videos
            </span>
          </h2>
          
          {/* Category Filter */}
          <div style={{
            display: 'flex',
            gap: '10px',
            marginBottom: '30px',
          }}>
            {['all', 'tutorial', 'demo', 'overview', 'deep-dive'].map(cat => (
              <button
                key={cat}
                style={{
                  padding: '10px 20px',
                  background: cat === 'all' ? 'rgba(0, 240, 255, 0.3)' : `${categoryColors[cat] || '#00f0ff'}20`,
                  border: `1px solid ${categoryColors[cat] || '#00f0ff'}`,
                  borderRadius: '20px',
                  color: categoryColors[cat] || '#00f0ff',
                  cursor: 'pointer',
                  fontSize: '13px',
                  textTransform: 'capitalize',
                }}
              >
                {cat === 'all' ? '🎬 All Videos' : cat.replace('-', ' ')}
              </button>
            ))}
          </div>
          
          {/* Video Grid */}
          <div style={{
            display: 'grid',
            gridTemplateColumns: 'repeat(auto-fill, minmax(350px, 1fr))',
            gap: '25px',
          }}>
            {videoLibrary.map(video => (
              <div
                key={video.id}
                onClick={() => {
                  setSelectedVideo(video);
                  setCurrentChapter(0);
                  setViewMode('player');
                }}
                style={{
                  background: 'rgba(10, 10, 26, 0.8)',
                  borderRadius: '16px',
                  overflow: 'hidden',
                  border: '1px solid rgba(0, 240, 255, 0.2)',
                  cursor: 'pointer',
                  transition: 'all 0.3s ease',
                }}
              >
                {/* Thumbnail */}
                <div style={{
                  aspectRatio: '16/9',
                  background: `url(${video.thumbnail})`,
                  backgroundSize: 'cover',
                  backgroundPosition: 'center',
                  position: 'relative',
                }}>
                  <div style={{
                    position: 'absolute',
                    inset: 0,
                    background: 'linear-gradient(to top, rgba(10,10,26,0.9), transparent)',
                  }}/>
                  <div style={{
                    position: 'absolute',
                    bottom: '15px',
                    left: '15px',
                    right: '15px',
                    display: 'flex',
                    justifyContent: 'space-between',
                    alignItems: 'flex-end',
                  }}>
                    <span style={{
                      padding: '5px 12px',
                      background: `${categoryColors[video.category]}`,
                      borderRadius: '15px',
                      fontSize: '11px',
                      color: '#0a0a1a',
                      fontWeight: 'bold',
                      textTransform: 'uppercase',
                    }}>
                      {video.category}
                    </span>
                    <span style={{
                      padding: '5px 10px',
                      background: 'rgba(0,0,0,0.7)',
                      borderRadius: '5px',
                      fontSize: '12px',
                    }}>
                      {video.duration}
                    </span>
                  </div>
                  
                  {/* Play overlay */}
                  <div style={{
                    position: 'absolute',
                    top: '50%',
                    left: '50%',
                    transform: 'translate(-50%, -50%)',
                    width: '60px',
                    height: '60px',
                    borderRadius: '50%',
                    background: 'rgba(0, 240, 255, 0.8)',
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                    fontSize: '24px',
                    color: '#0a0a1a',
                    opacity: 0.9,
                  }}>
                    ▶
                  </div>
                </div>
                
                {/* Info */}
                <div style={{ padding: '20px' }}>
                  <h3 style={{
                    margin: '0 0 10px 0',
                    fontSize: '18px',
                    color: '#ffffff',
                  }}>
                    {video.title}
                  </h3>
                  <p style={{
                    margin: '0 0 15px 0',
                    fontSize: '13px',
                    opacity: 0.7,
                    lineHeight: 1.5,
                    display: '-webkit-box',
                    WebkitLineClamp: 2,
                    WebkitBoxOrient: 'vertical',
                    overflow: 'hidden',
                  }}>
                    {video.description}
                  </p>
                  <div style={{
                    display: 'flex',
                    justifyContent: 'space-between',
                    alignItems: 'center',
                  }}>
                    <span style={{
                      fontSize: '12px',
                      color: '#00ff88',
                    }}>
                      📖 {video.chapters.length} chapters
                    </span>
                    <button style={{
                      padding: '8px 15px',
                      background: 'linear-gradient(90deg, #00f0ff, #ff00ff)',
                      border: 'none',
                      borderRadius: '20px',
                      color: '#ffffff',
                      fontSize: '12px',
                      fontWeight: 'bold',
                      cursor: 'pointer',
                    }}>
                      Watch Now →
                    </button>
                  </div>
                </div>
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}
