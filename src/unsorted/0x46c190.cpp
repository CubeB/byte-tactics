// Decompiled by space-bunny-free. Names are provisional.
#include <windows.h>

struct Game_0046c190 {
    char unknown_0[0x14];
    char field_14[1];
};

struct ElemA_0046c190 {
    char unknown_0[0x14];
    void* field_14;
};

struct ElemB_0046c190 {
    char unknown_0[8];
    void* field_8;
};

extern Game_0046c190* g_game;
extern int DAT_0051e590;
extern HMODULE DAT_0051e58c;
extern void (__cdecl* DAT_0051e558)();
extern ElemA_0046c190** DAT_0051e574;
extern ElemB_0046c190** DAT_0051e57c;
extern void** DAT_0051e550;

void __stdcall FUN_004c9b70(void* p);
void __stdcall FUN_004caa20(int param_1, int param_2);
void FUN_004d85a0(void* p);

// FUNCTION: 0x46c190
void FUN_0046c190()
{
    DAT_0051e590 = 0;
    FUN_004caa20(0, 0);
    if (DAT_0051e58c != 0) {
        FUN_004c9b70(g_game->field_14);
        if (DAT_0051e558 != 0)
            DAT_0051e558();
        FreeLibrary(DAT_0051e58c);
        DAT_0051e58c = 0;
    }
    for (int i = 0; i < 10; i++) {
        if (DAT_0051e574 != 0) {
            FUN_004d85a0(DAT_0051e574[i]->field_14);
            FUN_004d85a0(DAT_0051e574[i]);
        }
        if (DAT_0051e57c != 0) {
            FUN_004d85a0(DAT_0051e57c[i]->field_8);
            FUN_004d85a0(DAT_0051e57c[i]);
        }
        if (DAT_0051e550 != 0) {
            FUN_004d85a0(DAT_0051e550[i]);
        }
    }
    if (DAT_0051e574 != 0) {
        FUN_004d85a0(DAT_0051e574);
        DAT_0051e574 = 0;
    }
    if (DAT_0051e57c != 0) {
        FUN_004d85a0(DAT_0051e57c);
        DAT_0051e57c = 0;
    }
    if (DAT_0051e550 != 0) {
        FUN_004d85a0(DAT_0051e550);
        DAT_0051e550 = 0;
    }
}
