// Decompiled by Opus. Names are provisional.
// Fills n dwords with a value (STL uninitialized_fill_n shape), like 0x40d5b0.

// FUNCTION: 0x40d580
void __stdcall FUN_0040d580(int* param_1, unsigned int param_2, int* param_3)
{
    for (; param_2 > 0; param_2--) {
        if (param_1 != 0) {
            *param_1 = *param_3;
        }
        param_1++;
    }
}
