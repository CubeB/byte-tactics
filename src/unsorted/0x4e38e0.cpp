// Decompiled by Opus. Names are provisional.
// Sends one dword to the driver opened in DAT_00529e98 and reads back
// 8 bytes; succeeds only when exactly 8 bytes were returned.
#include <windows.h>

extern bool DAT_00529e9c;
extern HANDLE DAT_00529e98;

// FUNCTION: 0x4e38e0
bool FUN_004e38e0(DWORD a, void* out)
{
    DWORD in = a;
    if (!DAT_00529e9c) {
        return false;
    }
    DWORD returned;
    BOOL ok = DeviceIoControl(DAT_00529e98, 0x9c406404, &in, sizeof(in), out, 8, &returned, 0);
    if (returned != 8) {
        ok = 0;
    }
    return ok != 0;
}
