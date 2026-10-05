// Decompiled by Opus. Names are provisional.
#include <windows.h>

// FUNCTION: 0x4c2310
void __stdcall GetCursorPosition(int* x, int* y)
{
    POINT pt;
    GetCursorPos(&pt);
    *x = pt.x;
    *y = pt.y;
}
