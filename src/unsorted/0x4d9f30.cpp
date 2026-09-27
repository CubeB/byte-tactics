// Decompiled by Opus. Names are provisional.
// A DllMain-shaped entry point: on DLL_PROCESS_ATTACH it saves the module
// handle (read back by 0x4d9f50). `sub eax, 0; je; dec eax; jne` is a switch
// with cases 0 and 1.
#include <windows.h>

extern int DAT_005289c4;

// FUNCTION: 0x4d9f30
BOOL __stdcall FUN_004d9f30(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason) {
    case DLL_PROCESS_DETACH:
        break;
    case DLL_PROCESS_ATTACH:
        DAT_005289c4 = (int)instance;
        break;
    }
    return TRUE;
}
