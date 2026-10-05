// Decompiled by Opus. Names are provisional.
// Signed counterpart of 0x4e2d90: reads a REG_DWORD value clamped to
// [minValue, maxValue], or returns defaultValue.
#include <windows.h>

class Class_004e2d00 {
public:
    HKEY key;                        // +0x00
    int FUN_004e2d00(LPCSTR name, int minValue, int maxValue, int defaultValue);
};

// FUNCTION: 0x4e2d00
int Class_004e2d00::FUN_004e2d00(LPCSTR name, int minValue, int maxValue, int defaultValue)
{
    int value;
    DWORD size = 4;
    DWORD type = REG_DWORD;
    if (RegQueryValueExA(key, name, 0, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS
        && size == 4 && type == REG_DWORD) {
        if (value < minValue) {
            value = minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }
    return defaultValue;
}
