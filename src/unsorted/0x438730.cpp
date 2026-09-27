// Decompiled by Opus. Names are provisional.

// FUNCTION: 0x438730
int __stdcall FUN_00438730(char* param_1, char* param_2, unsigned int param_3)
{
    if (*(unsigned char*)(param_1 + 0x10f) & 2) {
        *(unsigned int*)(param_2 + 6) = param_3 | 4;
        return 2;
    }
    return 1;
}
