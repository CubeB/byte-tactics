// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
//
// NOT MATCHED (52.2%). The logic and the call sequence are believed correct;
// what still differs is register allocation and one stack slot.
//
// 1. The original keeps a separate 12-byte temporary for the FUN_0043e060
//    return at [esp+0x2c] and then copies it to `v` at [esp+0x20] (frame
//    sub esp,0x28). MSVC 5 elides that copy here and passes &v straight to the
//    callee (frame sub esp,0x1c). Tried and did not force the temporary:
//    Vec3 v = f(); Vec3 v; v = f(); Vec3 t = f(); Vec3 v = t; a one-element
//    Vec3 array, a const-returning callee, a user copy ctor / dtor / operator=,
//    an explicit &v pointer, an inline wrapper returning Vec3, and direct
//    initialisation Vec3 v(f()). Every one coalesced.
// 2. The original puts `this` in ebp (mov ebp,ecx) and then nx in edi, nz in
//    ebx, spilling ny to [esp+0x30]. Ours puts `this` in edi, nx in ebx, nz in
//    ebp. Same object family as 0x43cc20 (which also uses ebp for this), so the
//    choice is legitimate in this source file, but no local declaration order,
//    branch inversion, self alias, inline GridCell/Max helper or header set
//    (headers.py: 52.2% best, 5 sets tie) moved it.
//
// The final `u->flags` update also differs: the original emits a plain
// and/or (and eax,3; and ecx,0xfffffffc; or eax,ecx) where ours emits the
// xor pair a ^ ((a^b)&3) that MSVC 5 uses for a 2-bit bitfield write. Writing
// the assignment as a union bitfield (u->bits.mode = m) or as the
// (m & 3) | (flags & ~3) expression both still give the xor pair.

#include <string.h>

#pragma pack(push, 1)

struct Vec3 {
    int x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Point {
    short x, y;
};

struct Short3 {
    short x, y, z;
};

struct Game_0043d6d0 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;                         // +0x1427f
    char unknown_14280[0x38a47 - 0x14280];
    int field_38a47;                                // +0x38a47
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

struct Unit_0043d6d0 {
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
    unsigned int flags;                             // +0x110
};

#pragma pack(pop)

extern Game_0043d6d0* g_game;

Vec3 __stdcall FUN_0043e060(Path_0043d6d0* obj, int index);
Short3 __stdcall FUN_0043e180(Path_0043d6d0* obj, int index);
void __stdcall FUN_0048a9f0(Unit_0043d6d0* unit, Vec3 pos, int mode);
int __stdcall FUN_0047db70(UnitType_0043d6d0* type, short a8, Point cell, int mode);
void __stdcall FUN_0047d0e0(Unit_0043d6d0* unit);
void __stdcall FUN_0047cc30(Unit_0043d6d0* unit);
void __stdcall FUN_004827b0(Unit_0043d6d0* unit);
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

    void FUN_0043d6d0(Unit_0043d6d0* unit);
};
#pragma pack(pop)

// FUNCTION: 0x43d6d0
void Class_0043d6d0::FUN_0043d6d0(Unit_0043d6d0* u)
{
    if (u->obj != 0) {
        Vec3 v = FUN_0043e060(u->obj, u->index);
        if (u->type->b19) {
            int lim = (u->type->draft * 0xffff + g_game->seaLevel) << 16;
            v.y = v.y > lim ? v.y : lim;
        }
        FUN_0048a9f0(u, v, mode);
        Short3 o = FUN_0043e180(u->obj, u->index);
        u->f64 = o;
        if (u->obj->field_0 != 0) {
            field_20 = u->obj->field_0->field_20;
            p1 = u->obj->field_0->p1;
        } else {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            p1 = zero;
        }
        u->flags &= 0xfffeffff;
        return;
    }

    Vec3* pp = &p1;
    int nx = pp->x + u->pos.x;
    int ny = pp->y + u->pos.y;
    int nz = pp->z + u->pos.z;
    int m = mode;
    Vec3 pos;
    pos.x = nx;
    pos.y = ny;
    pos.z = nz;
    if (nx == u->pos.x && nz == u->pos.z && ny == u->pos.y && m == (int)(u->flags & 3))
        return;

    field_2a = g_game->field_38a47;
    Point draft = u->draft;
    Point cell;
    cell.x = (nx - (draft.x << 19) + 0x80000) >> 20;
    cell.y = (nz - (draft.y << 19) + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == (int)(u->flags & 3)) {
        u->pos.x = nx;
        u->pos.y = ny;
        u->pos.z = nz;
        u->flags |= 0x10000;
        return;
    }

    if (u->target->field_0 != 0 && (u->target->type == 1 || u->target->type == 2)) {
        flag = FUN_0047db70(u->type, u->a8, cell, m) == 0;
    }

    if (flag) {
        Point draft2 = u->draft;
        Point c = u->cell;
        int cx = (draft2.x + c.x * 2) << 19;
        int cz = (draft2.y + c.y * 2) << 19;
        if (nx > cx + 0x7ffff)
            pos.x = cx + 0x7ffff;
        else if (nx < cx - 0x7ffff)
            pos.x = cx - 0x7ffff;
        if (nz > cz + 0x7ffff)
            pos.z = cz + 0x7ffff;
        else if (nz < cz - 0x7ffff)
            pos.z = cz - 0x7ffff;
        int half = u->type->range / 2;
        if (field_20 > half) {
            field_20 = half;
            unsigned short angle = u->f64.y;
            pp->x = -FUN_004b70ef(angle, half);
            pp->y = 0;
            pp->z = -FUN_004b7123(angle, half);
        }
        u->pos.x = pos.x;
        u->pos.y = pos.y;
        u->pos.z = pos.z;
        u->flags |= 0x10000;
        return;
    }

    FUN_0047d0e0(u);
    u->pos.x = nx;
    u->pos.y = ny;
    u->pos.z = nz;
    u->cell = cell;
    u->flags = (u->flags & 0xfffffffc) | (m & 3);
    FUN_0047cc30(u);
    u->flags |= 0x10000;
    FUN_004827b0(u);
}
