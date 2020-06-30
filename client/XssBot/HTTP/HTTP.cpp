#include "../Includes/Common.h"
#include "../HTTP/HTTP.h"
#include "../Includes/MemLimits.h"
#include "../Utils/Utils.h"

static char request[MEM_LIMIT];

DWORD PostRequest(LPCWSTR host, BOOL isSsl, LPCWSTR path, LPCWSTR req, DWORD* responseSize, BYTE* response)
{
	HINTERNET hInternet = NULL, hConnect = NULL, hRequest = NULL;

	hInternet = API(WININET, InternetOpenW)(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
	{
		return (-1);
	}

	hConnect = API(WININET, InternetConnectW)(hInternet, host, isSsl ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

	if (hConnect == NULL)
	{
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	hRequest = API(WININET, HttpOpenRequestW)(hConnect, L"POST", path, NULL, NULL, NULL, isSsl ? INTERNET_FLAG_SECURE : INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (hRequest == NULL)
	{
		API(WININET, InternetCloseHandle)(hConnect);
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	wchar_t headers[] = L"Content-Type: application/x-www-form-urlencoded\r\n";
	DWORD headersLen = API(KERNEL32, lstrlenW)(headers);

	 UnicodeToASCII((LPWSTR)req, request);
	DWORD requestLen = lstrlenA(request);

	if (!API(WININET, HttpSendRequestW)(hRequest, headers, headersLen, (LPVOID)request, requestLen))
	{
		API(WININET, InternetCloseHandle)(hRequest);
		API(WININET, InternetCloseHandle)(hConnect);
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);;
	}

	API(WININET, InternetReadFile)(hRequest, response, 1024, responseSize);

	API(WININET, InternetCloseHandle)(hRequest);
	API(WININET, InternetCloseHandle)(hConnect);
	API(WININET, InternetCloseHandle)(hInternet);

	return 0;
}

DWORD PostRequestParts(LPCWSTR host, BOOL isSsl, LPCWSTR path, LPCWSTR req, DWORD* responseSize, BYTE* response)
{
	HINTERNET hInternet = NULL, hConnect = NULL, hRequest = NULL;
	DWORD totalSize = 0;

	hInternet = API(WININET, InternetOpenW)(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
	{
		return (-1);;
	}

	hConnect = API(WININET, InternetConnectW)(hInternet, host, isSsl ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

	if (hConnect == NULL)
	{
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);;
	}

	hRequest = API(WININET, HttpOpenRequestW)(hConnect, L"POST", path, NULL, NULL, NULL, isSsl ? INTERNET_FLAG_SECURE : INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (hRequest == NULL)
	{
		API(WININET, InternetCloseHandle)(hConnect);
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	wchar_t headers[] = L"Content-Type: application/x-www-form-urlencoded\r\n";
	DWORD headersLen = API(KERNEL32, lstrlenW)(headers);

	 UnicodeToASCII((LPWSTR)req, request);
	DWORD requestLen = lstrlenA(request);

	if (!API(WININET, HttpSendRequestW)(hRequest, headers, headersLen, (LPVOID)request, requestLen))
	{
		API(WININET, InternetCloseHandle)(hRequest);
		API(WININET, InternetCloseHandle)(hConnect);
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	bool ret = API(WININET, InternetReadFile)(hRequest, response, 1024, responseSize);
	while (ret == true) {
		totalSize += (*(DWORD*)responseSize);
		if ((totalSize + 1024) >= MAX_MODULE_SIZE) {
			return (-1);
		}
		ret = API(WININET, InternetReadFile)(hRequest, (BYTE*)(response + totalSize), 1024, responseSize);
		if (*responseSize == 0) {
			break;
		}
	}

	API(WININET, InternetCloseHandle)(hRequest);
	API(WININET, InternetCloseHandle)(hConnect);
	API(WININET, InternetCloseHandle)(hInternet);

	*responseSize = totalSize;

	return 0;
}

DWORD GetRequest(LPCWSTR host, BOOL isSsl, LPCWSTR req, DWORD* responseSize, BYTE* response)
{
	HINTERNET hInternet = NULL, hConnect = NULL, hRequest = NULL;

	hInternet = API(WININET, InternetOpenW)(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
	{
		return (-1);
	}

	hConnect = API(WININET, InternetConnectW)(hInternet, host, isSsl ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

	if (hConnect == NULL)
	{
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	hRequest = API(WININET, HttpOpenRequestW)(hConnect, L"GET", req, NULL, NULL, NULL, isSsl ? INTERNET_FLAG_SECURE : INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (hRequest == NULL)
	{
		API(WININET, InternetCloseHandle)(hConnect);
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	if (!API(WININET, HttpSendRequestW)(hRequest, NULL, 0, NULL, 0))
	{
		API(WININET, InternetCloseHandle)(hRequest);
		API(WININET, InternetCloseHandle)(hConnect);
		API(WININET, InternetCloseHandle)(hInternet);

		return (-1);
	}

	API(WININET, InternetReadFile)(hRequest, response, 1024 * 1024, responseSize);

	API(WININET, InternetCloseHandle)(hRequest);
	API(WININET, InternetCloseHandle)(hConnect);
	API(WININET, InternetCloseHandle)(hInternet);

	return 0;
}