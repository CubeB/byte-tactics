// Decompiled by Opus. Names are provisional.
// Shuts the application down: marks it as quitting, calls FUN_004b5510(0)
// when flag 1 is set, shows an optional message box and posts WM_DESTROY
// to the main window.
#include <windows.h>

struct App_004b6230 {
    char unknown_0[0xc];
    char* title;                       // +0xc
    char unknown_10[0x40 - 0x10];
    HWND hwnd;                         // +0x40
    char unknown_44[0xf0 - 0x44];
    unsigned short bit0 : 1;           // +0xf0
    unsigned short flag : 1;           // +0xf0, mask 2
    unsigned short bits2 : 9;
    unsigned short quitting : 1;       // +0xf1, mask 8
};

extern App_004b6230* DAT_0051fbd0;
extern void __stdcall FUN_004b5510(int);

// FUNCTION: 0x4b6230
void __stdcall FUN_004b6230(char* message)
{
    DAT_0051fbd0->quitting = 1;
    if (DAT_0051fbd0->flag) {
        FUN_004b5510(0);
    }
    if (message) {
        MessageBoxA(DAT_0051fbd0->hwnd, message, DAT_0051fbd0->title, 0);
    }
    PostMessageA(DAT_0051fbd0->hwnd, WM_DESTROY, 0, 0);
}
