// Decompiled by Haiku. Names are provisional.

extern int* g_game;

// FUNCTION: 0x474f80
int __fastcall FUN_00474f80(int param_1)
{
    int edx = *(int*)(param_1 + 0x10);
    int eax;

    if (edx == 0) {
        eax = 0;
    }
    else {
        eax = *(int*)(param_1 + 0x14) - edx;
        eax = eax >> 5;
    }

    unsigned char dl = (eax == 0) ? 1 : 0;
    if (dl != 0) {
        eax = *(int*)(param_1 + 4);
        int* ecx = g_game;
        unsigned int val = *(unsigned int*)((char*)ecx + 0x38a47);
        if ((unsigned int)eax < val) {
            eax = 1;
            return eax;
        }
    }

    eax = 0;
    return eax;
}
