#pragma once
#include "../Includes/Common.h"

HANDLE GetHeap();
void* hAlloc(size_t size);
void hFree(void* mem);
bool IsFileExist(wchar_t* path);
wchar_t* GetBotUid();
wchar_t* GetCurrentPath();
bool IsCIS();
size_t ASCIIToUnicode(char* ascii, wchar_t* unicode);
size_t UnicodeToASCII(wchar_t* unicode, char* ascii);
void* memCpy(void* dest, const void* src, size_t bytes);
wchar_t* hide_admin_path();
wchar_t* hide_ping_path();
wchar_t* hide_reg_path();
wchar_t* hide_download_module_path();
wchar_t* hide_error_path();
char* hide_str_task();
char* hide_str_reg();
char* hide_str_self_destroy();
char* hide_str_run_module();
char* hide_str_ping();
char* hide_str_exe();
wchar_t* hide_install_path();
wchar_t* hide_str_to_destroy();