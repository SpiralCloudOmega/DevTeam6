import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars } from '@react-three/drei'
import { Suspense, useRef, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

// Configuration constants for performance and visual tuning
const GRID_CONFIG = {
  WIDTH: 400,
  DEPTH: 800,
  SEGMENTS_X: 200,  // Higher = tighter triangles, more GPU intensive
  SEGMENTS_Z: 400,
  PERSPECTIVE_FACTOR: 0.5  // Controls how much the grid widens with distance
}

const PARTICLE_CONFIG = {
  COUNT: 1200,  // Particle count - reduce for lower-end devices
  BOUNDS: 40    // Z-axis bounds for particle wrapping
}

const LIGHTING_CONFIG = {
  SPOTLIGHT: {
    position: [0, 20, 10] as [number, number, number],
    intensity: 0.5,
    angle: 0.3,
    penumbra: 1
  }
}

// Enhanced Deep Space Perspective Grid with tighter triangles
// Creates the iconic "looking into deep space" effect with proper perspective and color shading
function DeepSpacePerspectiveGrid() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  const { positions, indices, colors } = useMemo(() => {
    const { WIDTH, DEPTH, SEGMENTS_X, SEGMENTS_Z, PERSPECTIVE_FACTOR } = GRID_CONFIG
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
        
        // World position - grid extends back into z with perspective scaling
        const perspectiveScale = 1 + nz * PERSPECTIVE_FACTOR
        const worldX = nx * WIDTH * perspectiveScale
        const worldZ = nz * DEPTH
        
        positions[posIdx] = worldX
        positions[posIdx + 1] = 0
        positions[posIdx + 2] = worldZ
        
        // Enhanced color gradient - cyan to purple to deep space blue
        const depthFactor = nz
        const xFactor = Math.abs(nx) * 2  // Edge to center gradient
        
        // Vibrant colors with depth shading
        colors[posIdx] = 0.1 + depthFactor * 0.3 + xFactor * 0.1  // R - purple hints
        colors[posIdx + 1] = 0.7 - depthFactor * 0.4 - xFactor * 0.2  // G - cyan fade
        colors[posIdx + 2] = 0.9 - depthFactor * 0.2  // B - stays strong
      }
    }
    
    // Create triangle mesh with proper winding
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
        
        // Multiple wave layers for organic motion - 45 degree angles
        const wave1 = Math.sin((x + z) * 0.015 + time * 0.4) * 4
        const wave2 = Math.cos((x - z) * 0.012 + time * 0.3) * 3
        const wave3 = Math.sin(z * 0.008 + time * 0.2) * 2
        
        // Waves diminish with distance for depth realism
        const distanceFade = 1 - (z / GRID_CONFIG.DEPTH) * 0.6
        const finalY = (wave1 + wave2 + wave3) * distanceFade
        
        positionAttr.setY(i, finalY)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, -30, -100]} rotation={[-Math.PI / 2.5, 0, 0]}>
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
        opacity={0.5}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Data Stream Lanes - Flowing particles representing data flow
function DataStreamLane({ position, color, direction }: {
  position: [number, number, number],
  color: string,
  direction: number
}) {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = 100
  const bounds = PARTICLE_CONFIG.BOUNDS
  
  const { positions, velocities } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const velocities = new Float32Array(particleCount)
    
    for (let i = 0; i < particleCount; i++) {
      positions[i * 3] = (Math.random() - 0.5) * 2
      positions[i * 3 + 1] = (Math.random() - 0.5) * 0.5
      positions[i * 3 + 2] = Math.random() * bounds * 2 - bounds
      velocities[i] = 0.3 + Math.random() * 0.4
    }
    
    return { positions, velocities }
  }, [bounds])
  
  useFrame(() => {
    if (pointsRef.current) {
      const positionAttr = pointsRef.current.geometry.getAttribute('position')
      
      for (let i = 0; i < particleCount; i++) {
        let z = positionAttr.getZ(i)
        z += velocities[i] * direction
        
        // Wrap around using configured bounds
        if (direction > 0 && z > bounds) z = -bounds
        if (direction < 0 && z < -bounds) z = bounds
        
        positionAttr.setZ(i, z)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  return (
    <points ref={pointsRef} position={position}>
      <bufferGeometry>
        <bufferAttribute
          attach="attributes-position"
          count={particleCount}
          array={positions}
          itemSize={3}
        />
      </bufferGeometry>
      <pointsMaterial size={0.1} color={color} transparent opacity={0.7} />
    </points>
  )
}

// Cyberpunk Torus with wireframe overlay for 3D depth
function CyberpunkTorus({ position, color, size, rotationAxis }: {
  position: [number, number, number],
  color: string,
  size: number,
  rotationAxis: 'x' | 'y' | 'z'
}) {
  const groupRef = useRef<THREE.Group>(null!)
  
  useFrame((state) => {
    if (groupRef.current) {
      const speed = 0.005
      if (rotationAxis === 'x') groupRef.current.rotation.x += speed
      else if (rotationAxis === 'y') groupRef.current.rotation.y += speed
      else groupRef.current.rotation.z += speed
      
      // Gentle floating
      groupRef.current.position.y = position[1] + Math.sin(state.clock.elapsedTime * 0.5) * 0.3
    }
  })
  
  return (
    <group ref={groupRef} position={position}>
      {/* Solid inner torus */}
      <mesh>
        <torusGeometry args={[size, size * 0.3, 16, 48]} />
        <meshStandardMaterial
          color={color}
          emissive={color}
          emissiveIntensity={0.3}
          transparent
          opacity={0.6}
        />
      </mesh>
      {/* Wireframe overlay for 3D depth */}
      <mesh>
        <torusGeometry args={[size * 1.05, size * 0.32, 16, 48]} />
        <meshBasicMaterial color={color} wireframe transparent opacity={0.4} />
      </mesh>
    </group>
  )
}

// Nebula Gas Cloud Layer - Creates ethereal space atmosphere with depth
function NebulaCloud({ position, color, scale, speed }: { 
  position: [number, number, number], 
  color: string, 
  scale: number,
  speed: number 
}) {
  const meshRef = useRef<THREE.Mesh>(null!)
  const innerRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    if (meshRef.current) {
      // Gentle pulsing
      const pulse = 1 + Math.sin(state.clock.elapsedTime * speed) * 0.15
      meshRef.current.scale.setScalar(scale * pulse)
      // Slow rotation
      meshRef.current.rotation.z += 0.0008
      meshRef.current.rotation.y += 0.0005
    }
    if (innerRef.current) {
      const pulse = 1 + Math.sin(state.clock.elapsedTime * speed * 1.5) * 0.2
      innerRef.current.scale.setScalar(scale * 0.6 * pulse)
    }
  })
  
  return (
    <group position={position}>
      {/* Outer glow */}
      <mesh ref={meshRef}>
        <sphereGeometry args={[1, 32, 32]} />
        <meshBasicMaterial
          color={color}
          transparent
          opacity={0.12}
          side={THREE.DoubleSide}
        />
      </mesh>
      {/* Inner core - brighter */}
      <mesh ref={innerRef}>
        <sphereGeometry args={[1, 24, 24]} />
        <meshBasicMaterial
          color={color}
          transparent
          opacity={0.25}
        />
      </mesh>
    </group>
  )
}

// Central Geodesic Sphere - The main focal point with enhanced visuals
function GeodesicCore() {
  const meshRef = useRef<THREE.Mesh>(null!)
  const wireRef = useRef<THREE.Mesh>(null!)
  const outerRef = useRef<THREE.Mesh>(null!)
  const glowRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    const time = state.clock.elapsedTime
    if (meshRef.current) {
      meshRef.current.rotation.y += 0.004
      meshRef.current.rotation.x = Math.sin(time * 0.5) * 0.15
    }
    if (wireRef.current) {
      wireRef.current.rotation.y += 0.004
      wireRef.current.rotation.x = Math.sin(time * 0.5) * 0.15
    }
    if (outerRef.current) {
      outerRef.current.rotation.y -= 0.002
      outerRef.current.rotation.z = Math.cos(time * 0.3) * 0.1
    }
    if (glowRef.current) {
      const pulse = 1 + Math.sin(time * 2) * 0.08
      glowRef.current.scale.setScalar(pulse)
    }
  })
  
  return (
    <group position={[0, 0, 0]}>
      {/* Outer glow sphere */}
      <mesh ref={glowRef}>
        <sphereGeometry args={[3.5, 32, 32]} />
        <meshBasicMaterial color="#00f0ff" transparent opacity={0.05} />
      </mesh>
      {/* Outer rotating shell */}
      <mesh ref={outerRef}>
        <icosahedronGeometry args={[2.8, 1]} />
        <meshBasicMaterial color="#7b2fff" wireframe transparent opacity={0.3} />
      </mesh>
      {/* Inner glowing core */}
      <mesh ref={meshRef}>
        <icosahedronGeometry args={[1.8, 2]} />
        <meshStandardMaterial
          color="#00f0ff"
          emissive="#00f0ff"
          emissiveIntensity={0.4}
          transparent
          opacity={0.4}
        />
      </mesh>
      {/* Wireframe overlay */}
      <mesh ref={wireRef}>
        <icosahedronGeometry args={[2.1, 2]} />
        <meshBasicMaterial
          color="#00f0ff"
          wireframe
          transparent
          opacity={0.7}
        />
      </mesh>
    </group>
  )
}

// Floating Particle Field - Enhanced with better depth distribution
function ParticleField() {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = PARTICLE_CONFIG.COUNT
  
  const { positions, colors } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const colors = new Float32Array(particleCount * 3)
    
    for (let i = 0; i < particleCount; i++) {
      // Distribute particles in 3D space with layered depth
      const theta = Math.random() * Math.PI * 2
      const phi = Math.random() * Math.PI
      const r = 5 + Math.random() * 50
      
      positions[i * 3] = r * Math.sin(phi) * Math.cos(theta)
      positions[i * 3 + 1] = r * Math.cos(phi) - 3
      positions[i * 3 + 2] = r * Math.sin(phi) * Math.sin(theta) + 25
      
      // Color based on distance - multi-color gradient
      const dist = r / 55
      const colorChoice = Math.random()
      if (colorChoice < 0.33) {
        // Cyan
        colors[i * 3] = 0.0
        colors[i * 3 + 1] = 0.9 - dist * 0.3
        colors[i * 3 + 2] = 1.0
      } else if (colorChoice < 0.66) {
        // Magenta
        colors[i * 3] = 1.0 - dist * 0.3
        colors[i * 3 + 1] = 0.0
        colors[i * 3 + 2] = 1.0
      } else {
        // Green
        colors[i * 3] = 0.0
        colors[i * 3 + 1] = 1.0 - dist * 0.2
        colors[i * 3 + 2] = 0.5
      }
    }
    
    return { positions, colors }
  }, [particleCount])
  
  useFrame((state) => {
    if (pointsRef.current) {
      pointsRef.current.rotation.y += 0.0003
      pointsRef.current.rotation.x = Math.sin(state.clock.elapsedTime * 0.1) * 0.02
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
      <pointsMaterial size={0.12} vertexColors transparent opacity={0.75} sizeAttenuation />
    </points>
  )
}

// Orbital Ring - Enhanced with glow effect
function OrbitalRing({ radius, color, yOffset, zOffset, rotationSpeed, tilt }: {
  radius: number,
  color: string,
  yOffset: number,
  zOffset: number,
  rotationSpeed: number,
  tilt?: number
}) {
  const ringRef = useRef<THREE.Group>(null!)
  
  useFrame((state) => {
    if (ringRef.current) {
      ringRef.current.rotation.z += rotationSpeed
      // Subtle wobble
      ringRef.current.rotation.x = (tilt || Math.PI / 2) + Math.sin(state.clock.elapsedTime * 0.5) * 0.05
    }
  })
  
  return (
    <group ref={ringRef} position={[0, yOffset, zOffset]} rotation={[tilt || Math.PI / 2, 0, 0]}>
      {/* Main ring */}
      <mesh>
        <torusGeometry args={[radius, 0.03, 16, 100]} />
        <meshBasicMaterial color={color} transparent opacity={0.7} />
      </mesh>
      {/* Glow ring */}
      <mesh>
        <torusGeometry args={[radius, 0.08, 8, 100]} />
        <meshBasicMaterial color={color} transparent opacity={0.15} />
      </mesh>
    </group>
  )
}

// Holographic Scanlines for retro-futuristic effect
function HolographicScanlines() {
  const linesRef = useRef<THREE.Group>(null!)
  const lineCount = 20
  
  const linePositions = useMemo(() => {
    const positions: number[] = []
    for (let i = 0; i < lineCount; i++) {
      positions.push(-8 + (i / lineCount) * 16)
    }
    return positions
  }, [])
  
  useFrame((state) => {
    if (linesRef.current) {
      linesRef.current.children.forEach((line, i) => {
        const offset = Math.sin(state.clock.elapsedTime * 0.5 + i * 0.3) * 0.5
        line.position.y = linePositions[i] + offset
      })
    }
  })
  
  return (
    <group ref={linesRef} position={[0, 0, -5]}>
      {linePositions.map((y, i) => (
        <mesh key={i} position={[0, y, 0]}>
          <planeGeometry args={[40, 0.02]} />
          <meshBasicMaterial color="#00f0ff" transparent opacity={0.08} side={THREE.DoubleSide} />
        </mesh>
      ))}
    </group>
  )
}

// Scene component - Enhanced with proper depth layering and new elements
function Scene() {
  return (
    <>
      {/* Layer 1: Stars - Furthest back (Z = very far) */}
      <Stars radius={500} depth={400} count={15000} factor={5} saturation={0} fade speed={0.2} />
      
      {/* Layer 2: Nebula clouds - Far background (Z = 50-150) */}
      <NebulaCloud position={[-50, 15, 100]} color="#ff00ff" scale={35} speed={0.25} />
      <NebulaCloud position={[60, -10, 120]} color="#7b2fff" scale={45} speed={0.18} />
      <NebulaCloud position={[0, 25, 140]} color="#00f0ff" scale={30} speed={0.35} />
      <NebulaCloud position={[-35, -20, 110]} color="#00ff88" scale={25} speed={0.3} />
      <NebulaCloud position={[45, 5, 90]} color="#ff6600" scale={20} speed={0.4} />
      
      {/* Layer 3: Deep space perspective grid - Mid-far background */}
      <DeepSpacePerspectiveGrid />
      
      {/* Layer 4: Data stream lanes */}
      <DataStreamLane position={[-15, 2, 0]} color="#00f0ff" direction={1} />
      <DataStreamLane position={[15, -1, 0]} color="#ff00ff" direction={-1} />
      <DataStreamLane position={[0, 4, 10]} color="#00ff88" direction={1} />
      
      {/* Layer 5: Holographic scanlines */}
      <HolographicScanlines />
      
      {/* Layer 6: Particle field - Mid-ground (Z = 5-55) */}
      <ParticleField />
      
      {/* Layer 7: Cyberpunk torus rings */}
      <CyberpunkTorus position={[-8, 0, 5]} color="#ff00ff" size={2.5} rotationAxis="y" />
      <CyberpunkTorus position={[8, 0, 5]} color="#00ff88" size={2.5} rotationAxis="y" />
      
      {/* Layer 8: Orbital rings at various depths */}
      <OrbitalRing radius={7} color="#00f0ff" yOffset={0} zOffset={8} rotationSpeed={0.001} tilt={Math.PI / 2.2} />
      <OrbitalRing radius={5.5} color="#ff00ff" yOffset={0} zOffset={5} rotationSpeed={-0.0015} tilt={Math.PI / 1.8} />
      <OrbitalRing radius={4} color="#00ff88" yOffset={0} zOffset={2} rotationSpeed={0.002} />
      <OrbitalRing radius={3} color="#7b2fff" yOffset={0} zOffset={0} rotationSpeed={-0.0025} tilt={Math.PI / 2.5} />
      
      {/* Layer 9: Central geodesic core - Focal point (Z = 0) */}
      <GeodesicCore />
      
      {/* Enhanced Lighting */}
      <ambientLight intensity={0.12} />
      <pointLight position={[15, 15, -15]} intensity={1} color="#00f0ff" />
      <pointLight position={[-15, -10, 25]} intensity={0.6} color="#ff00ff" />
      <pointLight position={[0, 10, 0]} intensity={0.4} color="#ffffff" />
      <spotLight 
        position={LIGHTING_CONFIG.SPOTLIGHT.position} 
        intensity={LIGHTING_CONFIG.SPOTLIGHT.intensity} 
        color="#7b2fff" 
        angle={LIGHTING_CONFIG.SPOTLIGHT.angle} 
        penumbra={LIGHTING_CONFIG.SPOTLIGHT.penumbra} 
      />
      
      <OrbitControls 
        enableZoom={true}
        enablePan={false}
        autoRotate
        autoRotateSpeed={0.15}
        minDistance={6}
        maxDistance={50}
        maxPolarAngle={Math.PI / 1.4}
      />
    </>
  )
}

// Main App with enhanced UI
function App() {
  return (
    <>
      <Canvas camera={{ position: [0, 3, 14], fov: 65 }}>
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
