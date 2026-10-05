// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x49adf0
int __stdcall FUN_0049adf0(int param1, unsigned int param2)
{
    unsigned char idx = (unsigned char)param2;
    int offset = idx * 7;
    int* intermediate = (int*)((char*)param1 + offset * 4 + 0x10);
    int result = *(int*)((char*)*intermediate + 0xdc);
    return result;
}
