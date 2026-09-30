// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, GPT-6.1-sol and space-bunny-free. Names are provisional.
// GPT-6.1-sol refinement after PR #2138: single-use output helper,
// address-taken at_low/at_high, and inline global getter all held at 78.9%;
// no score gain. Cases 0-2 still differ in argument register rotations.
// GPT-6.1-sol follow-up: six checks kept 78.9% (280/280 bytes). A forwarding
// helper and neutral arithmetic did not alter the best code. Case 0, 1 and 2
// argument register scheduling still differs.
// #1595 retry by Codex / GPT-6.1-sol: checkall reconfirmed 78.9% (280/280 bytes), no MATCH.
//
// Eighth pass (space-bunny-free): no score change, 78.9% (280 bytes), 1 check.py
// run. Still short of a match, but the case 0 rotation is now pinned down from
// both ends, and two earlier "negatives" in this file need qualifying.
//   - The size of the gap is one instruction, and it is only in case 0: the
//     original's `mov eax, ds:[0x51fef0]` is the 5-byte accumulator form while
//     ours is the 6-byte `mov edx, ds:[0x51fef0]`, because cl 5 only uses the
//     accumulator form for eax. That single byte is why every `je`/`ja` target
//     in ours is 0x4c71d2 where the original says 0x4c71d1, and why the
//     jump table lands at 0x4c71d9 instead of 0x4c71d8. Cases 1, 2 and 3 are
//     the same size as the original, so a case 0 match is worth chasing alone.
//   - The "g local" spellings are NOT all equal. `int g = DAT_0051fef0;` as a
//     real local (before or after the store) makes cl 5 forward the value
//     still sitting in ecx into the third argument: the reload disappears
//     entirely, the block becomes `push ecx; sub ecx, eax; push ecx; ...`,
//     and the third push vanishes. Only writing DAT_0051fef0 inline in the
//     argument keeps the reload. So the original's reload is cl 5 refusing to
//     forward across `mov [esi], 0`, which is an aliasing decision, and the
//     inline global is the only spelling that reproduces it.
//   - New spellings measured this pass, all byte-identical to the body below
//     (at_high hoisted into eax, the global reload into edx, so no diff at
//     all): `Range& out` (with `out.low`), `unsigned at_low/at_high`,
//     `unsigned value` too, `int* out` with `out[0]`/`out[1]`, an explicit
//     `default: return;` before case 3, `(int)` casts on both the first
//     argument and the global, `int d = size - offset;`, and `int g =
//     DAT_0051fef0;` placed after the store. Using `hi - value` in place of
//     the `offset` local restructures the block (as earlier passes found).
//   - What is left is one fact about cl 5's front end, not a missing value:
//     in the emitted IR of this body the first argument (the at_high load) is
//     walked and register-allocated BEFORE the third argument (the global
//     reload), so it takes the register that `sub ecx, eax` frees. The
//     original allocates in the opposite order, which is why the global lands
//     in the freed eax and at_high only finds a register after `push ecx`.
//     Nothing in the source spelling tried so far (locals, references, casts,
//     wrappers, unsigned, aliases) changes that walk order, so the next pass
//     should look for a source shape that changes the argument list's tree
//     shape rather than its spelling, e.g. a helper whose parameters the
//     inliner re-orders, or a case body that is not a plain call statement.
// The attempted if/else rewrite scored 60.4%; restored the earlier best with the jump table.
// GPT-6.1-sol follow-up: an if/else chain replacing the jump-table switch scored 60.4%; restored the prior 78.9% best.

//
// Seventh pass (deepseek-v4.1-flash): no score change, 78.9% (280 bytes). New
// negative results, so the next attempt can skip them: case 0's store through
// an `int& low = out->low` reference (the 0x41ba60 lever) does not stop the
// at_high hoist; neither does nesting the store as `(out->low = 0, FUN(...))`,
// nor locals for the distance/global/first argument declared after the store.
// Using the local `size` as case 0's third argument (with or without a `d`
// local) scores 276 bytes / 77.8% because the compiler then pushes ecx before
// `sub ecx, eax` and the DAT_0051fef0 reload disappears: the third argument
// really is the global. So the only remaining difference is still the
// register-priority rotation described below.
//
// Sixth pass (deepseek-v4.1-flash): no score change, 78.9% (280 bytes). Confirmed
// the remaining difference is not compiler state nor a flag:
//   - 0, 50, ..., 3000 unused function prototypes before the function: all
//     78.9%. So the earlier flat extern-int sweep is not specific to variable
//     declarations; the source shape is what differs.
//   - /Oa, /Ow and /Og through --flags: 78.7 / 78.7 / 78.9. /Oa and /Ow do not
//     produce the original's non-hoisted at_high load either, so the hoist is
//     not an aliasing decision a flag can flip.
//   - a struct-by-value third parameter instead of two int parameters does not
//     even compile in this harness (jumping over an initialiser), not pursued.
// The cleanest reading of the three mismatches is one allocation preference:
// the original always gives the eax freed by `sub ecx, eax` to the THIRD
// argument (the DAT_0051fef0 reload in case 0). That leaves ecx, still holding
// the second argument, unavailable to the first argument until ecx is pushed,
// so at_high lands late in ecx exactly as the original does. This file gives
// the freed eax to the first argument instead, and the third argument falls to
// edx. Case 2 is the same preference seen as a clean at_low/ecx, global/edx
// swap. The missing lever is whatever makes the third argument win that freed
// eax; it is a scheduling/priority choice, not a missing value.
//
// Fifth pass (space-bunny-free), no score change, 78.9% (280 bytes), 1 real
// check.py run. Two useful results for whoever picks this up:
//   1. The per-case comparator left by the earlier passes
//      (build/scratch/0x4c70d0/cmp.py) REPORTS CASE 3 AS FAILING when it
//      actually matches opcode for opcode. Its norm() rewrites `add esp, 12`
//      to `add esp,0xc` and only afterwards masks hex, so every `add esp`
//      becomes `add esp,AA` and never equals the expected `add esp,0xc`.
//      Collapse the tabs to spaces BEFORE calling norm and every case
//      reports correctly: the honest per-case score of the file below is
//      case 0 ., case 1 ., case 2 ., case 3 O.
//   2. Case 2 is a PURE register swap, nothing else: same instructions, same
//      order, same byte count. The original puts at_low in ecx and the global
//      reload in edx, this file puts at_low in edx and the reload in ecx. In
//      every one of the three failing blocks the FIRST value the original
//      materialises takes ecx (case 0's `sub ecx,eax`, case 2's at_low) and
//      the second takes eax (case 0's global); here case 0 gives that eax to
//      at_high instead and case 2 gives ecx to the global. So one allocator
//      ranking is off by one register across all three blocks, exactly as the
//      earlier passes concluded, and it is NOT the argument order: the push
//      order, the pushed addresses, the store positions and the whole
//      pre-switch are all already correct.
// New spellings tried this pass, every one still 78.9% with case 3 matching:
// an explicit local for each of the three arguments in all six declaration
// orders, a result local, a `Range* o = out` pointer, `(int)` and `+ 0`
// casts, `unsigned` for the global, the store moved after the call, the store
// written twice, `size + (G - G)` for the reload, a ternary reload, self
// assignments (`hi = hi`) as liveness probes, and `int c = DAT_0051fef0`
// placed before or after the `out->low` store. One variant breaks case 3
// (`(hi - value)` or `(value - lo)` spelled out for the second argument:
// the CSE restructures and case 3 stops matching), so do not try those.
// The honest conclusion is the earlier one: a1/a3 materialisation order
// that no source spelling tried so far reaches.
//
// Partial: 78.9% (280 bytes, the original is 280). The search loop, the tail
// arithmetic, the range test, the jump table and case 3 match byte for byte.
// The best form found switches case 0's third argument to DAT_0051fef0, which
// adds the global reload the original has there and keeps case 3 matching.
//
// Third pass (deepseek-v4.1-flash): case 2's third argument is now the global,
// reached through a local (`int c = DAT_0051fef0;`) so the store to out->low
// stops MSVC forwarding the earlier size store into it. That reproduces the
// original's real reload in case 2 (`mov ecx/edx, ds:0x51fef0`) and brings the
// function to the original's 280 bytes at the same 78.9%: the case 2 hunk is
// now only a two-register swap (the original loads at_low into ecx and the
// global into edx; ours does the reverse), with the late at_high load already
// correct. Writing DAT_0051fef0 directly in case 2 (no local) scores 74.6%
// because MSVC then hoists at_high into a register the original keeps free.
//
// The remaining three hunks are all the same shape: the first argument of
// FUN_004b7381 (at_high in cases 0 and 2, at_low in case 1) is loaded early by
// MSVC where the original loads it late, into the register the second argument
// just freed. A diagnostic that keeps offset live past `sub ecx, eax` (an
// extra `DAT_0051fefc = offset;` after the call, which must not be committed)
// makes case 0's schedule exactly the original's (global into eax, pushes, then
// at_high into ecx) but demotes out from esi to edi, so the original did not
// keep offset live that way. That confirms the cause is one scheduler/allocator
// state rather than a missing value: adding any live node flips which argument
// gets the freed register. Explicit argument locals in every order, an
// argument-reference wrapper, high-before-low store order, and the
// case-0-before-case-0 global mix were all rescored and none beat 78.9%.
// Still different, all three a pure scratch-register rotation with the push
// order, the pushed addresses and the store positions already correct:
//   case 0: the original computes `size - offset`, stores `out->low = 0`,
//           then reloads DAT_0051fef0 into eax and loads at_high late into
//           ecx. Here at_high is loaded early into eax and the global reload
//           lands in edx, so the block is rotated by one.
//   case 1: the original loads at_low into edx before any push, here it is
//           loaded into ecx after the first push (order of the two is swapped).
//   case 2: only the at_low/global pair is swapped now (the original puts
//           at_low in ecx and the global reload in edx; this file does the
//           reverse); the at_high load is already late as the original has it.
//
// Measured this pass, all on top of the base 78.7% form unless said:
//   - case 0's third argument = DAT_0051fef0: 78.9%, 276 bytes (this file).
//   - case 0 with `int d = size - offset;` before the store: same 78.9%.
//   - DAT_0051fef0 as the third argument of cases 0 and 2: 74.6%, and it
//     breaks case 3 too (the rotation moves into case 3), so case 2's global
//     was reverted.
//   - the global in all four cases: 74.6%. Global in cases 0, 1 and 2: 74.6%.
//   - explicit locals for the arguments (d, g, bound) and for the result,
//     comma expressions, `out[0]`/`out[1]`, and an `Interp`/`Ref` inline
//     wrapper: no change or worse.
//   - swapping the source order of the arguments via a reversed-argument
//     inline helper: 78.9% (280 bytes) but the same four mismatch hunks.
//   - reordering the case labels in the source: 59.0%.
//   - declaring `offset` before `size`: 32.8% (breaks the pre-switch).
//   - tools/headers.py --cpp: all 768 sets are 78.7%.
//   - defining the neighbour 0x4c71f0 before this function in the same file:
//     74.3% (compiler state does change the code, but not toward the original).
//
// The other three cases stay a register rotation. The sibling 0x4c71f0 has the
// same failure on its case 1; there the original reloads at_high late into ecx
// after two pushes and MSVC 5 hoists it into eax.
//
// Second pass (deepseek-v4.1-flash), all scored with --sym, no improvement on
// 78.9%:
//   - the N-declarations test (0 to 400 unused extern ints) is completely flat
//     at 78.9%, so the difference is source shape, not compiler state. Do not
//     spend time on headers or neighbouring functions.
//   - flags /Gz (72.9%), /Gr (54.9%), /G6 (75.7%), /Ot, /GB, /Ob1: default is
//     best.
//   - a 450-combination sweep over case-body spellings (inline getters/forwards,
//     explicit argument locals in every order, recomputed `hi - lo`, braces,
//     reference wrappers, comma forms, `size` vs DAT_0051fef0 independently per
//     case): every variant with the matching pre-switch stays at or below 78.9%,
//     and case 0's first hunk never changes. The global in all four cases (the
//     most self-consistent reading) is 74.6% because it breaks case 3.
//   - reordering the source cases (0213, 0132, 0312, 1023, 3120) lowers the
//     score.
// The recurring shape is: when the second argument is `size - offset`, the
// original computes it in ecx first and only then loads the first argument,
// late, straight into the register it just freed; MSVC 5 instead hoists that
// load into the dead eax (case 0), or keeps the second argument's register for
// the third argument and pushes it early (cases 1 and 2). The likely lever is
// an inlined helper whose argument temporaries change that choice, but every
// helper spelling tried folds to the same schedule.
//
// Fourth pass (deepseek-v4.1-flash): built a per-case instruction comparator
// (build/scratch/0x4c70d0/cmp.py) so each case is compared on its own
// normalized instruction list instead of the whole-function ratio. It
// confirms the three failing cases are exactly the register rotations above,
// and that case 3 matches opcode for opcode. New spellings tried, all still
// 78.9% (280 bytes) with case 3 matching: comma-expression sequencing
// `FUN((out->low = 0, at_high), ...)`, braces around cases 0 and 1, `int d =
// size - offset`, `int h = at_high`, `int g = DAT_0051fef0` both before and
// after the store, `(int)` casts, a local `int* p = out` with p[0]/p[1], a
// reversed-statement order, an `int r = FUN(...)` result local, and
// folded-zero liveness probes (`out->low = x - x`) on offset, lo, value, i,
// hi and size, to try to keep a value live the way the earlier offset-live
// diagnostic did. None moved a case.
// Byte-level detail: the original case 0 loads the global with the 5-byte
// `mov eax, ds:[0x51fef0]` (the accumulator form) while this file emits the
// 6-byte `mov edx, [0x51fef0]`. So matching case 0 is not only a similarity
// matter: the global must reach eax, which is the same consequence the
// offset-live diagnostic had (and that diagnostic also demoted out).

// GLOBAL: 0x51fe48
extern int DAT_0051fe48[];
// GLOBAL: 0x51fef0
extern int DAT_0051fef0;

struct Chunk {
    int field_0;
    int field_4;
};

struct Range {
    int low;
    int high;
};

// GLOBAL: 0x51fef8
extern Chunk* DAT_0051fef8;
// GLOBAL: 0x51fefc
extern int DAT_0051fefc;
// GLOBAL: 0x51ff00
extern int DAT_0051ff00;

int FUN_004b7381(int a, int b, int c);

// FUNCTION: 0x4c70d0
void __stdcall FUN_004c70d0(int value, Range* out, int at_low, int at_high)
{
    int i = DAT_0051ff00;
    Chunk* table = DAT_0051fef8;
    int j = DAT_0051fe48[i];
    DAT_0051fefc = j;
    while (value > table[j].field_4) {
        j = DAT_0051fe48[j];
        i = DAT_0051fe48[i];
        DAT_0051fefc = j;
        DAT_0051ff00 = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    DAT_0051fef0 = size;
    if (size == 0) {
        return;
    }
    switch (i) {
    case 0:
        out->low = 0;
        out->high = FUN_004b7381(at_high, size - offset, DAT_0051fef0);
        return;
    case 1:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = 0;
        return;
    case 2: {
        out->low = at_low;
        int c = DAT_0051fef0;
        out->high = FUN_004b7381(at_high, offset, c);
        return; }
    case 3:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = at_high;
        return;
    }
}
