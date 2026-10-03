// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by GPT-6. Names are provisional.
// Claude Opus 5.5 pass with tools/c2prio.py (#5260, 2026-10-03): still 84.7%.
// The residual is one colouring-order decision, now measured, not guessed:
// - The x difference (the b.x param web) has priority 74 and is coloured after
//   n (130) and the |dx| temp (104). Both of those take ecx first, so dx is
//   left with esi. In the original dx is in ecx and |dx| and n are in esi.
// - Proof that priority is the whole cause: three extra stores of b.x to
//   globals right after the subtractions lift dx to 134. The prologue then
//   becomes the original's exactly (b.x hoisted into ecx, |dx| and n in esi),
//   apart from the stores themselves.
// - Under C2's rule (w * K * cost per block) this IL cannot get there. dx has 3
//   refs in the subtraction block B0 and 1 in the division block. The |dx|
//   temp has 5 refs in B0. So dx loses to it for every K, which means the
//   original's IL must give dx references outside B0, or put |dx| in another
//   block. A second route: an uncoloured dx that prefers ecx (a copy with a
//   candidate already in ecx) would add an ecx cost to n and |dx|. No natural
//   source for either was found.
// - n stays at 130 in about 60 shapes (Steps/Div/Sub helpers by value and by
//   reference, template Max, loop forms, `fordec`, labs, __max). Computing n
//   in each arm (`if (abs(d.x) < abs(d.z)) n = abs(d.z) / 0x100000; else
//   n = abs(d.x) / 0x100000; n++;`) drops n to 96 with byte-identical output.
//   It still leaves dx at 68, below the |dx| temp at 106.
// - Also flat or worse: the Fixed union Vec3 from 0x4853b0 (84.7%), scalar
//   int parameters (322 bytes), the divisions moved into the loop (351
//   bytes), `d.z` divided first (76.6%), and a permuter run from the per-arm
//   form (seed 77, 16 minutes, 18499 candidates, 84.7% flat).
// Codex GPT-6 retry for #5210 (2026-10-03): current main remains 84.7%.
// #5322 Codex retry: re-confirmed 84.7%; the x-difference QFIELD/temp
// coloring order remains as measured in the C2 notes below.
// #5356 retry: re-confirmed 84.7%; the recorded 33-shape sweep still covers
// the source forms that could reverse the x/y difference register pairing.
// #5360 retry: re-confirmed 84.7%; the measured register-pair mismatch is
// unchanged on current main.
// #5379 retry: re-confirmed 84.7%; the x-difference and abs/divisor registers
// still receive the opposite allocation from the original.
// Existing frame/register probes and the prior 19019-candidate permuter found
// no better source; `/Gi` is already recorded at 46.4%.
// Claude Opus 5.5 retry (#5072): still 84.7%, about 1900 scratch variants, none
// emits the original's third instruction (`mov ecx, [esp+0x1c]`). New facts:
// - The x difference reaches ecx (b.y in eax, b.x in ecx, both hoisted, the
//   original's pair) only when n's computation is gone: `int n = 7;`, or the n
//   expression repeated in each use instead of named (424 bytes). So the x
//   difference loses ecx to n's chain, not to the subtraction's spelling.
// - n is not the lever in any natural shape: n used a third time, in the loop
//   body, at the return, as the loop variable, declared first (with d, i, best
//   in any order), behind a folding `if (n >= 0)` (81.5%), as `register`, via
//   `int m = n`, or computed by a Steps() inline helper (value, reference,
//   pointer, Vec3 by value): the x difference stays in esi every time.
// - MSVC splits live ranges here: with b.x also read in the loop, its prologue
//   range still gets esi and is spilled to a new slot for the loop.
// - `for (b.x = 0; b.x <= n; b.x++)` compiles byte-identically to `int i`,
//   which is why the down-counter lives in b.x's parameter slot.
// - Renaming a, b, d or n changes nothing (no name-order tie). A field-wise
//   copy constructor or operator= loses the d.x dead store (350 bytes); `ABS`
//   macros give jns/neg, not cdq/xor/sub; an int[3] member behaves as fields;
//   a non-POD step type (ctor, dtor) is still promoted; /Gi is 44 to 48% for
//   every shape tried.
// - Also flat: 0x485010, 0x485070 and 0x485140 defined above this function in
//   one file, the prologue in a nested block or an inline MakeStep() helper,
//   `d = a` then `d.F = b.F - d.F`, and loop-body rewrites (a `Game* g` local,
//   a Higher() max helper, a `Unit*` local): the prologue never moves.
// - Random sweeps (build/scratch/0x4851c0/gen.py, gen2.py: in-place, copy,
//   helper, scalar and mixed step shapes, decl orders, three max spellings,
//   division forms; 1600 files) top out at this file's 84.7%, and so does
//   tools/permute.py (seed 51, 15 minutes, 19019 candidates, score 355 flat).
// GPT-6 retry (#4971): baseline remains 84.7% at 354 bytes. classify.py calls
// this a frame diff (15 register changes, 8 instruction insertions/deletions);
// stackcmp still places every local. A scratch /Gi build scores 46.4%.
// Rechecked for issue #5130 on 2026-10-03; the same prologue/register hunk
// remains at 84.7%.
// DeepSeek V4.1 Flash pass (issue #4761): still 84.7%, 354 of 354 bytes, size
// exact. tools/permute.py ran the HARD-CAPPED 3 minutes / 3268 candidates and
// stayed flat at 84.7% (score 355); stackcmp reports no moved local, so no
// --stack names. The same single hunk remains: the original loads b.x into ecx
// before push ebx (dx in ecx, abs(dx)/n in esi); ours loads b.x after the pushes
// into esi (dx in esi, chain in ecx). Every other instruction is identical.
// Stopping: the score has not moved in any prior pass nor in this one.
// Space Bunny Free pass (issue #4682): still 84.7%, 354 of 354 bytes, the file's
// version left in place. New result this pass, and the reason the earlier notes'
// searches kept missing: there is a much better screen than check.py's ratio, and
// it settles the question. It is the register PAIR, not the percentage.
// TOOL FOR THE NEXT PASS: build/scratch/0x4851c0/probe.py compiles a whole
// directory of variants in parallel (one out_dir per thread) and prints, per
// variant, the byte size, how many leading instructions are textually identical
// to the original's, the check.py percentage, and the (dx, abs) register pair.
// 16 variants take 0.6 s against 7 s for one check.py run, and the pair moves
// with no movement at all in the ratio, so the pair is the screen. dump.py
// prints a full unified diff of a variant against the original and cmp.py diffs
// two variants against each other.
// 1. THE RULE, measured over 33 fresh shapes (build/scratch/0x4851c0/b1, b2):
//    write the x difference either as a QFIELD (in place on the by-value
//    parameter, `b.x -= a.x`, which is what the dead stores require) or as a
//    level-0 temp (a `static inline` helper result, `d.x = Dx(a, b)`, or
//    `d.x = b.x - a.x` into a local's field), and the pairing is forced:
//      x difference a QFIELD   ->  y difference eax, x difference esi
//                                  (a callee-saved one; the load of b.x cannot be
//                                  hoisted, which is the whole residual), and
//                                  the abs/divisor chain then takes ecx;
//      x difference a temp     ->  x difference eax, y difference ecx, both
//                                  loads hoisted, in the order b.x then b.y;
//    and in the second case that is true for EVERY source order and every
//    spelling (in place on b, in place on a local, through a helper, through a
//    scalar local, the field or the parameter as the destination, y first or x
//    first in the source): 12 of 12 shapes give b.x -> eax and b.y -> ecx. The
//    only shape that hoists the loads in the ORIGINAL's order, b.y then b.x, is
//    the all-helpers one (both differences through helpers into d's fields), and
//    there the registers are still b.y -> ecx and b.x -> eax, i.e. the original
//    with its two scratch registers exchanged, at 358 to 360 bytes.
//    So the original is "a QFIELD y difference and a temp x difference that is
//    walked second", and no spelling reaches it: a temp x difference is always
//    walked first.
// 2. WHY the walk order cannot be flipped from the source, which is new: the
//    walk follows the QFIELD number, and the QFIELD number follows the field
//    order of the struct. With {x, y, z} d.x is field 0 and is always walked
//    first, in every shape measured. The one way to put the y difference first
//    is to declare the struct {y, x, z}, and the frame forbids it: the loop
//    reads the x step from [esp+0x10] (= entry esp - 0xc + 0, the first local
//    dword) and the z step from [esp+0x18] (= the third), so d.x is the first
//    field. The dead store of the y difference into that same first slot is the
//    MSVC 5 slot mixup earlier passes reported, not evidence for a different
//    field order: with {y, x, z} the post-division store of d.x would land in
//    [esp+0x14], and the original's lands in [esp+0x10].
// 3. Cross-check against the exe, redone this pass with the two-hoist-plus-
//    `push ebx` prologue as the pattern: 23 hits, 19 of them already MATCHed, so
//    the near-copy is a dead end. The two that resemble a subtraction of two
//    by-value struct fields, 0x44d350 (`int dx = abs(px - x);`) and 0x440830
//    (`int left = a.x - field_4;`), get a QFIELD of a by-value parameter into
//    ecx/edx only because the subtraction sits INSIDE the abs as a level-0
//    expression temp; here that means spelling the x difference twice (once for
//    the max, once for d.x), which is 356 bytes. Both spellings of that were
//    scored this pass: naming the difference in a local that the max reads
//    (`int rx = b.x - a.x; d.x = rx; n = (abs(rx) < abs(d.z) ? ...)`, and the
//    three-field version) gives 337 to 360 bytes, and `abs(b.x - a.x)` spelled
//    twice gives 354 bytes at 83.9%, both with dx still in esi or eax.
// 4b. Two more families scored this pass, both aimed at making the x difference
//    a level-0 temp WITHOUT naming it in a local: an addressable d (a pointer
//    derived from the local, `pd->x = b.x - a.x`, and a `static inline` setter
//    that takes the difference as an argument, `SetX(&d, b.x - a.x)`), with the
//    y difference in place, with both differences through setters, and the
//    setters taking a pointer or a reference. Every one that keeps the in-place
//    preamble and the 354 bytes still gives b.x -> eax and b.y -> ecx (77.4%),
//    and the one that re-assigns d.x after the in-place x subtraction
//    (`SetX(&d, d.x)`) is byte-identical to this file at 84.7%: making d.x
//    addressable does not change its class, so the destination cannot be what
//    decides it either.
// 4. Also scored this pass, all 84.7% or worse: the n expression in eleven
//    spellings (named max local, named abs locals, an AbsI helper, a MaxAbs
//    helper, `1 + m / 0x100000`, `>> 20` instead of `/ 0x100000`, `n++` on its
//    own line, unsigned and long and short and char and unsigned short n), the
//    three differences in six orders with three spellings each crossed with
//    those n expressions (115 files), a `Vec3&` alias and a `Vec3*` alias of the
//    parameter, the copy written twice, `-=` versus `= b.f - a.f`, and the
//    division order. tools/headers.py, all 128 sets, flat at 84.7%.
// 4c. The best lead the exe offers, found by scanning for the WHOLE preamble
//    shape rather than the prologue (scan3.py, scan4.py in the same scratch
//    folder: every function with a `sar reg, 0x14` and two register-register
//    `sub`s; 0x4851c0 is the only one in the exe with this shape, so there is
//    no near-copy). The one analogue worth reading is 0x47e2d0 (MATCHed, 658
//    bytes): `short a = (v.x - (footprint.x << 19) + 0x80000) >> 20;` and the
//    same for z land in EAX and ECX, two scratch registers, both loads of v's
//    fields hoisted. The difference from here is visible in that source: its
//    SUBTRAHEND is a level-0 expression temp (`footprint.x << 19`), ours is a
//    QFIELD of a by-value parameter. So the rule to test next is "a subtraction
//    whose subtrahend is an expression temp lands in a scratch register". The
//    three ways of making a.y/a.x a temp that keep the code otherwise identical
//    were all folded by MSVC 5 this pass and leave the prologue byte-identical:
//    a `static inline int Sub(int m, int s)` helper (all four of the y, z, x,
//    and helper-vs-in-place combinations, 81.5% with this file's prologue), a
//    `static inline void SubX(Vec3&, int)` member helper, and a named local
//    holding the field. Only a shift, which changes the arithmetic, survives,
//    and that cannot be the original's source.
// 5. tools/permute.py, as the 0x47e5c0 note suggests, run from the WORSE
//    plausible shapes rather than the best file, with --jobs 4: from the 67.2%
//    all-helpers shape (seed 31, 5159 candidates) it reached 70.4%, and from the
//    77.4% `b.y -= a.y; b.z -= a.z; d = b; d.x = b.x - a.x;` shape (seed 32,
//    3880 candidates) 77.4%, i.e. neither beats the file. Both stalls, both
//    checked with check.py afterwards (360 bytes and 354 bytes).
// Space Bunny Free pass (issue #4667): still 84.7%, 354 of 354 bytes, size exact,
// the file's version left in place. A systematic 750-shape sweep and two of the
// three levers tried on other functions today both failed, but the sweep pins the
// remaining question much more tightly than any earlier note:
// 1. THE EXACT ORIGINAL PREFIX IS UNREACHABLE IN THE WHOLE FAMILY. I enumerated
//    every shape of the form <in-place statements on b, in any subset and any
//    order> + `Vec3_004851c0 d = b;` + <the rest, in any order>, with each of the
//    three differences independently spelled as one of five mechanisms (in place
//    on b, in place on d, `d.F = b.F - a.F`, `d.F = DF(a, b)` through a
//    `static inline` helper, `b.F = DF(a, b)`): 6 orders x 5^3 = 750 files, all
//    compiled. Best is the file's 84.7%; the tightest miss is 84.7% itself, and
//    NOT ONE of the 750 emits the original's first four instructions
//    (`sub esp,0xc; mov eax,[b.y]; mov ecx,[b.x]; push ebx`). Across all 750 the
//    load of b.x lands only in eax (hoisted, when the x difference is a level-0
//    temp), ebx, esi, ebp or edi, never in ecx.
// 2. NEW: the y-first load order with BOTH b.y and b.x hoisted into caller-saved
//    registers before `push ebx` is reachable in 52 of the 750 shapes (mechanism
//    set iDD / ihh / iDh / hiD / hDh / hDi and their perms), and in every one of
//    them the pairing is b.y -> ecx and b.x -> eax, i.e. the original's two
//    scratch registers exactly exchanged, as the earlier passes found. Those
//    shapes are all 360 bytes (six too many: d.z and d.y go to stack slots
//    instead of staying in edi/ecx, and a.y/ebp swap appears), so flipping the
//    pairing there would not reach a MATCH either. Best of them is 70.4%.
// 3. NEW, and this is why the pairing cannot be fixed by reordering: the pairing
//    is a WEIGHT effect, not a walk-order effect, and the weight is on the VALUE,
//    not on the field. In the all-in-place shapes whose first source statement
//    is not the y one (orders zxy, xzy, zyx: 354 bytes, 83.9%) MSVC also hoists
//    TWO loads, `mov eax,[b.y]; mov ecx,[a.x]; push ebx; mov ebx,[a.x]`, and
//    there b.y keeps eax and the SECOND hoisted value takes ecx. So b.y holds eax
//    whenever the competitor for the second scratch register is not b.x, and
//    loses it only when the x difference is a level-0 temp, in which case dx (four
//    register uses: the abs, the max compare, the idiv, the store) outranks dy
//    (one use: the dead store) and takes eax. The original has it the other way
//    round, so the original's dy must outrank its dx, which the emitted code does
//    not admit, unless the two are not weighed against each other at all.
// 4. REFUTED, the weighted-use lever from the guide (0x424890): thirteen neutral
//    extra uses of d.y (`+= 0`, `-= 0`, `*= 1`, `|= 0`, `-(-d.y)`, `d.y = d.y`,
//    two, three and four stacked copies, `d.y += 0; d.z += 0;`, and `b.y = b.y`)
//    plus a `d.x += 0` control, applied to three shapes (the file's in-place
//    baseline, the 70.4% `d.x = b.x - a.x; d.z = b.z - a.z;` shape and the 67.2%
//    helper shape) compile BYTE-IDENTICALLY in all 39 cases: 84.7%, 70.4% and
//    67.2% unchanged, same first four instructions. MSVC 5 folds those in the
//    front end, before the allocator ever weighs the value, so no amount of dead
//    arithmetic moves the tie.
// 5. RULED OUT this pass, both tried on other functions today and neither of them
//    touches the prologue here: (a) 0x47eee0's loop-invariant assignment as the
//    first statement of a loop body, in all seven of its spellings here
//    (`a = a;`, `b = b;`, `d = d;`, `n = n;`, `i = i;`, `best = best;`,
//    `c = c;`) placed at the top of the difference loop's body, and the same seven
//    placed just before the loop: every one is exactly 84.7% with a byte-identical
//    prologue. (b) 0x48b090's self-conditional phi, `d.x = d.x ? d.x : d.x;` and
//    `a.x = a.x ? a.x : a.x;` and `n = n ? n : n;` at the top of the loop body, and
//    `d.x`, `d.y`, `b.x`, `a.y` in the prologue: all 81.5%, same prologue, only the
//    loop perturbed. So this function's tie is decided before either of them runs.
// 6. A note on the original code itself, not on the source: the local the loop
//    reads as d.y (frame slot [esp+0x14], `mov eax,[esp+0x14]; add edx,eax` at
//    0x4852fa for `a.y += d.y`) is NEVER WRITTEN anywhere in the function. The
//    y step only ever reaches [esp+0x10] (0x4851de, the dead store, immediately
//    overwritten by the x step at 0x4851f4). So a.y accumulates whatever was on
//    the stack. It is harmless (a.y is dead after the loop) and our source
//    reproduces the machine code exactly, but it is a real defect in the shipped
//    code and worth reporting.
// Tooling note for the next pass: build/scratch/0x4851c0/score.py scores a whole
// directory of variants in parallel (ThreadPoolExecutor over
//    check.compile_source, one out_dir per thread, build/objS<n>) and prints the
//    first 26 normalised instructions per variant, which is what makes a 750-shape
//    sweep readable: screen the prologues, not the ratios, since a register-only
//    difference does not move the ratio at all.
// Space Bunny Free pass (issue #4573): still 84.7%, 354 of 354 bytes. Nothing
// beat the file's version; this pass's value is the mechanism, below, and one
// new measurement that pins the search.
// 1. The two differences are ONE allocator decision, and it is about which of
//    b.y / b.x / a.y / a.x gets a temp register (eax/ecx) rather than a
//    callee-saved one. The original hoists two stack loads above `push ebx`
//    (b.y -> eax, b.x -> ecx) and gives the two subtrahends a.x -> ebx and
//    a.y -> esi; ours hoists only b.y -> eax and hands b.x the esi that a.y
//    has just vacated. The tied pair is (b.x, abs(dx)/n), ours in esi/ecx and
//    the original's in ecx/esi.
// 2. NEW: the natural const-correct shape `Vec3 d; d.y = b.y - a.y; d.x = b.x
//    - a.x; d.z = b.z - a.z;` is the original MINUS EXACTLY ONE INSTRUCTION.
//    Verified instruction by instruction with a byte-level differ: 123 of our
//    124 instructions are textually identical to the original's, and the one
//    missing is `mov ecx, [esp+0x1c]` (b.x hoisted into the second temp
//    register). That is the whole 4-byte size gap (350 vs 354) and the whole
//    score gap. So the frame layout, the dead stores, the copy's slot mixup,
//    n, the divisions and the loop are all confirmed right; only the register
//    class of the b.x live range is wrong. Order y,x,z is best (75.3%,
//    y,z,x 69.6%), the other four orders 68.0 to 75.3%.
// 3. NEW: a QFIELD whose base is a fixed frame slot (a by-value struct
//    parameter's field) is a level-1 range var and can only get a callee-saved
//    register, but MSVC 5 does put such a field in eax/ecx when the load is
//    hoisted above the pushes. Measured: with the subtractions in a non-y
//    first order (`b.x -= a.x; b.y -= a.y; b.z -= a.z;` and the four other
//    non-y-first orders) MSVC hoists TWO operand loads and both go to temps
//    (b.y -> eax plus a.y -> edx or ecx), still 354 bytes, 83.9%. With any
//    y-first order only b.y is hoisted. So the hoist count is decided by the
//    order of the first statement, and the original's hoist of b.x (the
//    MINUEND of the second subtraction, not the subtrahend of the first) is
//    a third pattern that no order produced.
// 4. Cross-check against matched files, for the next attempt: no matched
//    function among the 3076 in data/progress.csv has this prologue (two
//    stack-argument loads into temps before `push ebx` and then `sub <temp>,
//    <callee-saved>` twice), so there is no near-copy to copy. The closest
//    analogues are 0x44d350 and 0x440830, both `abs(a - field)` shapes: there
//    a QFIELD of a by-value struct parameter (0x440830's `a`, 0x44d350's `px`)
//    does land in ecx/edx when its result goes to a named local, because the
//    operands are level-0 expression temps. That is the one spelling here not
//    yet tried: put the x difference in a NAMED LOCAL int whose result is then
//    copied into d.x, with d.x read from that local rather than from d.
// 3b. The pair {a.y, b.x} is the tie, and the allocator hands out one temp
//    and one callee-saved register between them: the original gives the temp
//    to b.x (b.y, b.x in eax/ecx) and the callee-saved to a.y (esi), every
//    non-y-first order gives the temp to a.y (b.y, a.y in eax/ecx or edx) and
//    the callee-saved to b.x, and the y-first in-place form gives esi to a.y
//    and then reuses it for b.x. No source shape moved the temp from a.y to
//    b.x.
// Scored this pass, all 84.7% or worse (about 400 variants in total): 30 unused
// inline functions in the translation unit (N = 1,2,3,4,8 x six bodies),
// which the 0x4c06e0 note says can fix a tie, does nothing here; twelve
// self-assignments (`b = b;`, `d.y = d.y;`, `d.y += 0;`, `d.y *= 1;`, `n = n;`
// and friends) before and after the copy, all exactly 84.7%; one extra unused
// `int` local in four positions, all 84.7%; the difference written into a
// temporary and copied to d in six orders (83.9 to 84.7%, `Vec3 t; t.x =
// b.x - a.x; ... Vec3 d = t;` reaches the same 84.7%); a used `static inline`
// helper doing the three in-place subtractions, taking `Vec3&` or
// `Vec3*` (83.9 to 84.7%); a helper taking the Vec3 by value, by reference
// or by pointer that returns the whole difference (80.6 to 81.5%); scalar
// `Sub(p,q)`, `AbsI(v)`, `MaxI(p,q)` helpers (62 to 81.5%); per-field readers
// through a reference or a pointer (58.7 to 72.1%); a member function
// `int Sub(const Vec3&) const`; the destination reached through `Vec3* pd =
// &d` or `Vec3& d = tmp` (64.8 to 72.1%, 350 bytes); the difference spelled
// twice, once into d and once in the max (356 bytes, 63.5 to 67.5%); and
// `d.f = b.f - a.f`, `b.f -= a.f`, `b.f = a.f - b.f`, `b.f -= -a.f` in all six
// orders in both the in-place-then-copy and the fresh-d shapes.
// Also flat or worse: the x difference in a named local int with d.x read from
// it (eleven variants, 71.3 to 82.3%, the best being `Vec3 d = b; d.y -= a.y;
// d.z -= a.z; int dx = d.x - a.x; d.x = dx;` at 82.3% and 354 bytes with the max
// still reading d.x); the parameters declared in the other order, `b` first,
// which is 354 bytes but 77.4 to 78.2% (63.2 to 68.8% for the fresh-d form);
// all three subtractions in one comma statement (83.9 to 84.7%); the same
// field read twice, once plainly and once through an inline getter, for d.x,
// d.z, b.x and the loop's a.x (66.7 to 81.5%); `Vec3` as a union (51.6%), as
// an aggregate initialiser of the three differences (74.5%), and a field
// reached through `*(int*)&b.x` (77.4%); the copy written field by field
// before the in-place subtractions, and the minuends read into named locals
// first (68.0 to 76.6%); the loop's three `+=` as an inlined member
// `operator+=`, as a free `AddTo_` helper, or both, in all six body orders
// (64.5 to 81.5%), which is the "inlined function boundary changes the
// register use" lever from the guide and does not work here either; and twenty
// translation-unit perturbations that are real code rather than declarations
// (a class with a constructor and destructor, a class with a member getter used
// from a free function, a template, a function that throws, a try/catch, a
// virtual class, a free `operator-`, a global array with an indexing helper, a
// double function), all exactly 84.7%.
// tools/permute.py, two runs on this file: seed 11 for 8 minutes, 3727
// candidates, and seed 12 for 7 minutes, 1871 candidates. Both 84.7% ->
// 84.7%, no byte moved. That is 5598 mechanical candidates on top of the
// hand-written shapes, and the score has not moved in any of them.
// 30-min checkpoint (space-bunny-free): still 84.7%, 354 of 354 bytes, 43 masked
// bytes differ, all in one hunk, and this pass established that the hunk is
// INVARIANT: of ~380 source shapes scored this pass (ten hand batches plus two
// random sweeps of 60 and 90 variants) every one that keeps 354 bytes and the y
// subtraction first emits the first thirteen instructions byte for byte as the
// current file, b.x's load always landing in esi. Nothing in the loop, the max,
// the divisions, the parameter spelling or the copy moves it: stripping the
// loop body down to `for (; i <= n; i++) a.x += d.x;` keeps the same prologue,
// and so do all six spellings of the loop condition, the loop body spellings,
// the six pointer-alias forms and the scalar/Vec3 helper forms of the
// difference. tools/permute.py, 5154 candidates with the default focus plus 2950
// more with --no-focus (19 minutes in all): 84.7% to 84.7% both times, no change.
// Two new diagnostics for the next attempt:
// 1. MSVC 5 produces only TWO load orders for the three subtractions, whatever
//    the source: y-first sources give b.y, a.x, a.y, [y-sub], b.x, a.z; a
//    non-y-first source gives b.y, a.y, a.x, b.x, a.z (a.y hoisted into the
//    second caller-saved register). The original's order, b.y, b.x, a.x, a.y,
//    a.z, is neither, so no permutation of the three statements reaches it.
// 2. I scanned the exe for prologues that hoist two argument loads above
//    `push ebx` (49 hits, 8 of them already matched: 0x4233a0, 0x423710,
//    0x49fb50, 0x49fba0, 0x49fbf0, 0x4b8b30, 0x4c06e0, 0x4cbab0). In every
//    matched one the two hoisted values are whole by-value parameters that no
//    callee-saved register is needed for (0x4c06e0, MATCH: `mov eax, [param2];
//    mov ecx, [param3]; push ebx`, then edx/ebp/ebx/esi/edi for the derived
//    values). The original here has that same shape, two caller-saved values
//    hoisted and four callee-saved ones after; ours instead gives the second
//    slot to a callee-saved value. So the missing source is one where b.x needs
//    no callee-saved register and is allocated before a.y, which pins the
//    remaining search on making the x subtraction's minuend a plain temporary
//    that is NOT also the destination, without the `mov esi, ebx` copy that
//    `b.x = a.x - b.x` costs (356 bytes).
// Also ruled out this pass: the dead-store lever (five spellings x three
// positions between the subtractions) and the Identity wrapper on a scalar
// leave the prologue byte-identical (they only perturb the loop, 81.5%);
// arithmetic shapes (`+= -a.f`, `- (int)a.f`, `+ 0`, `* 1`, `,`, a block, an
// `if (1)`), the member and free operator-= in all six body orders, MaxAbs with
// the abs inside the helper, GetCell as a macro, a named constant for the
// shift, the aggregate initialiser, three scalar difference locals, subtract on
// a local copy and copy that into d, and every return type.
// Space Bunny Free pass (issue #4629): still 84.7%, 354 of 354 bytes, size exact,
// and the whole diff is still the one register pair (dx in ecx, |dx|/n in esi in
// the original; dx in esi, |dx|/n in ecx here). About 120 more shapes scored,
// none above 84.7%, but this pass found the one mechanism that moves the x
// difference into a caller-saved register, which is the thing every earlier pass
// was looking for:
// - `static inline int Dx(const Vec3_004851c0& p, const Vec3_004851c0& q) { return
//   q.x - p.x; }` with its result stored to a LOCAL struct field, `d.x = Dx(a, b);`
//   after the in-place y/z subtractions and the copy, is the only spelling found
//   that puts BOTH b.x and b.y in caller-saved registers before `push ebx`: it
//   emits `mov eax,[esp+0x1c]` (b.x) then `mov ecx,[esp+0x20]` (b.y), i.e. the
//   original's two hoisted loads with the roles SWAPPED, 354 bytes, 74.2%.
// - The destination decides it: the same helper stored into the by-value
//   parameter's field (`b.x = Dx(a, b);`) puts the difference straight back in
//   esi (81.5%), as do the pointer and by-value-argument forms (81.5%). So an
//   inlined function's return value keeps a scratch register only when it lands
//   in a local, and b.x being a by-value struct parameter is exactly what costs
//   the original's ecx.
// - The order of the two hoisted loads can be flipped, but only by routing BOTH
//   differences through helpers into d's fields (b.z in place, then
//   `d.y = Dy(a,b); d.x = Dx(a,b);`): that gives `mov ecx,[b.y]` before
//   `mov eax,[b.x]`, the original's order, but it is 360 bytes and 64 to 67%.
// - The helper's DEFINITION order in the translation unit (yxz, xyz, zyx, yzx)
//   changes nothing: all four compile to identical bytes at 74.2%, so this is
//   not the compiler-state lever either.
// - Everything else tried this pass, all 84.7% or worse: the parameters declared
//   the other way round, `b` first (the x difference then lands in edi, not
//   ecx; 77.4 to 78.2%, 354 bytes, and the symbol is the same either way); all
//   six orders of the in-place form, of the fresh-d form and of copy-first (the
//   fresh-d orders put dx in esi for xyz/xzy/yxz and in ebp for yzx/zxy/zyx, so
//   no order reaches ecx); eight neutral extra uses of b.x (`-= 0`, `*= 1`,
//   `b.x == b.x`, a copy through a temp, `+= a.x; -= a.x`, `-(-b.x)`, `|= 0`)
//   at three positions each; the three differences through int locals rebuilt
//   into d; by-value, pointer, const-ref and member helpers for one, two or
//   three of the differences in all six orders; an `Ab()` helper for the abs;
//   and the max reading b's fields instead of d's.
// - NEW, the closest anyone has come to the mechanism, and what it rules out: a
//   shape exists whose first eight prologue instructions are the original's with
//   ONLY the first two registers exchanged,
//       b.y -= a.y;
//       Vec3_004851c0 d = b;
//       d.x = Dx(a, b);      // or Sx(b.x, a.x), a scalar-argument helper
//       d.z = Dz(a, b);
//   which emits `mov ecx,[b.y]`, `mov eax,[b.x]`, push ebx, ebx=a.x, push ebp,
//   push esi, esi=a.y, ebp=a.z against the original's `mov eax,[b.y]`,
//   `mov ecx,[b.x]` and the same six after it. 360 bytes, 67.2%. So the load
//   ORDER the original needs is reachable, and what is left is which of the two
//   scratch registers C1 hands out first: whenever the x difference is a level-0
//   temp (a helper result, or plain `d.x -= a.x` with b.x only read), b.x takes
//   the FIRST scratch register and the y difference the second, and the baseline
//   (both differences in place) is the only shape where the y difference takes
//   the first and the x difference misses the scratch pool entirely. No shape
//   tried produces "y difference first AND x difference level 0", which is
//   exactly what the original is.
// - A non-y-first in-place order (b.x first) puts the second scratch register in
//   EDX, not ecx, so the scratch pool is not simply [eax, ecx, edx] handed out
//   in allocation order; that order is 83.9%, 354 bytes.
// - The same holds for every shape in which b.x is only READ, with or without a
//   folding wrapper on the subtraction (`(b.x - a.x) * 1`, `+ 0`, `| 0`, `<< 0`,
//   `-(-(...))`, a cast): all eight are 354 bytes, 77.4%, and all emit
//   `mov eax,[b.x]; mov ecx,[b.y]`, so it is b.x being read-only, not the shape
//   of the expression, that wins it the first scratch register. Putting the same
//   wrappers on the y difference instead changes nothing at all (84.7%, esi).
// What still differs is only the register class of the x difference: the
// original loads b.x into ecx before `push ebx` and keeps the abs/max/n chain in
// esi; here b.x's load lands after the pushes in esi and the chain takes ecx.
// Every other instruction, from the frame down to the `ret 0x18`, is identical.
// Space Bunny Free pass: still 84.7%, 354 bytes, and I now know exactly what the
// tie is. Register by register, the original allocates six different registers:
// b.y->eax, b.x->ecx, a.x->ebx, a.y->esi, a.z->ebp, b.z->edi. Ours allocates the
// same six values but only four registers: b.y->eax, a.x->ebx, a.y->esi, then
// b.x REUSES esi (a.y is dead one instruction earlier than in the original),
// a.z->ebp, b.z->edi. So the whole difference is that the x-difference temp
// takes the register the y subtraction just freed instead of the still-free ecx,
// and |dx| and n then take ecx instead of esi. The two `mov` hoists follow from
// that: a load into a caller-saved register can be hoisted above `push ebx`, one
// into esi cannot. Nothing else differs: the dead `mov [esp+0x10], eax` /
// `mov [esp+0x10], ecx`, the copy's slot mixup and everything from
// `mov eax, [esp+0x10]` (0x4852ec) to the `ret` are byte-identical.
// New source shapes scored this pass, all 84.7% or worse: y,x,z produces output
// byte-identical to y,z,x (MSVC normalises the three in-place subtractions to
// y,x,z whatever the source order, so the order is not the lever; all six were
// re-scored, yzx and yxz 84.7%, the rest 83.9%); 43 compiler-state files of
// unused padding before the function in five kinds (extern int, extern int(),
// `static int f()`, struct, extern const int) at 4 to 196 items, every one
// exactly 84.7% and not one byte moved; a Vec3 with a user-defined two-argument
// constructor doing the difference (eight body orders, by value and by
// reference), 350 bytes, 64.8 to 72.1%; a helper taking the Vec3 by value that
// returns the difference (six body orders), 69.1 to 81.5%; field-by-field
// differences into d and field-by-field copies of b into d after the in-place
// subtractions, all six orders each, 350 bytes, 68.0 to 75.3%; `d = b` then
// subtract in place on d (82.3%, and 76.6% in x,y,z order); the max spelled
// `p > q ? p : q`, as a `max(a,b)` macro in both argument orders and as
// `__max` (83.9, 83.9, 76.6 and 84.7%, only `p < q ? q : p` matches the
// original's `jl`); four declaration orders of d/n/best/i (two of them cost 8
// bytes, the rest 84.7%); the max read from b's fields with the copy before
// and after the divisions (84.7% and 64.5%); comma expressions pairing two
// subtractions, `b.x = b.x - a.x`, `d.x = d.x / n`, the two divisions swapped,
// the three subtractions or the max inside their own block, and a `while`
// loop: 84.7% or worse. One diagnostic worth keeping: a stripped function with
// only the three subtractions and the copy folds to `mov eax,[d.x]; mov
// ecx,[b.x]; sub eax,ecx`, so the tie cannot be reproduced in a small
// reproducer, only in the whole function.
// Also closed this pass: all 48 combinations of the six subtraction orders with
// the two assignment forms (`-=` and `= b.F - a.F`) for each subtraction. The
// assignment form makes no difference at all, the eight forms of each order
// compile to identical bytes: the sixteen y-first combinations are 84.7% and
// the other thirty-two are 83.9%, so the only lever in the whole preamble is
// whether the first subtraction is the y one. tools/permute.py, 2474 candidates
// in 15 minutes, 84.7% to 84.7%, score 355 unchanged.
// claude-opus-5-5 (#4378): still 84.7%. The only difference is in the head:
// the original loads b.x into ecx before `push ebx` and keeps dx in ecx and the
// step count n in esi; ours swaps them (dx in esi, n in ecx). Tried: d built
// from b.x - a.x directly (74.5%), x/y/z subtraction order (83.9%), named abs
// locals, an if-based max (74.7%), and the 0x485140 GetCell spelling (a width
// local after the x >= 0 test) plus <memory.h> (no change). A 12-minute permuter
// run (542 candidates) found nothing.
//
// mimo-v2.6-pro retry pass (issue #4059), still 84.7%, 354 bytes. The one
// remaining hunk is unchanged: the original keeps the x difference in ecx
// (b.x loaded into ecx before the register pushes) and the abs/max/n chain in
// esi, ours keeps the x difference in esi and the chain in ecx. This pass
// ruled out the compiler-state theory far more thoroughly than before:
// - the unused-declaration sweep, redone at step 1 for N = 0..399 and at spot
//   values up to N = 3000, is bit-identical at every N (diff hash unchanged),
//   in every placement tried (after the include, before the function, after
//   the struct block) and with five unused prototype spellings up to 3000;
//   unlike 0x47dfc0 and 0x41ce90, declarations here move no byte at all;
// - tools/headers.py --cpp, all 768 sets (every C set crossed with none or
//   one of <string>, <vector>, <map>, <list>, <iostream>): flat 84.7%;
//   windows.h alone is 83.9% and flips the loop's [edx+ecx+0xfa] to
//   [ecx+edx+0xfa]; windows.h plus any C++ header returns to 84.7%;
// - defining each real neighbour above this function (0x485140, 0x485330,
//   0x485070, 0x485010, 0x47dfc0) as the guide suggests: only 0x485140 moves
//   anything (83.9%, same loop base/index flip as windows.h), the prologue
//   tie is identical in every one;
// - pack(2)/pack(4)/pack(8)/pack(16) on the struct block: codegen moves but
//   only downwards (66.7% and 54.3%).
// New source shapes scored here, all worse or flat: n read from b fields
// after the in-place subtractions (64.5%), divisions done on b (55.9%),
// no step struct at all (55.9%), three scalar difference locals used in the
// loop (75.3%), the x difference as a local rebuilt into d (77.4%),
// member operator-= (80.6%), a copy-returning helper (81.5%), labs (flat),
// throwaway extra uses of dx/dy/dz around the copy (all fold away, flat),
// interleaved per-field stores between the subtractions (75.3%), and T&
// field references (81.5% all three, flat with only x or only y). The
// prologue tie never moved in any of them, so the register swap is still
// unexplained; the instruction stream is byte-identical from the `sub`
// pair onward apart from the ecx/esi names.
// Third pass (space-bunny-free), still 84.7%, and a new fact about the tie:
// writing the x difference with the operands the other way round
// (`b.x = a.x - b.x`) is the only rewrite found that makes MSVC hoist b.x's
// load above `push ebx` into ecx, and it then reproduces the original's
// first ten instructions byte for byte, `mov ecx, [esp+0x1c]` ... `sub ecx,
// ebx` included. It cannot be the source: with a.x as the minuend the
// destination can no longer be a.x's own register (ebx is live across the
// loop), so the compiler copies it first and the function grows two bytes
// (`mov esi, ebx; sub esi, ecx`) and drops to 75.5%. So the original really
// is `<hoisted temp> - <loop-live register>`, i.e. `b.x -= a.x`, and the hoist
// of that one load is a scheduling decision no spelling reached. All 48
// combinations of the three subtraction orders and the two operand orders of
// each subtraction were scored this pass: the unflipped y-first orders stay
// best at 84.7%, the flipped ones are 67.5 to 75.5% and 354 to 358 bytes.
// Also re-scored this pass, all 354 bytes but worse: the house-style inline
// `operator-` (free const-ref, as at 0x40beb0, and a const member, as at
// 0x404730) with its body in x,y,z / y,x,z / z,y,x order (80.6 to 81.5%, all
// still stopping after the second instruction), a pointer to b modified in
// place (81.5%), the comma-operator and `-a.x` spellings of the same
// in-place subtractions (84.7%, identical code).
// Not matched yet, 84.7% (354 bytes, same size as the original). The only
// difference left is register allocation in the prologue: the original keeps
// the x difference in ecx (b.x is loaded into ecx before the register pushes,
// so `sub ecx, ebx`, the abs block runs in esi, and n ends up in esi), while
// here the difference lands in esi and the abs temporary and n use ecx.
//
// What made the jump from 75.3%: subtract in place on the by-value parameter
// (b.y -= a.y; b.z -= a.z; b.x -= a.x) and then copy it whole into the step
// vector (`Vec3 d = b;`). The struct copy is what keeps the original's dead
// store of the undivided x difference (`mov [esp+0x10], ecx`) and drops the z
// one; field-by-field copies, a local per difference, a Diff() helper, a
// `d = b` before the subtractions and an aggregate initialiser all score lower
// (71 to 82%). The y step is the undivided difference, as the original has it.
// Scored without changing 84.7%: all six subtraction orders (yzx and yxz are
// best), the three spellings of the abs maximum, `/=` vs `= x / n`, taking abs
// and the divisions from b or from d, n/i/c/best declared up front in every
// order, spelling each subtraction five ways, and 1500 random mixes of in-place
// and local differences.
//
// deepseek-v4.1-flash additions (all still 84.7% or lower): all 128 header
// sets from tools/headers.py (best 84.7% with <stdlib.h>), a Sub/MaxAbs
// static inline helper in every field order, separate `int` difference
// locals built into d, an explicit max local, __max, swapped abs order and
// swapped division order, `Vec3 p = a` copies, and six-arg layouts. The
// original loads b.x into ecx before the callee-saved pushes (a.y already
// owns esi there, and esi is reused for abs(dx)/n), while ours reuses esi for
// b.x and gives ecx to abs(dx)/n. Nothing tried moves that one choice.
//
// deepseek-v4.1-flash third pass (retry), still 84.7%, 354 bytes. Re-derived
// the allocator tie precisely: original has dx in ecx (b.x loaded into ecx
// before `push ebx`) and abs(dx)/n in esi; ours has dx in esi and abs(dx)/n in
// ecx. Scored every sign/operand-order spelling of the x and z subtractions
// (`b.f -= a.f`, `b.f = a.f - b.f`, `b.f = -a.f + b.f`, `b.f = b.f + -a.f`):
// the reversed x form (`b.x = a.x - b.x`) is again the only one that hoists
// `mov ecx, [b.x]` above the pushes (it reproduces `mov ecx, [esp+0x1c]`), but
// it then costs `mov esi, ebx; sub esi, ecx` (356 bytes, 75.5%) because a.x
// owns ebx and is loop-live; every other mixed form is 84.7% or lower. Also
// re-tested: n computed from the raw parameters before the differences (77.4%),
// d declared before n (77.4%), separate y/x/z int locals rebuilt into d with
// every division order (62.3 to 69.6%), and the earlier forms. The `b.x`
// hoist and the ecx/esi assignment are one allocator decision that no spelling
// reached; the instruction sequence after the first two instructions is
// identical. Everything below the division (the whole loop) is byte-identical.
//
// Second pass (deepseek-v4.1-flash), still 84.7%: the whole hunk is the one
// register pair dx=ecx/abs=esi (original) vs dx=esi/abs=ecx (ours). Tried
// again and ruled out: `Vec3_004851c0 d = b - a` with an inline operator-
// whose body order is xyz, yxz, yzx, and by-value or const-ref parameters
// (80.6 to 81.5%); `Vec3 d = b; d -= a;` (82.3%), and the copy before and
// after the subtractions, `d(b)`, default-construct then assign, d declared
// at function scope, and field-by-field copies (74.5 to 84.7%): every form
// that keeps the copy after the in-place subtraction reproduces the byte
// sequence and the dead `mov [esp+0x10], dx`, so the source shape is right.
// Also tried: all six subtraction orders, six raw int component locals
// initialised in the original load order (b.y, b.x, a.x, a.y, a.z, b.z) then
// subtracted, n computed from the parameter fields before the copy, and a
// reference alias of b. tools/headers.py confirms no header set beats 84.7%.
// The original's b.x load is hoisted above the push ebx and above a.y's
// load, which is a scheduler/allocator decision this source shape does not
// reach; the instruction sequence is otherwise identical.
//
// deepseek-v4.1-flash fourth pass (retry), still 84.7%, 354 bytes. Confirmed
// this is a compiler-state tie, not a source shape: the unused-declaration
// sweep (`extern int dummyN;` for N = 0..400, step 4) is flat at 84.7%, and
// tools/headers.py reports all 128 header sets flat at 84.7% (closest
// <stdlib.h>). Also scored this pass, all worse: a by-value member
// `operator-` in every one of the six body component orders (80.6 to 81.5%)
// and all six in-place subtraction orders (yxz and yzx stay best at 84.7%).
// The single remaining hunk is dx in ecx plus abs(dx)/n in esi (original)
// versus dx in esi plus abs(dx)/n in ecx (ours); both instruction streams are
// identical from the division onward. Left as-is per the flat-sweep rule.
// GPT-6.1-sol retry in issue #3205: baseline remains best at 84.7%.
// Register-qualified per-component locals for dx/dy/dz fell to 71.3%.
// Hoisting b.x to a local before the y/z in-place differences stayed at
// 84.7% with the same dx=esi versus dx=ecx allocation swap in the prologue.
#include <stdlib.h>

#pragma pack(push, 1)
struct Cell_004851c0 {
    unsigned short unit;               // +0x0
    char unknown_2[0x4 - 0x2];
    unsigned char height;              // +0x4
    char unknown_5[0x8 - 0x5];
    unsigned short field_8;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Type_004851c0 {
    char unknown_0[0x16e];
    int field_16e;                     // +0x16e
};

struct Unit_004851c0 {
    char unknown_0[0x6e];
    int field_6e;                      // +0x6e
    char unknown_72[0x92 - 0x72];
    Type_004851c0* type;               // +0x92
    char unknown_96[0x118 - 0x96];
};

struct Game_004851c0 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    unsigned char* mapping;            // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_004851c0* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit_004851c0* units;              // +0x14357
};
#pragma pack(pop)

extern Game_004851c0* g_game;

struct Vec3_004851c0 {
    int x;
    int y;
    int z;
};

static inline Cell_004851c0* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x4851c0
int __stdcall FUN_004851c0(Vec3_004851c0 a, Vec3_004851c0 b)
{
    b.y -= a.y;
    b.z -= a.z;
    b.x -= a.x;
    Vec3_004851c0 d = b;
    int n = (abs(d.x) < abs(d.z) ? abs(d.z) : abs(d.x)) / 0x100000 + 1;
    d.x /= n;
    d.z /= n;
    short best = 0;
    for (int i = 0; i <= n; i++) {
        Cell_004851c0* c = GetCell(a.x / 0x100000, a.z / 0x100000);
        if (c) {
            short v = g_game->mapping[c->field_8 * 256 + 0xfa] + c->height;
            if (best < v) best = v;
            if (c->unit) {
                short w = (g_game->units[c->unit].type->field_16e + g_game->units[c->unit].field_6e) >> 16;
                if (best < w) best = w;
            }
        }
        a.x += d.x; a.y += d.y; a.z += d.z;
    }
    return best;
}
