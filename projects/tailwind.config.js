/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{vue,js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        // Cyberpunk theme colors
        'cyber-cyan': '#00f0ff',
        'cyber-magenta': '#ff00ff',
        'cyber-green': '#00ff88',
        'cyber-purple': '#7b2fff',
        'cyber-orange': '#ff6600',
        'cyber-yellow': '#ffcc00',
        'cyber-dark': '#0a0a1a',
        'cyber-mid': '#1a0a2e',
        'cyber-light': '#2a1a3e',
      },
      fontFamily: {
        mono: ['JetBrains Mono', 'Fira Code', 'monospace'],
      },
      boxShadow: {
        'cyber': '0 0 20px rgba(0, 240, 255, 0.3)',
        'cyber-magenta': '0 0 20px rgba(255, 0, 255, 0.3)',
        'cyber-green': '0 0 20px rgba(0, 255, 136, 0.3)',
      },
      animation: {
        'pulse-cyan': 'pulse-cyan 2s cubic-bezier(0.4, 0, 0.6, 1) infinite',
        'flow': 'flow 3s linear infinite',
      },
      keyframes: {
        'pulse-cyan': {
          '0%, 100%': { boxShadow: '0 0 10px rgba(0, 240, 255, 0.3)' },
          '50%': { boxShadow: '0 0 30px rgba(0, 240, 255, 0.6)' },
        },
        'flow': {
          '0%': { strokeDashoffset: '100' },
          '100%': { strokeDashoffset: '0' },
        },
      },
    },
  },
  plugins: [],
}
