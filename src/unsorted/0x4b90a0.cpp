// Decompiled by deepseek-v4.1-flash, finished by Claude Sonnet 5.5, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by space-bunny-free. Names are provisional.
//
// space-bunny-free pass (#4686), 87.1% again, 4 check.py runs plus ~85 free
// whole function compiles scored with the probe below (nine whole-function
// batches, a 26-function micro-benchmark and a header sweep). FOUR things
// settled, three of them closing levers other passes were told to try:
//
//  1. THE PIN LEVER IS DEAD BECAUSE MSVC 5 FOLDS THE CONDITIONAL. The hand-off
//     note suggested `rp = rp ? rp : rp` on the derived plane pointer (worth 15
//     points on 0x473a00). It does nothing here, and the reason is now
//     measured, not guessed: with identical arms the ?: is collapsed at the
//     FRONT END, so no phi is ever created and not one instruction is emitted.
//     Eleven pin shapes (dp0 through a fresh rp, both planes, dp0 in place,
//     dst, src, the plane array &dst->plane0, xoff, stride, yoff, and the
//     same sum built as an unsigned then cast) all compile byte for byte to
//     the same 256-byte file with the same 87.1% and the same 9-instruction
//     head. So do four REAL phi-creating shapes, where the conditional cannot
//     be folded away: `row ? (xoff + dst->plane0) : (xoff + dst->plane0)`,
//     the same as an if/else with identical arms, a plane pointer declared in
//     the for-init and reassigned in its increment list (a genuine back-edge
//     phi), and a dummy counter whose increment does the assignment in a comma.
//     MSVC 5 collapses identical operands of a ?: and of an if/else, and
//     propagates the loop-init value of the third, so none of them survives
//     either. Do not re-try the pin or a same-armed conditional here.
//  2. THE YOFF SPILL MOVES ONLY AS A WHOLE BLOCK, AND DEFINING `yoff` ONLY IN
//     THE FOR-INIT (the shape that would put its store in the preheader by
//     construction) DOES NOT DO IT. Re-measured on this build: the
//     `yoff = stride;` copy (register-only local, textually AFTER the two
//     source-plane assignments, the shape #4523 measured at 87.1%) does not put
//     the store in the loop preheader on its own. MSVC emits it immediately
//     after xoff's store, in the guard block, before the first `jl`, in every
//     placement tried: after the guard, before the plane loads, as the for-init
//     comma `(yoff = stride, 0)`, and the shape that keeps the guard reading
//     yoff. So `yoff` is NOT stored where the source puts it: MSVC emits both
//     outgoing spills as one pair right after the `test` that fixes their
//     signs, and the source placement is irrelevant. That is store-at-death,
//     and the store is at the death of the register, which is before both
//     guards whatever the source says.
//     New this pass, the four shapes that should have forced the store into the
//     preheader, all 87.1% with the store in exactly the same place: (a) yoff
//     declared and initialised only in the for-init (`for (int row = 0, yoff =
//     dst->field_6 - src->field_6 + y; ...)`) with the guard testing the raw
//     expression, so yoff has no definition before the loop at all; (b) the
//     same with yoff initialised before the loop as well; (c) a `yoff0` that
//     dies at the guard and `yoff = yoff0` in the for-init; (d) yoff listed
//     before row in the for-init. In (a) MSVC common-subexpression-eliminates
//     the guard's copy of the expression into the for-init copy, the register
//     is therefore born before the guards, and both spills land together again.
//     The original instead has `mov [esp+0x14],eax` after the `jbe`, i.e. a
//     loop-header phi copy, so the original's build kept yoff's register alive
//     to the loop head. Store-at-death versus loop-header phi is an allocation
//     decision, not a source spelling, and no spelling of either the definition
//     or the loop's use moves it. That is why the 87.1% cannot be raised from
//     the source.
//  3. The row head is unchanged and still the whole 4-byte gap: `mov
//     esi,[edx+0x10]; add esi,ebx` here against `mov esi,ebx; mov
//     eax,[edx+0x10]; add esi,eax` in the original. Thirty-four more whole
//     function variants scored this pass, all emitting the same 9-instruction
//     head and all at 87.1%: the sum split into three statements per row,
//     `dp0 = plane; dp0 += stride` and `dp0 = plane; dp0 = dp0 + stride` and
//     their two-statement siblings, the same four again with the two plane
//     statements before the `stride = dst->width * yoff` line instead of after
//     it (the original copies xoff into esi before the imul, so the schedule
//     was worth varying too), `dst->plane0[xoff + stride]`, the plane written
//     first, the int sum written first `(xoff + stride) + plane`, an
//     `(unsigned)xoff` cast, `plane + xoff - (0 - stride)`, the planes as
//     integers summed as integers and cast once, an `int ox = xoff` copy, the
//     two planes read into pointer locals before the loop, four inline helpers
//     (a plane accessor per row, a 3-argument row helper with the pointer
//     first and last, and an identity function around the plane), the two row
//     statements in either order, `n = src->width` first, the two increment
//     orders swapped, the plane fields swapped between the rows, a fresh value
//     number for row 0's offset, row 1's offset, both, the stride or the second
//     stride, or one row given fresh copies of BOTH its operands (4 more), a
//     local `unsigned char* d[2]` for the two row pointers (both assigned then
//     read, and with an initialiser list), `*(unsigned char**)
//     &dst->plane1`, and `&dst->plane0` indexed as `pp[0]`/`pp[1]`. So the
//     fold is not a function of the statement order, the term order, the casts,
//     the operand types, the number of uses, a helper boundary or an array:
//     this build always puts the plane load in the add's destination.
//
//     WHAT THE FOLD ACTUALLY IS, from a 26-function micro-benchmark this pass
//     (build/scratch/0x4b90a0/micro.py and micro2.py, one scratch function per
//     spelling, all compiled and read back; the two files are worth reusing).
//     Three findings, and together they close the question:
//     (a) `mem + reg` ALWAYS folds the memory into the destination. All 14
//         spellings of `p0 + i` produce `mov dst,[b+0x10]; add dst,reg`: term
//         order both ways, the two-statement split, `+=`, `&p0[i]`, `(short)`
//         and `(long)` casts, `(int)` and `(unsigned)` planes summed as
//         integers, `p0 - (0 - i)`, the plane in a pointer local, a three-term
//         sum, and two row pointers live at once (which still gives
//         `mov eax,ebx; mov ebx,[ecx+0x10]; add eax,ebx`). So the fold is not
//         an artefact of a pointer type or of a cast.
//     (b) `mem + reg` DOES NOT fold when the index has to be materialised
//         into the destination first, i.e. when its own load changes width:
//         `p0 + (short)i` gives `movsx eax,word ptr [esp+8]; mov edx,[ecx+0x10];
//         add eax,edx`, which is the original's shape. This is the guide's
//         0x473590 rule in its real form: the destination register is already
//         committed, so the load is forced to materialise elsewhere. It cannot
//         be used here, because the original's own index is a plain 32-bit
//         register copy (`mov esi,ebx`), two bytes, not a widening load.
//     (c) `mem + mem` does NOT fold and keeps source order, and this build
//         already matches such a site: 0x43e060 is MATCH with the source
//         `obj->pos + FUN_0043def0(obj, param)` and the code
//         `mov ecx,[esi+0x6a]; mov eax,[edx]; add ecx,eax`, i.e. the LEFT
//         memory load in the destination and the right one in a scratch. So
//         source order is preserved when both operands are loads, and lost
//         exactly when one of them is a register. A scan of the whole exe for
//         the un-folded three-instruction shape `mov dst,src / mov tmp,[m] /
//         add dst,tmp` finds only 7 sites, 6 of them `rep movsd` prologues,
//         and the one real site (0x43e07a) is the `mem + mem` case above.
//     Together: the original's build evaluates `xoff + plane0` as two operands
//     with the index in the destination, and this build folds the memory load
//     into the destination whenever the index is a plain register. It should
//     resolve when the file is regrouped into its original translation unit.
//
//     THE MOST IMPORTANT THING FOUND THIS PASS, and it corrects the six passes
//     above that called this a build difference: THIS BUILD PRODUCES BOTH FORMS,
//     in one MATCHED sibling, from near-identical source. 0x4b9d70 is MATCH
//     with the source
//         unsigned char* s = src->data + srcRow * src->width + srcCol;
//         unsigned char* d = dst->data + dstRow * dst->width + dstCol;
//     and the code
//         mov eax,ebp / imul eax,ebx / add eax,[esi+0x10] / add eax,ecx
//         mov ecx,edx / imul ecx,ebx / mov ebx,[edi+0x10] / add ecx,ebx
//         mov ebx,[esp+0x14] / add ecx,ebx
//     i.e. row 0 is the MEMORY OPERAND form (`add reg,[m]`) and row 1 is the
//     form this function needs (`mov dst,index` ... `mov tmp,[m]` ... `add
//     dst,tmp`), which is byte for byte 0x4b9106's row head. The two rows have
//     the same source shape and differ only in which register was dead after
//     the imul: for row 1 `ebx` (the width) dies, so MSVC reused it for the
//     plane pointer; for row 0 nothing was free, so the memory operand went
//     straight into the add. So the choice is a register-allocation decision
//     inside this build, NOT a difference between builds, and source can reach
//     it in principle. A scan of every byte of the exe for the exact shape
//     (a `mov d,s` within 16 bytes of a `mov t,[r+disp8]` that a `add d,t`
//     immediately follows) finds only 5 sites: 0x4b9106 (this function),
//     0x4b9e12 (0x4b9d70, MATCH, above), 0x4c6f50, 0x4c7029 and 0x4e63ed. So
//     it is rare, but it is not exclusive to this function, and the sibling is
//     the reference to work from. What still has to be found is the source
//     shape that makes the allocator free a register at the row sum here. The
//     obvious candidate was the sibling's own shape, the multiply inline in the
//     row expression instead of in the named `stride` local (the sibling has no
//     named stride): all 6 spellings of that, with the multiply written
//     `dst->width * yoff` and `yoff * dst->width`, as the first, middle or
//     last term, parenthesised or not, and the two orders of the plane terms,
//     still give this function's folded head, 87.1%. Inlining the multiply
//     also does not move the imul here, contrary to what the #3409 notes say.
//
// The byte view of the residual, which is worth keeping because it says exactly
// how little is left (this pass compared the 256 compiled bytes against the
// original's 260: common prefix 53 bytes, common suffix 13, and the whole rest
// differs only as below).
//   OURS      89 44 24 14              mov [esp+0x14],eax   <- yoff spill, too early
//             0f 8c ..                 jl                   <- one byte short
//             ... 0f 86 ..             jbe
//             33 c9                    xor ecx,ecx
//             8b 72 10                 mov esi,[edx+0x10]   <- folded plane load
//             66 8b 0a                 mov cx,[edx]
//             03 f3                    add esi,ebx
//             0f af c8                 imul ecx,eax
//             8b 42 14                 mov eax,[edx+0x14]
//             03 f1 03 c3 03 c1        add esi,ecx / add eax,ebx / add eax,ecx
//   ORIGINAL  ... 0f 86 ..
//             89 44 24 14              mov [esp+0x14],eax   <- yoff spill, loop head
//             33 c9                    xor ecx,ecx
//             8b f3                    mov esi,ebx          <- copy of xoff
//             66 8b 0a                 mov cx,[edx]
//             0f af c8                 imul ecx,eax
//             8b 42 10                 mov eax,[edx+0x10]   <- plane into a scratch
//             03 f0                    add esi,eax
//             8b c3                    mov eax,ebx          <- second copy of xoff
//             8b 5a 14                 mov ebx,[edx+0x14]   <- ebx is plane1 now
//             03 f1 03 c3 03 c1        add esi,ecx / add eax,ebx / add eax,ecx
// So ebx holds xoff in our build and is used by both adds, and in the original
// it is freed for plane1 after xoff has been copied into esi and eax. That is a
// register-allocation difference on top of the codegen difference, and neither
// moves: the two row statements in either order, with `n = src->width` first,
// with the plane fields swapped, with the two increment orders swapped, and
// with a fresh value number for row 0's offset, row 1's offset, both, the
// stride or the second stride (10 more whole-function compiles) all emit the
// same head byte for byte.
// The probe used this pass is cheap and worth reusing: build/scratch/0x4b90a0/
// gen.py (writes a variant from a %s template of the row block), b1.py to b9.py
// (the whole-function batches named above), micro.py and micro2.py (the 26
// one-function-per-spelling probes), probe.py (imports tools/check.py, compiles
// with C.compile_source and prints size and difflib ratio, i.e. exactly what
// check.py would say) and pre.py (prints the prologue through the row head so
// the two spill stores can be seen). A variant is free to score; a check.py run
// is only needed to confirm.
//
// If you pick this up: do not spend another pass on expression spelling, term
// order, parenthesisation, casts, operand types, statement order, header sets
// (tools/headers.py re-run this pass at this 87.1% baseline: 128 sets, "none"
// is best at 87.1%, <memory.h> 82.8%, everything else 53.8% or worse), the pin
// or any same-armed conditional, where yoff is defined, or inlining the
// multiply into the row expression. The guide's inline accessor lever was worth
// a try here too and cuts the wrong way: an accessor for `width`, for `height`
// or for `colorKey` each costs 4.3 points (82.8) and all three cost 18.3
// (68.8), with the row head unchanged in every case. That leaves the register
// allocation of the row sum, which item 3 shows is reachable in this build and
// points at 0x4b9d70 as the reference: the goal is a source shape that leaves a
// register dead after the `imul ecx,eax`, so MSVC materialises the plane load
// into it instead of folding the load into the destination. One permuter round
// on the yoff-preheader shape (7309 candidates, 12 minutes, 4 jobs) was flat at
// 87.1%, score 365 unchanged, and nothing to read in best.diff.
//
// space-bunny-free pass (#4523), 87.1% confirmed unchanged, and the residual is
// now MEASURED to be exactly one thing. 6 counted check.py runs plus ~50 free
// `check.py --sym` scores of scratch files (all in build/scratch/0x4b90a0/;
// probe.py compiles a variant with /Fa and prints its loop head, so a variant
// that emits the same code is told apart from one that moves without spending a
// run, and rowvar.py/fullvar.py/cases*.py generate and score them).
// THE SCORE IS DOMINATED BY LABEL ALIGNMENT, so here is the measurement that
// matters. Guarding the second test on a variable with a stack home instead of
// the register (`if (xoff < 0 || stride < 0)` with `stride` never assigned, a
// C4700 uninitialised read, NOT a legal answer) makes the guard reload and
// test a slot, which adds exactly the 4 bytes the row head is short, and the
// file becomes 260 bytes: 92.0%, with every jump target in the function then
// identical to the original's and only three hunks left. So: (a) everything
// outside the row head, the yoff store and that one guard test is already
// byte-identical, (b) the whole 4-byte shortfall and most of the remaining
// score are the one thing below, and (c) a 260-byte build is reachable ONLY
// through the row head, so no amount of shuffling elsewhere can pay. Read the
// score with that in mind: 87.1% is 256 bytes with every label 4 out, and the
// 92.0% above is not a better decompilation, it is the same wrong code with the
// labels lined up.
//  1. THE ROW HEAD, the plane load folded into the add's destination
//     (`mov esi,[edx+0x10]; add esi,ebx` here, `mov esi,ebx; mov
//     eax,[edx+0x10]; add esi,eax` in the original, 2 bytes of copy per row).
//     New this pass, all emitting the SAME 9-instruction head byte for byte:
//     the plane pointer re-assigned as the first statement of the row body
//     (alone, both planes, stride line before or after), a `Bitmap*` alias, a
//     helper taking dst by value and returning the plane, a helper taking the
//     plane and both offsets by value (these two rotate the whole block, 64.5%),
//     the plane fields read into `int` locals with the cast at the store, the
//     planes as `unsigned char** pl = &dst->plane0` indexed [0]/[1], declared-
//     then-assigned pairs (guide 0x411f50), a self-assignment of the plane
//     local, the same value read twice in the sum, a dead `xoff = xoff;`
//     between the rows, a fresh value number for row 1's offset or for both or
//     for the stride (that one does move the two stride adds ahead of the two
//     xoff adds, 86.0%), the sum split into two statements per row,
//     `dst->plane0 + (xoff + stride)`, `int xo = xoff; int st = stride;`, and
//     `long` offsets. Pointer+pointer, the one IR path left, is not available:
//     MSVC 5 rejects `(unsigned char*)xoff + dst->plane0` with C2110.
//     The dead-store-inside-a-folded-branch lever (the one that took 0x47eee0
//     over the line) needs a local to fold the condition, and one local in the
//     row body reallocates the frame and rotates the block (17.8%); at file
//     scope the flag is a real load, not a folded 0 (32.1%). So the fold is the
//     IR rule that puts a memory operand in an ADD's destination register, and
//     only a value with two live uses stops it, which this source cannot give
//     it without changing what the function computes.
//     Reading of what the original's build did: it never puts a REGISTERED
//     memory operand on an ALU instruction anywhere in this function. The only
//     ALU instruction with a memory operand in the whole 260 bytes is
//     `cmp word ptr [ebp+2], 0`, a compare against a constant; every load that
//     feeds an arithmetic operation is materialised into a register first
//     (`xor edx,edx; mov dl,[edi]; add edx,ebx` for the threshold test, the
//     `mov cx,[edx]` partial write for the stride), and our build does exactly
//     the same everywhere EXCEPT this one add, where it folds the load into the
//     destination. The four missing bytes are the two `mov reg,reg` copies of
//     xoff that a build with no load-into-destination rule must make. Build
//     difference, not a source spelling.
//  2. THE YOFF SPILL, ours in the guard block, the original's a store in the
//     loop preheader, one instruction, and THIS PASS MOVED IT: it is reachable
//     in the preheader, but only through a copy with no stack slot of its own.
//     A copy with a home slot moves it and costs more than it wins: `int y2 =
//     yoff; if (xoff < 0 || y2 < 0) return; yoff = y2;` is 82.8% and puts the
//     store after `mov edi,[ebp+0x14]`, and the same assignment in the for-init
//     (`for (int row = 0, dummy = (yoff = y2, 0); ...`) puts it in exactly the
//     original's order among the stores at 83.9%, but a third local reverts the
//     threshold compare to `add ebx, edx` with *dp1 in eax. A copy in a
//     REGISTER-ONLY local costs no slot and keeps the compare: `stride =
//     dst->field_6 - src->field_6 + y; if (xoff < 0 || stride < 0) { return; }
//     yoff = stride;` with the `yoff = stride;` after the two source-plane
//     assignments is 87.1% (four spellings of it, all 87.1%) and puts the store
//     in the preheader between the sp0 and row stores, after which the guard
//     block matches the original exactly apart from the branch targets. All
//     that is left is one instruction: the original's store is after the `jbe`,
//     ours is before it. The same store as a side effect of the row
//     initialiser, `for (int row = (yoff = stride, 0); ...)`, is 87.1% and
//     lands before the row store instead (and with the comma the other way round
//     MSVC rewrites the whole loop test, 54.1%), and a self-assignment of yoff
//     at the top of the row body or just before the loop emits nothing at all.
//     Still 87.1% with the store where the file has it: the yoff definition in
//     a nested block that ends with the guard, the guard and the loop inside a
//     `do {} while (0)` (a real merge point after the guards: 80.6%, it moves
//     the `push edi` and the xoff store and leaves the yoff store alone), a
//     folded `if (1)` around the inner loop, yoff split into two definitions,
//     and two separate guard ifs. So the store is a store-at-definition in
//     both builds for xoff, whose value the loop never changes, while only
//     yoff, the one variable the loop increments, is sunk past the loop-entry
//     test by the original's build: a phi-copy placement decision, now one
//     instruction short of reachable.
// The file below is main's version unchanged; every variant above that ties at
// 87.1% and restructures the source (the register-only copy in particular) was
// left out, since a tie is not an improvement and the notes say which line to
// change. One permuter round on the NEW stride-copy shape, the only form here
// that had never been permuted, is flat too: 2490 candidates, 8 minutes, seed
// 11, score 365 unchanged, nothing to read in build/permute/0x4b90a0/best.diff.
// One guide rule checked and ruled out, so nobody chases it again: "MSVC 5 will
// not fold a memory operand whose base register is the destination of the same
// instruction" (the 0x473590 family) is the only documented way to force a
// load to materialise, and it does NOT apply here, because in the original the
// add's destination is esi while the plane load's base is edx. The original's
// build simply has no load-into-destination rule, so its row head is not
// reachable from this source.
//
// space-bunny-free pass (#4497), 87.1% confirmed unchanged: 1 check.py run plus
// about 110 free `check.py --sym` scores (all the scratch files are in
// build/scratch/0x4b90a0/, driven by batch.py/combo.py/pins.py/yoff.py there).
// Every new lever the earlier passes had not swept is now dead, so the two
// differences below are a build difference, not a spelling:
//  1. The row block, the fold of the plane load into the add's destination
//     (`mov esi,[edx+0x10]; add esi,ebx` here, `mov esi,ebx; mov
//     eax,[edx+0x10]; add esi,eax` in the original). The 4-byte size gap is
//     exactly the two `mov reg,reg` copies, once the yoff store is set aside:
//     11 instructions in the original's loop head, 9 in ours. New this pass,
//     all of them folding:
//       * the Identity() pin of #4241/#4299 in every position (xoff, stride,
//         yoff, level, each plane load, the row sum, both row pointers, int and
//         pointer flavours, and an extra real use of the pinned value): every
//         pin drops the threshold compare to 82.8/83.9 or rotates the whole
//         frame to 64.5-68.8, and none produces the copy form. This is the
//         opposite of what the pin does at 0x473a00, so the pin is a lever for
//         the frame's callee-saved choice, not for a load fold.
//       * the "two address nodes" trick of 0x473590 applied to the row block:
//         the planes as one `plane[2]` array (plain, indexed by a constant, and
//         read through a `Row(dst, xoff, stride, k)` helper), a dst reference
//         (`Bitmap& d = *dst`), a second dst pointer, a pointer-to-field
//         (`unsigned char** q = &dst->plane0`), a member accessor
//         (`dst->row0(xoff, stride)`), the planes as `void*` with a cast, an
//         inline accessor per plane, helpers taking the plane or xoff as an
//         argument, and helpers holding the whole three-term sum: 68.8 at best,
//         every one of them still folding.
//       * tree shapes: named bases (`b0 = xoff + dst->plane0` then `+stride`),
//         named int sums, `plane + (xoff + stride)`, `(xoff + stride) + plane`,
//         pointer locals, `unsigned char**` fields, two statements per row, the
//         rows swapped, `(int)` and `(unsigned)` casts of every term: 87.1 with
//         the same diff.
//       * BT_TOOLCHAIN=msvc5-rtm: the same 87.1%, the same 256 bytes.
//  2. The yoff spill (ours in the guard block, the original's a loop-header
//     copy after the `jbe`). New this pass, all leaving it in the guard block:
//     initialisers instead of assignments (`int xoff = ...; int yoff = ...;`),
//     the two assignment orders, both offsets computed by inline helpers, and
//     four guard spellings (`||`, `!(a>=0 && b>=0)`, the positive
//     `if (xoff >= 0 && yoff >= 0)` wrapper, two separate `if`s) crossed: 24
//     files, best 87.1.
// Two other levers closed for the record: the declaration-count state (N = 0 to
// 16 unused `extern int`s, N = 0 is best: 82.8 for N = 1-4, 64.5 for 5-7, 68.8
// for 8-16), and the dp-scope x compare-spelling x guard-order cross (12 files),
// which confirms the 87.1% shape is exactly dp0/dp1 declared in the loop body,
// `*dp1 <= *sp1 + level` and `xoff < 0 || yoff < 0`; everything else in that
// cross is 82.8/83.9 or worse.
// Facts worth keeping: the row counter really is in the parameter slot of `y`
// ([esp+0x28] = arg4) and the inner counter in the slot of `x` ([esp+0x24] =
// arg3), while `level` stays in its own slot and is reloaded twice inside the
// inner loop; the dead `mov edx,[esp+0x20]` reloads `dst`. Both builds agree on
// all of that, so the prologue and epilogue are settled and only the loop head
// is in question.
// If you pick this up: the documented way to stop MSVC 5 folding a load into
// an ALU instruction is to give the value TWO uses (the guide's "a named
// intermediate must be used twice", 0x4ded60), but that route is closed from
// the source here too: writing the plane load three times in one expression,
// `(unsigned char*)(xoff + ((int)dst->plane0 + (int)dst->plane0 - (int)dst->plane0) + stride)`
// (the same value, read three times), is CSEd straight back and still folds,
// 87.1 with the same diff. Extra reads of xoff and of stride behave the same.
// So the fold cannot be reached from this function's source with either
// toolchain, and the row block should be treated as settled compiler state
// until the file can be regrouped into its original translation unit.
//
// space-bunny-free pass (#4294), 87.1% confirmed unchanged, 2 check.py runs plus
// ~90 free --sym scores, two permuter rounds (7816 + partial candidates, no
// improvement), and a headers.py sweep at the NEW 87.1% baseline. Nothing beat
// the file below; the two known differences are both still there, and the
// evidence now says they are compiler-build differences, not spelling:
//  1. The row block. NEW: this build cannot emit the original's shape at all.
//     Every spelling of `xoff + plane + stride` puts the plane load in the add's
//     DESTINATION (`mov esi,[edx+0x10]; add esi,ebx`, 5 bytes, 2 insns) instead
//     of copying xoff and materialising the load (`mov esi,ebx; mov
//     eax,[edx+0x10]; add esi,eax`, 5 bytes, 3 insns). Same size, one more
//     instruction in the original, and the 4-byte size gap is exactly the two
//     extra `mov reg,reg` copies. Tried this pass, every one byte for byte the
//     same 256-byte file and 87.1%: all 6 orders of the three terms and 10
//     parenthesisations (`(x+plane)+stride`, `plane+(x+stride)`,
//     `stride+(x+plane)`, ...); `+=` per term; `(unsigned char*)(xoff+...)` and
//     `(int)plane` casts; the plane fields as `int` with int locals p0/p1 and
//     the cast at the store; `*(unsigned char**)&dst->plane0`; `(unsigned long)`
//     casts; an inline `RowOff(int,int)`, `RowOff(ptr,int)` and 3-argument
//     helpers (these score 64.5%, they reorder the two movsx blocks, but the row
//     block is still `mov esi,[plane]; add esi,xoff`); dp0/dp1 statement order
//     and declaration order; a dst alias. Decisive evidence: a 6-function micro
//     benchmark (loop, offsets already in registers) compiles identically to
//     `mov dl,[edx+ecx]`-style indexed forms for int+ptr, all-int casts, an
//     inline helper, `p += off`, `&p[off+st]` and int temporaries, i.e. MSVC 5
//     SP3 always picks the memory load as the add's destination and folds it
//     into the destination mov. So the original's `mov esi,ebx` means its build
//     had the destination assignment rule this one lacks: guide item "operand
//     order that nothing changes -> compiler state from earlier functions in the
//     original TU".
//  2. The yoff spill (ours before the two `jl` guards, the original's after the
//     `jbe`, i.e. in the loop body as a header phi copy). Tried this pass, the
//     store never moves: all 6 orders of {xoff,yoff,stride} and 4 declaration
//     scopes, `row` at function scope / `unsigned` / `long`, `!=` and
//     `(int)`-cast loop tests, the guard as `||`, two `if`s, `0 > x`, `!(a>=0
//     && b>=0)`, a `goto` body, `yoff++` as a body statement, sp0/sp1 read
//     before the guard. Note this may be a CONSEQUENCE of (1): our loop head
//     block has two instructions fewer, and the store is the one thing a
//     block-size change would move.
//  3. tools/headers.py --cpp at this baseline: 768 header sets, best is 82.8%
//     (<memory.h>), so header state is not the lever either.
// If you pick this up: do not re-try term order, parenthesisation, casts, int
// plane fields, inline helpers or header sets for the row block. The only
// untried lever left is the declaration-count state (unused `extern int`s), last
// swept at the 83.9% shape; and it is a compiler-state hack, not source.
//
// deepseek-v4.1-flash pass (#4294): 87.1%, up from the long-standing 83.9%.
// TWO of the three old differences are gone. The fix was pure declaration
// scope, not expression spelling:
//   * `unsigned char* dp0; dp1;` moved OUT of function scope and declared
//     inside the row loop body (`unsigned char* dp0 = ...` at the point of
//     use) flips the threshold compare to the original's registers: level ->
//     ebx, *sp1 zero-extended into edx, `add edx, ebx` (sum in *sp1's
//     register), then *dp1 into ebx and `cmp ebx, edx; jg`. With dp0/dp1 at
//     function scope the same source always gives `add ebx, edx` and
//     `cmp ebx, edx; jl`. Score 86.0 before the compare spelling change.
//   * with dp0/dp1 in the loop, the comparison MUST be spelled
//     `*dp1 <= *sp1 + level` (not `*sp1 + level >= *dp1`): that is what makes
//     the cmp operand order `cmp ebx, edx; jg` instead of `cmp edx, ebx; jl`.
//     Together: 87.1%.
// What still differs (2 items, both 4 bytes of missing `mov reg,reg`):
//   1. The yoff spill: ours stores yoff before the two `jl` guards (in the
//      guard block), the original stores it after the `jbe` loop guard (in
//      the loop preheader). Tried this pass, all kept the early store:
//      two separate guard ifs, `!(xoff>=0 && yoff>=0)`, the positive
//      `if (xoff >= 0 && yoff >= 0)` wrapper, an explicit `if (height == 0)
//      return;` before the loop (duplicates the test, worse), a
//      `goto done` guard (compile error, C++ init crossing), a copy variable
//      `yoff2 = yoff` before the loop / in the for-init / at function scope
//      (f1/f4/f5: the copy's store lands inside the guard block one
//      instruction earlier than the original's, never after the jbe; f1
//      scores 82.8), and reordering the {yoff,stride,xoff} declarations
//      (all 6 orders identical). The store position looks like a
//      register-allocator spill decision (spill at definition vs at the
//      loop preheader), not a source-visible block boundary.
//   2. The row pointers: ours is `mov esi,[edx+0x10]; add esi,ebx` (memory
//      operand becomes the destination), the original copies xoff first:
//      `mov esi,ebx; mov eax,[edx+0x10]; add esi,eax` (and the same two
//      extra movs for row 1). Every spelling tried this pass produced the
//      memory-first form byte for byte: int casts on both addends, the plane
//      fields as ints, int locals for the planes inside the loop, the sum
//      split into two statements, `(unsigned char*)xoff + plane`, `&plane[xoff]`,
//      `&plane[xoff+stride]`, plane locals p0/p1 first, an xoff copy, a
//      stride-in-loop form (that one reassociates to (plane+stride)+xoff and
//      costs 5 points, 81.7). The compare fix shows the allocator IS
//      steerable through declaration scope, so the remaining lever is
//      probably another scope/order change, not an expression spelling.
// Other facts from this pass: an inlined whole-inner-loop helper reallocates
// everything (frame grows, 74.2-ish shapes); N unused `extern int`
// declarations still move registers (N=8/16 put the yoff guard in ecx) but
// none fixed either remaining diff; declaring `stride` inside the loop
// reverts the compare fix and reassociates the row sum; the function-scope
// set {yoff,stride,xoff} is exactly right (adding any dummy local, int or
// byte, reverts the compare to 82.8, as do swapping sp0/sp1's declaration
// order, declaring `c` at function scope, and the increment order
// dp0,dp1,sp0,sp1); the increment order dp0,sp0,dp1,sp1 (current) and the
// pre-increment form are both 87.1; `unsigned`/`long` stride, const plane
// locals and an inner block scope all keep the 87.1 code.
//
// mimo-v2.6-pro pass: 83.9% unchanged (2 check runs, ~20 free compile/listing
// comparisons). New facts for the next attempt:
//  * The whole 4-byte size gap is exactly the two un-folded plane loads: the
//    original is `mov esi,ebx / mov eax,[edx+0x10] / add esi,eax` (and the same
//    for row 1) where every spelling we have folds to `mov esi,[edx+0x10] /
//    add esi,ebx`. The load is adjacent to its add in the original, so it is a
//    scheduler split of a NON-commuted `reg + mem` add; our build commutes the
//    add to `mem + reg` and folds the load into the mov. 9 row spellings (all
//    int casts, both operand orders, temps, plane locals right before each row
//    and at the top of the body) all fold identically.
//  * The threshold compare's cmp order DOES follow the spelling: `*dp1 <=
//    *sp1 + level` gives the original's `cmp reg,reg; jg` shape (only the two
//    registers are swapped), while the add destination stays level's register
//    in every spelling. The original adds into *sp1's register, so one
//    allocation tie flip fixes both the add and the *dp1 register.
//  * `static inline int Sum(int,int)` around both `*sp1 + level` uses moves
//    yoff into ecx and nothing else; not a lever.
//  * tools/headers.py swept all 128 header sets: none and <memory.h> both give
//    83.9, everything else worse. Header state does not flip the ties here.
//  * Restructures tried for the yoff spill (store after the jbe): yoff
//    assigned after the guards with the raw expression in the guard (moves the
//    movsx block after the xoff test and turns `test ebx,ebx` into `js`), yoff
//    assigned in the for-init (gets coalesced), a separate yoff0 feeding a
//    for-init copy (coalesced), two separate guard ifs, and the deleted-store
//    trick `int iy = yoff; yoff = iy;`. None sink the store past the guards.
// Retry #2444 (GPT-6.1-sol): best remains 83.9% after 4 check.py runs; a reversed
// comparison spelling fell to 82.8%. Row-pointer term ordering and threshold
// add destination remain the allocator differences described below.
// Partial, best 83.9% (re-checked by space-bunny-free, no better form found).
//
// space-bunny-free pass (#3409), 83.9% confirmed, 1 check run, 5 free --sym scores.
// The one lever no earlier pass had tried is the compiler BUILD: `tools/wcl` also
// ships an unpatched msvc5-rtm, and building this file with BT_TOOLCHAIN=msvc5-rtm
// gives the same loop head (`mov esi,[edx+0x10] / add esi,ebx`) and the same 256-byte
// code, so the row-pointer add order is not an SP3 regression either.
// Four more forms scored free, all byte for byte the 256-byte file and all 83.9%:
//   * `dp0 = (unsigned char*)(xoff + (int)dst->plane0) + stride;`, i.e. the cast in
//     the MIDDLE so that the first add is a pure int add with xoff as its first
//     operand (this is the only spelling that can reach `mov esi,ebx / add esi,eax`);
//   * `dp0 = dst->plane0 + xoff + stride`, pointer operand written first;
//   * the sum split into `dp0 = xoff + dst->plane0; dp0 = dp0 + stride;` (and the
//     same for dp1);
//   * the plane fields declared as `int`, so the whole row address is one integer
//     sum cast to `unsigned char*` at the end;
//   * the two plane fields declared as one `unsigned char* plane[2]` array (83.9,
//     identical code), and read through `__inline` plane accessors of the guide's
//     item 28 (67.7, much worse, so the accessors are not a lever here either).
// So MSVC 5 reorders the terms of the row-pointer sum whatever the operand types
// and however the statement is split, and the three remaining differences stay put.
//
// deepseek-v4.1-flash second pass (#1615): 83.9% again. Lever 6/7 tried with no
// change: reusing the parameters x and y themselves as the loop counters (the
// original's counters occupy the arg2/arg3 slots, which our `row`/`n` locals
// already get coloured into) scores 82.8 because xoff's add is then sunk below
// the inner-loop width load; splitting the two row statements into
// `dp0 = xoff + plane0; dp1 = xoff + plane1; dp0 += stride; dp1 += stride;`
// is byte for byte the old 256-byte form (MSVC still loads the plane into esi
// first), and <string.h> alone is 22.3. Confirms the reading that the three
// remaining differences are all one allocator decision this build does not take.
//
// Byte accounting: ours is 256 bytes, the original 260. Everything matches
// except the row loop head, where the original is 8 bytes longer; in exchange
// our prologue is 3 bytes longer (we spill yoff before the two `jl` guards,
// the original spills it at the top of the loop head). Two differences remain:
//  1. The destination row pointers: the original adds xoff to the plane first
//     and the row stride last ((xoff + plane) + stride), evaluating the two
//     rows one after the other. Every source form tried here compiles to
//     ((plane + stride) + xoff), with both plane loads hoisted to the top of
//     the loop head and the adds grouped by term, which is MSVC 5 SP3's
//     canonical order for the expression. Tried and all identical: every
//     operand order of `xoff + plane + stride`, pointer+int both ways,
//     `+=` per term, int/pointer temps and casts, `&plane[i]` indexing,
//     an inline `yoff * dst->width` instead of the `stride` local (that one
//     moves the imul and costs 30-odd percent elsewhere), a static inline
//     row-pointer helper, and <windows.h>/<stdio.h>/<string.h>/<math.h> in
//     every combination (all much worse, 22-53%). /Ob0, /Ob1, /Ob2, /Ob3,
//     /O1 and /O all give the same code, so it is not an optimisation level.
//  2. The threshold compare. Both spellings of the same test
//     (`*sp1 + level >= *dp1` and `*dp1 <= *sp1 + level`, plus
//     `level + *sp1 >= *dp1`) load level and *sp1 into ebx/edx in the same
//     order; the original adds them into edx and compares ebx (*dp1) with it
//     (`jg`), ours adds into ebx. Operand order alone never moves the add's
//     destination register, so this looks like the same allocation decision
//     as (1): the original was built by a compiler whose canonical operand
//     order for a commutative/associative chain differs from the SP3 build
//     in this repo. (Header-block state is the guide's other explanation.)
//
// The semantics below are exact, they are what the original does: the first
// argument supplies the source planes, the colour key, the width (inner count)
// and the height (outer count); the second supplies the destination row
// pointers and the row stride. The dead `mov edx, [esp+0x20]` after the
// inner loop is a reload of x, present in both.
//
// Claude Sonnet 5.5 pass (#589): 82.8 to 83.9 percent by declaring the locals at
// function scope in the order `yoff, stride, xoff, dp0, dp1` (that is what the
// file now has). All 120 orders of {xoff, yoff, stride, dp0, dp1} were scored:
// 60 give 83.9, 60 give 82.8, and the 83.9 ones are exactly those where `stride`
// is declared before `xoff` (and yoff before dp0/dp1), so the first operand of
// the row-pointer sum moves. The remaining diff is then only (1) the plane load
// (`mov esi,[plane0]; add esi,xoff` here, `mov esi,ebx; ...; add esi,eax` in the
// original, i.e. the original copies xoff and adds the plane afterwards), (2) the
// `yoff` spill after the guards, and (3) the add destination of the threshold
// compare. Crossed with all 6 orders of the row-pointer sum and the 4 spellings
// of the compare (24 files): no change (the two `*dp1 <=` spellings give 82.8).
// N unused `extern int` declarations, N = 8 to 208 step 8: 83.9 at best, the
// rest much worse (22 to 68), so it is not the declaration-count state that
// helped 0x4b9360 in the same issue.
//
// space-bunny-free pass (#1117), 83.9% confirmed again, nothing better found.
// Independent check of the argument roles and slots, since a misread there
// would have made every row-pointer form look hopeless: with 2 dword locals and
// the 4 pushed registers the first argument is read at [esp+0x10] before the
// pushes and the second at [esp+0x14] after two pushes, both of which resolve
// to E0+8 and E0+4, so the bitmap at E0+4 is the parameter this file calls
// `src` (its color key, width and height drive the loops) and the one at E0+8
// is `dst` (its +4/+6 offsets, its width as the row stride). The file already
// had the roles the right way round. The three int parameters sit at E0+0xc,
// E0+0x10 and E0+0x14, and the first two of those slots are reused as the
// inner and outer loop counters, which is why the level is reloaded from
// [esp+0x2c] twice inside the inner loop. Nothing is read past the arguments,
// so there is no over-read bug here.
// New results this pass, all scored for free with check.py --sym:
//   * rows written as three statements each, `base + plane` then `+= stride`
//     for both, and the int sum cast to unsigned char* before adding stride:
//     all 83.9, the diff in the loop head is unchanged.
//   * the two row statements in the other order: 83.9, no change.
//   * dst->plane0 and dst->plane1 read into two locals first: 83.9.
//   * `stride = yoff * dst->width` instead of `dst->width * yoff`: 83.9.
//   * `int stride` declared in the loop body instead of at function scope:
//     82.8, so the function-scope declaration is worth 1.1 points.
//   * the row count as `while (1) { ...; row++; yoff++; if (row >= h) break; }`
//     per item 9 of the guide: 78.3, and `yoff++` as its own statement at the
//     end of the body: 77.4. The `for` form is right.
//   * the threshold compare hoisted into a local (`int t = *sp1 + level;`):
//     72.7. It has to stay inline in the condition.
//   * `stride` inlined into the first row only: 45.2, the imul moves out of
//     the loop head.
// The three remaining differences are each a single instruction group and none
// of them moves under any spelling tried, which supports the reading that the
// original was not built by this compiler build (or at least not by MSVC 5
// SP3 with this header state): the row pointers, the yoff spill and the add
// destination of the threshold compare are all decided by the same allocator.

// space-bunny-free pass (#1857), 83.9% again, 1 check run, 1 free --sym score.
// This pass only had to settle what the original really does at 0x4b9111, the
// address the recorded rule was cited from, so I re-read the bytes instead of
// guessing. The full block is
//     mov esi, ebx              ; esi = xoff, copied out of ebx
//     mov cx, [edx]             ; dst->width
//     imul ecx, eax             ; stride = dst->width * yoff
//     mov eax, [edx+0x10]       ; dst->plane0
//     add esi, eax              ; <-- the differing add
//     mov eax, ebx              ; eax = xoff again, for the second row
//     mov ebx, [edx+0x14]       ; dst->plane1 (xoff's register is now free)
//     add esi, ecx              ; esi = (xoff + plane0) + stride
//     add eax, ebx
//     add eax, ecx              ; eax = (xoff + plane1) + stride
// so the source really is a plain `int + pointer` in a two-term chain, the int
// is the first operand of the first add, the result is used only as a row
// pointer, and it is not a pointer difference, an index multiply or a char*
// advanced by a value read through another type. The recorded rule
// ("int + pointer ALWAYS canonicalises pointer-first") therefore HOLDS here,
// and the row-pointer shape is not reachable by re-spelling it.
// The escape the rule does not cover is an all-integer sum, so I scored that
// (free, check.py --sym, no run spent): with the plane fields read as ints and
// the whole row address built as one integer sum, cast to unsigned char* only
// at the end, `dp0 = (unsigned char*)(xoff + (int)dst->plane0 + stride);`, the
// result is byte for byte the same 256-byte code and the same diff. So MSVC 5
// SP3 reorders the terms of `xoff + plane + stride` to (plane + xoff) + stride
// even when no pointer type is left in the tree, which puts the rule's real
// scope beyond int+pointer: it is the term order of the whole sum, and the
// original's is the source order. That also explains the third difference (the
// threshold add, `add edx, ebx` against our `add ebx, edx`) as the same single
// ordering decision, and the yoff store placement with it.
// The 83.9% file is unchanged; nothing I tried beat it.
//
// deepseek-v4.1-flash pass (#1281), 83.9% confirmed a fourth time. The whole
// 4-byte shortfall is the row-pointer block: the original makes xoff the add
// destination (`mov esi,ebx; mov eax,[edx+0x10]; add esi,eax; mov eax,ebx`)
// while this toolchain always canonicalises int+pointer to pointer-first
// (`mov esi,[edx+0x10]; add esi,ebx`). Tried this pass, all 83.9 and byte for
// byte the same: the plane fields as `int` with casts, `unsigned int` casts on
// both addends, separate int/unsigned temps assigned in one statement and
// materialised in the next (dp0/dp1 first, stride first, stride between),
// `(unsigned char*)xoff`, `+=` chains, a dst pointer alias, a local plane
// pointer pair, and recomputing the xoff expression in the row pointer. The
// yoff spill placement and the threshold add destination move with the same
// allocator decision. This is the compiler-state plateau the guide describes;
// it should resolve when the file is regrouped into its original translation
// unit.
struct Bitmap_004b90a0 {
    unsigned short width;      // +0x0
    unsigned short height;     // +0x2
    short field_4;             // +0x4
    short field_6;             // +0x6
    unsigned char colorKey;    // +0x8
    char unknown_9[7];         // +0x9
    unsigned char* plane0;     // +0x10
    unsigned char* plane1;     // +0x14
};

// FUNCTION: 0x4b90a0
void __stdcall FUN_004b90a0(Bitmap_004b90a0* src, Bitmap_004b90a0* dst,
                            int x, int y, int level)
{
    int yoff;
    int stride;
    int xoff;
    xoff = dst->field_4 - src->field_4 + x;
    yoff = dst->field_6 - src->field_6 + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++, yoff++) {
        stride = dst->width * yoff;
        unsigned char* dp0 = xoff + dst->plane0 + stride;
        unsigned char* dp1 = xoff + dst->plane1 + stride;
        int n = src->width;
        while (n--) {
            unsigned char c = *sp0;
            if (c != src->colorKey && *dp1 <= *sp1 + level) {
                *dp0 = c;
                *dp1 = *sp1 + level;
            }
            dp0++;
            sp0++;
            dp1++;
            sp1++;
        }
    }
}
