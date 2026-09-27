// Decompiled by Haiku. Names are provisional.

extern void* __cdecl FUN_004d83b0(char*, int);

extern char DAT_0050a430[];

// FUNCTION: 0x4ba5c0
int __stdcall FUN_004ba5c0(int param_1)
{
    void* result = FUN_004d83b0(DAT_0050a430, 0x10000);
    *(void**)((int)param_1 + 0xc0) = result;
    return 1;
}
