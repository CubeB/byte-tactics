// Decompiled by Haiku. Names are provisional.

extern void __cdecl ProtectPages(int, int, int);

// FUNCTION: 0x4d8690
void __cdecl ProtectPagesReadOnly(int param_1, int param_2)
{
    ProtectPages(param_1, param_2, 2);
}
