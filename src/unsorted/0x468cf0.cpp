// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, claude-sonnet-5-5 and Space Bunny Free. Names are provisional.
// (Later partial: Space Bunny Free. Earlier partials: deepseek-v4.1-flash,
// then GPT-6, then GPT-6.1-sol.)
// Space Bunny Free pass: 80.9 -> 82.5 percent (5884 -> 5890 bytes), via
// tools/permute.py (20 min, seed 11) from the 80.9 file, then hand-tidied.
// The permuter's win was almost entirely statement order (move_stmt,
// swap_commutative, decl_scope, move_decl, temp_intro, loop_form,
// do_while0/loop_back on the /8 and /16 remainder chains, nested_if on the
// iVar13 >= 0 && iVar13 < mv->f54 pair, and the `if (0 < L.local_204)`
// guard). Its five extracted helpers (inl0..inl4) and its tmp0/tmp1/tmp3/tmp8
// temporaries were all inlined or renamed by hand (cOwner, bShowBox, fZoomY,
// pfZoomY, pcGame, iDebugFlags, iBoxBottom) with no loss of score, and the
// empty `else {}`, the inverted `if (!c) A else B` at the 0x1426f test, the
// stray do/while(0) around the /8 chain, the unit-scan loop and the two
// Bits8 b5/b6 blocks were rewritten as ordinary source, also for free.
// Two permuter shapes are LOAD-BEARING and must stay:
// - the `for (;;) { do { ... } while (0); ... if (>=) break; }` around the
//   0x14367 scan, and the empty `if (owner != cVar3) {} else {...}` in it.
//   Removing the do/while(0) and reading the owner byte once into a named
//   char (the natural tidy spelling) drops the file to 67.6 percent: the
//   second re-read of [edi+0x96]+0x146 and the label placement are what the
//   original does at 0x469c63, and MSVC only emits them in this shape.
// - the `if (!(0 < *(ushort *)(...))) {} else { ... }` at the unit loop.
// A second 22 min permuter run from the 82.5 file (1129 candidates) found
// nothing further, so the rest is not reachable by these rewrites.
// Leftover artifacts to try again if the below stalls: the pfZoomY (0xe8)
// pointer temp at the 0x200 lerp, the pcGame temp for the 0x1431f/16 read,
// the iDebugFlags pointer, and the fZoomY temp before the first lerp.
// Space Bunny Free pass: 82.5 -> 83.1 percent (5890 -> 5897 bytes), 51 -> 48
// diff hunks. Another tools/permute.py run (16 min, seed 29, 1129 candidates)
// whose best_ratio.cpp scored 0.8311 against the 0.8249 it started from; that
// file is the one in place below, with its artifacts hand-tidied. What the run
// bought, all shape rather than register choice:
// - the first `0x1426f` 16-bit index read: `(int)g_game+0x14b*uVar19+0x1b63`
//   in place of `(int)g_game+uVar19*0x14b+0x1b63` (the multiplication order is
//   what decides MSVC's `shl 5 / add / lea ecx,[ecx+ecx*4]` shape).
// - `lVar26 = ((int)*(float*)(iVar12+0x8c));` written in full instead of
//   through the named fZoomY temp.
// - the +0x52 OverlayRect guard split into nested ifs (`if (a > 0.0f) { if
//   (b > c) {...} }` instead of `if (a > 0.0f && b > c) {...}`), likewise at
//   +0x72, with the pointer read hoisted to a named pR and the 0x22e source
//   spelled `iVar11 + 0x102` consistently.
// - `piVar16 = (int *)(iVar13 * 4 + mv->f4)` (index * stride + base, base
//   second) and `puVar9 = ((ushort*)puVar9) + 1`.
// - `L.local_204 + (int)L.local_208 > mv->f38 + -1` with the empty arm
//   dropped, and the same for `L.local_1c0 == 0` / `0 < L.local_204`.
// - the 0x14367 scan bound `(int)iVar13` read through a plain pointer and the
//   row/column coordinate adds regrouped as `0x80 + (a - b)` / `0x20 + (...)`.
// - a named `bool bVisible` for the `param_1 != 0` around FUN_00471f90(6..7),
//   which is what stops the load being re-read in the branch.
// - the 0x3923b test through a `Bits8` bitfield (`->b0`) as at the b5/b6 sites.
// Tidied by hand afterwards, each verified score-neutral at 83.1: the inl0
// helper inlined; tmp1, tmp2, tmp5 and tmp6 renamed bVisible, bDrew, cOwner
// and iRectBottom; pcGame renamed pcView; the pR declaration folded back onto
// one line; and both `MODE %s INFO %s` arms spelled `((int)g_game + 0x3923b)`
// so the two bitfields agree on the base.
// Re-measured this pass and left out (all worse or exactly neutral, so the
// 83.1 file is the best of these):
// - hoisting `mv = g_game + 0x141fb` to just after the player-index read
//   (80.4), and declaring MapGrid *mv first / last / after puVar9 (82.5 each:
//   declaration order does not move MSVC's pick here).
// - dropping the pcView temp, moving `iVar13 = 0` after the /16 reads, and
//   `mv->f54 > 0` for `0 < mv->f54` (all 82.5, byte-identical).
// - the tick update written with a named delta temp (80.9), with the old value
//   read into a temp first (80.9), with `*pDVar1 = DVar7` before the accumulate
//   (80.8) or after the following FUN_00483fa0 call (82.7). The 82.5 spelling
//   really is the one in place; the store order is not reachable from source.
// - reading the 0xdcb byte through the existing pcVar18 (67.2), through a new
//   pcFont temp (82.6), or as `*(byte*)((int)L.local_208 + 0xf)` (83.1).
// - the L.local_178/L.local_174 pair read at each call instead of hoisted
//   (81.9), assigned inside the 0x38a51 branch as well as after it (81.4),
//   assigned 174-before-178 (83.0), and split into two fresh locals (83.1).
// - the +0x52 / +0x72 float compared through a named temp (82.5 each) and the
//   0x1ca9 / 0x1b63 addresses split as `u + u*0x14a` (83.1 each: the extra
//   `lea eax,[eax+edx*2]` vs the original's folded `mov cl,[eax+edx*2+0x1ca9]`
//   is scheduling, not spelling, as the notes below already record).
// - the first insert loop rewritten as a plain `for` with the index times
//   stride and no `-4 + iVar13*4` (81.6), with the iVar14 temp inlined
//   (81.1), and with `-4` moved to the end of the index expression (83.1).
//   The inc-then-index form in place is what gives `inc ecx` before the store
//   and `mov [esi + ecx*4 - 4], edx`, so it is load-bearing too.
// - an alias `mvGrid`/`mvLoops` for MapGrid *mv (83.1 each, byte-identical:
//   MSVC does not keep two names for one value in registers).
// - the `FUN_00435100() == 3 || == 2` test read through a named Class_00435100
//   pointer (80.6 both ways): the original's two reloads of
//   `*(int *)(g_game + 0x391e9)` need the expression spelled out twice.
// - the "MODE %s INFO %s" pair through named char* temps (83.1) and both arms
//   spelled `((int)g_game + 0x3923b)` (83.1; kept, it is the tidier form).
// - the +0x52 / +0x72 guard float hoisted into a named float before the rect
//   copy (83.1 both, byte-identical), so the fld/fcomp-vs-copy order noted
//   below really is pure scheduling and not reachable from the source.
// Re-measured and reverted this pass (all at the 80.9 baseline):
// - moving the `mv = g_game + 0x141fb` assignment up to just after the
//   player-index read, so MSVC would `lea edi,[edx+0x141fb]` as the original
//   does at 0x469683: 80.7, and mv still lands in ebx, not edi. The blocker
//   really is the param_1 live range, not the assignment position.
// - reading L.local_174/L.local_178 inside each FUN_004b7f90 branch instead
//   of hoisting them into iVar12/iVar11: 78.3 (the original's two separate
//   edi/esi load pairs need the hoisted form we already have).
// - `(int)lVar25 + iVar21` for the L.local_1ab lerp: byte-identical to
//   `iVar21 + (int)lVar25`, confirming the old add-esi-vs-add-eax note.
// STILL DIFFERS, all register allocation and scheduling (48 hunks at 83.1):
// - The two big regions are still the same swap the earlier passes describe:
//   the original keeps mv in edi and the /16 view x in ebx, ours keeps mv in
//   ebx and the view x in edi, so param_1's CSE load moves with them. Every
//   attempt to make MSVC pick edi for mv has failed (see the retried notes),
//   and this pass adds that declaring MapGrid *mv first, last or mid-list
//   makes no difference either, so it is not the declaration order either.
//   This one region is most of the remaining 17 percent: the @@ -720 hunk
//   (the first insert loop and the 0x1435f/0x14367 scan) and the @@ -843 hunk
//   (both unit loops and the 0x71f90 calls between them) are 360 of the ~370
//   differing lines, and every one of them is this same edi/ebx pair.
// - `test edx,edx` (orig) vs `cmp edx,ebp` with ebp materialised as a zero
//   register (ours) in the first insert loop and the 0x1431f/16 setup; ours
//   spends a register on the zero that the original folds into `test`.
// - The FPU `fld dword ptr [ebp+0xa4]` / `fcomp` is issued before the +0x52
//   OverlayRect copy in the original and after it in ours (both rect copies,
//   the 0x278 and 0x485 hunks).
// - The `pDVar1[n] += DVar7 - *pDVar1; *pDVar1 = DVar7;` trio: the original
//   stores *pDVar1 after the push of the next call's argument, ours before.
// - `mov bl, byte ptr [ebp+ecx]` (orig) vs `[ecx+ebp]` (ours) for
//   L.local_1b0[iVar12]; the index and base registers are right but the
//   base/index slot order is not.
// - param_1 is reloaded into ebx by the original at 0x469bc9/0x469c01 and into
//   edi by ours, and the FUN_00471f90 3/4/5 calls around it are scheduled
//   before the test in ours and after it in the original.
// claude-sonnet-5-5 structural pass: 61.3 -> 80.9 percent (5963 -> 5884 bytes).
// What worked:
// - Rect copies at +0x52/+0x72 as OverlayRect struct assignments through a
//   pointer (`*(OverlayRect*)&L.local_188 = *pR`): gives the original's
//   `lea edi,[esi+0x52]; mov eax,edi` source-pointer copy.
// - puVar9 = L.local_208 placed on the else/merge path with `goto LAB_join1`
//   after the inner FUN_004bf6f0 call (original skips the merge reload).
// - Bit flags: `Bits8` with ushort bit-fields gives `mov cl,[m]; shr cl,n;
//   test cl,1` (a ushort container read as a byte); `UnitFlags` with uint
//   bit-fields (kind:2, b4) gives the original's dword load plus `and ecx,3`.
// - abs((int)local_1a3) in the plain "%d" arm; `-0x22 - FUN_004c1450() +
//   FUN_004b6710()` as one expression; (flags&0x40)?6:0 for the neg/sbb form.
// - bVar23 chain reordered (param_1, flags&8, 0x2cc3==0xe) with the call
//   result compared in place (`b = f() != 0` gives neg/sbb/neg, not setne).
// - Rect corners kept in plain locals (swap via temp) before the stores.
// - FUN_004b7f90(.., FUN_004b7f30(..), x, y) written as nested calls so x/y
//   are pushed before the inner call; counts as `0 < *(ushort*)` (jbe).
// - The g_game+0x141fb sub-struct is `MapGrid *mv`, and the /16 of the view
//   origin is computed before the first loop, as in the original.
// STILL DIFFERS (register allocation, all of it): the original keeps mv in
// edi and the /16 view x in ebx, ours swaps them (mv ends up in ebx, so the
// param_1 CSE load also moves from ebx to edi/eax); ours uses a zero register
// (ebp/ebx) for stores of 0 where the original uses immediates; the three
// `*pDVar1 = DVar7` stores are scheduled earlier than in the original; the
// FPU `fld/fcomp` vs struct-copy interleave at the two rect copies; the
// `lea edi,[eax+0xdcb]` (orig) vs `add eax,0xdcb` at the second 0xdcb pointer.
// deepseek-v4.1-flash retry 12 (10 min timebox): 57.4 -> 61.3 percent (5977 ->
// 5963 bytes). Three type/idiom fixes, all found by grepping the check.py diff
// for 16/8-bit load forms that only appear on OUR side:
// 1. In the 0x37f06 unit-label block the original writes a lone zero byte
//    (`mov byte ptr [esp+0x15], 0`) where we wrote a whole `movzx dx` plus
//    `mov word ptr [esp+0x14], dx` pair. The source was
//    `*(ushort*)&L.local_210 = (ushort)(byte)L.local_210;`; rewriting it as
//    `*((byte *)&L.local_210 + 1) = 0;` removes the pair and matches. 59.1.
// 2. The two `*(char *)(record->0x96 + 0x146) == L.local_1b4` compares: the
//    original emits `mov bl,byte ptr [esp+0x70]` plus `cmp byte ptr
//    [ecx+0x146],bl` (no sign-extend, 8-bit compare). Assigning
//    `cVar3 = L.local_1b4;` (a plain char) and comparing against cVar3 gives
//    exactly that; the old "byte compare regresses" note is stale. 59.5.
// 3. `iVar13 = *(int *)(g_game+0x1426f) + (uint)*(ushort *)(rec+8) * 0x100;`
//    emits `mov ax,[rec+8] / and eax,0xffff`; the original does
//    `xor ecx,ecx / mov cx,[rec+8]`. Hoisting the 16-bit read into a named
//    uint temp then multiplying gives the zeroed-register form. The compare
//    `(-1 < iVar13)` -> `(iVar13 >= 0)` after the `+0x10` also turned our
//    `cmp eax,-1 / jle` into the original's `js` (59.6), and the same 16-bit
//    read in the 0x14367 scan loop rewritten through an int temp
//    (`uVar6 = (int)*(ushort *)puVar9;`) gives the original's
//    `xor ecx,ecx / mov cx,[ebp]` and its index-register reuse in
//    `shl eax,3 / sub eax,ecx` (61.3).
// Also measured, all byte-identical or worse: the puVar17 = iVar11+0x52
// pointer form for the 0x52/0x72 OverlayRect reads (identical code), the
// 0x110 pointer temp (identical), and swapping the two 0x2cac/0x2cb4
// coordinate statements (59.2, reverted again). Still differing: the two big
// register-allocation regions (@@-717, @@-1153) where the original folds
// g_game+0x141fb into edi, keeps param_1 in ebx and picks esi/edi the other
// way round in the unit loops.
// deepseek-v4.1-flash retry 13 (10 min timebox, no gain, reverted): the
// original's `lea edi,[edx+0x141fb]` in the 0x4696xx region really is a base
// pointer for the first insert loop: retargeting the existing iVar8 to
// `(int)g_game + 0x141fb` (no new slot, so the frame is unchanged) reproduces
// the original's `[edi+0x54]/[edi+0x50]/[edi]/[edi+4]/[edi+8]` displacements
// exactly, just with ebx instead of edi (msvc picks the register: ours has
// param_1's ebx free there while the original keeps param_1 live in it), and
// the displaced ebp then becomes a zero register (`mov [edx+ecx*2-2],bp` where
// the original stores an immediate 0). 60.2 percent, 5849 bytes (closer to
// 5904) but with the same 28 hunks: the gain in the first insert loop is more
// than lost at @@-928, where the original goes back to g_game-based
// `[ebx+0x142xx]` access in the 0x4698xx pixel scan. Splitting the spelling
// (base pointer in the first loop only, g_game for the rest) gives the same
// 60.2 at 5986 bytes, so the two spellings must be reconciled with the
// param_1-in-ebx live range instead, which the notes above already identify as
// the real blocker.
// deepseek-v4.1-flash retry 11 (10 min timebox): re-confirmed 57.4 percent /
// 5977 bytes. All cheap named levers were re-checked against the notes below
// and are already tried or byte-neutral (the `add eax,esi` clamp operand order
// tied at 53.1 when swapped, the named delta temp in the pDVar1[19] tick
// update is neutral, the 0x52 read-pair and 0xdcb prologue pointer spellings
// regress). The residual is exactly the two big register/scheduling regions
// listed in retry 10 below.
// deepseek-v4.1-flash retry 10 (10 min timebox): 5979 -> 5977 bytes, still
// 57.4 percent. The 0x222 argument to the first FUN_004bf6f0 read the field as
// a byte with a zero-extend (`xor ecx,ecx; mov cl,[esi+0x222]`); the original
// loads the full dword (`mov ecx,[esi+0x222]` at 0x46911a), so the arg is now
// spelled `*(int *)(iVar11 + 0x222)`. Everything else still differs as listed
// below: the 0x52 read pair (original materialises edi = iVar11+0x52 at
// 0x4690b0, reads [eax]/[eax+4]/[eax+8]/[eax+0xc], keeps edi live across the
// _ftol at 0x46910e, reads the second group through [edi] at 0x469159, and only
// restores edi = L.local_208 at 0x469193/0x4691b9; ours re-reads [esi+0x52]
// with direct displacements and reloads L.local_208 early), the g_game+0xdcb
// lea/keep-in-edi vs our add/reload in the FUN_004c13a0 arg, and the big
// @@-717 / @@-1153 register-allocation regions.
// and reverted one variant: scoping piVar10 = g_game+0x141fb to just the
// 0x1424b insert loop (piVar10[20]/[21]/[1]/[2]/[0] spelling for the
// 0x1424b/0x1424f/0x141ff/0x14203/0x141fb slots, sized to keep edi) regresses
// 57.4 -> 49.4 at 5980 bytes: the extra pointer slot grows the frame and
// shifts the whole OverlayLocals struct, confirming the SHARED 0x468cf0 note
// that piVar10 must stay inlined at every use. Still differing: the big
// @@-717 and @@-1153 regions (original keeps a folded g_game+0x141fb base in
// edi with small displacements there while ours reloads g_game and uses
// [ebx+0x142xx]), plus the usual scheduling hunks (the first
// pDVar1[19] += DVar7 - *pDVar1 store position, the [eax+edx*2] lea order,
// and the add esi,eax vs add eax,esi forms at both /8 clamp sites).
// deepseek-v4.1-flash retry 4 (timeboxed): one variant tried, no gain, reverted.
// deepseek-v4.1-flash retry 8 (10 min timebox): 55.3 -> 57.4, 5963 -> 5979 bytes.
// Both +/-99999.0f sprintf pairs inverted to put the "%dK" arm first:
//   if (L.local_1a7 >  99999.0f) "%dK" /1000; else "%d";
//   if (L.local_1a3 < -99999.0f) "%dK" /1000; else "%d";
// This yields the original's jne-to-plain-arm / K-fall-through layout and its
// compare polarity at both sites (the old note that the local_1a3 inversion is
// worse was measured against the older register allocation and no longer holds).
// Also confirmed byte-neutral: `iVar21 += (int)lVar25; L.local_1ab = (float)iVar21;`
// in place of `L.local_1ab = (float)(iVar21 + (int)lVar25);` (identical output),
// so the add esi,eax vs add eax,esi difference is not statement placement.
// deepseek-v4.1-flash retry 7 (10 min timebox): 53.8 -> 55.3, 6023 -> 5963 bytes.
// Two shape fixes, both now in the file:
// 1. The prologue g_game+0xdcb pointer: introducing a plain local pointer
//    (reusing pcVar18) for the L.local_1b0 store and the two FUN_004be950
//    6th args ([0xf] reads) makes MSVC hold g_game+0xdcb in ebx across the
//    FUN_004c2470/FUN_004c6b10 calls instead of keeping g_game there, so the
//    whole lea ebx,[eax+0xdcb] / mov [esp+0x74],ebx prologue hunk disappears.
// 2. The first FUN_004c1420 index read at the memcmp!=0 branch was written as
//    (int)g_game + 0x3816b + uVar19*0x232; reading it through the iVar11 that
//    was just assigned ((int)((int *)(iVar11 + 0x22e))) makes the original's
//    base (= g+0x37f3d+u*0x232, small displacements everywhere) fall out and
//    kills the whole 0x380xx-displacement family of diffs.
// Also tried and reverted: Ghidra's split spelling of the 0x1ca9/0x1b63
// addresses (u + 0x14a*u) leaves the CSE add eax,edx (53.7); a piVar10 =
// g_game+0x141fb base pointer for the whole [edi+...] region gets the frames
// right but scores 54.4 (5852 bytes, 23 hunks vs 28) because edi is then lost
// to the other live range at 0x4691b9 that the original also keeps in edi.
// iVar21/iVar12 coordinate statements (iVar12 first) regresses 53.8 -> 53.5 and
// is byte-neutral size-wise (6023); hoisting the 0x14280 byte into cVar3 before
// the iVar12 line is compiled away (identical 6023 bytes, 53.8), so the mov cl
// between the two subs is still pure scheduling, not statement placement.
// deepseek-v4.1-flash retry 5 (10 min timebox, no gain, reverted): the prologue
// hunk (ours: mov ebx,eax / add eax,0xdcb / mov [esp+0x74],eax versus the
// original's lea ebx,[eax+0xdcb] / mov [esp+0x74],ebx, i.e. the original keeps
// ebx = g_game+0xdcb and reloads g_game, ours keeps g_game in ebx) was attacked
// from both ends of the be950 byte reads: reading them as *(byte*)((int)g_game
// +0xdda) (to kill the iVar11 live range) gives 53.2/6027, and as
// L.local_1b0[0xf] gives 52.2/6023 (confirming the earlier 51.9 note), so the
// ebx assignment is not reachable by rewriting those two reads alone.
// Correction to the note further down: the local_1a3 99999.0 site is the ONLY
// one whose compare idiom still differs (base diff, check.py output line ~679):
//   original: test ah,1 / je 0x46933e     ours: test ah,1 / jne 0x469340
// (all the other sites already emit test ah,0x41 and match, so the older note
// claiming 0x41-vs-0x1 is stale). Rewriting that site as
//   if (L.local_1a3 < -99999.0f) sprintf "%dK" /1000; else sprintf "%d";
// does give the original's je polarity but scores 53.4 (6031 bytes): the
// surrounding register allocation has to line up first, so the plain
// `>= -99999.0f` form stays. The whole remaining diff is 1831 lines of
// register/scheduling noise, so attack it block by block against the saved
// base diff (build/scratch/0x468cf0/base.txt in the issue-3534 worktree)
// rather than by dyadic rewrites of one statement.
// deepseek-v4.1-flash retry 3 (timeboxed): 53.1 -> 53.8, 6029 -> 6023 bytes.
// Three fixes, all statement/evaluation order rather than structure:
// 1. The 33-byte memcpy of g_game+0x37e3f must come BEFORE the cVar3 load in
//    the source: the original's mov cl,[eax+edx*2+0x1ca9] is scheduled between
//    rep movsd and movsb, so the load is emitted mid-memcpy only when the
//    memcpy statement precedes it. 53.1 -> 53.5.
// 2. The FUN_004c1420 index at 0x3816b is read through the iVar11 (= iVar12 +
//    0x1b63) pointer with +0x27, not through iVar12 + 0x1b8a: the original
//    folds +0x1b63 into the lea and reads [eax+0x27]. 53.5 -> 53.7.
// 3. The FUN_004c13a0 byte arg near the first FUN_004c1420 is read through the
//    just-stored L.local_208 (= g_game+0xdcb) pointer with +0xf: the original
//    emits lea edi,[eax+0xdcb]; mov [esp+0x1c],edi; mov cl,[edi+0xf]. 53.7 ->
//    53.8. Same trick did NOT help in the prologue (be950 args via
//    L.local_1b0[0xf], already known to give 51.9).
// Neutral (compiles to identical bytes): dropping the parens in the
// 0x2cb4 - (0x2cb0>>1) - 0x14323 expression, assigning local_19f before
// local_18f, a named delta temp in the pDVar1[19] tick update, and inlining
// FUN_004c13f0() into the FUN_004c13a0 argument list at either site.
// deepseek-v4.1-flash retry 2 (timeboxed, 3 variants, no gain): routing the two
// FUN_004be950 byte args through L.local_1b0[0xf] instead of *(byte*)(iVar11+0xdda)
// gives 51.9 (it perturbs far more than the 2 [ebx+0xf] sites); swapping the
// 0x2cac/0x2cb4 coordinate assignments gives 52.8; splitting the >>1 of 0x2cb0
// into its own leading statement is byte-neutral (53.1, same 6029 bytes).
// STATUS AT THAT TIME: partial 53.1% (deepseek-v4.1-flash retry). Two fixes that pass:
// 1. The `/8` remainder middle branch: the original emits `test eax,eax; jle`
//    (the trailing zero-case is the forward jump target), so the middle branch
//    must read `else if (iVar21 > 0) { divide; if==0 -> 1 } else { iVar21=0 }`
//    with the zero case LAST, not `else if (iVar21 <= 0) { 0 } else { divide }`
//    (which gives `cmp eax,1; jge` / `jg` fall-through). 52.9 -> 53.0 -> 53.1.
// WHAT STILL DIFFERS (large, register/scheduling, not source-controllable here):
// - Prologue: original does `lea ebx,[eax+0xdcb]` and keeps that pointer in ebx
//   for the whole function; ours does `add eax,0xdcb` and uses eax, reloading
//   g_game. Also `[ebx+0xf]` vs our `[ebx+0xdda]` (base reg is g_game not the
//   g_game+0xdcb pointer).
// - The add that forms `iVar21 + lVar25` for the local_1ab/local_19f updates:
//   original `add eax,esi` / `mov [esp+0x10],eax`; ours `add esi,eax` /
//   `mov [esp+0x10],esi` (commuted result register). Swapping the operand order
//   `(int)lVar25 + iVar21` did NOT change it (tied at 53.1).
// - movsx/lea ordering in the cVar3/iVar12 address arithmetic (ours splits
//   `lea eax,[eax+edx*2]`/`lea ebp,[eax+0x1b63]`, original folds
//   `mov cl,[eax+edx*2+0x1ca9]`/`lea ebp,[eax+edx*2+0x1b63]`).
// - The 99999.0 compare flag idiom (`test ah,0x41; jne` vs our `test ah,1; je`).
// - The pDVar1[14] += DVar7 - *pDVar1 tick update and the OverlayRect argument
//   copy at the first FUN_004c6b10 (ours uses eax as destination).
// No obvious original bug spotted in this pass.
// Partial, 48.1% (deepseek-v4.1: 46.2% partial then three fixes below).
// The big local struct sits at the TOP of the locals region, so its base is
// 0x10 + (bytes of scalar slots below it); the original has exactly one scalar
// slot (the fild/fistp conversion scratch at [esp+0x10]), giving base 0x14 and
// local_178 at [esp+0xac]. Ours had two (the piVar10 home was the extra one);
// computing g_game+0x141fb inline at each use freed that slot, which moved
// every struct field back by 4 and took the file 40.0 -> 45.3. Assigning
// local_1b0 after the 48-byte memcpy instead of before it (as the original's
// instruction order shows: mov ecx,0xc / lea edi / mov esi / lea ebx /
// rep movsd / mov [esp+0x74],ebx) gave 45.3 -> 46.2.
// deepseek-v4.1 additions, 46.2 -> 48.1:
// 1. The two FUN_004be950 calls in the 0x14280 == 2 branch: the original folds
//    +0x80 into iVar21 and +0x20 into iVar12 in the definitions (its code does
//    add edi,0x80 / add esi,0x20 right after the loads and then passes
//    edi+2, esi, edi-2, esi and edi, esi-2, edi, esi+2), not into the call
//    arguments. Moving the constants there took 46.2 -> 48.0.
// 2. The 33-byte memcpy of g_game+0x37e3f into local_1ac comes after the
//    cVar3/iVar12 address arithmetic in the original (mov cl,[eax+edx*2+0x1ca9]
//    after the mov ecx,8 setup, movsb last). Moving it after those two
//    statements took 48.0 -> 48.1.
// deepseek-v4.1, 48.1 -> 51.6 (five independent fixes):
// 1. The /8 and /16 remainders: the original's bodies use the cdq form
//    (cdq / and edx,7 / add eax,edx / sar eax,3), i.e. plain `x / 8`, not the
//    hand-written `(x + (x >> 0x1f & 7)) >> 3` shift form (49.3).
// 2. The four 99999.0 comparisons are against a FLOAT constant in the original
//    (fcomp dword ptr), so 99999.0f / -99999.0f (49.3).
// 3. The big struct is 4 bytes too small: local_100 is 256 bytes in the
//    original, not 252, which makes the frame 0x214 and moves the param homes
//    to [esp+0x228]/[esp+0x22c], matching (49.4).
// 4. Float compare operand order: the original loads local_1ab (not local_193)
//    first, so the source reads `local_1ab > local_193`, and likewise the
//    `> 0.0f` guards and `local_1a3 >= -99999.0f` put the memory operand on
//    the left (50.0, 51.1).
// 5. The two FUN_004658e0 calls pass full 32-bit values in the original (it
//    does mov ebx,[esp+0x14] and pushes ebp, with no movsx), so drop the
//    (short) narrowing casts on puVar9 / iVar12 / L.local_210 (51.6).
// Also worth trying next: the original keeps the row and column loop counters
// in the struct fields L.local_210 (row, inc per row) and L.local_208 (column),
// reading them back with mov/movsx, rather than in the registers our version
// uses; the original also reads the spilled local_1b4 byte back for the
// *0x14b index (mov eax,[esp+0x70] / and eax,0xff) instead of re-reading
// g_game+0x2a43.
// deepseek-v4.1-flash retry, 51.6 -> 52.9:
// 1. Re-read of the player index: the original reads the spilled local_1b4
//    back for the g_game + idx*0x14b base (`mov eax,[esp+0x70]; and eax,0xff`
//    right after `mov byte ptr [esp+0x70], al`), so use L.local_1b4 in the
//    iVar12 = g_game + (uint)L.local_1b4*0x14b line instead of re-reading
//    g_game+0x2a43. 51.6 -> 52.9.
// Tried and reverted (worse): (a) dropping the early `iVar11 = (int)g_game`
// and reading `L.local_1b0[0xf]` for the FUN_004be950 args to try to get the
// original's `lea ebx,[eax+0xdcb]` (50.4); (b) making the two
// `*(char*)(...[0x96]+0x146) == local_1b4` comparisons byte (the original
// does `cmp byte ptr [ecx+0x146], bl`, ours sign-extends to int via
// `movsx`/`and 0xff`/`cmp ecx,ebx`) (52.8, size 12 smaller but a hair lower).
// WHAT STILL DIFFERS: the same large register/scheduling issues as before plus
// the earlier `lea ebx,[eax+0xdcb]` vs `add eax,0xdcb`, the address CSE in the
// local_1ac region, and the 99999.0 compare flag idiom (`test ah,0x41; jne`
// vs our `test ah,1; je`, same semantics, different emitted form).
// NEXT: the byte compare above is likely right semantically but needs the
// surrounding register picks (edi/esi swap, ebp/ebx) to also line up before it
// scores; the `[esp+0x70]` byte reads should use bVar5 (byte) on both sides
// only once the block's base registers match.
// OLD NOTES (kept):
// The original keeps the local_1b0 pointer (= g_game+0xdcb) in ebx for the
// whole function (lea ebx,[eax+0xdcb] at 0x468d49, [ebx+0xf] at both
// FUN_004be950 calls) and reloads param_1 into ebx at 0x469b02/0x469d31/
// 0x469f29; ours keeps g_game in ebx, writes [ebx+0xdda], and reloads param_1
// into eax/edi/esi. Also outstanding: the pDVar1[14] += DVar7 - *pDVar1 tick
// updates (ours hoists the *pDVar1 store), the OverlayRect argument copy at the
// first FUN_004c6b10 call (ours uses eax as destination), and the address CSE
// in the local_1ac region.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
extern char* g_game;
unsigned long FUN_004b6560();
int __stdcall FUN_00415fa0(int);
int __stdcall FUN_00417f30(int,int);
int __stdcall FUN_00418310(int);
int __stdcall FUN_00420b00(int);
struct Class_00435100 { int FUN_00435100(); };
int __stdcall FUN_0045ac20(int,int);
int __stdcall FUN_0045ffb0(int);
int __stdcall FUN_00464060(int);
float __stdcall FUN_00464ab0(int);
float __stdcall FUN_00464ac0(int);
float __stdcall FUN_00464af0(int);
float __stdcall FUN_00464b00(int);
int __stdcall FUN_004658e0(int,int,int,int,int,int);
int __stdcall FUN_00466b00(int);
int __stdcall FUN_00467a20(int,int,int,int);
int __stdcall FUN_00467c00(int,int,int,int);
int __stdcall FUN_00468380(int);
int __stdcall FUN_004689c0(int);
struct Class_0046a400 { int FUN_0046a400(int); };
int __stdcall FUN_0046a430(int,int,int,int);
int __stdcall FUN_0046a530(int,int);
int __stdcall FUN_0046a610(int,int,int,int);
int __stdcall FUN_0046a860(int);
int __stdcall FUN_0046b900(int,int,int);
int __stdcall FUN_00471f90(int,int);
int __stdcall FUN_00483fa0(int);
int __stdcall FUN_004848e0(int);
int __stdcall FUN_0048c190(int,int);
int __stdcall FUN_0048cc30(int,int);
int __stdcall FUN_004948e0(int);
int __stdcall FUN_0049be60(int);
int __stdcall FUN_004ab170(int,int,int);
int FUN_004b66a0();
int FUN_004b6710();
int __stdcall FUN_004b6720(int,int,int);
int __stdcall FUN_004b7f30(int,int);
int __stdcall FUN_004b7f90(int,int,int,int);
int __stdcall FUN_004be950(int,int,int,int,int,int);
int __stdcall FUN_004bf6f0(int,int,int);
int __stdcall FUN_004bf8c0(int,int,int);
int __stdcall FUN_004c13a0(int,int);
int FUN_004c13f0();
int __stdcall FUN_004c1420(int);
int FUN_004c1450();
int __stdcall FUN_004c1480(int,int);
int __stdcall FUN_004c14f0(int,int,int,int,int);
int __stdcall FUN_004c1b80(int);
int FUN_004c2470();
int FUN_004c2870();
int __stdcall FUN_004c5740(int);
int FUN_004c63a0();
int __stdcall FUN_004c69a0(int);
int __stdcall FUN_004c69c0(int);
struct OverlayRect { int left,top,right,bottom; };
struct Class_004c6b10 { int FUN_004c6b10(OverlayRect); };
typedef unsigned long DWORD;
typedef double float10;
typedef __int64 longlong;
void FUN_00444ba0();
#pragma pack(push,1)
struct OverlayLocals {
  int local_210;
  int local_20c;
  ushort *local_208;
  int local_204;
  uint local_200;
  int local_1fc;
  uint local_1f8;
  int local_1f4;
  uint local_1f0 [12];
  int local_1c0;
  int local_1bc;
  int local_1b8;
  byte local_1b4;
  byte pad_1b3[3];
  byte *local_1b0;
  char local_1ac;
  float local_1ab;
  float local_1a7;
  float local_1a3;
  float local_19f;
  float local_19b;
  float local_197;
  float local_193;
  float local_18f;
  byte resource_pad[3];
  uint local_188;
  undefined4 local_184;
  undefined4 local_180;
  undefined4 local_17c;
  int local_178;
  int local_174;
  byte local_170 [32];
  byte local_150 [80];
  byte local_100 [256];

};
#pragma pack(pop)
struct MapGrid { int f0; int f4; int f8; char pad1[0x2c]; int f38; int f3c; char pad2[0x10]; int f50; int f54; char pad3[0x34]; int f8c; };
struct Bits8 { ushort b0:1; ushort b1:1; ushort b2:1; ushort b3:1; ushort b4:1; ushort b5:1; ushort b6:1; ushort b7:1; };
struct UnitFlags { uint kind:2; uint b2:1; uint b3:1; uint b4:1; uint b5:1; uint b6:1; uint b7:1; };
// FUNCTION: 0x468cf0
void __stdcall FUN_00468cf0(int param_1,int param_2)

{
  unsigned int bDrew;
  int iDebugFlags;
  float* pfZoomY;
  short *psVar2;
  char cVar3;
  ushort uVar4;
  byte bVar5;
  int uVar6;
  DWORD DVar7;
  int iVar8;
  MapGrid *mv;
  int lVar25;
  int lVar26;
  ushort *puVar9;
  int *piVar10, iVar11, iVar14;
  int iVar13;
  OverlayRect *pR2;
  int *piVar16;
  uint uVar15;
  char *pcVar18;
  uint uVar19;
  UnitFlags *pF;
  int iVar21;
  int bVar23;
  float10 fVar24;
  DWORD *pDVar1;
  OverlayLocals L;

  int iVar12;

  L.local_178 = (*(int *)((int)g_game + 0x37e1f) + 0x80) / 2;
  L.local_174 = *(int *)((int)g_game + 0x37e23) / 2;
  FUN_004c69a0((int)(*(undefined4 *)((int)g_game + 0x37e1b)));
  pcVar18 = (char *)((int)g_game + 0xdcb);
  memcpy(L.local_1f0,*(void**)(g_game+0x37e1b),48);
  L.local_1b0 = (byte *)pcVar18;
  FUN_004c2470();
  ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  iVar12 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[19] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  FUN_00483fa0((int)((ushort *)L.local_1f0));
  FUN_00418310((int)(L.local_1f0));
  iVar21 = (int)*(short *)((int)g_game + 0x2cac) - *(int *)((int)g_game + 0x1431f) + 0x80;
  iVar12 = (int)*(short *)((int)g_game + 0x2cb4) - ((int)*(short *)((int)g_game + 0x2cb0) >> 1)
           - *(int *)((int)g_game + 0x14323) + 0x20;
  if (*(char *)((int)g_game + 0x14280) == '\x02') {
    FUN_004be950((int)(L.local_1f0),(int)(iVar21 + -2),(int)(iVar12),(int)(iVar21 + 2),(int)(iVar12),(int)((uint)(byte)pcVar18[0xf]));
    FUN_004be950((int)(L.local_1f0),(int)(iVar21),(int)(iVar12 + -2),(int)(iVar21),(int)(iVar12 + 2),(int)((uint)(byte)pcVar18[0xf]));
  }
  FUN_004c69c0((int)((int *)L.local_1f0));
  iVar11 = (int)g_game;
  uVar19 = (uint)*(byte *)((int)g_game + 0x2a43);
  iVar12 = (int)g_game+0x14b*uVar19+0x1b63;
  memcpy(&L.local_1ac,g_game+0x37e3f,33);
  cVar3 = *(char*)(g_game+0x14b*uVar19+0x1ca9);
  L.local_1ac = cVar3;
  lVar25 = (int)L.local_1ab;
  lVar26 = ((int)*(float*)(iVar12+0x8c));
  iVar21 = (int)lVar26 - (int)lVar25;
  if (iVar21 < 0) {
    iVar21 = iVar21 / 8;
    do {
        if (iVar21 == 0) {
          iVar21 = -1;
        }
    } while (0);
  }
  else if (iVar21 > 0) {
    iVar21 = iVar21 / 8;
    if (iVar21 == 0) {
      iVar21 = 1;
    }
  }
  else {
    iVar21 = 0;
  }
  L.local_1ab = (float)(iVar21 + (int)lVar25);
  lVar25 = (int)L.local_19f;
  lVar26 = (int)*(float*)(iVar12+0x98);
  iVar21 = (int)lVar26 - (int)lVar25;
  if (iVar21 < 0) {
    iVar21 = iVar21 / 8;
    if (iVar21 == 0) {
      iVar21 = -1;
    }
  }
  else if (iVar21 > 0) {
    iVar21 /= 8;
    if (iVar21 == 0) {
      iVar21 = 1;
    }
  }
  else {
    iVar21 = 0;
  }
  L.local_193 = *(float *)(iVar12 + 0xa4);
  L.local_18f = *(float *)(iVar12 + 0xa8);
  L.local_19f = (float)((int)lVar25 + iVar21);
  if (L.local_1ab > L.local_193) {
    L.local_1ab = L.local_193;
  }
  if (L.local_19f > L.local_18f) {
    L.local_19f = L.local_18f;
  }
  if (*(uint *)(((int)iVar12) + 0xf8) < *(uint *)(iVar11 + 0x38a47)) {
    *(uint *)(iVar12 + 0xf8) = *(uint *)(iVar12 + 0xf8) + 0x1e;
    fVar24 = FUN_00464ab0((int)(iVar12));
    L.local_1a7 = (float)fVar24;
    fVar24 = FUN_00464ac0((int)(iVar12));
    L.local_1a3 = (float)fVar24;
    fVar24 = FUN_00464af0((int)(iVar12));
    L.local_19b = (float)fVar24;
    fVar24 = FUN_00464b00((int)(iVar12));
    L.local_197 = (float)fVar24;
    iVar11 = (int)g_game;
  }
  if (memcmp(g_game+0x37e3f,&L.local_1ac,33) != 0) {
    memcpy(0x37e3f+g_game,&L.local_1ac,33);
    uVar19 = (uint)*(byte *)(*(int *)(iVar12 + 0x27) + 0x95);
    iVar11 = (int)g_game + 0x37f3d + uVar19 * 0x232;
    FUN_004c1420((int)(*(int *)(0x22e + iVar11)));
    FUN_004c1450();
    L.local_208 = (ushort *)((int)g_game + 0xdcb);
    iVar8 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)((byte *)L.local_208)[0xf]),(int)(iVar8));
    iVar21 = 0x81;
    do {
      puVar9 = (ushort *)
               FUN_004b7f30((int)(*(ushort **)
                             (83999 + (int)g_game + (uVar19 + (uint)(0x81 < iVar21) * 5) * 4)),(int)(0));
      FUN_00467a20((int)(L.local_1f0),(int)(puVar9),(int)(iVar21),(int)(0));
      iVar21 = iVar21 + (uint)*puVar9;
    } while (iVar21 < *(int *)((int)g_game + 0x37e1f));
    FUN_00467c00((int)L.local_1f0,iVar12,iVar11+0x42,0);
    OverlayRect *pR = (OverlayRect *)(iVar11 + 0x52);
    *(OverlayRect *)&L.local_188 = *((OverlayRect *)(iVar11 + 0x52));
    if (*(float *)(iVar12 + 0xa4) > 0.0f) {
      lVar25 = (int)(((int)L.local_180-(int)L.local_188)*L.local_1ab / *(float*)(iVar12+0xa4)+(int)L.local_188);
      L.local_180 = (undefined4)lVar25;
      FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_188),(int)(*(int *)(iVar11 + 0x222)));
      if ((*(float *)(iVar12 + 0xe8) > 0.0f)) {
          if ((*(float *)(iVar12 + 0x8c) > *(float *)(iVar12 + 0xe8))) {
            *(OverlayRect *)&L.local_200 = *pR;
            pfZoomY = (float*)(iVar12+0xe8);
            lVar25 = (int)(((int)L.local_1f8-(int)L.local_200)* *pfZoomY / *(float*)(iVar12+0xa4)+(int)L.local_200);
            L.local_200 = (uint)lVar25;
            L.local_1f8 = L.local_200 + 2;
            puVar9 = L.local_208;
            FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_200),(int)(*(byte *)((int)puVar9 + 0xc)));
            goto LAB_join1;
          }
      }
    }
    puVar9 = L.local_208;
LAB_join1:
    sprintf((char*)L.local_170,"%d",(int)L.local_1ab);
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x62)),(int)(*(int *)(iVar11 + 0x66)),(int)(-1));
    FUN_004c14f0((int)(L.local_1f0),(int)("0"),(int)(*(int *)((((int)iVar11) + 0xd2))),(int)(*(int *)(iVar11 + 0xd6)),(int)(-1));
    sprintf((char*)L.local_170,"%d",(int)*(float*)(iVar12+0xa4));
    iVar21 = FUN_004c1480((int)(*(int *)(iVar11 + 0x22e)),(int)(L.local_170));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xb2) - iVar21),(int)(*(int *)(iVar11 + 0xb6)),(int)(-1));
    if (L.local_1a7 > 99999.0f) sprintf((char*)L.local_170,"%dK",(int)L.local_1a7/1000);
    else sprintf((char*)L.local_170,"%d",(int)L.local_1a7);
    iVar21 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 10)),(int)(iVar21));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xf2)),(int)(*(int *)(iVar11 + 0xf6)),(int)(-1));
    if (L.local_1a3 < -99999.0f) sprintf((char*)L.local_170,"%dK",(int)L.local_1a3/1000);
    else sprintf((char*)L.local_170,"%d",abs((int)L.local_1a3));
    iVar21 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xc)),(int)(iVar21));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x102)),(int)(*(int *)(iVar11 + 0x106)),(int)(-1));
    pR2 = (OverlayRect *)(iVar11 + 0x72);
    *(OverlayRect *)&L.local_188 = *((OverlayRect *)(iVar11 + 0x72));
    if (*(float *)(iVar12 + 0xa8) > 0.0f) {
      lVar25 = (int)((int)L.local_188+L.local_19f*((int)L.local_180-(int)L.local_188) / *(float*)(iVar12+0xa8));
      L.local_180 = (undefined4)lVar25;
      FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_188),(int)(*(int *)(iVar11 + 0x226)));
      if ((*(float *)(iVar12 + 0xe4) > 0.0f)) {
          if ((*(float *)(iVar12 + 0x98) > *(float *)(0xe4 + iVar12))) {
            *(OverlayRect *)&L.local_200 = *pR2;
            lVar25 = (int)(((int)L.local_1f8-(int)L.local_200)* *(float*)((iVar12+0xe4)) / *(float*)(iVar12+0xa8)+(int)L.local_200);
            L.local_200 = (uint)lVar25;
            L.local_1f8 = L.local_200 + 2;
            FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_200),(int)(*(byte *)((int)puVar9 + 0xc)));
          }
      }
    }
    iVar12 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xf)),(int)(iVar12));
    sprintf((char*)L.local_170,"%d",(int)L.local_19f);
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x82)),(int)(*(int *)(iVar11 + 0x86)),(int)(-1));
    FUN_004c14f0((int)(L.local_1f0),(int)("0"),(int)(*(int *)(iVar11 + 0xe2)),(int)(*(int *)(iVar11 + 0xe6)),(int)(-1));
    sprintf((char*)L.local_170,"%d",(int)*(float*)(iVar12+0xa8));
    iVar12 = FUN_004c1480((int)(*(int *)(iVar11 + 0x22e)),(int)(L.local_170));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xc2) - iVar12),(int)(*(int *)(iVar11 + 0xc6)),(int)(-1));
    sprintf((char*)L.local_170,"%.1f",L.local_19b);
    iVar12 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 10)),(int)(iVar12));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x112)),(int)(*(int *)(iVar11 + 0x116)),(int)(-1));
    sprintf((char*)L.local_170,"%.1f",fabs(L.local_197));
    iVar12 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xc)),(int)(iVar12));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x122)),(int)(*(int *)(iVar11 + 0x126)),(int)(-1));
  }
  FUN_0046a860((int)(L.local_1f0));
  FUN_00466b00((int)(L.local_1f0));
  ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  iVar11 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[14] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  L.local_1b4 = *(byte *)(g_game+0x2a43);
  iVar12 = (int)g_game + (uint)L.local_1b4 * 0x14b;
  iVar11 = iVar12 + 0x1b63;
  FUN_004c1420((int)(*(int *)((int)g_game + 0x3816b +
                       (uint)*(byte *)(*(int *)(iVar11 + 0x27) + 0x95) * 0x232)));
  FUN_004c13a0((int)((uint)*(byte *)((int)g_game + 0xdda)),(int)FUN_004c13f0());
  iVar13 = 0;
  char* pcView;
  pcView = g_game;
  iVar12 = *(int *)((int)pcView + 0x1431f) / 16;
  iVar21 = *(int *)((int)g_game + 0x14323) / 16;
  mv = (MapGrid *)((int)g_game + 0x141fb);
  if (0 < mv->f54) {
      do {
          iVar14 = mv->f50 * iVar13;
          iVar13 = iVar13 + 1;
          *(int *)(mv->f4 + -4 + iVar13 * 4) = mv->f0 + iVar14 * 4;
          *(undefined2 *)(mv->f8 + -2 + iVar13 * 2) = 0;
        } while (iVar13 < mv->f54);
  }
  L.local_210 = mv->f54;
  L.local_1b8 = iVar21 - 0x10;
  if (L.local_1b8 < 0) {
    L.local_210 = L.local_210 + L.local_1b8;
    L.local_1bc = -L.local_1b8;
    L.local_1b8 = 0;
  }
  else {
    L.local_1bc = 0;
  }
  if (L.local_210 + L.local_1b8 > mv->f3c + -1) {
    L.local_210 = (mv->f3c - L.local_1b8) + -1;
  }
  L.local_204 = mv->f50;
  L.local_208 = (ushort *)(iVar12 - 10);
  if ((int)L.local_208 < 0) {
    L.local_204 = L.local_204 + (int)L.local_208;
    L.local_208 = (ushort *)0x0;
  }
  if (L.local_204 + (int)L.local_208 > mv->f38 + -1) L.local_204 = (mv->f38 - (int)L.local_208) + -1;
  puVar9 = *(ushort **)((int)g_game + 0x1435f);
  L.local_20c = 0;
  iVar12 = (int)g_game;
  if (0 < *(int *)((int)g_game + 0x14367)) {
    do {
      uVar6 = (int)*(ushort *)puVar9;
      iVar21 = *(int *)(iVar12 + 0x14357);
      iVar13 = (int)*(short *)(((int)iVar21) + 0x74 + 0x118 * (uint)uVar6) - *(int *)(iVar12 + 0x14323);
      iVar13 = iVar13 / 16 + 0x10;
      if ((iVar13 >= 0)) {
          if ((iVar13 < mv->f54)) {
            piVar16 = (int *)(iVar13 * 4 + mv->f4);
            psVar2 = ((short *)(mv->f8 + iVar13 * 2));
            *psVar2 = *psVar2 + 1;
            iVar12 = (int)g_game;
            if ((int *)0x0 != (int *)*piVar16) {
              *(int *)*piVar16 = iVar21 + (uint)uVar6 * 0x118;
              *piVar16 = *piVar16 + 4;
              iVar12 = (int)g_game;
            }
          }
      }
      L.local_20c = L.local_20c + 1;
      puVar9 = ((ushort*)puVar9) + 1;
    } while (L.local_20c < *(int *)(iVar12 + 0x14367));
  }
  FUN_00471f90((int)(L.local_1f0),(int)(0));
  FUN_00471f90((int)(L.local_1f0),(int)(1));
  FUN_00471f90((int)(L.local_1f0),(int)(2));
  if (0 < L.local_210) {
    L.local_1c0 = L.local_210;
    iVar12 = L.local_1b8;
    for (;;) {
      do {
          iVar21 = 0xd * ((int)(mv->f38 * iVar12 + (int)L.local_208)) +
                   mv->f8c;
          if (L.local_204 > 0) {
            L.local_20c = L.local_204;
            puVar9 = L.local_208;
            do {
              *(byte *)(iVar21 + 0xc) = *(byte *)(iVar21 + 0xc) & 0xfb;
              if (*(ushort *)(8 + iVar21) < 0xfffb) {
                uVar15 = (uint)*(ushort *)(iVar21 + 8);
                iVar13 = *(int *)((int)g_game + 0x1426f) + uVar15 * 0x100;
                do {
                    if (*(byte *)(iVar13 + 0xfa) < 10) {
                      if (((*(byte *)(iVar13 + 0xff) & 8) == 0) ||
                         ((*(byte *)(iVar21 + 0xc) >> 3 & 0xf) == L.local_1b4)) {
                        FUN_0046a610((int)(L.local_1f0),(int)(iVar21),(int)((int)puVar9),(int)(iVar12));
                      } else {
                        bVar23 = FUN_004658e0(((int)(iVar11)),(int)puVar9,(int)iVar12,(int)(*(short *)(iVar13 + 0x94)),(int)(*(short *)(iVar13 + 0x96)),(int)((ushort)*(byte *)(iVar21 + 4)));
                        bDrew = bVar23 != 0;
                        if (bDrew) {
                          FUN_0046a610((int)(L.local_1f0),(int)(iVar21),(int)((int)puVar9),(int)(iVar12));
                        }
                      }
                    }
                    else {
                      *(byte *)(0xc + iVar21) = *(byte *)(iVar21 + 0xc) | 4;
                    }
                } while (0);
              } else {
              }
              L.local_20c = L.local_20c + -1;
              puVar9 = (ushort *)((int)puVar9 + 1);
              iVar21 = iVar21 + 0xd;
            } while (L.local_20c != 0);
          }
          L.local_1c0 = L.local_1c0 + -1;
      } while (0);
      iVar12 = iVar12 + 1;
        if (L.local_1c0 == 0)
            break;
    }
  }
  FUN_00471f90((int)(L.local_1f0),(int)(3));
  FUN_00471f90((int)(L.local_1f0),(int)(4));
  iVar12 = L.local_210;
  if (0 < L.local_210) {
    L.local_1c0 = iVar12;
    L.local_210 = L.local_1b8;
    L.local_1b8 = L.local_1bc - L.local_1b8;
    L.local_20c = L.local_1bc * 2;
    do {
      iVar12 = 0;
      piVar16 = (int *)(mv->f0 + (L.local_1b8 + L.local_210) * mv->f50 * 4);
      if (!(0 < *(ushort *)(L.local_20c + mv->f8))) {
      } else {
        do {
          iVar21 = *piVar16;
          pF = (UnitFlags *)(iVar21 + 0x110);
          do {
              if ((pF->kind == 1) && (param_1 != 0)) {
                if (pF->b4) {
                  FUN_0046a530((int)(L.local_1f0),(int)(iVar21));
                }
                if (*(int *)(iVar21 + 0x9a) != 0) {
                  FUN_0045ac20((int)(L.local_1f0),(int)(iVar21));
                }
              }
              iVar12 = iVar12 + 1;
          } while (0);
          piVar16 = piVar16 + 1;
        } while (iVar12 < (int)(uint)*(ushort *)(L.local_20c + mv->f8));
      }
      iVar12 = (int)(L.local_210 * mv->f38 + (int)L.local_208) * 0xd +
               mv->f8c;
      if (L.local_204 > 0) {
        L.local_1bc = L.local_204;
        puVar9 = L.local_208;
        do {
          iVar21 = L.local_210;
          if (((*(byte *)(iVar12 + 0xc) & 4) != 0) &&
             (((uVar15 = (uint)*(ushort *)(iVar12 + 8),
               iVar13 = uVar15 * 0x100 + *(int *)((int)g_game + 0x1426f),
               (*(byte *)(iVar13 + 0xff) & 8) == 0 ||
               ((*(byte *)(iVar12 + 0xc) >> 3 & 0xf) == L.local_1b4)) ||
              (bVar23 = FUN_004658e0((int)(iVar11),(int)puVar9,(int)L.local_210,(int)(*(short *)(((int)iVar13) + 0x94)),(int)(*(short *)(iVar13 + 0x96)),(int)((ushort)*(byte *)(iVar12 + 4))),
              bVar23 != 0)))) {
            FUN_0046a610((int)(L.local_1f0),(int)(iVar12),(int)((int)puVar9),(int)(iVar21));
          }
          puVar9 = (ushort *)((int)puVar9 + 1);
          iVar12 = iVar12 + 0xd;
          L.local_1bc = L.local_1bc + -1;
        } while (L.local_1bc != 0);
      }
      L.local_20c = L.local_20c + 2;
      L.local_210 = L.local_210 + 1;
      L.local_1c0 = L.local_1c0 + -1;
    } while (L.local_1c0 != 0);
  }
  FUN_00471f90((int)(L.local_1f0),(int)(5));
  bool bVisible = param_1 != 0;
  if (bVisible) {
    FUN_00471f90((int)(L.local_1f0),(int)(6));
    FUN_0049be60((int)(L.local_1f0));
    FUN_00420b00((int)(L.local_1f0));
    FUN_00471f90((int)(L.local_1f0),(int)(7));
    L.local_204 = 0;
    if (0 < mv->f54) {
      for (;;) {
        piVar16 = (int *)(mv->f0 + mv->f50 * L.local_204 * 4);
        iVar12 = 0;
        if (0 < *(ushort *)(mv->f8 + L.local_204 * 2)) {
          do {
            iVar21 = *piVar16;
            pF = (UnitFlags *)(iVar21 + 0x110);
            if (pF->kind != 1) {
              if (pF->b4) {
                FUN_0046a530((int)(L.local_1f0),(int)(iVar21));
              }
              if (*(int *)(iVar21 + 0x9a) != 0) {
                FUN_0045ac20((int)(L.local_1f0),(int)(iVar21));
              }
            }
            iVar12 = iVar12 + 1;
            piVar16 = piVar16 + 1;
          } while (iVar12 < (int)(uint)*(ushort *)(L.local_204 * 2 + mv->f8));
        }
        L.local_204 = L.local_204 + 1;
          if (L.local_204 >= mv->f54)
              break;
      }
    }
  }
  FUN_00471f90((int)(L.local_1f0),(int)(8));
  bVar23 = FUN_004c1b80((int)(0xf9));
  if (bVar23 != 0) {
    FUN_0048cc30((int)(L.local_1f0),(int)((uint *)((int)g_game + 0x142f3)));
  }
  if (param_1 != 0) {
    L.local_20c = 0;
    L.local_208 = *(ushort **)((int)g_game + 0x1435f);
    iVar11 = (int)g_game;
    if (*(int *)((int)g_game + 0x14367) > 0) {
      for (;;) {
        do {
            iVar12 = *(int *)(((int)iVar11) + 0x14357) + (uint)*L.local_208 * 0x118;
            if (((*(byte *)(iVar11 + 0x37f06) & 1) != 0) || (*(int *)(iVar12 + 0xac) != 0)) {
              *((byte *)&L.local_210 + 1) = 0;
              iVar21 = 0x80 + ((int)*(short *)(0x6c + iVar12) - *(int *)(iVar11 + 0x1431f));
              iVar8 = 0x20 + (((int)*(short *)(iVar12 + 0x74) - *(int *)(iVar11 + 0x14323)) -
                      ((int)*(short *)(0x70 + iVar12) >> 1));
              if ((*(byte *)(iVar11 + 0x37f06) & 1) != 0) {
                cVar3 = L.local_1b4;
                if (*(char *)((*(int *)(iVar12 + 0x96)) + 0x146) != cVar3) {
                } else {
                  FUN_0046a430((int)(L.local_1f0),(int)(iVar12),(int)(iVar21),(int)(iVar8 + 10));
                  iVar11 = (int)g_game;
                }
                char cOwner;
                cOwner = *(char *)(*(int *)(iVar12 + 0x96) + 0x146);
                if ((cOwner == cVar3) &&
                   (0 != *(int *)(iVar12 + 0xac))) {
                  *(byte*)&L.local_210 = *(char *)(iVar12+0xac)+'0';
                  FUN_004c14f0((int)(L.local_1f0),(int)((byte *)&L.local_210),(int)(iVar21),(int)(iVar8 + 0xe),(int)(-1));
                  iVar11 = (int)g_game;
                }
              }
            }
        } while (0);
        L.local_20c = L.local_20c + 1;
        L.local_208 = L.local_208 + 1;
          if (L.local_20c >= *(int *)(iVar11 + 0x14367))
              break;
      }
    }
    FUN_00471f90((int)(L.local_1f0),(int)(9));
  }
  iVar11 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[15] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  if (((*(ushort *)((int)g_game + 0x3923b) & 1) != 0)) {
      if (((2 & *(ushort *)((int)g_game + 0x3923b)) != 0)) {
        if (0 == param_1) goto LAB_00469d93;
        piVar10 = (int *)FUN_0048c190((int)(0),(int)(0));
        FUN_00417f30((int)(L.local_1f0),(int)(piVar10));
      }
  }
  if (param_1 != 0) {
    FUN_004848e0((int)(L.local_1f0));
  }
LAB_00469d93:
  iVar11 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[16] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  if (((int)param_1) == 0) bVar23 = false;
  else if ((*(byte *)(0x2cc6 + (int)g_game) & 8) != 0) {
    bVar23 = true;
  }
  else if (*(char *)((int)g_game + 0x2cc3) == '') {
    bVar23 = FUN_004b6720((int)((int *)((int)g_game + 0x37e27)),(int)(*(int *)((int)g_game + 0x2c76)),(int)(*(int *)((int)g_game + 0x2c7a))) != 0;
  }
  else {
    bVar23 = false;
  }
  if (bVar23 != 0) {
    iVar21 = (*(int *)((int)g_game + 0x2c92) - *(int *)((int)g_game + 0x1431f)) + 0x80;
    iVar11 = ((*(int *)((int)g_game + 0x2c9a) - (*(int *)((int)g_game + 0x2c96) >> 1)) -
             *(int *)((int)g_game + 0x14323)) + 0x20;
    iVar13 = (*(int *)((int)g_game + 0x2c9e) - *(int *)((int)g_game + 0x1431f)) + 0x80;
    int iRectBottom = ((*(int *)((int)g_game + 0x2ca6) - (*(int *)((int)g_game + 0x2ca2) >> 1)) -
                      *(int *)((int)g_game + 0x14323)) + 0x20;
    iVar14 = iRectBottom;
    if (*(char *)((int)g_game + 0x2cc3) == '') iVar12 = (((0x40 & *(byte *)((int)g_game + 0x2cc6)) ? 6 : 0) + 4); else iVar12 = 0xf;
    uVar15 = (uint)L.local_1b0[iVar12];
    if (iVar13 < iVar21) {
      iVar8 = iVar21;
      iVar21 = iVar13;
      iVar13 = iVar8;
    }
    if (iVar14 < iVar11) {
      iVar8 = iVar11;
      iVar11 = iVar14;
      iVar14 = iVar8;
    }
    L.local_200 = iVar21;
    L.local_1fc = iVar11;
    L.local_1f8 = iVar13;
    L.local_1f4 = iVar14;
    FUN_004bf8c0((int)(L.local_1f0),(int)(&L.local_200),(int)(uVar15));
    L.local_200 = L.local_200 + 1;
    L.local_1fc = L.local_1fc + 1;
    L.local_1f8 = L.local_1f8 - 1;
    L.local_1f4 = L.local_1f4 + -1;
    if (*(char *)((int)g_game + 0x2cc3) == '\x0e') {
      FUN_004bf8c0((int)(L.local_1f0),(int)(&L.local_200),(int)(uVar15));
    }
    else {
      FUN_004bf8c0((int)(L.local_1f0),(int)(&L.local_200),(int)((uint)*L.local_1b0));
    }
  }
  iVar11 = ((Class_00435100*)(*(undefined4 **)((int)g_game + 0x391e9)))->FUN_00435100();
  if ((iVar11 == 3) ||
     (iVar11 = ((Class_00435100*)(*(undefined4 **)((int)g_game + 0x391e9)))->FUN_00435100(), iVar11 == 2)) {
    FUN_004c69c0((int)((int *)L.local_1f0));
    FUN_004948e0((int)((int *)L.local_1f0));
    ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  }
  FUN_004689c0((int)(L.local_1f0));
  if (*(int *)((int)g_game + 0x391c3) != 0) {
    FUN_00468380((int)(L.local_1f0));
  }
  if (param_1) {
    FUN_00464060((int)(L.local_1f0));
  }
  if (((*(byte *)((int)g_game + 0x3923b) & 2) != 0)) {
      if ((param_1 != 0)) {
        iVar11 = FUN_004c13f0();
        FUN_004c13a0((int)((uint)L.local_1b0[0xf]),(int)(iVar11));
        FUN_004c1420((int)(*(int *)((int)g_game + 0x391f9)));
        uVar6 = FUN_004c1450();
        iVar11 = 3 * uVar6 + -10;
        sprintf((char*)L.local_150,"FRATE: %d\n",FUN_004b66a0());
        FUN_004c14f0((int)(L.local_1f0),(int)(L.local_150),(int)(0x83),(int)(iVar11),(int)(-1));
        FUN_004c14f0((int)(L.local_1f0),(int)((byte *)"[Release]"),(int)(0xbc),(int)(iVar11),(int)(-1));
        sprintf((char*)L.local_150,"MODE %s INFO %s",((Bits8 *)((int)g_game+0x3923b))->b1?"DEBUG":"NORMAL",((Bits8 *)((int)g_game+0x3923b))->b0?"ON":"OFF");
        FUN_004c14f0((int)(L.local_1f0),(int)(L.local_150),(int)(0x1ee),(int)(iVar11),(int)(-1));
        iVar11 += FUN_004c1450();
        iDebugFlags = (int)g_game + 0x2a44;
        if ((*(byte *)(iDebugFlags) & 1) != 0) {
          FUN_00415fa0((int)(L.local_150));
          FUN_004c14f0((int)(L.local_1f0),(int)(L.local_150),(int)(0xbc),(int)(iVar11),(int)(-1));
        }
      }
  }
  iVar11 = L.local_178;
  iVar12 = L.local_174;
  if ((*(byte *)((int)g_game + 0x38a51) & 1) != 0) {
      FUN_004b7f90((int)(L.local_1f0),(int)(FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x1481b)),(int)(0))),(int)(iVar11),(int)(iVar12));
  }
  if ((*(byte *)(*(int *)((int)g_game + (uint)*(byte *)(0x2a42 + (int)g_game) * 0x14b + 0x1b8a) +
                0x9b) & 0x40) == 0) {
    if (((Bits8 *)((int)g_game + 0x3923b))->b5) {
          FUN_004b7f90((int)(L.local_1f0),(int)(FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x14813)),(int)(0))),(int)(iVar11),(int)(iVar12));
        }
        if (((Bits8 *)((int)g_game + 0x3923b))->b6) {
          FUN_004b7f90((int)(L.local_1f0),(int)(FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x14817)),(int)(0))),(int)(iVar11),(int)(iVar12));
        }
  }
  if (((Bits8 *)((int)g_game + 0x37f2f))->b6) {
    uint ticks=*(uint*)(g_game+0x38a47), hours=ticks/108000;
    int rest=ticks-hours*108000;
    int minutes;
    minutes = rest/1800;
    int seconds=(rest-(rest/1800)*1800)/30;
    sprintf((char*)L.local_100,"%s : %02d:%02d:%02d",(char*)FUN_004c5740((int)"Game Time"),hours,minutes,seconds);
    iVar11 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)L.local_1b0[0xf]),(int)(iVar11));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_100),(int)(0x82),(int)(-0x22 - FUN_004c1450() + FUN_004b6710()),(int)(-1));
  }
  if (((Bits8 *)((int)g_game + 0x38a51))->b1) {
    FUN_004b7f90((int)(L.local_1f0),(int)(FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x148cf)),(int)(0))),(int)(*(int *)((int)g_game + 0x37e1f) + -0x10),(int)(*(int *)((int)g_game + 0x37e23) + -0x50));
  }
  FUN_004c69c0((int)((int *)L.local_1f0));
  FUN_004ab170((int)((int)g_game + 0x519),(int)(L.local_1f0),(int)((int *)(0x37e27 + (int)g_game)));
  if ((*(int *)(0x38dd5 + (int)g_game) != 0) && (param_1 != 0)) {
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Network"),(int)(0));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Units"),(int)(1));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Logic"),(int)(2));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Render Static"),(int)(3));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Render Stuff"),(int)(4));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Render Fog"),(int)(5));
    FUN_0046b900((int)(L.local_1f0),(int)("SFX"),(int)(6));
    FUN_0046b900((int)(L.local_1f0),(int)(((byte *)"Weapon")),(int)(7));
    FUN_0046b900((int)(L.local_1f0),(int)("Misc"),(int)(8));
  }
  FUN_0045ffb0((int)L.local_1f0);
  FUN_004c2870();
  if ((param_1 != 0) && (param_2 != 0)) {
    FUN_004c63a0();
  }
  ((Class_0046a400*)((void *)((int)g_game + 0x38d85)))->FUN_0046a400((int)(3));
  return;
}