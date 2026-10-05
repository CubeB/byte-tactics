// Decompiled by Opus. Names are provisional.

extern int __stdcall AccessRegistryValue(void*, void*, void*, void*, int, int);

// FUNCTION: 0x4b69b0
int __stdcall ReadRegistryData(void* param_1, void* param_2, void* param_3, void* param_4)
{
    return AccessRegistryValue(param_1, param_2, param_3, param_4, 0, 1);
}
