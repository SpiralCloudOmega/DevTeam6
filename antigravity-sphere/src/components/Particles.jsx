import { useRef, useMemo } from 'react';
import { useFrame, useThree } from '@react-three/fiber';
import * as THREE from 'three';

const COUNT = 1000;

export default function Particles() {
  const meshRef = useRef();
  const { viewport, mouse } = useThree();
  
  const dummy = useMemo(() => new THREE.Object3D(), []);
  
  const particles = useMemo(() => {
    const temp = [];
    for (let i = 0; i < COUNT; i++) {
      const t = Math.random() * 100;
      const factor = 20 + Math.random() * 100;
      const speed = 0.01 + Math.random() / 200;
      const xFactor = -50 + Math.random() * 100;
      const yFactor = -50 + Math.random() * 100;
      const zFactor = -50 + Math.random() * 100;
      temp.push({ t, factor, speed, xFactor, yFactor, zFactor, mx: 0, my: 0 });
    }
    return temp;
  }, []);

  useFrame((state, delta) => {
    if (!meshRef.current) return;

    particles.forEach((particle, i) => {
      let { t, factor, speed, xFactor, yFactor, zFactor } = particle;
      
      // Update time
      t = particle.t += speed / 2;
      const a = Math.cos(t) + Math.sin(t * 1) / 10;
      const b = Math.sin(t) + Math.cos(t * 2) / 10;
      
      // Mouse interaction (repulsion)
      const mouseX = (mouse.x * viewport.width) / 2;
      const mouseY = (mouse.y * viewport.height) / 2;
      
      // Update dummy object position
      dummy.position.set(
        (particle.mx + Math.cos(t) * 10) + xFactor + Math.cos(t/2) * 5 + (mouseX * 2 * (i % 10 === 0 ? 1 : 0)), 
        (particle.my + Math.sin(t) * 10) + yFactor + Math.sin(t/2) * 5 + (mouseY * 2 * (i % 10 === 0 ? 1 : 0)),
        (particle.my / 10) * b + zFactor + Math.cos(t/3) * 5
      );
      
      // Scale pulsing
      const s = Math.cos(t);
      dummy.scale.set(s, s, s);
      
      // Rotation
      dummy.rotation.set(s * 5, s * 5, s * 5);
      
      dummy.updateMatrix();
      meshRef.current.setMatrixAt(i, dummy.matrix);
    });
    
    meshRef.current.instanceMatrix.needsUpdate = true;
  });

  return (
    <instancedMesh ref={meshRef} args={[null, null, COUNT]}>
      <dodecahedronGeometry args={[0.2, 0]} />
      <meshStandardMaterial color="#60a5fa" transparent opacity={0.6} roughness={0.5} />
    </instancedMesh>
  );
}
