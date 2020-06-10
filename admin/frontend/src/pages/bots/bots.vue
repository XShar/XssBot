<template>
  <el-row style="margin: 20px;" v-loading="isLoading">
    <el-card class="bots-card" shadow="hover">
        <div slot="header" class="clearfix">
            <span>Bot list</span>
        </div>
        <el-table :data="tableData">
            <el-table-column prop="id" label="ID" width="70"></el-table-column>
            <el-table-column prop="uid" label="UID"></el-table-column>
            <el-table-column label="IP">
                <template slot-scope="props">
                    <span>{{props.row.ip}}</span>
                    <br>
                    <span>{{props.row.country}}</span>
                </template>
            </el-table-column>
            <el-table-column prop="win_ver" label="Win"></el-table-column>
            <el-table-column prop="username" label="User"></el-table-column>
            <el-table-column prop="computername" label="Comp"></el-table-column>
            <el-table-column prop="cpu_name" label="CPU"></el-table-column>
            <el-table-column prop="gpu_name" label="GPU"></el-table-column>
            <el-table-column prop="ram_amount" label="RAM"></el-table-column>
            <el-table-column prop="screen_resolution" label="Display"></el-table-column>
            <el-table-column label="Joined">
                <template slot-scope="props">
                    <span>{{props.row.joined | timeago}}</span>
                </template>
            </el-table-column>
            <el-table-column label="Status">
                <template slot-scope="props">
                    <el-tag v-if="isOnline(props.row.seen)" type="success">Online</el-tag>
                    <el-tag v-else type="danger">Offline</el-tag>
                </template>
            </el-table-column>
        </el-table>
    </el-card>
  </el-row>
</template>

<script>
import api from '../../api/'

export default {
    data() {
        return {
            isLoading: false,
            tableData: [],
        }
    },
    async created() {
        await this.updateData();
    },
    filters: {
        timeago(timeStamp) {
            const diff = Math.round(+new Date()/1000) - timeStamp
            const msPerMinute = 60;
            const msPerHour = msPerMinute * 60;
            const msPerDay = msPerHour * 24;
            const msPerMonth = msPerDay * 30;
            const msPerYear = msPerDay * 365;

            if (diff < msPerMinute) {
                return Math.round(diff) + 's ago';
            }
            else if (diff < msPerHour) {
                return Math.round(diff/msPerMinute) + 'm ago';
            }
            else if (diff < msPerDay) {
                return Math.round(diff/msPerHour) + 'h ago';
            }
            else if (diff < msPerMonth) {
                return Math.round(diff/msPerDay) + 'd ago';
            }
            else if (diff < msPerYear) {
                return Math.round(diff/msPerMonth) + ' months ago';
            }
            else {
                return Math.round(diff/msPerYear) + ' years ago';
            }
        }
    },
    methods: {
        isOnline(timestamp) {
            return (Math.round(+new Date()/1000) - timestamp) < 60
        },
        async updateData() {
            this.isLoading = true;
            this.tableData = await api.getBotsData();
            this.isLoading = false;
        },
    },
}
</script>