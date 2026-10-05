// Decompiled by Opus. Names are provisional.
// Creates (truncates) memdump.txt and closes it again; the argument is unused.
#include <windows.h>

// FUNCTION: 0x416240
void __stdcall FUN_00416240(char* args)
{
    HANDLE file = CreateFileA("memdump.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, 0);
    if (file != 0) {
        CloseHandle(file);
    }
}
