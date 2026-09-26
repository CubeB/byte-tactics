// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x4b6af0
int __stdcall FUN_004b6af0(int param_1, int param_2)
{
    int ecx = 0;
    int edx = 0;

    while (edx != param_2) {
        unsigned char al = *(unsigned char*)(ecx + param_1);
        if (al == 0) {
            edx++;
            ecx++;
        } else if (al == 0x0a) {
            edx++;
            ecx++;
        } else {
            ecx++;
        }
    }

    return ecx + param_1;
}
