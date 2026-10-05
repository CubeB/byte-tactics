// Decompiled by Opus. Names are provisional.
// Keeps pumping messages and updating the menu while its current screen's
// name matches the given one.
#include <windows.h>
#include <string.h>

struct Screen_0049f9c0 {
    char unknown_0[4];
    char* name;                        // +0x4
};

struct Menu_0049f9c0 {
    char unknown_0[0x18];
    Screen_0049f9c0* screen;           // +0x18
};

void __stdcall FUN_004a9fd0(Menu_0049f9c0* menu);
void __stdcall FUN_004ab0b0(void* param_1, unsigned int* param_2, int* param_3);
void FUN_004c2870();
void FlipScreen();

// FUNCTION: 0x49f9c0
void __stdcall FUN_0049f9c0(Menu_0049f9c0* menu, char* name)
{
    MSG msg;
    while (1) {
        if (!menu->screen || _strnicmp(menu->screen->name + 2, name, 16) != 0)
            break;
        if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        FUN_004a9fd0(menu);
        FUN_004ab0b0(menu->screen, 0, 0);
        FUN_004c2870();
        FlipScreen();
    }
}
