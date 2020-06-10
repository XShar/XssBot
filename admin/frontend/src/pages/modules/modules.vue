<template>
  <el-row style="margin: 20px;" v-loading="isLoading">
    <el-col style="width: 65%">
        <el-card class="bots-card" shadow="hover">
            <div slot="header" class="clearfix">
                <span>Modules</span>
            </div>
            <el-table :data="tableData">
                <el-table-column prop="module_name" label="Module name"></el-table-column>
                <el-table-column prop="size" label="Size"></el-table-column>
                <el-table-column label="Actions">
                    <template slot-scope="props">
                        <el-button type="danger" icon="fal fa-trash" circle size="mini" @click="delModule(props.row.module_name)"></el-button>
                    </template>
                </el-table-column>
            </el-table>
        </el-card>
    </el-col>
    <el-col style="width: 33%; margin-left: 20px;">
        <el-card class="bots-card" shadow="hover">
            <div slot="header" class="clearfix">
                <span>Upload module</span>
            </div>
            <el-upload  drag
                        accept=".dll"
                        action=""
                        :limit="1"
                        :on-change="setPath"
                        :auto-upload = "false">
                <i class="fad fa-cloud-upload" style="font-size: 67px; color: #c0c4cc; margin: 40px 0 16px; line-height: 50px;"></i>
                <div class="el-upload__text">Drop module here or <em>click to upload</em></div>
                <div slot="tip" class="el-upload__tip">Can only upload .dll files</div>
            </el-upload>
            <el-button type="primary" style="margin-top: 20px;" @click="uploadModule">Upload</el-button>
        </el-card>
    </el-col>
  </el-row>
</template>

<script>
import api from '../../api/'

export default {
    data() {
        return {
            isLoading: false,
            tableData: [],
            modulePath: '',
        }
    },
    async created() {
        await this.updateData();
    },
    methods: {
        async delModule(module_name) {
            this.isLoading = true;
            await api.delModule(module_name);
            await this.updateData();
            this.isLoading = false;
        },
        async updateData() {
            this.isLoading = true;
            this.tableData = await api.getModulesData();
            this.isLoading = false;
        },
        setPath (file, fileList) {
            this.modulePath = fileList;
        },
        async uploadModule() {
            this.isLoading = true;
            await api.uploadModule(this.modulePath);
            await this.updateData();
            this.isLoading = false;
        },
    },
}
</script>