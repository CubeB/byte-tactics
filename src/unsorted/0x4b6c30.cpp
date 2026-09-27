// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial (57.5%): a Park-Miller random number generator.
//   if (range < 2) return 0;
//   seed = seed * 16807 - (seed / 127773) * 2147483647;
//   if ((int)seed <= 0) seed += 2147483647;
//   return seed % range;
// The arithmetic and instruction sequence match, but MSVC 5 keeps the result
// in esi (callee-saved) here, so it stores the global after the final div and
// adds a "mov eax, esi" before it, while the original keeps the result in eax
// (store before div, no move). ~40 source variants (one/two locals, global
// direct, comma, signedness, helper inlining, preceding function 0x4b6ba0,
// /G3-/G6, /O1/Os/Ox, RTM vs SP3) all produce the esi form. The header set
// decides only whether the *16807 becomes a lea chain (<windows.h>, matching)
// or an imul. The remaining difference looks like compiler state of the
// original translation unit, not source.
#include <windows.h>

extern unsigned int DAT_0051fc88;

// FUNCTION: 0x4b6c30
int __stdcall FUN_004b6c30(int range)
{
    if (range < 2)
        return 0;

    unsigned int seed = DAT_0051fc88;
    seed = seed * 16807 - (seed / 127773) * 2147483647;
    if ((int)seed <= 0)
        seed += 2147483647;
    DAT_0051fc88 = seed;
    return seed % range;
}
