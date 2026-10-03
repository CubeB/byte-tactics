// Decompiled by Space Bunny Free, finished by muse-spark-1.3-free, finished by space-bunny-free, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by DeepSeek V4.1 Flash, checked by GPT-6. Names are provisional.
// Codex GPT-6 retry for #5202 (2026-10-03): /Gi scores 97.4%, below
// the existing 99.3% default-flags source and does not fix the constant.
// GPT-6 retry (#4911): making sb.size address-taken through a local pointer
// before the allocator call leaves the 99.3% candidate unchanged. The remaining
// difference is still `mov eax, 0x14` versus `not eax; and eax, 0x14`.
// Rechecked for issue #5120 on 2026-10-03; the same constant-encoding hunk
// remains at 99.3%.
//
// DeepSeek V4.1 Flash session (issue 4790): 99.3% unchanged, still only the
// size constant encoding at 0x4bd183 (`mov eax,0x14` original versus
// `not eax; and eax,0x14` ours). tools/permute.py ran 2150 candidates in
// 3 minutes and stayed flat at 99.3%. Fresh structural attempts, all scored
// with check.py's diff:
//   99.1%  `sb.size = -(unsigned)(size_t)z + 20u;` emits
//          `mov eax,0x14 / sub eax,ecx` before the buf store: the mirror
//          schedule of the original's `mov eax,ecx / store / mov eax,0x14`.
//          `(unsigned)(size_t)z * -1 + 20u` is the same shape. Both keep the
//          zero in ecx and the early store, but the subtraction is not folded.
//   99.1%  `struct HapiBuf t = {0}; sb.size = (unsigned)(size_t)sb.buf + 20u;
//          sb.buf = FUN(t.buf, ...)` is the closest fold found: it gives the
//          original's `mov eax,0x14` and early store, but the sum-base buf
//          zero lands in eax (`xor eax,eax`) while the alloc-argument zero
//          lands in ecx, so no dead `mov eax,ecx`.
//   <=97.2% two-aggregate shapes that try to decouple the sum base from the
//          alloc argument (store one zero into the other struct, swap which
//          aggregate feeds the sum and which feeds the argument, local copies
//          of each zero) either swap the two zero registers or fold the
//          aggregate to immediate stores.
// Reading: MSVC 5 folds an opaque zero's `+ 20` into `mov r,0x14` only when
// the base register is dead after the sum (W3 note above and the 99.1% case);
// when the base is live as the alloc argument MSVC gives the base eax and the
// sum ecx, and never emits the dead copy. No spelling found that keeps the
// base in ecx, live, and still folds the constant.
//
// space-bunny-free session (issue 4714): 99.3%, a real step up from 99.1%, and
// the ONLY remaining difference is the encoding of the constant 20. The whole
// nine-instruction buffer-setup region now matches in shape and order:
//     xor ecx,ecx / mov eax,ecx / mov [esp+0x18],ecx / push eax
//     push "Package Data" / push ecx / mov [esp+0x20],eax / call
// reached with the aggregate plus an OPAQUE computed size:
//     struct HapiBuf sb = {20};
//     char* z = sb.buf;
//     sb.size = 20u & ~(unsigned)(size_t)z;
//     sb.buf = (char*)FUN_004d84a0(sb.buf, "Package Data", sb.size);
// which compiles to `xor ecx,ecx / mov eax,ecx / mov [esp+0x18],ecx
// / not eax / and eax,0x14 / push eax / ...`, so only
//     - mov eax, 0x14        (original)
//     + not eax / and eax, 0x14   (ours)
// differs. Both are 580 bytes overall, so nothing else differs.
// WHAT THIS CONFIRMS (several earlier notes had it backwards): the buffer's
// zero has to stay OPAQUE to the optimiser. A constant 0 would CSE into ebp
// (the `extra = 0` value) or become a literal, and the original's `push ecx`
// plus its fresh `xor ecx,ecx` show the zero is a real runtime value that
// happens to be zero: the aggregate's zero-fill. With an opaque zero, MSVC 5
// (a) keeps the copy `mov eax,ecx` that the earlier notes called an
// unexplainable "dead copy", (b) hoists the buffer store before the three
// argument pushes, and (c) puts the size store in the call delay slot, exactly
// as the original has them. The 20 must ALSO come from the `{20}` aggregate
// initialiser, because that is the only spelling that materialises it in a
// register (`mov eax,0x14`) shared by the push and the store; a plain
// `sb.size = 20;` folds both uses into immediates.
// STILL MISSING: a spelling whose LAST operation on eax folds to the literal
// 20 while the preceding copy of the zero stays. Measured this session, with
// `char* z = sb.buf` opaque and `{20}` (about 2900 variants in 14 one-MSLVC-run
// batches, build/scratch/4bd160/{g,s,grp,pat,mc,b2..b17}.py, all scored with
// check.py's own ratio and diff in parallel, 0.2 s a shape instead of 7 s):
//   9/9, 580 b, 99.3%  this file, and 24 byte-identical variants of it:
//     `z ^ 20u`            -> xor eax,0x14   (3 bytes, function 578)
//     `20u & ~z`           -> not eax; and eax,0x14  (2+3 = 5 bytes, 580)
//     mask-folded `&~`     -> same `not eax; and eax,0x14`
//   8/9, 580 b, 99.1%  the size read back from the aggregate:
//     `sb.size = sb.size + (unsigned)(size_t)z;` with `{20}` compiles to
//     mov eax,0x14 / xor ecx,ecx / add eax,ecx / mov [buf],ecx / push eax ...
//     i.e. the right registers, the right EARLY buffer store and the right
//     delay-slot size store, with `add eax,ecx` where the original has the
//     dead `mov eax,ecx` and the constant AFTER the store. This is the closest
//     structural miss measured anywhere in the exe for this function, and it
//     says what the original's two instructions are: an INDEPENDENT constant
//     (`mov eax,0x14`), not the result of any operation on the zero.
//   2/9, 576 b  every form that folds the block: `20u +/- 0u*z`, all the
//     algebraic identities (`z^z`, `z-z`, `z*0`, `z&0`), every folded ternary
//     (`z ? 20u : 20u` and 38 more), `20u` written as 0x14/8+12/'P'-60/
//     sizeof(Hapi_004bd160)/an enum/hex and octal literals, and every
//     `sb.size = 20;` (with `{0}`, `{0,0}`, `{20,0}`, a ctor, a nested
//     `struct HapiBuf t = {20}`, a by-value `Make(20)` helper, or a dead
//     `sb.size = z` before it). They emit `push 0x14 / mov [mem],0x14`.
//   Measured dead ends: `tools/headers.py --cpp` over all 1536 header sets
//     changes nothing (best stays 99.3%); separate `unsigned size; char* buf;`
//     locals cannot carry the frame at all (0x54 -> 0x50, 567 bytes), so the
//     struct is required; `sb.size = 20u * (1u+z)` and `(z+1u)*20u` give
//     `inc eax / lea eax,[eax+eax*4] / shl eax,2`; `20u*(1u+(z&0u))` folds.
//   Why the copy exists at all: MSVC only breaks the live range of the opaque
//     zero into eax for an operation that cannot be done in place, because ecx
//     has to stay live for `push ecx`. That is why `~`, `^` and `&` produce the
//     copy and `+`, `-` and `*` (doable as `add eax,ecx` in place) do not. The
//     original's copy is therefore an operation whose result MSVC then dropped
//     while keeping the operand copy, which no spelling tried this session
//     (nor in any earlier session's notes) produces: MSVC 5 folds an opaque
//     operand's operation only when the operation itself disappears, and it
//     then also deletes the copy. The byte pattern `8b c1 .. b8 14 00 00 00`
//     (`mov eax,ecx` within 8 bytes of `mov eax,0x14`) occurs EXACTLY ONCE in
//     the whole exe, here, so there is no near-copy to learn the spelling from.
//     tools/permute.py found nothing better either: 28740 rewrites in the
//     issue-4366 session and 7703 candidates in 15 minutes from this source
//     (both runs flat at the score they started from).
//
// claude-sonnet-5-5 (issue 4611): same 99.1% (580 of 580 bytes) with plain code
// and no dead `z` temporary: `struct HapiBuf sb = {0}; sb.size += 20;
// sb.buf = FUN(sb.buf, ...)`. Also 99.1% (same bytes): `sb.size = sb.size + 20`,
// `FUN(..., sb.size += 20)`, an inline Grow(HapiBuf&, n) helper, an anonymous
// union with `unsigned w[2]`, and `ibuf` as `unsigned`. The only difference
// left is that the aggregate's size zero is a separate `xor eax,eax` before
// `xor ecx,ecx` (then `mov eax,0x14`), where the original copies the buf zero
// with `xor ecx,ecx / mov eax,ecx` (one shared zero node). Ctor, `{0,0}`,
// `{0}` with plain stores, and assignment-chain spellings fold to immediates (576).
// (Older notes below describe earlier sources with a dead `z` temporary.)
//
// space-bunny-free session (issue 4366): still 99.1%, and the size is exactly
// the original's 580 bytes, so the whole remaining difference is the constant
// encoding in the buffer setup:
//     original                        ours (the file below)
//     xor  ecx, ecx                   xor  ecx, ecx
//     mov  eax, ecx                   mov  eax, 0x14
//     mov  [sb.buf], ecx              sub  eax, ecx
//     mov  eax, 0x14                  mov  [sb.buf], ecx
//     push eax / push str / push ecx  push eax / push str / push ecx
//     mov  [sb.size], eax             mov  [sb.size], eax
// `sb.size = 20u - (unsigned)z` is the only spelling found that keeps the
// aggregate's buf store EARLY (before the three argument pushes) and the size
// store late (after them), exactly as the original has them; it costs the
// extra `sub eax,ecx` where the original has the dead `mov eax,ecx`.
//
// BIGGEST NEW CLUE (7 of the original's 8 instructions, 578 bytes, 94.8%):
//     struct HapiBuf sb = {20};
//     char* z = sb.buf;
//     sb.size = 20u ^ (unsigned)((size_t)z | 0u);
//     sb.buf = (char*)FUN_004d84a0(z, "Package Data", sb.size);
// compiles to exactly
//     xor ecx,ecx / mov eax,ecx / mov [esp+0x18],ecx / xor eax,0x14
//     push eax / push 0x0 / push ecx / mov [esp+0x20],eax / call
// i.e. the DEAD `mov eax,ecx` and the original's exact store schedule and push
// order both appear, and only the constant is encoded as `xor eax,0x14`
// instead of `mov eax,0x14` (`&` instead of `^` gives `and eax,0x14`). So the
// dead copy is the accumulator copy of a bitwise operand whose operation MSVC 5
// keeps while folding its value to 20, and the original's size is a plain
// literal node. The spelling needed must both keep a value copy of the zero
// into eax and leave the folded constant as a literal: `+` forms delete the
// copy (`xor eax,eax / lea ecx,[eax+0x14]`, or immediate stores), `-` keeps it
// but needs the live `sub eax,ecx`, `&`/`^` keep it and fold to and/xor.
// 400+ hand-built variants were compiled this session (harness in
// build/scratch/4bd160/many*.py, one MSVC run per batch): aggregate forms
// ({20}, {0}, {20,0}, plain), zero sources (literal, local, member read,
// computed), casts (`(char*)(size_t)z`, `(unsigned)z`, `(int)z`, `(long)z`),
// bitwise and arithmetic no-ops on the zero, dead stores, unions, arrays,
// struct copies, by-value factories, assignment-expression arguments and
// call-argument spellings. tools/permute.py --jobs 2 also ran 28740 rewrites
// in 20 minutes from this source and found nothing better.
//
// What the dead `mov eax,ecx` is elsewhere in the exe: the image holds only
// five `xor ecx,ecx / mov eax,ecx` pairs (bytes 33 c9 8b c1) and the other
// four are in MATCHED functions, all register work rather than a copy of a
// stored zero: 0x480770 at 0x480a57 is the return value of a `return 0` whose
// zero sits in ecx after a float compare; 0x42d2e0 at 0x42d622 is the array
// index scaling of `field_1439b[index]` for `unsigned short index = 0`;
// 0x4a2e40 at 0x4a3037 is the same index scaling; 0x4ce260 at 0x4ce28d is
// `for (int i = 0; i < 100; i++) arr[i] = (i % 4) + 1`. A dead copy in the
// middle of straight-line code is therefore the accumulator copy of an
// expression, and it only survives when its parent operation survives.
//
// deepseek-v4.1-flash, issue 4317 (no change to the 98.3% source below, still
// differs only at 0x4bd17b..0x4bd198). BIG CLUE found this session: the size
// CAN fold to `mov eax,0x14` while keeping the fresh zero and the early buf
// store, by computing size FROM the buffer zero and passing a LITERAL 0 as the
// alloc p arg:
//     struct HapiBuf sb = {20};
//     sb.size = (unsigned)sb.buf + 20;
//     sb.buf = (char*)FUN_004d84a0((void*)0, "Package Data", sb.size);
// compiles to   xor eax,eax / mov [esp+0x18],eax / mov eax,0x14
//               push eax / push str / push ebp / mov [esp+0x20],eax / call
// i.e. fresh zero, early buf store, NO extra store, and the +20 FOLDS to the
// constant 20 (mov eax,0x14), NOT a lea. Verified with objdump. The aggregate
// zero is only opaque (lea ecx,[eax+0x14]) when sb.buf is ALSO the alloc arg;
// when the arg is a literal 0 the compiler folds sb.buf+20 to 20 because the
// zero is not live across the call. Three things separate this from the
// original: (1) the zero lands in eax (xor eax,eax), where the original uses
// ecx (xor ecx,ecx); (2) therefore no dead `mov eax,ecx` base copy (the fold
// overwrites eax directly); (3) the alloc p arg is a literal 0 that CSEs to
// ebp (push ebp), where the original pushes the buf zero itself (push ecx).
// The target is this W3 shape with the zero in ecx (so the fold emits
// `mov eax,ecx / mov eax,0x14` and the arg CSEs to it as `push ecx`). Every
// attempt to move the fold base from eax to ecx (value copies, deferring
// extra=0, local zeros, computed zeros,20-buf forms which put the zero in ecx
// but emit `sub eax,ecx` with no dead mov) kept the base in eax or folded to
// immediates. `sb.size = 20-(unsigned)sb.buf` gives the RIGHT order and a fresh
// ecx zero with an early buf store and no extra store, but emits
// `mov eax,0x14 / sub eax,ecx` (a live sub, no fold, no dead mov).
// The dead `mov eax,ecx` needs the +20 form (fold) with the base in ecx.
//
// mimo-v2.6-pro session (issue 3877 retry, ~26 fresh shapes, all scored with
// check.py --sym): best unchanged at 98.3%, this file. Key new fact: the
// aggregate's zero-fill value is OPAQUE to the optimizer, not a folded
// constant. `sb.size = (unsigned)sb.buf + 20;` after `struct HapiBuf sb =
// {20};` compiles to `xor eax,eax / mov [esp+0x18],eax / lea ecx,[eax+0x14]`,
// NOT to 20, so the zero feeds arithmetic as an unknown runtime value that
// merely happens to be zero. That spelling also gives the ORIGINAL'S EXACT
// STORE SCHEDULE (buf store early before the pushes, size store after the
// last push, push size / push str / push zero order) but with the registers
// swapped (zero in eax, size in ecx as the lea) and no dead mov, 92.2%.
// Everything else tried this session: assignment-expression arguments
// `(sb.buf = 0)` / `(sb.size = 20)` (98.3% / immediate form), member reads
// through char** and unsigned* aliases (94.1%), inlined AllocB(p,n,s)
// helpers with the values forwarded or literal (94.1% / immediate), a ctor
// H16(s, b = 0) with a default zero argument (immediate form), literal
// (0, "Package Data", 20) arguments (immediate), `sb.buf + (sb.size -
// sb.size)` and member-ref argument shapes (94.1% / 93.8%), dead statements
// `off = (int)sb.buf;` and `int dv = (int)sb.buf;` (fully deleted, 94.1%),
// the guide's live-range round-trip `char* zb = sb.buf; sb.buf = zb;` (94.1%),
// and `sb.size = 20;` restatement (immediate form). None reproduces the
// dead `mov eax, ecx` or the fresh `xor ecx, ecx` with the early store.
// Reading: the dead `mov eax, ecx` is register work, a live-range split of
// the opaque zero whose other consumer (the add of `+ 20`, folded to the
// constant 20 in the original?) is not reproducible by any spelling tested;
// compare the guide's dead-reload note for 0x453360.
// GPT-6.1-sol retry in #3194: seven checker invocations, best remains 98.3%; no MATCH. Reverse field assignments, void* typing for extra and HapiBuf buffer, and a declaration-order variant did not improve it. The allocator zero-init and store order still differ; see earlier detailed notes.
//
// deepseek-v4.1-flash session: still 98.3%, same single difference
// (0x4bd17b..0x4bd198). New shapes tried this session, all scored with
// check.py: plain separate assignments `sb.size = 20; sb.buf = 0;` and the
// reverse (92.6%, MSVC folds both to immediates); `struct HapiBuf sb = {0};`
// placed AFTER the if (92.4% to 97.1%) so the dead size-zero store is not
// hoisted past the je; aggregate plus a computed first argument
// `(char*)(key & 0)`, `(key ^ key)`, `(key - key)` (all 93.8%, early buf
// store via ebp but no dead `mov eax, ecx`); passing `*(char**)&sb.buf`,
// `*pp` or `pb->buf` (94.1%); a local `int`/`unsigned`/pointer zero (98.3%,
// CSEs into ebp); `memset(&sb.buf, 0, 4)` (93.8%); `#include <windows.h>`
// (98.3%, no change); and a `static HapiBuf MakeB()` helper returned by value.
// The helper result is the first shape that gives the original's FRESH
// `xor ecx, ecx` zero (no ebp CSE) but both member stores still sink below
// the three argument pushes and the dead `mov eax, ecx` is absent. The dead
// copy plus the early buf store look like a front-end value copy for a store
// that the next `mov eax, 0x14` makes dead, and no plain assignment, computed
// zero or by-value helper body tested reproduces it.
// deepseek-v4.1-flash new pass: a full tools/headers.py sweep of the 94.1%
// `struct HapiBuf sb = {20};`-alone shape tried all 128 header sets and none
// matches (best stays 94.1%), so the schedule is not a header tie. Also
// dumped: every computed/dead-zero spelling tried this pass (`off = (int)(sb.buf
// = 0)`, `sb.buf = (char*)(off = 0)`, `{20, 0}`, `*(__int64*)&sb = 0`, two
// immediate member stores, `memset(&sb,0,8)`, by-value factory
// `NullBuf().buf`, compound literal, self-assignments) lands at 92 to 98.3%
// and never produces the original's `xor ecx,ecx / mov eax,ecx` pair with an
// early buf store. The best file is unchanged at 98.3%.
// #2572 retry (deepseek-v4.1-flash): confirmed the same 98.3% wall. The
// literal-argument form hoists the buffer store but CSEs the zero into ebp
// (93.8%); the member-read form keeps a fresh xor ecx,ecx but sinks both
// stores (94.1%); force-reading sb.buf does not hoist; separate char*/unsigned
// locals collapse the frame to 0x50 (48.8%). No new shape beat 98.3%.
// Best remains 98.3% after an additional source variant; still differs only in
// the buffer initialization sequence at 0x4bd17b..0x4bd198, as detailed below.
// GPT-6.1-sol refinement (issue 2310): the existing 98.3% source remains best.
// Returning HapiBuf from an inline initializer scored 93.4%; copying the
// initialized sb.buf through a local scored 94.1%. The remaining mismatch is
// still the buffer setup at 0x4bd17b..0x4bd198.
//
// 98.3%: 580 of 580 bytes, and every byte from 0x4bd198 to the end is identical
// to the original. The whole remaining difference is the eight instructions of
// the buffer setup at 0x4bd17b..0x4bd198:
//
//   original                        ours
//   xor  ecx, ecx                   mov  eax, 0x14
//   mov  eax, ecx                   mov  [sb.buf], ebp      (early)
//   mov  [sb.buf], ecx              push eax / push str / push ebp
//   mov  eax, 0x14                  mov  [sb.size], eax
//   push eax / push str / push ecx  mov  [sb.buf], ebp      (extra, dead)
//   mov  [sb.size], eax             call
//   call
//
// Three linked things remain, and by the "one upstream cause" lesson they are
// one problem: the original rematerialises the buffer's 0 into a FRESH ecx
// after the `if (cb) cb(0)` merge, where ours keeps reusing the ebp zero that
// `extra = 0` already holds; the original carries a dead `mov eax, ecx`, the
// fingerprint of that 0 being materialised in eax for a store; and its buffer
// store is scheduled before the size is materialised, where ours lands after.
// Fixing the fresh-zero register should fix all three.
//
// The source as it stands is `struct HapiBuf sb = {20}; sb.buf = 0;`. The
// aggregate gives the register-materialised size (`mov eax,0x14`, shared by the
// push and the store, which the original has), and the explicit `sb.buf = 0`
// is the only early buffer store; the aggregate's own zero-fill of `sb.buf` is
// the extra dead store at the end.
//
// Everything tried this session, scored free with `check.py --sym`:
//   98.3%  this file, and byte-identical variants using a local `char* b = 0`,
//          an inline `char* Zero()`, `(char*)0`, or a literal 0 argument.
//   96.7%  the previous file (kept below in history): `struct HapiBuf sb = {0}`
//          before the if, then `sb.size = 20; sb.buf = FUN(sb.buf, ...)`; that
//          keeps a dead `sb.size = 0` store before the je.
//   94.1%  `sb = {20}` alone: the buffer store sinks past the pushes.
//   92.6%  `struct HapiBuf sb; sb.buf = 0; sb.size = 20;`, `sb = {0}` after the
//          if, `sb = {20,0}`, every constructor shape (1-arg size, 2-arg, both
//          member-init orders), an inline `Zero()`, an inline `sb.Zero()`,
//          `sb.buf = sb.buf`, and separate `size`/`buf` locals: all compile to
//          576 bytes with both stores grouped after the argument pushes.
//   48.8%  separate `unsigned size; char* buf;` locals (frame drops to 0x50).
// Not tried: any construct that gives the buffer's zero a node distinct from
// `extra = 0` (an inlined helper returning it, a by-value struct copy, ...).
//
// deepseek-v4.1 added: `struct HapiBuf sb = {20};` ALONE is the construct that
// produces the original's fresh `xor ecx,ecx` (the explicit `sb.buf = 0;`
// statement always CSEs the zero into ebp, the register that already holds
// `extra = 0`, which is what leaves the extra store in this file). Its codegen,
// verified with objdump on the object, is
//     mov eax,0x14 / xor ecx,ecx / push eax / push str / push ecx
//     mov [esp+0x20],eax / mov [esp+0x24],ecx / call        (94.1%)
// i.e. the right values in the right registers but BOTH member stores sunk
// below the three argument pushes, where the original stores buf=0 (0x18)
// before the pushes and sinks only the size=20 store. So the remaining problem
// is store scheduling, not the constant or its register.
// Also measured this session (each compiled and objdumped):
//   98.3%  `struct HapiBuf sb = {20}; sb.buf = (char*)0;` and
//          `{20}; char* z = 0; sb.buf = z;` (same as the file: ebp, two stores)
//   97.1%  `{0}; sb.buf = 0; sb.size = 20;` (three zero stores, one early)
//   94.1%  `{20}; struct HapiBuf t = {20}; struct HapiBuf sb = t;` (copy)
//   92.6%  `{20, 0}`, `{20, (char*)0}`, `{0}; sb.size = 20;`, and
//          plain assignments (`sb.size = 0; sb.buf = 0; sb.size = 20;`), which
//          all fold the constants into immediates (mov [mem],0x14) instead of
//          the original's `mov eax,0x14` shared by the push and the store.
// A micro file (same shapes, stores kept alive by an escape after the call)
// shows `{20}` always emits eax=20 first and both stores last, and that an
// array initialiser `unsigned sb[2] = {20};` gives the identical shape, so the
// dead `mov eax,ecx` is not explained by the array form either.
//
// deepseek-v4.1, second session (each variant compiled with tools/wcl /O2 /Ob2
// /MT and objdumped, scoring 576 bytes / under 98.3% unless noted):
//   98.3%  unchanged file (the best): `{20}` + explicit `sb.buf = 0;` gives the
//          early buf store but through ebp, and keeps the aggregate's second
//          buf store after the pushes.
//   576    `{20}; sb.buf = FUN(sb.buf, ...)` (no explicit zero): arg forwarding
//          uses the aggregate's fresh ecx zero for `push ecx`, but BOTH stores
//          stay after the pushes (mov [esp+0x20],eax / mov [esp+0x24],ecx).
//   576    `{20}; sb.buf = FUN((char*)(sb.size - sb.size), ...)`: the member
//          reference in the argument HOISTS the buf store before the pushes and
//          drops the second buf store (exactly the original's schedule), but the
//          zero folds into ebp and the dead `mov eax,ecx` pair is absent. Same
//          for `(char*)(unsigned int)(sb.size - sb.size)` and `(char*)q` copies.
//   576    `{20, 0}`, `{20, (char*)0}`, ctor `HapiBuf() { size = 20; buf = 0; }`
//          and `HapiBuf() : size(20), buf(0) {}`: all fold to immediate stores.
//   580    `{20}; sb.buf = (char*)(sb.size - 20);` inserts a `lea ecx,[eax-0x14]`
//          and a late pointer store, so a computed zero in the statement is not
//          the original's shape either.
// Conclusion: the original's early `mov [esp+0x18],ecx` plus dead `mov eax,ecx`
// is a front end value copy (a zero node distinct from the `extra = 0` ebp
// zero) that no plain assignment, aggregate, ctor or computed-zero spelling
// tested reproduces. Everything from 0x4bd198 to the end is already identical.
//
// deepseek-v4.1 (second session, 20 more shapes dumped instruction by
// instruction) confirms the diagnosis and narrows it: the 94.1% `{20}` shape is
// the ONLY construct that materialises the buffer's zero fresh (`xor ecx,ecx`)
// instead of folding it into the live ebp zero, and it always sinks BOTH member
// stores below the argument pushes. Every attempt to flush that zero early
// (an explicit `sb.buf = 0`, a zeroed local declared before or after the if,
// inlined helpers that store-and-return, `SetBuf(&sb,0)`, `memset`, reading the
// aggregate member into a temporary, assignment-expression arguments such as
// `FUN(sb.buf = 0, ..., sb.size = 20)`, constructor and struct-returning
// initialisers, computed-zero right-hand sides like `sb.size - sb.size`,
// `key ^ key`, `i = 0`) either re-CSEs the zero into ebp (the 98.3% here, two
// stores) or folds the size into an immediate store (576 bytes). Writing the
// aggregate before the `if` makes the front end emit both stores before the je
// with an immediate size, so the original's early flush is not a statement
// order effect either. The `mov eax,ecx` plus early flush look like a front end
// value-copy (an inlined call result materialised for a store) that no plain
// assignment spelling reproduces.
//
// deepseek-v4.1 tried (all scored with check.py against scratch copies):
//   92.6%  `char* buf = 0; unsigned size = 0; size = 20;` then an
//          uninitialised `struct HapiBuf sb;` assigned from both: the frame
//          drops to 0x50 (576 bytes) and everything shifts, so separate
//          locals cannot carry the original frame; MSVC also folds both
//          stores to immediates and keeps the ebp zero.
//   97.1%  `buf`/`size` locals plus `struct HapiBuf sb = {20};` and
//          `sb.size = size;`: frame is right but the size push becomes
//          `push 0x14` and both member stores land after the pushes.
//   97.4%  zero produced through a union member (`union { unsigned s;
//          char* p; } u; u.s = 0; sb.buf = u.p;`): same 98.3% layout but
//          the union gets its own slot, so one more instruction.
//   98.3%  `*(char**)&sb.buf = 0;`, identical to the explicit assignment.
// Conclusion: the fresh `xor ecx,ecx` is a property of the aggregate's own
// zero-fill (the `{20}` alone produces it), while any explicit `= 0`
// statement CSEs into ebp; the dead `mov eax,ecx` looks like a value copy
// from a second target that the optimizer dropped, which no plain
// assignment spelling reproduces.
//
// Frame layout (21 dwords, do not disturb): X+0x00 extra, X+0x04 sb.size,
// X+0x08 sb.buf, X+0x0c year[8], X+0x14 copyright[0x40].
#include <stdio.h>
#include <string.h>
#include <time.h>

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(int* p);
int __stdcall FUN_004bd3b0(char* path, unsigned int* size, char** extra);
int __stdcall FUN_004bd830(char* path, void* buf, int off, FILE* f,
                           void (__cdecl* cb)(int), char* extra, int key, int flags);

struct HapiBuf {
    unsigned int size;
    char* buf;
};

struct Hapi_004bd160 {
    char magic[4];
    char a4;
    char a5;
    char a6;
    char a7;
    unsigned int size;
    unsigned char key;
    char ad;
    unsigned short ae;
    int extra;
};

// FUNCTION: 0x4bd160
int __stdcall FUN_004bd160(char* srcname, char* dstname, void (__cdecl* cb)(int),
                           unsigned int key, int flags)
{
    char* extra = 0;
    char year[8];
    char copyright[0x40];
    int off;
    FILE* f;
    int i, n;
    unsigned char* p;

    if (cb)
        cb(0);
    struct HapiBuf sb = {20};
    char* z = sb.buf;
    sb.size = 20u & ~(unsigned)(size_t)z;
    sb.buf = (char*)FUN_004d84a0(sb.buf, "Package Data", sb.size);
    off = FUN_004bd3b0(srcname, &sb.size, &extra);

    {
        struct Hapi_004bd160* h = (struct Hapi_004bd160*)sb.buf;
        unsigned int m;
        unsigned int t;
        unsigned char k;
        strncpy(sb.buf, "HAPI", 4);
        h->a4 = 0;
        h->a5 = 0;
        h->a6 = 1;
        h->a7 = 0;
        h->size = sb.size;
        m = key & 0xff;
        if ((unsigned char)key == 0)
            k = 0;
        else {
            t = (m >> 2) | (m << 6);
            k = ~t;
        }
        h->key = k;
        h->ad = 0;
        h->ae = 0;
        h->extra = off;
    }

    f = fopen(dstname, "wb");
    if (!f) {
        if (sb.buf)
            FUN_004d85a0((int*)sb.buf);
        return 0;
    }
    fwrite(sb.buf, sb.size, 1, f);
    if (cb)
        cb(5);
    FUN_004bd830(srcname, sb.buf, off, f, cb, extra, key, flags);
    if (cb)
        cb(0x5f);
    n = (int)sb.size - 20;
    p = (unsigned char*)sb.buf + 20;
    if ((unsigned char)key) {
        for (i = 0; i < n; i++)
            p[i] = (char)~((unsigned char)(i + 20) ^ (unsigned char)key ^ p[i]);
    }
    rewind(f);
    fwrite(sb.buf, sb.size, 1, f);
    {
    time_t now;
    struct tm* t;
    now = time(0);
    t = localtime(&now);
    sprintf(year, "%i", t->tm_year + 1900);
    strcpy(copyright, "Copyright 0000 Cavedog Entertainment");
    strncpy(strstr(copyright, "0000"), year, 4);
    fseek(f, 0, SEEK_END);
    fprintf(f, copyright);
    fclose(f);
    if (sb.buf)
        FUN_004d85a0((int*)sb.buf);
    }
    return 1;
}
