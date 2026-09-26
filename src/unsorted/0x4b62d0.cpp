// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

struct GameCtx_4b62d0 {
    char unknown_0[0xe8];
    int f_e8;   // +0xe8
};

extern GameCtx_4b62d0* DAT_0051fbd0;
extern int DAT_0051fbe0[];
extern int DAT_0051fc80;
extern unsigned int DAT_0051fc84;

// FUNCTION: 0x4b62d0
void __stdcall FUN_004b62d0(int param_1)
{
    DAT_0051fbd0->f_e8 = param_1;
    DAT_0051fc80 = 0;

    int* p = DAT_0051fbe0;
    do {
        *p = -1;
        p += 4;
    } while ((int)p < (int)&DAT_0051fc80);

    int tick = (int)GetTickCount();
    GameCtx_4b62d0* p2 = DAT_0051fbd0;
    int uVar1 = p2->f_e8 * tick;
    DAT_0051fc84 = (unsigned int)uVar1 / 1000u;
}
