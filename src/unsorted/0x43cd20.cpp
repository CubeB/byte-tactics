// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
//
// NOT MATCHED (45.2%, 927 bytes against 943). The earlier version scored 47.5
// but only because it modelled the slow-path amount as -dz; that kept dz live
// to the end and changed the allocation. With the correct -rate it is 45.2.
// Correct semantics now, but the register allocation and frame
// still differ: ours is 0x38 bytes of locals against the original's 0x44, so
// every stack displacement below 0x10 is 12 too low. The original keeps four
// values live in callee-saved registers across the calls (unit in edi, the
// address of unit->pos in ebx, d1 in ebp, adiff in esi) and therefore spills
// the high words of the 64-bit products at [esp+0x18], [esp+0x20] and
// [esp+0x28]; ours frees registers earlier, keeps fewer values live and uses
// only two scalar slots.
//
// What this function does: the path object (vtable 0x4fd458, see
// 0x44f010.cpp; slot 3 is 0x44f150, slot 5 is 0x44f290) hands over the next
// three waypoints. Waypoint 1 is pulled back along the first segment when the
// unit is farther than 0x500000 from it, the turn toward it is applied and
// clamped to the type's max_turn, and then the distance this frame may travel
// is either the type's +0x19e or the negated rate at +0x19a.
//
// Corrections to the earlier attempt (all three are evidenced by the
// disassembly):
//  - The slow-path amount is -type->field_19a, not -dz. At 0x43d03a
//    `mov [esp+0x24],esi` runs after four pushes, so it writes [esp+0x14],
//    overwriting the dz slot, and 0x43d0b4 `mov eax,[esp+0x14]; neg eax`
//    reads that value back. The original's dz is dead after 0x43ce8c.
//  - q's numerator is ((field_20*field_20) >> 16) << 16, not >> 32 << 16.
//  - lim is (turned*turned >> 32) * 4, where turned is the earlier 64-bit
//    quotient (stored at 0x43d027 and reloaded at 0x43d087); the earlier
//    attempt used rate*rate. r is q*q >> 32.
//
// Suspected original bug: dz is read only inside the `gap1 > 0x500000` branch
// (0x43cdde and 0x43ce51) and is never used afterwards, so this is not the
// uninitialised read the earlier note claimed; the value read at 0x43d0b4 is
// the spilled rate. The unused store of turned at 0x43d027 followed by the
// overwrite at 0x43d03a is real (the 0x43d087 reload reads 0x43d027, so the
// value is live, not dead).
//
// Tried without changing the allocation: local pointer and reference copies
// of unit->pos, an inline DistSq/DistSqP helper, __int64 intermediates for
// d1/d2/q, uninitialised declarations up front, swapping the ax/az and
// d1/d2 evaluation order, `*0x10000` versus `<<16`, address-taken locals, and
// the header sets headers.py covers. All compiled to the same 0x38 frame or
// worse. A fake extra live local does reach 0x44 and scores 51.0, but it is
// not in the original.

#include <math.h>

struct Vec3 { int x; int y; int z; };

#pragma pack(push, 1)
struct UnitType_0043cd20 {
    char unknown_0[0x19a];
    int field_19a;                    // +0x19a, the rate
    int field_19e;                    // +0x19e, the long-step distance
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
    Iface_0043dd20* obj;              // +0x0
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

    Vec3 p[3];
    obj->v3(p, 0, 3);

    int ax = p[1].x - unit->pos.x;
    int az = p[1].z - unit->pos.z;
    int gap1 = (int)_hypot(ax, az);

    int dz;
    if (gap1 > 0x500000) {
        int dx = p[1].x - p[0].x;
        dz = p[1].z - p[0].z;
        int len = (int)_hypot(dx, dz);
        if (len >= 0x10000) {
            int ndx = (int)(((__int64)dx << 16) / len);
            int ndz = (int)(((__int64)dz << 16) / len);
            int back = gap1 - 0x500000;
            if (back > len)
                back = len;
            p[1].x -= (int)(((__int64)ndx * back) >> 16);
            p[1].z -= (int)(((__int64)ndz * back) >> 16);
        }
    }

    ax = p[1].x - unit->pos.x;
    az = p[1].z - unit->pos.z;
    int d1 = (int)(((__int64)ax * ax) >> 32) + (int)(((__int64)az * az) >> 32);

    short diff = FUN_0048a980(&unit->pos, &p[1]) - unit->heading;
    int sdiff = diff;
    int adiff = sdiff < 0 ? -sdiff : sdiff;

    int bx = p[2].x - unit->pos.x;
    int bz = p[2].z - unit->pos.z;
    int d2 = (int)(((__int64)bx * bx) >> 32) + (int)(((__int64)bz * bz) >> 32);

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

    int turned = (int)(((__int64)((unsigned short)adiff * field_20)
                        / unit->type->max_turn));
    int rate = unit->type->field_19a;
    int q = (int)(((( (__int64)field_20 * field_20) >> 16) * 0x10000) / (2 * rate));
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)(((__int64)turned * turned) >> 32) * 4;

    if (d1 > lim && d2 > r)
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, unit->type->field_19e);
    else
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, -rate);
}
