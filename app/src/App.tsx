import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars, Float, Text3D, Center } from '@react-three/drei'
import { Suspense, useRef, useState } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

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

  useFrame((state) => {
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

// Scene component
function Scene() {
  return (
    <>
      <ambientLight intensity={0.2} />
      <pointLight position={[10, 10, 10]} intensity={1} color="#00f0ff" />
      <pointLight position={[-10, -10, -10]} intensity={0.5} color="#ff00ff" />
      <spotLight position={[0, 10, 0]} intensity={0.5} color="#00ff88" angle={0.3} />
      
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
          <a href="/DevTeam6/landing" className="btn secondary">
            🌊 Landing
          </a>
          <a href="/DevTeam6/genui" className="btn primary">
            🎨 GenUI
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
