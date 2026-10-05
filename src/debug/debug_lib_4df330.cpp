// Decompiled by Opus. Names are provisional.
// Dialog procedure: on WM_INITDIALOG it stores the object passed as lParam in
// the window's user data (and the window handle in the object's first field),
// then forwards every message to that object's handler.
#include <windows.h>

class PerformanceDialog {
public:
    HWND hwnd;

    BOOL HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

// FUNCTION: 0x4df330
BOOL __stdcall PerformanceDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((PerformanceDialog*)lParam)->hwnd = hwnd;
    }
    PerformanceDialog* obj = (PerformanceDialog*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->HandlePerformanceMessage(msg, wParam, lParam);
    return 0;
}
