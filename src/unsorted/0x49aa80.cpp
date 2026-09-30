// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by GPT-6.1-sol,
// retried by deepseek-v4.1-flash. Names are provisional.
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
