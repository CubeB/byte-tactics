// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

extern char* DAT_0050d72c;

void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);
void __cdecl FUN_004e3400(HWND hwnd, char* name);
void __cdecl FUN_004e0790(void);

class Class_004e05f0 {
public:
    HWND hwnd;                  // +0x00
    char unknown_4[0x74];
    char flag_78;               // +0x78
    void FUN_004e05f0(char on);
};

// FUNCTION: 0x4e05f0
void Class_004e05f0::FUN_004e05f0(char on)
{
    if (on) {
        if (hwnd != NULL) {
            EnableWindow(hwnd, TRUE);
            SetFocus(hwnd);
            SetForegroundWindow(hwnd);
            FUN_004e33d0(hwnd, DAT_0050d72c, 1.0, 1.0);
            SetTimer(hwnd, 1, 200, NULL);
            return;
        }
        flag_78 = 1;
        return;
    }
    if (IsWindowVisible(hwnd)) {
        KillTimer(hwnd, 1);
        FUN_004e3400(hwnd, DAT_0050d72c);
        ShowWindow(hwnd, SW_HIDE);
        FUN_004e0790();
    }
}
