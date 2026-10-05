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

extern App_004b6290* g_display;

// FUNCTION: 0x4b6290
void __stdcall FatalError(char* message)
{
    if (message) {
        MessageBoxA(g_display->hwnd, message, g_display->title, MB_ICONHAND | MB_SYSTEMMODAL);
    }
    exit(1);
}
