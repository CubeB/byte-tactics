// Decompiled by Opus. Names are provisional.
// Changes the current directory to the one holding the executable.
#include <windows.h>

char* __stdcall FUN_004bb120(char* path);

// FUNCTION: 0x49f540
void FUN_0049f540()
{
    char path[256];

    GetModuleFileNameA(NULL, path, 256);
    FUN_004bb120(path);
    SetCurrentDirectoryA(path);
}
