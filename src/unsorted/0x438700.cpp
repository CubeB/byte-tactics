// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x438700
unsigned int __stdcall FUN_00438700(void* param_1, void* param_2, unsigned int param_3)
{
    if ((*(unsigned char*)((char*)param_1 + 0x10f) & 1) == 0) {
        *(unsigned int*)((char*)param_2 + 6) = param_3 | 4;
        return 2;
    }
    return 1;
}
