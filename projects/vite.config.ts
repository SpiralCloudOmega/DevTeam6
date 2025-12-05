import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve } from 'path'

export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': resolve(__dirname, 'src'),
    },
  },
  base: '/DevTeam6/projects/',
  server: {
    port: 5174,
  },
  build: {
    outDir: 'dist',
    sourcemap: true,
  },
})
