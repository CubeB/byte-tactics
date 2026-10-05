// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4d1b70
int __stdcall SquashEncrypt(char* param1, unsigned int param2)
{
    unsigned int i = 0;
    while (i < param2) {
        unsigned char dl = *param1;
        dl ^= i;
        dl += i;
        *param1 = dl;
        param1++;
        i++;
    }
    return 0;
}
