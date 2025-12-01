import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars, Float } from '@react-three/drei'
import { Suspense, useRef, useState, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

// High-Resolution Perspective Grid - Creates deep 3D space with tight triangles
// Designed for 8K-like resolution with compressed triangle mesh
function PerspectiveGrid() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  // High resolution grid configuration - tight triangles for quality
  const GRID_WIDTH = 200
  const GRID_DEPTH = 400
  const SEGMENTS_X = 200  // High segment count for tight triangles
  const SEGMENTS_Z = 400  // More segments for depth
  
  const { positions, indices, colors } = useMemo(() => {
    const positions = new Float32Array((SEGMENTS_X + 1) * (SEGMENTS_Z + 1) * 3)
    const colors = new Float32Array((SEGMENTS_X + 1) * (SEGMENTS_Z + 1) * 3)
    const indices: number[] = []
    
    for (let z = 0; z <= SEGMENTS_Z; z++) {
      for (let x = 0; x <= SEGMENTS_X; x++) {
        const idx = (z * (SEGMENTS_X + 1) + x)
        const posIdx = idx * 3
        
        // Position in world space
        const worldX = (x / SEGMENTS_X - 0.5) * GRID_WIDTH
        const worldZ = (z / SEGMENTS_Z) * GRID_DEPTH - 50  // Start closer to camera
        
        positions[posIdx] = worldX
        positions[posIdx + 1] = 0
        positions[posIdx + 2] = worldZ
        
        // Color gradient - cyan to purple based on depth
        const depthFactor = z / SEGMENTS_Z
        colors[posIdx] = 0 + depthFactor * 0.3      // R
        colors[posIdx + 1] = 0.6 - depthFactor * 0.3  // G (cyan fades)
        colors[posIdx + 2] = 0.8 + depthFactor * 0.2  // B (stays blue/purple)
      }
    }
    
    // Create triangle indices
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
        
        // Smooth wave animation - 45 degree angle waves
        const wave1 = Math.sin((x + z) * 0.03 + time * 0.4) * 1.5  // Diagonal wave
        const wave2 = Math.sin((x - z) * 0.02 + time * 0.3) * 1.0  // Counter diagonal
        const wave3 = Math.sin(z * 0.015 - time * 0.5) * 2.0       // Forward moving wave
        
        // Combine waves with distance fade
        const distanceFade = Math.max(0, 1 - Math.abs(z + 50) / 350)
        const finalY = (wave1 + wave2 + wave3) * distanceFade
        
        positionAttr.setY(i, finalY)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, -8, 0]} rotation={[-Math.PI / 2.5, 0, 0]}>
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
        opacity={0.7}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Secondary deeper grid layer for parallax depth
function DeepSpaceGrid() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  const GRID_SIZE = 300
  const SEGMENTS = 150
  
  const { positions, indices } = useMemo(() => {
    const positions = new Float32Array((SEGMENTS + 1) * (SEGMENTS + 1) * 3)
    const indices: number[] = []
    
    for (let z = 0; z <= SEGMENTS; z++) {
      for (let x = 0; x <= SEGMENTS; x++) {
        const idx = (z * (SEGMENTS + 1) + x) * 3
        positions[idx] = (x / SEGMENTS - 0.5) * GRID_SIZE
        positions[idx + 1] = 0
        positions[idx + 2] = (z / SEGMENTS) * GRID_SIZE * 2 - 100
      }
    }
    
    for (let z = 0; z < SEGMENTS; z++) {
      for (let x = 0; x < SEGMENTS; x++) {
        const a = z * (SEGMENTS + 1) + x
        const b = a + 1
        const c = a + SEGMENTS + 1
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
        
        // Slower, deeper waves for background layer
        const wave = Math.sin((x + z) * 0.02 + time * 0.2) * 
                     Math.cos((x - z) * 0.015 + time * 0.15) * 3
        
        const distanceFade = Math.max(0, 1 - Math.abs(z + 100) / 500)
        positionAttr.setY(i, wave * distanceFade)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, -20, -50]} rotation={[-Math.PI / 2.3, 0, 0]}>
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
      <meshBasicMaterial
        color="#1a0a3e"
        wireframe
        transparent
        opacity={0.4}
        side={THREE.DoubleSide}
      />
    </mesh>
  )
}

// Gravity Field Mesh - Einstein-style spacetime curvature with high resolution
function GravityFieldMesh() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  // Constants for gravity calculations
  const GRAVITY_RADIUS_MULTIPLIER = 4
  const GRAVITY_FALLOFF_POWER = 2.5
  
  // Gravity sources - positions and "mass" that warp the mesh
  const gravitySources = useMemo(() => [
    { x: 0, z: 0, mass: 6.0, radius: 10 },     // Center - main DevTeam6 logo
    { x: -6, z: 3, mass: 3.0, radius: 5 },     // Left torus
    { x: 6, z: 3, mass: 3.0, radius: 5 },      // Right torus
    { x: 0, z: -5, mass: 2.5, radius: 4 },     // Core shape
  ], [])
  
  const { positions, indices, colors } = useMemo(() => {
    const width = 100
    const height = 120
    const segments = 150  // Higher resolution for tighter triangles
    const positions = new Float32Array((segments + 1) * (segments + 1) * 3)
    const colors = new Float32Array((segments + 1) * (segments + 1) * 3)
    const indices: number[] = []
    
    for (let y = 0; y <= segments; y++) {
      for (let x = 0; x <= segments; x++) {
        const idx = (y * (segments + 1) + x)
        const posIdx = idx * 3
        
        positions[posIdx] = (x / segments - 0.5) * width
        positions[posIdx + 1] = 0
        positions[posIdx + 2] = (y / segments - 0.5) * height
        
        // Gradient colors based on position
        const t = y / segments
        colors[posIdx] = 0 + t * 0.2          // R
        colors[posIdx + 1] = 0.94 - t * 0.4   // G (cyan)
        colors[posIdx + 2] = 1                 // B
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
        
        // Calculate gravity well depth from all sources
        let gravityDepth = 0
        for (const source of gravitySources) {
          const dx = x - source.x
          const dz = z - source.z
          const distance = Math.sqrt(dx * dx + dz * dz)
          
          if (distance < source.radius * GRAVITY_RADIUS_MULTIPLIER) {
            const normalizedDist = distance / (source.radius * GRAVITY_RADIUS_MULTIPLIER)
            const wellDepth = source.mass * Math.pow(1 - normalizedDist, GRAVITY_FALLOFF_POWER)
            gravityDepth += wellDepth
          }
        }
        
        // 45 degree diagonal wave animation
        const wave1 = Math.sin((x + z) * 0.08 + time * 0.4) * 0.4
        const wave2 = Math.sin((x - z) * 0.06 + time * 0.3) * 0.3
        
        const finalY = -gravityDepth + wave1 + wave2
        positionAttr.setY(i, finalY)
      }
      
      positionAttr.needsUpdate = true
      geometry.computeVertexNormals()
    }
  })
  
  return (
    <mesh ref={meshRef} position={[0, 0, 5]} rotation={[-Math.PI / 2.6, 0, 0]}>
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
      {/* Stars in far background - render first so they appear behind everything */}
      <Stars radius={300} depth={200} count={15000} factor={6} saturation={0} fade speed={0.5} />
      
      {/* Lighting */}
      <ambientLight intensity={0.12} />
      <pointLight position={[10, 10, 10]} intensity={1.0} color="#00f0ff" />
      <pointLight position={[-10, -10, -10]} intensity={0.5} color="#ff00ff" />
      <pointLight position={[0, 15, 0]} intensity={0.6} color="#00ff88" />
      <spotLight position={[0, 20, 10]} intensity={0.5} color="#7b2fff" angle={0.4} />
      
      {/* Deep background grid layers - rendered first for proper depth */}
      <DeepSpaceGrid />
      <PerspectiveGrid />
      
      {/* Gravity Field Mesh - Einstein-style curvature (in front of perspective grid) */}
      <GravityFieldMesh />
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
