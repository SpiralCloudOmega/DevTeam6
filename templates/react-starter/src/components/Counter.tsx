import { useState } from 'react'

function Counter() {
  const [count, setCount] = useState(0)

  return (
    <div style={{ margin: '2rem 0' }}>
      <h2>Sample Component</h2>
      <p>This is a reusable counter component</p>
      <button onClick={() => setCount(count + 1)}>
        Component count: {count}
      </button>
    </div>
  )
}

export default Counter
