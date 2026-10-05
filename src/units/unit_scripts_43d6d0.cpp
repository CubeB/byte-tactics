// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// Moves the unit one step. With a path object it snaps to the path's next
// point (raised to the draft/sea-level floor for units with the type flag at
// +0x241 bit 19) and copies the path's velocity; without one it adds the
// velocity p1 to the position, and if the unit moves into a new cell that the
// target says is blocked it clamps the position to the current cell and caps
// the speed at half the type's range.
//
// MATCH (Claude Opus 5.5, 2026-10-03; earlier attempts stopped at 74.3%).
// What the bytes needed, each measured:
//   - (mimo-v2.6-pro) the floor clamp's address select (`lea eax,[esp+0x24];
//     jmp` / `mov [esp+0x3c],eax; lea eax,[esp+0x3c]`, then one load) is a
//     MAX macro over the Fixed union with a prvalue second operand; an int
//     max hoists the lea and folds the shift chain.
//   - the path branch is `Vec3 v; v = GetPiecePosition(...)`: the copy through the
//     returned pointer. It only lands right once the no-path branch gives the
//     frame its real layout.
//   - the new position is `pos = p1 + u->pos` through an inline
//     `operator+(const Vec3&, const Vec3&)`, assigned (not initialised): the
//     operator's result temporary is the 12-byte object at frame+0x1c whose y
//     the exe spills to its own home and reloads later (the "ny inside the
//     dead GetPiecePosition temp" the old notes could not place), and its
//     reference to p1 is the `lea ecx,[ebp+8]` the exe keeps at frame+0x8 for
//     the later `p1 = vec`. Separate int sums, a named n, `Vec3 pos = ...`
//     and a `Vec3* pp` all schedule the three adds differently.
//   - u->pos is written with whole-struct copies (`u->pos = pos`), which is
//     what gives the clamp tail its `or dword ptr [esi+0x110],0x10000`.
//   - the cell is assigned field by field, with the half-cell offset written
//     as `draft.x * 0x80000` (the `<< 19` spelling evaluates draft.x first).
//   - the clamp to the current cell is an inline helper taking both Points by
//     value: its parameters are the exe's second dword copies of u->cell and
//     u->draft (into the dead parameter slot and draft's slot), read back
//     through the registers.
//   - `u->mode = (short)m`: a narrow source is what makes VC5 store the
//     2-bit field with `and/and/or` instead of its `xor/and/xor` form.
//   - the target test is two nested ifs, the z component of the speed cap
//     goes through an int temporary, and the file includes <stdlib.h> (the
//     header state; 0x43cd20, which uses abs(), likely shared this file).
//     Each of these three alone scores lower; together they match.
#include <stdlib.h>


#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1427f];
    unsigned char seaLevel;                         // +0x1427f
    char unknown_14280[0x38a47 - 0x14280];
    int field_38a47;                                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

#pragma pack(push, 1)

struct FP_0043d6d0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_0043d6d0 {
    int value;
    FP_0043d6d0 parts;
};

static inline Fixed_0043d6d0 MakeFixed_0043d6d0(int i)
{
    Fixed_0043d6d0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

inline int operator>(const Fixed_0043d6d0& a, const Fixed_0043d6d0& b) { return a.value > b.value; }

#define MAXM_0043d6d0(a, b) ((a) > (b) ? (a) : (b))

struct Vec3 {
    Fixed_0043d6d0 x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) { x.value = a; y.value = b; z.value = c; }
};

struct Point {
    short x, y;
    Point() {}
    Point(int a, int b) : x(a), y(b) {}
};

struct Short3 {
    short x, y, z;
};

struct UnitType_0043d6d0 {
    char unknown_0[0x192];
    int range;                                      // +0x192
    char unknown_196[0x22c - 0x196];
    unsigned char draft;                            // +0x22c
    char unknown_22d[0x241 - 0x22d];
    unsigned int low : 19;                          // +0x241
    unsigned int b19 : 1;
    unsigned int rest : 12;
};

struct Target_0043d6d0 {
    int field_0;                                    // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                             // +0x73
};

struct TargetData_0043d6d0 {
    char unknown_0[8];
    Vec3 p1;                                        // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                                   // +0x20
};

struct Path_0043d6d0 {
    TargetData_0043d6d0* field_0;                   // +0x0
};

struct Unit {
    char unknown_0[0x64];
    Short3 f64;                                     // +0x64
    Vec3 pos;                                       // +0x6a
    Point cell;                                     // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point draft;                                    // +0x7e
    char unknown_82[0x86 - 0x82];
    Path_0043d6d0* obj;                             // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0043d6d0* type;                        // +0x92
    Target_0043d6d0* target;                        // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short a8;                                       // +0xa8
    char unknown_aa[0xf9 - 0xaa];
    signed char index;                              // +0xf9
    char unknown_fa[0x110 - 0xfa];
    union {
        unsigned int flags;                         // +0x110
        struct {
            unsigned int mode : 2;                  // +0x110 bits 0-1
            unsigned int flags_2 : 14;
            unsigned int moved : 1;                 // +0x110 bit 16
            unsigned int flags_17 : 15;
        };
    };
};

#pragma pack(pop)


Vec3 __stdcall GetPiecePosition(Path_0043d6d0* obj, int index);
Short3 __stdcall GetPieceAngles(Path_0043d6d0* obj, int index);
void __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int mode);
int __stdcall FUN_0047db70(UnitType_0043d6d0* type, short a8, Point cell, int mode);
void __stdcall RemoveUnitFromMap(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall UpdateUnitLineOfSight(Unit* unit);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

#pragma pack(push, 1)
class Class_0043d6d0 {
public:
    char unknown_0[8];
    Vec3 p1;                                        // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                                   // +0x20
    char unknown_24[0x2a - 0x24];
    int field_2a;                                   // +0x2a
    unsigned char mode : 2;                         // +0x2e
    unsigned char flag : 1;                         // bit 2
    unsigned char unknown_2f : 5;

    void UpdatePosition(Unit* unit);
};
#pragma pack(pop)

static inline void ClampToCell(Vec3& pos, Point cell, Point draft)
{
    int cx = (draft.x + cell.x * 2) << 19;
    int cz = (draft.y + cell.y * 2) << 19;
    if (pos.x.value > cx + 0x7ffff)
        pos.x.value = cx + 0x7ffff;
    else if (pos.x.value < cx - 0x7ffff)
        pos.x.value = cx - 0x7ffff;
    if (pos.z.value > cz + 0x7ffff)
        pos.z.value = cz + 0x7ffff;
    else if (pos.z.value < cz - 0x7ffff)
        pos.z.value = cz - 0x7ffff;
}

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x.value = a.x.value + b.x.value;
    r.y.value = a.y.value + b.y.value;
    r.z.value = a.z.value + b.z.value;
    return r;
}

// FUNCTION: 0x43d6d0
void Class_0043d6d0::UpdatePosition(Unit* u)
{
    if (u->obj != 0) {
        Vec3 v;
        v = GetPiecePosition(u->obj, u->index);
        if (u->type->b19) {
            v.y = MAXM_0043d6d0(v.y, MakeFixed_0043d6d0(u->type->draft * 0xffff + g_game->seaLevel));
        }
        SetUnitPosition(u, v, mode);
        Short3 o = GetPieceAngles(u->obj, u->index);
        u->f64 = o;
        if (u->obj->field_0 != 0) {
            field_20 = u->obj->field_0->field_20;
            p1 = u->obj->field_0->p1;
        } else {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            p1 = zero;
        }
        u->moved = 0;
        return;
    }

    Vec3 pos;
    pos = p1 + u->pos;
    int m = mode;
    if (pos.x.value == u->pos.x.value && pos.z.value == u->pos.z.value && pos.y.value == u->pos.y.value && m == u->mode)
        return;

    field_2a = g_game->field_38a47;
    Point draft = u->draft;
    Point cell;
    cell.x = (pos.x.value - draft.x * 0x80000 + 0x80000) >> 20;
    cell.y = (pos.z.value - draft.y * 0x80000 + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == u->mode) {
        u->pos = pos;
        u->moved = 1;
        return;
    }

    if (u->target->field_0 != 0) {
        if (u->target->type == 1 || u->target->type == 2)
            flag = FUN_0047db70(u->type, u->a8, cell, m) == 0;
    }

    if (flag) {
        ClampToCell(pos, u->cell, u->draft);

        if (field_20 > (u->type->range / 2)) {
            int half = u->type->range / 2;
            field_20 = half;
            unsigned short angle = u->f64.y;
            Vec3 vec;
            vec.x.value = -FUN_004b70ef(angle, half);
            vec.y.value = 0;
            int z = -FUN_004b7123(angle, half);
            vec.z.value = z;
            p1 = vec;
        }
        u->pos = pos;
        u->moved = 1;
        return;
    }

    RemoveUnitFromMap(u);
    u->pos = pos;
    u->cell = cell;
    u->mode = (short)m;
    AddUnitToMap(u);
    u->moved = 1;
    UpdateUnitLineOfSight(u);
}
