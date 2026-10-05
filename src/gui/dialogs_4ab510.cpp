// Decompiled by Opus. Names are provisional.
// Returns 1 when the object's message is a button-down or double-click of a
// button selected by the mask (1 = left, 2 = right). The message is read
// through a reference taken up front, which is why the object pointer is
// loaded before the first test.
#include <windows.h>

struct Object_004ab510 {
    char unknown_0[0x4c];
    int message;                       // +0x4c
};

// FUNCTION: 0x4ab510
int __stdcall FUN_004ab510(Object_004ab510* obj, unsigned char buttons)
{
    int& msg = obj->message;
    if (buttons & 1) {
        if (msg == WM_LBUTTONDOWN)
            return 1;
        if (msg == WM_LBUTTONDBLCLK)
            return 1;
    } else if (buttons & 2) {
        if (msg == WM_RBUTTONDOWN)
            return 1;
        if (msg == WM_RBUTTONDBLCLK)
            return 1;
    }
    return 0;
}
