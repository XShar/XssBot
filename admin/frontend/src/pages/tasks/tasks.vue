<template>
  <el-row style="margin: 20px;" v-loading="isLoading">
    <el-card class="bots-card" shadow="hover">
        <div slot="header" class="clearfix">
            <span>Task list</span>
            <el-button icon="fal fa-plus-circle" style="float: right; margin-top: -6px;" circle size="small" type="primary" @click="showModal = true"></el-button>
            <el-dialog title="Create task" :visible.sync="showModal" width="325px">
                <el-form :model="form" style="margin-top: -20px;" label-width="100px" label-position="left">
                    <el-form-item label="Select type">
                        <el-select v-model="form.type" placeholder="Type" size="small">
                            <el-option label="run_module" value="run_module"></el-option>
                            <el-option label="self_destroy" value="self_destroy"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Module" v-if="form.type == 'run_module'">
                        <el-select v-model="form.module_name" placeholder="Module" size="small">
                            <el-option v-for="aviable_module in aviable_modules" :key="aviable_module.module_name" :label="aviable_module.module_name" :value="aviable_module.module_name"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="Param" size="small">
                        <el-input v-model="form.param"></el-input>
                    </el-form-item>
                    <el-form-item label="UIDs" size="small">
                        <el-input v-model="form.uids"></el-input>
                    </el-form-item>
                    <el-form-item label="Runs limit" size="small">
                        <el-input-number v-model="form.limit" :min="1"></el-input-number>
                    </el-form-item>
                </el-form>
                <span slot="footer" class="dialog-footer">
                    <el-button size="small" @click="showModal = false">Cancel</el-button>
                    <el-button size="small" type="primary" @click="modalSubmit()">Submit</el-button>
                </span>
            </el-dialog>
        </div>
        <el-table :data="tableData">
            <el-table-column prop="id" label="ID" width="70"></el-table-column>
            <el-table-column prop="type" label="Type"></el-table-column>
            <el-table-column prop="module_name" label="Module"></el-table-column>
            <el-table-column prop="param" label="Param"></el-table-column>
            <el-table-column prop="uids" label="UIDs"></el-table-column>
            <el-table-column prop="limit" label="Limit"></el-table-column>
            <el-table-column prop="loads" label="Loads"></el-table-column>
            <el-table-column prop="runs" label="Runs"></el-table-column>
            <el-table-column label="Errors">
                <template slot-scope="props">
                    <el-button v-if="props.row.errors" type="danger" icon="far fa-times" size="mini" plain @click="showErrorsModal(props.row.id)"> {{props.row.errors}}</el-button>
                    <span v-else>{{props.row.errors}}</span>
                </template>
            </el-table-column>
            <el-table-column label="Status">
                <template slot-scope="props">
                    <el-tag v-if="props.row.is_enabled" type="success">Enabled</el-tag>
                    <el-tag v-else type="danger">Disabled</el-tag>
                </template>
            </el-table-column>
            <el-table-column label="Actions">
                <template slot-scope="props">
                    <el-button icon="fal fa-step-backward" circle size="mini" @click="startTask(props.row)"></el-button>
                    <el-button icon="fal fa-edit" circle size="mini" @click="editTask(props.row)"></el-button>
                    <el-button type="danger" icon="fal fa-trash" circle size="mini" @click="delTask(props.row.id)"></el-button>
                </template>
            </el-table-column>
        </el-table>
        <el-dialog title="Task errors" :visible.sync="showErrors">
            <el-table :data="task_errors">
                <el-table-column prop="task_id" label="TaskId"></el-table-column>
                <el-table-column prop="bot_uid" label="Bot UID"></el-table-column>
                <el-table-column prop="error" label="Error"></el-table-column>
                <el-table-column prop="getlasterror_code" label="GetLastError()"></el-table-column>
            </el-table>
        </el-dialog>
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
            showModal: false,
            isModalCreate: true,
            form: {
                id: 0,
                type: '',
                module_name: '',
                param: '',
                uids: '',
                limit: 1
            },
            aviable_modules: [],
            task_errors: [],
            showErrors: false,
        }
    },
    async created() {
        await this.updateData();
        this.aviable_modules = await api.getModulesData();
    },
    methods: {
        async startTask(row) {
            this.form.id = row.id;
            await api.taskStart(this.form);
            await this.updateData();
        },
        editTask(row) {
            this.form.id = row.id;
            this.form.type = row.type;
            this.form.param = row.param;
            this.form.uids = row.uids;
            this.form.limit = row.limit;
            this.isModalCreate = false;
            this.showModal = true;
        },
        async delTask(id) {
            this.form.id = id;
            await api.taskDelete(this.form);
            await this.updateData();
        },
        async updateData() {
            this.isLoading = true;
            this.tableData = await api.getTasksData();
            this.isLoading = false;
        },
        async modalSubmit() {
            if (this.isModalCreate) {
                await api.taskCreate(this.form);
                this.showModal = false;
            } else {
                await api.taskEdit(this.form);
                this.showModal = false;
            }
            
            await this.updateData();
        },
        async showErrorsModal(id) {
            this.task_errors = await api.getTaskErrors(id)
            this.showErrors = true;
        },
    },
}
</script>