// Decompiled by Haiku. Names are provisional.
#include <windows.h>

class Class_004e2d70 {
public:
    HKEY field_0;

    void WriteDword(LPCSTR param_1, DWORD param_2);
};

// FUNCTION: 0x4e2d70
void Class_004e2d70::WriteDword(LPCSTR param_1, DWORD param_2) {
    RegSetValueExA(field_0, param_1, 0, REG_DWORD, (LPBYTE)&param_2, 4);
}
