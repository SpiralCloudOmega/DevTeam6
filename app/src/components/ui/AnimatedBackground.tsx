import { useRef, useMemo, useState, useEffect } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { Points, PointMaterial } from '@react-three/drei';
import * as THREE from 'three';

function ParticleField({ count = 3000 }) {
  const ref = useRef<THREE.Points>(null);
  
  const particles = useMemo(() => {
    const pos = new Float32Array(count * 3);
    for (let i = 0; i < count; i++) {
      pos[i * 3] = (Math.random() - 0.5) * 50;
      pos[i * 3 + 1] = (Math.random() - 0.5) * 50;
      pos[i * 3 + 2] = (Math.random() - 0.5) * 50;
    }
    return pos;
  }, [count]);

  useFrame((state) => {
    if (ref.current) {
      ref.current.rotation.x = state.clock.elapsedTime * 0.02;
      ref.current.rotation.y = state.clock.elapsedTime * 0.03;
    }
  });

  return (
    <Points ref={ref} positions={particles} stride={3} frustumCulled={false}>
      <PointMaterial
        transparent
        color="#6366f1"
        size={0.05}
        sizeAttenuation
        depthWrite={false}
        blending={THREE.AdditiveBlending}
      />
    </Points>
  );
}

// CSS gradient fallback for devices without WebGL
function GradientFallback() {
  return (
    <div 
      className="fixed inset-0 -z-10"
      style={{
        background: 'linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0a0a1a 100%)',
        animation: 'gradientShift 15s ease infinite',
      }}
    >
      <style>{`
        @keyframes gradientShift {
          0%, 100% { 
            background: linear-gradient(135deg, #0a0a1a 0%, #1a0a2e 50%, #0a0a1a 100%);
          }
          50% { 
            background: linear-gradient(225deg, #0f0f23 0%, #1a0f2e 50%, #0f0f23 100%);
          }
        }
      `}</style>
    </div>
  );
}

// Check WebGL support
function checkWebGLSupport(): boolean {
  try {
    const canvas = document.createElement('canvas');
    const gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');
    return !!gl;
  } catch (e) {
    return false;
  }
}

export function AnimatedBackground() {
  const [hasWebGL, setHasWebGL] = useState<boolean | null>(null);
  const [error, setError] = useState(false);

  useEffect(() => {
    // Check WebGL support on mount
    setHasWebGL(checkWebGLSupport());
  }, []);

  // Show fallback during check or if no WebGL
  if (hasWebGL === null || hasWebGL === false || error) {
    return <GradientFallback />;
  }

  return (
    <div className="fixed inset-0 -z-10">
      <Canvas 
        camera={{ position: [0, 0, 10], fov: 75 }}
        onCreated={({ gl }) => {
          // Enable better performance on mobile
          gl.setPixelRatio(Math.min(window.devicePixelRatio, 2));
        }}
        onError={() => {
          console.warn('WebGL error detected, falling back to CSS gradient');
          setError(true);
        }}
      >
        <color attach="background" args={['#0f0f23']} />
        <ParticleField />
        <ambientLight intensity={0.5} />
      </Canvas>
    </div>
  );
}

export default AnimatedBackground;
