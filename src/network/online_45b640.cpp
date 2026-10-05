// Decompiled by Haiku. Names are provisional.

#include <windows.h>

extern HMODULE DAT_00512eec;

// FUNCTION: 0x45b640
void FUN_0045b640(void)
{
    if (DAT_00512eec != 0) {
        FreeLibrary(DAT_00512eec);
        DAT_00512eec = 0;
    }
}
