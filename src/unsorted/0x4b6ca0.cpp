// Decompiled by Opus. Names are provisional.
// Seeds a random number generator (the seed is kept odd).

extern unsigned int DAT_0051fc88;

// FUNCTION: 0x4b6ca0
void __stdcall FUN_004b6ca0(unsigned int seed)
{
    DAT_0051fc88 = (seed ^ 0x66e29572) | 1;
}
