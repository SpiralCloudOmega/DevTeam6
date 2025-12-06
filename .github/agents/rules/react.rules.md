# ⚛️ React Specialist Rules

> **@react agent guidelines for React/TypeScript/Three.js development**

---

## 🎯 Expertise Areas

- React 18+ with hooks and functional components
- TypeScript strict mode
- Three.js / React Three Fiber
- Vite build system
- State management (useState, useReducer, Zustand)
- CSS-in-JS and styled-components
- Testing with Vitest

---

## 📋 Code Standards

### Component Structure
```typescript
// 1. Imports (external, then internal)
import { useState, useCallback } from 'react'
import { SomeComponent } from './SomeComponent'

// 2. Types/Interfaces
interface Props {
  title: string
  onAction: () => void
}

// 3. Component
export function MyComponent({ title, onAction }: Props) {
  // 3a. Hooks
  const [state, setState] = useState<string>('')
  
  // 3b. Callbacks
  const handleClick = useCallback(() => {
    onAction()
  }, [onAction])
  
  // 3c. Effects
  useEffect(() => {
    // setup
    return () => { /* cleanup */ }
  }, [])
  
  // 3d. Render
  return (
    <div>
      <h1>{title}</h1>
      <button onClick={handleClick}>Action</button>
    </div>
  )
}
```

### Naming Conventions
| Type | Convention | Example |
|------|------------|---------|
| Components | PascalCase | `MyComponent` |
| Hooks | camelCase with `use` prefix | `useMyHook` |
| Event handlers | camelCase with `handle` prefix | `handleClick` |
| Constants | SCREAMING_SNAKE_CASE | `MAX_COUNT` |
| Types/Interfaces | PascalCase | `UserProps` |

### File Structure
```
src/
├── components/
│   ├── common/        # Shared components
│   ├── features/      # Feature-specific components
│   └── layout/        # Layout components
├── hooks/             # Custom hooks
├── utils/             # Utility functions
├── types/             # TypeScript types
├── pages/             # Page components
└── styles/            # Global styles
```

---

## 🔧 Best Practices

### Hooks
- Use custom hooks for reusable logic
- Keep hooks at the top of components
- Don't call hooks conditionally
- Use `useCallback` for stable function references
- Use `useMemo` for expensive computations

### State Management
- Keep state as close to usage as possible
- Lift state only when necessary
- Use reducers for complex state logic
- Consider Zustand for global state

### Performance
- Use React.memo for expensive renders
- Implement proper key props in lists
- Lazy load components with React.lazy
- Use virtualization for long lists

### Testing
- Test behavior, not implementation
- Use Vitest + React Testing Library
- Write tests for edge cases
- Maintain test coverage above 80%

---

## 🎨 Three.js / R3F Guidelines

### Component Pattern
```typescript
import { useFrame } from '@react-three/fiber'
import { useRef } from 'react'

export function AnimatedMesh() {
  const meshRef = useRef<THREE.Mesh>(null!)
  
  useFrame((state, delta) => {
    meshRef.current.rotation.y += delta
  })
  
  return (
    <mesh ref={meshRef}>
      <boxGeometry args={[1, 1, 1]} />
      <meshStandardMaterial color="#00f0ff" />
    </mesh>
  )
}
```

### Performance Tips
- Dispose of geometries and materials
- Use instancing for repeated objects
- Implement LOD for complex scenes
- Optimize draw calls

---

## 🚫 Anti-Patterns to Avoid

1. **Don't** mutate state directly
2. **Don't** use index as key for dynamic lists
3. **Don't** fetch data in render
4. **Don't** create components inside components
5. **Don't** overuse Context for local state

---

*Version: 1.0.0*
*Last Updated: 2025-12-05*
