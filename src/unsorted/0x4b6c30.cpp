// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// Retry (deepseek-v4.1-flash, 2026-10): confirmed the neg/shl/sub target block
// appears for EVERY spelling whose correction is a real multiply tree, inline
// or materialised: ((-q)<<31)-q, q*0x7fffffff, q*-2147483647, -q*2147483648,
// (0u-q)<<31, q*0x80000000+q, and a left-operand correction. All of them
// produce the correct 11-byte block but hoist the seed*16807 lea chain before
// the division and take ecx for seed / esi for the product (103 bytes, 57.5%).
// The folded shift form ((q<<31)-q) keeps seed in esi and the chain in eax but
// distributes the subtraction (99 bytes, 91.1%, retained). A dummy-declaration
// sweep 0..400 (step 8) on the neg form is flat at 57.5%, so compiler state is
// not the tie. The mismatch is the single allocation/scheduling tie below.
// Verified by GPT-6.1-sol for #1705: best retained score 91.1%; not a MATCH.
// Partial (91.1%): a Park-Miller random number generator (seed * 16807 mod
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
// GPT-6.1-sol refinement: tried the direct negative-shift form `-q << 31` and an
// algebraic dependency form `(seed+q)*16807-q*16807` to make the seed product wait
// for q. Both preserved the wrong evaluation schedule and scored 57.5%; writing the
// equivalent final update as `seed*16807 + (q<<31) + q` did too. Baseline retained.
// GPT-6.1-sol refinement (issue 2309): baseline check remained 91.1%. A fresh
// correction local `q * 0x80000001u` scored 57.5% by moving seed*16807 before
// quotient calculation; restored this 91.1% version. The remaining mismatch is
// the neg/shl/sub ordering for the quotient correction and the following branch.
// deepseek-v4.1-flash, 2026-10 (finishing): materialising the correction, with
// either `corr = q * 2147483647;` or `corr = (q << 31) - q;` and then
// `seed = seed * 16807 - corr;`, emits the exact target block
//     mov edx, ecx; neg edx; shl edx, 0x1f; sub edx, ecx; sub eax, edx
// so the `neg` does not need a real multiply, only a materialised value. The
// catch stays the same: materialising flips seed into ecx and the product into
// esi, computed before the division's mul (103 bytes, 57.5%). The inline shift
// form keeps seed in esi and the product in eax (99 bytes, 91.1%) but MSVC
// distributes the subtraction instead. Statement-reuse (t = q*C; t = seed*16807
// - t;), `long`/`int` types, a helper for the quotient or the correction, and
// the global read directly for the product all give the identical 103-byte /
// 57.5% form. Sweeps: headers.py 768 sets and 0 to 10000 unused prototypes are
// flat at 91.1%, so this is the compiler-state tie the guide describes, not a
// source spelling.
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
// GPT-6.1-sol refinement (issue 3121): rechecked the retained shift form; 91.1% remains the best. The only difference is the quotient correction sequence and the resulting short-branch offset.
