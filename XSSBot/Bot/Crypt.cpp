#include <Windows.h>
#include "Crypt.h"
#include "Debug_to_file.h"
#include "core/core.h"
#include "core/Common.h"
#include <ctime>
#include "MemLimits.h"

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

//Создание случайной строки
static bool is_start_random = false;
void gen_random(uint8_t* s, const int len) {
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

//RC4 шифрование
#define swap(type,a,b,c) do{type t=a[b];a[b]=a[c];a[c]=t;}while(0)
uint8_t* encRC4(uint8_t* pKey, DWORD dwKeyLen, uint8_t* pData, DWORD dwDataLen)
{
	BYTE s[256];
	DWORD i, j, c;

	for (i = 0; i < 256; s[i] = (BYTE)i++);

	for (i = j = 0; i < 256; ++i) {
		j = (j + s[i] + pKey[i % dwKeyLen]) % 256;
		swap(BYTE, s, i, j);
	}

	i = j = c = 0;

	while (dwDataLen--) {
		j = (j + s[i = (i + 1) % 256]) % 256;
		swap(BYTE, s, i, j);

		pData[c++] ^= s[(s[i] + s[j]) % 256];
	}

	return pData;
}

//RC4 расшифровка
uint8_t* decRC4(uint8_t* pKey, DWORD dwKeyLen, uint8_t* pData, DWORD dwDataLen)
{
	if (dwDataLen > 0)
	{
		DWORD i, j, c;
		int gg;
		BYTE s[256];
		gg = 256;

		for (i = 0; i < gg; s[i] = (BYTE)i++);

		for (i = j = 0; i < gg; ++i)
		{
			j = (j + s[i] + pKey[i % dwKeyLen]) % gg;
			swap(BYTE, s, i, j);
		}

		i = j = c = 0;

		while (dwDataLen--)
		{
			j = (j + s[i = (i + 1) % gg]) % gg;
			swap(BYTE, s, i, j);
			pData[c++] ^= s[(s[i] + s[j]) % gg];
		}

		return pData;
	}

	return NULL;
}

//BASE64 шифровка
static uint8_t base46_map[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
					 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
					 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
					 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };
uint8_t* base64_encode(uint8_t* plain) {

	uint8_t counts = 0;
	uint8_t buffer[3];
	uint8_t* cipher = NULL;

	cipher = (uint8_t*)malloc(MEM_SIZE);
	if (cipher == NULL) {
		return NULL;
	}

	int i = 0, c = 0;
	for (i = 0; plain[i] != '\0'; i++) {

		if (i >= MAX_MODULE_SIZE) {
			free(cipher);
			return NULL;
		}

		buffer[counts++] = plain[i];
		if (counts == 3) {
			cipher[c++] = base46_map[buffer[0] >> 2];
			cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
			cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
			cipher[c++] = base46_map[buffer[2] & 0x3f];
			counts = 0;
		}
	}

	if (counts > 0) {
		cipher[c++] = base46_map[buffer[0] >> 2];
		if (counts == 1) {
			cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
			cipher[c++] = '=';
		}
		else {                      // if counts == 2
			cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
			cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
		}
		cipher[c++] = '=';
	}

	cipher[c] = '\0';   /* string padding character */
	return cipher;
}

//BASE64 расшифровка
uint8_t* base64_decode(uint8_t* cipher) {

	uint8_t counts = 0;
	uint8_t buffer[4];

	uint8_t* plain = NULL;
	plain = (uint8_t*)malloc(MEM_SIZE);
	if (plain == NULL) {
		free(plain);
		return NULL;
	}
	
	int i = 0, p = 0;

	for (i = 0; cipher[i] != '\0'; i++) {
		
		if (i >= MAX_MODULE_SIZE) {
			return NULL;
		}
		
		uint8_t k;
		for (k = 0; k < 64 && base46_map[k] != cipher[i]; k++);
		buffer[counts++] = k;
		if (counts == 4) {
			plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
			if (buffer[2] != 64)
				plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
			if (buffer[3] != 64)
				plain[p++] = (buffer[2] << 6) + buffer[3];
			counts = 0;
		}
	}

	plain[p] = '\0';    /* string padding character */
	return plain;
}