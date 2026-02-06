import { OrbitControls, Stars } from '@react-three/drei';
import Core from './Core';
import Particles from './Particles';

export default function Scene() {
  return (
    <>
      <ambientLight intensity={0.5} />
      <directionalLight position={[10, 10, 5]} intensity={1} />
      <pointLight position={[-10, -10, -10]} color="red" intensity={1} />
      
      <Stars radius={100} depth={50} count={5000} factor={4} saturation={0} fade speed={1} />
      
      <Core />
      <Particles />
      
      <OrbitControls enableZoom={true} enablePan={false} autoRotate autoRotateSpeed={0.5} />
    </>
  );
}
