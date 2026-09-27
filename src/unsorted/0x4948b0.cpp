// Decompiled by Haiku. Names are provisional.

extern char DAT_0051f2c8[];
extern char DAT_0051e810[];

// FUNCTION: 0x4948b0
void __stdcall FUN_004948b0(int param_1, int param_2)
{
    if (param_1 >= 0) {
        DAT_0051f2c8[param_1] = 0x1e;
    }
    if (param_2 >= 0) {
        DAT_0051e810[param_2] = 0x1e;
    }
}
