#include <Windows.h>
#include "StringHelpers.h"

size_t strLenA(const char* str)
{
    size_t len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    return len;
}

char* strCpyA(char* dest, const char* src)
{
    char* wcp = dest - 1;
    wint_t c;
    const ptrdiff_t off = src - dest + 1;
    do
    {
        c = wcp[off];
        *++wcp = c;
    } while (c != '\0');

    return wcp;
}

int strCmpA(char* s1, char* s2)
{
    char c1, c2;
    do
    {
        c1 = *s1++;
        c2 = *s2++;
        if (c2 == '\0')
            return c1 - c2;
    } while (c1 == c2);

    return c1 < c2 ? -1 : 1;
}

char* strCatA(char* dest, const char* src)
{
    char* s1 = dest;
    const char* s2 = src;
    char c;
    do
        c = *s1++;
    while (c != '\0');

    s1 -= 2;

    do
    {
        c = *s2++;
        *++s1 = c;
    } while (c != '\0');

    return dest;
}

static char* mystrchr(const char* s, const int c) {
    return (*s) ? ((*s == c) ? (char*)s : mystrchr(s + 1, c)) : NULL;
}

char* strTokA(char* str, const char* delim) {
    static char* ptr;

    if (str)
        for (ptr = str; mystrchr(delim, *ptr); ++ptr)
            ;

    if (*ptr == '\0')
        return nullptr;

    str = ptr;
    while (*ptr && !mystrchr(delim, *ptr))
        ++ptr;
    while (*ptr && mystrchr(delim, *ptr))
        *ptr++ = '\0';

    return str;
}

char* itoaW(unsigned long i, char b[])
{
    char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0' };
    char* p = b;
    if (i < 0)
    {
        *p++ = '-';
        i *= -1;
    }

    int shifter = i;
    do
    {
        ++p;
        shifter = shifter / 10;
    } while (shifter);

    *p = '\0';

    do
    {
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

wchar_t* strCpyW(wchar_t* dest, const wchar_t* src)
{
    wchar_t* wcp = dest - 1;
    wint_t c;
    const ptrdiff_t off = src - dest + 1;
    do
    {
        c = wcp[off];
        *++wcp = c;
    } while (c != L'\0');

    return wcp;
}