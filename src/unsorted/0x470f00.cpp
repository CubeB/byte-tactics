// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x470f00
int* __stdcall FUN_00470f00(int* param_1, int* param_2, int* param_3)
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
