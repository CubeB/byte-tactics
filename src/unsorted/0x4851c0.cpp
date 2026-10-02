// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
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