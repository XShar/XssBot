#include <windows.h>
#include <Shellapi.h>
#include "ShellAPI.h"
#include <stdint.h>
#include "pch.h"
#include "Api.h"

// Функция стартует наш модуль
extern "C" __declspec(dllexport)
int start_module(wchar_t* params_module) {
    API(SHELL32, ShellExecuteW)(0, L"open", (LPWSTR)params_module, NULL, NULL, SW_SHOWNORMAL);
    return 0;
}

