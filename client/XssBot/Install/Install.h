#pragma once
#include "../Includes/Common.h"

class Install
{
private:
	wchar_t* currentPath;
public:
	Install(wchar_t* currentPath)
	{
		this->currentPath = currentPath;
	}
	Install() 
	{
		this->currentPath = nullptr;
	}

	wchar_t* GetMainPath();
	wchar_t* GetInstallPath();
	bool MoveToInstallPath();
	bool AddAutorun();
	void Restart();
};

