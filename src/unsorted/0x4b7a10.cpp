// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x4b7a10
int __stdcall FUN_004b7a10(char* param_1, int param_2)
{
    for (int i = 0; i < param_2; i++) {
        if (param_1[i] == '\n') {
            return i;
        }
    }
    return param_2;
}
