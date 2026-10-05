// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4d1ba0
int __stdcall SquashDecrypt(unsigned char* param_1, unsigned int param_2)
{
    unsigned int i = 0;
    if (param_2 > 0) {
        do {
            unsigned char dl = param_1[0];
            dl = dl - (unsigned char)i;
            dl = dl ^ (unsigned char)i;
            param_1[0] = dl;
            param_1++;
            i++;
        } while (i < param_2);
    }
    return 0;
}
