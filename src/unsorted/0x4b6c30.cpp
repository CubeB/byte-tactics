// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5, finished by space-bunny-free, finished by DeepSeek V4.1 Flash. Names are provisional.
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
// space-bunny-free (issue 4469): 95.0% kept, still no MATCH. What is new:
// (a) The original's block is MSVC's own expansion of a real multiply by
// 2147483647. A probe, `unsigned int f(unsigned int q) { return q*2147483647u; }`,
// compiles to exactly `mov eax,ecx; neg eax; shl eax,31; sub eax,ecx` (the neg
// before the shl is a no-op MSVC emits), and one probe
// `a * 16807 - q * 2147483647u` gives that block followed by `sub eax,edx`,
// exactly the original's shape. So the original source was almost certainly the
// plain `seed = seed * 16807 - (seed / 127773) * 2147483647;`.
// (b) That plain form with <windows.h> (which is what the lea chain needs) is
// 57.5%: the division expansion, the whole lea chain and the correction block
// come out byte-identical to the original, and the only difference is the
// schedule and the allocation. Ours runs the lea chain before the division
// (seed in ecx, product in esi, the store and the `mov eax,esi` for the div
// after it); the original interleaves the two (seed in esi, product in eax, the
// quotient fixup in ecx, the store before the div).
// (c) The two halves cannot be combined from the source. Shapes that do reach
// the original's schedule (`unsigned int product = seed * 16807; seed = product;
// seed = seed - correction;` and the `seed = seed*16807; seed -= correction;`
// forms) put the division first exactly as the original, but then the product
// shares the seed's register and MSVC compiles 16807 to a single `imul`
// (83 bytes, 33.8%). The retained redundant `seed / 127773` (or any second
// division node, e.g. `+ seed/127773 - q`) does give the schedule, the
// allocation and the byte count exactly, but MSVC's flattening pass then
// rewrites the correction into `add ecx,edx; add eax,ecx`. The trigger is
// specifically a second division node: pads made of `q - q`, `seed - seed`, a
// second `* 16807` or `q*2 - q*2` all leave the 57.5% schedule.
// (d) Sweeps, all flat: dummy `extern int` 0..3000 step 8, dummy prototypes
// 0..6000 step 8 and dummy function definitions 0..600 step 4 on the plain
// form; prototypes 0..4000 step 16 on this file; headers.py --cpp (768 sets) on
// both this file and the plain form, best 95.0% here and 57.5% there; the
// unpatched msvc5-rtm compiler gives the same two scores. tools/permute.py:
// 8.4 min / 4374 candidates on the plain form reached 67.5% (101 bytes); 5 min /
// 2343 candidates on this file reached nothing.
// (e) Also unchanged: every operand order, `0x7fffffff` and `-2147483647`
// spellings, int and long types, (int) casts, a static helper for the whole
// update or for either part, `old`/`s2` copies of the seed, the quotient read
// from the global before the seed, the global used directly, the extra division
// split into its own statement, all 18 orderings of the four statements
// (quotient, correction, product, subtraction) with a real multiply (57.5% at
// best), and a hand written lea chain (which compiles to the same bytes as the
// multiply).
// Claude Opus 5.5 (found with tools/permute.py): 95.0%, up from 91.1%, the
// right size (101 bytes). The correction term is a named local that divides
// seed by 127773 a second time instead of reusing q. Both halves are needed
// for these bytes: the same expression written inline stays at 91.1%, and the
// named local with q (`(q << 31) - q`) is 83 bytes, 33.8%. <windows.h> is now
// left out: with it MSVC merges the two divisions into one and also gives the
// 83-byte, 33.8% form. What still differs is the sign of the last two operations: the
// original subtracts (`sub edx, ecx; sub eax, edx`), this adds the negated
// term (`add ecx, edx; add eax, ecx`).

extern unsigned int DAT_0051fc88;

// claude-opus-5-5 (#4406): still 95.0%. `q * 0x7fffffff` alone reproduces the
// original's neg / shl 31 / sub correction exactly, but every spelling without
// the second `seed / 127773` (separate correction statement, signed q, int seed,
// 16807 * seed order, an `old` copy) compiles 16807 as one imul and keeps seed
// in ecx (83 bytes, 33.8%). Only the duplicated division keeps seed in esi with
// the lea chain. A 10-minute permuter run (558 candidates) found nothing.
// space-bunny-free (issue 4516, retry): 95.0% kept, still no MATCH. What is
// new is a map of the three code shapes this function falls into, and evidence
// that the shape is decided by the source form plus the include, not by the
// compiler heap: 0 to 3000 dummy `extern int fN();`, 0 to 300 dummy static
// definitions and 0 to 300 dummy static prototypes are completely flat on all
// three (the earlier notes blamed heap state).
//   A (95.0%, 101 bytes, the original's schedule and allocation): the
//     correction is a SUBTRACTION with a second division node, e.g.
//     `unsigned int c = (q << 31) - seed / 127773;` with no <windows.h>. The
//     lea chain for 16807 is there, interleaved with the division, seed in esi
//     and the product in eax, and the diff is always exactly these two lines:
//     the original has `sub edx, ecx; sub eax, edx`, we get
//     `add ecx, edx; add eax, ecx`. Every spelling that reaches A (about 40
//     of them) differs in those same two instructions and nothing else.
//   B (57.5%, 103 bytes): a real multiply for the correction plus
//     <windows.h> (the plain Park-Miller line lands here). Right instructions
//     including the neg / shl 31 / sub block, but the whole lea chain is
//     hoisted before the division, seed in ecx, the product in esi, and the
//     store moves after the div.
//   C (33.8%, 83 bytes): a real multiply for the correction and no
//     <windows.h>. The schedule is the original's (division first, the
//     interleave kept) and the tail is the original's neg / shl 31 / sub, but
//     16807 becomes one `imul ecx, ecx, 0x41a7` and seed and product share
//     ecx.
// The original is A's schedule with B/C's tail, a fourth combination that no
// spelling has reached. A cannot have B/C's tail because A's tree is the
// flattened sum `product + (q - (q << 31))`, and the two adds are the only
// way to emit that, while the original's tree is
// `product - (q * 0x7fffffff)` with the multiply still a multiply node at
// codegen time, which the backend expands late into
// `mov edx, ecx; neg edx; shl edx, 31; sub edx, ecx`. A real multiply in the
// source leaves state A, and A's shape loses the multiply, so from the source
// as written the two requirements cannot both be met.
// Also tried this round, all flat or worse: signed and long mixes for the
// quotient, the correction and the seed (13 spellings, every one that reaches
// A has the identical two line diff); hybrid corrections holding both a
// multiply and a subtraction (`q * 2147483647u - d2 + q` and 14 more, 15
// spellings, all 95% with the same two line diff); a `static inline` helper
// for the correction, for the division or for the whole update (best 59.7%,
// 93 bytes); the update inside `do { } while (0)`, a bare block,
// `if (range > 1) ... else` and while / for guards; dead stores and dead
// loops (`int t = 0; if (t) ...`, a dead for, `if (range & 0)`, a null
// pointer test, a static flag) between the quotient and the correction, which
// drop it to 43.6% (99 bytes) but never change the tail; a static global used
// as a store-and-reload barrier around the multiply (33.3%, 89 bytes); a hand
// written lea chain for 16807 next to a multiply correction (MSVC folds it
// back and gives C); all 120 orders of the statements {quotient, second
// division, correction, product, subtraction} with the locals declared up
// front, for a shift and for a multiply correction (16 orders reach A, all
// with the same two line diff, the multiply family never reaches A); and about
// 2500 randomised combinations of include, helper, quotient, correction, final
// statement and pad. tools/permute.py: 11.9 min / 3806 candidates on the
// plain Park-Miller spelling only reached 67.5% (it walks back into state B,
// the 101-byte chain-hoisted form); 5 min on this file found nothing.
// A parallel scorer that runs check.py over a list of generated variants at
// about 16 per second was in build/scratch/0x4b6c30/fast.py of that worktree.
// deepseek-v4.1-flash (issue 4516, finishing): 95.0% kept, still no MATCH.
// (a) Step-1 (not step-8) dummy-declaration sweeps, N = 0 to 1200, are flat on
//     all forms: plain (no <windows.h>) stays 33.8%, plain + <windows.h> stays
//     57.5%, the retained shift form stays 95.0%, shift + <windows.h> 33.8%.
//     So the missing combination is not a narrow compiler-heap window (the
//     earlier step-8 sweeps did not miss one).
// (b) A fresh 3-minute tools/permute.py run (3719 candidates) found nothing.
// (c) About 250 variants were tried this round: the second division moved
//     inside the correction statement next to a real multiply (V1/V2/W family,
//     all fall back to plain 57.5/33.8), named-local cancelling forms
//     (`c = q*C + seed/127773 - q` and 20 more), the multiply spelled with
//     negative constants (0x80000001, -2147483647, -0x7fffffff), and register
//     storage on seed/q. None changed the outcome.
// (d) The one interesting near miss: `c = (seed/127773) * -2147483647;
//     seed = seed*16807 - c;` scores 72.2% (101 bytes) without <windows.h>.
//     MSVC keeps a real multiply node, the division stays first and seed stays
//     in esi, so the correction block `neg; shl 31; sub` appears (in eax, not
//     edx) and the product becomes `imul esi,esi,0x41a7`. It is not usable: the
//     value is p + q*C, the wrong sign, so the emitted tail adds. Writing the
//     correct-sign version folds the double negative and drops back to 33.8% or
//     57.5%. The retained shift form is still the only spelling that keeps the
//     lea chain and the original schedule; its two-instruction tail (`sub edx,
//     ecx; sub eax, edx` vs our `add ecx, edx; add eax, ecx`) remains.
// FUNCTION: 0x4b6c30
int __stdcall FUN_004b6c30(int range)
{
    if (range < 2)
        return 0;

    unsigned int seed = DAT_0051fc88;
    unsigned int q = seed / 127773;
    unsigned int correction = (q << 31) - seed / 127773;
    seed = seed * 16807 - correction;
    if ((int)seed <= 0)
        seed += 2147483647;
    DAT_0051fc88 = seed;
    return seed % range;
}
// GPT-6.1-sol refinement (issue 3121): rechecked the retained shift form; 91.1% remains the best. The only difference is the quotient correction sequence and the resulting short-branch offset.