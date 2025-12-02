import { Canvas } from '@react-three/fiber'
import { OrbitControls, Stars } from '@react-three/drei'
import { Suspense, useRef, useMemo } from 'react'
import { useFrame } from '@react-three/fiber'
import * as THREE from 'three'

// Configuration constants for performance and visual tuning
const GRID_CONFIG = {
  WIDTH: 400,
  DEPTH: 600,
  SEGMENTS_X: 200,  // Higher = tighter triangles, more GPU intensive
  SEGMENTS_Z: 300,
  PERSPECTIVE_FACTOR: 0.6  // Controls how much the grid widens with distance
}

const PARTICLE_CONFIG = {
  COUNT: 1200,  // Particle count - reduce for lower-end devices
  BOUNDS: 40,   // Z-axis bounds for particle wrapping
  ELECTRON_ORBIT_COUNT: 30,  // Number of electron particles per orb
  STAR_COUNT: 18000  // Background star count
}

const LIGHTING_CONFIG = {
  SPOTLIGHT: {
    position: [0, 20, 10] as [number, number, number],
    intensity: 0.5,
    angle: 0.3,
    penumbra: 1
  }
}

// Black Hole Vortex Grid - Creates TWO mirrored grids that meet at center
// Like being sucked into a black hole with mesh converging at center
function BlackHoleVortexGrid({ isFlipped = false }: { isFlipped?: boolean }) {
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
        
        // World position - grid converges toward center point with perspective
        const perspectiveScale = 1 + nz * PERSPECTIVE_FACTOR
        const worldX = nx * WIDTH * perspectiveScale
        const worldZ = nz * DEPTH
        
        positions[posIdx] = worldX
        positions[posIdx + 1] = 0
        positions[posIdx + 2] = worldZ
        
        // Color gradient - cyan to purple with depth shading
        // Creates vortex color effect converging to center
        const depthFactor = nz
        const xFactor = Math.abs(nx) * 2
        const vortexIntensity = 1 - depthFactor * 0.5
        
        // Different colors for top and bottom grids
        if (isFlipped) {
          colors[posIdx] = 0.5 * vortexIntensity + xFactor * 0.1  // R - magenta
          colors[posIdx + 1] = 0.0  // G
          colors[posIdx + 2] = 0.9 * vortexIntensity  // B - purple
        } else {
          colors[posIdx] = 0.0 + depthFactor * 0.3  // R
          colors[posIdx + 1] = 0.8 * vortexIntensity - xFactor * 0.2  // G - cyan
          colors[posIdx + 2] = 1.0 * vortexIntensity  // B
        }
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
  }, [isFlipped])
  
  useFrame((state) => {
    if (meshRef.current) {
      const geometry = meshRef.current.geometry
      const positionAttr = geometry.getAttribute('position')
      const time = state.clock.elapsedTime
      
      for (let i = 0; i < positionAttr.count; i++) {
        const x = positionAttr.getX(i)
        const z = positionAttr.getZ(i)
        
        // Vortex wave - pulls toward center with spiral motion
        const distToCenter = Math.sqrt(x * x + (z - GRID_CONFIG.DEPTH/2) * (z - GRID_CONFIG.DEPTH/2))
        const spiralAngle = distToCenter * 0.01 + time * 0.3
        
        const wave1 = Math.sin((x + z) * 0.015 + time * 0.4) * 4
        const wave2 = Math.cos((x - z) * 0.012 + time * 0.3) * 3
        const wave3 = Math.sin(spiralAngle) * 2  // Spiral wave
        
        // Waves intensify toward center for vortex pull effect
        const centerPull = 1 + (1 - z / GRID_CONFIG.DEPTH) * 0.5
        const finalY = (wave1 + wave2 + wave3) * centerPull * 0.8
        
        positionAttr.setY(i, finalY)
      }
      
      positionAttr.needsUpdate = true
    }
  })
  
  // Position and rotation differ for top vs bottom grid
  const yPos = isFlipped ? 35 : -35
  const rotX = isFlipped ? Math.PI / 2.5 : -Math.PI / 2.5
  
  return (
    <mesh ref={meshRef} position={[0, yPos, -50]} rotation={[rotX, 0, 0]}>
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

// Electron Energy Orb - Swirling energy sphere like a planet with electron rings
function ElectronEnergyOrb({ 
  position, 
  color, 
  size,
  orbitSpeed = 1,
  pulseSpeed = 1
}: { 
  position: [number, number, number], 
  color: string, 
  size: number,
  orbitSpeed?: number,
  pulseSpeed?: number
}) {
  const groupRef = useRef<THREE.Group>(null!)
  const coreRef = useRef<THREE.Mesh>(null!)
  const ring1Ref = useRef<THREE.Mesh>(null!)
  const ring2Ref = useRef<THREE.Mesh>(null!)
  const ring3Ref = useRef<THREE.Mesh>(null!)
  const glowRef = useRef<THREE.Mesh>(null!)
  const particlesRef = useRef<THREE.Points>(null!)
  
  // Electron particles orbiting the orb
  const electronPositions = useMemo(() => {
    const count = PARTICLE_CONFIG.ELECTRON_ORBIT_COUNT
    const positions = new Float32Array(count * 3)
    for (let i = 0; i < count; i++) {
      const angle = (i / count) * Math.PI * 2
      const r = size * 1.5
      positions[i * 3] = Math.cos(angle) * r
      positions[i * 3 + 1] = Math.sin(angle * 3) * 0.3
      positions[i * 3 + 2] = Math.sin(angle) * r
    }
    return positions
  }, [size])
  
  useFrame((state) => {
    const time = state.clock.elapsedTime
    
    if (groupRef.current) {
      // Gentle floating motion
      groupRef.current.position.y = position[1] + Math.sin(time * 0.5 * pulseSpeed) * 0.5
    }
    
    if (coreRef.current) {
      // Core rotation
      coreRef.current.rotation.y += 0.01 * orbitSpeed
      coreRef.current.rotation.z = Math.sin(time * 0.5) * 0.1
    }
    
    // Electron rings orbit at different angles and speeds
    if (ring1Ref.current) {
      ring1Ref.current.rotation.x = Math.PI / 2 + Math.sin(time * 0.3) * 0.2
      ring1Ref.current.rotation.z += 0.02 * orbitSpeed
    }
    if (ring2Ref.current) {
      ring2Ref.current.rotation.y += 0.015 * orbitSpeed
      ring2Ref.current.rotation.x = Math.PI / 3 + Math.cos(time * 0.4) * 0.15
    }
    if (ring3Ref.current) {
      ring3Ref.current.rotation.z += 0.025 * orbitSpeed
      ring3Ref.current.rotation.y = Math.PI / 4 + Math.sin(time * 0.35) * 0.1
    }
    
    // Pulsing glow
    if (glowRef.current) {
      const pulse = 1 + Math.sin(time * 2 * pulseSpeed) * 0.2
      glowRef.current.scale.setScalar(pulse)
    }
    
    // Orbiting electron particles
    if (particlesRef.current) {
      particlesRef.current.rotation.y += 0.03 * orbitSpeed
      particlesRef.current.rotation.x = Math.sin(time * 0.5) * 0.3
    }
  })
  
  return (
    <group ref={groupRef} position={position}>
      {/* Outer glow sphere */}
      <mesh ref={glowRef}>
        <sphereGeometry args={[size * 1.8, 16, 16]} />
        <meshBasicMaterial color={color} transparent opacity={0.08} />
      </mesh>
      
      {/* Swirling core - icosahedron for energy look */}
      <mesh ref={coreRef}>
        <icosahedronGeometry args={[size, 2]} />
        <meshStandardMaterial
          color={color}
          emissive={color}
          emissiveIntensity={0.6}
          transparent
          opacity={0.7}
        />
      </mesh>
      
      {/* Inner wireframe for depth */}
      <mesh>
        <icosahedronGeometry args={[size * 0.8, 1]} />
        <meshBasicMaterial color="#ffffff" wireframe transparent opacity={0.3} />
      </mesh>
      
      {/* Electron orbit rings - thin torus rings at different angles */}
      <mesh ref={ring1Ref}>
        <torusGeometry args={[size * 1.4, 0.02, 8, 64]} />
        <meshBasicMaterial color={color} transparent opacity={0.6} />
      </mesh>
      <mesh ref={ring2Ref}>
        <torusGeometry args={[size * 1.6, 0.015, 8, 64]} />
        <meshBasicMaterial color="#ffffff" transparent opacity={0.4} />
      </mesh>
      <mesh ref={ring3Ref}>
        <torusGeometry args={[size * 1.2, 0.025, 8, 64]} />
        <meshBasicMaterial color={color} transparent opacity={0.5} />
      </mesh>
      
      {/* Orbiting electron particles */}
      <points ref={particlesRef}>
        <bufferGeometry>
          <bufferAttribute
            attach="attributes-position"
            count={PARTICLE_CONFIG.ELECTRON_ORBIT_COUNT}
            array={electronPositions}
            itemSize={3}
          />
        </bufferGeometry>
        <pointsMaterial size={0.1} color={color} transparent opacity={0.9} />
      </points>
    </group>
  )
}

// Glowing central ring effect - creates the "event horizon" look
function VortexEventHorizon() {
  const ring1Ref = useRef<THREE.Mesh>(null!)
  const ring2Ref = useRef<THREE.Mesh>(null!)
  const ring3Ref = useRef<THREE.Mesh>(null!)
  const glowRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state) => {
    const time = state.clock.elapsedTime
    
    if (ring1Ref.current) {
      ring1Ref.current.rotation.z += 0.008
      ring1Ref.current.rotation.x = Math.sin(time * 0.3) * 0.1
    }
    if (ring2Ref.current) {
      ring2Ref.current.rotation.z -= 0.006
      ring2Ref.current.rotation.y = Math.cos(time * 0.4) * 0.1
    }
    if (ring3Ref.current) {
      ring3Ref.current.rotation.z += 0.004
    }
    if (glowRef.current) {
      const pulse = 1 + Math.sin(time * 2) * 0.15
      glowRef.current.scale.setScalar(pulse)
    }
  })
  
  return (
    <group position={[0, 0, 0]}>
      {/* Central glow */}
      <mesh ref={glowRef}>
        <sphereGeometry args={[3, 32, 32]} />
        <meshBasicMaterial color="#00f0ff" transparent opacity={0.08} />
      </mesh>
      {/* Inner bright ring */}
      <mesh ref={ring1Ref} rotation={[Math.PI / 2, 0, 0]}>
        <torusGeometry args={[4, 0.06, 16, 100]} />
        <meshBasicMaterial color="#00f0ff" transparent opacity={0.8} />
      </mesh>
      {/* Middle ring */}
      <mesh ref={ring2Ref} rotation={[Math.PI / 2.2, 0.2, 0]}>
        <torusGeometry args={[5.5, 0.04, 16, 100]} />
        <meshBasicMaterial color="#ff00ff" transparent opacity={0.6} />
      </mesh>
      {/* Outer ring */}
      <mesh ref={ring3Ref} rotation={[Math.PI / 1.8, -0.1, 0]}>
        <torusGeometry args={[7, 0.03, 16, 100]} />
        <meshBasicMaterial color="#7b2fff" transparent opacity={0.4} />
      </mesh>
    </group>
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

// Scene component - Black Hole Vortex with Electron Energy Orbs
function Scene() {
  return (
    <>
      {/* Layer 1: Stars - Furthest back (Z = very far) */}
      <Stars radius={500} depth={400} count={PARTICLE_CONFIG.STAR_COUNT} factor={5} saturation={0} fade speed={0.2} />
      
      {/* Layer 2: Nebula clouds - Far background (Z = 50-150) */}
      <NebulaCloud position={[-50, 15, 100]} color="#ff00ff" scale={35} speed={0.25} />
      <NebulaCloud position={[60, -10, 120]} color="#7b2fff" scale={45} speed={0.18} />
      <NebulaCloud position={[0, 25, 140]} color="#00f0ff" scale={30} speed={0.35} />
      <NebulaCloud position={[-35, -20, 110]} color="#00ff88" scale={25} speed={0.3} />
      <NebulaCloud position={[45, 5, 90]} color="#ff6600" scale={20} speed={0.4} />
      
      {/* Layer 3: BLACK HOLE VORTEX - Two mirrored grids meeting at center */}
      {/* Bottom grid - cyan colored, extends downward */}
      <BlackHoleVortexGrid isFlipped={false} />
      {/* Top grid - magenta/purple colored, extends upward (mirrored) */}
      <BlackHoleVortexGrid isFlipped={true} />
      
      {/* Layer 4: Data stream lanes */}
      <DataStreamLane position={[-15, 2, 0]} color="#00f0ff" direction={1} />
      <DataStreamLane position={[15, -1, 0]} color="#ff00ff" direction={-1} />
      <DataStreamLane position={[0, 4, 10]} color="#00ff88" direction={1} />
      
      {/* Layer 5: Holographic scanlines */}
      <HolographicScanlines />
      
      {/* Layer 6: Particle field - Mid-ground (Z = 5-55) */}
      <ParticleField />
      
      {/* Layer 7: ELECTRON ENERGY ORBS - Replace doughnut torus with swirling planet-like orbs */}
      {/* Large orbs on the sides - like swirling planets */}
      <ElectronEnergyOrb position={[-10, 0, 5]} color="#ff00ff" size={2.2} orbitSpeed={1.2} pulseSpeed={0.8} />
      <ElectronEnergyOrb position={[10, 0, 5]} color="#00ff88" size={2.2} orbitSpeed={0.9} pulseSpeed={1.1} />
      {/* Medium orbs at different depths */}
      <ElectronEnergyOrb position={[-6, 4, 12]} color="#00f0ff" size={1.5} orbitSpeed={1.5} pulseSpeed={0.9} />
      <ElectronEnergyOrb position={[7, -3, 15]} color="#7b2fff" size={1.4} orbitSpeed={1.1} pulseSpeed={1.2} />
      <ElectronEnergyOrb position={[0, 5, 20]} color="#ff6600" size={1.2} orbitSpeed={0.8} pulseSpeed={1.0} />
      {/* Small distant orbs for depth */}
      <ElectronEnergyOrb position={[-15, 3, 25]} color="#00f0ff" size={0.8} orbitSpeed={2.0} pulseSpeed={0.7} />
      <ElectronEnergyOrb position={[12, -5, 30]} color="#ff00ff" size={0.7} orbitSpeed={1.8} pulseSpeed={1.3} />
      <ElectronEnergyOrb position={[-8, -4, 35]} color="#00ff88" size={0.6} orbitSpeed={1.6} pulseSpeed={0.6} />
      
      {/* Layer 8: Event horizon rings at the vortex center */}
      <VortexEventHorizon />
      
      {/* Layer 9: Central geodesic core - Small focal point behind text */}
      <group position={[0, 0, -3]}>
        <GeodesicCore />
      </group>
      
      {/* Enhanced Lighting */}
      <ambientLight intensity={0.15} />
      <pointLight position={[15, 15, -15]} intensity={1.2} color="#00f0ff" />
      <pointLight position={[-15, -10, 25]} intensity={0.8} color="#ff00ff" />
      <pointLight position={[0, 10, 0]} intensity={0.5} color="#ffffff" />
      <pointLight position={[0, -15, 10]} intensity={0.6} color="#7b2fff" />
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
        autoRotateSpeed={0.12}
        minDistance={8}
        maxDistance={60}
        maxPolarAngle={Math.PI / 1.3}
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
