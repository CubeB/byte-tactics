// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x40cc40
unsigned int* __stdcall FUN_0040cc40(unsigned int* param_1, unsigned int* param_2, unsigned int* param_3)
{
    if (param_1 != param_2) {
        do {
            if (param_3 != 0) {
                param_3[0] = param_1[0];
                param_3[1] = param_1[1];
            }
            param_1 += 2;
            param_3 += 2;
        } while (param_1 != param_2);
    }
    return param_3;
}
