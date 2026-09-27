// Decompiled by Opus. Names are provisional.
#include <windows.h>

extern HWND DAT_0051ff18;

void __stdcall FUN_004b6b50(unsigned int param_1);

// A method of the object at g_game+0x10 (its only caller, 0x426190, loads ecx
// from there) that never uses `this`.
class Class_004ce190 {
public:
    void FUN_004ce190();
};

// FUNCTION: 0x4ce190
void Class_004ce190::FUN_004ce190()
{
    if (DAT_0051ff18) {
        SendMessageA(DAT_0051ff18, WM_CLOSE, 0, 0);
        SendMessageA(DAT_0051ff18, WM_QUIT, 0, 0);
        FUN_004b6b50(500);
        DAT_0051ff18 = 0;
    }
}
