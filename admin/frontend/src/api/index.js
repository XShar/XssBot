import axios from 'axios';
import cookies from 'vue-cookie'

// т.к наш бэкэнд запущен на другом порту - укажем адрес. 
const backendAddr = 'http://localhost';
// когда соберём в продакшен билде - уберём

export default {
    getUserCookie() {
        return cookies.get('user')
    },
    async login(data) {
        var status = false;
        // отправляем POST-запрос на сервер
        await axios({
            method: 'POST',
            url: backendAddr+'/login/',
            data,
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            if (response.data['cookie']) {
                // кука, которую нужно установить
                const cookie = response.data['cookie'];

                // домен, для которого установить эту куку
                const domain = response.data['domain'];

                // устанавливаем куки
                cookies.set('user', cookie, {expires: '12h', domain: domain});
                status = true;
            }
        })
        .catch(error => {
            console.error(error);
        });
        return status;
    },
    async getCurrentUser() {
        var currentUser = null;
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie()},
            url: backendAddr+'/getCurrentUser/',
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            currentUser = response.data['username'];
        })
        .catch(error => {
            console.error(error);
        });
        return currentUser;
    },
    async getBotsData() {
        var data = null;
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie()},
            url: backendAddr+'/getBotsData/',
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            data = response.data;
        })
        .catch(error => {
            console.error(error);
        });
        return data;
    },
    async getTasksData() {
        var data = null;
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie()},
            url: backendAddr+'/getTasksData/',
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            data = response.data;
        })
        .catch(error => {
            console.error(error);
        });
        return data;
    },
    async taskCreate(form) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), form},
            url: backendAddr+'/taskCreate/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async taskStart(form) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), form},
            url: backendAddr+'/taskStart/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async taskEdit(form) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), form},
            url: backendAddr+'/taskEdit/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async taskDelete(form) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), form},
            url: backendAddr+'/taskDelete/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async getModulesData() {
        var data = null;
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie()},
            url: backendAddr+'/getModulesData/',
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            data = response.data;
        })
        .catch(error => {
            console.error(error);
        });
        return data;
    },
    async uploadModule(modulePath) {
        let formData = new FormData();
        formData.append('cookie', this.getUserCookie());
        formData.append('file', modulePath[0] ? modulePath[0].raw : '');

        await axios({
            method: 'POST',
            data: formData,
            url: backendAddr+'/uploadModule/',
            headers: {'Content-Type': 'multipart/form-data'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async delModule(module_name) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), 'module_name': module_name},
            url: backendAddr+'/delModule/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async getTaskErrors(id) {
        var data = null;
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), 'task_id': id},
            url: backendAddr+'/getTaskErrors/',
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            data = response.data;
        })
        .catch(error => {
            console.error(error);
        });
        return data;
    },
    async getUsersData() {
        var data = null;
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie()},
            url: backendAddr+'/getUsersData/',
            headers: {'Content-Type': 'application/json'}
        }).then(response => {
            data = response.data;
        })
        .catch(error => {
            console.error(error);
        });
        return data;
    },
    async updateUser(form) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), form},
            url: backendAddr+'/updateUser/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async createUser(form) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), form},
            url: backendAddr+'/createUser/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
    async delUser(username) {
        await axios({
            method: 'POST',
            data: {'cookie': this.getUserCookie(), username},
            url: backendAddr+'/delUser/',
            headers: {'Content-Type': 'application/json'}
        })
        .catch(error => {
            console.error(error);
        });
    },
}