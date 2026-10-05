// Decompiled by Haiku. Names are provisional.

#include <windows.h>

extern HMODULE g_onlineDll;

// FUNCTION: 0x45b640
void OnlineUnload(void)
{
    if (g_onlineDll != 0) {
        FreeLibrary(g_onlineDll);
        g_onlineDll = 0;
    }
}
