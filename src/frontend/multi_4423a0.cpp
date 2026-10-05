// Decompiled by Haiku. Names are provisional.

extern int DAT_00512770;

// FUNCTION: 0x4423a0
void __stdcall FUN_004423a0(int param_1, int* param_2)
{
    int val = (int)*(short*)((char*)param_2 + 0xba);
    if (val >= 0) {
        DAT_00512770 = val + 1;
    }
}
