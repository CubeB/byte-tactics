// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

HWND FUN_004da540(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK FUN_004df330(HWND, UINT, WPARAM, LPARAM);

class Class_004df250 {
public:
    void FUN_004df250(void);
};

// FUNCTION: 0x4df250
void Class_004df250::FUN_004df250(void)
{
    if (FUN_004da540(0x67, GetDesktopWindow(), (DLGPROC)FUN_004df330, (LPARAM)this) == 0) {
        MessageBoxA(0, "Performance dialog failed to open", "Cavedog", 0);
    }
}
