// Decompiled by Claude Opus 5.5. Names are provisional.
// Partial (86.1%): a Park-Miller random number generator (seed * 16807 mod
// 2^31 - 1, with q = seed / 127773 to avoid overflow), then seed % range.
//
// Writing q * 2147483647 as the shift form (q << 31) - q gets the order and
// registers right: the division comes first, seed stays in esi, the result is
// built in eax and stored before the final div. What still differs is the
// three instructions for q * 2147483647: the original has
//     mov edx, ecx; neg edx; shl edx, 31; sub edx, ecx; sub eax, edx
// which is MSVC's own expansion of a real multiply, while this file gives
//     mov edx, ecx; shl edx, 31; sub eax, edx; add eax, ecx
//
// The plain form `seed = seed * 16807 - q * 2147483647` (57.5%) gives exactly
// the original's instructions, but MSVC then evaluates seed * 16807 before
// the division, so the product lives in esi and the store moves after the
// div. Every spelling that keeps a real multiply by 2147483647 (q in its own
// statement, inline helpers for either product or the division, Schrage's
// 16807 * (s - q * 127773) - 2836 * q, which MSVC folds to the same tree,
// signed/unsigned types, the global used directly) gives that same order.
// Spellings that make the product an in-place statement (`s *= 16807;`)
// keep the division first but turn the lea chain into imul.
//
// <windows.h> is needed for the lea chain in seed * 16807: without it (or
// with only some of it) MSVC emits imul instead. This is compiler heap
// state, not the header's contents: 2700 to 5400 unused prototypes in place
// of <windows.h> flip it the same way. Defining the preceding functions of
// the file (0x4b69b0 to 0x4b6ba0) changes nothing.
#include <windows.h>

extern unsigned int DAT_0051fc88;

// FUNCTION: 0x4b6c30
int __stdcall FUN_004b6c30(int range)
{
    if (range < 2)
        return 0;

    unsigned int seed = DAT_0051fc88;
    unsigned int q = seed / 127773;
    seed = seed * 16807 - ((q << 31) - q);
    if ((int)seed <= 0)
        seed += 2147483647;
    DAT_0051fc88 = seed;
    return seed % range;
}
