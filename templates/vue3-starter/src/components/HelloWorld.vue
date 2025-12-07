<script setup lang="ts">
import { ref } from 'vue'

defineProps<{
  msg: string
}>()

const count = ref(0)
</script>

<template>
  <div class="card">
    <h1>{{ msg }}</h1>

    <div class="counter">
      <button type="button" @click="count++">count is {{ count }}</button>
      <p>
        Edit
        <code>components/HelloWorld.vue</code> to test HMR
      </p>
    </div>

    <p class="tip">
      Check out
      <a href="https://vuejs.org/guide/quick-start.html" target="_blank">
        create-vue
      </a>, the official Vue + Vite starter
    </p>

    <p class="tip">
      Click on the Vite and Vue logos to learn more
    </p>
  </div>
</template>

<style scoped>
.card {
  padding: 2em;
}

.counter {
  margin: 2em 0;
}

button {
  border-radius: 8px;
  border: 1px solid transparent;
  padding: 0.6em 1.2em;
  font-size: 1em;
  font-weight: 500;
  font-family: inherit;
  background-color: #1a1a1a;
  cursor: pointer;
  transition: border-color 0.25s;
}

button:hover {
  border-color: #646cff;
}

button:focus,
button:focus-visible {
  outline: 4px auto -webkit-focus-ring-color;
}

.tip {
  margin-top: 1.5em;
  color: #888;
}

code {
  background-color: var(--color-background-mute);
  padding: 0.2em 0.4em;
  border-radius: 4px;
}

@media (prefers-color-scheme: light) {
  button {
    background-color: #f9f9f9;
  }
}
</style>
