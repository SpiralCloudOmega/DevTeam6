import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve } from 'path'
import { createBuildConfig } from '../vite.config.shared'

export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': resolve(__dirname, 'src'),
    },
  },
  ...createBuildConfig({
    base: '/DevTeam6/projects/',
    port: 5174,
  }),
})
