// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// RETRY NOTE: Current best is 97.5% (205 bytes), verified once this pass.
// The remaining two-instruction block-order diff is the target argument load:
// target build loads ebx before `push 4` / `lea eax, [esp+0x14]`; ours loads
// ebx after them. Prior passes tried headers, loop/exit shapes, declaration
// orders, and target parameter copies without changing that scheduler choice.
// THIRTEENTH PASS (space-bunny-free: headers.py first, one real check.py run
// for the baseline, then free check.py --sym probes. No change: still 97.5%,
// 205 bytes, the same two instruction preheader displacement, verified by a
// fresh run of the unchanged file.)
//   * headers.py: 128 sets, no match, closest 97.5% (<windows.h>, <string.h>,
//     <ddraw.h> and the pairs), confirming the eleventh pass.
//   * NEW, and it settles an ambiguity every earlier pass left open: the
//     original's frame arithmetic. esp is S = E0-20 after the prologue, so
//         [esp+0x0c] = E0-8   total (local)
//         [esp+0x10] = E0-4   name[4] (local)
//         [esp+0x14] = E0+0   the RETURN ADDRESS slot, reached only as
//                             &name AFTER the `push 4`, i.e. at esp = S-4
//         [esp+0x18] = E0+4   the `off` local, in the dead first argument slot
//         [esp+0x1c] = E0+8   the `target` argument
//     so the off local really is in the dead file argument slot (esi keeps the
//     live copy), and the strncmp's first argument is `name`, four bytes at
//     E0-4, not the return address. The identical trick appears in the
//     sibling 0x4d0910 (`push 4` then `lea eax,[esp+0x10]` = its tag), which
//     is the proof that the 0x14 lea belongs to the tag and not to a second
//     buffer. There is no out-of-frame read and no bug here.
//   * NEW negative: the loop's match exit as `goto found;` to a trailing
//     `found: return off;` (build/scratch/0x4d0720/g1.cpp) is byte identical
//     at 97.5% / 205 bytes with the identical diff, so the two separate
//     epilogues do not depend on the exit being a `return` inside the loop,
//     and the preheader fault is independent of the exit block shape too.
//   * The preheader in the original is, instruction for instruction, the latch
//     with the induction update substituted (push 4 / lea / push ebx / push
//     lea / <induction op> / call) plus one parameter load at its head, and
//     the only thing this source cannot produce is that head load: MSVC 5
//     emits the def of the callee-saved ebx at its use (after the lea, one
//     push late, displacement 0x20) for every source shape tried, while the
//     sibling 0x4d0910 needs no load there at all (its strncmp argument is
//     the string constant "data") and its preheader already matches. So the
//     blocker is a single backend decision: where a parameter copy into a
//     callee-saved register is materialised inside a rotated loop preheader.
// TWELFTH PASS (deepseek-v4.1-flash): body unchanged at 97.5%, 205 bytes, the
// same two-instruction preheader displacement. headers.py was rerun first: no
// header set matches, closest is 97.5%. Six fresh probes, all scored without
// spending a real run beyond the baseline:
//   * the test in the `for` condition (build/scratch/0x4d0720/pa.cpp) is
//     91.6% / 218 bytes, not rotated, the known top-tested/not-rotated shape.
//   * `pos = 0x14;` BETWEEN the name and off reads (pb.cpp) is 97.5% / 205,
//     identical diff, confirming the earlier "above the last read" result.
//   * an inline `Same(char*, char*)` comparison helper (pc.cpp) is 97.5%.
//   * a live `char* t = target;` copy across the whole loop, used in both
//     tests (pd.cpp), and the same copy placed between the name read and the
//     off read (pf.cpp), are both 97.5%: the copy coalesces into the
//     parameter, as all earlier copy spellings did.
//   * an explicitly peeled first test plus a bottom test (pe.cpp) is
//     56.3% / 223 bytes, the same non-rotated shape as the earlier peeled
//     probes.
// ELEVENTH PASS (deepseek-v4.1-flash, check.py --sym scoring only; the file is
// unchanged at 97.5%, 205 bytes, the same two-instruction preheader
// displacement). Only genuinely new negatives, none of which repeats an
// earlier pass:
//   * flags. /Gz, /Gr, /G5, /Gd, /GA, /Oi, /Ow, /Ot and /Ox all give the
//     identical 97.5% / 205 byte diff; /G6 is 93.8%. So the preheader tie is
//     calling-convention and flag invariant, unlike the guide's 0x44b990.
//   * a `char* np = name;` local used for every name read and the strncmp is
//     flat (the stack address rematerialises).
//   * `for (pos = 0x14; ; pos += 8)` with the increment in the for clause and
//     the body's trailing `pos += 8` removed is flat.
//   * `pos = sizeof(unsigned int) * 5;` and `pos = 20;` are flat.
//   * a live `char* t = target;` copy used in BOTH strncmp tests (so its live
//     range spans the whole loop) is flat: it still coalesces into the
//     parameter and the load is not hoisted.
//   * a `struct M { char* t; } m; m.t = target;` wrapper is flat.
//   * comma expressions that assign pos inside a strncmp argument
//     (`(pos = 0x14, target)` as arg2, `(pos = 0x14, 4)` as arg3) drop to
//     36.6% / 190 bytes, so the assignment cannot ride on the argument setup.
// NINTH PASS (deepseek-v4.1-flash): no change, still 97.5%, 205 bytes, the same
// two instruction displacement. This pass focused on the preheader's basic
// block structure and confirms the residual is exactly the scheduler tie the
// guide records at 0x4b6570: the load's displacement shifts by precisely 4
// (0x1c against 0x20) across one push, the signature the guide says not to
// spend budget on. Every variant below was scored free with check.py --sym.
// New negatives:
//   * explicit rotated double tests do NOT produce the preheader. The two-test
//     `if (strncmp(...) != 0) { do { ... } while (strncmp(...) != 0); }` shape
//     is 56.3% / 223 bytes, and a goto double test is identical: with both
//     tests spelled out MSVC keeps a second exit and never forms the clone.
//   * `for (pos = 0x14; strncmp(...) != 0; pos += 8)` (three spellings) is
//     91.6% / 218 bytes with the SAME preheader diff plus a duplicated
//     epilogue, so the preheader fault is independent of how the loop is
//     written, which rules out the remaining rotation-clone theory.
//   * `while (strncmp(...) != 0) { ... } return off;` is 91.6% / 218 bytes
//     (not inverted); every do/while spelling is 70.4% or worse.
//   * inline `Name(name)` / `Target(target)` identity accessors (the item 28
//     allocation lever), used together, alone, and as a `char*&` helper, are
//     all 97.5% with the identical diff.
//   * parameter spellings `const char*`, `char* const`, `unsigned char*`,
//     `int`, and an `unsigned char name[4]` buffer are all 97.5%.
//   * argument expressions around target: `(target, target)`, `*(&target)`,
//     `(char*)(void*)target`, `(const char*)target`, `(target + 0)` are all
//     97.5%; only a real ternary `(target ? target : target)` moves the block
//     (69.3% / 187 bytes, the loop restructured).
// The source shape is settled; what remains is backend block scheduling.
// EIGHTH PASS (deepseek-v4.1-flash): no change, still 97.5%, 205 bytes, the same
// two instruction displacement. Attacked the "is the load a hoisted definition
// or a use point reload" question directly, all free with /Fa listings and
// check.py --sym:
//   * an inlined identity helper (`Get(target)`), an inlined reference helper
//     (`char*& Ref(char*&)`), the `register` keyword, a union round trip
//     through `unsigned int`, `(char*)((unsigned int)target & 0xffffffff)`,
//     `target + (total - total)`, and a `char* t = target;` copy declared
//     INSIDE the loop body (so LICM has something to hoist) all coalesce or
//     rematerialise and leave the preheader byte identical.
//   * `pos = 0x14;` moved before the reads or between the two reads removes the
//     pos init from the preheader entirely (it is emitted before the off read),
//     and the target load STILL lands after the `lea`, not at the block top.
//     That is the proof that the load is a use point reload inserted next to
//     `push ebx`, not a hoisted definition: no source shape tried makes MSVC
//     define the parameter register ahead of the argument setup. The original's
//     `mov ebx,[esp+0x1c]` at the block head is a backend scheduler/list tie of
//     the same class the guide records at 0x4b6570, 0x426200 and 0x4a35a0, not
//     reachable from this source.
// SEVENTH PASS (space-bunny-free, free scratch scoring and /Fa listings only).
// Still 97.5%, 205 bytes, the same two instruction displacement. Two new facts,
// both from the compiler's own listing, worth keeping:
//   * where the pos initialiser sits in the SOURCE decides where its def is
//     materialised, and the two choices are not equal. With `pos = 0x14;`
//     AFTER the off read (this file) the def lands inside the preheader, in
//     the middle, between the lea and the first argument push:
//         push 4 / lea eax / mov ebx,target / mov edi,0x14 / push / push / call
//     With `pos = 0x14;` hoisted ABOVE the off read (build/scratch/0x4d0720/
//     e1.cpp) the same def is sunk across the following call instead, into the
//     pre-loop block right before it, and the preheader loses it entirely:
//         lea edx,&off / push 4 / push edx / push esi / mov edi,0x14 / call
//         push 4 / lea eax / mov ebx,target / push ebx / push eax / call
//     So the original's copy, which is the LAST thing in its preheader, is
//     neither of the two positions this source can reach: the copy is material
//     ised in the preheader in the original, and every spelling that keeps it
//     in the preheader puts it before the argument pushes.
//   * the bottom tested spellings DO NOT peel once the initialiser is hoisted
//     above the off read. do {} while(strncmp(...) != 0), do {} while(!...),
//     do {} while(...) truthy, and a bottom tested `if (...) break;` to a
//     trailing `return off;` (build/scratch/0x4d0720/t1.cpp t2.cpp t3.cpp) all
//     keep the register promotion of pos (add ecx,edi / add edi,ecx / add
//     edi,8 in the body, and the register form of total += 8 in the pre-loop)
//     and all four converge on the SAME preheader as the top tested for(;;),
//     with the ebx load in the middle. So the rotation is not a bottom tested
//     only transformation here: MSVC 5 makes it for every spelling, and the
//     preheader is always the loop's own header block, scheduled from scratch.
//     The original's preheader is instead byte for byte the latch with the
//     induction variable update substituted (same seven instructions, same
//     relative order, the lea in eax here against ecx in the latch), plus the
//     hoisted target load at its head, which is what a rotation CLONE looks
//     like. The sixth pass already concluded no spelling reaches it; this pass
//     confirms it from the other side, since the bottom tested forms now give
//     the top tested preheader rather than a cloned one.
// Also measured and flat at this same diff: a comma expression
// `(pos = 0x14, name)` as strncmp's first argument, which costs the whole
// allocation (target lands in edi, pos folds to off + 0x14 and the frame loses
// a register), so it is not the way to sink the copy to the end of the block.
// SIXTH PASS (space-bunny-free, one real check.py run for the baseline plus
// free check.py --sym scoring of 27 variants, ~0.3 s each). Still 97.5% and
// 205 bytes, still the same two instruction displacement in the preheader.
// New negatives, all free:
//   * the AUTHENTIC TA source shape, `for (pos = 0x14; pos < total; pos += 8)`
//     with the body's redundant `if (pos >= total) break;` and a trailing
//     `return 0;`: 57.5% / 202 bytes. MSVC peels the first iteration and
//     constant folds pos, exactly as for every other bottom tested spelling.
//   * a hand SPLIT of the preheader, to test whether the original's preheader
//     is really a separate basic block: a `goto loop;` immediately before the
//     loop label, so the pre-loop statements and the strncmp test end up in
//     two blocks (the redundant jump is dropped by the block layout, so the
//     byte stream is unchanged). Six shapes: bottom tested after the split
//     70.4% / 203 bytes, and top tested after the split (`goto loop;` then
//     `for(;;)` with the test at the top, with a nested block around the pos
//     init, with a break, and as a bottom tested `goto` to a trailing test
//     label) all 97.5% / 205 bytes with the identical diff. So the preheader
//     cannot be split at the source level into two blocks that allocate
//     differently, which kills the "the original's preheader is a rotation
//     clone" theory as a source shape: MSVC 5 makes the same decision for a
//     split and an unsplit preheader.
//   * `for (pos = 0x14; ; pos += 8)` with the test at the top of the body,
//     `pos = pos + 8`, `while (1)` with the test at the top, and a dead
//     `unsigned int i = 0x14; (void)i;` before the init: all 97.5%.
//   * the two exits swapped, i.e. `for (;;) { if (test) break; ... } return
//     off;` and `while (1) { ...; if (test) return off; }`: 97.5% and 69.3%
//     / 187 bytes respectively. So the epilogue order is not the lever either.
//   * every bottom tested spelling that puts `mov ebx,[esp+0x1c]` at the top
//     of the preheader (do/while, do/while with the test in the condition, a
//     goto to a bottom test with and without the preheader split) is also
//     exactly the spelling that folds `pos + off` to `off + 0x14` and
//     `total + 8` to an in place add: 70.4% / 203 bytes, 202 to 203 bytes
//     against the original's 205. The load's position and pos's register
//     promotion are ONE decision, and the original has the combination no
//     source spelling found produces.
// PARTIAL: 97.5%, one instruction out of place. Confirmed by two real
// check.py runs in the last pass, 97.5% both times. Code size matches
// exactly (205 bytes) and every instruction matches except the position of
// two copies inside the loop preheader at 0x4d076c-0x4d077d.
//
// The original:
//     mov ebx,[esp+0x1c]      ; target, loaded first
//     push 4
//     lea eax,[esp+0x14]      ; &name
//     push ebx
//     push eax
//     mov edi,0x14            ; pos = 0x14, sunk to just before the call
//     call strncmp
//
// this source gives:
//     push 4
//     lea eax,[esp+0x14]
//     mov ebx,[esp+0x20]      ; same load, one push later
//     mov edi,0x14
//     push ebx
//     push eax
//     call strncmp
//
// The 0x20 against 0x1c is not a second fault, it is the same one: the
// displacement follows the push the load sits behind. The latch is already
// identical in both versions, including its own
// push 4 / lea ecx,[esp+0x14] / push ebx / push ecx / add edi,8 / call
// sequence, so the preheader is the latch plus one hoisted parameter load and
// one induction variable initialiser.
//
// NEW EVIDENCE (second pass, all free with check.py --sym). The two shapes are
// separated by exactly one thing, the loop's test position, and it cannot be
// had both ways:
//   * every TOP tested spelling emits this same 205 byte body with the same two
//     instruction displacement: for(;;) with the test first, while(1), do{}
//     while(1), a goto back to a top test, the test first with break plus one
//     return at the end, pos += 8 written as the for loop's third clause,
//     strncmp(...) == 0, !strncmp(...) and 0 == strncmp(...), unsigned int and
//     int for off, char* t = target used in the test, name as an unsigned int
//     read through a cast, and 0x14 / 0x14u spellings. All 97.5%, all 205
//     bytes, all with the identical two line diff.
//   * every BOTTOM tested spelling DOES put the load at the top of the
//     preheader, as the original has it: do{}while(strncmp(...) != 0) and
//     while(strncmp(...) != 0) both give
//     `mov ebx, dword ptr [esp + 0x1c]` first. They are also the only forms
//     that score 70.4% / 203 bytes, because the loop then loses its register
//     promotion: pos stays in memory (add dword ptr [esp+0x14], 8 / mov eax,
//     [esp+0x18] / add eax, 0x14 / lea edi, [ecx+0x14]), where the original has
//     pos in edi. Putting the test at the end of the body as a goto back, or
//     as a continue, or fusing pos += off + 8, keeps the load at the top and
//     still loses the promotion (55.8% to 70.4%).
//   So the original's preheader behaves like a rotation CLONE of the latch
//   (same relative order of the two copies, and the lea in eax here versus ecx
//   in the latch, so the two blocks really are allocated separately), but no
//   bottom tested source keeps the loop's register allocation. Somebody with a
//   stronger model should look for what makes the top tested spelling promote
//   pos to edi, and then rotate that loop by hand with a goto.
//   * a third data point on where the copies come from: hoisting
//     `pos = 0x14;` above the FUN_004bb7c0(file, &off, 4) read (so it is dead
//     across that call and must be rematerialised) moves `mov edi, 0x14` to
//     the TOP of the preheader, still 97.5% and 205 bytes. So the copy is
//     emitted at its source point and then moved by the block scheduler, and
//     no spelling found emits the target load before the first argument push.
//
// Everything else is byte exact:
//   * frame: sub esp,8, push ebx/esi/edi, file in esi, pos in edi, target in
//     ebx, and the "off" local living in the dead `file` argument slot at
//     [esp+0x18]. Confirmed by the compiler's own /Fa listing, which prints
//     _file$ = 8, _target$ = 12, _off$ = 8, _name$ = -4, _total$ = -8.
//   * "total += 8" in a register with the store after the argument pushes
//     (mov edi,[total] / push 0xc / add edi,8 / push esi / mov [total],edi),
//     not the in place add dword ptr [total],8 that the bottom tested forms
//     and the neighbours 0x4d0910 and 0x4d07f0 get.
//   * the destructive seek argument mov ecx,[off] / add ecx,edi / push ecx,
//     with `off` reloaded after the call, so the source really is
//     FUN_004bb710(file, pos + off); pos += off; written twice over.
//   * the rotated loop: test at the top with je to the "return off" block
//     placed after the loop, inverted test and back edge in the latch, the
//     ja e unsigned compare against total, and both epilogues separate.
//   * strncmp, not memcmp, and 4 as the third argument in both calls.
//
// TRIED AND REJECTED, all scored with check.py --sym on a scratch copy
// (free, so none of these cost a run):
//   * "for (pos = 0x14; ; )" and "unsigned int pos = 0x14;" as an
//     initialiser: the second sinks "mov edi,0x14" to the top of the block
//     and turns "total += 8" into the in place add, 97.5% -> lower.
//   * "char* t = target;" copied at the top of the body, after the last
//     read, and as a declared-then-assigned local: the copy is coalesced
//     into the parameter either way and the block is unchanged.
//   * the block, hoisted-by-hand ("if (strncmp(...) == 0) return off;" before
//     the loop, and the test again at the latch): 56%, because with the
//     first test explicit MSVC proves pos is still 0x14 and the seek folds
//     to a constant, so nothing is left to rotate.
//   * "while (strncmp(...) != 0) { ... return 0; } return off;": 91.6% and
//     218 bytes, the loop is not rotated, the exit needs a 6 byte
//     relocatable jump, and the second epilogue's xor eax,eax is hoisted
//     above the pops. Its preheader is the same 97.5% one, so the loop
//     spelling and the preheader fault are independent.
//   * "do { ... } while (strncmp(...) != 0);": 70.4%, 203 bytes.  With
//     "total <= pos" for the bound, or with pos initialised in its
//     declaration, 69.2% and 70.4% of the same 203 bytes, still no
//     promotion.
//   * "while (1) { if (...) return off; ... }": rotates the whole loop,
//     moves file to edi and the offset to esi, and reorders the back edge.
//     Do not try while(1) here.  With break and one return at the end it is
//     69.3% / 187 bytes.
//   * a goto back to a top test: 97.5%, identical diff.  A goto back from the
//     bottom: 70.4%, 203 bytes, load at the top, no register promotion.
//   * "pos += off + 8" fused into one update: 55.8%, 195 bytes.
//   * "pos = 0x14;" hoisted above the last read: 97.5%, 205 bytes, the
//     "mov edi, 0x14" moves to the top of the preheader (see above).
//   * hoisting it above the second-to-last read: 66.3%, 212 bytes, and it
//     moves total and name in the frame.
//   * test inverted with an else arm, the test result in a local, a
//     static __inline and a macro for the comparison, 4 written as 4u,
//     (unsigned long)4 and sizeof(name), explicit (char*) casts on either
//     argument, strncmp(target, name, 4), a swap of the two reads in the
//     loop body, "pos += 8" moved before the reads, "off + pos" and
//     "pos = pos + off", int instead of unsigned int for pos, off or total,
//     name as an unsigned int, and six declaration orders of name/total/pos/
//     off: all byte identical at 97.5% with this same two instruction
//     displacement. (Two declaration orders, off first, do change the
//     allocation, 82.5%, because the seek becomes "lea edx,[edi+ecx]"
//     instead of the destructive "add ecx,edi"; the other four are
//     unchanged.)
//   * headers.py: no header set beats <string.h>.
// The loop spelling and the whole tail are therefore settled. The residual is
// the same tie the guide records at 0x4b6570 and 0x426200, a local reload
// drifting around a call's argument pushes with the displacement shifting by
// 4 per push, except that here it is the register allocator plus list
// scheduler tie inside one basic block.
//
// THIRD PASS (deepseek-v4.1-flash, free --sym scoring only; the baseline was
// the one real run). All still 97.5%, same two-instruction displacement:
//   * the natural `while (strncmp(...) != 0) { ... } return off;` is 91.6% /
//     218 bytes (not rotated), the peeled `if (...) { do {} while (...) }` and
//     `do {} while` are 56.3% / 223 bytes, `while (1) { ... break; }` is 61.4%
//     / 187, so the top-tested `for (;;)` is not in doubt.
//   * all the identity forms of a target local (`char* t = target;` before or
//     after `pos = 0x14`, `&target[0]`, `(char*)(void*)target`, a union, a
//     struct copy, `t = *(&target)`, `t = *pp` with `char** pp = &target`)
//     coalesce into the parameter and leave the block untouched.
//   * making `name` an `unsigned int` array (`(char*)&name`), declaring the
//     parameter `const char*`, passing the count through a local, `Cmp(name,
//     target)` / macro / `static __inline` helpers with either parameter order
//     (including one whose body swaps the arguments) all leave it untouched.
//   * swapping the two pre-loop reads is 95.0%, hoisting `pos = 0x14` above
//     them is 93.8%, and 0..400 unused `extern int` declarations, 400..2400 in
//     steps of 20 unused prototypes and all 768 header sets of headers.py
//     --cpp are flat at 97.5%. This is compiler state, not a missing source
//     shape.
// The one remaining difference is the preheader's target load drifting one
// slot: it is the same scheduler tie the guide records at 0x4b6570, 0x426200
// and 0x4a35a0, not reachable from this function's source.
//
// FOURTH PASS (space-bunny-free, one real run plus free --sym scoring).
// Still 97.5%, 205 bytes, same two instruction displacement. New negatives:
//   * dead __inline helpers called 1, 2, 3, 4 and 6 times after the last read
//     (item 14 of the brief): flat at 97.5%. Nothing here is inlined, so the
//     /Ob2 budget has nothing to flip.
//   * a 72 way sweep of the four spellings that could move a scheduler tie
//     (four tests of the strncmp result, three forms of the bound, the pos
//     initialiser as a statement or as a declaration, and int against
//     unsigned for off and total): every combination is either the 97.5%
//     diff above, 95.0% (the ne/else spelling, a different compare), 91.6%
//     (the ne spelling, 218 bytes, loop not rotated) or 91.1% (pos as a
//     declaration with initialiser, 199 bytes, pos demoted to memory).
//   * four more ways of spelling the copy that might have survived copy
//     propagation, so that the ebx load is emitted at the copy's source point
//     instead of being hoisted: char* const t = target, const char* t =
//     target, char* t; t = target, and a struct field. All four coalesce into
//     the parameter and the block is byte identical. So the load really is a
//     hoisted invariant use of the parameter, never a source level copy.
//
// The one thing that DOES put "mov edi, 0x14" last, just before the call, as
// the original has it, is the identical loop in the neighbouring function
// 0x4d0910, whose strncmp second argument is a string CONSTANT. There the
// preheader is push 4 / lea / push const / push eax / mov edi, 0x14 / call,
// with no load to schedule at all, and that file matches. So the ebx load
// is what drags the copy one slot earlier, and no spelling of the source
// moves the load ahead of the first argument push.
//
// FIFTH PASS (space-bunny-free, one real check.py run for the baseline plus
// free --sym scoring of every variant below; scoring costs ~0.3 s each, so
// this pass was a wide one). Still 97.5%, 205 bytes, same two instruction
// displacement. New negatives, all with the identical diff:
//   * a full 32 way combinatorial sweep of (total: unsigned/int) x (off:
//     unsigned/int) x ("total += 8" / "total = total + 8") x ("pos += 8" /
//     "pos = pos + 8") x (pos initialised by a statement / by a declaration
//     initialiser), all with the good declaration order (name, total, pos,
//     off). Every one of the 32 is 205/205 at 97.5% with the same two lines.
//     The two orders that put `off` before `pos` are 82.5% because the seek
//     becomes "lea edx,[edi+ecx]", so that order is the only load bearing
//     one and it is already right.
//   * (void)target; as the first statement, which extends the parameter's IR
//     live range to the top of the function without emitting anything:
//     97.5%, unchanged. So the ebx def really is materialised inside the
//     preheader and nothing about the parameter's live range reaches it.
//   * "if (!strncmp(...))", braces on the two exits, a nested block around
//     the loop body and around the tail of the pre-loop, "pos = 0x10 + 4",
//     "(unsigned char)name" plus a cast at the strncmp, "register unsigned
//     int pos", "FUN_004bb7c0(file, &name[0], 4)", and a local prototype
//     for strncmp with an unsigned count instead of <string.h>: all 97.5%.
//   * re-measured the loop shapes. "while (strncmp(...) != 0) { body }
//     return off;" and the if/else with the body in the then arm are 91.6%
//     / 218 bytes, and BOTH of them keep the preheader of the 97.5% version
//     (load after the lea), which is the third independent confirmation
//     that the preheader fault does not depend on the loop spelling. Only
//     "do { body } while (strncmp(...) != 0);" puts the ebx load at the top
//     of the preheader, and that one is 70.4% / 203 bytes because MSVC peels
//     the first iteration and folds the seek to "mov eax, off / add eax,
//     0x14": with the test at the bottom, pos is provably still 0x14 at the
//     loop entry, so no spelling can keep both the register promotion and
//     the top-of-block load. See the listing in
//     build/scratch/0x4d0720/bt2.lst for the peeled shape.
// The conclusion of the four passes before this one stands: the code size,
// the frame, the block structure and every instruction are right, and the
// residual is where the backend materialises the ebx def inside the rotated
// preheader (top of block in the original, after the lea in ours). The
// /Fa listing of ours (build/scratch/0x4d0720/t.lst) shows the whole
// preheader attributed to the "pos = 0x14;" line, so the two def trees were
// inserted by the register allocator after the block's argument-setup trees,
// while the original has the load ahead of them. Nothing in the source
// reaches that decision.
//
// SIXTH PASS (deepseek-v4.1-flash, one real check.py run for the baseline plus
// free --sym scoring). Still 97.5%, 205 bytes, the same two instruction
// displacement. This pass attacked the "one upstream cause" idea and the
// compiler-state idea directly, and both are now ruled out with wide sweeps:
//   * headers: all 1024 subsets of the ten headers the source could plausibly
//     have included (<windows.h>, <stdio.h>, <stdlib.h>, <string.h>, <math.h>,
//     <memory.h>, <ddraw.h>, <mmsystem.h>, <dsound.h>, <dplay.h>); 880 compile
//     and every one is 97.5%. The prior headers.py runs missed mmsystem.h,
//     dsound.h and dplay.h, which is why they are named here.
//   * compiler state: the N-declarations test redone at step 1 for N = 0..1499
//     unused `extern int` declarations. Every single N is 97.5%. The earlier
//     step of 20 could not have missed a narrow window this wide.
//   * the real preceding function: defining the matched 0x4d06c0 (and 0x4d0620)
//     above this function in the same file is 97.5%, so it is not the original
//     file's earlier contents either.
//   * a 648 way sweep of 6 declaration orders x 4 type sets (unsigned/int
//     total, off, pos) x 3 loop forms (for(;;) with return, for(;;) with break
//     to a shared `return off`, a `test:` label with goto) x 3 condition
//     spellings (== 0, !, 0 ==) x 3 `pos = 0x14` placements (after both reads,
//     between them, before them). The whole space collapses to four states,
//     97.5 / 91.1 / 82.5 / 78.5; nothing beats 97.5, and 97.5 is always this
//     exact preheader.
//   * the comparison in an inline helper (Match/Cmp, == and !, 4 as a
//     parameter) is 97.5 or 95.0; `char** pp = &target` with `*pp`, `target+0`,
//     `&target[0]` and `(target, target)` are all the same coalesced 97.5; a
//     `char* t = target` copy at the top, before `pos = 0x14`, and at the use
//     site are all 97.5.
//   * callee spellings: `long` return on FUN_004bb710 (its real signature) and
//     `unsigned char*` on FUN_004bb7c0 both leave the block byte identical.
// The two lines are a scheduler tie inside one basic block: the target load
// wants to be the block's first instruction (as the original has it) and the
// `mov edi, 0x14` wants to be the last before the call, while MSVC 5 emits
// both after the `lea` for every source shape tried. Every other instruction,
// including the whole loop, both epilogues and every frame offset, matches.
//
// Walks a chunked file's marker table: the header holds the table size (plus
// the 8 bytes of the two header fields) and the first marker, then the table
// is a run of [4 byte name][4 byte offset] pairs. Returns the offset of the
// named marker, or 0 when the table runs out.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d0720
int __stdcall FUN_004d0720(void* file, char* target)
{
    char name[4];
    unsigned int total;
    unsigned int pos;
    unsigned int off;

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &total, 4);
    total += 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, name, 4);
    FUN_004bb7c0(file, &off, 4);
    pos = 0x14;
    for (;;) {
        if (strncmp(name, target, 4) == 0)
            return off;
        FUN_004bb710(file, pos + off);
        pos += off;
        if (pos >= total)
            return 0;
        FUN_004bb7c0(file, name, 4);
        FUN_004bb7c0(file, &off, 4);
        pos += 8;
    }
}
