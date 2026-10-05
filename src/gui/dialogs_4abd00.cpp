// Decompiled by Haiku. Names are provisional.

extern void __stdcall FUN_004a0300(int, int, int);
extern char DAT_00502ae8;

// FUNCTION: 0x4abd00
void __stdcall FUN_004abd00(int param_1)
{
    int edx = *(int*)(param_1 + 0x18);
    FUN_004a0300(*(int*)(edx + 4), *(int*)(param_1 + 0x60), (int)&DAT_00502ae8);
}
