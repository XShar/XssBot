#include "Common.h"

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
