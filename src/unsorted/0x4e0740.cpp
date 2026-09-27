// Decompiled by Opus. Names are provisional.
#include <windows.h>

LPCRITICAL_SECTION FUN_004e06f0(void);

extern HMODULE DAT_005295bc;
extern int DAT_00529508;
extern char DAT_005295cc;

// FUNCTION: 0x4e0740
void FUN_004e0740(void)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    EnterCriticalSection(cs);
    if (DAT_005295bc != 0) {
        FreeLibrary(DAT_005295bc);
        DAT_005295bc = 0;
        DAT_00529508 = 0;
    }
    DAT_005295cc = 1;
    LeaveCriticalSection(cs);
}
