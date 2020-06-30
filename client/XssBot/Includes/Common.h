#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Wininet.h>
#include <Shlobj.h>
#include <iostream>
#include "ntddk.h"

#include "StaticData.h"
#include "../DynImport/Api.h"
#include "../Utils/Utils.h"
#include "../Install/Install.h"
#include "../HTTP/HTTP.h"
#include "../BotWorks/Crypt.h"
#include "../BotWorks/RegBot.h"