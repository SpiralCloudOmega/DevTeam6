import { Canvas } from '@react-three/fiber';
import Scene from './components/Scene';
import './App.css';

function App() {
  return (
    <div className="canvas-container">
      <div className="overlay">
        <h1>ANTIGRAVITY</h1>
        <p>Interactive 3D Experience</p>
      </div>
      <Canvas camera={{ position: [0, 0, 30], fov: 45 }}>
        <Scene />
      </Canvas>
    </div>
  );
}

export default App;
