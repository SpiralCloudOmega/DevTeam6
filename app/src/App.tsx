import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars } from '@react-three/drei'
import { Suspense, useRef, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

// Deep Space Perspective Grid - The main background grid that extends into infinity
// Creates the iconic "looking into deep space" effect with proper perspective
function DeepSpacePerspectiveGrid() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  // Grid extends from close to camera into far distance
  // Using perspective projection to create depth
  const GRID_WIDTH = 300
  const GRID_DEPTH = 600
  const SEGMENTS_X = 120
  const SEGMENTS_Z = 240
  
  const { positions, indices, colors } = useMemo(() => {
    const positions = new Float32Array((SEGMENTS_X + 1) * (SEGMENTS_Z + 1) * 3)
    const colors = new Float32Array((SEGMENTS_X + 1) * (SEGMENTS_Z + 1) * 3)
    const indices: number[] = []
    
    for (let z = 0; z <= SEGMENTS_Z; z++) {
      for (let x = 0; x <= SEGMENTS_X; x++) {
        const idx = (z * (SEGMENTS_X + 1) + x)
        const posIdx = idx * 3
        
        // Normalized coordinates
        const nx = x / SEGMENTS_X - 0.5
        const nz = z / SEGMENTS_Z
        
        // World position - grid extends back into z
        const worldX = nx * GRID_WIDTH
        const worldZ = nz * GRID_DEPTH
        
        positions[posIdx] = worldX
        positions[posIdx + 1] = 0
        positions[posIdx + 2] = worldZ
        
        // Deep space colors - dark blue/purple gradient based on depth
        const depthFactor = nz
        // Cyan near, purple far
        colors[posIdx] = 0.0 + depthFactor * 0.2      // R
        colors[posIdx + 1] = 0.5 - depthFactor * 0.3  // G
        colors[posIdx + 2] = 0.7 + depthFactor * 0.3  // B
      }
    }
    
    // Create triangle mesh
    for (let z = 0; z < SEGMENTS_Z; z++) {
      for (let x = 0; x < SEGMENTS_X; x++) {
        const a = z * (SEGMENTS_X + 1) + x
        const b = a + 1
        const c = a + SEGMENTS_X + 1
        const d = c + 1
        indices.push(a, c, b)
        indices.push(b, c, d)
      }
    }
    
    return { positions, indices, colors }
  }, [])
  
  useFrame((state) => {
    if (meshRef.current) {
      const geometry = meshRef.current.geometry
      const positionAttr = geometry.getAttribute('position')
      const time = state.clock.elapsedTime
      
      for (let i = 0; i < positionAttr.count; i++) {
        const x = positionAttr.getX(i)
        const z = positionAttr.getZ(i)
        
        // Gentle rolling waves - like looking at an ocean of space
        const wave1 = Math.sin(x * 0.02 + z * 0.015 + time * 0.3) * 3
        const wave2 = Math.cos(x * 0.015 - z * 0.01 + time * 0.2) * 2
        
        // Waves diminish with distance for realism
        const distanceFade = 1 - (z / GRID_DEPTH) * 0.5
        const finalY = (wave1 + wave2) * distanceFade
        
        positionAttr.setY(i, finalY)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, -25, -50]} rotation={[-Math.PI / 2.8, 0, 0]}>
      <bufferGeometry>
        <bufferAttribute
          attach="attributes-position"
          count={positions.length / 3}
          array={positions}
          itemSize={3}
        />
        <bufferAttribute
          attach="attributes-color"
          count={colors.length / 3}
          array={colors}
          itemSize={3}
        />
        <bufferAttribute
          attach="index"
          count={indices.length}
          array={new Uint32Array(indices)}
          itemSize={1}
        />
      </bufferGeometry>
      <meshBasicMaterial
        vertexColors
        wireframe
        transparent
        opacity={0.6}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Nebula Gas Cloud Layer - Creates ethereal space atmosphere
function NebulaCloud({ position, color, scale, speed }: { 
  position: [number, number, number], 
  color: string, 
  scale: number,
  speed: number 
}) {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    if (meshRef.current) {
      // Gentle pulsing
      const pulse = 1 + Math.sin(state.clock.elapsedTime * speed) * 0.1
      meshRef.current.scale.setScalar(scale * pulse)
      // Slow rotation
      meshRef.current.rotation.z += 0.001
    }
  })
  
  return (
    <mesh ref={meshRef} position={position}>
      <sphereGeometry args={[1, 32, 32]} />
      <meshBasicMaterial
        color={color}
        transparent
        opacity={0.15}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Central Geodesic Sphere - The main focal point like the center orb
function GeodesicCore() {
  const meshRef = useRef<THREE.Mesh>(null!)
  const wireRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    if (meshRef.current) {
      meshRef.current.rotation.y += 0.003
      meshRef.current.rotation.x = Math.sin(state.clock.elapsedTime * 0.5) * 0.1
    }
    if (wireRef.current) {
      wireRef.current.rotation.y += 0.003
      wireRef.current.rotation.x = Math.sin(state.clock.elapsedTime * 0.5) * 0.1
    }
  })
  
  return (
    <group position={[0, 0, 0]}>
      {/* Inner glowing core */}
      <mesh ref={meshRef}>
        <icosahedronGeometry args={[1.8, 2]} />
        <meshBasicMaterial
          color="#00f0ff"
          transparent
          opacity={0.3}
        />
      </mesh>
      {/* Outer wireframe */}
      <mesh ref={wireRef}>
        <icosahedronGeometry args={[2, 2]} />
        <meshBasicMaterial
          color="#00f0ff"
          wireframe
          transparent
          opacity={0.8}
        />
      </mesh>
    </group>
  )
}

// Floating Particle Field - Depth particles scattered in 3D space
function ParticleField() {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = 800
  
  const { positions, colors } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const colors = new Float32Array(particleCount * 3)
    
    for (let i = 0; i < particleCount; i++) {
      // Distribute particles in 3D space with proper depth
      const theta = Math.random() * Math.PI * 2
      const phi = Math.random() * Math.PI
      const r = 5 + Math.random() * 40
      
      positions[i * 3] = r * Math.sin(phi) * Math.cos(theta)
      positions[i * 3 + 1] = r * Math.cos(phi) - 5
      positions[i * 3 + 2] = r * Math.sin(phi) * Math.sin(theta) + 20
      
      // Color based on distance - cyan near, purple far
      const dist = r / 45
      colors[i * 3] = 0.5 * dist
      colors[i * 3 + 1] = 0.9 - 0.4 * dist
      colors[i * 3 + 2] = 1
    }
    
    return { positions, colors }
  }, [])
  
  useFrame((state) => {
    if (pointsRef.current) {
      pointsRef.current.rotation.y += 0.0005
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
      <pointsMaterial size={0.15} vertexColors transparent opacity={0.8} />
    </points>
  )
}

// Orbital Ring - Clean, simple rings at different depths
function OrbitalRing({ radius, color, yOffset, zOffset, rotationSpeed }: {
  radius: number,
  color: string,
  yOffset: number,
  zOffset: number,
  rotationSpeed: number
}) {
  const ringRef = useRef<THREE.Mesh>(null!)
  
  useFrame(() => {
    if (ringRef.current) {
      ringRef.current.rotation.z += rotationSpeed
    }
  })
  
  return (
    <mesh ref={ringRef} position={[0, yOffset, zOffset]} rotation={[Math.PI / 2, 0, 0]}>
      <torusGeometry args={[radius, 0.02, 16, 100]} />
      <meshBasicMaterial color={color} transparent opacity={0.6} />
    </mesh>
  )
}

// Scene component - Proper depth layering from back to front
function Scene() {
  return (
    <>
      {/* Layer 1: Stars - Furthest back (Z = very far) */}
      <Stars radius={400} depth={300} count={12000} factor={4} saturation={0} fade speed={0.3} />
      
      {/* Layer 2: Nebula clouds - Far background (Z = 50-150) */}
      <NebulaCloud position={[-40, 10, 80]} color="#ff00ff" scale={30} speed={0.3} />
      <NebulaCloud position={[50, -5, 100]} color="#7b2fff" scale={40} speed={0.2} />
      <NebulaCloud position={[0, 20, 120]} color="#00f0ff" scale={25} speed={0.4} />
      <NebulaCloud position={[-30, -15, 90]} color="#00ff88" scale={20} speed={0.35} />
      
      {/* Layer 3: Deep space perspective grid - Mid-far background */}
      <DeepSpacePerspectiveGrid />
      
      {/* Layer 4: Particle field - Mid-ground (Z = 5-45) */}
      <ParticleField />
      
      {/* Layer 5: Orbital rings at various depths */}
      <OrbitalRing radius={6} color="#00f0ff" yOffset={0} zOffset={5} rotationSpeed={0.002} />
      <OrbitalRing radius={4.5} color="#ff00ff" yOffset={0} zOffset={3} rotationSpeed={-0.003} />
      <OrbitalRing radius={3} color="#00ff88" yOffset={0} zOffset={1} rotationSpeed={0.004} />
      
      {/* Layer 6: Central geodesic core - Focal point (Z = 0) */}
      <GeodesicCore />
      
      {/* Lighting */}
      <ambientLight intensity={0.15} />
      <pointLight position={[10, 10, -10]} intensity={0.8} color="#00f0ff" />
      <pointLight position={[-10, -10, 20]} intensity={0.4} color="#ff00ff" />
      
      <OrbitControls 
        enableZoom={true}
        enablePan={false}
        autoRotate
        autoRotateSpeed={0.2}
        minDistance={8}
        maxDistance={40}
        maxPolarAngle={Math.PI / 1.5}
      />
    </>
  )
}

// Main App
function App() {
  return (
    <>
      <Canvas camera={{ position: [0, 2, 12], fov: 70 }}>
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
