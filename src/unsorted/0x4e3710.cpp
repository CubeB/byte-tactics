// Decompiled by Opus. Names are provisional.
#include <windows.h>

extern char DAT_00529e9c;
extern HANDLE DAT_00529e98;

// FUNCTION: 0x4e3710
bool FUN_004e3710()
{
    if (DAT_00529e9c) {
        DAT_00529e9c = 0;
        if (DAT_00529e98) {
            BOOL ok = CloseHandle(DAT_00529e98);
            DAT_00529e98 = 0;
            if (ok)
                return true;
        }
    }
    return false;
}
