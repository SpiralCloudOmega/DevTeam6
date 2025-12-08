<script setup lang="ts">
import HelloWorld from '../components/HelloWorld.vue'
import { useCounterStore } from '../stores/counter'

const counter = useCounterStore()
</script>

<template>
  <div class="home">
    <div class="logo">
      <img src="../assets/vue.svg" alt="Vue logo" />
    </div>
    
    <HelloWorld msg="Vue 3 + TypeScript + Vite" />
    
    <div class="counter-demo">
      <h2>Pinia Store Demo</h2>
      <p>Count: {{ counter.count }}</p>
      <button @click="counter.increment">Increment</button>
      <button @click="counter.decrement">Decrement</button>
    </div>
  </div>
</template>

<style scoped>
.home {
  text-align: center;
  padding: 2rem;
}

.logo img {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}

.logo img:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}

.counter-demo {
  margin-top: 2rem;
  padding: 2rem;
  background: #1a1a1a;
  border-radius: 8px;
}

.counter-demo button {
  margin: 0 0.5rem;
}
</style>
