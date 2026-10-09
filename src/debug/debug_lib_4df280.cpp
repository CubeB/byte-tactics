// Decompiled by space-bunny-free. Names are provisional.
// Performance-window show/hide. A non-zero argument enables the window, focuses
// it, brings it to the front, re-centres it and starts a 200ms timer. If the
// window handle is still null the flag at +0x20 is raised instead, so the
// caller can ask again later. A zero argument hides the window, but only while
// it is still visible.
// Near-copy of 0x4e05f0, which does the same for another dialog.
#include <windows.h>

extern char* g_performanceWindowName;

void __cdecl RestoreWindow(HWND hwnd, char* name, double a, double b);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);

class PerformanceDialog {
public:
    HWND hwnd;                          // +0x00
    char unknown_4[0x1c];
    unsigned char flag_20;              // +0x20

    void SetPerformanceWindowVisible(char show);
};

// FUNCTION: 0x4df280
void PerformanceDialog::SetPerformanceWindowVisible(char show)
{
    if (show) {
        if (hwnd) {
            EnableWindow(hwnd, 1);
            SetFocus(hwnd);
            SetForegroundWindow(hwnd);
            RestoreWindow(hwnd, g_performanceWindowName, 1.0, 1.0);
            SetTimer(hwnd, 1, 200, 0);
        } else {
            flag_20 = 1;
        }
    } else if (IsWindowVisible(hwnd)) {
        KillTimer(hwnd, 1);
        SaveWindowPosition(hwnd, g_performanceWindowName);
        ShowWindow(hwnd, 0);
    }
}
