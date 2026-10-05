// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

extern HANDLE DAT_0052a4f8;
extern int DAT_0052a4fc;

// FUNCTION: 0x4d09e0
void FUN_004d09e0()
{
    DAT_0052a4fc = 0;
    if (DAT_0052a4f8 != 0) {
        ResetEvent(DAT_0052a4f8);
        return;
    }
    DAT_0052a4f8 = CreateEventA(0, 0, 0, 0);
}
