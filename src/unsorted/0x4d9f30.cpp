// Decompiled by Haiku. Names are provisional.

extern int DAT_005289c4;

// FUNCTION: 0x4d9f30
int __stdcall FUN_004d9f30(int param_1, int param_2, int param_3)
{
    int eax = param_2;
    eax = eax - 0;
    if (eax == 0) {
        return 1;
    }
    eax = eax - 1;
    if (eax != 0) {
        return 1;
    }
    DAT_005289c4 = param_1;
    return 1;
}
