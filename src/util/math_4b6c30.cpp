// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5, finished by space-bunny-free, finished by DeepSeek V4.1 Flash. Names are provisional.
// FLAGS: /Gi
// A Park-Miller random number generator (seed * 16807 mod 2^31 - 1, with
// q = seed / 127773 to avoid overflow), then seed % range.

// No <windows.h>: with it the two divisions below merge into one.
extern unsigned int g_randomSeed;

// Stays in its own file: it matches only with /Gi, and that per-file flag
// moves DotProduct and CrossProduct in src/util/math.cpp.
// FUNCTION: 0x4b6c30
int __stdcall RandomInt(int range)
{
    if (range < 2)
        return 0;

    unsigned int seed = g_randomSeed;
    unsigned int q = seed / 127773;
    // Divides seed a second time instead of reusing q.
    unsigned int correction = (q << 31) - seed / 127773;
    seed = seed * 16807 - correction;
    if ((int)seed <= 0)
        seed += 2147483647;
    g_randomSeed = seed;
    return seed % range;
}
// GPT-6.1-sol refinement (issue 3121): rechecked the retained shift form; 91.1% remains the best. The only difference is the quotient correction sequence and the resulting short-branch offset.
