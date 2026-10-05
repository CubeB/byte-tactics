// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <string.h>

struct Display_004be0b0 {
    char unknown_0[0x618];
    int* files;                        // +0x618
    int count;                         // +0x61c
};

extern char DAT_0050a5c0[];            // "HAPIFILE array"

Display_004be0b0* FUN_004b6220();
void* __stdcall FUN_004bdd70(const char* name, int mode);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);

// FUNCTION: 0x4be0b0
void* __stdcall FUN_004be0b0(LPCSTR param_1, int param_2)
{
    Display_004be0b0* display = FUN_004b6220();
    char fullPath[0x100];
    char* filePart;
    GetFullPathNameA(param_1, 0x100, fullPath, &filePart);

    for (int i = 0; i < display->count; i++) {
        if (_strcmpi(fullPath, (char*)display->files[i] + 0x14) == 0)
            return 0;
    }

    void* file = FUN_004bdd70(param_1, param_2);
    if (!file)
        return 0;

    display->files = (int*)FUN_004d84a0(display->files, DAT_0050a5c0, display->count * 4 + 4);
    display->files[display->count] = (int)file;
    display->count++;
    return file;
}
