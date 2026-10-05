// Decompiled by Opus. Names are provisional.
// Returns 1 when the object's message is a double-click of a button
// selected by the mask (1 = left, 2 = right); compare 0x4ab510.
#include <windows.h>

struct Object_004ab570 {
    char unknown_0[0x4c];
    int message;                       // +0x4c
};

// FUNCTION: 0x4ab570
int __stdcall FUN_004ab570(Object_004ab570* obj, unsigned char buttons)
{
    int& msg = obj->message;
    if (buttons & 1) {
        if (msg == WM_LBUTTONDBLCLK)
            return 1;
    } else if (buttons & 2) {
        if (msg == WM_RBUTTONDBLCLK)
            return 1;
    }
    return 0;
}
