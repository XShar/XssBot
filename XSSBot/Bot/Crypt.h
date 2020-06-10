#pragma once
#include <stdint.h>
void gen_random(uint8_t* s, const int len);
uint8_t* encRC4(uint8_t* pKey, DWORD dwKeyLen, uint8_t* pData, DWORD dwDataLen);
uint8_t* decRC4(uint8_t* pKey, DWORD dwKeyLen, uint8_t* pData, DWORD dwDataLen);
uint8_t* base64_encode(uint8_t* plain);
uint8_t* base64_decode(uint8_t* cipher);
