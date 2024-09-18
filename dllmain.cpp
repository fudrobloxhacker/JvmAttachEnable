#include <Windows.h>
#include <winternl.h>
#include <cstdint>
#include <cstdio>
#include <cwchar>

// target address (0x180B878CC) - base address (0x180001000) = 0xB868CC
const uintptr_t DISABLE_ATTACH_MECHANISM_OFFSET = 0xB868CC;

HMODULE GetModuleHandleModern(const wchar_t* moduleName) {
    PPEB pPeb = reinterpret_cast<PPEB>(__readgsqword(0x60));
    PPEB_LDR_DATA pLdr = pPeb->Ldr;
    PLIST_ENTRY firstLink = &pLdr->InMemoryOrderModuleList;
    PLIST_ENTRY currentLink = firstLink->Flink;

    while (currentLink != firstLink) {
        LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(currentLink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        if (entry->FullDllName.Buffer) {
            wchar_t* fullPath = entry->FullDllName.Buffer;
            wchar_t* baseName = wcsrchr(fullPath, L'\\');
            if (baseName) {
                baseName++;
            }
            else {
                baseName = fullPath;
            }

            if (_wcsicmp(baseName, moduleName) == 0) {
                return reinterpret_cast<HMODULE>(entry->DllBase);
            }
        }
        currentLink = currentLink->Flink;
    }
    return nullptr;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        AllocConsole();
        FILE* f;
        if (freopen_s(&f, "CONOUT$", "w", stdout) == 0) {
            HMODULE jvmModule = GetModuleHandleModern(L"jvm.dll");
            if (jvmModule) {
                printf("found jvm.dll\n");

                bool* disableAttachMechanism = reinterpret_cast<bool*>(
                    reinterpret_cast<uintptr_t>(jvmModule) + DISABLE_ATTACH_MECHANISM_OFFSET);

                printf("DisableAttachMechanism address: %p, initial value: %d\n",
                    static_cast<void*>(disableAttachMechanism), *disableAttachMechanism);

                *disableAttachMechanism = false;
                printf("new value: %d\n", *disableAttachMechanism);
            }
            else {
                printf("failed to find jvm.dll\n");
            }
            fclose(f);
        }
        FreeConsole();
    }
    return TRUE;
}