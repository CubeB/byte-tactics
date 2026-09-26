// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

struct GameCtx_4b6370 {
    char unknown_0[0xe8];
    unsigned int f_e8;
};

extern GameCtx_4b6370* DAT_0051fbd0;
extern unsigned int DAT_0051fc84;
extern int DAT_0051fc80;
extern int DAT_0051fbe0;

typedef int (__stdcall *Callback_4b6370)(int);

// FUNCTION: 0x4b6370
void FUN_004b6370()
{
    unsigned int q1 = (GetTickCount() * DAT_0051fbd0->f_e8) / 1000;
    int diff = (int)q1 - DAT_0051fc84;
    DAT_0051fc84 = (GetTickCount() * DAT_0051fbd0->f_e8) / 1000;

    int* p = &DAT_0051fbe0;
    do {
        if (p[0] >= 0) {
            if ((p[1] -= diff) <= 0) {
                int arg = p[-1];
                Callback_4b6370 fn = (Callback_4b6370)p[-2];
                fn(arg);
                p[1] = p[0];
            }
        }
        p += 4;
    } while ((int)p < (int)&DAT_0051fc80);
}
