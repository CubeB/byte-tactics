// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by Space Bunny Free.
// finished by Space Bunny Free.
// Names are provisional.
//
// MATCH. Read this before changing anything: the body is exactly the 0x49abb0
// idiom (that sibling is a MATCH, and its two by-value boundaries are what this
// function needs too), and the only free choice left is the include set.
//
// WHAT THE LAST PASS ADDED, in the order the steps paid off (space-bunny-free,
// 2026-10-02, 8 check.py runs, ~110 scored scratch variants, 68.7% -> 100%):
//   1. 73.3% / 301 bytes: the line-of-fire call goes through a by-value helper
//      `static inline short LineOfFire_0049aa80(Vec3 to, Vec3 from, int s,
//      int f)`, called with `(*a2, *a3, wdef->field_68, wdef->field_c8)`. Taking
//      both positions BY VALUE is what puts a2 in esi and a3 in edi for the whole
//      function, which is the single register decision every earlier pass was
//      fighting (they all saw "the original holds a2 in esi and a3 in edi" and
//      no spelling of the first block would produce it). Straightfield
//      differences (73.3%), differences in named int locals inside the helper
//      (73.3%), const parameters (73.3%), the arguments swapped (71.6%) and the
//      helper taking pointers (68.7% or 62.1%) are all worse or inert.
//   2. 80.0%: the same helper, but its body is `Vec3 d = from - to;` using a
//      real `operator-` that takes both points BY VALUE and returns one by
//      value, again copied verbatim from 0x49abb0. The helper body written as
//      plain field expressions gives 73.3%.
//   3. 96.6% / exactly 301 bytes: the distance is the 0x49abb0 `Dist2` idiom
//      written out in the body, NOT the `__int64 d[2]` array every earlier pass
//      settled on:
//          int dz = a3->z - a2->z;
//          int dx = a3->x - a2->x;
//          (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32)
//      The z difference is computed first, which is the original's order; the
//      array cannot express that (its slots are pinned by index, d[0] = z, so
//      assigning z first and squaring x first pulls the frame to 0x14 or drops
//      the instructions that spill dx.hi). With the array still in place this
//      change scores 55-70%; with the two by-value boundaries in place it is
//      96.6%. dx-first is 92.2%, so the order of the two named locals matters.
//   4. 100% MATCH: the one remaining hunk was five instructions in the height
//      check, the two sides of the add coming out in each other's registers,
//      which is the guide's "the header set decides the comparison's operand
//      order" case. `uv run tools/headers.py 0x49aa80 --cpp` reports 304 sets
//      that MATCH; <stdio.h> on top of <windows.h> is the smallest. Every
//      source rewrite of that comparison (swapping the operands, a named short
//      or int local, `!(x > y)`, the reversed comparison) is byte-identical at
//      96.6%, which is why the header is load-bearing here and not decoration.
//
// WHY THE OLD LEADS WERE WRONG (they cost this function several passes):
//   * "the low frame pair belongs to whichever element is assigned SECOND, so
//     z-first and z-in-the-low-pair are in tension": the frame slots follow the
//     ARRAY INDEX (d[0] at E+0, d[1] at E+0x8), not the definition order, and a
//     union of __int64[2] with a named-member struct, d[3] and d[4] all behave
//     exactly like d[2] here. The tension was real only for the array; the
//     named `int` locals remove it entirely (lead 1, refuted, the array is not
//     the answer).
//   * "one more genuine use of a2 breaks the register-priority tie": no extra
//     use of a2 was needed, and none of them (hoisted component reads, pointer
//     copies, a NULL check, a fold-away use) put a2 in a register. What did was
//     giving a2 and a3 a by-value copy at a call site, which is a different
//     mechanism: MSVC allocates the copies' home in the frame and then keeps
//     the pointers in the callee-saved registers that survive the call.
//   * "the sibling's Dist2 idiom does not travel to this function" (48.5%,
//     338 bytes, measured by an earlier pass): true for the DIST2 HELPER
//     (`Dist2(a2, a3)` as a call), false for the same statements written out in
//     the function body, and false for it once the line-of-fire helper is
//     by-value. A near copy of a matched sibling is worth reading before a
//     syntax matrix.
//   * the permuter (seed 12, 1482 candidates, 4.1 min) found nothing on the
//     68.7% file, and 0..8 dummy inline functions and twelve includes were all
//     inert at 68.7%, so the "compiler state" sweeps were measuring a state
//     the function never had.
//
// FREE TOOLING in build/scratch/0x49aa80/ (not committed): `sc.py` (score a
// body variant in-process, no check.py run), `ann.py` (disassemble the original
// or a built variant with every [esp+N] resolved to an absolute frame slot,
// modelling _allmul as ret 0x10), and `e1.py`..`e20.py`, the batches above.
#include <windows.h>
#include <stdio.h>      // tools/headers.py: this header set on top of
                        // <windows.h> gives the original's operand order in the
                        // height check (math.h, memory.h, vector, map and list
                        // do too; stdio.h is the smallest). With <windows.h>
                        // alone the function is 5 instructions short there.

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

// The subtraction of two points is a real by-value operator taking both
// operands by value and returning the point by value, and the line-of-fire test
// goes through a by-value helper, both exactly as in the matching sibling
// 0x49abb0 (whose MATCH this idiom produced). Those two boundaries are what put
// a2 in esi and a3 in edi for the whole function and keep the weapon def in
// ebx, which is the first block's register allocation (see the pass note at the
// top of the file).
inline Vec3_0049aa80 operator-(Vec3_0049aa80 a, Vec3_0049aa80 b)
{
    Vec3_0049aa80 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

static inline short LineOfFire_0049aa80(Vec3_0049aa80 to, Vec3_0049aa80 from, int s, int f)
{
    Vec3_0049aa80 d = from - to;
    return FUN_0049a890(d.x, d.y.value, d.z, s, f);
}

// FUNCTION: 0x49aa80
int __stdcall FUN_0049aa80(Unit_0049aa80* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = a1->weapons[a4 & 0xff].def;

    int dz = a3->z - a2->z;
    int dx = a3->x - a2->x;
    if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.bit16)
        return 1;

    if (a2->y.parts.whole + a1->def->field_170 <= g_game->field_1427f)
        return 0;

    if (wdef->flags.bit1) {
        if (LineOfFire_0049aa80(*a2, *a3, wdef->field_68, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}
