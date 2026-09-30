// Decompiled by Claude Opus 5.5, finished by space-bunny-free. Names are provisional.
// Verified by GPT-6.1-sol for #1705: best retained score 91.1%; not a MATCH.
// Partial (91.1%): a Park-Miller random number generator (seed * 16807 mod
// 2^31 - 1, with q = seed / 127773 to avoid overflow), then seed % range.
//
// space-bunny-free, 2026-09 (second pass, #1901): still 91.1%, still not a
// MATCH, and the two halves of the problem are now known to be exclusive:
//
//   * Every spelling with a REAL multiply for q * 2147483647 gives exactly
//     103 bytes / 57.5%, and it always looks the same: the lea chain for
//     seed * 16807 is hoisted above the division and its result is moved
//     into esi, so the store to DAT_0051fc88 slips past the div. Tested and
//     all 57.5%: q * 2147483647 with the temp in either position, the plain
//     statement, both constants (2147483647 / 2147483649 / 0x7fffffff /
//     0x7FFFFFFFu / 2147483647u), the negative constant (q * -2147483647),
//     the whole product written as s*16807 - (s/127773)*2147483647 in one
//     statement, a signed q, a signed s with unsigned casts, separate
//     s0/a/b temps, an s local distinct from the seed, reading DAT_0051fc88
//     twice (the div off the global), both while(1)+break and do/while(0)
//     and an if-wrapped assignment, declaring the locals before the early
//     return, and a sheared-up tail. That is not a spelling problem, it is
//     MSVC's interleave test: the multiply node's worth keeps the two trees
//     from being interleaved, so the whole lea chain is generated first.
//
//   * The shift spelling reproduces the original's SCHEDULE and REGISTER
//     ALLOCATION exactly (div first, seed in esi, chain in eax, interleaved,
//     store before the div): both
//         s = s * 16807 - ((q << 31) - q);      (99 bytes)
//         s = s * 16807 - (q << 31) - q;        (99 bytes)
//     score 91.1% and differ from the original only in the three
//     instructions of q * 2147483647:
//         original   mov edx,ecx; neg edx; shl edx,31; sub edx,ecx; sub eax,edx
//         this file   mov edx,ecx;       shl edx,31;       sub eax,edx; sub eax,ecx
//     i.e. the original materialised the product in edx (MSVC's expansion of
//     a real multiply) while the shift form folds it into the eax tree.
//
// So what is needed is the multiply's codegen with the shift form's
// interleaving. Diagnostics that bear on that:
//   * B = q * 4 / q * 65536 / q * 1073741824: the div is generated first,
//     then all of B, then all of A, no interleaving. So a B that lowers to
//     one instruction does not interleave; a B that is a shift under a
//     commutative + does (that is the shape that matches here).
//   * Writing the neg explicitly does not help: (0 - q) << 31, -q << 31,
//     q * -2147483648, 0 - ((q<<31) + q) all fold straight back into the
//     multiply and give 57.5%.
//   * headers.py --cpp (768 sets) on the multiply form: every set 57.5%.
//
// deepseek-v4.1-flash, 2026-09: confirmed that the neg IS obtainable. Both a
// real multiply (`-q * 2147483647`, or a temp `unsigned int t = (q << 31) - q;`)
// compile to exactly the original's block
//     mov edx, ecx; neg edx; shl edx, 31; sub edx, ecx
// so the original source was almost certainly `seed * 16807 - q * 2147483647`
// (the temp merely blocks MSVC from distributing the -q into the outer sub).
// The catch is coupled: materialising the product flips the whole allocation.
// The lea chain for seed * 16807 then becomes hoistable into esi (the mul does
// not clobber esi), so seed lands in ecx and the final store moves after the
// div (57.5% for every such spelling, 83 bytes for `s *= 16807` forms). Only
// the folded shift form keeps seed in esi and the chain in eax, which is the
// part that matches. Same result for `-q * 0x7fffffff`, Schrage's identity,
// signed/unsigned q, and t declared at function scope.
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
