// Decompiled by Haiku. Names are provisional.
extern void __cdecl ProtectBlock(void*, int);

// FUNCTION: 0x4d8780
void __cdecl ProtectBlockReadWrite(void* param_1)
{
    ProtectBlock(param_1, 4);
}
