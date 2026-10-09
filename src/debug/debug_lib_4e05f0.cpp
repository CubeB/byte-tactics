// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

extern char* g_memoryStatusWindowName;

void __cdecl RestoreWindow(HWND hwnd, char* name, double a, double b);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void __cdecl UnloadPsapi(void);

class MemoryStatusDialog {
public:
    HWND hwnd;                  // +0x00
    char unknown_4[0x74];
    char flag_78;               // +0x78
    void SetMemoryStatusWindowVisible(char on);
};

// FUNCTION: 0x4e05f0
void MemoryStatusDialog::SetMemoryStatusWindowVisible(char on)
{
    if (on) {
        if (hwnd != NULL) {
            EnableWindow(hwnd, TRUE);
            SetFocus(hwnd);
            SetForegroundWindow(hwnd);
            RestoreWindow(hwnd, g_memoryStatusWindowName, 1.0, 1.0);
            SetTimer(hwnd, 1, 200, NULL);
            return;
        }
        flag_78 = 1;
        return;
    }
    if (IsWindowVisible(hwnd)) {
        KillTimer(hwnd, 1);
        SaveWindowPosition(hwnd, g_memoryStatusWindowName);
        ShowWindow(hwnd, SW_HIDE);
        UnloadPsapi();
    }
}
