// Decompiled by Space Bunny Free. Names are provisional.
// NOT MATCHED (47.5%). Still differs: the frame is 8 bytes smaller than the
// original's (`sub esp,0x3c` against 0x44, and the path array lands at
// [esp+0x28] against [esp+0x30]), so every reference to a local has a
// displacement 8 too low. The original keeps two more live stack slots below
// the array (the spilled sign words of the 64-bit products at [esp+0x18],
// [esp+0x20] and [esp+0x28]) and I could not find a declaration shape that
// makes MSVC 5 allocate them. The instruction sequence, the call sequence, the
// nine _allmul/_alldiv fixed-point helpers and the branch structure all match.
//
// What the function does: the path object (its vtable is 0x4fd458, see
// 0x44f010.cpp; slot 3 is 0x44f150, which copies up to three of its points out
// as Vec3, slot 5 is 0x44f290) hands over the next waypoints. Waypoint 1 is
// pulled back along the first segment when the unit is very close to it, the
// unit's turn is applied and clamped to the type's max_turn, and the distance
// this frame is allowed to travel is either the type's field at +0x19e or the
// negated z component of the first segment.
//
// Suspected original bugs (kept as the original has them):
//  - `dz` is read uninitialised. When the first _hypot is 0x500000 or less,
//    0x43cdbd jumps straight to 0x43ceb4, which never writes [esp+0x14], and
//    0x43d0b4 reads that slot for the amount.
//  - the same slot is reused: when the pull-back block runs, [esp+0x14] holds
//    the normalised 16.16 step (0x43ce51), not the segment's z component, so
//    the amount becomes -nz where the other path gives -dz.
//  - the value of `(unsigned short)adiff * field_20 / max_turn` (0x43d022) is
//    stored at 0x43d027 and overwritten at 0x43d03a without ever being read.

#include <math.h>

struct Vec3 { int x; int y; int z; };

#pragma pack(push, 1)
struct UnitType_0043cd20 {
    char unknown_0[0x19a];
    int field_19a;                    // +0x19a, the distance to travel
    char unknown_19e[2];
    int field_19e;                    // +0x19e, the other distance
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short max_turn;          // +0x1ba
};

struct Unit_0043cc20 {
    char unknown_0[0x66];
    short heading;                    // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3 pos;                         // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0043cd20* type;          // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags_0 : 16;        // +0x110
    unsigned int moved : 1;           // +0x110 bit 16
    unsigned int flags_17 : 15;
};
#pragma pack(pop)

// Hand-written fixed-point atan2 in the gap at 0x4b70a0.
int __stdcall FUN_0048a980(Vec3* from, Vec3* to);

// The path object: slot 5 (vtable +0x14) says whether a path is active, slot 3
// (vtable +0xc) copies `count` points out starting at `first`.
class Iface_0043dd20 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* out, int first, int count);
    virtual void v4();
    virtual int v5();
};

class Class_0043cc20 {
public:
    void FUN_0043cc20(Unit_0043cc20* unit, int amount);
};

class Class_0043cd20 {
public:
    Iface_0043dd20* obj;             // +0x0
    char unknown_4[0x20 - 0x4];
    int field_20;                     // +0x20
    short turn;                       // +0x24

    void FUN_0043cd20(Unit_0043cc20* unit);
};

// FUNCTION: 0x43cd20
void Class_0043cd20::FUN_0043cd20(Unit_0043cc20* unit)
{
    int hasPath = obj->v5();
    if (hasPath == 0) {
        turn = hasPath;
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, -unit->type->field_19a);
        return;
    }

    Vec3* up = &unit->pos;
    Vec3 p[3];
    obj->v3(p, 0, 3);

    // Distance to the next waypoint, in 16.16 fixed point.
    int ax = p[1].x - up->x;
    int az = p[1].z - up->z;
    int gap1 = (int)_hypot(ax, az);
    int dz;
    if (gap1 > 0x500000) {
        // Too close to the waypoint: step back along the segment by the
        // overhang, so the heading is taken from further away.
        int dx = p[1].x - p[0].x;
        dz = p[1].z - p[0].z;
        int len = (int)_hypot(dx, dz);
        if (len >= 0x10000) {
            dx = (int)(((__int64)dx << 16) / len);
            dz = (int)(((__int64)dz << 16) / len);
            int back = gap1 - 0x500000;
            if (back > len)
                back = len;
            p[1].x -= (int)(((__int64)dx * back) >> 16);
            p[1].z -= (int)(((__int64)dz * back) >> 16);
        }
    }

    // 32.32 fixed-point distance to the (moved) waypoint.
    int d1 = (int)((((__int64)ax * ax) >> 32) + (((__int64)az * az) >> 32));

    short diff = FUN_0048a980(up, &p[1]) - unit->heading;
    int sdiff = diff;
    int adiff = sdiff < 0 ? -sdiff : sdiff;

    int bx = p[2].x - up->x;
    int bz = p[2].z - up->z;
    int d2 = (int)((((__int64)bx * bx) >> 32) + (((__int64)bz * bz) >> 32));

    if (diff != 0) {
        unsigned short max = unit->type->max_turn;
        if (sdiff >= max)
            turn = max;
        else if (sdiff <= -max)
            turn = -max;
        else
            turn = diff;
        unit->heading += turn;
        unit->moved = 1;
    } else {
        turn = 0;
    }

    int turned = (int)(((__int64)((unsigned short)adiff) * field_20)
                       / unit->type->max_turn);
    int rate = unit->type->field_19a;
    int q = (int)((((__int64)field_20 * field_20) >> 32) << 16) / (2 * rate);
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)((((__int64)rate * rate) >> 32) * 4);

    // Only take the long step when the turn can be taken at that distance.
    if (d1 > lim && d2 > r)
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, unit->type->field_19e);
    else
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, -dz);
}
