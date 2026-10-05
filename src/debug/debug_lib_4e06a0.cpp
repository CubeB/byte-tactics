// Decompiled by Opus. Names are provisional.
// Dialog procedure: on WM_INITDIALOG it stores the object passed as lParam in
// the window's user data (and the window handle in the object's first field),
// then forwards every message to that object's handler. Same shape as 0x4df330.
#include <windows.h>

class MemoryStatusDialog {
public:
    HWND hwnd;

    BOOL HandleMemoryStatusMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

// FUNCTION: 0x4e06a0
BOOL __stdcall MemoryStatusDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((MemoryStatusDialog*)lParam)->hwnd = hwnd;
    }
    MemoryStatusDialog* obj = (MemoryStatusDialog*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->HandleMemoryStatusMessage(msg, wParam, lParam);
    return 0;
}
