// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x406c10
unsigned int* __stdcall FUN_00406c10(unsigned int* param_1, unsigned int* param_2, unsigned int* param_3)
{
    if (param_1 != param_2) {
        do {
            if (param_3 != 0) {
                *param_3 = *param_1;
            }
            param_1++;
            param_3++;
        } while (param_1 != param_2);
    }
    return param_3;
}
