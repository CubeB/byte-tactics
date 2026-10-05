// Decompiled by Haiku. Names are provisional.

extern void __cdecl FUN_004d85a0(int);

// FUNCTION: 0x4afcf0
void __stdcall FUN_004afcf0(int param_1)
{
    int ptr = *(int*)(param_1 + 0xa6);
    FUN_004d85a0(ptr);
    *(int*)(param_1 + 0xa6) = 0;
    *(int*)(param_1 + 0xae) = 0;
}
