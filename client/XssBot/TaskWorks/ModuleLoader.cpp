#include <windows.h>
#include "../DynImport/Api.h"
#include "../String/StringHelpers.h"

#define MAX_SECTIONS 10  // максимальное кол-во секций в файле
#pragma warning(disable:4200)

typedef unsigned long ULONG, * PULONG;
typedef unsigned short USHORT, * PUSHORT;

// структура для релоков
typedef struct _FIXREC
{
	ULONG PageRVA;
	ULONG BlockSize;
	USHORT TOR[];
} TFIXREC, * PFIXREC;

// структура таблицы импорта
typedef struct _IMPORT_TABLE
{
	ULONG LookUp;
	ULONG TimeStamp;
	ULONG ForwardChain;
	ULONG NameRVA;
	ULONG AddresTableRVA;
} IMPORT_TABLE, * PIMPORT_TABLE;

#pragma pack(1)

// структура таблица адресов
typedef struct _ADDRESS_TABLE
{
	USHORT Hint;
	char Name[];
} ADDRESS_TABLE, * PADDRESS_TABLE;

#pragma pack()

// Обработчик импорта
static ULONG ProgressImport(ULONG filebase)
{
	ULONG PE;
	HMODULE lib;
	PIMPORT_TABLE ImportTable;
	PADDRESS_TABLE AddressTable;
	ULONG IAT_Index;
	ULONG RVA;
	ULONG addr;

	PE = *(ULONG*)(filebase + 0x3C) + filebase; // адрес PE заголовка
	if (!*(ULONG*)(PE + 0x80)) return 1; // если нет импорта то выходим

	ImportTable = (PIMPORT_TABLE)(*(ULONG*)(PE + 0x80) + filebase); // адрес таблицыы импорта

	while (ImportTable->NameRVA) // пока есть DLL откуда нужно импортировать функции
	{
		// проверим что DLL была ранее загружена
		lib = API(KERNEL32, GetModuleHandleA)((char*)(ImportTable->NameRVA + filebase));
		if (!lib) // если не загружена была, то загрузим её.
		{
			lib = API(KERNEL32, LoadLibraryA)((char*)(ImportTable->NameRVA + filebase));
		}

		if (!lib) return 0; // если не загрузилась, значит ошибка

		if (ImportTable->LookUp) // Если импорт идет через LookUp
		{
			RVA = ImportTable->LookUp + filebase;
		}
		else // если через таблицу адресов импорта
		{
			RVA = ImportTable->AddresTableRVA + filebase;
		}

		IAT_Index = 0;

		while (*(ULONG*)RVA) // если есть ссылка на таблицу имен
		{
			AddressTable = (PADDRESS_TABLE)(*(ULONG*)RVA + filebase); // получаем адрес структуры где хранится HINT NAME
			if (AddressTable->Name[0]) // если импорт по имени
			{
				addr = (ULONG)API(KERNEL32, GetProcAddress)(lib, AddressTable->Name); // найдем адрес
			}
			else // если импорт по ординалу
			{
				addr = (ULONG)API(KERNEL32, GetProcAddress)(lib, (char*)AddressTable->Hint);
			}
			// если есть IAT то сохраним в неё найденный адрес
			if (ImportTable->AddresTableRVA)
			{
				*(ULONG*)(ImportTable->AddresTableRVA + filebase + IAT_Index) = addr;
			}
			else // иначе сохраним туда откуда брали
			{
				*(ULONG*)RVA = addr;
			}
			RVA += 4; // сделающий элемент
			IAT_Index += 4;
		}
		ImportTable = (PIMPORT_TABLE)((ULONG)ImportTable + sizeof(IMPORT_TABLE)); // следующая таблица
	}
	return 1;
}

// обработка релоков
static ULONG ProgressReloc(ULONG filebase)
{
	ULONG PE;
	ULONG IB;
	ULONG cnt;
	ULONG x;
	ULONG Delta;
	PFIXREC fixrec;
	USHORT fixtype;
	USHORT fixoffset;

	PE = *(ULONG*)(filebase + 0x3C) + filebase; // получаем адрес PE заголовка
	IB = *(ULONG*)(PE + 0x34); // IMAGE BASE 
	if (filebase == IB) return 1; // Если совпадает с загруженным адресом, то фиксить не нужно ничего
	Delta = filebase - IB; // выцесляем дельта смещение.

	if (!*(ULONG*)(PE + 0xA0)) return 1; // если нет релоков то выходим
	fixrec = (PFIXREC)(*(ULONG*)(PE + 0xA0) + filebase); // получаем адрес таблицы релоков

	while (fixrec->BlockSize) // если таблица не пуста
	{
		cnt = (fixrec->BlockSize - 8) >> 1; // вычеслим кол-во элементов
		for (x = 0; x < cnt; x++)
		{
			fixtype = (fixrec->TOR[x]) >> 12; // типа фиксации
			fixoffset = (fixrec->TOR[x]) % 4096; // офсет внутри 4-х килобайтового блока
			if (!fixtype) continue; // если 0, то фиксация не нужна
			if (fixtype == 3) // если 3, то прибавить дельта смещение
			{
				*(ULONG*)(filebase + fixoffset + fixrec->PageRVA) = *(ULONG*)(filebase + fixoffset + fixrec->PageRVA) + Delta;
			}
			else return 0; // все остальные случае вызовут ошибку (хотя их и не будет теоретически)
		}
		fixrec = (PFIXREC)((ULONG)fixrec + fixrec->BlockSize); // следующая таблица реловок
	}
	return 1;
}

// непосредственно функция загрузки DLL 
HMODULE __stdcall LoadModule(char* module)
{
	ULONG retadr = 0;
	IMAGE_DOS_HEADER DosHeader;
	IMAGE_NT_HEADERS PeHeader;
	IMAGE_SECTION_HEADER Section[MAX_SECTIONS];

	char tmp[1024];

	// считаем DOS заголовок
	memCpy(&DosHeader, module, sizeof(IMAGE_DOS_HEADER));
	if (DosHeader.e_magic == IMAGE_DOS_SIGNATURE) // проверим сигнатуру
	{
		// если есть какимето данные между DOS заголовком и PE 
		// то считаем их. В MS компиляторах это часто Rich данные
		if (sizeof(IMAGE_DOS_HEADER) < DosHeader.e_lfanew)
		{
			memCpy(&tmp[0], (char*)(module + sizeof(IMAGE_DOS_HEADER)), DosHeader.e_lfanew - sizeof(IMAGE_DOS_HEADER));
		}

		// установим указатель
		char* module_pheader = module + DosHeader.e_lfanew;

		// считаем заголовок
		memCpy(&PeHeader, module_pheader, sizeof(IMAGE_NT_HEADERS));

		if (PeHeader.Signature == IMAGE_NT_SIGNATURE) // проверим сигнатуру
		{
			// считаем 10 секций
			memCpy(&Section[0], (char*)(module_pheader + sizeof(IMAGE_NT_HEADERS)), sizeof(IMAGE_SECTION_HEADER) * PeHeader.FileHeader.NumberOfSections);
			// выделим память столько, сколько указано в SIZE OF BASE
			retadr = (ULONG)API(KERNEL32, VirtualAlloc)(0, PeHeader.OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE | MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);

			if (retadr) // если память выделилась
			{
				// скопируем туда DOS заголовок
				memCpy((void*)retadr, &DosHeader, sizeof(IMAGE_DOS_HEADER));
				// скопируем туда PE заголовок
				memCpy((void*)(retadr + DosHeader.e_lfanew), &PeHeader, sizeof(IMAGE_NT_HEADERS));
				// скопируем туда таблицу секций
				memCpy((void*)(retadr + DosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS)), &Section[0], sizeof(IMAGE_SECTION_HEADER) * PeHeader.FileHeader.NumberOfSections);

				// если есть Rich данные то и их тоже скопируем
				if (sizeof(IMAGE_DOS_HEADER) < DosHeader.e_lfanew)
				{
					memCpy((void*)(retadr + sizeof(IMAGE_DOS_HEADER)), &tmp[0], DosHeader.e_lfanew - sizeof(IMAGE_DOS_HEADER));
				}

				// обработаем каждую секцию
				for (int i = 0; i < PeHeader.FileHeader.NumberOfSections; i++)
				{
					// установим указатель на начало секции в файле
					char* PointerToRawData = module + Section[i].PointerToRawData;
					// считаем всё секцию
					memCpy((void*)(retadr + Section[i].VirtualAddress), PointerToRawData, Section[i].SizeOfRawData);
				}

				// Обработаем релоки
				if (!ProgressReloc(retadr)) // если ошибка
				{
					API(KERNEL32, VirtualFree)((void*)retadr, 0, MEM_RELEASE); // освободим память
					retadr = 0;
				}
				else if (!ProgressImport(retadr))// обработаем импорт
				{
					API(KERNEL32, VirtualFree)((void*)retadr, 0, MEM_RELEASE);// если ошибка освободим память
					retadr = 0;
				}
				else
				{
					__asm
					{
						mov eax, PeHeader.OptionalHeader.AddressOfEntryPoint
						add eax, retadr // EAX = ENTRY POINT
						push 0
						push DLL_PROCESS_ATTACH // ставим флаг что подгрузили DLL
						push retadr
						call eax // передадим управление на точку входа в DLL 
					}
				}
			}
		}
	}
	return (HMODULE)retadr; // возвращаем адрес загруженного модуля в памяти
}

// Непосредственно функция Поиска адреса
FARPROC _stdcall GetModuleProcAddress(HMODULE hModule, char* lpProcName)
{
	ULONG PE;
	PIMAGE_EXPORT_DIRECTORY ED;
	PULONG NamePointerRVA;
	PUSHORT OrdinalTableRVA;
	PULONG AddressTableRVA;
	ULONG ret = 0;
	USHORT hint = 0;
	USHORT index;
	char* name;
	ULONG addr;
	ULONG CNT_FUNC;

	if ((ULONG)lpProcName <= 0xFFFF)// если импорт по ординалу
	{
		hint = (USHORT)lpProcName; // запишем наш ординал
	}

	if (hModule) // если указан модуль откуда грузить
	{
		PE = *(ULONG*)((ULONG)hModule + 0x3C) + (ULONG)hModule;// адрес PE заголовка
		ED = (PIMAGE_EXPORT_DIRECTORY)(*(ULONG*)((ULONG)PE + 0x78) + (ULONG)hModule); // адрес таблицы экспорта
		NamePointerRVA = (ULONG*)(ED->AddressOfNames + (ULONG)hModule); // адрес таблицы имен
		OrdinalTableRVA = (USHORT*)(ED->AddressOfNameOrdinals + (ULONG)hModule); // адрес таблицы ординалов
		AddressTableRVA = (ULONG*)(ED->AddressOfFunctions + (ULONG)hModule); // адрес таблицы адерсов

		// вычесляем наибольшее значение - кол-во функций
		if (ED->NumberOfNames > ED->NumberOfFunctions)
		{
			CNT_FUNC = ED->NumberOfNames;
		}
		else
		{
			CNT_FUNC = ED->NumberOfFunctions;
		}

		// пройдемся по всем функциям
		for (USHORT x = 0; x < CNT_FUNC; x++)
		{
			if (x < ED->NumberOfFunctions) // если есть имя у функции
			{
				name = (char*)(NamePointerRVA[x] + (ULONG)hModule); // запомним имя
				index = OrdinalTableRVA[x]; // запомним ординал
			}
			else // если имени нет
			{
				name = 0; // имени нет
				index = x; // ординал = текущей позиции
			}

			// вычесляем адрес функции 
			addr = AddressTableRVA[index] + (ULONG)hModule;
			if ((hint == index + ED->Base) || // если это наш ординал

				(name && !strCmpA(name, lpProcName))) // если есть имя и оно наше

			{
				ret = addr; // нашли адрес
				break; // прекратим обход экспорта
			}
		}
	}
	return (FARPROC)ret; // вернем адрес 
}

// функция выгрузки DLL
BOOL __stdcall FreeLoadedModule(HMODULE hModule)
{
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS PeHeader;
	BOOL ret = false;
	ULONG EP;

	if (hModule) // если указан адрес модуля
	{
		DosHeader = (PIMAGE_DOS_HEADER)hModule; //  DOS заголово
		PeHeader = (PIMAGE_NT_HEADERS)(DosHeader->e_lfanew + (ULONG)hModule);// PE заголовок
		EP = PeHeader->OptionalHeader.AddressOfEntryPoint + (ULONG)hModule; // Точка входа
		__asm
		{
			push 0
			push DLL_PROCESS_DETACH //  флаг выгрузки DLL
			push hModule
			call EP // передаем управление на точку входа в DLL
		}
		API(KERNEL32, VirtualFree)(hModule, 0, MEM_RELEASE); // очищаем память
		ret = true;
	}
	return ret;
}