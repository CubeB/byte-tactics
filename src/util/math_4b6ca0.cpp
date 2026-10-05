// Decompiled by Opus. Names are provisional.
// Seeds a random number generator (the seed is kept odd).

extern unsigned int g_randomSeed;

// FUNCTION: 0x4b6ca0
void __stdcall SeedRandom(unsigned int seed)
{
    g_randomSeed = (seed ^ 0x66e29572) | 1;
}
