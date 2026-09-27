// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x4b7f30
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2)
{
    int result = 0;
    if (param_2 >= 0 && param_2 < (int)*param_1 && param_1 != 0) {
        result = *(int*)((char*)param_1 + param_2 * 8 + 0x28);
    }
    return result;
}
