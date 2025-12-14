import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { createBuildConfig } from '../vite.config.shared'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
  ...createBuildConfig({
    base: '/DevTeam6/',
    port: 3000,
    open: true,
  }),
})
