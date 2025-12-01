import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars, Float } from '@react-three/drei'
import { Suspense, useRef, useState, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

// Parametric Wave Background - Creates flowing wave mesh
function ParametricBackground() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  const { positions, indices } = useMemo(() => {
    const width = 40
    const height = 40
    const segments = 80
    const positions = new Float32Array((segments + 1) * (segments + 1) * 3)
    const indices: number[] = []
    
    for (let y = 0; y <= segments; y++) {
      for (let x = 0; x <= segments; x++) {
        const idx = (y * (segments + 1) + x) * 3
        positions[idx] = (x / segments - 0.5) * width
        positions[idx + 1] = 0
        positions[idx + 2] = (y / segments - 0.5) * height
      }
    }
    
    for (let y = 0; y < segments; y++) {
      for (let x = 0; x < segments; x++) {
        const a = y * (segments + 1) + x
        const b = a + 1
        const c = a + segments + 1
        const d = c + 1
        indices.push(a, c, b)
        indices.push(b, c, d)
      }
    }
    
    return { positions, indices }
  }, [])
  
  useFrame((state) => {
    if (meshRef.current) {
      const geometry = meshRef.current.geometry
      const positionAttr = geometry.getAttribute('position')
      const time = state.clock.elapsedTime
      
      for (let i = 0; i < positionAttr.count; i++) {
        const x = positionAttr.getX(i)
        const z = positionAttr.getZ(i)
        const y = Math.sin(x * 0.3 + time * 0.5) * Math.cos(z * 0.3 + time * 0.3) * 1.5 +
                  Math.sin(x * 0.5 + z * 0.5 + time * 0.8) * 0.5
        positionAttr.setY(i, y)
      }
      positionAttr.needsUpdate = true
      geometry.computeVertexNormals()
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, -8, -5]} rotation={[-Math.PI / 4, 0, 0]}>
      <bufferGeometry>
        <bufferAttribute
          attach="attributes-position"
          count={positions.length / 3}
          array={positions}
          itemSize={3}
        />
        <bufferAttribute
          attach="index"
          count={indices.length}
          array={new Uint16Array(indices)}
          itemSize={1}
        />
      </bufferGeometry>
      <meshStandardMaterial
        color="#00f0ff"
        emissive="#00f0ff"
        emissiveIntensity={0.15}
        wireframe
        transparent
        opacity={0.3}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Flowing Helix Particles
function HelixParticles() {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = 300
  
  const { positions, colors } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const colors = new Float32Array(particleCount * 3)
    
    for (let i = 0; i < particleCount; i++) {
      const t = (i / particleCount) * Math.PI * 8
      const radius = 6 + Math.sin(t * 0.5) * 2
      positions[i * 3] = Math.cos(t) * radius
      positions[i * 3 + 1] = (i / particleCount - 0.5) * 20
      positions[i * 3 + 2] = Math.sin(t) * radius
      
      // Gradient colors
      const colorMix = i / particleCount
      colors[i * 3] = colorMix
      colors[i * 3 + 1] = 0.94 * (1 - colorMix) + 0.5 * colorMix
      colors[i * 3 + 2] = 1 * (1 - colorMix)
    }
    
    return { positions, colors }
  }, [])
  
  useFrame((state) => {
    if (pointsRef.current) {
      pointsRef.current.rotation.y += 0.003
      pointsRef.current.position.y = Math.sin(state.clock.elapsedTime * 0.2) * 0.5
    }
  })
  
  return (
    <points ref={pointsRef}>
      <bufferGeometry>
        <bufferAttribute
          attach="attributes-position"
          count={particleCount}
          array={positions}
          itemSize={3}
        />
        <bufferAttribute
          attach="attributes-color"
          count={particleCount}
          array={colors}
          itemSize={3}
        />
      </bufferGeometry>
      <pointsMaterial size={0.08} vertexColors transparent opacity={0.9} />
    </points>
  )
}

// Animated Torus component with cyberpunk colors
function CyberpunkTorus({ position, color }: { position: [number, number, number], color: string }) {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    meshRef.current.rotation.x = Math.sin(state.clock.elapsedTime) * 0.3
    meshRef.current.rotation.y += 0.01
  })

  return (
    <Float speed={2} rotationIntensity={0.5} floatIntensity={0.5}>
      <mesh ref={meshRef} position={position}>
        <torusGeometry args={[1, 0.4, 16, 100]} />
        <meshStandardMaterial 
          color={color} 
          emissive={color}
          emissiveIntensity={0.5}
          metalness={0.8}
          roughness={0.2}
        />
      </mesh>
    </Float>
  )
}

// Central rotating icosahedron
function CoreShape() {
  const meshRef = useRef<THREE.Mesh>(null!)
  const [hovered, setHovered] = useState(false)

  useFrame((state) => {
    meshRef.current.rotation.x += 0.005
    meshRef.current.rotation.y += 0.01
    meshRef.current.scale.setScalar(hovered ? 1.2 : 1 + Math.sin(state.clock.elapsedTime) * 0.1)
  })

  return (
    <mesh 
      ref={meshRef}
      onPointerOver={() => setHovered(true)}
      onPointerOut={() => setHovered(false)}
    >
      <icosahedronGeometry args={[1.5, 1]} />
      <meshStandardMaterial
        color="#00f0ff"
        emissive="#00f0ff"
        emissiveIntensity={hovered ? 1 : 0.3}
        wireframe
        metalness={1}
        roughness={0}
      />
    </mesh>
  )
}

// Particle ring
function ParticleRing() {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = 500
  
  const { positions, colors } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const colors = new Float32Array(particleCount * 3)
    
    for (let i = 0; i < particleCount; i++) {
      const angle = (i / particleCount) * Math.PI * 2
      const radius = 4 + Math.random() * 0.5
      positions[i * 3] = Math.cos(angle) * radius
      positions[i * 3 + 1] = (Math.random() - 0.5) * 0.5
      positions[i * 3 + 2] = Math.sin(angle) * radius
      
      // Cyberpunk colors
      const colorChoice = Math.random()
      if (colorChoice < 0.33) {
        colors[i * 3] = 0; colors[i * 3 + 1] = 0.94; colors[i * 3 + 2] = 1; // Cyan
      } else if (colorChoice < 0.66) {
        colors[i * 3] = 1; colors[i * 3 + 1] = 0; colors[i * 3 + 2] = 1; // Magenta
      } else {
        colors[i * 3] = 0; colors[i * 3 + 1] = 1; colors[i * 3 + 2] = 0.53; // Green
      }
    }
    
    return { positions, colors }
  }, [])

  useFrame(() => {
    if (pointsRef.current) {
      pointsRef.current.rotation.y += 0.002
    }
  })

  return (
    <points ref={pointsRef}>
      <bufferGeometry>
        <bufferAttribute
          attach="attributes-position"
          count={particleCount}
          array={positions}
          itemSize={3}
        />
        <bufferAttribute
          attach="attributes-color"
          count={particleCount}
          array={colors}
          itemSize={3}
        />
      </bufferGeometry>
      <pointsMaterial size={0.05} vertexColors transparent opacity={0.8} />
    </points>
  )
}

// Floating Orbs for depth - using deterministic positions based on index
function FloatingOrbs() {
  // Pre-calculated positions for consistent rendering
  const orbConfigs = [
    { x: -10, y: 5, z: -15, scale: 0.5, speed: 1.0 },
    { x: 8, y: -3, z: -12, scale: 0.4, speed: 1.2 },
    { x: -5, y: 7, z: -18, scale: 0.6, speed: 0.8 },
    { x: 12, y: -5, z: -10, scale: 0.35, speed: 1.4 },
    { x: -8, y: -6, z: -16, scale: 0.45, speed: 0.9 },
    { x: 6, y: 4, z: -14, scale: 0.55, speed: 1.1 },
    { x: -3, y: -2, z: -20, scale: 0.4, speed: 0.7 },
    { x: 10, y: 6, z: -8, scale: 0.5, speed: 1.3 },
  ]
  
  const colors = ['#00f0ff', '#ff00ff', '#00ff88', '#7b2fff']
  
  return (
    <>
      {orbConfigs.map((config, i) => (
        <Float key={i} speed={config.speed} floatIntensity={2}>
          <mesh 
            position={[config.x, config.y, config.z]} 
            scale={config.scale}
          >
            <sphereGeometry args={[1, 16, 16]} />
            <meshStandardMaterial
              color={colors[i % 4]}
              emissive={colors[i % 4]}
              emissiveIntensity={0.8}
              transparent
              opacity={0.6}
            />
          </mesh>
        </Float>
      ))}
    </>
  )
}

// DNA-like Double Helix structure
function DNAHelix() {
  const groupRef = useRef<THREE.Group>(null!)
  const nodeCount = 20
  const radius = 2
  const height = 12
  const barThickness = 0.05
  
  useFrame(() => {
    if (groupRef.current) {
      groupRef.current.rotation.y += 0.005
    }
  })
  
  const nodes = useMemo(() => {
    const result = []
    for (let i = 0; i < nodeCount; i++) {
      const t = i / nodeCount
      const angle = t * Math.PI * 4
      const y = (t - 0.5) * height
      
      // First strand
      result.push({
        pos: [Math.cos(angle) * radius, y, Math.sin(angle) * radius] as [number, number, number],
        color: '#00f0ff',
        scale: 0.15
      })
      
      // Second strand (offset by PI)
      result.push({
        pos: [Math.cos(angle + Math.PI) * radius, y, Math.sin(angle + Math.PI) * radius] as [number, number, number],
        color: '#ff00ff',
        scale: 0.15
      })
    }
    return result
  }, [])
  
  // Connecting bars between strands
  const bars = useMemo(() => {
    const result = []
    for (let i = 0; i < nodeCount; i += 2) {
      const t = i / nodeCount
      const angle = t * Math.PI * 4
      const y = (t - 0.5) * height
      result.push({
        y,
        angle,
        color: i % 4 === 0 ? '#00ff88' : '#7b2fff'
      })
    }
    return result
  }, [])
  
  return (
    <group ref={groupRef} position={[8, 0, -5]}>
      {/* DNA nodes */}
      {nodes.map((node, i) => (
        <mesh key={i} position={node.pos} scale={node.scale}>
          <sphereGeometry args={[1, 16, 16]} />
          <meshStandardMaterial
            color={node.color}
            emissive={node.color}
            emissiveIntensity={0.8}
          />
        </mesh>
      ))}
      
      {/* Connecting bars */}
      {bars.map((bar, i) => (
        <mesh 
          key={`bar-${i}`} 
          position={[0, bar.y, 0]}
          rotation={[0, bar.angle, 0]}
        >
          <boxGeometry args={[radius * 2, barThickness, barThickness]} />
          <meshStandardMaterial
            color={bar.color}
            emissive={bar.color}
            emissiveIntensity={0.5}
            transparent
            opacity={0.7}
          />
        </mesh>
      ))}
    </group>
  )
}

// Scene component
function Scene() {
  return (
    <>
      <ambientLight intensity={0.2} />
      <pointLight position={[10, 10, 10]} intensity={1} color="#00f0ff" />
      <pointLight position={[-10, -10, -10]} intensity={0.5} color="#ff00ff" />
      <spotLight position={[0, 10, 0]} intensity={0.5} color="#00ff88" angle={0.3} />
      
      {/* Parametric 3D Background */}
      <ParametricBackground />
      <HelixParticles />
      <FloatingOrbs />
      <DNAHelix />
      
      <CoreShape />
      <ParticleRing />
      
      <CyberpunkTorus position={[-4, 0, 0]} color="#ff00ff" />
      <CyberpunkTorus position={[4, 0, 0]} color="#00ff88" />
      <CyberpunkTorus position={[0, 3, 0]} color="#7b2fff" />
      
      <Stars radius={100} depth={50} count={5000} factor={4} saturation={0} fade speed={1} />
      
      <OrbitControls 
        enableZoom={true}
        enablePan={false}
        autoRotate
        autoRotateSpeed={0.5}
        minDistance={5}
        maxDistance={20}
      />
    </>
  )
}

// Loading component
function Loading() {
  return (
    <div className="loading">
      <div className="loading-text">Loading DevTeam6 Experience...</div>
    </div>
  )
}

// Main App
function App() {
  return (
    <>
      <Canvas camera={{ position: [0, 0, 10], fov: 60 }}>
        <Suspense fallback={null}>
          <Scene />
        </Suspense>
      </Canvas>
      
      <div className="overlay">
        <div className="header">
          <h1>DevTeam6</h1>
          <p>THE OMEGA TOOL KIT • Interactive 3D Experience</p>
        </div>
        
        <div className="stats">
          <p>1000+ Curated Resources</p>
          <p>500+ AI Tools</p>
          <p>300+ Awesome Lists</p>
        </div>
        
        <div className="controls">
          <a href="https://github.com/SpiralCloudOmega/DevTeam6" target="_blank" rel="noopener noreferrer" className="btn">
            View Repository
          </a>
          <a href="/DevTeam6/knowledge-hub" className="btn primary">
            🧠 Knowledge Hub
          </a>
          <a href="/DevTeam6/landing" className="btn secondary">
            🌊 Landing
          </a>
          <a href="/DevTeam6/genui" className="btn primary">
            🎨 GenUI
          </a>
          <a href="/DevTeam6/node-graph" className="btn secondary">
            ⚡ Node Graph
          </a>
          <a href="/DevTeam6/videos" className="btn primary">
            🎬 Videos
          </a>
          <a href="/DevTeam6/gamification" className="btn secondary">
            🏆 Leaderboard
          </a>
          <a href="/DevTeam6/roadmap" className="btn primary">
            📋 Roadmap
          </a>
          <a href="/DevTeam6/onboarding" className="btn secondary">
            🚀 Get Started
          </a>
        </div>
      </div>
    </>
  )
}

export default App
