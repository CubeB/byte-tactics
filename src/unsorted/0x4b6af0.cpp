// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4b6af0
int __stdcall FUN_004b6af0(int param_1, int param_2)
{
    int ecx = 0;
    int edx = 0;

    while (true) {
        if (param_2 == edx) break;

        unsigned char al = *(unsigned char*)(ecx + param_1);
        if (al != 0x00 && al != 0x0a) {
            ecx++;
            continue;
        }
        edx++;
        ecx++;
    }

    return ecx + param_1;
}
