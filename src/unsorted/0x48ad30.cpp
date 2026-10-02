// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by longcat-2.5-preview-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free (#4163): 84.5% to 97.5%, 849 bytes both ways, and the unit
// body is now instruction for instruction. Two changes, both in the outer loop.
//
// 1. The value returning helper went away. PlayerOk(i, off) returned a Player*
//    and the body was if (p) { ... }, so every failed test became an if
//    converted select (xor edi,edi / jmp) and the entry guard came out
//    jb <body>. Spelling the three rejects as a flat short circuit chain with
//    continue inside for (; i < 10; i++, off += 0x14b), with PlayerMore(i)
//    (a static inline in the body that tests the counter, so the trip count
//    pass gives up) reproducing the original's xor al,al / cmp al,0xa / jae
//    <latch> outright, costs nothing: before this change 94.1% of the diff lines
//    already agreed and the only thing the old shape was buying was the guard.
// 2. off has to be memory resident, and that is the whole 9 bytes. Promoted
//    into edi it compiles to xor edi,edi / mov [esp+0x18],edi / mov [ebx],edi,
//    10 bytes shorter than the original's mov dword ptr [esp+0x14],0 /
//    mov dword ptr [ebx],0, and the head becomes mov eax,[edi+ecx+0x1b63]
//    instead of mov edx,[esp+0x14] / mov eax,[ecx+edx+0x1b63]. MSVC 5 only
//    stops promoting a local whose address escapes, and the address has to
//    escape for real: an int& or int* parameter of an inlined helper,
//    int& r = off, *(&off), a union { int off; int raw; }, an int offv[1], a
//    packed {unsigned char i; int off;} local passed by reference and a by
//    value parameter of an __inline helper are ALL folded away, each measured,
//    and each gives a head byte identical to the promoted form. What does work
//    is the escape at a site the optimiser deletes:
//
//        if (0) { g_leak = &off; }
//
//    The front end has already marked off address exposed, the dead store
//    emits nothing, and off stays in its frame slot for the whole function.
//    That one line is 84.5% to 97.0% on its own, and it also puts cnt and off in
//    the original's slots (i 0x13, off 0x14, cnt 0x18) and turns both zero
//    stores back into immediates. while (0) {...}, for (; 0;) {...},
//    switch (0) { case 0: ... }, a for (int k = 0; k < 0; k++) and a function
//    local static all behave the same way. A LIVE escape costs
//    lea ecx,[esp+8] / mov [g_leak],ecx and the function comes out at 858.
//
// STILL DIFFERENT (97.5%, all register allocation and scheduling, no semantics
// left): the off = 0 store is emitted before cmp al,0xa where the original
// emits it after the *cnt = 0 store and the byte store for i (2 lines); at the
// loop head the two loads land in the opposite registers,
// mov ecx,[esp+0x14] / mov edx,[g_game] against the original's
// mov ecx,[g_game] / mov edx,[esp+0x14], same sizes (2 lines); and the
// mov ebx,[esp+0x18] reload of the counter pointer sits just after the
// FUN_0041bd10 call where the original puts it after the last call of the block,
// FUN_0048a870, which also shifts the two je/jne targets that follow it
// (4 lines). None of the three moved with any spelling of the loop head (p
// and its f0 == 0 test as one expression, as a raw cast expression, through
// static inline PlayerAt(int*), through a char* q base, or the two uses of the
// address expression spelled out separately), with any of the six orders of
// cnt / i / off, with off += 0x14b in either clause order, with the counter
// pointer or the byte counter escaped as well (96.6% and 847 bytes), or with
// #include <windows.h>; all measured. Two orderings do move the store block,
// which is why the initialisers are separate statements here: putting
// *cnt = 0 before the i and off declarations, or off = 0 after it, gives the
// escape's effect back up and drops to 74.6%.
//
// MEASURED AND WORSE OR NO CHANGE THIS ROUND, so the next pass does not repeat
// them. Loop head: p with its f0 == 0 test as one expression, as a raw cast
// expression on the address, through static inline PlayerAt(int*), through a
// char* q base local, through two locals (int o = off / char* g), with the
// address expression written twice, commuted, or parenthesised: all 97.5% or
// 96.6%, none moves the two loads into ecx/edx. Slot(i, off) returning the
// player pointer, and PlayerMore(i, off) taking the offset by value: 78.4% and
// 78.7%, both 851 bytes, the select comes back. Escapes that are folded away:
// PlayerAt(int*)/PlayerMore(.., int&) with &off, PlayerMore(i, int&) by value,
// int& r = off, *(&off), a union of two ints, int offv[1], a packed
// {unsigned char i; int off;} local by reference and by value: every one leaves
// the head byte identical to the promoted form. Escaping the byte counter as
// well: 96.6%, 847 bytes. Escaping the counter pointer as well: 74.6%, 847
// bytes. The u->def block through a local, if (u->def), the b14 test without
// braces, and the two per unit calls before or after the def block: 97.5%,
// 89.4% and 63.9% (the last two reorder the calls and cost 12 bytes). A live
// escape through a file scope static, a function local static, or straight into
// g_game->f14353: 858, 858 and 856 bytes. #include <windows.h>: 97.5%, no
// change. All six orders of cnt / i / off with the escape in place: 97.0% or
// 97.5%, two of them 850 bytes. The declaration order that puts cnt in 0x18 and
// off in 0x14 is what the escape buys; it cannot be had without it.
//
// Two permuter runs over this version, 2016 and then 3254 candidates with
// different seeds, found nothing above 97.5% and nothing that moved any of the
// three residuals above, so treat 97.5% as a local optimum for rewrites of
// this source rather than as a lead worth chasing further.// deepseek-v4.1-flash (#3932) retry: reversing the do-while latch to
// `off += 0x14b; i++;` is byte-flat at 84.5% / 849 bytes, so the latch register
// roles (al/edx against our cl/eax) are not steered by increment order here.
// deepseek-v4.1-flash (#3834) retry: the SHARED.md slot-order idea was re-tested
// inside this 84.5 percent helper/select form: `unsigned char i = 0; int off = 0;
// int* cnt = &g_game->f14353; *cnt = 0;` does give the original homes (i 0x13,
// off 0x14, cnt 0x18) but the build grows to 850 bytes and drops to 68.9 percent,
// so the loop-rotation loss outweighs the slot win here too and this order stays.
// deepseek-v4.1-flash (#3790) retry: declaring `unsigned char i = 0; int off = 0;
// int* cnt = &g_game->f14353; *cnt = 0;` DOES reach the original slot layout
// i 0x13 / off 0x14 / cnt 0x18, but the zero stores stay forwarded into registers
// (mov [esp+0x18], eax; mov [ebx], eax) and the guard stays `jb` + select at
// 850 bytes / 68.9 percent; cnt first with *cnt = 0 right after is 853 / 69.7.
// The 84.5 percent order (cnt, i, off decls, *cnt = 0 last) stays.
// deepseek-v4.1-flash (#3754) retry: a do-while with an explicit top `if (i >= 10) break;`
// plus break-style rejects and no helper reproduces the documented 74.3% / 831 bytes,
// so the break reading still cannot beat the 84.5% helper/select form, which stays.
// deepseek-v4.1-flash (#3684) retry: confirmed from the disassembly that the
// three rejects in the head (`je 0x48b008` for f0==0, kind not 1/2/3, f146==0xa)
// jump to the code AFTER the loop, i.e. they are BREAKS, not skips, and that
// the body has no i>=10 test. Writing the loop as
// `while (i < 10) { ... if (p->f0 == 0) break; k = p->f73; if (k!=1&&k!=2&&k!=3)
// break; if (p->f146 == 0xa) break; ... i++; off += 0x14b; }` with the checks
// inline (no value-returning helper) reproduces the original head
// instruction-for-instruction (mov eax,[ecx+edx+0x1b63] / lea edi,[...] /
// test / je / mov al,[edi+0x73] / cmp chain / cmp [edi+0x146],0xa / je), so the
// break reading is right, but it costs the top-of-loop test and the loop
// rotation and lets off live in edi: 74.3% / 831 bytes (also 70.4% / 839 with
// the old PlayerOk helper calls kept). The do-while `if (p)` form below keeps
// 84.5% / 849 bytes and stays.
// deepseek-v4.1-flash (#3453) retry: 84.5% unchanged (849 bytes). Tried the
// init order `unsigned char i = 0; int* cnt = ...; *cnt = 0; int off = 0;`
// (store between the two decls): it does give the original's immediate
// `mov dword ptr [ebx], 0` for *cnt, but the guard stays `jb` + select, the
// frame slots stay i 0x13 / cnt 0x14 / off 0x18 (original i 0x13 / off 0x14 /
// cnt 0x18), and the head grows 4 bytes to 853 (69.7%). Also tried
// `int off = 0; unsigned char i = 0;` (cnt 0x14 / off 0x18 unchanged, 69.7%).
// So the immediate *cnt store is reachable but only in the order that costs
// the slot layout; the two cannot be had together from the initialisers.
// GPT-6.1-sol retry: moved the eliminated-player check before the kind chain and tried unsigned-range and positive-conjunction spellings; scores fell to 68.2-70.1%, so the prior 84.5% version is retained. Remaining differences are helper/select and prologue register/slot allocation, plus cnt reload placement.
// deepseek-v4.1-flash (#2567) retry: 84.5% (849 bytes, size matches). The one
// real change this round: the owner test in the flag bit 4 block was WRONG in
// the 84.1 version. It read `u->owner == 0 || !(u->owner->f110.bits.b30)` and
// so cleared bit 4 when the unit had no owner, but the original's
// `mov eax,[esi+0x86] / test eax,eax / je 0x48ae5f` SKIPS the clear for a null
// owner. Corrected to `(u->owner != 0 && !(u->owner->f110.bits.b30))`, which
// matches Ghidra's `owner != 0 && (owner->f110 & 0x40000000) == 0`: 84.1 -> 84.5.
// Still differing (all register allocation, no semantics left): head guard is
// `jb <body>` + a `xor edi,edi / jmp` select where the original has
// `xor al,al / cmp al,0xa / jae <latch>`; i lands in cl not al, off in eax not
// edx; cnt/off home slots are swapped (ours cnt 0x14 / off 0x18, original
// off 0x14 / cnt 0x18) and the tail reloads cnt from 0x14 at a different point.
// Tried this round and worse: declaring off before cnt (67.9%, 861 bytes), the
// source order i/off/cnt (68.9%, 850), and cnt/*cnt/i/off (69.7%, 853); slot
// assignment did not move to the original in any of them.
// deepseek-v4.1 (#1208) second retry: 84.1% kept (849 bytes, size matches).
// Re-verified the v22 lead (build/scratch/0x48ad30/v22.cpp): its head emits
// `push edi` before `xor al,al` and shares one zero register for both
// `*cnt = 0` and `off = 0` (`xor edi,edi / mov [esp+0x18],edi / mov [ebx],edi`)
// where the original stores immediates and pushes edi after `cmp al,0xa`.
// That zero is then forwarded into the body as edi=off, so the original's
// `mov edx,[esp+0x14]` reload never appears: 9 bytes short (840 vs 849).
// Tried this run, all 840-851 bytes and 75.1% or worse: for-init initialisers
// with the declarations split out (t2/t3: the *cnt store does become an
// immediate, `mov [ebx],0`, but off's zero still lands in edi and the slot
// order stays cnt 0x14 / off 0x18), `int off` declared before `int* cnt` in
// every order (t1/t4: slots do not move, though the entry g_game load moves
// to ecx), the player test written twice as a raw cast expression or via a
// fresh local (t5/t6: unchanged), a second inlined helper
// `PlayerAt(int& off)` returning p (t8: unchanged), and using
// `g_game->f14353` directly with no cnt local (t7: 851 bytes, 60.9%, g_game
// gets reloaded for the counter).
// Status: the head differs from the original in exactly these bytes: guard
// `jb <body>` plus `xor edi,edi / jmp` select instead of `jae <latch>` and a
// flat `je <latch>` test chain; i in cl not al; off in eax not edx; cnt at
// [esp+0x14] and off at [esp+0x18] instead of the reverse.
// deepseek-v4.1 (#1208) retry: still 84.1% (849 bytes, equal size). New confirmed
// lead, much closer than this file, saved as build/scratch/0x48ad30/v22.cpp
// (840 bytes, 75.1% only because every body jump target shifts by 9 bytes):
// dropping the value-returning helper and writing the player test as a FLAT
// short-circuit chain with `continue`s in a for loop gives the original's entry
// guard outright, `xor al,al / cmp al,0xa / jae <latch>`, with the counter in al
// (the byte-local register the original uses) and no if-converted null-select:
//
//     static inline int PlayerMore(unsigned char i) { if (i >= 10) return 0; return 1; }
//     for (; i < 10; i++, off += 0x14b) {
//         if (!PlayerMore(i)) continue;
//         Player_0048ad30* p = (Player_0048ad30*)((char*)&g_game->players[0] + off);
//         if (p->f0 == 0) continue;
//         unsigned char k = p->f73;
//         if (k != 1 && k != 2 && k != 3) continue;
//         if (p->f146 == 0xa) continue;
//         ...body unchanged...
//     }
//
// The helper must exist and must test i (that is what keeps the counter live in
// the body and blocks the trip count pass; a plain for loop still becomes a down
// counter). Written this way the whole player-test region is branch-for-branch
// the original's, the guard lands on the latch, and only two things are left,
// both in the prologue:
//   1. off: the original reloads it (`mov edx,[esp+0x14]` then
//      `mov eax,[ecx+edx+0x1b63]` / `lea edi,[ecx+edx+0x1b63]`), our build
//      forwards the known zero and emits `mov eax,[edi+ecx+0x1b63]` /
//      `lea edi,[edi+ecx+0x1b63]` with edi = 0, 8 bytes shorter, plus it shares
//      the zero with `*cnt = 0` (`xor edi,edi`, `mov [ebx],edi`,
//      `mov [esp+0x18],edi`) where the original stores immediates
//      (`mov [ebx],0`, `mov [esp+0x14],0`).
//   2. slots: our cnt pointer gets [esp+0x14] and off [esp+0x18]; the original
//      has off at [esp+0x14] and cnt at [esp+0x18]. Permuting the three
//      declarations (all six orders tried) does not move them; the only order
//      that changes anything is `int* cnt; *cnt = 0; unsigned char i; int off;`
//      (build/scratch/0x48ad30/v24.cpp, 844 bytes) which makes `*cnt = 0` an
//      immediate store but leaves off in a register and the slots swapped.
//   3. the tail `mov ebx,[esp+0x18]` (cnt reload) happens after the last two
//      calls in the original and before the FUN_0043b7c0 call in ours.
//
// The 84.1% version below keeps the old value-returning helper: it if-converts
// the failed tests into `xor edi,edi / jmp` and comes out `jb` + select instead
// of `jae`, but it does keep off address-taken (memory, reloaded) and is the
// highest scoring version so far, so it stays in the file.
// Also tried and worse this run: `*cnt = 0` before the declarations (69.7%, 853),
// `int off` declared before `int* cnt` (69.7%, 854/844, slots unchanged),
// helper as `PlayerMore(i, off)` with the reference dropped by the inliner
// (identical 75.1%), the same for loop with the value-returning helper (84.1%,
// identical bytes to the do-while below).
// What still differs in this file: the guard `jb <body>` plus the
// `xor edi,edi / jmp` select against the original's `jae <latch>` (which v22
// fixes), i in cl against al, off in eax against edx, and the cnt/off slots.
// LongCat 2.5 (#1208): 84.1%, 849 bytes (size matches). The guard and init stores now
// appear: a `static inline PlayerOk(unsigned char i, int& off)` helper in the body tests
// the loop counter (blocking the trip count pass, so the guard survives) and returns the
// player pointer, which keeps `off` address-taken (in memory, no strength reduction).
// The unit body matches instruction for instruction. What still differs is register
// allocation in the prologue/player-tests/latch: the type byte lands in cl not al, `off` in
// eax not edx, the player address in one `lea` not load+lea, and the helper's last early
// return (`p->f146 == 0xa`) is if-converted to a branchless select (neg/sbb/and) instead
// of the original's `cmp byte ptr [edi+0x146],0xa / je`. Tried and worse: minimal helper
// (72.3%, no guard + strength reduction), nested ifs (69.9%), bool return + out-param
// (79.7%), f146 in body (68.9%), reordered conditions (68.2%), inline player tests
// (74.9%), struct state (83.3%), headers.py (no change).
// Sonnet 5.5 retry (#1091): 77.6% (was 74.2%). The +3.4 came from spelling the progress swap
// `u->ff7 = u->ff6; u->ff6 = v;` (the original loads dl, stores ff6, then ff7). Lead for the loop
// head: the original guard `xor al,al / cmp al,0xa / jae <latch>` is an in-body `if (i < 10)` test
// on an initialised counter (`unsigned char i = 0; int off = 0;`, body wrapped in
// `if (i < 10) { p = ...; if (p->f0 != 0) {...} }`, do-while latch as below): that spelling gives
// the exact guard, init stores and `mov eax,[ecx+edx+0x1b63]; lea edi,...` head (845 bytes, 72.6%
// only because the rest shifts). What it still gets wrong: off is carried in edi across the back
// edge (`mov edi,[esp+0x18]` in the latch, `[edi+ecx+0x1b63]` at the top) where the original reloads
// it from [esp+0x14] into edx, and the cnt/off slots come out swapped (cnt 0x14, off 0x18).
// Not kept in this file because the uninitialised do-while below scores higher.
// The per-tick unit housekeeping loop (called from one place): clears the
// counter at g_game+0x14353, then for each of the ten 0x14b player records at
// g_game+0x1b63 walks the unit list (first +0x67, last +0x6b, stride 0x118),
// and for every unit that still has something to do (+0xa6) it ticks the
// animation counter, runs FUN_0049e1a0 for a human or computer player, stops
// the sound object, ages the two timers at +0xfa / +0xfb, clears flag bit 4
// when the unit has been unloaded, refreshes the two progress bytes +0xf6 and
// +0xf7 every 30 ticks, and then does the water damage, the shield/energy
// transfer every 8 ticks, the two per unit updates and the flag bit 14 effect.
// After the unit list it runs FUN_0048b710 when the global bit at +0x2a44 is
// set, and at the end it ages the 0x14371 counter when bit 1 of +0x14373 is
// set and FUN_004c1b80(0xf9) says no.
//
// NOT MATCHED: 74.2%, 828 bytes against 849. Everything except the outer
// loop's shape and a handful of register choices matches instruction for
// instruction. What is missing is exactly the loop's entry guard and the two
// induction variable initialisations:
//
//     xor al, al / cmp al, 0xa          <- the loop's entry guard
//     mov dword ptr [esp + 0x18], ebx    <- the counter pointer's home slot
//     mov dword ptr [ebx], 0
//     mov byte ptr [esp + 0x13], al      <- i = 0
//     mov dword ptr [esp + 0x14], 0      <- off = 0
//     jae 0x48b008                       <- into the latch, not past the loop
//
// so the original is a rotated for/while loop whose entry guard MSVC 5 kept
// (dead: the guard is never false because i starts at 0) and whose latch is
// the ordinary up-counting one, `load / load / inc al / add edx,0x14b /
// cmp al,0xa / store / store / jb`. The do-while below drops the two
// initialiser stores, which is why the compiler warns C4700 on `i` and `off`
// here: the stores only come back with the guard.
// Getting MSVC 5 to keep that guard while
// still fusing the increment into the latch is the whole remaining problem,
// and the trip count pass is what removes it: with a constant bound of 10 it
// rewrites the loop as a down counter in a fresh int register and drops the
// guard.
//
// THE MISSING TRICK, found and confirmed (this is the way in). A
// `static inline` helper in the loop body that TESTS THE LOOP COUNTER makes
// MSVC 5 hoist that test out of the body to the top of the loop as
// `xor al,al / cmp al,0xa / jae <the latch>`, and because the byte counter is
// then live inside the body the trip count pass gives up, so the guard
// survives and the latch stays the up-counting one. Five spellings were
// compiled and all five produce the guard plus a byte-for-byte identical
// latch, from a plain `do { } while (i < 10)` with no other change:
//
//     static inline int PlayerOk(unsigned char i, Player_0048ad30* p)
//     { if (i >= 10) return 0; ...tests on p...; return 1; }
//     do { Player* p = ...; if (PlayerOk(i, p)) { ...body... }
//          i++; off += 0x14b; } while (i < 10);
//
// (0x44fe40, the other MATCHed function in the exe with this guard, is the
// same recipe: its inlined `PlayerId(unsigned char i)` opens with
// `if (i == 10 ...) return -1;` and the guard in its code at 0x44fe52 is
// that test, hoisted.)
//
// What still blocks the match with that helper in place: with the loop now
// analysed, the front end also strength-reduces the player address into a
// register IV, so the body computes `lea edi, [edi + ecx + 0x1b63]` and keeps
// accumulating, where the original reloads the offset (`mov edx,
// [esp+0x14]`) and rebuilds `[ecx + edx + 0x1b63]` every iteration, and the
// guard comes out as `jb <body>` plus a dead `xor edi,edi / jmp` instead of
// `jae <latch>`. Best helper variant scored 70.9% (the address stays in
// memory there, but `if (helper(...))` gets if-converted into
// `xor edi,edi / jmp` select code all over the body). Moving the address
// expression into the helper, or into a second helper, does not stop the
// strength reduction. So the remaining job is one decision: keep the counter
// live in the body (for the guard) but keep the address affine use of `off`
// invisible to the induction variable pass.
//
// Ruled out for the guard, each verified with a scratch score:
// - `for (i = 0, off = 0; i < 10; i++, off += 0x14b)` and the same loop with
//   `off += 0x14b` as the last statement of the body: 69.5%, `mov dword ptr
//   [esp+0x18], 0xa` plus `dec eax` in the latch.
// - `while (i < 10) { ... off += 0x14b; i++; }`: 69.5%, same down counter.
// - `for (char i = 0; ...)`, `for (int i = 0; ...)`, and the body indexing
//   `g_game->players[i]` instead of walking a byte offset: 69.5%, 68.2% and
//   69.5%, all down counters. Note 0x406db0 and 0x456850 are MATCHed with
//   exactly `for (char i = 0; i < 10; i++)`: both have a `return` inside the
//   loop, and that early exit is what stops the trip count pass there. The
//   only two places in the whole exe with this loop's `xor al,al / cmp al,0xa`
//   guard are this function and 0x44fe40, which is also MATCHed and also has
//   an early `return i`. So the original almost certainly had an early exit
//   from this loop too, but its assembly has no edge from the body to the
//   epilogue other than through the latch, so it cannot be spelled in C++
//   without emitting that edge.
// - The flat body with `continue` for each of the three failed tests (the
//   latch is shared by all the exits in the original, which is what a
//   `continue` spells): 69.9%, still a down counter.
// - `i != 10`, `i <= 9`, `i = i + 1`, an `int off = 0` initialised before the
//   loop, an empty `for (; i < 10; )` third expression and `off` as the first
//   loop initialiser: every one of them is a down counter as well (checked by
//   compiling, not by scoring, since all of them lose the same 16 bytes).
// - A loop whose test is an inlined helper, `for (i = 0; More(i); i++)` with
//   `static inline int More(unsigned char i) { if (i >= 10) return 0; return
//   1; }`: this also blocks the trip count pass and keeps the byte counter,
//   but the loop is left unrotated, with a memory compare
//   (`cmp byte ptr [esp+0x13], 0xa; jae`) at the top and an unconditional
//   `inc bl; jmp` back edge, so it is not the original's shape. The helper
//   that works has to be in the BODY, testing the counter, not the condition.
// - `i = NextI(i)` with the increment in an inlined helper, `off =
//   NextOff(off)`, a flat body whose three failed tests are `goto next` to a
//   label at the end of the body, and the body indexing `&g_game->players[i]`:
//   all down counters, all losing the same 16 bytes.
// - `while (1) { if (i >= 10) break; ... }`, brief item 9's form: 70.8%, and
//   it does keep a guard, but the guard is a memory compare
//   (`cmp byte ptr [esp+0x13], 0xa`) instead of the original's register
//   compare on the value `xor al,al` has just produced.
// - The do-while that is in the file, `do { ... i++; off += 0x14b; } while
//   (i < 10);`: 74.2%, the best of these. Its latch is byte for byte the
//   original's, and swapping the two increment statements to `i++` first was
//   worth 0.5 points (71.3% to 71.8%), so the original increments the index
//   before the offset.
// - The local slot order follows from the same decision: the original homes
//   the counter pointer at +0x18 and the offset at +0x14, and the guard's
//   presence is what keeps the offset below it.
//
// Three things that were real bugs in earlier versions of this file, all
// worth points and all invisible in the pseudo-C: the unit's owner test in
// the flag bit 4 block is `!(owner->flags & 0x40000000)`, not
// `owner->flags & 0x40000000` (the original's `jne` skips the clear, so the
// clear needs the bit clear); `u->f110.bits.b4 != 0` rather than
// `u->f110.bits.b4` is what makes MSVC 5 emit the original's `test cl, 0x10`
// instead of a `shr eax, 4 / test al, 1` extraction; and loading the unit
// list's `last` (+0x6b) before its `first` (+0x67) is worth half a point
// (72.2% to 72.2% with the previous 71.8%, both scheduler tie-breaks).
//
// A fourth misread value, found this round and now in the file: the shield /
// energy transfer divides by 30, not by 15. The original's sequence is
// `imul ecx (0x88888889) / add edx,ecx / sar edx, 4 / mov eax,edx / shr
// eax,0x1f / add edx,eax` and `/ 15` gives `sar edx, 3` with the same magic
// (the magic is signed negative, so the shift is one less than for `/ 30`),
// so `(float)(n / 15)` produced a one byte difference. Worth 0.4 points
// (73.8% to 74.2%). The same `mov ax, [f200] / and eax,0xffff / shl eax,3`
// before it is what shows the dividend is `(unsigned short) * 8` widened to
// int, not a short.
//
// Also unmatched, and all downstream of the loop: the four register choices
// in the tail half (`mov eax` vs `mov edx` for the g_game reloads, `dx` vs
// `ax` for seaLevel, `dh` vs `ah` for the bit 12 test, `dl` vs `cl` for the
// progress byte swap) and the extra `mov ebx, [esp+0x18]` reload that the
// original only performs on the path through the player block. They are one
// decision: the original lets the type pointer die at the `div` and reloads
// `[esi+0x92]` for each later use, while this version keeps one copy alive
// in ecx across the progress byte swap, which is what costs it `dl` and
// pushes the following g_game load into edx. Tried and worse: the flag word
// at +0x110 as a plain int with mask tests (68.4%: the bit 14 test stops
// being a shift) and as a bitfield group starting at bit 0 instead of bit 4
// (71.2%).
#pragma pack(push, 1)

class Class_00435100 {
public:
    char unknown_0[0xd4c];
    int waterDoesDamage;               // +0xd4c
    int waterDamage;                   // +0xd50
};

class Class_0043dd20;
struct Player_0048ad30;

class Class_004b0d60 {
public:
    char unknown_0[8];
    void FUN_004b0d60(int n);
};

struct Type_0048ad30 {
    char unknown_0[0x1fa];
    unsigned int f1fa;                 // +0x1fa
    char unknown_1fe[0x200 - 0x1fe];
    unsigned short f200;               // +0x200
    char unknown_202[0x241 - 0x202];
    union F241_0048ad30 {
        struct {
            unsigned int low : 12;
            unsigned int floats : 1;   // bit 12
            unsigned int rest : 19;
        } bits;
        unsigned int all;
    } f241;                            // +0x241
};

union F110_0048ad30 {
    struct {
        unsigned int low : 4;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int mid : 8;
        unsigned int b14 : 1;
        unsigned int mid2 : 15;
        unsigned int b30 : 1;
        unsigned int top : 1;
    } bits;
    unsigned int all;
};

struct Unit_0048ad30 {
    Class_0043dd20* def;                // +0x00
    char unknown_4[0x70 - 4];
    short f70;                         // +0x70
    char unknown_72[0x86 - 0x72];
    Unit_0048ad30* owner;              // +0x86
    char unknown_8a[0x92 - 0x8a];
    Type_0048ad30* type;               // +0x92
    Player_0048ad30* player;           // +0x96
    Class_004b0d60* f9a;                // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    unsigned short fa6;                // +0xa6
    char unknown_a8[0xf5 - 0xa8];
    unsigned char ff5;                 // +0xf5
    unsigned char ff6;                 // +0xf6
    unsigned char ff7;                 // +0xf7
    char unknown_f8[0xfa - 0xf8];
    unsigned char ffa;                 // +0xfa
    int ffb;                           // +0xfb
    char unknown_ff[0x104 - 0xff];
    float f104;                        // +0x104
    short f108;                        // +0x108
    char unknown_10a[0x110 - 0x10a];
    F110_0048ad30 f110;                // +0x110
    char unknown_114[0x118 - 0x114];
};

class Class_0043dd20 {
public:
    char unknown_0[0x8a];
    void FUN_0043dd20(Unit_0048ad30* u);
};

struct Player_0048ad30 {
    int f0;                            // +0x00
    char unknown_4[0x67 - 4];
    Unit_0048ad30* f67;                // +0x67
    Unit_0048ad30* f6b;                // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char f73;                 // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f146;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_0048ad30 {
    char unknown_0[0x1b63];
    Player_0048ad30 players[10];       // +0x1b63
    char unknown_1c3f[0x2a44 - 0x1b63 - 10 * 0x14b];
    unsigned char f2a44;               // +0x2a44
    char unknown_2a45[0x1427f - 0x2a45];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14353 - 0x14280];
    int f14353;                        // +0x14353
    char unknown_14357[0x14371 - 0x14357];
    short f14371;                      // +0x14371
    union F14373_0048ad30 {
        struct {
            unsigned int b0 : 1;
            unsigned int b1 : 1;
            unsigned int rest : 30;
        } bits;
        unsigned int all;
    } f14373;                          // +0x14373
    char unknown_14377[0x38a47 - 0x14377];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Class_00435100* mode;              // +0x391e9
};
#pragma pack(pop)

extern Game_0048ad30* g_game;

void __stdcall FUN_00437910(Unit_0048ad30* u);
void __stdcall FUN_0049e1a0(Unit_0048ad30* u);
void __stdcall FUN_0043b7c0(Unit_0048ad30* u);
void __stdcall FUN_0043bad0(Unit_0048ad30* u);
void __stdcall FUN_0048a870(Unit_0048ad30* u);
void __stdcall FUN_004864b0(Unit_0048ad30* u, int n);
void __stdcall FUN_00489bb0(int a, Unit_0048ad30* u, int damage, int kind, int flag);
int __stdcall FUN_0041bd10(Unit_0048ad30* u, Unit_0048ad30* u2, float f);
void __stdcall FUN_0048b710(Player_0048ad30* p);
void __stdcall FUN_0048d790(void);
int __stdcall FUN_004c1b80(int n);
void __stdcall FUN_0041c2e0(int n);

// The original keeps its player offset in a frame slot for the whole function:
// both zero initialisers are immediate stores and the loop head reloads it, so
// MSVC 5 never promoted it to a register. Taking the offset's address is what
// reproduces that. Like the self-assignment on 0x450530, the store itself emits
// no code, but it is worth 22.4 points and so stays: with it this function is
// 97.5% / 849 bytes, without it 75.1% / 840 bytes. Every live spelling of the
// escape adds a lea plus a mov and lands the function at 858 bytes.
static int* g_leak;

static inline int PlayerMore(unsigned char i)
{
    if (i >= 10) return 0;
    return 1;
}

// deepseek-v4.1-flash (#4087, 10 min timebox): re-confirmed 84.5% / 849 bytes as
// the best for this helper/select form; the residual is still the prologue slot
// order (cnt 0x14 / off 0x18 against the original's off 0x14 / cnt 0x18) with the
// latch roles cl/eax instead of al/edx, and the *cnt reload placement at the tail.

// FUNCTION: 0x48ad30
void __stdcall FUN_0048ad30(void)
{
    int* cnt = &g_game->f14353;
    int off;
    // No code, but worth 22.4 points: with this line 97.5% / 849 bytes, without
    // it 75.1% / 840 bytes. See g_leak above.
    if (0) {
        g_leak = &off;
    }
    off = 0;
    *cnt = 0;
    unsigned char i = 0;
    for (; i < 10; i++, off += 0x14b) {
        if (!PlayerMore(i)) continue;
        Player_0048ad30* p = (Player_0048ad30*)((char*)&g_game->players[0] + off);
        if (p->f0 == 0) continue;
        unsigned char k = p->f73;
        if (k != 1 && k != 2 && k != 3) continue;
        if (p->f146 == 0xa) continue;
        {
            Unit_0048ad30* last = p->f6b;
            Unit_0048ad30* u = p->f67;
            while (u <= last) {
                    if (u->fa6 != 0) {
                        (*cnt)++;
                        FUN_00437910(u);
                        if (p->f0 != 0) {
                            unsigned char k2 = p->f73;
                            if (k2 == 1 || k2 == 2) {
                                FUN_0049e1a0(u);
                            }
                        }
                        if (u->f9a != 0) {
                            u->f9a->FUN_004b0d60(1);
                        }
                        if (u->ffa != 0) {
                            u->ffa--;
                        }
                        if (u->ffb != 0) {
                            u->ffb--;
                        }
                        if (u->f110.bits.b4 != 0) {
                            if (!(u->f110.bits.b5) || u->f104 != 0.0f || u->ffb != 0
                                || (u->owner != 0 && !(u->owner->f110.bits.b30))) {
                                u->f110.bits.b4 = 0;
                            }
                        }
                        if (g_game->ticks % 30 == 0) {
                            int v = u->f108 * 100 / u->type->f1fa;
                            if (v < 0) {
                                v = 0;
                            }
                            if (v > 100) {
                                v = 100;
                            }
                            u->ff7 = u->ff6;
                            u->ff6 = v;
                        }
                        Player_0048ad30* pl = u->player;
                        if (pl->f0 != 0) {
                            unsigned char k3 = pl->f73;
                            if (k3 == 1 || k3 == 2) {
                                if (g_game->mode->waterDoesDamage != 0
                                    && g_game->mode->waterDamage != 0
                                    && g_game->ticks % 30 == 0 && u->f70 <= g_game->seaLevel
                                    && !u->type->f241.bits.floats) {
                                    FUN_00489bb0(0, u, g_game->mode->waterDamage, 0xb, 0);
                                }
                                if (u->type->f200 != 0 && u->f108 < u->type->f1fa
                                    && (g_game->ticks & 7) == 0) {
                                    int n = u->type->f200 * 8;
                                    FUN_0041bd10(u, u, (float)(n / 30));
                                }
                            }
                            FUN_0043b7c0(u);
                            FUN_0043bad0(u);
                            if (u->def != 0) {
                                u->def->FUN_0043dd20(u);
                                FUN_0048a870(u);
                            }
                        }
                        if (u->f110.bits.b14) {
                            FUN_004864b0(u, u->ff5);
                        }
                    }
                    u = (Unit_0048ad30*)((char*)u + 0x118);
                }
                if (g_game->f2a44 & 1) {
                    if (p->f0 != 0) {
                        unsigned char k4 = p->f73;
                        if (k4 == 1 || k4 == 2) {
                            FUN_0048b710(p);
                        }
                    }
                }
            }
        }
    if (g_game->f14373.bits.b1) {
        if (!FUN_004c1b80(0xf9)) {
            g_game->f14371--;
            if (g_game->f14371 <= 0) {
                g_game->f14371 = 0x5a;
                FUN_0048d790();
                FUN_0041c2e0(0);
            }
        }
    }
}
