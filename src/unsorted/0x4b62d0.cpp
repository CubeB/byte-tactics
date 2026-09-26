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

extern GameCtx_4b62d0* DAT_0051fbd0;
extern Slot_4b62d0 DAT_0051fbe0[10];
extern int DAT_0051fc80;
extern unsigned int DAT_0051fc84;

// FUNCTION: 0x4b62d0
void __stdcall FUN_004b62d0(int param_1)
{
    DAT_0051fbd0->f_e8 = param_1;
    DAT_0051fc80 = 0;
    for (int i = 0; i < 10; i++) {
        DAT_0051fbe0[i].id = -1;
    }
    DAT_0051fc84 = (GetTickCount() * DAT_0051fbd0->f_e8) / 1000;
}
