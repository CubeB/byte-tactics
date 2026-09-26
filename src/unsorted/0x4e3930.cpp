// Decompiled by Opus. Names are provisional.
#include <windows.h>

extern bool DAT_00529e9c;
extern HANDLE DAT_00529e98;

// FUNCTION: 0x4e3930
bool FUN_004e3930(DWORD a, DWORD b, DWORD c)
{
    if (!DAT_00529e9c) {
        return false;
    }
    DWORD in[3];
    in[0] = a;
    in[1] = b;
    in[2] = c;
    DWORD returned;
    BOOL ok = DeviceIoControl(DAT_00529e98, 0x9c406400, in, sizeof(in), 0, 0, &returned, 0);
    if (returned != 0) {
        ok = 0;
    }
    return ok != 0;
}
