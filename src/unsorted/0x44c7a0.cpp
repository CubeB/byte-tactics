// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x44c7a0
int __cdecl FUN_0044c7a0(unsigned char* param_1, unsigned char* param_2)
{
    unsigned char c1, c2;

    while (true) {
        c2 = param_2[0];
        c1 = param_1[0];
        if (c1 != c2) break;
        if (c1 == 0) return 0;

        c2 = param_2[1];
        c1 = param_1[1];
        if (c1 != c2) break;
        param_1 += 2;
        param_2 += 2;
        if (c1 == 0) return 0;
    }

    return c1 < c2 ? -1 : 1;
}
