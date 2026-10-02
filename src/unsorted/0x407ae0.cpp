// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
//
// Space Bunny Free pass (2026-10-02, 91.1% -> 92.1%, 601 of 601 bytes, about 300
// compiles via build/scratch/0x407ae0/sweep.py). One real gain, and it is the
// statement ORDER of the second roll inside the scatter loop, not its name:
//
//     int dx = FUN_004b6c30(w) - hw;
//     dest.x = pos.x.value + (dx << 16);
//     int dz = FUN_004b6c30(h) - hh;      <-- here, after the dest.x store
//     dest.y = pos.y.value;
//     dest.z = pos.z.value + (dz << 16);
//
// Naming the second roll and putting its declaration AFTER the dest.x store
// makes MSVC issue the second call before dest.y is written, which reshapes the
// whole first half of the loop body and is worth a point at an unchanged 601
// bytes. The older notes recorded "naming dz" as 58.6%/613 bytes, which is
// right but was always measured with the declaration BEFORE the sums; position
// is the whole lever, and `dest.z` before `dest.y` ties at 92.1% too. Three
// more body shapes tie at 92.1%: `dx` inlined (only dz named), `dest.z` before
// `dest.y`, and the reversed addends `(dz << 16) + pos.z.value`. Measured
// negatives against this body, all 601 bytes unless noted: dz before the sums
// 58.6%, dz between dest.y and dest.z 91.1%, dest.y assigned first 91.6%,
// `int dz = FUN_004b6c30(h); dest.z = ... ((dz - hh) << 16)` 92.1% (tie),
// dx inlined and dz named 92.1% (tie). Which locals exist is the other knob
// (guide item 19), and it is a measured 48-case cross product of the five
// choices: `delay`, `dz`, `hw` and `hh` must all be named (inline the delay and
// it falls to 74.4%, hw to 88.8%, hh to 91.6%, dz to 91.1%) and `dx` is
// optional, because inlining it still gives 92.1% at 601 bytes.
//
// Cheap checks run and recorded, both of which the older passes left open:
// * Declaration order is INERT here. All 120 permutations of the five
//   scatter-loop declarations (declared together, assigned in source order) are
//   byte-identical at 92.1% / 601 bytes (build/scratch/0x407ae0/declorder.py).
//   That is the fifth function on this tree where a measured sweep proves it.
// * tools/permute.py has now run here seven times. Seeds 61 and 91 on the 91.1%
//   file (1604 and 1770 candidates, structural kinds only in the second) both
//   ended 91.1% -> 91.1%, score 1120 -> 1120. Seed 5 on the 92.1% file ran 3070
//   candidates and ended 92.1% -> 92.1%, so the permuter's own metric agrees
//   this file is better (the fine score drops 1120 -> 985) and finds nothing
//   left in it.
//
// The remaining difference is one hunk, the loop preheader plus the first half
// of the loop body, and it is now known to be exactly TWO decisions:
//
// (1) The block split. The original computes w and h, tests the loop, and only
//     then computes the two halves:
//
//         sar esi, 3 ; sar edi, 3
//         cmp ecx, ebx ; jle end
//         mov eax, esi ; cdq ; sub eax, edx ; mov ebp, eax ; sar ebp, 1
//         mov eax, edi ; cdq ; sub eax, edx ; sar eax, 1
//         mov [esp + 0x14], eax
//
//     Here the halves are computed before the test, so C1 interleaves the two
//     `sar ..., 3` with them and the spill of hh lands last. Every shape that
//     gets the halves after the test does so by putting them in a nested scope
//     (inside the loop body, or between a guard and a do-while), and every one
//     of those costs the allocation: `this` then takes ebx, the counter takes
//     ebp and both halves go to the stack, which is 52.9-59.0% at 595-597 bytes
//     (measured: hw/hh declared in the body, assigned in the body, a guarded
//     do-while, a guarded for, `for(;;)` with `if (++i == n) break;`, the
//     negative `if (n > i)`, and half/half variants of each). This file's
//     register allocation is the original's exactly (esi = w, edi = h, ebx = i,
//     ebp = hw, [esp+0x10] = this, [esp+0x14] = hh), so the split is the only
//     thing that has to move, and every way of moving it moves this too.
//     Re-measured against the 92.1% body the split is one thing and nothing
//     else: every spelling that declares hw/hh where LICM can hoist them gives
//     the same 59.0% at 597 bytes (body-declared, assigned-in-body, w and h
//     declared in the for-init with the halves in the body, copies of the
//     halves taken in the body), and every spelling that declares them before
//     the loop gives 92.1%, including a plain `{}` block wrapped around both
//     the halves and the loop, which is byte-identical. So the split is a
//     scope, not a statement order, and no scope buys it without the register
//     change.
//
// (2) The entry guard. The original spells the first test `cmp ecx, ebx; jle`,
//     that is `n <= i` with ebx holding i, and C1 did not fold i to the constant
//     zero it was assigned two hundred bytes earlier. Here the same test is
//     `test ecx, ecx`. Roughly sixty spellings fold it: for-init, branch-scope,
//     function-scope and do-while counters, `i != n`, `i + 1 <= n`, `n - 1 >= i`,
//     `n > i`, `i = i + 1`, `++i`, an outer `if (n > 0)` guard (which then adds
//     a SECOND guard), and pointers to the counter. Only something MSVC 5
//     cannot constant-fold would stop it, and the only such thing available for
//     a local is `volatile`, which is not allowed. So (2) looks unreachable and
//     (1) is the only lead left.
//
// claude-sonnet-5-5 pass (2026-10-02, still 91.1%): the guard is the key. `if (n > 0)` written
// as a source-level test gives the original's `cmp ecx, ebx; jle` BEFORE the halves ONLY when
// the zero register is live there; with `if (n > 0) { halves; for (int i...) }` (58.7%) the
// guard and block order match but MSVC then keeps a second `test ecx,ecx` guard for the for,
// puts `this` in ebp and spills i and hw. do-while forms (`if (n > 0) { int i = 0; do {...}
// while (++i < n); }`, with i inside or outside, with `i++; while (i < n)`) give a single
// guard but i and hw go to the stack and this to ebx (52 to 59%): i loses the callee-saved
// ranking against `this`/hw whenever the for-loop's extra guard use of i is missing.
// `int i = 0; if (n > 0) do {...} while` with halves BEFORE the if is byte-identical to the
// 91.1% file. Inline accessors (Grp() for field_8, Rand() wrapper, Coord::V()) are no-ops
// here. Permuter: 2 more runs (seed 41 from the 58.7% `if (n > 0)` file reached 75.2% with a
// do/while(0) tangle, seed 52 from the 91.1% file: nothing).
// Lesson from sibling 0x402da0 (MATCH): a trivial inline accessor (`Unit* Target()`) can be
// the missing lever, found by permute.py from a structurally right 87.9% start file.
// Slot 0 of Class_00407a90 (vtable 0x4fc998), derived from Class_00407350
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Sets field_c to 30..929 ticks from now. A group of fewer than 5 units
// either moves (mode 9) to the unit nearest its average position, or, when
// FUN_0040ba80 gives a rally point for field_10, is sent to 2..3 random points
// around it (mode 2 first, then mode 9). A bigger group is sent to a random
// point on the map edge.
//
// Partial (92.1% after the Space Bunny Free pass above, 91.1% before it):
// everything outside the scatter loop matches byte for byte, including the
// whole "5 units or more" branch. One hunk differs, the loop preheader plus the
// first half of the loop body, and the pass note above reduces it to the block
// split and the unfolded entry guard:
//
// 30-min checkpoint (space-bunny-free, 2026-10-02, second pass). Still 91.1%,
// 601 of 601 bytes, the file below is unchanged from the previous passes. All
// three priority steps are done and every one of them is a negative, which
// closes off the two hypotheses the older notes above still leave open:
// * tools/permute.py HAS now been run here (it never had been). Seeds 11 and
//   12: 2789 and 3973 candidates, both 91.1% -> 91.1%, nothing. Seed 13 climbed
//   the fine score 1120 -> 920 and check.py to 92.5%, and that 92.5% is a
//   difflib artifact, not progress: its best.diff (build/permute/0x407ae0/,
//   snapshot in build/scratch/0x407ae0/s13/) aligns the loop hunk better while
//   *regressing* a hunk the 91.1% file matches - in the "5 units or more"
//   branch its `unsigned int tmp1 = FUN_004b6c30(2)` makes MSVC emit
//   `cmp eax, ebx` where the original has `test eax, eax`. It is also a tangle
//   of tmp0/inl0 names, a self-store and `do ... while (0)` leftovers. So it is
//   not taken; the only real thing it found is recorded below.
// * BT_TOOLCHAIN=msvc5-rtm uv run tools/check.py 0x407ae0 prints 91.1% with the
//   same 601 bytes. The toolchain is NOT the lever, so every "91.1% local
//   optimum" note in this file is about the right compiler. Worth one line on
//   its own: nobody had checked it in five passes.
// * The unused-declaration sweep and the helper count are both dead. Four
//   unused-declaration sizes (4, 16, 64, 256) and N = 1..8 uncalled
//   `static inline` helpers all score exactly 91.1%, so this is source shape
//   and not MSVC 5 allocator state. A Game* constructor on Vec3_00407410, the
//   two ctors defined above with a self-store, and MakeFixed reshaped into the
//   0x407d40 `int` from `double` shape are 91.1% too (83.4% for the MakeFixed
//   reshape, which is worse).
// * The guard is not MSVC folding a spelling: asking for the condition twice
//   does not help. The dead-store / self-store / two-names-for-one-zero levers
//   applied to the loop *counter* (rather than to n) are 91.1%: a store in a
//   statically folded branch, `i = i;`, `int z = i; i = z;`, `int i = n - n;`.
//   And `for (;;) { ... i++; if (!(i < n)) break; }` under `if (i < n)` is
//   91.1%, so the `cmp ecx,ebx` guard is not just the loop test asked for
//   twice. Swapping the addends in the two sums - `dest.x = (dx << 16) +
//   pos.x.value` and `dest.z = ((FUN_004b6c30(h) - hh) << 16) + pos.z.value` -
//   is 91.1% on its own, though seed 13 needed it together with its tangle to
//   reach the 92.5% artifact.
// * A lead the permuter did isolate, worth trying: seed 13's body ends with
//   pos.x hoisted into a temp, `dest.y` assigned before `dest.x`, and
//   `dest.x = (shifted random) + (pos.x temp)`, which is what made MSVC put
//   the sum's destination in the pos.x register (`add ecx, eax`) as the
//   original has (`add edx, eax`). Clean variants of that on their own are
//   91.1%, so whatever makes it stick needs something else as well.
// * Seeds 13 and 14 both climb to the same 92.5% artifact (fine score 920), so
//   that is a systematic attractor of the permuter's metric, not a find.
// * Self-assignment and `(void)` forms, now worth measuring per AGENTS.md.
//   Nine of them, on the values the remaining decision turns on: `i = i;` and
//   `(void)i;` before the loop, `i = i;` in place of the for-init, `hw = hw;`,
//   `hh = hh;`, `(void)hh;`, `w = w;`, `(void)n;`, `dx = dx;` and `(void)dx;`
//   in the body, plus `vx = vx;` on a named x sum. All nine are 91.1% at 601
//   bytes, so MSVC 5 DELETED every one of them: they emit no code, and they
//   do not move the score either. "Deleted by the compiler" and "scored the
//   same" are both true here, and neither makes this a lever. The script is
//   build/scratch/0x407ae0/hand_s.py.
// * The body shape above behaves the same way: hoisting pos.x and the shifted
//   random into their own temps with `dest.y` assigned between them is 91.1%
//   at 601 bytes, with or without a helper for the pos.y load, and the same
//   shape applied to the z sum as well is 59.5% at 613 bytes.
// * The frame map in the notes below is right, and the "10 dword(s) of locals"
//   from ctx.py checks out: with `sub esp,0x28` plus four pushes the ten
//   locals are [esp+0x10] (this, spilled at entry, reloaded as field_8 in
//   both loop arms), [esp+0x14] (hh), [esp+0x18] (n), [esp+0x1c] (the &dest
//   for FUN_00407410), [esp+0x20] (dest.x), [esp+0x24] (dest.y then dest.z),
//   [esp+0x28..0x30] (pos.x, pos.y, pos.z) and [esp+0x34]. The return address
//   is at [esp+0x18] of the *pre-push* frame, i.e. above all of them, which is
//   why storing this at [esp+0x14] while the argument is pushed is safe.
//
// One more thing the older notes above leave open, now confirmed: the original's
// top guard is commuted (`cmp ecx,ebx` = `n <= i`) while its bottom test is
// not (`cmp ebx,eax; jl` = `i < n`). Spelling the loop condition `n > i`
// commutes both, so the loop's own condition is spelled `i < n` and the top
// guard is a source-level `if` of its own. Every spelling of that `if` tried
// so far also moves `this` out of esi and drops the file to 52.9%:
//
// - The original splits the loop preheader: `sar esi,3; sar edi,3` (w and h)
//   come before the entry test `cmp ecx,ebx; jle`, and the two halves (ebp =
//   w/2, [esp+0x14] = h/2) come after it as a block of their own. Here the
//   halves are computed before the test, and the test is `test ecx,ecx`,
//   emitted between the halves' last shift and the spill of h/2.
// - In the body the original loads pos.x into edx and adds the shifted random
//   delta into it (`add edx, eax`), loading pos.y only after dest.x is
//   finished; here pos.x goes to ecx, pos.y to edx, both are loaded up front
//   and both sums land in eax. Same knock-on: which register the block takes
//   as its first free temp.
// - space-bunny-free re-confirmed the walls the earlier passes hit. Both are
//   register-allocation walls, not byte counts: the early `mov [esp+0x14],
//   esi` spill of `this` (which is what frees esi for w), `this` living in
//   esi, and ebp holding w/2. Writing the halves as loop-body expressions
//   (w / 2 inline, so LICM would sink them below the test) drops the file to
//   27.2%: the early spill of `this` disappears and ebx becomes ebp. An
//   explicit `if (n > 0) { int hw, hh; for (...) }` is 58.7% and puts `this`
//   in ebp, as the notes said. Declaring w and h in ONE statement is
//   byte-identical to two statements, so that split is not a statement
//   boundary effect either.
// - deepseek-v4.1: both remaining ideas collapse the whole allocation, so the
//   91.1% form is a hard local optimum. Declaring hw and hh inside the loop
//   body (LICM then hoists them into the preheader after the entry test) is
//   52.9%: `this` leaves esi for ebx and the loop counter takes ebp. Dropping
//   the `dx` local and inlining the FUN_004b6c30(w) call into the dest.x
//   expression is 52.0% (595 bytes) with the same shake-up. Writing the loop
//   as `for (int i = 0; n > i; i++)` is 90.2%: it only swaps the bottom test
//   to `cmp eax,ebx; jg` and moves the `if (i == 0)` test, while our top guard
//   stays `test ecx,ecx`. The `cmp ecx,ebx` form is part of the block split,
//   not of the loop condition's spelling. An early `if (n <= 0) return;` plus
//   a do-while is 59.0% (`this` moves to ebp), and moving w and h into the
//   loop with the halves is 19.5% (frame drops to 0x20: the divisions are not
//   hoisted).
// - Untried: making the halves depend on something LICM cannot hoist out of
//   the body (a value reloaded from the group), which is the only way seen so
//   far to get a block between the entry test and the body without opening a
//   nested scope.
// - tools/headers.py: no header set does better than 91.1%.
// - deepseek-v4.1-flash retry: re-derived the two branches the note already
//   lists and confirmed them. `if (n > i) { int hw, hh; do {...} while (i < n); }`
//   with i declared at else-scope is 52.9% (same as declaring hw/hh in the
//   body): the guard `cmp ecx,ebx` does appear, but declaring the counter at
//   branch scope moves `this` out of esi (to ebx) at the very top of the
//   function, so the whole function shifts. `int i = 0; if (n > i) { int hw,
//   hh; for (; i < n; i++) }` is 52.7% with the same shake-up. The 91.1% form
//   above remains the best: the only real difference is the loop preheader
//   ordering (halves computed before the entry test, and `test ecx, ecx`
//   instead of `cmp ecx, ebx`) plus the register the body loads pos.x into.
// - deepseek-v4.1-flash retry 2 (brute-forced about 30 scratch variants, all
//   scored with check.py --sym): the `cmp ecx, ebx` guard only appears once
//   the counter `i` is a named variable declared at branch scope, but that
//   alone moves `this` from esi to ebx at the first instruction (the whole
//   function shifts, 52.9%). Ordering the halves as an assignment inside the
//   `if` (the do-while form that reproduces the original block order) always
//   triggers that same `this` -> ebx move: `int i=0; int hw,hh; if (n>i) {
//   hw=w/2; hh=h/2; do {...} while (i<n); }` is 52.9% and `this` leaves esi.
//   Moving the for-init scope earlier (`int i,hw,hh; for (i=0; ...)`) is
//   52.9%, and `for (int i=0; n>i; i++)` / `while (n>i)` are 90.2%: they only
//   swap the bottom test to `jg`. Keeping the counter in the for-init and the
//   halves in the preheader is the only form that keeps `this` in esi, and it
//   is exactly the 91.1% file below. The two remaining diffs (halves before
//   the entry test, and pos.x loaded into ecx instead of edx) both follow from
//   that register choice, so this is a local optimum for this compiler.
// - deepseek-v4.1-flash retry 3 (2026-10-01, 900s): re-confirmed 91.1% is the
//   local optimum. Tried: `int i = 0;` at function scope (53.5%), a do-while
//   under `if (n > i)` (52.7%), `if (n > 0)` (58.7%), an early `if (n <= 0)
//   return;` (58.7%), halves declared in the loop body (52.9%), a `while`
//   (52.9%), `int i;` assigned 0 just before the loop (91.1%, identical),
//   hw/hh in an inner scope block (91.1%, identical), reordering the hw/hh
//   declarations (90.7%), swapping the w/h declarations (89.3%), defining the
//   two preceding functions (ctor 0x407a90 and ??_G 0x407ac0) above this one
//   (91.1%, no change), and all 128 header sets from tools/headers.py (91.1%,
//   none better). Every construct that emits the original's `cmp ecx,ebx`
//   entry guard also moves `this` out of esi to ebx and the loop counter to
//   ebp, so the guard encoding and the preheader block order are one
//   allocation decision: the halves must be lexically after the guard, and no
//   scope that does that keeps `this` spilled.
// - Reordering the three `dest` stores (e.g. dest.z before dest.y) scores up
//   to 92.1% on check.py, but true instruction LCS is unchanged and the store
//   order no longer matches the original (x, y, z); those are difflib
//   artifacts, so the file keeps the x, y, z order.
// - The packet for this address misprints one operand: 0x407c8b is
//   `mov dword ptr [esp + 0x20], eax`, not [esp + 0x24]. The >= 5 branch of
//   the saved source is byte-exact, so trust check.py, not the packet.
//
// - space-bunny-free retry (2026-10-01): confirmed 91.1% is still the local
//   optimum, and the frame map makes the two allocation choices explicit. With
//   F = the frame base (the `sub esp,0x28` result), the locals are: F+0 `this`
//   (the early spill, reloaded in both loop branches, never written again),
//   F+4 hh, F+8 n, F+0xc..F+0x14 dest, F+0x18..F+0x20 pos (the four saved
//   registers are just below F, the return address at F+0x28). Live across
//   the loop: w (esi),
//   h (edi), the counter (ebx) and one of hw/`this`: the original spills
//   `this` (it already has a stack home from the early spill) and keeps hw in
//   ebp, while every form that puts the halves inside the loop body makes MSVC
//   drop the early spill, give ebx to `this`, ebp to the counter and push both
//   halves to the stack (52.9%, 595 bytes, both allocations exactly as wide).
//   So the block order and `this`'s home are one decision: hoisting the halves
//   past the guard only happens with LICM (a temp, so a stack slot) or with a
//   source-level `if` + do-while (this moves to ebx). The guard spelling
//   `cmp ecx,ebx; jle` never appeared in any of ~25 spellings tried here (for,
//   while, do-while under `if (i < n)` / `if (n > i)`, counter declared at
//   function, branch or for-init scope, unsigned n): MSVC folds i==0 to
//   `test ecx,ecx` in every one of them, so the original's i was a variable
//   MSVC could not constant-fold, which no local spelling reproduced.
//   Body notes: putting `dest.z` before `dest.y` in the source is 92.1% on
//   check.py but the emitted stores are still x,y,z and the instruction LCS is
//   unchanged, another difflib artifact (as the note below says). Swapping
//   dest.x/dest.y in the source, naming the x sum in a temporary, reading the
//   three pos fields into locals first, or writing dest.y through a Coord
//   temporary are all byte-identical to the file below.
// - deepseek-v4.1-flash retry 4 (2026-10-01): still 91.1%, 601 bytes. New
//   negatives: `if (n > 0)` around only the for loop (halves still outside) is
//   51.9% / 617 bytes, and `int hw = w / 2, hh = h / 2;` in one declaration is
//   byte-identical to the two-statement form. The block split stays unreachable.
//
// - space-bunny-free (2026-10-02, notes only, nothing compiled): this address
//   has never been through tools/permute.py (build/permute/ holds only
//   0x407d40), so all five passes so far were hand work. Sweep seeds, not one
//   run: permute.py 0x407ae0 --jobs 4 --minutes 10 --seed 11, then 12, 13, 14.
//   The remaining diff is a scheduler and allocation tie, which is what the
//   permuter is for.
// - Levers the permuter cannot reach (it only rewrites this function and the
//   inline helpers this function calls), none of them tried yet, all written
//   out and scored by `uv run python build/scratch/0x407ae0/sweep.py` (see the
//   README beside it; nothing in that batch has been compiled yet):
//   * N = 1..8 uncalled `static inline` functions in this TU. The guide's
//     0x4ac970 MATCH keeps one nothing calls, and 0x4c06e0 was fixed by adding
//     one, so the count is a knob on MSVC 5's allocator state.
//   * `MakeFixed`, the one helper this function inlines, given 0x407d40's
//     winning shape: an `int` local declared on its own and assigned in a
//     separate statement from a `double` local. Also a `same0` self-store in it.
//   * a `Game*` constructor on Vec3_00407410 in that same shape: on the sibling
//     the deciding IL nodes were in a neighbouring class's constructor, never
//     in the function being compiled.
//   * 0x407350's and 0x407a90's constructors defined above this function with a
//     self-assign and an `int` from a `double` in a statically dead `if`
//     (defining them plain was 91.1%, no change).
//   * `int t = 0; if (t) n = 0;` before the loop, or before `hw = 0` after the
//     halves: a dead store in a folded branch blocks copy propagation, no code.
//   * `for (;;) { ...; i++; if (!(i < n)) break; }` under `if (i < n)`: the one
//     spelling that asks for the loop condition twice, so the top guard cannot
//     be specialised to `i == 0` and the hoisted halves land after it.
// - Two rule-outs nobody has run here either, both from the guide: a flat sweep
//   of unused declarations (0 to 700, guide 0x4624a0 / 0x46e640: if the score
//   never moves, the difference is source shape, not compiler state; the sweep
//   script does 4, 16, 64 and 256), and one build with the unpatched compiler,
//   `BT_TOOLCHAIN=msvc5-rtm uv run tools/check.py 0x407ae0` (guide 0x4732e0).
//   If that moves the score, every note above about the "91.1% local optimum" is
//   about the wrong compiler.
//
// `field_c = g_game->ticks + FUN_004b6c30(900) + 30` in one expression folds
// to `lea eax, [eax+edx+0x1e]`; the delay has to be computed first.
// The final MakeFixed ternaries give the `lea eax, [tmp]; mov ecx, [eax]`
// selection. The unit FUN_004071f0 returns is used without a null check.
#include <vector>

#pragma pack(push, 1)
struct Game_00407ae0 {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x38a47 - 0x1422b];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game_00407ae0* g_game;

struct FixedParts_00407ae0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_00407ae0 {
    int value;
    FixedParts_00407ae0 parts;
};

static inline Fixed_00407ae0 MakeFixed(int i)
{
    Fixed_00407ae0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

struct Vec3_00407410 {
    int x;
    int y;
    int z;

    Vec3_00407410() {}
    Vec3_00407410(int a, int b, int c) : x(a), y(b), z(c) {}
};

union Coord_00407ae0 {
    int value;
    struct {
        unsigned short frac;
        short whole;
    } s;
};

struct Pos_00407ae0 {
    Coord_00407ae0 x;
    Coord_00407ae0 y;
    Coord_00407ae0 z;
};

#pragma pack(push, 1)
struct Unit_00407ae0 {
    char unknown_0[0x6a];
    Vec3_00407410 pos;                 // +0x6a
};
#pragma pack(pop)

struct Group_00407ae0 {
    void* player;                      // +0x0
    int id;                            // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit_00407ae0*> units; // +0x10
};

struct Class_00408cb0 {                // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

class Class_004071f0 {
public:
    Unit_00407ae0* FUN_004071f0(Vec3_00407410 pos);
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1

    int FUN_00407410(Vec3_00407410* out);
};

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class Class_00407a90 : public Class_00407350 {
public:
    Class_00407a90(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407ae0
};

int __stdcall FUN_004b6c30(int range);
void __stdcall FUN_0040ba80(int index, Pos_00407ae0* out);
void __stdcall FUN_00480460(void* player, int id, int mode, int remove, int* target,
                            Vec3_00407410* pos, int flags, int extra);

// FUNCTION: 0x407ae0
void Class_00407a90::FUN_00407380()
{
    Vec3_00407410 dest;
    int delay = FUN_004b6c30(900) + 30;
    field_c = g_game->ticks + delay;
    if ((int)((Group_00407ae0*)field_8)->units.size() < 5) {
        Pos_00407ae0 pos;
        FUN_0040ba80(field_10, &pos);
        if ((pos.x.s.whole | pos.z.s.whole) == 0) {
            FUN_00407410(&dest);
            Unit_00407ae0* target = ((Class_004071f0*)owner)->FUN_004071f0(dest);
            FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                         9, 1, 0, &target->pos, 0, 0);
        } else {
            int n = FUN_004b6c30(2) + 2;
            int w = g_game->baseX / 8, h = g_game->baseY / 8;
            int hw = w / 2;
            int hh = h / 2;
            for (int i = 0; i < n; i++) {
                int dx = FUN_004b6c30(w) - hw;
                dest.x = pos.x.value + (dx << 16);
                int dz = FUN_004b6c30(h) - hh;
                dest.y = pos.y.value;
                dest.z = pos.z.value + (dz << 16);
                if (i == 0)
                    FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                                 2, 0, 0, &dest, 0, 0);
                else
                    FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                                 9, 1, 0, &dest, 0, 0);
            }
        }
    } else {
        dest.y = 0;
        if (FUN_004b6c30(2)) {
            dest.x = FUN_004b6c30(g_game->baseX) << 16;
            dest.z = (FUN_004b6c30(2) ? MakeFixed(0) : MakeFixed(g_game->baseY - 1)).value;
        } else {
            dest.x = (FUN_004b6c30(2) ? MakeFixed(0) : MakeFixed(g_game->baseX - 1)).value;
            dest.z = FUN_004b6c30(g_game->baseY) << 16;
        }
        FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                     9, 0, 0, &dest, 0, 0);
    }
}
