import { UserConfigExport } from 'vite'

/**
 * Shared Vite configuration options for DevTeam6 projects
 */
export interface SharedViteConfig {
  /** Base path for deployment */
  base?: string
  /** Server port */
  port?: number
  /** Whether to auto-open browser */
  open?: boolean
  /** Output directory */
  outDir?: string
  /** Enable source maps */
  sourcemap?: boolean
}

/**
 * Create a common Vite build configuration
 */
export function createBuildConfig(options: SharedViteConfig = {}): UserConfigExport {
  return {
    base: options.base || '/',
    build: {
      outDir: options.outDir || 'dist',
      sourcemap: options.sourcemap !== false,
    },
    server: {
      port: options.port || 3000,
      host: true,
      ...(options.open !== undefined && { open: options.open }),
    },
  }
}
