import lang from 'element-ui/lib/locale/lang/en';
import locale from 'element-ui/lib/locale';
locale.use(lang);

import { 
    Loading,
    Row, 
    Col,
    Card,
    Input, 
    Button,
    Menu,
    MenuItem,
    Submenu,
    Table,
    TableColumn,
    Tag,
    Dialog,
    Form,
    FormItem,
    Select,
    Option,
    InputNumber,
    Upload,
    Avatar,
} from 'element-ui';

import 'element-ui/lib/theme-chalk/index.css'
import './assets/font-awesome-5.13.0/css/fontawesome-all.min.css'

export default {
    install(Vue) {
        Vue.use(Loading.directive);
        Vue.component(Row.name, Row);
        Vue.component(Col.name, Col);
        Vue.component(Card.name, Card);
        Vue.component(Input.name, Input);
        Vue.component(Button.name, Button);
        Vue.component(Menu.name, Menu);
        Vue.component(MenuItem.name, MenuItem);
        Vue.component(Submenu.name, Submenu);
        Vue.component(Table.name, Table);
        Vue.component(TableColumn.name, TableColumn);
        Vue.component(Tag.name, Tag);
        Vue.component(Dialog.name, Dialog);
        Vue.component(Form.name, Form);
        Vue.component(FormItem.name, FormItem);
        Vue.component(Select.name, Select);
        Vue.component(Option.name, Option);
        Vue.component(InputNumber.name, InputNumber);
        Vue.component(Upload.name, Upload);
        Vue.component(Avatar.name, Avatar);
    }
};