// Decompiled by Haiku. Names are provisional.

extern void* DAT_005119c0[];

// FUNCTION: 0x40c200
int __stdcall FUN_0040c200(int param_1, unsigned int param_2)
{
    unsigned int idx = param_2 & 0xffff;
    void* p = DAT_005119c0[param_1];
    int* ptr = (int*)((char*)p + 0x81);
    short* arr = (short*)(*ptr);
    return (int)arr[idx];
}
