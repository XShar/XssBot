#pragma once
#include "Common.h"

DWORD PostRequest(LPCWSTR host, BOOL isSsl, LPCWSTR path, LPCWSTR req, DWORD* responseSize, BYTE* response);
DWORD PostRequestParts(LPCWSTR host, BOOL isSsl, LPCWSTR path, LPCWSTR req, DWORD* responseSize, BYTE* response);
DWORD GetRequest(LPCWSTR host, BOOL isSsl, LPCWSTR req, DWORD* responseSize, BYTE* response);