// Decompiled by Opus. Names are provisional.
#include <windows.h>

extern int DAT_0052a4e8;
extern HANDLE DAT_0052a4f0;

// FUNCTION: 0x4c22a0
void FUN_004c22a0()
{
    DAT_0052a4e8 = 0;
    if (DAT_0052a4f0) {
        ResetEvent(DAT_0052a4f0);
        return;
    }
    DAT_0052a4f0 = CreateEventA(NULL, FALSE, FALSE, NULL);
}
