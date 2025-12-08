// Sandpack template configurations for CodeWorkstation
// Provides ready-to-use code examples for React, Vue 3, and Vanilla TS

export const reactTemplate = {
  template: "react-ts" as const,
  files: {
    "/App.tsx": {
      code: `import { useState } from 'react';
import './styles.css';

// React 18 Counter Component with TypeScript
// Demonstrates useState hook and event handling
export default function App() {
  const [count, setCount] = useState(0);

  return (
    <div className="app">
      <div className="card">
        <h1>⚛️ React Counter</h1>
        <p className="description">
          Click the button to increment the counter
        </p>
        
        <div className="counter-display">
          <span className="count">{count}</span>
        </div>
        
        <div className="button-group">
          <button 
            onClick={() => setCount(count + 1)}
            className="button button-primary"
          >
            Increment
          </button>
          <button 
            onClick={() => setCount(0)}
            className="button button-secondary"
          >
            Reset
          </button>
        </div>
        
        <p className="hint">
          Edit <code>App.tsx</code> to see changes live!
        </p>
      </div>
    </div>
  );
}`,
    },
    "/styles.css": {
      code: `* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
  background: linear-gradient(135deg, #0f0f23 0%, #1a1a3e 100%);
  min-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #e2e8f0;
}

.app {
  padding: 2rem;
}

.card {
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 1rem;
  padding: 2rem;
  max-width: 400px;
  text-align: center;
}

h1 {
  font-size: 2rem;
  margin-bottom: 0.5rem;
  background: linear-gradient(135deg, #6366f1 0%, #22d3ee 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.description {
  color: #94a3b8;
  margin-bottom: 2rem;
}

.counter-display {
  margin: 2rem 0;
}

.count {
  font-size: 4rem;
  font-weight: bold;
  color: #6366f1;
  text-shadow: 0 0 20px rgba(99, 102, 241, 0.5);
}

.button-group {
  display: flex;
  gap: 1rem;
  margin-bottom: 2rem;
}

.button {
  flex: 1;
  padding: 0.75rem 1.5rem;
  border: none;
  border-radius: 0.5rem;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s;
}

.button-primary {
  background: #6366f1;
  color: white;
}

.button-primary:hover {
  background: #4f46e5;
  transform: translateY(-2px);
}

.button-secondary {
  background: rgba(255, 255, 255, 0.1);
  color: #e2e8f0;
}

.button-secondary:hover {
  background: rgba(255, 255, 255, 0.15);
}

.hint {
  font-size: 0.875rem;
  color: #64748b;
}

code {
  background: rgba(99, 102, 241, 0.2);
  padding: 0.2rem 0.4rem;
  border-radius: 0.25rem;
  font-family: 'Courier New', monospace;
  color: #22d3ee;
}`,
    },
  },
  customSetup: {
    dependencies: {
      react: "^18.2.0",
      "react-dom": "^18.2.0",
    },
  },
};

export const vueTemplate = {
  template: "vue-ts" as const,
  files: {
    "/App.vue": {
      code: `<script setup lang="ts">
import { ref } from 'vue';

// Vue 3 Composition API Counter
// Demonstrates reactive refs and computed properties
const count = ref(0);

const increment = () => {
  count.value++;
};

const reset = () => {
  count.value = 0;
};

const doubleCount = () => count.value * 2;
</script>

<template>
  <div class="app">
    <div class="card">
      <h1>💚 Vue 3 Counter</h1>
      <p class="description">
        Using Composition API with TypeScript
      </p>
      
      <div class="counter-display">
        <span class="count">{{ count }}</span>
      </div>
      
      <div class="stats">
        <div class="stat">
          <span class="stat-label">Double:</span>
          <span class="stat-value">{{ doubleCount() }}</span>
        </div>
      </div>
      
      <div class="button-group">
        <button 
          @click="increment"
          class="button button-primary"
        >
          Increment
        </button>
        <button 
          @click="reset"
          class="button button-secondary"
        >
          Reset
        </button>
      </div>
      
      <p class="hint">
        Edit <code>App.vue</code> to see reactive changes!
      </p>
    </div>
  </div>
</template>

<style scoped>
* {
  box-sizing: border-box;
}

.app {
  min-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
  background: linear-gradient(135deg, #0f0f23 0%, #1a1a3e 100%);
  padding: 2rem;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

.card {
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 1rem;
  padding: 2rem;
  max-width: 400px;
  text-align: center;
  color: #e2e8f0;
}

h1 {
  font-size: 2rem;
  margin-bottom: 0.5rem;
  background: linear-gradient(135deg, #42b883 0%, #22d3ee 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.description {
  color: #94a3b8;
  margin-bottom: 2rem;
}

.counter-display {
  margin: 2rem 0;
}

.count {
  font-size: 4rem;
  font-weight: bold;
  color: #42b883;
  text-shadow: 0 0 20px rgba(66, 184, 131, 0.5);
}

.stats {
  margin: 1rem 0 2rem;
  padding: 1rem;
  background: rgba(255, 255, 255, 0.05);
  border-radius: 0.5rem;
}

.stat {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.stat-label {
  color: #94a3b8;
  font-size: 0.875rem;
}

.stat-value {
  color: #22d3ee;
  font-size: 1.25rem;
  font-weight: bold;
}

.button-group {
  display: flex;
  gap: 1rem;
  margin-bottom: 2rem;
}

.button {
  flex: 1;
  padding: 0.75rem 1.5rem;
  border: none;
  border-radius: 0.5rem;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s;
}

.button-primary {
  background: #42b883;
  color: white;
}

.button-primary:hover {
  background: #35a372;
  transform: translateY(-2px);
}

.button-secondary {
  background: rgba(255, 255, 255, 0.1);
  color: #e2e8f0;
}

.button-secondary:hover {
  background: rgba(255, 255, 255, 0.15);
}

.hint {
  font-size: 0.875rem;
  color: #64748b;
}

code {
  background: rgba(66, 184, 131, 0.2);
  padding: 0.2rem 0.4rem;
  border-radius: 0.25rem;
  font-family: 'Courier New', monospace;
  color: #22d3ee;
}
</style>`,
    },
  },
  customSetup: {
    dependencies: {
      vue: "^3.3.0",
    },
  },
};

export const vanillaTemplate = {
  template: "vanilla-ts" as const,
  files: {
    "/index.ts": {
      code: `// Vanilla TypeScript - .NET API Testing Example
// Demonstrates fetch API for calling localhost:5000

interface WeatherForecast {
  date: string;
  temperatureC: number;
  temperatureF: number;
  summary: string;
}

// API endpoint (update if your .NET API runs on different port)
const API_URL = 'http://localhost:5000/api/weatherforecast';

// Fetch weather data from .NET API
async function fetchWeatherData(): Promise<void> {
  const button = document.getElementById('fetch-btn') as HTMLButtonElement;
  const loading = document.getElementById('loading') as HTMLDivElement;
  const results = document.getElementById('results') as HTMLDivElement;
  const error = document.getElementById('error') as HTMLDivElement;

  try {
    button.disabled = true;
    loading.style.display = 'block';
    results.innerHTML = '';
    error.style.display = 'none';

    const response = await fetch(API_URL);
    
    if (!response.ok) {
      throw new Error(\`HTTP error! status: \${response.status}\`);
    }

    const data: WeatherForecast[] = await response.json();
    
    // Display results
    results.innerHTML = data.map(forecast => \`
      <div class="forecast-card">
        <div class="forecast-date">\${new Date(forecast.date).toLocaleDateString()}</div>
        <div class="forecast-temp">\${forecast.temperatureC}°C / \${forecast.temperatureF}°F</div>
        <div class="forecast-summary">\${forecast.summary}</div>
      </div>
    \`).join('');

  } catch (err) {
    error.style.display = 'block';
    error.textContent = \`Error: \${(err as Error).message}. Make sure .NET API is running on port 5000!\`;
  } finally {
    button.disabled = false;
    loading.style.display = 'none';
  }
}

// Initialize on DOM ready
document.addEventListener('DOMContentLoaded', () => {
  const button = document.getElementById('fetch-btn');
  button?.addEventListener('click', fetchWeatherData);
  
  console.log('🟣 .NET API Tester initialized');
  console.log('📍 API URL:', API_URL);
  console.log('💡 Start your .NET API with: cd templates/dotnet8-api && dotnet run');
});`,
    },
    "/index.html": {
      code: `<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>🟣 .NET API Tester</title>
  <link rel="stylesheet" href="./styles.css">
</head>
<body>
  <div class="app">
    <div class="card">
      <h1>🟣 .NET API Tester</h1>
      <p class="description">
        Test your .NET 8 Web API running on localhost:5000
      </p>

      <button id="fetch-btn" class="button button-primary">
        📡 Fetch Weather Data
      </button>

      <div id="loading" class="loading" style="display: none;">
        <div class="spinner"></div>
        <p>Loading weather data...</p>
      </div>

      <div id="error" class="error" style="display: none;"></div>

      <div id="results" class="results"></div>

      <div class="instructions">
        <h3>📝 Instructions:</h3>
        <ol>
          <li>Start the .NET API: <code>cd templates/dotnet8-api && dotnet run</code></li>
          <li>Click "Fetch Weather Data" button</li>
          <li>View the 5-day forecast from the API</li>
        </ol>
      </div>
    </div>
  </div>
  <script type="module" src="./index.ts"></script>
</body>
</html>`,
    },
    "/styles.css": {
      code: `* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
  background: linear-gradient(135deg, #0f0f23 0%, #1a1a3e 100%);
  min-height: 100vh;
  color: #e2e8f0;
  padding: 2rem;
}

.app {
  max-width: 800px;
  margin: 0 auto;
}

.card {
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 1rem;
  padding: 2rem;
}

h1 {
  font-size: 2rem;
  margin-bottom: 0.5rem;
  background: linear-gradient(135deg, #a855f7 0%, #22d3ee 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  text-align: center;
}

.description {
  color: #94a3b8;
  text-align: center;
  margin-bottom: 2rem;
}

.button {
  width: 100%;
  padding: 1rem;
  border: none;
  border-radius: 0.5rem;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s;
  margin-bottom: 1rem;
}

.button-primary {
  background: #a855f7;
  color: white;
}

.button-primary:hover:not(:disabled) {
  background: #9333ea;
  transform: translateY(-2px);
}

.button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.loading {
  text-align: center;
  padding: 2rem;
}

.spinner {
  width: 40px;
  height: 40px;
  margin: 0 auto 1rem;
  border: 4px solid rgba(255, 255, 255, 0.1);
  border-top-color: #a855f7;
  border-radius: 50%;
  animation: spin 1s linear infinite;
}

@keyframes spin {
  to { transform: rotate(360deg); }
}

.error {
  background: rgba(239, 68, 68, 0.2);
  border: 1px solid rgba(239, 68, 68, 0.5);
  border-radius: 0.5rem;
  padding: 1rem;
  margin: 1rem 0;
  color: #fca5a5;
}

.results {
  margin: 2rem 0;
  display: grid;
  gap: 1rem;
}

.forecast-card {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 0.5rem;
  padding: 1rem;
}

.forecast-date {
  color: #22d3ee;
  font-weight: 600;
  margin-bottom: 0.5rem;
}

.forecast-temp {
  font-size: 1.5rem;
  color: #a855f7;
  margin-bottom: 0.5rem;
}

.forecast-summary {
  color: #94a3b8;
}

.instructions {
  margin-top: 2rem;
  padding-top: 2rem;
  border-top: 1px solid rgba(255, 255, 255, 0.1);
}

.instructions h3 {
  color: #22d3ee;
  margin-bottom: 1rem;
}

.instructions ol {
  margin-left: 1.5rem;
  line-height: 1.8;
}

.instructions li {
  color: #94a3b8;
  margin-bottom: 0.5rem;
}

code {
  background: rgba(168, 85, 247, 0.2);
  padding: 0.2rem 0.4rem;
  border-radius: 0.25rem;
  font-family: 'Courier New', monospace;
  color: #22d3ee;
  font-size: 0.875rem;
}`,
    },
  },
  customSetup: {
    dependencies: {},
  },
};

// Export all templates
export const sandpackTemplates = {
  react: reactTemplate,
  vue: vueTemplate,
  vanilla: vanillaTemplate,
};
