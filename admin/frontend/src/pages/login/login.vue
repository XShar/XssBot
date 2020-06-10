<template>
 <div v-loading.fullscreen.lock="isLoading">
   <el-card>
      <div slot="header">
        <span> Login | XssBot</span>
      </div>
      <el-row>
        <el-col>
          <el-input v-model="data.username" 
                    prefix-icon="fal fa-user"
                    placeholder="login"
          >
          </el-input>
        </el-col>
      </el-row>
      <el-row>
        <el-col>
          <el-input v-model="data.password" 
                    prefix-icon="fal fa-key"
                    placeholder="password" 
                    show-password
          >
          </el-input>
        </el-col>
      </el-row>
      <el-row style="text-align: center;">
        <el-col>
          <el-button type="primary" @click="login()">login</el-button>
        </el-col>
      </el-row>
    </el-card> 
 </div>
</template>

<script>
import api from '../../api/'

export default {
  data() {
    return {
      isLoading: false,
      data: {
        username: '',
        password: ''
      }
    };
  },
  mounted() {
    
  },
  methods: {
    async login() {
        this.isLoading = true;
        if (await api.login(this.data)) {
            // редиректим на страницу со списком ботов
            this.$router.push('bots');
        }
        this.isLoading = false;
    }
  }
};
</script>

<style scoped>
.el-card {
  max-width: 300px;
  margin: 0 auto;
  margin-top: 20%;
}
.el-row {
  margin-bottom: 20px;
}
</style>