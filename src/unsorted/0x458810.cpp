// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by Space Bunny Free, finished by Fledge Alpha Free, finished by GPT-6. Names are provisional.
// GPT-6 retry (#4982): best remains 97.2% at 427 bytes. /Gi keeps the same
// block order; frame-first and goto variants both recolor and score 73.8%.
// GPT-6 retry recheck: an inline AtFirstFrame(list) predicate is byte-identical
// at 97.2%; putting the frame test before the bitmap-null test reproduces the
// known 73.8% register recoloring. Keep the current source order.
//
// EIGHTH PASS (Fledge Alpha Free, issue 4848). 427 of 427 bytes, 97.2%, up
// from 88.9%: the doubled `test eax,eax` at 0x4588bd is back and it is in the
// right registers. The ONLY thing left is the ORDER of two blocks.
//
// THE DOUBLED TEST. Add `Bitmap_458810* bitmap = list->bitmap;` right before
// the `if ((owner->flags & 0x20000000) != 0)` and spell the LAST field_14
// conjunct through it (`bitmap->field_14 == 0`; the first stays
// `list->bitmap->field_14 == 0`). The frontend cannot fold two differently
// rooted member accesses, the backend still CSEs the load, and both
// `test eax,eax / jne` pairs appear. This is the #2233 spelling, unchanged.
//
// THE ORDER LEVER, which is new. With the local present the frame test and the
// bitmap test are emitted in SOURCE order, and that order decides the whole
// colouring:
//   * bitmap test first, frame test second (THIS FILE): correct colours
//     (ebx=bitmap, edx=zero, `sete bl`, flags in edx), 427 bytes, and the
//     whole tail matches. The only diff is that the emitted frame-test block
//     and bitmap-test block are swapped relative to the original (6 moved
//     instructions plus their jump targets; 14 diff lines, 97.2%).
//   * frame test first (the seventh pass's order): the allocator mirrors
//     ebx/edx (zero in ebx, bitmap in edx, flags in ebx, and the visible else
//     merges into one store), 425 bytes, 73.8%. The seventh pass's 88.9% base
//     cannot carry the doubled test for this reason.
// Diagnostic: with the local present but BOTH field tests spelled through it
// (they fold into one test), the base is emitted byte for byte (423/88.9), so
// it is the surviving second test, not the local, that flips the allocator.
//
// THE FRAME-FIRST FAMILY, all measured and negative. Everything below keeps
// the frame test first and the doubled test alive:
//   * local declared before the frame test: correct colours, but the bitmap
//     load is hoisted above the frame compare and the frame test becomes
//     `mov eax,[edi+4]; cmp eax,edx` (429 bytes, 85.9%, shape 98.3%).
//   * field reference `Bitmap_458810*& bitmap = list->bitmap;` (or a
//     `Bitmap_458810**` to the field): correct colours and the doubled test,
//     but the reference reloads the pointer for the second test
//     (`mov edx,[edi+0x10]; mov eax,[edx+0x14]`), 433 bytes, 88.4%.
//   * a named `unsigned special = owner->flags & 0x20000000;` in the outer
//     `if`: correct colours, but the compiler computes the mask in place in
//     edx and reloads the flags for `test dh,0x20`; 428 bytes, 85.1%. Same
//     for int/long/bool/const/register spellings and for `owner->flags >> 29`.
//   * 120 declaration permutations of the five prologue locals, in both the
//     before-the-flag-if position and the before-the-frame-test position:
//     max 85.9%.
//   * headers.py on the recoloured frame-first shape: all 256 sets 73.8%.
//   * permute.py 15 min from this file and 15 min from the recoloured
//     frame-first file: no gain (best 85.1%).
// The tie is between the bitmap value and the zero constant and is decided by
// the surviving second test, so no local spelling, declaration order, type or
// header moves it. Do not re-sweep those.
//
// WHAT IS STILL DIFFERENT: the original emits
//   cmp [edi+4],edx / jne / mov [esp+0x10],1 / mov ebx,[edi+0x10] / ...
// while this file emits the bitmap test first and the frame test after it.
// Nothing else differs. The frame-first source recolours, so the next attempt
// has to find a way to make the compiler emit the frame block first without
// moving the bitmap load (the load's position is what picks the colours).
//
// SEVENTH PASS (Space Bunny Free, issue 4723). 423 of 427 bytes, 88.9%, up
// from 87.6%. Two independent changes, and both of them overturn a negative
// recorded below.
//
//  1. THE esi/edi TIE IS SOLVED, and its cause is the FAST-PATH
//     `list->frame++`, not the tail reload. Measured on the reload family
//     (the tail spelled `if (list->bitmap != 0)`, so `this` lands in ebx and
//     the whole tail matches):
//       * deleting the fast-path increment alone: 87.8%, 420 bytes, and the
//         function is byte-identical to the original except that one
//         `inc dword ptr [edi + 4]`;
//       * deleting both increments: 86.6%, 413 bytes;
//       * keeping both, in any spelling: 69.4%, 423 bytes with `list` in esi
//         and the shifted `x` in edi, where the original has esi=x,
//         edi=list.
//     So the trigger is a STORE through `list` after a call inside the
//     conditional block, and it is not a spelling question. Sixteen
//     spellings all score exactly 423 bytes / 69.4%: pre- and post-increment,
//     `= list->frame + 1`, `+= 1`, `= 1 + list->frame`, a `(List_458810*)`
//     cast, `*(int*)((char*)list + 4) += 1`, a `static inline void
//     BumpFrame(List_458810*)` helper, a `int* fp = &list->frame` pointer
//     read through it, `return (list->frame++, 0)`, a `static inline void
//     Bump2(int&)` helper, the increment in each arm of an if/else,
//     `while (1) { ...; break; }`, `do { } while (0)`, an inverted
//     `if (list->bitmap == 0)`, and a `goto`.
//     WHAT WORKS is to write ONE increment, after the if/else:
//         if (list->bitmap != 0) {
//             FUN_00459200(result, list, coords, visible);
//         } else {
//             for (int i = list->pieceCount - 1; i >= 0; i--) { ... }
//         }
//         list->frame++;
//     MSVC 5 tail-duplicates it into the fast path as
//     `inc dword ptr [edi + 4]` (0x45894d) and keeps
//     `mov eax,[edi + 4] / inc eax / mov [edi + 0x4],eax` at the loop exit
//     (0x4589aa). Both are the original's, so nothing is deleted: the source
//     has one increment and the compiler emits the two the original has.
//     That one change took the reload family from 69.4% to 88.2% and made
//     the prologue, the rebuild diamond, the `mov ecx,[edi + 0x10]` reload,
//     `this` in ebx, the memory-resident loop counter and the whole piece
//     loop match byte for byte.
//  2. `coords.y` MUST BE SOURCED FROM A SECOND UNINITIALISED Vec3:
//         Vec3_458810 coords;
//         Vec3_458810 src;
//         coords.x = x;
//         coords.y = src.y;
//         coords.z = z;
//     which homes the value in coords.y's own frame slot so the load reads
//     [esp + 0x20] as the original does. An uninitialised `int local_8`
//     emits the same bytes but homes the value in the `result` argument slot
//     ([esp + 0x30]); that was the last non-jump difference, worth 88.2% ->
//     88.9%. `coords.y = coords.y`, `coords.y = *(int*)((char*)&coords + 4)`,
//     `Vec3_458810* cp = &coords; coords.y = cp->y` and
//     `int ly = coords.y; coords.y = ly;` all lose the write-back pair
//     (419 bytes, 76.0%). Moving `local_8` to the top of the function is
//     byte-neutral (88.9%), so it is the SECOND OBJECT that matters, not
//     where the uninitialised int is declared.
//
// CORRECTION TO THE FRAME NOTE BELOW. [esp + 0x20] is NOT "the dead saved
// ebp slot". With `sub esp,0x18` and four pushes, esp is entry-0x28, so
// [esp + 0x10] through [esp + 0x24] are the six 4-byte locals the FPO
// directory reports: 0x10 rebuild, 0x14 `this`, 0x18 `special`, and
// 0x1c/0x20/0x24 coords.x/coords.y/coords.z. [esp + 0x28] is the return
// address, [esp + 0x2c] is the incoming `list` slot and [esp + 0x30] is
// `result`. The original really does home `visible` in the incoming `list`
// slot (0x45885c, 0x45886c), which is legal because `ret 8` lets the callee
// reuse the argument slots, and this file matches that. So the earlier
// reading of those bytes as the saved ebp is wrong, and so is the claim that
// the uninitialised `local_8` lives there.
//
// THE SHARED `List` TYPE CHECKS OUT. `List_458810` here and `List_459c70` in
// 0x459c70 (the other method of this same `Class_004581e0`) agree where they
// overlap: count at +0x00, owner at +0x0c, bitmap at +0x10, pieces at +0x22 on
// a 0x36 stride, and both `Piece_*` structs put info at +0x00 and vertices at
// +0x22. This function additionally needs the frame counter at +0x04 and
// field_14 at +0x14, which 0x459c70 does not read; adding them is additive
// and does not move any other field. The `Vec3` in 0x459c70 is also
// `{ int x; int y; int z; }`, the same as `Vec3_458810` here, so 0x459200's
// by-value parameter and this function's `Vec3_458810*` are the same type.
//
// WHAT IS STILL DIFFERENT: the doubled `test eax,eax / jne` at 0x4588bd,
// which is 4 bytes (`mov eax,[ebx + 0x14] / test eax,eax / jne X / test
// eax,eax / jne X`), and every jump target after it, which moves by 4
// because of those 4 bytes. Nothing else differs.
//
// THE DOUBLED TEST IS STILL UNREACHABLE, and it now costs the whole byte
// gap. The ONE shape that does emit it is a `Bitmap_458810* bp =
// list->bitmap;` that only the second conjunct reads (p6/u1 in
// build/scratch/0x458810/spec18.py and spec19.py): that gives exactly the
// original's `mov eax,[reg + 0x14] / test eax,eax / jne X / test eax,eax /
// jne X`. But `bp` is a fifth register candidate, so the allocator demotes
// the bitmap from ebx to edx and the flags and the zero constant from edx to
// ebx (`xor ebx,ebx` where the original has `xor edx,edx`, `test bh,0x20`
// where it has `test dh,0x20`, `sete al` where it has `sete bl`), and it
// merges the two `visible` arms into one block. That is 425 bytes and 73.8%.
// The polarity form `if (list->bitmap == 0) rebuild = 1; else if (...)
// rebuild = 1;` with the same local is 431 bytes and 79.5%, and the flat
// polarity form without it is 414 and 56.1%. So the doubled test costs 15
// points either way and is not worth taking.
//
// Every other route folds to the single test or recolours. Measured on this
// exact shape, all worse than 88.9%:
//  * `Bitmap_458810* bitmap = list->bitmap;` used by all five pre-call
//    tests: 431 bytes, 60.7%; used only by the doubled conjunct: 431, 60.7%;
//    block-scoped inside the flag `if`: 425, 73.8%;
//  * a `Bitmap_458810&` reference for the second conjunct: 425, 73.1%; an
//    `int&` bound to the field: 425, 73.1%;
//  * `static inline int F14(Bitmap_458810*)` with the return type `int`
//    (427, 73.8%) and with `bool` (438, 78.6%). The `int` form does reach
//    the original's exact 427 bytes, but by growing the tail, not by
//    emitting the second `test`;
//  * an in-class `int F14() const { return field_14; }`: 427, 73.8%;
//  * a `static Bitmap_458810* cb;`: 436, 87.7%;
//  * and all of these fold to the single test and stay at 423/88.9%:
//    a union overlay on the field, `List_458810::iv[5]`, an extra `int f14`
//    field, `*(&list->bitmap->field_14)`, `*(int*)&X`,
//    `(*(int*)((char*)list->bitmap + 0x14) == 0)`,
//    `(*(Bitmap_458810**)((char*)list + 0x10))->field_14`,
//    `(*list).bitmap->field_14`, a second `List_458810*` copy, a
//    `(const List_458810*)` cast, `0 == X`, `X == 0L`, `!(X != 0)`,
//    `(X ^ 0) == 0`, `X + 0 == 0`, `(short)X == 0`, `(char)X == 0`, `!X`,
//    `(X ? 0 : 1)` and `(X == 0 ? 1 : 0)`; a second `List_458810*` copy read
//    only by the doubled conjunct (423, 88.9%, folds); a `{ Bitmap_458810*
//    b; }` wrapper struct (425, 73.8%); naming the value
//    `int f14 = list->bitmap->field_14;` and testing the local plus the
//    member (433, 54.5%); and splitting the chain into two separate `if`
//    statements (438, 49.8%).
//
// ALSO MEASURED HERE AND NEGATIVE, so the shape above is not one spelling out
// of many. Each is 423 bytes / 88.9% or worse: declaration order of the five
// prologue statements (all 120 permutations; max 69.4% on the reload family,
// inert at 88.9% here); `x`/`z` unsigned, read through a `static inline`
// getter, split into two statements, or read through a `char* game = g_game`
// local; `unsigned short visible`; a `Class_004581e0* self = this` used at
// every call site; a named `List_458810* lp = list` for the tail test; the
// loop as `int i = ...; for (; i >= 0; i--)`; `owner` declared late; the
// tail re-read as `0 != list->bitmap`, `list->bitmap`,
// `*(Bitmap_458810**)list`, `FBM(list)` or `((char*)list + 0x10)`; coords
// declared in the top block; the fast path inverted; a second `List_458810*`
// copy for the tail test; and a `static inline` predicate, tried BOTH ways as
// the brief suggests, around the tail test, the frame test, the bitmap-zero
// test and `if (rebuild)`: every `int`-returning form is byte-neutral at
// 423/88.9% and every `bool`-returning form is worse (430 bytes / 79.7% for
// the tail and the rebuild test, 432 / 85.6% for the frame test), which is the
// same split 0x438ea0 recorded. Passing `list` to the calls through a pointer
// to a local that is never modified (`List_458810* pl = list;
// List_458810** ppl = &pl; ... *ppl`) is 87.5% when the tail test also goes
// through it and byte-neutral at 88.9% when only the call argument does.
//
// The variants live in build/scratch/0x458810/: v0 the previous best, v1 the
// reload family, v3 the merge-point increment, v4 the coords.y fix, v5 this
// file, with the sweeps in spec1..spec17 and the drivers runner.py (score a
// scratch file without touching src/) and sweep.py (apply substitutions and
// score each).
//
// claude-opus-5-5 (#4634): still 87.6% here, but one structural finding for the
// next attempt: after the optional rebuild call the original re-reads
// `list->bitmap` (FUN_004586a0 may rebuild it), so the final test is
// `if (list->bitmap != 0)`, not the cached local. With that change the whole
// second half (this in ebx, the count-down piece loop with the counter in a
// stack slot, the duplicated return tails) lines up and the size becomes 425 of
// 427 bytes, but the head swaps registers (list in esi and x in edi, the
// original has list in edi and x in esi) and the score drops to 57.2%; a
// 15-minute permuter run from that version reached only 65.5%.
// SIXTH PASS (space-bunny-free, issue 3237). The two register ties in this
// function are INDEPENDENT, and one of them is now off the table: what decides
// the `bitmap`/`flags` pair is whether the `bitmap` LOCAL exists, and deleting it
// fixes that pair even in the reload family. With every pre-branch use spelled
// `list->bitmap` (no local at all) and the tail spelled `if (list->bitmap != 0)`,
// the whole prologue block matches the original: `mov ebx,[edi+0x10]` for the
// bitmap, `mov edx,[ecx+0x110]` for the flags, `test dh,0x20` and
// `mov eax,[ebx+0x14]`. That variant is 423 of 427 bytes and 69.4% (v1/d1 in
// build/scratch/0x458810/), and the tail and the loop match byte for byte.
// So:
//   K1 (bitmap local present?)  decides bitmap->ebx vs bitmap->edx.
//   K2 (tail re-reads the field?) decides esi=x/edi=list vs esi=list/edi=x.
// K1 wants no local; K2 wants the reload. This file has K1=no local (wrong, it
// scores 69.4%) or K1=yes + K2=no reload (87.6%, this file), never both.
//
// WHY K2 IS A DEAD END (do not re-sweep it). With the reload in place the
// priority order of the three prologue temps is list, z, x and the register
// order esi, ebp, edi; without it the order is x, z, list. Only the x/list pair
// swaps; z->ebp, bitmap->ebx and flags->edx are the same in both. Measured
// today, all on top of the reload: all 120 permutations of the five declaration
// statements (max 69.4%, 25 of them 87.0-69.0, none above), the z-before-x read
// order (55.2%, which does move x and z: ebp and edi), unsigned x/z, the shift
// split into its own statement, `char* game = g_game` first, a comma
// declaration, both reads through inline helpers, one read through a helper, an
// unused inline function in the unit, `Identity()` wrappers, self-assignments
// (x = x, list = list, bitmap = bitmap, owner = owner), dead stores in folded
// branches after each of the four loads, an extra unused int local, `result`
// and `pieceCount` copies, a second List* copy, `visible = 0`, and every
// placement of coords/local_8 among the other locals. Every one of them is
// 69.4% or worse, byte for byte the same colouring. The only knob that ever
// moved anything was the presence of the local (K1).
//
// WHAT IS STILL WRONG IN THE 69.4% VARIANT besides x/list, for whoever picks
// the tie up: the doubled `test eax,eax` at 0x4588bd needs two structurally
// different expressions for `bitmap->field_14`, and with no local every spelling
// tried folds (a `char*` cast, a `(Bitmap*)` cast, an inline getter taking the
// Bitmap* and one taking the List*, the expression written twice); giving the
// first conjunct a fresh local inside the block gives 431 bytes and 60.0%.
// Then `local_8` is homed in the `result` argument slot (`mov eax,[esp+0x30]`)
// where the original reads the dead saved-ebp slot (`mov eax,[esp+0x20]`), and
// the loop-exit `pop edi` sits before `inc eax` instead of after the store.
//
// The permuter agrees and adds nothing here: 3304 candidates (seed 12) from the
// 87.6% file and 5485 candidates (seed 13) from the 69.4% one both end at exactly
// the score they started at. Sweeping 1 to 5 uncalled `static inline` functions
// into the unit (the compiler-state lever) changes neither basin either.
// Retry (deepseek-v4.1-flash, issue 3042): confirmed the esi/edi priority tie
// is unreachable. The tail-reload family (this promoted to ebx) recolours
// identically (esi=list, edi=x, edx=bitmap) at 57.2% across six new spellings
// (l-copy, x/z swap, cast access, res-copy, owner->kind, local_8 order); a
// coords.y self-copy fixes the y load but drops to 78.7%. Kept the 410/427
// no-reload best.
// GPT-6.1-sol retest in #2859: seven checks kept the 87.6% best. Declaration
// order was unchanged; result alias and tail reload scored lower. Remaining
// reload, coordinate-slot, counter, and loop-result differences are below.
// PARTIAL, 87.6% (410 of 427 bytes; up from 81.9%). SUPERSEDED by the
// seventh pass at the top of this file, which is 88.9% and 423 bytes; the two
// ceilings recorded below are both real but neither is the answer.
//
// WHAT IS SOLVED. The piece array starts at list+0x22, not +0x44, with `info`
// at piece+0, `vertices` at +0x22 and `flags` at +0x28 on a 0x36 stride, under
// `#pragma pack(push,1)`. The `Vec3` really is a local, materialised at
// frame+0x1c and passed by pointer to 0x4584d0, while 0x459200 takes it *by
// value*, and getting that parameter order right is what produces the exact
// `sub esp,0xc / mov edx,esp / push list` interleaving. The two flag conditions
// must be written as inline `owner->flags & 0x20000000` expressions rather than
// through an `int special` local: the local lets MSVC common the two uses, and
// the inline form is what forces the spill to frame+0x18 and the reload for the
// second test, which also fixes the frame at 0x18 and puts the bitmap in ebx and
// the flags in edx (`test dh,0x20`). The prologue then falls out of two changes
// together: moving `int rebuild = 0;` above the two `g_game` reads *and* deleting
// the second `bitmap = list->bitmap;`. That reassignment alone was flipping the
// whole function's allocation, and together they give the original's
// esi=x, ebp=z, edi=list, ecx=owner exactly. `kind` is `unsigned char`, and so
// is the sixth parameter of 0x4584d0, which drops the `xor ecx,ecx` before the
// byte load. The `visible` decision needs `int t = (unsigned char)~owner->field_10e;
// if (t & 1) ... else ...`, which is the only spelling of about sixteen that emits
// the extra `and eax,0xff` in the `not al` sequence.
//
// WHERE THE 12 POINTS ARE, restated after a second pass (Space Bunny Free).
// The earlier note here claimed the 81.9% shape is a miscompilation because it
// "reloads `this` from two different stack slots, one of which holds the
// `special` value". THAT IS WRONG, and I have removed it. Recounting the
// outstanding pushes, all three `this` reloads read post+0x14, which is exactly
// where the prologue stored `this`:
//   0x1c  mov [esp+0x14],ecx            esp=post   -> post+0x14, the `this` home
//   0xe6  mov ecx,[esp+0x14]            esp=post   -> post+0x14  (before the call)
//   0x114 mov ecx,[esp+0x18]            esp=post-4 -> post+0x14  (after `push ecx`)
//   0x174 mov ecx,[esp+0x28]            esp=post-0x14 -> post+0x14 (5 pushes)
// `special` really is at post+0x18, and nothing reads it as `this`. So this
// file is semantically faithful, and 81.9% is honest progress, not a lucky
// miscompilation. Keep it.
//
// THE REAL SPLIT, and it is a register-priority tie I could not break.
// SUPERSEDED: the tie exists but it is not between the two shapes below. It
// is caused by the fast-path `list->frame++`, and one increment written after
// the if/else dissolves it. The two ceilings measured here (81.9% and 69.4%)
// are both real; they just are not where the answer was.
// The original does two things at once that this source cannot do at once:
//   (a) it re-reads `list->bitmap` into ecx AFTER the 0x4586a0 call (0x107),
//       so the pre-call bitmap value is dead and ebx is freed for `this`;
//   (b) it still has esi=x, ebp=z, edi=list, ebx=this in the prologue.
// Spelling the tail as `if (list->bitmap != 0)` is the one-token change that
// forces (a): it frees ebx, MSVC promotes `this` to ebx, the loop counter is
// pushed out of ebx into the dead `this` slot at post+0x14, and the WHOLE tail
// then matches, including both `mov ebx,[esp+0x14]` copies, the `mov ecx,ebx`
// at each call site and the counter's `mov [esp+0x14],eax / dec / mov`. That
// variant is 423 of 427 bytes, i.e. four bytes of `lea`/`inc` reordering away.
//
// But (a) costs (b): with `this` promoted, MSVC gives `list` to esi and lets
// `coords.x` have edi, where the original has them the other way round. I
// measured about forty shapes of the reload family (statement order of
// x/z/rebuild/owner/visible, coords field order, `list->owner` vs `owner`,
// `&list->pieces[i]` vs `list->pieces + i`, a second `List*` copy, a second
// `Bitmap*` copy, a `Bitmap**` pointer-to-field, a `self` copy of `this` used
// at all three call sites, and declaring the g_game reads directly into
// `coords.x`/`coords.z`) and EVERY one of them scores exactly 69.4% with the
// same esi/edi split. It is a priority tie, not a scheduling accident: do not
// re-sweep it. The no-reload family is equally flat at exactly 81.9% across
// about twenty-five shapes. Both ceilings are recorded, with the variants, in
// build/scratch/0x458810/ (v0.cpp is this file, v1.cpp the reload family, and
// sw1..sw13 the sweeps).
//
// FRAME ARITHMETIC worth keeping. Only two locals are inside the 0x18 frame:
// `rebuild` at post+0x10 and the `this` copy at post+0x14. `special` is at
// post+0x18 and the Vec3 at post+0x1c..0x27, that is, MSVC put them ON TOP OF
// the ebx/esi/ebp/edi save area, which is legal because none of those four is
// read again before its `pop`. That is why the reload of `coords.y` reads
// post+0x20, i.e. the saved ebp slot: the value in ebp on entry. It is the
// caller's ebp, not `z`, because the `push ebp` at 0x458819 happens before
// `mov ebp,[eax+0x14323]`. So the original really does copy an uninitialised
// int into coords.y, and this file models that with the uninitialised
// `local_8`; deleting `local_8` and never assigning coords.y drops this file
// to 71.4%, so the explicit uninitialised local is required.
//
// ALSO STILL DIFFERENT. The duplicated `test eax, eax; jne` after
// `mov eax, [ebx+0x14]` (0x4588bd/0x4588bf, 3 bytes). The byte pattern
// `85 c0 75 0c 85 c0` has exactly ONE hit in the whole exe, here, so it is not
// a shared idiom; I tried seven spellings (a doubled `&&` operand, a nested
// `if (c) if (c)`, `!(c != 0) && !(c != 0)`, and a separate statement) and
// MSVC 5 folds every one of them, so this one is not reachable from a
// plausible source spelling. And `coords.y` is read from the arg2 slot at
// [esp+0x30] here rather than from post+0x20; both are "an uninitialised
// slot", 0x10 apart.
//
// THIRD PASS (Space Bunny Free). Re-measured both families and added the missing
// one. Deleting the `bitmap` local entirely and spelling every use as
// `list->bitmap` (so MSVC reloads wherever it must) is NOT a third family: it
// scores exactly 69.4%, the same as the tail-only reload, because a single
// `list->bitmap` read after the 0x4586a0 call is all it takes to promote `list`.
// The ceiling is therefore two-valued and real: 81.9% (no reload) against 69.4%
// (reload), and the gap is the esi/edi priority tie, not the loop.
// On the loop itself, four shapes that try to move the induction variable into
// the frame slot at post+0x14 the way the original has it, while leaving the
// prologue's esi=x / edi=list alone, all lose: `for (n = count; n > 0; n--)`
// over `pieces[n-1]` 78.3%, a `while (n > 0)` with `n--` 79.4%, a manual
// pointer walk with the counter used only by the test (`piece--; n--;`) 76.9%.
// A named `Class_004581e0* self = this;` used at both explicit call sites is
// byte-for-byte identical to the `this` form, 81.9%. So the this-versus-counter
// choice is not reachable from the loop shape either, which is consistent with
// the note above that the difference is a register priority tie.
// Variants live in build/scratch/0x458810/ (v0 this file, v1 tail reload,
// j reload everywhere, a/b/e/f/i loop shapes, g self copy).
//
// FOURTH PASS (space-bunny-free). Three findings, none of which moves the score,
// recorded so nobody repeats them.
//  1. The /Gz lever is ALREADY pulled. The harness compiles this file so that the
//     plain `__thiscall` definition already emits `ret 8` and mangles as
//     `?FUN_00458810@Class_004581e0@@QAEXPAUList_458810@@PAUVec3_458810@@@Z`
//     (QAE). Writing `__stdcall` on the definition mangles it QAGX instead and
//     check.py then cannot even pick the function out of the object, so there is
//     no <xutility>/<algorithm> stand-in to add here: this file calls no template
//     and no out-of-line helper besides the three real members. headers.py's 128
//     header sets are all 81.9% as well.
//  2. The tail-reload family, in its best spelling (`if (list->bitmap != 0)` after
//     the 0x4586a0 call, so ebx is freed and MSVC promotes `this` into it), is 423
//     of 427 bytes and matches the whole tail and the whole loop byte for byte.
//     The ONE thing it gets wrong is the prologue: it gives esi to `list` and edi
//     to the shifted `g_game` x, where the original has esi=x and edi=list. That
//     is the esi/edi priority tie recorded above, confirmed here to be the only
//     remaining difference and not a knock-on: a named `self` copy of `this` used
//     at both call sites, a second `List*` copy `l` used for every field access,
//     and hoisting `int visible` above the two g_game reads all still emit esi=list
//     and edi=x (66.9% each, vC/w1/w2/w3 in build/scratch/0x458810/). Note that
//     the original's `mov [esp+0x14],ecx` write of `this` in the prologue is
//     absent in that family, which is the same tie seen from the other side.
//  3. The loop is a genuine pointer walk, not `&list->pieces[i]`: the original
//     jumps back from 0x4589a8 to the flag test at 0x458972 and only does
//     `sub esi,0x36`, while this file's index form re-derives the address with the
//     `lea ecx,[eax+eax*2] / lea ecx,[ecx+ecx*8] / lea esi,[edi+ecx*2+0x44]` chain
//     every iteration. A manual `p = &list->pieces[n-1]; ... p--; n--;` walk is
//     76.9% on its own and 66.9% combined with the tail reload, i.e. it is not a
//     free win on top of the 81.9% shape either (vA/vB/vC/vD).
// So the 81.9% here is still the best known. The two open items are unchanged:
// the esi/edi priority tie, and the doubled `test eax, eax` at 0x4588bd.
//
// No suspected bug in the original. The duplicated `test eax, eax` is redundant
// and `coords.y` is an uninitialised int read out of the save area, but both look
// like ordinary MSVC artefacts of how the source was written rather than mistakes
// by Cavedog.
//
// deepseek-v4.1-flash confirmation pass. Re-measured the reload family with the
// one-line `if (list->bitmap != 0)` tail: 423 of 427 bytes, 69.4%. The ONLY
// difference left in that variant is that the compiler promotes `list` to esi
// and `x` to edi; the whole tail (ebx=this, the fresh `mov ecx,[esi+0x10]`
// reload, `mov ecx,ebx` at each call, the loop counter in the dead this slot)
// matches byte for byte. So the tail-reload and the prologue's esi=edi swap are
// one tie, exactly as recorded. The doubled `test eax,eax` was retried as
// `bitmap->field_14 == 0 && bitmap->field_14 == 0` and it still folds to the
// single test (406 bytes, 81.9%). Both ceilings stand: 81.9% no-reload against
// 69.4% reload, and 81.9% is kept here.
// UPDATE (deepseek-v4.1): 410 of 427 bytes, 87.6%. The doubled `test eax,eax`
// is SOLVED. Spelling the last conjunct twice, once through the local and once
// through `list->bitmap`, is what the original wrote:
//     && list->bitmap->field_14 == 0
//     && bitmap->field_14 == 0
// The frontend cannot fold two structurally different member accesses, the
// backend still CSEs both loads into one `mov eax,[ebx+0x14]`, and the result
// is the original's two adjacent `test eax,eax / jne` pairs. Writing the same
// expression twice (or via a `(char*)` cast) folds and stays at 406 bytes.
//
// WHAT REMAINS, exactly the four hunk groups below, all one root cause: the
// original frees ebx after the branch and lets `this` live in ebx from 0x4588fa
// to the end, while this source keeps `bitmap` (a source variable, so its value
// survives the 0x4586a0 call in ebx) live into the tail. Effects:
//   0x4588fa `mov ebx,[esp+0x14]` and its twin at 0x458913 (this -> ebx) are
//   missing; the rebuild call reloads ecx from the slot instead of `mov ecx,ebx`.
//   0x458917 the original RELOADS `list->bitmap` into ecx (0x107 mov ecx,[edi+0x10])
//   and this source tests the stale ebx instead (0x45891e `test ecx,ecx`).
//   0x45891a `mov eax,[esp+0x20]`: the uninitialised `local_8` is homed in
//   coords.y itself in the original (a self-copy), here it is homed in the
//   `result` argument slot, so the load reads [esp+0x30].
//   0x45895f onward: with `this` in ebx the original reloads `result` into ebp
//   and keeps the loop counter in memory at [esp+0x14] (`inc eax` /
//   `mov [esp+0x14],eax` / reload / `dec`), where this source has result in ebx
//   and the counter in ebp (`lea ebp,[eax+1]` / `dec ebp`), plus `mov ecx,ebx`
//   at the FUN_004584d0 call instead of a slot reload.
// Every reload spelling tried (a fresh `Bitmap*` local, `if (list->bitmap)`,
// `bitmap = list->bitmap;`, an early-declared late-assigned `bitmap2`, a `self`
// copy of `this` used at the call sites) produces the CORRECT tail (this in ebx,
// mov ecx,ebx, counter in memory, bitmap reload in ecx) at 425 bytes, but the
// allocator then recolours the whole function: `list` moves from edi to esi and
// `coords.x` from esi to edi, and the pre-branch bitmap moves from ebx to edx,
// so the score drops to 57.2%. The pre-branch plan here (esi=x, ebp=z, edi=list,
// ebx=bitmap, this spilled) is correct and must not be disturbed.
extern char* g_game;

struct Vertex_458810 { int x; int y; int z; };

struct Owner_458810 {
    void* relation;               // +0x00
    char unknown_4[0x1c];
    int field_20;                 // +0x20
    char unknown_24[0xff - 0x24];
    unsigned char kind;           // +0xff
    char unknown_100[4];
    float intensity;              // +0x104
    char unknown_108[6];
    unsigned char field_10e;      // +0x10e
    char unknown_10f;
    unsigned int flags;           // +0x110
    unsigned char field_114;      // +0x114
    char unknown_115[3];
};

struct Bitmap_458810 {
    char unknown_0[0x14];
    int field_14;                 // +0x14
};

struct PieceInfo_458810 { char unknown_0[4]; int vertexCount; };

#pragma pack(push, 1)
struct Piece_458810 {
    PieceInfo_458810* info;       // +0x00
    char unknown_4[0x1e];
    Vertex_458810* vertices;      // +0x22
    char unknown_26[2];
    unsigned char flags;          // +0x28
    char unknown_29[0xd];
};

struct List_458810 {
    int pieceCount;               // +0x00
    int frame;                    // +0x04
    char unknown_8[4];
    Owner_458810* owner;          // +0x0c
    Bitmap_458810* bitmap;        // +0x10
    int field_14;                 // +0x14
    char unknown_18[0x22 - 0x18];
    Piece_458810 pieces[1];       // +0x22
};
#pragma pack(pop)

struct Vec3_458810;

class Class_004584d0 {
public:
    void FUN_004584d0(List_458810* list, Vec3_458810* param_2, void* param_3,
        PieceInfo_458810* info, Vertex_458810* vertices, unsigned char kind, int visible);
};

class Class_00459200 {
public:
    void FUN_00459200(void* param_1, List_458810* list, Vec3_458810 coords, int visible);
};

class Class_004581e0 {
public:
    int FUN_004586a0(List_458810* list, int param_2, int param_3);
    void FUN_00458810(List_458810* list, Vec3_458810* result);
};

struct Vec3_458810 { int x; int y; int z; };

// FUNCTION: 0x458810
void Class_004581e0::FUN_00458810(List_458810* list, Vec3_458810* result)
{
    int rebuild = 0;
    int x = *(int*)(g_game + 0x1431f) << 16;
    int z = *(int*)(g_game + 0x14323) << 16;
    int visible;
    Owner_458810* owner = list->owner;
    if (owner->flags & 0x20000000) {
        int t = (unsigned char)~owner->field_10e;
        if (t & 1)
            visible = 1;
        else
            visible = 0;
    } else {
        visible = *(int*)((char*)owner->relation + 0x20) == 0;
    }
    if (list->bitmap == 0)
        list->field_14 = 0;
    if (list->frame == 0)
        rebuild = 1;
    Bitmap_458810* bitmap = list->bitmap;
    if ((owner->flags & 0x20000000) != 0) {
        if (list->bitmap == 0
            || (owner->intensity != 0.0f
                && (owner->flags & 0x2000) != 0
                && list->bitmap->field_14 == 0
                && bitmap->field_14 == 0))
            rebuild = 1;
    }
    if (list->bitmap == 0 && (owner->flags & 0x20000000) != 0)
        rebuild = 1;
    if ((owner->field_114 & 1) != 0 && list->bitmap == 0)
        rebuild = 1;
    if (rebuild) {
        list->field_14 = 0;
        FUN_004586a0(list, 0, 1);
    }
    Vec3_458810 coords;
    Vec3_458810 src;
    coords.x = x;
    coords.y = src.y;
    coords.z = z;
    if (list->bitmap != 0) {
        ((Class_00459200*)this)->FUN_00459200(result, list, coords, visible);
    } else {
        for (int i = list->pieceCount - 1; i >= 0; i--) {
            Piece_458810* piece = &list->pieces[i];
            if (piece->flags & 1) {
                unsigned char kind = list->owner->kind;
                ((Class_004584d0*)this)->FUN_004584d0(list, result, &coords, piece->info,
                    piece->vertices, kind, visible);
            }
        }
    }
    // ONE increment for both paths: MSVC 5 tail-duplicates it into the fast
    // path as `inc dword ptr [edi + 4]` and keeps the three-instruction form
    // at the loop exit, which is what the original has. Writing it twice, or
    // in any other shape, flips esi/edi and costs 19 points.
    list->frame++;
}
