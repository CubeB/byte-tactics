// Decompiled by Haiku. Names are provisional.

extern void __stdcall AccessRegistryValue(void*, void*, void*, void*, int, int);

// FUNCTION: 0x4b6a00
void __stdcall WriteRegistryBinary(void* param_1, void* param_2, void* param_3, int unused)
{
    AccessRegistryValue(param_1, param_2, param_3, (void*)&unused, 3, 0);
}
