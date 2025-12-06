import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory('/DevTeam6/projects/'),
  routes: [
    {
      path: '/',
      name: 'home',
      component: () => import('@/views/HomeView.vue'),
    },
    {
      path: '/editor',
      name: 'editor',
      component: () => import('@/views/EditorView.vue'),
    },
    {
      path: '/mindmap',
      name: 'mindmap',
      component: () => import('@/views/MindmapView.vue'),
    },
  ],
})

export default router
