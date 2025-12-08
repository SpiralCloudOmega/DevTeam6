<script setup lang="ts">
defineProps<{
  msg: string
}>()
</script>

<template>
  <div class="greetings">
    <h1 class="green">{{ msg }}</h1>
    <p>
      You've successfully created a project with
      <a href="https://vitejs.dev/" target="_blank" rel="noopener">Vite</a> +
      <a href="https://vuejs.org/" target="_blank" rel="noopener">Vue 3</a> +
      <a href="https://www.typescriptlang.org/" target="_blank" rel="noopener">TypeScript</a>
    </p>
    <p>
      Recommended IDE setup:
      <a href="https://code.visualstudio.com/" target="_blank" rel="noopener">VS Code</a>
      +
      <a href="https://marketplace.visualstudio.com/items?itemName=Vue.volar" target="_blank" rel="noopener">Volar</a>
    </p>
  </div>
</template>

<style scoped>
h1 {
  font-weight: 500;
  font-size: 2.6rem;
}

.greetings h1,
.greetings p {
  text-align: center;
}

.green {
  color: #42b983;
}

a {
  color: #42b983;
  text-decoration: none;
  font-weight: 500;
}

a:hover {
  color: #35a372;
}

@media (min-width: 1024px) {
  .greetings h1,
  .greetings p {
    text-align: left;
  }
}
</style>
