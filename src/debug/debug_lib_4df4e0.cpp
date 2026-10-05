// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Enables or disables eight controls of the performance dialog from one
// value: the release-enabled global, but only when the settings have been
// loaded (FUN_004e1680 reports that).
#include <windows.h>

extern unsigned char DAT_00529dd8;
unsigned char __cdecl FUN_004e1680(void);

class Class_004df4e0 {
public:
    HWND hwnd;                          // +0x00
    void FUN_004df4e0();
};

// FUNCTION: 0x4df4e0
void Class_004df4e0::FUN_004df4e0()
{
    unsigned char b = FUN_004e1680() && DAT_00529dd8;
    EnableWindow(GetDlgItem(hwnd, 0x3f5), b);
    EnableWindow(GetDlgItem(hwnd, 0x3ed), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f3), b);
    EnableWindow(GetDlgItem(hwnd, 0x3ee), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f1), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f6), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f7), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f8), b);
}
