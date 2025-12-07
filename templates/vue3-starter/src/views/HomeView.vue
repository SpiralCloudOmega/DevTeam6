<script setup lang="ts">
import HelloWorld from '@/components/HelloWorld.vue'
import { useCounterStore } from '@/stores/counter'

const counter = useCounterStore()
</script>

<template>
  <div class="home">
    <div class="logos">
      <a href="https://vitejs.dev" target="_blank">
        <img src="/vite.svg" class="logo" alt="Vite logo" />
      </a>
      <a href="https://vuejs.org/" target="_blank">
        <img src="@/assets/vue.svg" class="logo vue" alt="Vue logo" />
      </a>
    </div>
    
    <HelloWorld msg="Vue 3 + TypeScript + Vite" />

    <div class="store-example">
      <h2>Pinia Store Example</h2>
      <p>Store count: {{ counter.count }}</p>
      <button @click="counter.increment">Increment Store</button>
    </div>
  </div>
</template>

<style scoped>
.home {
  text-align: center;
}

.logos {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 2rem;
  margin: 2rem 0;
}

.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}

.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}

.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}

.store-example {
  margin-top: 2rem;
  padding: 2rem;
  border: 1px solid var(--color-border);
  border-radius: 8px;
}

button {
  margin-top: 1rem;
}
</style>
