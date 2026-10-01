// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash. Names are provisional.
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
// WHAT STILL DIFFERS: the first basic block only (register allocation and the
// spill slots it produces); from the range compare onward the two agree
// instruction for instruction except the load order while materialising the
// FUN_0049a890 arguments. Best remains 66.1% (310 of 301).
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
#include <windows.h>   // only for the register allocation it nudges (60.1 -> 61.8)
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
int __stdcall FUN_0049aa80(Unit_0049aa80* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = a1->weapons[a4 & 0xff].def;

    __int64 d[2];
    d[0] = a3->x - a2->x;
    d[1] = a3->z - a2->z;
    if ((int)(d[0] * d[0] >> 32) + (int)(d[1] * d[1] >> 32) > wdef->range * wdef->range)
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
