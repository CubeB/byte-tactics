// Decompiled by Haiku. Names are provisional.
extern void __cdecl ProtectBlock(void*, int);

// FUNCTION: 0x4d8710
void __cdecl ProtectBlockReadOnly(void* param_1)
{
    ProtectBlock(param_1, 2);
}
