// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <string.h>

extern HWND g_cdPlayerWindow;        // the CD player's window, if found

// EnumWindows callback: remembers the window whose class is the CD player's.
// FUNCTION: 0x4ce1e0
BOOL __stdcall FindCdPlayerWindow(HWND hwnd, LPARAM param)
{
    char className[200];
    GetClassNameA(hwnd, className, sizeof(className) - 1);
    if (strcmp(className, "SJE_CdPlayerClass") == 0)
        g_cdPlayerWindow = hwnd;
    return TRUE;
}
