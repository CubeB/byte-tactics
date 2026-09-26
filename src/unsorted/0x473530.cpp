// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x473530
void __stdcall FUN_00473530(int* param_1, unsigned int param_2, int* param_3)
{
    for (; param_2 > 0; param_2--) {
        if (param_1 != 0) {
            *param_1 = *param_3;
        }
        param_1++;
    }
}
