// Decompiled by Opus. Names are provisional.
// Reads or writes a DWORD registry value depending on the mode flag
// (compare 0x4e2ee0, the short version).
#include <windows.h>

class CavedogRegistryKey {
public:
    DWORD ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue);
};

class Class_004e2e00 {
public:
    void WriteInt(LPCSTR name, int value);
};

class Class_004e2e20 {
public:
    HKEY key;                        // +0x0
    char reading;                    // +0x4
    void FUN_004e2e20(LPCSTR name, DWORD* value, DWORD minValue, DWORD maxValue, DWORD defaultValue);
};

// FUNCTION: 0x4e2e20
void Class_004e2e20::FUN_004e2e20(LPCSTR name, DWORD* value, DWORD minValue, DWORD maxValue, DWORD defaultValue)
{
    if (reading) {
        *value = ((CavedogRegistryKey*)this)->ReadDword(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2e00*)this)->WriteInt(name, *value);
    }
}
