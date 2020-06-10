#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_INFO_SIZE 1*1024
static char debug_info[DEBUG_INFO_SIZE];

#define NAME_LOG L"debug.txt"

static void clear_log(void) {
	DeleteFile(NAME_LOG);
}

static void debug_to_file(...) {
	DWORD result = 0;
	//Копирование строки в буфер
	unsigned int size_string = strlen(debug_info) + 1;

	//Запись строки в файл, если файл уже создан, то будет дописываться в конец файла новые строки.
	HANDLE hFile = CreateFile(NAME_LOG, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, debug_info, size_string, &result, NULL);

	//Закрытие файла
	CloseHandle(hFile);
}

#define DEBUG_TO_FILE(...) snprintf(debug_info, DEBUG_INFO_SIZE - 2, __VA_ARGS__);   debug_to_file(__VA_ARGS__);
