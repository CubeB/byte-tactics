// Decompiled by Opus. Names are provisional.
// Reads a 4-byte value through FUN_004b6880 (same family as 0x4b69b0).

extern int __stdcall FUN_004b6880(void*, void*, void*, void*, int, int);

// FUNCTION: 0x4b69d0
int __stdcall FUN_004b69d0(void* param_1, void* param_2, void* param_3)
{
    int size = 4;
    return FUN_004b6880(param_1, param_2, param_3, &size, 0, 1);
}
