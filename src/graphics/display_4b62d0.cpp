// Decompiled by Opus. Names are provisional.
#include <windows.h>

struct GameCtx_4b62d0 {
    char unknown_0[0xe8];
    int f_e8;                        // +0xe8
};

struct Slot_4b62d0 {
    int id;                          // +0x0
    int unknown_4[3];
};

extern GameCtx_4b62d0* g_display;
extern Slot_4b62d0 DAT_0051fbe0[10];
extern int g_timerCount;
extern unsigned int DAT_0051fc84;

// FUNCTION: 0x4b62d0
void __stdcall InitTimers(int param_1)
{
    g_display->f_e8 = param_1;
    g_timerCount = 0;
    for (int i = 0; i < 10; i++) {
        DAT_0051fbe0[i].id = -1;
    }
    DAT_0051fc84 = (GetTickCount() * g_display->f_e8) / 1000;
}
