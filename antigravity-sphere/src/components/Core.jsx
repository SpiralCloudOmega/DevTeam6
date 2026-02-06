import { useRef } from 'react';
import { useFrame } from '@react-three/fiber';
import { Float } from '@react-three/drei';

export default function Core() {
  const meshRef = useRef();
  
  useFrame((state, delta) => {
    if (meshRef.current) {
        meshRef.current.rotation.x += delta * 0.2;
        meshRef.current.rotation.y += delta * 0.3;
    }
  });

  return (
    <Float speed={2} rotationIntensity={0.5} floatIntensity={0.5}>
      <mesh ref={meshRef}>
        <icosahedronGeometry args={[1.5, 0]} />
        <meshStandardMaterial 
          color="#8b5cf6" 
          emissive="#7c3aed"
          emissiveIntensity={2}
          wireframe={true}
        />
        <pointLight distance={10} intensity={5} color="#8b5cf6" />
      </mesh>
      
      {/* Internal solid core */}
      <mesh scale={0.8}>
        <dodecahedronGeometry args={[1, 0]} />
        <meshStandardMaterial color="#000000" roughness={0.1} metalness={1} />
      </mesh>
    </Float>
  );
}
