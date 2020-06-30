#pragma once

char* itoaA(unsigned long i, char b[]);
char* strTokA(char* s, const char* sep);
size_t strcspnA(const char* s, const char* c);
size_t strspnA(const char* s, const char* c);
char* strchrnulA(const char* s, int c);
char* strCatA(char* dest, const char* src);
int strCmpA(char* s1, char* s2);
char* strCpyA(char* dest, const char* src);
size_t strLenA(const char* str);
wchar_t* strCpyW(wchar_t* dest, const wchar_t* src);