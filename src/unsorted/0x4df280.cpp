// Decompiled by space-bunny-free. Names are provisional.
// Performance-window show/hide. A non-zero argument enables the window, focuses
// it, brings it to the front, re-centres it and starts a 200ms timer. If the
// window handle is still null the flag at +0x20 is raised instead, so the
// caller can ask again later. A zero argument hides the window, but only while
// it is still visible.
// Near-copy of 0x4e05f0, which does the same for another dialog.
#include <windows.h>

extern char* DAT_0050d660;

void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);
void __cdecl FUN_004e3400(HWND hwnd, char* name);

class Class_004df280 {
public:
    HWND hwnd;                          // +0x00
    char unknown_4[0x1c];
    unsigned char flag_20;              // +0x20

    void FUN_004df280(char show);
};

// FUNCTION: 0x4df280
void Class_004df280::FUN_004df280(char show)
{
    if (show) {
        if (hwnd) {
            EnableWindow(hwnd, 1);
            SetFocus(hwnd);
            SetForegroundWindow(hwnd);
            FUN_004e33d0(hwnd, DAT_0050d660, 1.0, 1.0);
            SetTimer(hwnd, 1, 200, 0);
        } else {
            flag_20 = 1;
        }
    } else if (IsWindowVisible(hwnd)) {
        KillTimer(hwnd, 1);
        FUN_004e3400(hwnd, DAT_0050d660);
        ShowWindow(hwnd, 0);
    }
}
