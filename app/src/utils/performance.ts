/**
 * Performance utilities for monitoring and optimizing the application
 */

// FPS Monitor
export class FPSMonitor {
  private frames: number[] = []
  private lastTime = performance.now()
  private readonly sampleSize = 60 // Track last 60 frames
  
  update(): number {
    const currentTime = performance.now()
    const delta = currentTime - this.lastTime
    this.lastTime = currentTime
    
    const fps = 1000 / delta
    this.frames.push(fps)
    
    if (this.frames.length > this.sampleSize) {
      this.frames.shift()
    }
    
    return fps
  }
  
  getAverageFPS(): number {
    if (this.frames.length === 0) return 0
    const sum = this.frames.reduce((a, b) => a + b, 0)
    return sum / this.frames.length
  }
  
  reset() {
    this.frames = []
    this.lastTime = performance.now()
  }
}

// Debounce function for expensive operations
export function debounce<T extends (...args: unknown[]) => void>(
  func: T,
  wait: number
): (...args: Parameters<T>) => void {
  let timeout: ReturnType<typeof setTimeout> | null = null
  
  return function executedFunction(...args: Parameters<T>) {
    const later = () => {
      timeout = null
      func(...args)
    }
    
    if (timeout) clearTimeout(timeout)
    timeout = setTimeout(later, wait)
  }
}

// Throttle function for frequent event handlers
export function throttle<T extends (...args: unknown[]) => void>(
  func: T,
  limit: number
): (...args: Parameters<T>) => void {
  let inThrottle: boolean
  
  return function executedFunction(...args: Parameters<T>) {
    if (!inThrottle) {
      func(...args)
      inThrottle = true
      setTimeout(() => (inThrottle = false), limit)
    }
  }
}

// Memoization for expensive computations
export function memoize<T extends (...args: unknown[]) => unknown>(
  fn: T
): T {
  const cache = new Map<string, unknown>()
  
  return ((...args: unknown[]) => {
    const key = JSON.stringify(args)
    
    if (cache.has(key)) {
      return cache.get(key)
    }
    
    const result = fn(...args)
    cache.set(key, result)
    return result
  }) as T
}

// Request Animation Frame helper for smooth animations
export class RAFQueue {
  private queue: (() => void)[] = []
  private rafId: number | null = null
  
  add(callback: () => void) {
    this.queue.push(callback)
    
    if (!this.rafId) {
      this.rafId = requestAnimationFrame(() => this.flush())
    }
  }
  
  private flush() {
    const callbacks = this.queue.splice(0)
    callbacks.forEach(cb => cb())
    this.rafId = null
  }
  
  clear() {
    if (this.rafId !== null) {
      cancelAnimationFrame(this.rafId)
      this.rafId = null
    }
    this.queue = []
  }
}

// Performance marker for measuring code blocks
export class PerformanceMarker {
  private marks = new Map<string, number>()
  
  start(name: string) {
    this.marks.set(name, performance.now())
  }
  
  end(name: string): number {
    const startTime = this.marks.get(name)
    if (!startTime) {
      console.warn(`Performance marker "${name}" was never started`)
      return 0
    }
    
    const duration = performance.now() - startTime
    this.marks.delete(name)
    return duration
  }
  
  measure(name: string, fn: () => void): number {
    this.start(name)
    fn()
    return this.end(name)
  }
}

// Lazy loading helper
export function lazyLoad<T>(
  factory: () => Promise<T>
): () => Promise<T> {
  let instance: T | null = null
  let promise: Promise<T> | null = null
  
  return () => {
    if (instance) {
      return Promise.resolve(instance)
    }
    
    if (promise) {
      return promise
    }
    
    promise = factory().then(result => {
      instance = result
      promise = null
      return result
    })
    
    return promise
  }
}

// Intersection Observer helper for lazy rendering
export function createIntersectionObserver(
  callback: IntersectionObserverCallback,
  options?: IntersectionObserverInit
): IntersectionObserver {
  return new IntersectionObserver(callback, {
    rootMargin: '50px',
    threshold: 0.01,
    ...options
  })
}

// Memory usage helper (if available)
export function getMemoryUsage(): {
  used: number
  total: number
  limit: number
} | null {
  if ('memory' in performance) {
    const mem = (performance as unknown as { memory: { 
      usedJSHeapSize: number
      totalJSHeapSize: number
      jsHeapSizeLimit: number
    }}).memory
    return {
      used: mem.usedJSHeapSize,
      total: mem.totalJSHeapSize,
      limit: mem.jsHeapSizeLimit
    }
  }
  return null
}
