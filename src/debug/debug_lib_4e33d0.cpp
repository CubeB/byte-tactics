// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

void __cdecl RestoreWindowPosition(HWND hwnd, char* name, double a, double b, int flag);

// FUNCTION: 0x4e33d0
void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b)
{
    RestoreWindowPosition(hwnd, name, a, b, 1);
}
