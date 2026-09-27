// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4b6820
int __stdcall FUN_004b6820(int param_1, int param_2)
{
    int cl = *(char*)(param_1 + 1);
    int bl = *(char*)(param_2 + 1);
    return cl == bl ? 1 : 0;
}
