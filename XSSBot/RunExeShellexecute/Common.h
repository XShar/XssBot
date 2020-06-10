#pragma once
#define SECURITY_WIN32
#include <Windows.h>
#include <Security.h>
#include <Shlobj.h>
#include <TlHelp32.h>
#include <Wininet.h>
#include <intrin.h>
#include "ntddk.h"
#include <wbemidl.h>
#include <comutil.h>

#define USER32 0
#define KERNEL32 1
#define SHLWAPI 2
#define SHELL32 3
#define WININET 4
#define MSVCRT 5
#define NTDLL 6
#define OLE32 7
#define ADVAPI32 8
//#define API(DLL, FUNC) ((T_##FUNC)GetWinAPI(DLL, hash##FUNC))
#define API(DLL, FUNC) ((T_##FUNC)GetWinAPI(DLL, StaticData::##FUNC))

LPVOID GetApiAddr(HMODULE hModule, UINT uHash);
LPVOID GetWinAPI(int dll, LPCSTR funcName);
int strCmp(const char* src, const char* dst);
size_t strLen(LPCSTR str);
char* strCat(char* dest, const char* src);
char* strCpy(char* dest, const char* src);
unsigned int CalcHash(const byte* ptr, int c_ptr);


/*#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")*/