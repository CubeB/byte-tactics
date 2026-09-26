// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x470f30
void __stdcall FUN_00470f30(int* param_1, unsigned int param_2, int* param_3)
{
    while (param_2 > 0) {
        if (param_1 != 0) {
            *param_1 = *param_3;
        }
        param_1++;
        param_2--;
    }
}
