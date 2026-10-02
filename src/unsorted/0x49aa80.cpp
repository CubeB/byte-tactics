// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by Space Bunny Free.
// Names are provisional.
//
// 30-MIN CHECKPOINT (space-bunny-free, 2026-10-02): best is still 68.7%, the same
// version that shipped, so nothing to flush. ~90 scratch variants scored this
// pass, all free with check.py --sym, and NOTHING beat 68.7%: the full 32-way
// matrix over (which array index holds each difference) x (subtraction
// direction) x (statement order) x (sum order) tops out at this spelling;
// named __int64 locals (61.8%), a {__int64 dz, dx;} struct (68.7% only in the
// x-first statement order), int temps, named products, `Vec3* p2 = a2` pointer
// and `Vec3& v = *a2` reference copies, const parameters, tail differences
// named or inlined through accessors or a two-pointer helper, a by-value
// Vec3 tail, moving the weapon lookup later (39.8%), a by-value struct tail
// (35.3%), and every dead-store / Id() identity placement tried before all
// come out byte-IDENTICAL to this file at 68.7%, i.e. they are the same code
// and not the lever. Read with the slot-resolving annotator in
// build/scratch/0x49aa80/annot2.py (which prints every [esp+N] as an absolute
// slot from the entry esp and models _allmul as ret 0x10, _allshr as cdecl),
// the original's first block is:
//   mov esi, a2 / mov edi, a3 at the TOP of the prologue (interleaved with the
//   four register pushes), a2->x hoisted into ebp, the Z subtraction issued
//   FIRST, then the X one, dx.lo promoted to ebp with dx.hi spilled to [E-4],
//   and dz.hi spilled LATE (after the four argument pushes). Our build loads
//   a3 into edi in the same place but gives esi to the temporary a2->x, keeps
//   BOTH dx halves in registers (ecx and ebp), reloads a2 from [E+8] and hoists
//   a3->x into the dead a4 home. So the whole residual is the same single
//   register decision the older notes describe: which value gets esi.
// NEW TOOLING in build/scratch/0x49aa80/ (not committed): annot2.py (absolute
// slots, see above), mk.py (`submany spec.json` applies a list of old/new
// source substitutions to this file and scores every one with check.py --sym;
// `sweep spec.json` writes a variant body and scores it), gen2.py (the 32-way
// matrix), plus the json specs s1..s23 and every generated .cpp/.obj/.asm.
//
// FINAL (space-bunny-free, 2026-10-02, ~55 min, ~150 scratch variants, 2 real
// check.py runs): best is STILL 68.7% and the shipped source is unchanged, so
// this pass found no new best. Everything below is measured, not guessed.
// The permuter (seed 11, 8 min, 2538 candidates) improved three intermediate
// candidates but finished at the same 68.7% / 310 bytes, and its best.diff is
// empty, so there is nothing to adopt from it.
// NEWLY REFUTED IN THIS PASS (each compiled byte-identically to this file at
// 68.7%, i.e. the same code, so none of them is the lever):
//   * every dead-store shape again, but placed exactly at the rematerialisation
//     point: `int t = 0; if (t) wdef = wdef;` between the distance compare and
//     the flag test, `if (t) d[0] = 0;` between the blocks and after the two
//     _allmul calls, a dead `for (k = 0; k < 0; k++)`, a `while (t)`, a dead
//     local array element, `if (wdef->flags.value & 0) t = 1;`. A NULL-pointer
//     check on a2 is NOT inert (49.8%, 319 bytes) and a dead array element is
//     not either (48.7%, 329 bytes), so a dead store has to fold completely
//     away to be inert;
//   * the trivial-identity wrapper, on the a2 POINTER (`Vec3* q = Idp(a2)`
//     used in all three blocks), on the three tail differences, and on the
//     distance sum (`Id((int)(...) + ...)` is 37.6%, 348 bytes, so wrapping the
//     sum is NOT inert while wrapping the pointer is);
//   * the compiler-state sweep: N = 0..8 dummy inline functions in front all
//     score 68.7% except N = 8, which drops to 67.0%, so there is no state
//     lever here either;
//   * the include sweep: <windows.h>, <math.h>, <stdlib.h>, <stdio.h>,
//     <memory.h>, <ctype.h> all 68.7%; <string.h>, <limits.h>, <assert.h>
//     67.0%. The older note that <math.h> flips the height check's load order
//     does not reproduce through a plain include swap any more;
//   * inline accessors per component (`X_(a2) - X_(a3)`) in the tail, in the
//     height check, in the distance block, or in all of them at once, and
//     per-component helpers in the first block;
//   * the weapon lookup shapes: `&a1->weapons[slot]`, a `slot` local, a `u`
//     unit-pointer local in both blocks, and a `Wdef_(a1, slot)` helper;
//   * extra genuine uses of a2/a3 that fold away (an `early` local added to
//     the height sum, three uses of one field, one extra use before or after
//     the distance block): all worse (39.7% to 50.0%), so this is not a
//     one-more-use tie in the direction the older notes suggest;
//   * moving the `if (wdef->flags.bit1)` body out of its own if with an
//     equivalent early `return 1` (56.1%, 322 bytes).
// TWO THINGS A LATER PASS SHOULD KNOW. (1) m_zi0_dxx_dzx_second_second scores
// 64.4% and comes out of the matrix with frame 0x10 and the right slots but
// computes the Z subtraction FIRST, which is the original's order; it loses only
// on which operand MSVC keeps in a register. So "assign z first" and "put z in
// the low frame pair" really are in tension for a two-element __int64 array,
// and the array spelling cannot satisfy both at once. (2) The original's
// prologue loads a2 and a3 into esi and edi interleaved with the four register
// pushes (mov esi between push esi and push edi), which is MSVC's "copy the
// parameters into callee-saved registers at entry" shape; it only happens when
// a parameter must survive a call AND the register is still free, so it is the
// same esi decision seen from the other side, not a separate difference.
//
// WRAP-UP (space-bunny-free, 2026-10-02): best 68.7%, 2 real check.py runs,
// ~150 free check.py --sym variants, no new best. WHAT STILL DIFFERS is one
// register decision in the first basic block: the original holds a2 in esi and
// a3 in edi across both __allmul calls (loads interleaved with the four
// register pushes in the prologue, a2->x hoisted into ebp, Z subtraction
// issued first, dx.lo in ebp with dx.hi spilled to [E-4]); our build gives esi
// to the temporary a2->x, reloads a2 from [E+8] three times, keeps BOTH dx
// halves in registers (ecx and ebp), and hoists a3->x into the dead a4 home at
// [E+16]. Everything from the distance compare onward matches.
// LEADS FOR THE NEXT ATTEMPT, in the order I would try them:
// 1. The Z-first tension is the real constraint, not a spelling accident. For
//    `__int64 d[2]` the low frame pair belongs to whichever element is assigned
//    SECOND, so "z in the low pair" needs z second while "z subtracted first"
//    needs z first, and no matrix member gives both. Find a THIRD container for
//    the two 64-bit values whose slot order and evaluation order are
//    independent: a union of a __int64[2] with a struct of two named members
//    (index through one, name through the other, use only the named side in
//    the sum) is the obvious untried shape, as is `d[3]` with the z difference
//    at index 1 or 2 so the unused index absorbs the low pair.
// 2. Permuter seed sweep: this pass only ran seed 11 (8 min, 2538 candidates,
//    no gain). Seeds 12, 13 and a longer run are still untried, and the seed
//    demonstrably decides the outcome on other addresses.
// 3. The dead-store and identity levers are both exhausted for source that
//    keeps the four arguments as plain pointers; what has NOT been tried is
//    dead-store or identity applied to a source that first defeats the
//    z-first constraint (lead 1), since every attempt here combined the two and
//    could not separate them.
// 4. The one place the array forces MSVC's hand that was never isolated: the
//    sum's first term. m_zi0_dxx_dzx_second_second (z assigned first, z squared
//    first, 64.4%) has the original's instruction order but the wrong
//    register; adding an inline helper around ONE of the two squares in that
//    variant, rather than in this one, is untested.
//
// SEMANTIC AUDIT (deepseek-v4.1-flash, 2026-10-01): CONFIRMED ALLOCATOR
// ARTIFACT, not a missing use. The original reads NO a2/a3 field and makes no
// other use of the pointer values after the second _allmul call that this
// source does not reproduce. Complete memory-operand list of the original's
// post-call region (0x49aaf1..end) and where each read lives in this source:
//   [ebx+0xdc]                wdef->range          -> wdef->range * wdef->range
//   [ebx+0x111]               wdef->flags          -> flags.bit16 / flags.bit1
//   [esp+0x24]                a1 argument home     -> a1->def
//   [0x511de8]                g_game               -> g_game->field_1427f
//   [esi+6]                   a2->y.parts.whole    -> height check
//   [edx+0x92] / [edx+0x170]  a1->def / def->field_170 -> height check
//   [ebp+0x1427f]             g_game->field_1427f  -> height check
//   [edi]/[esi]/[edi+4]/[esi+4]/[edi+8]/[esi+8]   a3->x, a2->x, a3->y,
//                             a2->y, a3->z, a2->z  -> FUN_0049a890 arguments
//   [ebx+0xc8] / [ebx+0x68]   wdef->field_c8 / field_68 -> FUN_0049a890 args
// Nothing else touches memory after the calls, and esi/edi are never tested,
// compared, passed or arithmetically used, only used as load bases (they are
// then clobbered by the loaded values at 0x49ab6d/0x49ab70). Between the two
// _allmul calls the only memory operands are the dz spills [esp+0x10] and
// [esp+0x14]. So the pointers being live across the calls is fully explained
// by post-call reads this file already emits: the residual is purely register
// allocation (original keeps a2 in esi and a3 in edi; ours rematerialises
// both from their argument homes and spills into the dead a4 home).
//
// WHAT STILL DIFFERS (superseded by the pass note at the FUNCTION line below):
// the first basic block's register allocation. Best is now 68.7% (see below);
// the older 66.1% figures in the notes below are superseded.
// TWO CLAIMS IN THE OLDER NOTES BELOW ARE WRONG, corrected by the frame-slot
// analysis in the pass note at the FUNCTION line:
//   * "ours rematerialises BOTH a2 and a3" - ours keeps a3 in edi across the
//     first block; only a2 is rematerialised. That is exactly the asymmetry the
//     original does not have, and it is the whole remaining difference.
//   * "dz-first matches the original's instruction order AND its spill slots
//     (dz at locals +0/+4)" - dz-first matches the instruction order but not
//     the slots: MSVC hands the LOW pair of a two-slot 64-bit pair to whichever
//     is assigned SECOND, so with dz assigned first its halves land in the UPPER
//     pair and the frame grows to 0x14. The original has dz.lo/dz.hi in the low
//     pair [esp+0x10]/[esp+0x14] and dx.hi in [esp+0x1c], which is what this
//     file's d[0] = z spelling now produces.
//
// WHAT STILL DIFFERS (unchanged): register allocation in the first block. The
// original keeps a2 in esi and a3 in edi across both _allmul calls, with
// dx.lo in ebp, dx.hi at [esp+0x1c] and dz.lo/dz.hi at [esp+0x10]/[esp+0x14];
// our build rematerialises a2/a3 from their argument homes. THE STACKED
// SCALARS FAMILY IS NOW REFUTED (deepseek-v4.1-flash, 2026-10-01): naming the
// operand halves as unsigned int pairs (dxLo/dxHi/dzLo/dzHi, either
// declaration order) and rebuilding the 64-bit values from them scores 36.6%
// (the shifts to re-form the values defeat cdq entirely); named product
// locals with their halves read out is 60.1%; a union of __int64 and an
// lo/hi struct pair is 45.0%. All below this file's 66.1%, which stands.
//
// FINAL PASS (deepseek-v4.1-flash, watchdog stop): best stays 66.1% (310 of 301).
// STORE-BACK ANGLE IS REFUTED: the disassembly has NO stores through a2 or a3
// anywhere (every [esi]/[edi] access is a read: a2->x/y/z and a3->x/y/z), so
// a2/a3 are input positions, not out-params, and there are no result stores
// that could keep the pointers live in esi/edi.
// WHAT STILL DIFFERS: register allocation in the first block only. The original
// holds a2 in esi and a3 in edi across both _allmul calls (dx.lo in ebp, dx.hi
// spilled at [esp+0x1c], dz.lo/dz.hi spilled at [esp+0x10]/[esp+0x14]); our
// build rematerialises a2/a3 from their argument homes, spills a3->x into the
// dead a4 home at [esp+0x30] and keeps dz in the d[1] slots (+8/+0xc).
// THE OLD UNTESTED LEAD IS NOW TESTED AND DEAD (deepseek-v4.1-flash): vA
// (d[0] = a3->z - a2->z computed first, d[1] = a3->x - a2->x, dx product
// first) scores 45.9%, vB (same shape with named __int64 dz/dx locals) 45.0%,
// both far below this spelling's 66.1%; even though dz-first matches the
// original's instruction order and its spill slots (dz at locals +0/+4, dx.hi
// at +0xc with dx.lo promoted to ebp), MSVC responds by growing the frame to
// 0x14 and still rematerialises a2. Named product locals (__int64 q0/q1 for
// the two 64-bit products; MSVC 5.0 rejects the `long long` spelling) are
// byte-flat at 66.1%. Everything else in the old notes below stands.
// GPT-6.1-sol retry in #3259: five checker invocations, best remains 66.1%; no MATCH. A line-fire helper scored 59.5%, the height-gate helper 57.4%, and one helper swap failed to compile. Both helpers worsened pointer/register allocation, so the inherited best remains.
// deepseek-v4.1-flash (#3101 retry): still 66.1% (310 of 301). The original keeps
// a2 in esi and a3 in edi across both __allmul calls (frame `sub esp,0x10`, dx.lo
// in ebp, dx.hi and dz spilled); ours rematerialises a2/a3 from their argument homes
// (frame 8, only dz spills). The worktree board's watchdog STOP was honoured; the
// best `__int64 d[2]` version was already flushed.
//
// RETRY (deepseek-v4.1-flash, 2026-10): kept the 66.1% `__int64 d[2]` version.
// A new sweep of spellings confirmed the ceiling: reference parameters for
// a2/a3 (identical code, 66.1), pointer-local copies of a2/a3 either in the
// first block only or throughout (same 66.1, coalesced), an inline
// DistSq(p2,p3) helper with either named or array locals (61.8 and 66.1),
// reading a2->y into a local before the distance test (50.0, reorders the
// frame), and an added `a2 != a3` folding use (51.5) all fail to make MSVC
// keep the a2 pointer in esi. The remaining diff is exactly that: the
// original holds a2 in esi and a3 in edi across both __allmul calls; ours
// reloads them from their argument homes and clobbers edi with a2->z.
// #2400 retry by GPT-6.1-sol: five checks kept the valid 66.1% best. Pointer
// aliases and an inline FireAngle helper did not improve it; other helper
// variants scored lower. The first distance block still differs in register
// allocation and spills around __allmul; see the mismatch notes below.
//
// RETRY RESULT (deepseek-v4.1-flash): best is 66.1%, up from 61.8%, by
// declaring the two differences as a `__int64 d[2]` array instead of two
// named `__int64 dx, dz` locals (see the body). The array makes MSVC emit
// `sub esp,0x10` (the original frame) and matches the first five prologue
// instructions, so the frame and its offsets are now correct. It does NOT
// fix the live lead: the original still keeps the a2 pointer in esi and a3
// in edi across both __allmul calls, while this build puts a2->x in esi and
// re-reads the a2 pointer from its argument home, so a3->x is spilled to an
// argument slot. d[1] assigned before d[0] scores 45.9%, two separate
// initialized locals score 61.8%, `int` temps with casts score 61.8%.
// Everything below is the earlier notes; only the array spelling is new.
// GPT-6.1-sol retry: moving the weapon lookup after the two difference
// assignments drops the score to 39.8%. Explicit register aliases for a2/a3
// compile to the same 66.1% code. Best source remains the array version above.
// GPT-6.1-sol retry #2038: five checks kept 66.1%; aliases and separate
// coordinate locals matched the same code.
//
// FRAME SIZE IS A SYMPTOM, NOT THE LEVER. A later round concluded that the
// 8-byte frame deficit (original `sub esp,0x10`, ours `sub esp,8`) was the cause
// and that forcing the 64-bit `dx` to spill its high word would reach it. That is
// REFUTED by measurement. Frame size is a consequence of which direction the two
// subtractions are written, and every source that reaches frame 0x10 scores
// LOWER:
//
//   dx = a3->x - a2->x, dz = a3->z - a2->z (this file)  -> frame  8, 61.8%
//   dz declared before dx                                 -> frame 12, 60.1%
//   mixed directions (x: a3-a2, z: a2-a3)                 -> frame 16, 53.6%
//   int locals + (__int64) cast in the expression        -> frame 20, 48.5%
//
// So do not chase the frame. A further ~1500 mechanical variants of expression
// shape (subtraction direction x declaration order x product order x cast form x
// wdef position x height-check form x line-of-fire form) all topped out at 61.8%.
//
// THE LIVE LEAD is the `a2` register, not the frame. The original holds a2 in
// esi and a3 in edi across both __allmul calls. This build already gets a3 into
// edi correctly, but re-reads a2 from its stack slot three times. Since the two
// are used symmetrically in the tail, that is a register PRIORITY TIE, and per
// docs/agent-guide.md the fix for a tie is one more genuine use of the loser (or
// one fewer of the winner) in a natural construct. Spelling changes are exhausted;
// the answer is more likely structural, i.e. some helper or reference form.
//
// FREE TOOLING left in build/scratch/0x49aa80/ (not committed):
//   probe.py  - frame size plus which registers a2/a3 land in, from the /Fa
//               listing alone, with NO check.py run. This is the cheap filter:
//               search for shapes where probe reports a2 in esi AND a3 in edi,
//               then score only those.
//   run.py, score.py - normalise an /Fa listing to `opcode operands` (resolving
//               _sym$[esp+N] and N+[esp+K] to absolute displacements) and diff it
//               against a hand-written ref.txt of the original's first block.
//   rscore.py - real check.py --sym scoring for scratch variants, which is free
//               against the run budget.
//   sweep*.py - the mechanical sweeps behind the table above.
//   NOTE: there is no objdump on this machine; these read the /Fa listing, which
//   is why they work.
#include <windows.h>   // inert at 68.7% (tools/headers.py: all 128 sets tie); kept
                       // because it moved the score in the earlier rounds
//
// Partial. Offsets, branches, bit tests, the team/height check and both
// squared-distance multiplies are right; the first basic block does not match.
// The original keeps the weapon def in ebx and the two positions in esi/edi
// across both __allmul calls, spills the high half of dx (0x10 bytes of
// locals) and only reloads the positions in the last block. Our build keeps
// the whole of dx in registers, spills only dz (8 bytes), reloads the
// positions earlier and recomputes the z difference from stack temps.
// Source spellings tried without changing that allocation: int vs __int64
// locals, a named distance, an inlined SquaredDistance helper (both by value
// and by pointer), operator-/Square methods, the array access inlined at each
// use, and several std headers (best 61.8% with <windows.h>).
//
// PURPOSE (read from the callers): a unit can fire the weapon in slot
// (a4 & 0xff) at a point when the ground distance in 16.16 fixed point,
// (dx*dx >> 32) + (dz*dz >> 32), is within the weapon range squared, the
// weapon is not flagged out, the shooter's team/height test passes and the
// line-of-fire helper returns a real angle. Callers pass the unit in a1, the
// unit's own position at a1+0x6a, the target point in a2/a3 and 0 or a slot
// index in a4.
//
// Retry (deepseek-v4.1-flash) confirmed the first block is the only difference
// and that it is a register-assignment problem, not a source-shape one in the
// usual sense. The original keeps a2 in esi and a3 in edi across both __allmul
// calls, leaving ebp as the only free callee-saved register; MSVC then puts
// dx.lo in ebp, spills dx.hi, and spills all of dz. Our build instead
// rematerialises a2/a3 from their stack argument slots, so ecx and ebp are
// free, dx ends up wholly in registers and only dz spills (8-byte frame vs 16).
// tools/headers.py --cpp (768 sets) changes nothing, and a sweep of 0..400
// dummy extern declarations in front of the function (the compiler-state trick
// in the guide) keeps the score pinned at 61.8, so state alone is not it.
// int vs __int64 locals, local pointer and reference copies of a2/a3 used
// throughout, inline DistSq / SquaredDistance helpers by pointer, reference and
// as methods, an inline operator-, reordering declarations so a2 is read first,
// and defining the matched neighbours 0x49a850 and 0x49adf0 in the same file
// were all tried; every one either compiled to the same 61.8 code or worse.

#pragma pack(push, 1)

union Fixed_0049aa80 {
    int value;
    struct { unsigned short fraction; short whole; } parts;
};

struct Vec3_0049aa80 {
    int x;                             // +0x0 (16.16 fixed point)
    Fixed_0049aa80 y;                  // +0x4
    int z;                             // +0x8
};

struct WeaponDef_0049aa80 {
    char unknown_0[0x68];
    int field_68;                      // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                      // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                         // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;         // tested here (line of fire)
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;        // tested here (skip the team check)
        unsigned int bit17_31 : 15;
    } flags;                           // +0x111
};

struct Weapon_0049aa80 {
    char unknown_0[8];
    WeaponDef_0049aa80* def;           // +0x8
    char unknown_c[0x1c - 0xc];
};

struct UnitDef_0049aa80 {
    char unknown_0[0x170];
    short field_170;                   // +0x170
};

struct Unit_0049aa80 {
    char unknown_0[8];
    Weapon_0049aa80 weapons[3];        // +0x8, stride 0x1c
    char unknown_5c[0x92 - 0x5c];
    UnitDef_0049aa80* def;             // +0x92
};

struct Game_0049aa80 {
    char unknown_0[0x1427f];
    unsigned char field_1427f;         // +0x1427f
};

#pragma pack(pop)

extern Game_0049aa80* g_game;

short __stdcall FUN_0049a890(int dx, int dy, int dz, int a, int b);

// FUNCTION: 0x49aa80
// space-bunny-free pass (2026-10-01, 2 check.py runs, ~25 scratch variants): best
// 68.7%, up from 66.1%, from ONE change: index the array the other way round
// (d[1] = the x difference, d[0] = the z difference, and the sum written
// d[1]*d[1] first). That reproduces the original's frame exactly:
//   sub esp,0x10 with dz.lo/dz.hi at [E-0x10]/[E-0xc] and d[1].lo unused,
// which is where the previous d[0]=x/d[1]=z spelling put dz in the UPPER pair.
// Read with a new tool (build/scratch/0x49aa80/annot.py, which resolves
// [esp+N] to absolute frame slots and models _allmul as ret 0x10 / _allshr as
// cdecl), the original's first block is:
//   mov esi,a2 / mov edi,a3 at the top of the prologue (both pointers live in
//   callee-saved registers across both __allmul calls), a2->x hoisted into ebp,
//   the Z subtraction issued FIRST, dx.lo kept in ebp with dx.hi spilled to
//   [E-4], and dz.hi spilled late (after the four argument pushes).
// This file now matches the frame, the slot map and every instruction from the
// distance compare onward; what is left is register allocation in the first
// block, plus the load order in the height check and the argument push order in
// the line-of-fire call:
//   * ours gives esi to the temporary a2->x and rematerialises a2 from its
//     argument home (ecx), the original holds a2 in esi and a3 in edi;
//   * ours has both dx halves in registers (ecx, ebp), the original splits
//     dx.lo into ebp and dx.hi into [E-4];
//   * ours hoists a3->x into the dead a4 home at [E+16] and reloads it in the
//     line-of-fire block (MSVC 5 shares that load across the calls), the
//     original re-reads a3->x from edi there;
//   * the height sum's two sides load in the opposite order (ours: the a1
//     pointer chain first, then a2->y.whole).
// MEASURED AND REFUTED IN THIS PASS (all with the same includes and flags).
// Every one of these compiles BYTE-IDENTICALLY to this file at 68.7% (they are
// all the same machine code, so none of them is the lever):
//   * swapping the height sum's operands (a1->def->field_170 + a2->y.parts.whole);
//   * naming the three line-of-fire differences in int locals, or the distance
//     in an `int dist` local, or the range in a local;
//   * `Vec3* p2 = a2, *p3 = a3` locals (any mix of the three blocks that use
//     them) and `Vec3& v2 = *a2, v3 = *a3` references: MSVC coalesces them back;
//   * inline helpers around the squares: by value (__int64 v), by pointer
//     (&d[1]), a Dist2(a, b) helper holding the whole body, HiSq with the shift
//     inside or outside, an access per component (Ax_/Az_), a nested scope;
//   * `const Vec3*` / `Vec3* const` / `Vec3&` parameters (the mangled name
//     changes, the code does not);
//   * deleted-store nudges (d[1] = d[1;, wdef = wdef;) and extra uses that fold
//     away (a3->x - (a2->x - 0), + 0, * 1, a2 == a2 ? 1 : 0): all inert;
//   * tools/headers.py over all 128 sets and 0..79 dummy externs: all 68.7%.
// WORTH KNOWING: adding <math.h> (or defining the preceding FUN_0049a850 in the
// same file, which includes it) drops the score to 67.0% but flips the height
// check's load order to the original's, `movsx reg, word ptr [a2+6]` BEFORE the
// a1->def->field_170 chain instead of after. So the operand order there is a
// free parameter of the compiler state, not a source decision, and 68.7% with
// the chain first still scores better because it keeps the block at 6
// instructions. That block needs 6 instructions only when a2 is in a register,
// which is the same single missing decision as the first block.
// WORSE, and instructive:
//   * reversing either subtraction direction (a2->x - a3->x, a2->z - a3->z; the
//     squares are unchanged): 44.8% (frame 0x14) and 44.3% (frame 8);
//   * the 0x49abb0 sibling's Dist2 idiom (an inline helper with `int dz; int dx;`
//     and (__int64) casts on the products): 48.5% and 338 bytes, so that helper
//     shape does not travel to this function;
//   * a two-member 64-bit struct local, `{__int64 dz, dx;}`, which pins the
//     slots to declaration order: z-first gives 45.9% (frame 0x14) or 64.4%,
//     x-first 66.1%. Named __int64 locals give frame 0xc (only 12 bytes: the
//     register-resident half of dx gets no slot), which cannot match the
//     original's four slots. __int64 d[2] is the only spelling found that gets
//     all four slots, and its slot order fixes d[0] = z and d[1] = x, hence the
//     swap above; MSVC then hands the LOW pair to whichever of the two is
//     assigned SECOND, which is why the statements must assign x first even
//     though the original issues the z subtraction first.
// FREE TOOLING in build/scratch/0x49aa80/ (not committed):
//   annot.py - disassembles the original or a built variant and rewrites every
//              [esp+N] as an absolute frame slot (E+4 is the a1 home, E-16 is
//              [E-0x10]); it models _allmul as ret 0x10 and _allshr as cdecl,
//              and resets esp after a ret so the other blocks annotate right.
//              This is what found the slot map, and it is the fastest way to
//              compare a variant against the original here.
//   sweep.py + sweep3..sweep20.py - generate/compile/score a batch of variants
//              in one process (~5s per variant) and print the first block with
//              its slots for anything that beats the incumbent.
//   state.py - the compiler-state sweep (N dummy externs in front).
//   v.py, fa.py - single-variant scoring and the raw /Fa listing.
int __stdcall FUN_0049aa80(Unit_0049aa80* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = a1->weapons[a4 & 0xff].def;

    __int64 d[2];
    d[1] = a3->x - a2->x;
    d[0] = a3->z - a2->z;
    if ((int)(d[1] * d[1] >> 32) + (int)(d[0] * d[0] >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.bit16)
        return 1;

    if (a2->y.parts.whole + a1->def->field_170 <= g_game->field_1427f)
        return 0;

    if (wdef->flags.bit1) {
        if (FUN_0049a890(a2->x - a3->x, a2->y.value - a3->y.value, a2->z - a3->z,
                         wdef->field_68, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}
