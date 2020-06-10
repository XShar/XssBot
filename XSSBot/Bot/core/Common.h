#pragma once
#define SECURITY_WIN32
#include <Windows.h>
#include <Security.h>
#include <Shlobj.h>
#include <TlHelp32.h>
#include <Wininet.h>
#include <intrin.h>
#include "ntddk.h"
#include <wbemidl.h>
#include <comutil.h>

#include "core.h"
#include "Api.h"
#include "xor.h"
#include "StaticData.h"
