import Vue from 'vue'

import VueRouter from 'vue-router';
Vue.use(VueRouter);

import Components from './components'
Vue.use(Components);

import App from './App.vue'
import api from './api/'

const router = new VueRouter({
  routes:
  [
    {
      path: '/login',
      name: 'Login',
      meta: {layout: () => import('./pages/login/layout.vue')},
      component: () => import('./pages/login/login.vue')
    },
    {
      path: '/',
      component: () => import('./pages/layout.vue'),
      redirect: '/bots',
      children: 
      [
        {
          path: 'bots',
          name: 'Bots',
          meta: {auth: true},
          component: () => import('./pages/bots/bots.vue'),
        },
        {
          path: 'tasks',
          name: 'Tasks',
          meta: {auth: true},
          component: () => import('./pages/tasks/tasks.vue'),
        },
        {
          path: 'modules',
          name: 'Modules',
          meta: {auth: true},
          component: () => import('./pages/modules/modules.vue'),
        },
        {
          path: 'users',
          name: 'Users',
          meta: {auth: true},
          component: () => import('./pages/users/users.vue'),
        },
      ]
    },
    { path: '*' }
  ],
});

router.beforeEach(async function (to, from, next) {
  const currentUser = await api.getCurrentUser();
  const requireAuth = to.matched.some(record => record.meta.auth);
  if (requireAuth && !currentUser) {
    next('/login');
  } else {
    next();
  }
});

new Vue({
  el: '#app',
  render: h => h(App),
  router
})
