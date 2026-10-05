// Decompiled by Opus. Names are provisional.
#include <windows.h>

extern HWND g_cdPlayerWindow;

void __stdcall FUN_004b6b50(unsigned int param_1);

// A method of the object at g_game+0x10 (its only caller, 0x426190, loads ecx
// from there) that never uses `this`.
class Class_004ce190 {
public:
    void CloseCdPlayerWindow();
};

// FUNCTION: 0x4ce190
void Class_004ce190::CloseCdPlayerWindow()
{
    if (g_cdPlayerWindow) {
        SendMessageA(g_cdPlayerWindow, WM_CLOSE, 0, 0);
        SendMessageA(g_cdPlayerWindow, WM_QUIT, 0, 0);
        FUN_004b6b50(500);
        g_cdPlayerWindow = 0;
    }
}
