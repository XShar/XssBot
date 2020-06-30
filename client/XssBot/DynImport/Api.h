#pragma once
#include "../Includes/Common.h"

LPVOID GetWinAPI(int dll, LPCSTR funcName);

#define KERNEL32 1
#define USER32 2
#define SHELL32 3
#define OLE32 4
#define WININET 5
#define ADVAPI32 6

#define API(DLL, FUNC) ((T_##FUNC)GetWinAPI(DLL, StaticData::##FUNC))

typedef HMODULE(WINAPI* T_LoadLibraryA)(PCHAR);
typedef FARPROC(WINAPI* T_GetProcAddress)(HMODULE, LPCSTR);
typedef DWORD(WINAPI* T_GetFileAttributesW)(LPCWSTR);
typedef UINT(WINAPI* T_GetWindowsDirectoryW)(LPCWSTR, UINT);
typedef BOOL(WINAPI* T_GetVolumeInformationW)(LPCWSTR, LPCWSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPCWSTR, DWORD);
typedef int(WINAPI* T_wsprintfW)(LPCWSTR, LPCWSTR, ...);
typedef LPVOID (WINAPI* T_HeapAlloc)(HANDLE,DWORD,SIZE_T);
typedef BOOL (WINAPI* T_HeapFree)(HANDLE, DWORD, LPVOID);
typedef HRESULT(WINAPI* T_SHGetFolderPathW)(HWND, int, HANDLE, DWORD, LPCWSTR);
typedef LPSTR(WINAPI* T_lstrcatW)(LPWSTR, LPCWSTR);
typedef BOOL(WINAPI* T_CreateDirectoryW)(LPCWSTR, LPSECURITY_ATTRIBUTES);
typedef HANDLE(WINAPI* T_CreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef DWORD(WINAPI* T_GetFileSize)(HANDLE, LPDWORD);
typedef BOOL(WINAPI* T_CloseHandle)(HANDLE);
typedef BOOL(WINAPI* T_ReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef BOOL(WINAPI* T_WriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef DWORD(WINAPI* T_GetModuleFileNameW)(HMODULE, LPWSTR, DWORD);
typedef HRESULT(WINAPI* T_CoInitialize)(LPVOID);
typedef HRESULT(WINAPI* T_CoCreateInstance)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);
typedef void (WINAPI* T_CoUninitialize)();
typedef HINSTANCE (WINAPI* T_ShellExecuteW)(HWND, LPCWSTR, LPWSTR, LPWSTR, LPWSTR, INT);
typedef HINSTANCE (WINAPI* T_ShellExecuteA)(HWND, LPCSTR, LPCSTR, LPCSTR, LPCSTR, INT);
typedef VOID(WINAPI* T_ExitProcess)(UINT);
typedef HANDLE(WINAPI* T_CreateMutexW)(LPSECURITY_ATTRIBUTES, BOOL, LPCWSTR);
typedef DWORD(WINAPI* T_GetLastError)();
typedef LANGID(WINAPI* T_GetUserDefaultLangID)();
typedef int (WINAPI* T_lstrcmpW)(LPCWSTR, LPCWSTR);
typedef BOOL (WINAPI* T_ReleaseMutex)(HANDLE);
typedef int (WINAPI* T_lstrlenW)(LPCWSTR);
typedef int (WINAPI* T_lstrlenA)(LPCSTR);
typedef int (WINAPI* T_MultiByteToWideChar)(UINT, DWORD,LPCCH, int, LPWSTR, int);
typedef int (WINAPI* T_WideCharToMultiByte)(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL);
typedef HINTERNET(WINAPI* T_InternetOpenW)(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD);
typedef HINTERNET(WINAPI* T_InternetConnectW)(HINTERNET, LPCWSTR, INTERNET_PORT, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD_PTR);
typedef HINTERNET(WINAPI* T_HttpOpenRequestW)(HINTERNET, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR*, DWORD, DWORD_PTR);
typedef BOOL(WINAPI* T_InternetCloseHandle)(HINTERNET);
typedef BOOL(WINAPI* T_HttpSendRequestW)(HINTERNET, LPCWSTR, DWORD, LPVOID, DWORD);
typedef BOOL(WINAPI* T_InternetReadFile)(HINTERNET, LPVOID, DWORD, LPDWORD);
typedef BOOL (WINAPI* T_IsWow64Process)(HANDLE, PBOOL);
typedef BOOL(WINAPI* T_GetUserNameW)(PWCHAR , LPDWORD);
typedef BOOL(WINAPI* T_GetComputerNameW)(PWCHAR, LPDWORD);
typedef BOOL(WINAPI* T_EnumDisplayDevicesW)(LPCWSTR, DWORD, PDISPLAY_DEVICEW, DWORD);
typedef BOOL(WINAPI* T_GlobalMemoryStatusEx)(LPMEMORYSTATUSEX);
typedef HWND(WINAPI* T_GetDesktopWindow)();
typedef BOOL(WINAPI* T_GetWindowRect)(HWND, LPRECT);
typedef HWND(WINAPI* T_GetForegroundWindow)();
typedef int (WINAPI* T_GetWindowTextW)(HWND, LPWSTR, int);
typedef DWORD (WINAPI* T_GetTickCount)();
typedef VOID (WINAPI* T_Sleep) (DWORD);
typedef HMODULE(WINAPI* T_GetModuleHandleA) (LPCSTR);
typedef LPVOID(WINAPI* T_VirtualAlloc) (LPVOID, SIZE_T, DWORD, DWORD);
typedef BOOL(WINAPI* T_VirtualFree) (LPVOID, SIZE_T, DWORD);
typedef HANDLE(WINAPI* T_CreateThread) (LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);

#define hashLoadLibraryA 0x0aadf0f1
#define hashGetProcAddress 0x0b3c1d03