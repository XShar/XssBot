<template>
  <div class="">
    <el-menu default-active="bots" router mode="horizontal" size="small">
      <el-menu-item style="font-size: 24px;"><i class="fal fa-alicorn"> XssBot</i></el-menu-item>
      <el-menu-item index="bots">Bots</el-menu-item>
      <el-menu-item index="tasks">Tasks</el-menu-item>
      <el-menu-item index="modules">Modules</el-menu-item>
      <el-menu-item index="users">Users</el-menu-item>
    </el-menu>
    <router-view></router-view>
  </div>
</template>
<script>
export default {};
</script>
