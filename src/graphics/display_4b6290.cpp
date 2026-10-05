// Decompiled by Opus. Names are provisional.
// Fatal error: shows an optional message box and exits the process.
#include <windows.h>
#include <stdlib.h>

struct App_004b6290 {
    char unknown_0[0xc];
    char* title;                       // +0xc
    char unknown_10[0x40 - 0x10];
    HWND hwnd;                         // +0x40
};

extern App_004b6290* DAT_0051fbd0;

// FUNCTION: 0x4b6290
void __stdcall FUN_004b6290(char* message)
{
    if (message) {
        MessageBoxA(DAT_0051fbd0->hwnd, message, DAT_0051fbd0->title, MB_ICONHAND | MB_SYSTEMMODAL);
    }
    exit(1);
}
