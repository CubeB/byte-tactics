// Decompiled by DeepSeek V4.1 Flash and space-bunny-free, finished by deepseek-v4.1-flash and space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol and space-bunny-free. Names are provisional.
//
// space-bunny-free pass (#4676): MATCH, 100.0%, 280 of 280 bytes, all
// references ok. The whole fix is case 1's second argument: it is computed into
// a local and then passed to the call THROUGH A POINTER TO THAT LOCAL, which is
// what makes cl5 5 schedule the block the way the original does.
//   * The body is otherwise unchanged from the earlier passes: one line of case
//     1, `out->high = FUN_004b7381(at_high, size - offset, DAT_0051fe40);`,
//     becomes
//         int span = size - offset;
//         int* spanp = &span;
//         out->high = FUN_004b7381(at_high, *spanp, DAT_0051fe40);
//     and that alone takes cases 1 AND 2 to byte-identical (and with them the
//     1-byte `je`/`ja`/jump-table offset, since the global reload lands in eax
//     and so uses the 5-byte accumulator form). The `int*` reads like a
//     leftover in Cavedog's source; it is kept with a comment because the bytes
//     need it.
//   * What it does: `size - offset` written in the argument is copied into the
//     register that `sub ecx, eax` frees, which is eax, which the hoisted
//     at_high load then wants. Going through `&span` stops that copy
//     propagation, so the block is numbered in the original's order: store,
//     `mov eax, ds:[0x51fe40]` (5 bytes), `push eax`, `push ecx`, then
//     `mov ecx, [esp+0x24]` for at_high as the last push, and case 2's at_high
//     load lands after `mov [ecx], eax` in eax as the original has it. So the
//     whole residual of the previous passes (one allocator tie, two hunks) is a
//     copy-propagation tie on case 1's second argument.
//   * Exact statement of the lever, for docs/agent-guide.md: when a call's
//     argument is an expression the compiler would otherwise materialise into the
//     register the previous instruction freed, assigning that expression to a
//     local first is NOT enough (cl5 copies it straight back), but taking the
//     local's address and passing `*spanp` blocks the propagation and renumbers
//     the block. Confirmed here on all four spellings: the pointer before the
//     store, after the store, a pointer to `offset` as well, and a pointer to
//     `at_high` as well, all MATCH. It does NOT work through an inlined helper
//     (`static inline int Span(int* s, int o) { return *s - o; }` called as
//     `Span(&size, offset)` is 80.0% again: the inliner folds it back to the
//     plain expression), and `*&d` is 80.0% too, so it must be a real local
//     pointer used as an argument. Cases 0, 2 and 3 need nothing.
//   * New tools in build/scratch/0x4c71f0/: h.py imports tools/check.py and
//     prints a per-region tag line (pre c0 c1 c2 c3 epi pad, O or D) per
//     variant, ~0.1 s a variant with six threads, so a variant that fixes one
//     case and breaks another shows at once; sw.py is the plain scorer;
//     b1.py..b6.py are the batches of this pass.
//   * THE SIBLING LEAD IN THE NOTES BELOW IS WRONG, and it closed the best idea
//     several earlier passes had. 0x4c70d0 is NOT a match in this worktree:
//     check.py prints 78.9% (280 of 280 bytes). Its case 0 block is the SAME
//     miscompile ours had:
//         ours      mov eax,[esp+0x1c]; mov [esi],0; mov edx,[DAT_0051fef0]
//         original  mov [esi],0;           mov eax,[DAT_0051fef0]
//     so "our build of the sibling emits exactly the schedule we want" is not
//     true and there was no sibling case to copy. Do not spend budget on
//     0x4c70d0 for this. Its case 2 is the one that already scheduled at_high
//     late (`out->low = at_low; int c = DAT_0051fef0;
//     out->high = FUN_004b7381(at_high, offset, c);`), which is what pointed at
//     the second argument being the thing to change.
//   * Measured this pass, all byte-identical to the 80.0% body, so do not retry:
//     the at_high load through a `static inline` self-conditional
//     `(i == 1) ? at_high : at_high` (as a local, inline in the argument, and
//     the `at_high > at_high` form: cl5 folds all three, no phi survives);
//     `(int)` on case 1's first argument; `int* p = &at_high; ... *p` for the
//     first argument; case 2 with `DAT_0051fe40` as its third argument; the loop
//     as `for (; cond;)`, `do`/`while`, a `goto` and a comma condition; the
//     loop body re-reading `table = DAT_0051fef8;` (the guide's "re-assign the
//     pointer in the loop body" lever, which adds a global load and so changes
//     the loop); nine loop and tail wordings (`goto`, `for` with the increment
//     in the header, initialisation order, `table[j].field_4` inlined in the
//     condition, `hi`/`lo`/`size`/`offset` declared in every order, the test on
//     the global, `if (size)` instead of an early return, a switch on a copy of
//     `i`); and twenty dead-code perturbations that emit nothing (`if (0)`,
//     `int t = 0; if (t) ...`, self assignments and a ten-statement dead block,
//     in the loop body, in the tail and in cases 1 and 2). The block's
//     allocation is insensitive to all of them.
//   * Case 1's second argument as `value - lo` (algebraically `size - offset`)
//     is 54.1%, and case 2's the same is 56.0%: reading `lo` inside the switch
//     keeps it live to the switch and the whole pre-switch renumbers (table
//     moves to edi, i to ecx, lo to edx). So `lo` must not be read in a case.
//
// space-bunny-free pass (#4547): still 80.0%, 280 bytes (exact size), no MATCH;
// the body below is unchanged and remains the best. New harness in
// build/scratch/0x4c71f0/ (harness.py scores six variants per 0.7 s and tags
// each of the original's four case blocks OK/DIFF by searching for the block's
// instruction text, so a variant that fixes one case and breaks another is
// visible at once; gen1.py, gen2.py write the variants from the base file,
// anchored on code so no comment is ever rewritten).
//   * The residual is one decision per block: whether the at_high stack load is
//     hoisted to the earliest point where its register is free. Ours hoists it
//     (case 1: into eax, which `sub ecx,eax` frees, before the store; case 2:
//     into edx, free from the top, before `add esp,0xc` and the result store),
//     the original keeps it at its point of use. Since both blocks then hand
//     the earliest free register to whichever load comes first in the emitted
//     order, the global lands in eax (original) only because at_high yields it.
//     "Which load gets the first free register" is therefore not the lever;
//     the hoisting is. Hoisting across the same store does happen in the
//     original, in case 3 (`mov edx,[esp+0x1c]` above `mov [esi],0`), so the
//     original is not simply refusing to cross stores; case 3's store writes an
//     immediate and so takes no register.
//   * Ruled out this pass, every one byte-identical to the body below, so do
//     not retry: the 0x463610 member-sub-object shape in full (a `RangeSlot`
//     sub-object with an inline `Set`, all four case bodies rewritten to
//     `out->low.Set(...)`); references to both fields taken once per case
//     (`int& lo = out->low; int& hi = out->high;`), a reference to at_high
//     (`const int& a = at_high;`), and at_high read as
//     `*(int*)((char*)&at_low + 4)`; each case body inside its own inlined
//     helper taking out/at_low/at_high (and d, or offset and size) by value;
//     SetLow/SetHigh helpers on all four cases; `int* p = (int*)out` with p[0]
//     and p[1] on all four cases; `Range* o = Id(out)` on the stores; one, two,
//     three, five and eight uncalled `static inline` functions at file scope;
//     the dead-store-in-a-folded-branch trick aimed the other way (`int t = 0;
//     if (t) bits = 0;` with bits = at_high, and `if (t) at_high = 0;` itself),
//     so a folded dead store naming the parameter does not block the hoist
//     either; `int d = size - offset;` in a braced case 1; `if (!size) return;`
//     and `if (size) { switch ... }`; and a SetBoth(out, lo, hi) helper, which
//     is 276 bytes / 77.8%, worse.
//   * MEASURED, and the sharpest fact of this pass: a micro copy of this
//     function (build/scratch/0x4c71f0/mi2_*.cpp, mi.sh compiles one and prints
//     the asm) localises the hoist to the SEARCH LOOP, not to anything in the
//     case bodies. With the loop the case-1 load of at_high is hoisted above the
//     store; delete the whole `while` (keeping every table read) and the same
//     body emits the original's order, with the load last:
//         mov esi,[out]; mov edx,[at_low]; sub eax,ecx; mov [esi],edx;
//         mov edx,[G]; push edx; push eax; mov eax,[at_high]; push eax; call
//     It is not the loop's mere presence, nor its stores, nor its table read:
//     a dummy loop over a counter, over a global and over `table[j].field_4`
//     all keep the load late, and so does forcing two values live across the
//     call; but removing any single statement of the real loop still hoists
//     (no `i = DAT_0051fea0[i]`, no `j = DAT_0051fea0[j]`, no global stores).
//     So the trigger is the loop keeping i, j and table live across its back
//     edge into the switch: that is the register-pressure context the whole
//     switch is numbered in, which is why no spelling of the two failing
//     bodies moves them and why the original's own loop must have had an IR
//     shape no wording tried here reproduces. The sibling 0x4c70d0 lands in the
//     same shape by accident: there arg2 is `offset` in eax, live until its
//     push, so the at_high load is forced late by pressure instead.
//   * The sibling 0x4c70d0 is the best lead in the game and nobody has pushed
//     it: its case 0 is `out->low = 0; out->high = FUN_004b7381(at_high,
//     size - offset, DAT_0051fef0);`, i.e. our case 1's argument shape with an
//     immediate store instead of `out->low = at_low`, and OUR build of the
//     sibling emits exactly the schedule we want here:
//         mov esi,[out]; sub ecx,eax; mov [esi],0; mov eax,[DAT]; push eax;
//         push ecx; mov ecx,[esp+0x24]; push ecx; call
//     (and matches it; 0x4c70d0 is at 78.9% for its case 1 and case 3 only).
//     So the same argument shape can be scheduled both ways in one compiler,
//     and the deciding factor is the rest of the switch, not the case: our
//     micro confirms it is not the store value either, since changing only
//     `out->low = at_low` to `out->low = 0` in a copy of this function still
//     hoists, while the same change in the micro with the sibling's case set
//     does not.
//   * Also ruled out this pass, all byte-identical to the body below: the
//     search loop written through an inlined `More(a,b)` helper returning
//     `a > b` (300 bytes: cl5 5 does NOT fold `setg al; test al,al` back into
//     the compare, so the helper shape cannot produce this loop's bytes), the
//     same helper written as `if (a > b) return 1; return 0;` (276 bytes), the
//     loop's four body statements moved into an inlined `Step(int*, int*)`
//     (identical), and the loop condition spelled `table[j].field_4 < value`
//     (75.7%, the compare's operands swap but cases 1 and 2 do not move, so
//     the loop's tree shape is not what decides them); no-op conversions as the
//     guide recommends for stopping a hoist (`(int)(long)` on the at_high
//     argument, on the store value, on the call result, and all three
//     together); each case body wrapped in `do { ... } while (0);`; a lone `;`
//     after the first store of every case; the matching case bodies (0 and 3,
//     which are byte-correct) perturbed through `Id()` on their at_low/at_high,
//     a field reference for case 3's `out->low`, a comma joining case 0's two
//     stores, and braces on both, in case the switch's shared allocation
//     follows them (it does not move); and a compiler-state sweep through the
//     declared types: Range before Chunk, Range with a third field (size 12),
//     Range with a leading pad (71.7%, the store offsets move), an unused
//     struct ahead of both, Range as a union, unused typedefs, dead static
//     arrays/text/pointers, and dead `static inline` helpers N = 0 to 8.
//   * And the argument list itself cannot be made opaque: an inlined helper
//     with an unused trailing parameter, with an unused leading one, and with
//     an unused `float` one; a helper taking the first argument by reference;
//     and two of the three arguments travelling in a two-int struct passed BY
//     VALUE (cl5 scalarises it and the bytes do not move at all), all on case 1
//     and, for the float one, on case 2 as well. Only a helper that also does
//     the low store changes anything, and it is worse (276 bytes / 78.9%).
//   * Permuter, seed 11, 7 minutes, 1867 candidates, 122 compile errors: no
//     change, 80.0%, so the file is a local maximum of that mutation set.
//   * Next attempt: the micro says the hoist is a consequence of i, j and
//     table being live across the loop's back edge, and every spelling of the
//     loop that keeps the same bytes leaves those live ranges spanning the
//     back edge, so the next attempt needs a loop whose IR is genuinely
//     different at the same bytes. Worth trying: the body in an inlined helper
//     that returns the pair (i, j) rather than taking pointers, a loop whose
//     induction variable is `value` itself, and a `#include` set that shifts
//     cl5's inline budget (the loop through a helper is the only shape found
//     so far that changes anything at all: an inlined `More(a, b)` returning
//     `a > b` gives 300 bytes, because cl5 5 does not fold `setg al;
//     test al,al` back into the compare, so this loop's bytes require a plain
//     condition).
// space-bunny-free pass (#4488): still 80.0%, 280 bytes (exact size), no MATCH;
// the body below is unchanged and remains the best. About 2700 fresh shapes
// were scored with a local harness in build/scratch/0x4c71f0/ (harness.py scores six
// variants at a time, 0.3 s each; cb.py prints the four case blocks beside the
// original's; dump.py, mdump.py; generators gen.py..gen6.py, micro.cpp, micro2.cpp).
// Nothing moved either hunk, but two new facts change what is worth trying next.
//   * The original's case-1 SCHEDULE is reachable; only the at_high load is
//     wrong. Give case 1's FIRST argument a global load instead of the stack
//     parameter (`+DAT_0051fe40`, or `Id(DAT_0051fe40)`) and case 1 becomes
//         mov esi,[esp+0x14]; mov edx,[esp+0x18]; sub ecx,eax; mov [esi],edx;
//         mov edx,[G]; push edx; push ecx; push edx; call
//     which is the original's order instruction for instruction (store first,
//     global loaded at its argument, arg1 pushed last) with at_high's own load
//     missing and the global in edx instead of eax. The reason it stops being
//     hoisted is aliasing: a global load may not cross `mov [esi],edx`, a
//     stack-parameter load may. So the original's late `mov ecx,[esp+0x24]`
//     means cl5 did not treat the parameter slot as clobbered by the store
//     through `out`, and nothing tried here makes it: `&at_high` in an inlined
//     `Id(int&)`, a `const int&` parameter, `Ignore(&at_high)` (an inlined
//     no-op that still takes the address), `*(int*)&at_high` and
//     `*(volatile int*)&at_high` are all byte-identical to the body below.
//     A micro testbed (micro2.cpp) shows the same for every store target type:
//     Range*, int*, char*, void*, short*, `R&`, `R*`, `R[1]` all hoist the
//     arg1 stack load above the store. Only `R** out` stops it, and that needs
//     an extra `mov eax,[esi]`, so it cannot be spelled here.
//   * The switch's allocation is decided for the WHOLE switch, not per case:
//     that same global-as-arg1 change also moves case 2 (its at_low load goes
//     to the arg1 position in eax instead of into ecx after `push ecx`). So
//     perturbing one case's argument expression is a real lever on the other
//     case's block, which is why the two hunks always move together.
//   * Also measured, all 80.0% and byte-identical to the body below, so do not
//     retry: `Identity`/`Id`/`Cid`/`Val`/`CVal` wrappers on each argument, on
//     the store value and on the call result (cl5 folds every one of them);
//     unary `+`, `*&`, `(int)` casts and `*(int*)&x` on every argument and on
//     both store values; forwarders `Rev(c,b,a)`, `Rev` with locals or a dead
//     `+ 0` or a dead `if`, `Step(d,h,g)`, `Fwd`, `Mix(a,b)`, `Sub2(a,b)`,
//     `Ld(int*)`, in case 1 and case 2; `int g = DAT_0051fe40;` and
//     `int h = at_high;` and `int d = size - offset;` after the store in every
//     order and braced or not (the micro's version of the `g` local DOES put
//     the global in eax, but the micro is not faithful: it reloads `size` from
//     the stack for `size - off`, while here both are already in registers);
//     `Ignore()` address probes on at_high, at_low, size, offset, out and
//     out->low; case 2 with the statements swapped (77.8%), braced with a
//     result local, with `+at_low`, with a local `h = at_high`, with `Fwd`, and
//     with `DAT_0051fe40` as arg3; six loop wordings (`for`, `do`/`while`, the
//     reversed and the negated condition, a `goto` and `while (1)` with a
//     `break`) which are byte-identical or worse.
//   * Next attempt: the trigger is that cl5 may not cross the store with the
//     at_high load, so look for a way to make the frame slot look reachable
//     from `out` (a struct type whose field overlaps, a `Chunk`-style cast of
//     the pointer, an inline helper that stores through a second pointer), or
//     accept the aliasing reading and find a spelling that puts the global in
//     eax while the arg1 load stays where the global-as-arg1 experiment leaves
//     it.
//   * The strongest negative result of this pass: 2304 more variants in two
//     knob sweeps (sweep2.py, build/scratch/0x4c71f0/t/) over the case-2 body
//     (arg1 x arg2 x arg3 x store value x store target x a local for the call
//     result x braces, 1728 combinations) and the case-1 body (store value x
//     store target x the three arguments x a local for the result, 576) give
//     exactly ONE case-2 block and ONE case-1 block between them, both
//     identical to the body below's. So no spelling of the two failing bodies
//     can move them: the schedule is fixed by something structural above them.
//     A goto-shaped loop (test at the bottom, two entry points, gen8.py) is the
//     only structural rewrite that changed the switch at all, and it merges the
//     arms (48.9%). Six other loop wordings and an inline helper owning the
//     whole switch (three parameter orders) are byte-identical.
//   * Harness left behind: build/scratch/0x4c71f0/{harness,cb,dump,mdump}.py,
//     sweep.py, sweep2.py, gen.py..gen9.py, micro.cpp, micro2.cpp, micro3.cpp
//     and the g_/h_/p_/q_/r_/b_/w_/y_/z_/t_ variant files.
//
// space-bunny-free pass: still 80.0%, 280 bytes (exact size, no MATCH). About 130
// fresh source shapes were measured with a local harness kept in
// build/scratch/0x4c71f0/ (fast.py scores variants six at a time, blocks.py prints
// the original's four case blocks beside ours and finds each block through the jump
// table so a differently sized case still lines up, dump.py disassembles, gen*.py
// generate the variants). None of them moved the two hunks, so the body below is
// still the best. What is new:
//   * The whole residual is ONE instruction's position. In case 1 the two blocks
//     differ only by where `mov <reg>, [esp + 0x1c]` (at_high) sits: after the two
//     pushes in the original, hoisted above `mov [esi], edx` into eax here. Case 2
//     is the same argument one instruction later. Fix the position and the size
//     follows, since only eax gets the 5-byte `mov eax, ds:[G]`.
//   * A diagnostic that reproduces the original's case 1 EXACTLY
//     (build/scratch/0x4c71f0/s1_offset_store.cpp): one dead statement
//     `DAT_0051fef4 = offset;` after the call in case 1 yields
//         sub ecx, esi; mov [edi], edx; mov eax, [0x51fe40]; push eax;
//         push ecx; mov ecx, [esp + 0x24]; push ecx; call
//     which is the original's order with the global in eax, the late at_high load
//     in ecx, and case 2 corrected at the same time. So the choice is register
//     pressure across the call, not a missing value: keeping offset live there
//     makes MSVC pick the original's schedule. It costs the extra store and
//     demotes out from esi to edi (offset takes esi), so it cannot be committed.
//   * A micro testbed (build/scratch/0x4c71f0/micro*.cpp) shows the late at_high
//     load whenever the low store moves after the call (both stores in one inline
//     helper, or the call written first) and in a stripped-down copy of this
//     function without the loop. Dropping only the loop from the real pre-switch,
//     with the table reads kept, gives the late load; every spelling that keeps
//     the loop hoists. The loop's presence, not its wording, decides it: a `for`
//     with an empty increment, a chained assignment, temps in the body, a swapped
//     store order and eight more byte-identical rewrites of the loop and of the
//     pre-switch all still hoist.
//   * Ruled out this pass, every one byte-identical to the body below or worse,
//     so do not retry them: an `int g = DAT_0051fe40;` local declared AFTER the
//     store in case 1 (0x4c70d0's case 2 trick does not transfer, the inline
//     global already reloads); the same local before the store (276 bytes);
//     `int d = size - offset;` before or after the store; `int h = at_high;`
//     before or after the store; all three together in six orders; the call
//     first, into a temporary, and both orders of a comma; braces, a hex case
//     label, `break`, an explicit `default`, a duplicated store; stores through
//     `Range&`, `int& lo`, `int& hi`, `&out->low`, `(*out)`, `out + 0`; inline
//     SetLow/SetHigh/Store/SetRange/Set members, with the call as an argument and
//     with a temporary; a `Diff(size, offset)` arithmetic helper and a two-step
//     `Hi(at_high, 0)`; six prototypes for FUN_004b7381 (unsigned, long, unsigned
//     long, char, unsigned return); the pre-switch without `lo`, with `offset`
//     before `size`, with `size`/`offset` split into two statements, with
//     `table[j].field_4` written out twice, `size` derived from `lo`, and seven
//     loop wordings; `volatile int at_high` and `*(volatile int*)&at_high` (cl 5
//     hoists a volatile stack load anyway); flags without /Gz and with /Gd, /Gr,
//     /Ot, /Ox, /frandom (all 80.0%). A copy of the switch expression
//     (`int k = i; switch (k)`) is 80.0% too, and swapping which parameter feeds
//     the store and which feeds the first argument is 78.9%, so the two
//     parameter roles above are the original's.
//   * Next attempt: the trigger is register pressure around the call in case 1,
//     so look for a shape that keeps a value live across it without an extra
//     instruction and without demoting out from esi, or for a loop form whose
//     graph the allocator numbers differently while emitting the same bytes.
//   * Scratch harness left behind: build/scratch/0x4c71f0/{fast,blocks,dump}.py
//     with run.sh, f.sh, b.sh, c1.sh, d.sh and gen.py..gen16.py.
// claude-sonnet-5-5 (#4423): still 80.0%. Permuter (12 min, 420 candidates) found nothing; all 24 orders of hi/lo/size/offset declared at the function top and assigned after the loop (the 0x4c90b0 lever) are byte-identical. Original case 1 evaluates in strict right-to-left order (G in eax, then size-offset, then at_high loaded last into ecx); case 2 loads at_low into edx before the first push and at_high into eax after the result store.
//
// 30-min checkpoint (DeepSeek V4.1 Flash, #4154): still 80.0%, 280 bytes, no
// MATCH; the body below is unchanged and remains the best. This pass measured
// several hundred fresh shapes with a fast local harness
// (build/scratch/0x4c71f0/harness.py, 0.3 s per compile, six workers in
// search1.py) and pinned the mechanism down further; nothing moved the two
// hunks, so none is kept. The shape of the residual is now clear and it is
// worth writing down for the next attempt.
//
// Mechanism (all four blocks agree, including the two that match):
//   * cl5 evaluates a call's arguments right to left. Experiment
//     `out->low = F(at_high, at_low - value, G);` with no store gives
//     `mov eax,[G]` (5-byte accumulator form), then arg2, then arg1, i.e. G
//     gets eax. Case 3 of this function is that same order and matches.
//   * A load is then emitted as early as its destination register allows:
//     a stack-parameter load may be hoisted above a pointer store (case 3
//     proves it), a global load may not (aliasing).
//   * The allocator gives each new value the register that was freed by the
//     instruction before its load. Case 0 (matching): at_low takes eax, the
//     register `sub ecx, eax` just freed. Case 3 (matching): at_high takes
//     edx at the top, then G takes ecx.
//   * Our case 1: at_low->edx, sub frees eax, at_high takes eax (hoisted
//     above the store, which the stack load is allowed to cross), store frees
//     edx, G takes edx. The original instead leaves eax to G and gives
//     at_high ecx, the register arg2's push frees; that one register choice
//     is the whole difference. Same in case 2: the original gives at_high
//     eax after `mov [ecx],eax` frees it; ours gives it edx before add esp.
//   * So both failing hunks are one allocator choice: the original hands the
//     register freed by the preceding instruction to the value loaded LAST
//     (arg1 / the after-call store), ours hoists the same value into the
//     first free register. The 1-byte length gap follows: only eax gets the
//     5-byte `mov eax, ds:[G]`.
//
// Measured this pass, every one 80.0% and byte-identical to the body below
// unless noted, so do not retry them:
//   - all six declaration orders of `int h = at_high; int d = size - offset;
//     int g = DAT_0051fe40;` declared after the store (the 0x461990 pattern,
//     where declaration order does drive the load order); declared before the
//     store they are 78.9% because the global load then crosses the store.
//   - comma forms `out->high = F(at_high, size - offset,
//     (out->low = at_low, DAT_0051fe40));`, the comma in argument 1, the two
//     statements joined by a comma, and the whole pair wrapped in parentheses:
//     the front end normalises the store back out, byte-identical.
//   - inline helpers owning the whole case body with the five parameters in
//     six orders, including reversed, and helpers copying the parameters to
//     locals first (the inliner re-orders the binding back).
//   - `const int at_high`, `const int at_low`, `long at_high`, `unsigned`
//     everywhere (77.8%): no change or worse.
//   - flags: /Ot /Ox /Og /Oi /Oy /GA /GE /G5 /GB are all byte-identical;
//     /G6 is 75.7%, /Oa and /Ow 59.4%, /Os and /O1 28.2%, /Od 16.1%.
//   - defining the sibling 0x4c70d0 or 0x4c7080 before or after this function
//     in the same file: byte-identical.
//   - `*(volatile int*)&at_high` for case 1's first argument: cl5 folds it,
//     byte-identical (no extra load).
//   - `default: return;` before case 3, braces around case 1, `break` instead
//     of `return`: byte-identical.
// Next attempt: the only lever left is the allocator's choice of the register
// for the value loaded last in a block that has a store before the call. It
// must be made busy at the top of the block so the load cannot hoist; every
// source spelling tried so far leaves eax free there. Do not spend budget on
// headers, flags, helpers or declaration orders again.
//
// GPT-6.1-sol retry in #3210: two checker invocations, one initial attempt returned no output. Best remains 80.0%; no MATCH. Existing passes already tried helper, header, declaration, and argument-order variants; case 1/2 argument scheduling still differs.
// #3006 retry by GPT-6.1-sol: two checks retained 80.0%; an inline helper for
// the first case's low output did not change the argument-evaluation mismatch.
//
// Pass (deepseek-v4.1-flash, #2437 retry): still 80.0%, 280 bytes. Four more
// source shapes, every one byte-identical to the body below, so none is kept:
// `int* ph = &at_high;` used as case 1's first argument (the address-taken
// load is folded straight back to the slot and still hoists into eax);
// inline `Range::SetLow`/`SetHigh` methods for the case 1 stores; the stores
// through `(Range*)(int)out` to defeat aliasing knowledge; and a `Pair`
// sub-object layout for Range (the 0x463610 member-sub-object lever). All four
// leave case 1 with at_high hoisted into eax before `mov [esi],edx` and the
// global in edx, and case 2 with at_low in ecx after `push ecx`. That closes
// the aliasing, inline-method and sub-object readings on top of the earlier
// passes, so the residual really is the arg1-vs-arg3 register priority tie
// already documented by the sibling 0x4c70d0.
//
// Pass (deepseek-v4.1, #2097 retry): still 80.0%, 280 bytes, six check.py runs.
// New shapes, every one byte-identical to the body below, so none is kept: the
// case 1 store folded into the call as a comma expression in argument 3,
// `(out->low = at_low, DAT_0051fe40)`, or in argument 1,
// `(out->low = at_low, at_high)`; the same comma in case 2,
// `(out->low = FUN_004b7381(at_low, offset, size), at_high)`; (int) casts on
// all three case 1 arguments; `at_low`/`at_high` copied to `lo_`/`hi_` locals at
// the function top and used everywhere in the switch; a `Range* p = out;`
// local; one dead `static int` before the function; and 256 dummy typedefs
// before the function to perturb compiler state. Since even those leave case 1
// with at_high hoisted into eax and the global loaded into edx, and case 2 with
// at_low in ecx and at_high in edx, the two remaining hunks below are an
// allocator/scheduler choice that statement, argument and declaration shape
// cannot move.
//
// Pass (deepseek-v4.1, #2097): 80.0%, 280 bytes, 9 check.py runs. Six new source
// shapes, all 80.0% and byte-identical to the body below (so none is kept):
//   - a `Mix1(a,b)` inline helper whose body loads DAT_0051fe40 into a local
//     FIRST and then calls FUN_004b7381(a,b,g) (hoping the inliner walks the
//     global before the first argument),
//   - a `Mix2(a,b,c)` helper with `int g = c;` as its first statement,
//   - case 1 with `int r;` for the call result stored afterwards,
//   - case 2 with `int r = FUN(...)` then `out->low = r; out->high = at_high;`,
//   - case 2 with DAT_0051fe40 inline as the third argument,
//   - case 1 with the first argument as `(int)(DAT_0051fe40, at_high)` and
//     case 2 with at_low through an `Id(x)` inline identity.
//   Reordering case 1 so the call precedes `out->low = at_low;` (v5) is 276
//   bytes / 77.8%: MSVC 5 sinks nothing, the store stays where it is written.
// Still differs (only these two hunks): case 1 wants the same bytes as the
// original (store, `mov eax,[DAT_0051fe40]` in the 5-byte moffs form, two
// pushes, then `mov ecx,[esp+0x24]` for at_high as the last push); ours hoists
// the at_high load above the store into eax and moves the global into edx.
// Case 2 wants at_low in edx before the first push and at_high in eax after
// `mov [ecx],eax`; ours loads at_low into ecx after `push ecx` and at_high
// into edx before `add esp,0xc`. The front end's argument walk order is the
// remaining unknown; helper shapes that reorder the parameter walk were tried
// above and did not move it.
//
// Fourth pass (space-bunny-free, #1795): still 80.0%, 280 bytes, no MATCH. Four
// new spellings, every one exactly 80.0% at 280 bytes, so none is kept:
//   - `Range& out` as the second parameter. It also settles the parameter type:
//     the original's own mangled name is ...YGXHPAURange@@HH@Z, and the
//     reference form compiles to ...HAAURange@@HH@Z, so the pointer is right.
//   - case 1's third argument through an inlined getter `SizeGlobal()` that
//     returns DAT_0051fe40, i.e. a call node whose result MSVC must put in eax.
//     It does not change the schedule (still hoists at_high into eax).
//   - `unsigned at_low, unsigned at_high`: no change at all.
//   - the tail as `if (size) { switch (i) { ... break; } }` instead of an early
//     return and four returns: no change.
// tools/headers.py over 128 sets: still nothing, flat 80.0%.
// NEW BYTE FACT, useful for the two failing blocks and for the sibling 0x4c70d0:
// the 280-byte window is 263 bytes of code, one 0x90 pad and the 16-byte jump
// table, and our code is ONE byte longer than the original's. The only length
// difference is case 1's global load: MSVC 5 emits `mov eax, ds:[G]` in the
// 5-byte accumulator form (A1 moffs32), but the same load into edx or ecx is 6
// bytes (`8B 15` / `8B 0D` + moffs32). That is why the original's `je 0x4c72f1`
// lands one byte before ours (`je 0x4c72f2`). So case 1 is not only a cosmetic
// rotation: getting DAT_0051fe40 into eax is also what makes the size right, and
// the same 1-byte rule applies to every case of 0x4c70d0.
//
// Second pass (#1547, deepseek-v4.1-flash): still 80.0% (280 bytes). Three
// genuinely new spellings, all 80.0% and the same two hunks, so skip them:
//   - the function declared as a __thiscall member (unused `this` in ecx): no
//     change at all, so ecx is not reserved for this when it is never read.
//   - case 1 with `int g = DAT_0051fe40;` declared BEFORE `out->low = at_low`
//     and used as the third argument: 276 bytes / 78.9%, worse (the declared
//     local is scheduled at its declaration, so the global load moves above
//     the store). The guide's sibling recipe wants the local AFTER the store,
//     which is already in the ruled-out list above.
//   - case 2 with `int lo = at_low;` before the call: no change, MSVC folds
//     the local back and still loads at_low into ecx after the first push.
//   - case 1 with an inlined `Late(a,b,c)` forwarder: no change.
// tools/headers.py over 128 sets: none match, closest 80.0% flat.
// Calling convention checked again (#1188): __stdcall ret 0x10 matches, callee 0x4b7381 is cdecl (add esp,0xc) and is declared so; not the cause. Tried (&at_low)[1] for at_high in case 1: 80.0%, same 280 bytes, case 2 diff unchanged.
//
// deepseek-v4.1-flash pass (#1210), no score change, 80.0% (280 bytes). The jump
// table was dumped from the exe (0x4c72f8 = 0x4c7260, 0x4c7284, 0x4c72ad,
// 0x4c72cf), so the case labels are confirmed correct and the two mismatches are
// purely at_high's materialisation. New spellings tried, all 80.0% unless said:
//   - reordering the case bodies in the source (0,2,1,3 and 1,2,0,3): 74.6% and
//     56.2%, so body emission order is load-bearing here as in 0x4c70d0.
//   - writing the case 1 store through `int& low = out->low` or `Range& r = *out`
//     (the 0x41ba60 lever for a load MSVC hoists above a store the original keeps
//     first): unchanged, so that lever does not apply to a hoisted stack-parameter
//     load.
//   - a `const int&` alias for at_high, and a local `int h = at_high;` placed
//     after the store: unchanged.
//   - an inline three-argument forwarder `Forward(a,b,c)` returning
//     FUN_004b7381(a,b,c), and a reversed one `Rev(c,b,a)` calling FUN(a,b,c):
//     both 80.0%, so the inlined-call boundary does not change the schedule here.
//   - `int* out` with out[0]/out[1] instead of Range*: 80.0%, identical bytes.
// The 80.4% variant that passes `value` for case 1 stays rejected: it reads
// parameter 1, not parameter 4 (see the stack arithmetic below).
// Claude Sonnet 5.5 pass (#624): compiler state ruled out (0 to 400 unused `extern
// int` declarations in steps of 8, all 80.0% and 280 bytes). More source shapes
// scored, none moved case 1 or case 2: an inline helper `Scale(a, b)` that reads
// DAT_0051fe40 itself and calls FUN_004b7381(a, b, DAT_0051fe40) in all four cases
// (80.0%; with the global copied to a local first it is 260 bytes and 58.8%), the
// last two parameters as one by-value `Range at` (80.0%, identical bytes), and in
// case 1 a local for size - offset, a `Range* o` alias for the stores, a local for the
// call result, a local copy of at_high, the call as the last statement or the
// first (all 80.0%; passing `size` instead of DAT_0051fe40 or putting the call
// first is 276 bytes, 78.9 and 77.8%). What the original does differently in cases
// 1 and 2: it loads at_low into edx at the top of the case, before the first push
// (case 2 `mov edx,[esp+0x18]; push ecx; push eax; push edx`), and in case 1 it
// loads at_high into ecx only after `push eax(global); push ecx(size-offset)`, i.e.
// as the last push; ours loads the second parameter early into eax and moves the
// global into edx. In case 2 the store of at_high is `mov eax,[esp+0x1c]` after the
// call; ours hoists that load above `add esp, 0xc`.
//
// Partial: 80.0%. The search loop, the tail arithmetic, the size test, the
// jump table, case 0 and case 3 match byte for byte, and cases 1 and 2 differ
// only because of one thing, case 1 (see below).
//
// Case 1's first argument is the FOURTH parameter, at_high, not value. The
// stack arithmetic settles it: the prologue pushes three registers, so esp is
// entry-0xc, and by the time of the load two arguments have been pushed, so
// `mov ecx, [esp + 0x24]` reads entry+0x10, which is parameter 4. The same
// holds at `mov edi, [esp + 0x10]` in the prologue (entry+4, parameter 1) and
// at `mov esi, [esp + 0x14]` / `mov edx, [esp + 0x18]` in case 2 (entry+8 and
// entry+0xc, parameters 2 and 3), so the offsets are all consistent.
// An earlier version of this note claimed it was parameter 1 and changed the
// argument to `value`; that scores 80.4% against this file's 80.0% but is the
// wrong parameter, and the 0.4% is not worth reading the wrong slot. Passing
// `at_high` is also what gives the right size, 280 bytes against 276.
//
// What still differs is one thing in case 1, and it is a placement decision
// rather than a missing value. The original never keeps at_high in a register:
// it re-loads the slot at its point of use, into ecx, after the third and
// second arguments have already been pushed, because ecx is still holding the
// second argument until then.
//     mov eax, [DAT_0051fe40]      ; third argument
//     push eax
//     push ecx                     ; second argument (size - offset)
//     mov ecx, [esp + 0x24]        ; at_high, reloaded here
//     push ecx
//     call FUN_004b7381
// Here MSVC 5 hoists the at_high load to the top of the block into eax and
// leaves ecx free, so the global ends up in edx and the pushes come out in a
// different order. Cases 0, 2 and 3 and all the pre-switch code match.
//
// Tried on top of this version, none of which moved it:
//   - `volatile int at_high` as the parameter, which is the natural reading of
//     "re-loaded from its slot on every use" and is the exception the current
//     AGENTS.md allows. It changes nothing: the load is still hoisted into eax.
//   - an `int g = DAT_0051fe40;` local read after `out->low = at_low;`, to try
//     to force the global into eax the way the original has it. Unchanged.
//   - `value` as the argument (80.4%, 276 bytes): wrong parameter, see above.
// The rest of the ruled-out list from the previous pass follows.
//
// Tried and measured, all on top of this version (80.4% unless said):
//   - an inlined search helper that owns the loop and takes `value` as its
//     own parameter (the 0x4523e0 pattern from the guide): the pre-switch
//     code still matches byte for byte, but the parameter copy is propagated
//     and case 1 still gets `push edi`.
//   - the same helper also returning the offset (272 bytes, 47%: it breaks
//     the tail's register order) or the offset and the size (80.4%, still
//     propagated).
//   - re-reading the parameter as `*(int*)&value`, `*(long*)&value`,
//     `*(unsigned*)&value`, through a `const int&` local, through an
//     `int*` local, through a reference or pointer parameter of an inlined
//     helper, `value + zero`, `(int)(long)value`, `sizeof(int) ? value : 0`,
//     a nested comma assignment, a local for the distance, a local for the
//     call result, a `Range&` for the stores: all propagated to the edi copy.
//   - `at_high` as the anchor (80.0%, 280 bytes: the right size but the
//     wrong value), reversed statements, comma expressions, `out[0]/out[1]`.
//   - tools/headers.py over 768 header sets (each of windows.h, stdio.h,
//     stdlib.h, string.h crossed with none and the C++ headers): all 80.4%.
//   - `*(volatile int*)&value` gives the wanted load and register but also a
//     second, mandatory load of the same slot at the top of the case, so it
//     is 280 bytes and still 80.0%; not a real reading of the original, since
//     a volatile parameter would also be re-read inside the loop.

// GLOBAL: 0x51fe98
extern int DAT_0051fe98;
// GLOBAL: 0x51fea0
extern int DAT_0051fea0[];
// GLOBAL: 0x51fef8
extern struct Chunk* DAT_0051fef8;
// GLOBAL: 0x51fef4
extern int DAT_0051fef4;
// GLOBAL: 0x51fe40
extern int DAT_0051fe40;

struct Chunk {
    int field_0;
    int field_4;
};

struct Range {
    int low;
    int high;
};

int __cdecl FUN_004b7381(int a, int b, int c);

// FUNCTION: 0x4c71f0
void __stdcall FUN_004c71f0(int value, Range* out, int at_low, int at_high)
{
    int i = DAT_0051fe98;
    Chunk* table = DAT_0051fef8;
    int j = DAT_0051fea0[i];
    DAT_0051fef4 = j;
    while (value > table[j].field_4) {
        j = DAT_0051fea0[j];
        i = DAT_0051fea0[i];
        DAT_0051fef4 = j;
        DAT_0051fe98 = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    DAT_0051fe40 = size;
    if (size == 0) {
        return;
    }
    switch (i) {
    case 0:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = 0;
        return;
    case 1:
        out->low = at_low;
        {
            // The distance is passed through a pointer to a local. It reads
            // like a leftover from the original, but it is what puts case 1's
            // global reload in the 5-byte accumulator form (see the notes).
            int span = size - offset;
            int* spanp = &span;
            out->high = FUN_004b7381(at_high, *spanp, DAT_0051fe40);
        }
        return;
    case 2:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = at_high;
        return;
    case 3:
        out->low = 0;
        out->high = FUN_004b7381(at_high, offset, DAT_0051fe40);
        return;
    }
}