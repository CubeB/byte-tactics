// Decompiled by Opus. Names are provisional.
// Writes an integer registry value (same code as 0x4e2d70).
#include <windows.h>

class Class_004e2e00 {
public:
    HKEY key;                          // +0x0

    void FUN_004e2e00(LPCSTR name, int value);
};

// FUNCTION: 0x4e2e00
void Class_004e2e00::FUN_004e2e00(LPCSTR name, int value)
{
    RegSetValueExA(key, name, 0, REG_DWORD, (LPBYTE)&value, 4);
}
