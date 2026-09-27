// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

HWND FUN_004da540(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK FUN_004e06a0(HWND, UINT, WPARAM, LPARAM);

extern char DAT_0050d6b4[]; // "Performance dialog failed to open"
extern char DAT_0050c8ac[]; // "Cavedog"

class Class_004e05c0 {
public:
    void FUN_004e05c0();
};

// FUNCTION: 0x4e05c0
void Class_004e05c0::FUN_004e05c0()
{
    HWND result = FUN_004da540(0x66, GetDesktopWindow(), (DLGPROC)FUN_004e06a0, (LPARAM)this);
    if (result == 0)
        MessageBoxA(0, DAT_0050d6b4, DAT_0050c8ac, 0);
}
