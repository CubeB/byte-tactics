// Decompiled by Opus. Names are provisional.
// Dialog procedure: on WM_INITDIALOG it stores the object passed as lParam in
// the window's user data (and the window handle in the object's first field),
// then forwards every message to that object's handler. Same shape as 0x4df330.
#include <windows.h>

class Class_004e0b90 {
public:
    HWND hwnd;

    BOOL FUN_004e0b90(UINT msg, WPARAM wParam, LPARAM lParam);
};

// FUNCTION: 0x4e06a0
BOOL __stdcall FUN_004e06a0(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((Class_004e0b90*)lParam)->hwnd = hwnd;
    }
    Class_004e0b90* obj = (Class_004e0b90*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->FUN_004e0b90(msg, wParam, lParam);
    return 0;
}
