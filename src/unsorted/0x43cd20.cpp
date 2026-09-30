// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by
// space-bunny-free. Names are provisional.
//
// NOT MATCHED (53.5%, 940 bytes against 943). Semantically correct. The frame
// is 0x3c here, the original's is 0x44 (the earlier 45.2% note's 0x38 is stale).
//
// Third pass notes (space-bunny-free, 1 check run). Frame slot map, measured
// from the original, everything relative to the parameter slot (which is where
// the original keeps several dead locals):
//   -0x04 .. -0x08  the three dwords above the array: the array ends 4 bytes
//                   below the parameter, ours ends 8 bytes below it
//   -0x28           &p[0]  (ours: -0x2c)
//   -0x2c, -0x30    the two temps the first _hypot's int args are spilled to
//                   before the two filds (ours reuses ONE slot, -0x34)
//   -0x34           len (gap1 sits at -0x3c)
//   -0x38           a hole, which is where a nested block would end
//   -0x3c           gap1
//   -0x44           dz, then ndz
//   -0x48           the spill of `this` (the top-of-function
//                   `mov [esp+0x10], esi`); ours spills it at -0x44
//   the parameter slot itself (0x00) holds, in turn, dx, ndx, p[1].z - pos.z
//   and the FUN_0048a980 result
// So the original needs 8 more bytes than ours, and it gets them from two more
// live slots below the array plus one dword more above it.
//
// The root cause of nearly every difference is that ours keeps re-reading
// `unit` from its parameter slot (see _unit$[esp+72] three times in the tail)
// while the original parks it in edi with &unit->pos in ebx from just after the
// v3 call to the end. Because the parameter stays live, the compiler never
// frees its slot, so the original's `dx` in the parameter slot (0x43cdd1) has
// nowhere to go in ours. Getting unit into a callee-saved register is the
// thing to chase; rewriting the expressions will not move it.
// The `hasPath == 0` arm wants the parameter load first
// (`mov ecx, [esp+0x58]` then `mov word [esi+0x24], ax`); ours stores the
// turn first whatever the spelling of the two statements was.
// Also tried: naming both _hypot arguments as locals and swapping their order
// (byte-identical output to the inline form, still one temp slot, still 0x3c).
//
// What this function does: the path object (vtable 0x4fd458, see
// 0x44f010.cpp; slot 3 is 0x44f150, slot 5 is 0x44f290) hands over the next
// three waypoints. Waypoint 1 is pulled back along the first segment when the
// unit is farther than 0x500000 from it, the turn toward it is applied and
// clamped to the type's max_turn, and then the distance this frame may travel
// is either the type's +0x19e or the negated rate at +0x19a.
//
// The slow-path amount is -type->field_19a, not -dz. At 0x43d03a
// `mov [esp+0x24],esi` runs after four pushes, so it writes [esp+0x14],
// overwriting the dz slot, and 0x43d0b4 `mov eax,[esp+0x14]; neg eax` reads
// that value back; dz is dead after 0x43ce8c. q's numerator is
// ((field_20*field_20) >> 16) << 16, not >> 32 << 16. lim is
// (turned*turned >> 32) * 4 where turned is the earlier 64-bit quotient
// (stored at 0x43d027, reloaded at 0x43d087); r is q*q >> 32.
//
// What helped: a local `Vec3* ppos = &unit->pos` used for every pos access
// (lever 6) reproduced the original's ebx = &unit->pos and, with the
// operator-/Square member functions added to Vec3 (the idiom the matched
// siblings 0x404730/0x414a80 use), the frame grew from 0x3c to the original
// 0x44 and the score rose from 50.7 to 58.6 (that variant reused the
// pre-branch ax/az for d1, which is wrong because the pull-back modifies
// p[1]; recomputing them is correct but scores 53.5).
//
// What still differs (first hunks): in the hasPath == 0 branch the original
// loads unit into ecx before storing turn, ours stores turn first; and after
// the v3 call the original computes ax = p[1].x - pos.x before az = p[1].z -
// pos.z (slots B+0x14, B+0x1c) while ours computes az first (slots B+0x10,
// B+0x04). unit ends in eax (original edi); d1 ends in ebp in both.
//
// Suspected original bug: none beyond the dead dz store and the reused
// argument home; the store of turned at 0x43d027 is live (0x43d087 reloads it).
//
// Also tried: inline (non-local) recomputation for d1 (49.0), Vec3 delta
// temporaries, and the header sets headers.py covers.

#include <math.h>
#include <stdlib.h>

struct Vec3 {
    int x, y, z;
    Vec3 operator-(const Vec3& other) const {
        Vec3 r; r.z = z - other.z; r.y = y - other.y; r.x = x - other.x; return r;
    }
    int Square() const {
        __int64 a = x, b = z;
        return (int)((a*a) >> 32) + (int)((b*b) >> 32);
    }
};

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
    Vec3* ppos = &unit->pos;

    int ax = p[1].x - ppos->x;
    int az = p[1].z - ppos->z;
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

    short diff = FUN_0048a980(ppos, &p[1]) - unit->heading;
    int sdiff = diff;
    int adiff = abs(sdiff);

    int bx = p[2].x - ppos->x;
    int bz = p[2].z - ppos->z;
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

