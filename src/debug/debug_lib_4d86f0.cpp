// Decompiled by Haiku. Names are provisional.

void __cdecl ProtectPages(int param_1, int param_2, int param_3);

// FUNCTION: 0x4d86f0
void __cdecl ProtectPagesReadWrite(int param_1, int param_2)
{
    ProtectPages(param_1, param_2, 4);
}
