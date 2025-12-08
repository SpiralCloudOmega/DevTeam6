<script setup lang="ts">
import { RouterLink, RouterView } from 'vue-router'
</script>

<template>
  <div id="app">
    <header>
      <nav>
        <RouterLink to="/">Home</RouterLink>
        <RouterLink to="/about">About</RouterLink>
      </nav>
    </header>

    <main>
      <RouterView />
    </main>
  </div>
</template>

<style scoped>
header {
  padding: 1rem 2rem;
  background: #1a1a1a;
  border-bottom: 1px solid #333;
}

nav {
  display: flex;
  gap: 2rem;
}

nav a {
  color: #42b983;
  text-decoration: none;
  font-weight: 500;
  transition: color 0.3s;
}

nav a:hover {
  color: #35a372;
}

nav a.router-link-active {
  color: #ffffff;
}

main {
  padding: 2rem;
}
</style>
