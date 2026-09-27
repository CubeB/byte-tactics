// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x439cf0
int __stdcall FUN_00439cf0(void* param_1)
{
    int eax;
    if (param_1 == 0) {
        eax = 0;
    } else {
        int* field_5c = *(int**)((char*)param_1 + 0x5c);
        if (field_5c == 0)
            eax = 0;
        else
            eax = *(int*)((char*)field_5c + 0x42);
    }
    if ((eax & 8) != 0)
        return *(int*)((char*)*(int**)((char*)param_1 + 0x5c) + 0x16);
    return 0;
}
