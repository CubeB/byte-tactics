// Decompiled by Opus. Names are provisional.
// Dialog procedure: on WM_INITDIALOG it stores the object passed as lParam in
// the window's user data (and the window handle in the object's first field),
// then forwards every message to that object's handler.
#include <windows.h>

class Class_004df590 {
public:
    HWND hwnd;

    BOOL FUN_004df590(UINT msg, WPARAM wParam, LPARAM lParam);
};

// FUNCTION: 0x4df330
BOOL __stdcall FUN_004df330(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((Class_004df590*)lParam)->hwnd = hwnd;
    }
    Class_004df590* obj = (Class_004df590*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->FUN_004df590(msg, wParam, lParam);
    return 0;
}
