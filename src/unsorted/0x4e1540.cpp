// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

extern int DAT_00529dc0;
extern CRITICAL_SECTION DAT_00529da8;

// FUNCTION: 0x4e1540
void FUN_004e1540()
{
    if (DAT_00529dc0 == 2) {
        LeaveCriticalSection(&DAT_00529da8);
    }
}
