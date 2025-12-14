import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { createBuildConfig } from '../../vite.config.shared'

export default defineConfig({
  plugins: [react()],
  ...createBuildConfig({
    port: 3000,
  }),
})
