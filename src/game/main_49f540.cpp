// Decompiled by Opus. Names are provisional.
// Changes the current directory to the one holding the executable.
#include <windows.h>

char* __stdcall StripFileName(char* path);

// FUNCTION: 0x49f540
void ChdirToExeDirectory()
{
    char path[256];

    GetModuleFileNameA(NULL, path, 256);
    StripFileName(path);
    SetCurrentDirectoryA(path);
}
