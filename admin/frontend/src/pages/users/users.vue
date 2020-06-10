<template>
  <el-row style="margin: 20px;">
    <el-col style="width: 31%; margin-left: 20px;">
        <el-card class="bots-card" shadow="hover">
            <div slot="header" class="clearfix">
                <span>Edit current user</span>
            </div>
            <div style="text-align: center;">
                <el-avatar size="large" icon="fal fa-user"></el-avatar>
            </div>
            <div style="text-align: center; margin-top: 10px;">
                <span>root</span>
            </div>
            <el-form :model="updateUser" style="margin-top: -15px;" label-width="100px" label-position="left">
                <el-form-item label="Username" style="margin-top: 30px;" size="small">
                    <el-input prefix-icon="fal fa-user" clearable v-model="updateUser.username"></el-input>
                </el-form-item>
                <el-form-item label="Password" style="margin-top: 20px;" size="small">
                    <el-input prefix-icon="fal fa-key" show-password v-model="updateUser.password"></el-input>
                </el-form-item>
                <el-button type="primary" style="margin-top: 20px;" size="small" @click="updateUserData">Update</el-button>
                <el-button type="danger" style="margin-top: 20px;" size="small" @click="logout">Logout</el-button>
            </el-form>
        </el-card>
    </el-col>
    <el-col style="width: 31%; margin-left: 20px;">
        <el-card class="bots-card" shadow="hover">
            <div slot="header" class="clearfix">
                <span>Create user</span>
            </div>
            <el-form :model="createUser" label-width="100px" label-position="left">
                <el-form-item label="Username" style="margin-top: 30px;" size="small">
                    <el-input prefix-icon="fal fa-user" clearable v-model="createUser.username"></el-input>
                </el-form-item>
                <el-form-item label="Password" style="margin-top: 20px;" size="small">
                    <el-input prefix-icon="fal fa-key" show-password v-model="createUser.password"></el-input>
                </el-form-item>
                <el-button type="primary" style="margin-top: 20px;" size="small" @click="createNewUser">Create</el-button>
            </el-form>
        </el-card>
    </el-col>
    <el-col style="width: 31%; margin-left: 20px;">
        <el-card class="bots-card" shadow="hover">
            <div slot="header" class="clearfix">
                <span>Users</span>
            </div>
            <el-table :data="tableData">
                <el-table-column prop="username" label="Username"></el-table-column>
                <el-table-column label="Actions">
                    <template slot-scope="props">
                        <el-button type="danger" icon="fal fa-trash" circle size="mini" @click="delUser(props.row.username)"></el-button>
                    </template>
                </el-table-column>
            </el-table>
        </el-card>
    </el-col>
  </el-row>
</template>

<script>
import api from '../../api/'
import cookies from 'vue-cookie'

export default {
    data() {
        return {
            current_user: '',
            updateUser: {
                username: '',
                password: '',
            },
            createUser: {
                username: '',
                password: '',
            },
            tableData: [],
        }
    },
    async created() {
        await this.updateData();
    },
    methods: {
        async updateData() {
            this.tableData = await api.getUsersData();
        },
        async updateUserData() {
            await api.updateUser(this.updateUser)
            await this.updateData();
        },
        async createNewUser() {
            await api.createUser(this.createUser)
            await this.updateData();
        },
        logout() {
            cookies.delete('user');
            this.$router.push('login');
        },
        async delUser(username) {
            await api.delUser(username)
            await this.updateData();
        }
    },
}
</script>