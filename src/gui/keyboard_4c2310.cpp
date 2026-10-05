// Decompiled by Opus. Names are provisional.
#include <windows.h>

// FUNCTION: 0x4c2310
void __stdcall FUN_004c2310(int* x, int* y)
{
    POINT pt;
    GetCursorPos(&pt);
    *x = pt.x;
    *y = pt.y;
}
