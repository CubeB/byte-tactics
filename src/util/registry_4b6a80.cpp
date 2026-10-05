// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <string.h>

// FUNCTION: 0x4b6a80
int __stdcall GetWindowsUserName(char* out)
{
    DWORD size;
    char name[256];
    size = 256;
    if (GetUserNameA(name, &size) && name[0] != 0) {
        strcpy(out, name);
        return 1;
    }
    return 0;
}
