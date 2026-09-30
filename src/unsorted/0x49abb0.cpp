// Decompiled by GPT-5.6-Terra, finished by Space Bunny Free, finished by GPT-6.1-sol. Names are provisional.
// Retry #1736: GPT-6.1-sol verified the saved source at 93.7% (576/568); no MATCH. The line-of-fire block still reloads unit2 after copying its position.
// Partial, 93.7% (576 of 568 bytes; up from 90.9%). Logic, offsets and every branch match.
// Two things moved it: `(height >> 1) + whole` (not `whole + (height >> 1)`) gives the
// original's `add edx, ecx` operand order in the half-height test, and the two includes
// below change MSVC's register choice in the sea-level tests (headers.py found them;
// without them the def pointer and the y word swap registers).
// What still differs, all in the line-of-fire block: the original copies unit2->pos
// (x to [esp+0x10], z to [esp+0x18], y kept in ebp) with `lea edx,[ebx+0x6a]`, keeping
// unit2 in ebx for the second distance tail; here MSVC does `add ebx,0x6a` and reloads
// ebx from the stack afterwards, so the tail differs by one reload (8 bytes).
// Tried: by-value and by-pointer Vec3 params in every order, plain-int Vec3, local
// copies (the copy is then optimised away, frame shrinks to 8), dx/dy/dz statement
// orders, def pointer locals; pointer params make MSVC merge the two distance tails.
//
// ---- space-bunny-free pass, 23 scratch variants, no improvement, best still 93.7% ----
// Verified first, both cheap: the call count is 9 in the original and 9 here (4x
// _allmul, 4x _allshr, 1x FUN_0049a890), so no call is missing. `ret 0xc` against
// the 3-arg declaration and FUN_0049a890's `ret 0x14` against 5 int args are both
// right, so the calling convention is NOT the cause.
// One divergence region, at 0x49ad2c (`je`), class (d)/(c): the whole line-of-fire
// block is rescheduled and ours carries 3 extra instructions. First divergence is
// 0x49ad2e `lea edx,[ebx+0x6a]` against our `add ebx,0x6a`, class (c) plus a
// register-allocation consequence: MSVC turns the by-value `from` aggregate into a
// live POINTER (ebx, then ebp) and reads the three fields through it, instead of
// materialising the copy and reading the fields from the source with a lea'd scratch.
// Killing that `add` is worth 3 instructions: the pointer copy, the reload of ebx
// from the stack before the second distance tail, and one field access.
// The frame is `sub esp,0xc` (3 dwords). With no pushes outstanding, [esp+0x10] is
// the unit1 argument home and [esp+0x18] the weapon argument home, so the original
// stores the 3-dword `from` copy over two DEAD argument homes and never writes its
// third dword (from.y stays in ebp). Ours writes exactly the same two slots, so the
// aggregate home is already right; only the access path is wrong.
// Best new lead, not enough on its own: making the aggregate copy REAL, by feeding
// the first by-value parameter from a helper that returns Vec3 by value
// (`static inline Vec3 CopyPos(Unit* u) { return u->pos; }`), does produce the
// original's `lea edx,[ebx+0x6a]`, keeps ebx alive and deletes the tail reload. But
// it scores 92.1% (579 bytes) because MSVC then stores all THREE fields (the return
// buffer is filled completely), loads w->field_c8 and w->field_68 and pushes them
// FIRST instead of last, and orders the differences dz, dy, dx instead of dx, dy, dz;
// its aggregate home also lands 4 bytes higher. So: real copy fixes the register
// choice, and the next person needs a form that is a real copy without the full
// three-field store.
// Everything measured, all scratch variants in build/scratch/0x49abb0/: 93.7%/576 is
// the ceiling and is byte-stable for w1 (block-scoped `Unit* t = unit2` copy), w2
// (the two scalar params swapped), w11 (`== -32768` instead of `(short)0x8000`) and
// v16 (dx/dy/dz in named int locals), all byte-identical to this file. Worse: v2 and
// w10 (Vec3 param order swapped) 92.1%/579, v5/v8/v12/v13/v14/v15 (a real local copy
// of unit2->pos, by assignment, by struct-returning helper, or with named dx/dy/dz)
// 92.1%/579, v1/v7/x3/x5 (the 0x49aa80 form, plain expressions, no by-value Vec3)
// 49.8%/538 - and that last one shows why the by-value aggregate is load-bearing:
// without it the line-of-fire block steals edi, the weapon-def pointer, and the tail
// has to reload it. v3/v4 (one Vec3 by value, the other by pointer) 76.8% and 75.2%,
// w8 (`const Vec3&`) 75.2%, and a Vec3 class with a user copy constructor does
// not emit the function at all (0 bytes).
// GPT-6.1-sol attempted two-field by-value aggregates with 8-byte and padded 12-byte
// layouts; both changed the frame/register allocation and scored 75.2%. A malformed
// aggregate call failed to compile. Best remains 93.7%, with the line-of-fire block
// differences described above.
// Conclusion: the by-value Vec3 pair is right, and the last 6.3% is one register
// allocator decision inside the line-of-fire block. Lead #1431 introduced a local pointer to unit2->pos at the call site; output stayed byte-identical at 93.7%. It is not an operand order, a
// frame size, a call count or a convention problem, and it is not reachable by
// reordering the arguments.
#include <stdlib.h>
#include <math.h>
#pragma pack(push, 1)

union Fixed_0049abb0 {
    int value;                                      // 16.16 fixed point
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Vec3_0049abb0 {
    int x;
    Fixed_0049abb0 y;
    int z;
};

struct WeaponDef_0049abb0 {
    char unknown_0[0x68];
    int field_68;                                   // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                                 // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                                      // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;                      // line of fire
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;                     // skip the ground test
        unsigned int bit17 : 1;                     // target must be landed
        unsigned int bit18_31 : 14;
    } flags;                                        // +0x111
};

struct Weapon_0049abb0 {
    WeaponDef_0049abb0* def;                        // +0x0
    char unknown_4[0x1c - 0x4];
};

struct UnitDef_0049abb0 {
    char unknown_0[0x170];
    short height;                                   // +0x170
    char unknown_172[0x241 - 0x172];
    struct {
        unsigned int bit0_11 : 12;
        unsigned int bit12 : 1;                     // half height counts
        unsigned int bit13_18 : 6;
        unsigned int bit19 : 1;                     // ignore sea level
        unsigned int bit20_31 : 12;
    } flags;                                        // +0x241
};

struct Unit_0049abb0 {
    char unknown_0[0x10];
    Weapon_0049abb0 weapons[1];                     // +0x10, stride 0x1c
    char unknown_2c[0x6a - 0x2c];
    Vec3_0049abb0 pos;                              // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0049abb0* def;                          // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int state;                             // +0x110
};

struct Game_0049abb0 {
    char unknown_0[0x1427f];
    unsigned char sea_level;                        // +0x1427f
};

#pragma pack(pop)

extern Game_0049abb0* g_game;

short __stdcall FUN_0049a890(int dx, int dy, int dz, int a, int b);

// The target is passed by value: that is what puts the 12-byte copy of its
// position in the frame, while the shooter is read where it is.
static inline short LineOfFire_0049abb0(Vec3_0049abb0 from, Vec3_0049abb0 to, int s, int f)
{
    return FUN_0049a890(to.x - from.x, to.y.value - from.y.value, to.z - from.z, s, f);
}

static inline int Dist2_0049abb0(Vec3_0049abb0* b, Vec3_0049abb0* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x49abb0
int __stdcall FUN_0049abb0(Unit_0049abb0* unit1, Unit_0049abb0* unit2, unsigned char weapon)
{
    WeaponDef_0049abb0* w = unit1->weapons[weapon].def;

    if (w->flags.bit16) {
        if (!unit2->def->flags.bit19 && unit2->pos.y.parts.whole > g_game->sea_level)
            return 0;
        if (unit2->def->flags.bit12 && (unit2->def->height >> 1) + unit2->pos.y.parts.whole > g_game->sea_level)
            return 0;
        return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
    }

    if (unit1->pos.y.parts.whole + unit1->def->height <= g_game->sea_level)
        return 0;
    if (unit2->pos.y.parts.whole + unit2->def->height <= g_game->sea_level)
        return 0;
    if (w->flags.bit17 && (unit2->state & 3) != 2)
        return 0;
    if (w->flags.bit1) {
        if (LineOfFire_0049abb0(unit2->pos, unit1->pos, w->field_68, w->field_c8) == (short)0x8000)
            return 0;
    }
    return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
}
