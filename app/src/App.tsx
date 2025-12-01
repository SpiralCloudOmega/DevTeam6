import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars, Float } from '@react-three/drei'
import { Suspense, useRef, useState, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

// Gravity Field Mesh - Einstein-style spacetime curvature
// Objects create "wells" in the mesh like mass curves spacetime
function GravityFieldMesh() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  // Constants for gravity calculations
  const GRAVITY_RADIUS_MULTIPLIER = 4
  const GRAVITY_FALLOFF_POWER = 2.5
  
  // Gravity sources - positions and "mass" that warp the mesh
  const gravitySources = useMemo(() => [
    { x: 0, z: 0, mass: 5.0, radius: 8 },      // Center - main DevTeam6 logo
    { x: -6, z: 3, mass: 2.5, radius: 4 },     // Left torus
    { x: 6, z: 3, mass: 2.5, radius: 4 },      // Right torus
    { x: 0, z: -5, mass: 2.0, radius: 3 },     // Core shape
  ], [])
  
  const { positions, indices, uvs } = useMemo(() => {
    const width = 80
    const height = 100
    const segments = 80
    const positions = new Float32Array((segments + 1) * (segments + 1) * 3)
    const uvs = new Float32Array((segments + 1) * (segments + 1) * 2)
    const indices: number[] = []
    
    for (let y = 0; y <= segments; y++) {
      for (let x = 0; x <= segments; x++) {
        const idx = (y * (segments + 1) + x)
        const posIdx = idx * 3
        const uvIdx = idx * 2
        
        positions[posIdx] = (x / segments - 0.5) * width
        positions[posIdx + 1] = 0
        positions[posIdx + 2] = (y / segments - 0.5) * height
        
        uvs[uvIdx] = x / segments
        uvs[uvIdx + 1] = y / segments
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
    
    return { positions, indices, uvs }
  }, [])
  
  useFrame((state) => {
    if (meshRef.current) {
      const geometry = meshRef.current.geometry
      const positionAttr = geometry.getAttribute('position')
      const time = state.clock.elapsedTime
      
      for (let i = 0; i < positionAttr.count; i++) {
        const x = positionAttr.getX(i)
        const z = positionAttr.getZ(i)
        
        // Calculate gravity well depth from all sources
        let gravityDepth = 0
        for (const source of gravitySources) {
          const dx = x - source.x
          const dz = z - source.z
          const distance = Math.sqrt(dx * dx + dz * dz)
          
          // Inverse square-ish falloff with smooth edges
          if (distance < source.radius * GRAVITY_RADIUS_MULTIPLIER) {
            const normalizedDist = distance / (source.radius * GRAVITY_RADIUS_MULTIPLIER)
            const wellDepth = source.mass * Math.pow(1 - normalizedDist, GRAVITY_FALLOFF_POWER)
            gravityDepth += wellDepth
          }
        }
        
        // Add subtle wave animation
        const wave = Math.sin(x * 0.12 + time * 0.4) * Math.cos(z * 0.12 + time * 0.25) * 0.5
        
        // Combine gravity wells with wave
        const finalY = -gravityDepth + wave
        positionAttr.setY(i, finalY)
      }
      
      positionAttr.needsUpdate = true
      geometry.computeVertexNormals()
    }
  })
  
  return (
    <group>
      {/* Main visible wireframe mesh */}
      <mesh ref={meshRef} position={[0, -2, 8]} rotation={[-Math.PI / 2.8, 0, 0]}>
        <bufferGeometry>
          <bufferAttribute
            attach="attributes-position"
            count={positions.length / 3}
            array={positions}
            itemSize={3}
          />
          <bufferAttribute
            attach="attributes-uv"
            count={uvs.length / 2}
            array={uvs}
            itemSize={2}
          />
          <bufferAttribute
            attach="index"
            count={indices.length}
            array={new Uint32Array(indices)}
            itemSize={1}
          />
        </bufferGeometry>
        <meshStandardMaterial
          color="#00f0ff"
          emissive="#00f0ff"
          emissiveIntensity={0.4}
          wireframe
          transparent
          opacity={0.6}
          side={THREE.DoubleSide}
        />
      </mesh>
    </group>
  )
}

// Holographic Grid Floor - extends far down with visible depth
function HolographicGrid() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  // Grid configuration constants
  const GRID_WIDTH = 120
  const GRID_DEPTH = 200
  const WAVE_FADE_DISTANCE = 120
  
  const { positions, indices } = useMemo(() => {
    const segmentsX = 60
    const segmentsZ = 100
    const positions = new Float32Array((segmentsX + 1) * (segmentsZ + 1) * 3)
    const indices: number[] = []
    
    for (let z = 0; z <= segmentsZ; z++) {
      for (let x = 0; x <= segmentsX; x++) {
        const idx = (z * (segmentsX + 1) + x) * 3
        positions[idx] = (x / segmentsX - 0.5) * GRID_WIDTH
        positions[idx + 1] = 0
        positions[idx + 2] = (z / segmentsZ) * GRID_DEPTH - 30
      }
    }
    
    for (let z = 0; z < segmentsZ; z++) {
      for (let x = 0; x < segmentsX; x++) {
        const a = z * (segmentsX + 1) + x
        const b = a + 1
        const c = a + segmentsX + 1
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
        
        // Gentle waves that move toward viewer
        const wave = Math.sin(z * 0.08 - time * 0.6) * 0.8 + 
                     Math.sin(x * 0.06 + time * 0.4) * 0.5
        
        // Fade out waves in distance
        const distanceFade = Math.max(0, 1 - Math.abs(z) / WAVE_FADE_DISTANCE)
        
        positionAttr.setY(i, wave * distanceFade)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, -15, 0]} rotation={[-Math.PI / 2.2, 0, 0]}>
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
          array={new Uint32Array(indices)}
          itemSize={1}
        />
      </bufferGeometry>
      <meshStandardMaterial
        color="#7b2fff"
        emissive="#7b2fff"
        emissiveIntensity={0.25}
        wireframe
        transparent
        opacity={0.4}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Holographic scanlines effect
function HolographicScanlines() {
  const linesRef = useRef<THREE.Group>(null!)
  
  useFrame((state) => {
    if (linesRef.current) {
      // Move scanlines up slowly
      linesRef.current.position.y = (state.clock.elapsedTime * 0.5) % 20 - 10
    }
  })
  
  // Deterministic scanline configurations
  const lines = useMemo(() => {
    const result = []
    for (let i = 0; i < 30; i++) {
      // Use sine-based pseudo-random for deterministic values
      const seed = i * 0.1
      result.push({
        y: i * 0.7 - 10,
        opacity: 0.1 + (Math.sin(seed * 7.3) * 0.5 + 0.5) * 0.2,
        width: 30 + (Math.sin(seed * 13.7) * 0.5 + 0.5) * 20
      })
    }
    return result
  }, [])
  
  return (
    <group ref={linesRef}>
      {lines.map((line, i) => (
        <mesh key={i} position={[0, line.y, -5]}>
          <planeGeometry args={[line.width, 0.02]} />
          <meshBasicMaterial
            color="#00f0ff"
            transparent
            opacity={line.opacity}
            side={THREE.DoubleSide}
          />
        </mesh>
      ))}
    </group>
  )
}

// Glowing 3D Text that pushes through the mesh
function GravityText() {
  const groupRef = useRef<THREE.Group>(null!)
  const glowRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    if (groupRef.current) {
      // Subtle floating animation
      groupRef.current.position.y = Math.sin(state.clock.elapsedTime * 0.5) * 0.2
    }
    if (glowRef.current) {
      // Pulsing glow
      const scale = 1 + Math.sin(state.clock.elapsedTime * 2) * 0.05
      glowRef.current.scale.set(scale, scale, 1)
    }
  })
  
  return (
    <group ref={groupRef} position={[0, 2, 0]}>
      {/* Glow backdrop */}
      <mesh ref={glowRef} position={[0, 0, -0.5]}>
        <planeGeometry args={[12, 3]} />
        <meshBasicMaterial
          color="#00f0ff"
          transparent
          opacity={0.1}
          side={THREE.DoubleSide}
        />
      </mesh>
      
      {/* Main holographic rings around text area */}
      <mesh rotation={[Math.PI / 2, 0, 0]}>
        <torusGeometry args={[5, 0.05, 8, 64]} />
        <meshStandardMaterial
          color="#00f0ff"
          emissive="#00f0ff"
          emissiveIntensity={0.8}
          transparent
          opacity={0.6}
        />
      </mesh>
      
      <mesh rotation={[Math.PI / 2, 0, Math.PI / 4]}>
        <torusGeometry args={[4.5, 0.03, 8, 64]} />
        <meshStandardMaterial
          color="#ff00ff"
          emissive="#ff00ff"
          emissiveIntensity={0.6}
          transparent
          opacity={0.4}
        />
      </mesh>
    </group>
  )
}

// Flowing Helix Particles
function HelixParticles() {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = 400
  
  const { positions, colors } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const colors = new Float32Array(particleCount * 3)
    
    for (let i = 0; i < particleCount; i++) {
      const t = (i / particleCount) * Math.PI * 10
      const radius = 8 + Math.sin(t * 0.5) * 3
      positions[i * 3] = Math.cos(t) * radius
      positions[i * 3 + 1] = (i / particleCount - 0.5) * 30
      positions[i * 3 + 2] = Math.sin(t) * radius
      
      // Gradient colors - cyan to magenta to purple
      const colorMix = i / particleCount
      if (colorMix < 0.33) {
        colors[i * 3] = 0
        colors[i * 3 + 1] = 0.94
        colors[i * 3 + 2] = 1
      } else if (colorMix < 0.66) {
        colors[i * 3] = 1
        colors[i * 3 + 1] = 0
        colors[i * 3 + 2] = 1
      } else {
        colors[i * 3] = 0.48
        colors[i * 3 + 1] = 0.18
        colors[i * 3 + 2] = 1
      }
    }
    
    return { positions, colors }
  }, [])
  
  useFrame((state) => {
    if (pointsRef.current) {
      pointsRef.current.rotation.y += 0.002
      pointsRef.current.position.y = Math.sin(state.clock.elapsedTime * 0.15) * 0.5
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
      <pointsMaterial size={0.1} vertexColors transparent opacity={0.85} />
    </points>
  )
}

// Animated Torus component with wireframe for 3D depth effect
function CyberpunkTorus({ position, color }: { position: [number, number, number], color: string }) {
  const meshRef = useRef<THREE.Mesh>(null!)
  const wireRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    if (meshRef.current) {
      meshRef.current.rotation.x = Math.sin(state.clock.elapsedTime) * 0.3
      meshRef.current.rotation.y += 0.01
    }
    if (wireRef.current) {
      wireRef.current.rotation.x = Math.sin(state.clock.elapsedTime) * 0.3
      wireRef.current.rotation.y += 0.01
    }
  })

  return (
    <Float speed={2} rotationIntensity={0.5} floatIntensity={0.5}>
      <group position={position}>
        {/* Solid inner torus */}
        <mesh ref={meshRef}>
          <torusGeometry args={[1, 0.35, 16, 48]} />
          <meshStandardMaterial 
            color={color} 
            emissive={color}
            emissiveIntensity={0.6}
            metalness={0.9}
            roughness={0.1}
            transparent
            opacity={0.7}
          />
        </mesh>
        {/* Wireframe outer for 3D depth */}
        <mesh ref={wireRef}>
          <torusGeometry args={[1.1, 0.4, 12, 36]} />
          <meshStandardMaterial 
            color={color} 
            emissive={color}
            emissiveIntensity={0.8}
            wireframe
            transparent
            opacity={0.5}
          />
        </mesh>
      </group>
    </Float>
  )
}

// Central rotating icosahedron with holographic effect
function CoreShape() {
  const meshRef = useRef<THREE.Mesh>(null!)
  const outerRef = useRef<THREE.Mesh>(null!)
  const [hovered, setHovered] = useState(false)

  useFrame((state) => {
    meshRef.current.rotation.x += 0.005
    meshRef.current.rotation.y += 0.01
    meshRef.current.scale.setScalar(hovered ? 1.3 : 1 + Math.sin(state.clock.elapsedTime) * 0.1)
    
    if (outerRef.current) {
      outerRef.current.rotation.x -= 0.003
      outerRef.current.rotation.y -= 0.005
    }
  })

  return (
    <group>
      <mesh 
        ref={meshRef}
        onPointerOver={() => setHovered(true)}
        onPointerOut={() => setHovered(false)}
      >
        <icosahedronGeometry args={[1.5, 1]} />
        <meshStandardMaterial
          color="#00f0ff"
          emissive="#00f0ff"
          emissiveIntensity={hovered ? 1.2 : 0.4}
          wireframe
          metalness={1}
          roughness={0}
        />
      </mesh>
      
      {/* Outer holographic shell */}
      <mesh ref={outerRef}>
        <icosahedronGeometry args={[2, 0]} />
        <meshStandardMaterial
          color="#ff00ff"
          emissive="#ff00ff"
          emissiveIntensity={0.2}
          wireframe
          transparent
          opacity={0.3}
        />
      </mesh>
    </group>
  )
}

// Particle ring
function ParticleRing() {
  const pointsRef = useRef<THREE.Points>(null!)
  const particleCount = 600
  
  const { positions, colors } = useMemo(() => {
    const positions = new Float32Array(particleCount * 3)
    const colors = new Float32Array(particleCount * 3)
    
    for (let i = 0; i < particleCount; i++) {
      const angle = (i / particleCount) * Math.PI * 2
      const radius = 4.5 + Math.sin(i * 0.1) * 0.5
      positions[i * 3] = Math.cos(angle) * radius
      positions[i * 3 + 1] = Math.sin(i * 0.3) * 0.3
      positions[i * 3 + 2] = Math.sin(angle) * radius
      
      // Cyberpunk colors
      const colorChoice = (i / particleCount)
      if (colorChoice < 0.33) {
        colors[i * 3] = 0; colors[i * 3 + 1] = 0.94; colors[i * 3 + 2] = 1
      } else if (colorChoice < 0.66) {
        colors[i * 3] = 1; colors[i * 3 + 1] = 0; colors[i * 3 + 2] = 1
      } else {
        colors[i * 3] = 0; colors[i * 3 + 1] = 1; colors[i * 3 + 2] = 0.53
      }
    }
    
    return { positions, colors }
  }, [])

  useFrame(() => {
    if (pointsRef.current) {
      pointsRef.current.rotation.y += 0.003
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
      <pointsMaterial size={0.06} vertexColors transparent opacity={0.9} />
    </points>
  )
}

// Floating Orbs with wireframe for 3D depth
function FloatingOrbs() {
  const orbConfigs = [
    { x: -12, y: 6, z: -18, scale: 0.6, speed: 1.0 },
    { x: 10, y: -4, z: -15, scale: 0.5, speed: 1.2 },
    { x: -6, y: 8, z: -22, scale: 0.7, speed: 0.8 },
    { x: 14, y: -6, z: -12, scale: 0.4, speed: 1.4 },
    { x: -10, y: -7, z: -20, scale: 0.55, speed: 0.9 },
    { x: 8, y: 5, z: -17, scale: 0.65, speed: 1.1 },
    { x: -4, y: -3, z: -25, scale: 0.45, speed: 0.7 },
    { x: 12, y: 7, z: -10, scale: 0.55, speed: 1.3 },
    { x: 0, y: -8, z: -30, scale: 0.5, speed: 0.6 },
    { x: -15, y: 3, z: -14, scale: 0.4, speed: 1.0 },
  ]
  
  const colors = ['#00f0ff', '#ff00ff', '#00ff88', '#7b2fff']
  
  return (
    <>
      {orbConfigs.map((config, i) => (
        <Float key={i} speed={config.speed} floatIntensity={2.5}>
          <group position={[config.x, config.y, config.z]} scale={config.scale}>
            {/* Inner glow sphere */}
            <mesh>
              <sphereGeometry args={[0.9, 12, 12]} />
              <meshStandardMaterial
                color={colors[i % 4]}
                emissive={colors[i % 4]}
                emissiveIntensity={0.8}
                transparent
                opacity={0.4}
              />
            </mesh>
            {/* Wireframe outer sphere for 3D depth */}
            <mesh>
              <icosahedronGeometry args={[1, 1]} />
              <meshStandardMaterial
                color={colors[i % 4]}
                emissive={colors[i % 4]}
                emissiveIntensity={0.6}
                wireframe
                transparent
                opacity={0.7}
              />
            </mesh>
          </group>
        </Float>
      ))}
    </>
  )
}

// DNA-like Double Helix structure - enhanced
function DNAHelix() {
  const groupRef = useRef<THREE.Group>(null!)
  const nodeCount = 25
  const radius = 2.5
  const height = 16
  const barThickness = 0.04
  
  useFrame((state) => {
    if (groupRef.current) {
      groupRef.current.rotation.y += 0.004
      groupRef.current.position.y = Math.sin(state.clock.elapsedTime * 0.3) * 0.5
    }
  })
  
  const nodes = useMemo(() => {
    const result = []
    for (let i = 0; i < nodeCount; i++) {
      const t = i / nodeCount
      const angle = t * Math.PI * 5
      const y = (t - 0.5) * height
      
      result.push({
        pos: [Math.cos(angle) * radius, y, Math.sin(angle) * radius] as [number, number, number],
        color: '#00f0ff',
        scale: 0.12
      })
      
      result.push({
        pos: [Math.cos(angle + Math.PI) * radius, y, Math.sin(angle + Math.PI) * radius] as [number, number, number],
        color: '#ff00ff',
        scale: 0.12
      })
    }
    return result
  }, [])
  
  const bars = useMemo(() => {
    const result = []
    for (let i = 0; i < nodeCount; i += 2) {
      const t = i / nodeCount
      const angle = t * Math.PI * 5
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
    <group ref={groupRef} position={[10, 0, -8]}>
      {nodes.map((node, i) => (
        <mesh key={i} position={node.pos} scale={node.scale}>
          <sphereGeometry args={[1, 16, 16]} />
          <meshStandardMaterial
            color={node.color}
            emissive={node.color}
            emissiveIntensity={0.9}
          />
        </mesh>
      ))}
      
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
            emissiveIntensity={0.6}
            transparent
            opacity={0.8}
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
      <ambientLight intensity={0.15} />
      <pointLight position={[10, 10, 10]} intensity={1.2} color="#00f0ff" />
      <pointLight position={[-10, -10, -10]} intensity={0.6} color="#ff00ff" />
      <pointLight position={[0, 15, 0]} intensity={0.8} color="#00ff88" />
      <spotLight position={[0, 20, 10]} intensity={0.6} color="#7b2fff" angle={0.4} />
      
      {/* Gravity Field Mesh - Einstein-style curvature */}
      <GravityFieldMesh />
      <HolographicGrid />
      <HolographicScanlines />
      <GravityText />
      
      {/* Particles and effects */}
      <HelixParticles />
      <FloatingOrbs />
      <DNAHelix />
      
      <CoreShape />
      <ParticleRing />
      
      <CyberpunkTorus position={[-5, 0.5, 0]} color="#ff00ff" />
      <CyberpunkTorus position={[5, 0.5, 0]} color="#00ff88" />
      <CyberpunkTorus position={[0, 4, 0]} color="#7b2fff" />
      
      <Stars radius={150} depth={80} count={8000} factor={5} saturation={0} fade speed={0.8} />
      
      <OrbitControls 
        enableZoom={true}
        enablePan={false}
        autoRotate
        autoRotateSpeed={0.3}
        minDistance={5}
        maxDistance={30}
        maxPolarAngle={Math.PI / 1.8}
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
