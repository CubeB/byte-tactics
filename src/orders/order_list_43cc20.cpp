// Decompiled by space-bunny-free, Space Bunny Free, deepseek-v4.1, deepseek-v4.1-flash, mimo-v2.6-pro, Opus, Sonnet and Claude Opus 5.5. Names are provisional.
// A unit's mover (the 0x2f-byte object at unit+0): its velocity, speed and
// turn, the steering for ground units and aircraft, and the position update.
//
// <stdlib.h> and <memory.h> must stay: header state UpdatePosition and
// SteerAircraft depend on.
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#pragma pack(push, 1)
struct FP_0043d6d0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_0043d6d0 {
    int value;
    FP_0043d6d0 parts;
};
#pragma pack(pop)

static inline Fixed_0043d6d0 MakeFixed_0043d6d0(int i)
{
    Fixed_0043d6d0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

inline int operator>(const Fixed_0043d6d0& a, const Fixed_0043d6d0& b) { return a.value > b.value; }

#define MAXM_0043d6d0(a, b) ((a) > (b) ? (a) : (b))

static inline void MulFixed(int& v, int f)
{
    v = (int)(((__int64)v * f) >> 16);
}

// A 16.16 position or velocity.
struct Vec3 {
    int x;
    union {
        int y;
        Fixed_0043d6d0 fy;
        struct {
            unsigned short yFraction;
            short yWhole;
        };
    };
    int z;

    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
    int Length() const {
        double a = x, b = y, c = z;
        return (int)sqrt(a * a + b * b + c * c);
    }
    Vec3 operator-(const Vec3& o) const {
        Vec3 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Vec3& operator+=(const Vec3& o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    void Scale(int s) {
        MulFixed(x, s);
        MulFixed(y, s);
        MulFixed(z, s);
    }
    void Add(const Vec3* o)
    {
        x += o->x;
        y += o->y;
        z += o->z;
    }
};

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

struct Point {
    short x, y;
    Point() {}
    Point(int a, int b) : x(a), y(b) {}
};

#pragma pack(push, 1)
struct Short3 {
    short x, y, z;
};

struct Game {
    char unknown_0[0x14263];
    int count2;                        // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int field_142b7;                   // +0x142b7
    char unknown_142bb[0x38a47 - 0x142bb];
    int field_38a47;                   // +0x38a47
};

struct UnitType_0043cd20 {
    char unknown_0[0x192];
    int field_192;                     // +0x192, the range
    char unknown_196[0x19a - 0x196];
    int field_19a;                     // +0x19a, the rate (top speed)
    int field_19e;                     // +0x19e, the long-step distance (acceleration)
    int field_1a2;                     // +0x1a2
    int field_1a6;                     // +0x1a6
    char unknown_1aa[0x1ae - 0x1aa];
    int field_1ae;                     // +0x1ae
    int field_1b2;                     // +0x1b2
    int field_1b6;                     // +0x1b6
    unsigned short max_turn;           // +0x1ba
    char unknown_1bc[0x22c - 0x1bc];
    unsigned char draft;               // +0x22c
    char unknown_22d[0x241 - 0x22d];
    union {
        int field_241;                 // +0x241
        struct {
            unsigned int mode_bits : 11;
            unsigned int flag_800 : 1; // bit 11
        };
        struct {
            unsigned int low : 19;
            unsigned int b19 : 1;      // bit 19
            unsigned int rest : 12;
        };
    };
};

struct Target_0043d6d0 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
};

struct TargetData_0043d6d0 {
    char unknown_0[8];
    Vec3 velocity;                     // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                      // +0x20
};

struct Path_0043d6d0 {
    TargetData_0043d6d0* field_0;      // +0x0
};
#pragma pack(pop)

class CobScript { public: void StartScript(const char*, int, int); };

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x64];
    union {
        Short3 f64;                    // +0x64
        struct {
            short field_64;            // +0x64
            short heading;             // +0x66
            short field_68;            // +0x68, in 2048ths of a circle
        };
    };
    Vec3 pos;                          // +0x6a
    Point cell;                        // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point draft;                       // +0x7e
    int field_82;                      // +0x82
    Path_0043d6d0* obj;                // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0043cd20* type;           // +0x92
    Target_0043d6d0* target;           // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short id;                 // +0xa8
    char unknown_aa[0xf9 - 0xaa];
    signed char index;                 // +0xf9
    char unknown_fa[0x110 - 0xfa];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;     // +0x110 bits 0-1
            unsigned int flags_2 : 14;
            unsigned int moved : 1;    // +0x110 bit 16
            unsigned int flags_17 : 15;
        };
    };

    void SetStateBits(int param_1, int param_2);
};

// The movement state saved as "u%04xmob".
struct Record_0043dd70 {
    Vec3 velocity;                     // +0x00
    Vec3 p2;                           // +0x0c
    int field_20;                      // +0x18
    short field_24;                    // +0x1c
    int field_26;                      // +0x1e
    unsigned char mode : 2;            // +0x22 bits 0-1
    unsigned char flag : 1;            // +0x22 bit 2
};
#pragma pack(pop)

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int size);
    int WriteBox(void* src, int len);
};

extern Game* g_game;
extern signed char DAT_00505205[];

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __cdecl FUN_004b7173(short angle, int* xz);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
Vec3 __stdcall GetPiecePosition(Path_0043d6d0* obj, int index);
Short3 __stdcall GetPieceAngles(Path_0043d6d0* obj, int index);
void __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int mode);
int __stdcall FUN_0047db70(UnitType_0043cd20* type, short a8, Point cell, int mode);
void __stdcall RemoveUnitFromMap(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall UpdateUnitLineOfSight(Unit* unit);

// The behaviour object at +0x0: the path the mover follows.
class Iface_0043dd20 {
public:
    virtual void v0(int);
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* out, int first, int count);
    virtual void v4(Vec3* a, Vec3* b, short* heading);
    virtual int v5();
};

class Class_00490880 { public: char unknown[0x27]; Class_00490880(Unit* unit); };
class Class_004907e0 { public: char unknown[0x28]; Class_004907e0(Unit* unit); };
class Class_0044f570 { public: char unknown[0x1c]; Class_0044f570(Unit* unit); };
class Class_0044f010 { public: char unknown[0x65]; Class_0044f010(Unit* unit); };
class Class_0043db50 { public: void UpdateSfxOccupy(Unit* u); };

static inline void ClampToZero(int& value)
{
    if (value < 0)
        value = 0;
}

static inline void AddFixed(int& v, float f)
{
    v += (int)((double)f * 65536.0);
}

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

static inline void ClampToCell(Vec3& pos, Point cell, Point draft)
{
    int cx = (draft.x + cell.x * 2) << 19;
    int cz = (draft.y + cell.y * 2) << 19;
    if (pos.x > cx + 0x7ffff)
        pos.x = cx + 0x7ffff;
    else if (pos.x < cx - 0x7ffff)
        pos.x = cx - 0x7ffff;
    if (pos.z > cz + 0x7ffff)
        pos.z = cz + 0x7ffff;
    else if (pos.z < cz - 0x7ffff)
        pos.z = cz - 0x7ffff;
}

#pragma pack(push, 1)
class UnitMotion {
public:
    Iface_0043dd20* obj;               // +0x0
    int field_4;                       // +0x4
    Vec3 velocity;                     // +0x8
    Vec3 p2;                           // +0x14
    int field_20;                      // +0x20, speed
    short field_24;                    // +0x24, turn this tick
    int field_26;                      // +0x26
    int field_2a;                      // +0x2a
    union {
        unsigned char field_2e;        // +0x2e
        struct {
            unsigned char mode : 2;    // bits 0-1
            unsigned char flag : 1;    // bit 2
            unsigned char rest : 5;
        };
    };

    void FUN_0043cc20(Unit* unit, int amount);
    void SteerGroundUnit(Unit* unit);
    void ApplyBankAndPitch(Unit* owner, Vec3* v);
    void SetFlightMode(Unit* owner, int state);
    void SteerAircraft(Unit* unit);
    void UpdatePosition(Unit* unit);
    void UpdateMoveRate(Unit* unit);
    UnitMotion(Unit* unit);
    void DestroyObject();
    void UpdateMotion(Unit* u);
    void SaveMotion(Unit* info, HapiBank* file);
    void LoadMotion(Unit* unit, HapiBank* file);
};
#pragma pack(pop)

// Adds `amount` to the object's distance accumulator (clamped at zero), limits
// it to a range taken from the table at DAT_00505205, then writes the offset
// for the unit's heading at that distance into the velocity.
// FUNCTION: 0x43cc20
void UnitMotion::FUN_0043cc20(Unit* unit, int amount)
{
    field_20 = field_20 + amount;
    ClampToZero(field_20);

    // Direction index, limited to the eleven entries of the table.
    int idx = unit->field_68 >> 11;
    if (idx < -5)
        idx = -5;
    if (idx > 5)
        idx = 5;

    // 16.16 range from the table entry, halved below sea level.
    int range = (int)(((__int64)(DAT_00505205[idx] << 16) * unit->type->field_192) >> 16);
    range = (int)(((__int64)range << 16) / 0x640000);
    if (unit->pos.yWhole < g_game->seaLevel && !(unit->type->field_241 & 0x81000))
        range = (int)(((__int64)range * 0x8000) >> 16);
    if (field_20 > range)
        field_20 = range;

    int dist = field_20;
    unsigned short angle = unit->heading;
    Vec3 v;
    v.x = -FUN_004b70ef(angle, dist);
    v.y = 0;
    v.z = -FUN_004b7123(angle, dist);
    velocity = v;
}

// Decompiled by Space Bunny Free, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// FUNCTION: 0x43cd20
void UnitMotion::SteerGroundUnit(Unit* unit)
{
    if (obj->v5() == 0) {
        field_24 = 0;
        // Bound temporary: loads unit before the turn store and keeps the rate in eax.
        const int& amount = -unit->type->field_19a;
        FUN_0043cc20(unit, amount);
        return;
    }

    Vec3 p[3];
    obj->v3(p, 0, 3);

    Vec3* ppos = &unit->pos;
    Vec3 d = p[1] - *ppos;
    int gap1 = (int)_hypot(d.x, d.z);

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

    int az = p[1].z - unit->pos.z;
    int ax = p[1].x - unit->pos.x;
    int d1 = (int)(((__int64)ax * ax) >> 32) + (int)(((__int64)az * az) >> 32);

    short ang = (short)GetHeadingBetween(ppos, &p[1]);
    short diff = ang - unit->heading;
    int sdiff = diff;
    int adiff = abs(sdiff);

    int bz = p[2].z - unit->pos.z;
    int bx = p[2].x - unit->pos.x;
    int d2 = (int)(((__int64)bx * bx) >> 32) + (int)(((__int64)bz * bz) >> 32);

    if (diff != 0) {
        unsigned short max = unit->type->max_turn;
        if (sdiff >= max)
            field_24 = max;
        else if (sdiff <= -max)
            field_24 = -max;
        else
            field_24 = diff;
        unit->heading += field_24;
        unit->moved = 1;
    } else {
        field_24 = 0;
    }

    // field_20 is copied to spd below so its sign extension is not shared with this
    // multiply; otherwise it becomes _allmul instead of a one-operand imul.
    int turned = (int)((((__int64)(adiff & 0xffff) * (__int64)field_20)
                        / unit->type->max_turn));
    int rate = unit->type->field_19a;
    int spd = field_20;
    int t = (int)(((__int64)spd * spd) >> 16);
    int q = (int)(((__int64)t << 16) / (2 * rate));
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)(((__int64)turned * turned) >> 32) * 4;

    // FUN_0043cc20 stays defined above this function so the two calls cross-jump.
    if (d1 > lim && d2 > r)
        FUN_0043cc20(unit, unit->type->field_19e);
    else
        FUN_0043cc20(unit, -rate);
}

// Scales the object's second vector (+0x14) by 0.95 (16.16 fixed point), adds the
// offset `v`, rotates the (x, z) pair by the owner's heading, then derives two
// short offsets from the rotated components and the owner type's fields at
// +0x1a2/+0x1a6.
//
// Suspected original bug: the second FUN_004b715a call reads xz[0] (the rotated
// x component) instead of xz[1], so field_68 (the z offset) is computed from
// the rotated x.
// The original calls this from SetFlightMode and SteerAircraft.
#pragma auto_inline(off)
// FUNCTION: 0x43d0d0
void UnitMotion::ApplyBankAndPitch(Unit* owner, Vec3* v)
{
    // Scale and Add stay inlined methods, not separate statements on p2:
    // each field is then stored immediately.
    p2.Scale(0xf333);
    p2.Add(v);
    int xz[2];
    xz[0] = p2.x;
    xz[1] = p2.z;
    FUN_004b7173(owner->heading, xz);
    int n = (int)(((__int64)g_game->count2 << 16) / 0xccd);
    owner->field_64 = (short)FUN_004b715a((int)(((__int64)owner->type->field_1a2 * -xz[0]) >> 16), n);
    owner->field_68 = (short)FUN_004b715a((int)(((__int64)owner->type->field_1a6 * -xz[0]) >> 16), n);
}
#pragma auto_inline(on)

// Changes the 2-bit mode at +0x2e; entering state 1 clears the velocity and
// field_20 and clears flag 1 on the owner, any other state sets it.
// FUNCTION: 0x43d210
void UnitMotion::SetFlightMode(Unit* owner, int newState)
{
    if (mode != newState) {
        if (newState == 1) {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
            ApplyBankAndPitch(owner, &zero);
            owner->SetStateBits(1, 0);
        } else {
            owner->SetStateBits(1, 1);
        }
        mode = newState;
    }
}

// Steering update for the mover object (UnitMotion): in mode 2 it asks the
// path object for a target position `a`, a target velocity `b` and a heading,
// damps the velocity, clamps its horizontal speed (bleeding the excess off
// along the unit's heading), turns the unit towards the heading, then steers
// p1 towards the target: k * (pos - a) - (velocity - b), limited to the acceleration
// f18. Finally it stores the new speed and hands the velocity change on.
// FUNCTION: 0x43d290
void UnitMotion::SteerAircraft(Unit* unit) {
    if (mode != 2) {
        velocity = Vec3(0, 0, 0);
        field_20 = 0;
        field_24 = 0;
        return;
    }

    Vec3 old = velocity;
    Vec3 a;
    Vec3 b;
    short heading;
    obj->v4(&a, &b, &heading);

    UnitType_0043cd20* type = unit->type;
    const float eps = 1.52587890625e-05f;

    float f18 = (float)type->field_19e * eps;
    int q = (int)(((__int64)type->field_19e << 16) / type->field_192);
    int scale = 0x10000 - q;
    velocity.Scale(scale);

    float dist = (float)_hypot(velocity.x, velocity.z) * eps;
    float maxd = (float)unit->type->field_19a * eps;
    if (dist > maxd) {
        int f = (int)((double)(maxd / dist) * 65536.0);
        // MulFixed and AddFixed take the component by reference; written
        // inline the clamp differs.
        MulFixed(velocity.x, f);
        MulFixed(velocity.z, f);
        int g = (int)((double)(dist - maxd) * 65536.0);
        velocity += Offset(unit->f64.y, g);
    }

    Vec3 da = unit->pos - a;
    Vec3 db = velocity - b;

    if (unit->field_82 != g_game->field_142b7) {
        int lim;
        if ((field_20 & -4) < 0x40000)
            lim = 0x10000;
        else
            lim = field_20 >> 2;
        if (da.y <= -lim)
            velocity.y = lim;
        else if (da.y >= lim)
            velocity.y = -lim;
        else
            velocity.y = -da.y;
    }

    float hd = (float)_hypot(da.x, da.z) * eps;
    short d = (short)(heading - unit->f64.y);
    if (d != 0) {
        unsigned short max = unit->type->max_turn;
        if (d >= (int)max)
            field_24 = max;
        else if (d <= -(int)max)
            field_24 = (short)-max;
        else
            field_24 = d;
        unit->f64.y = (short)(unit->f64.y + field_24);
        unit->moved = 1;
    } else {
        field_24 = 0;
    }

    if (hd < 8.0f)
        hd = 8.0f;

    float k = -sqrt((2.0f * f18) / hd);
    float vx = (float)da.x * k * eps - (float)db.x * eps;
    float vz = (float)da.z * k * eps - (float)db.z * eps;
    float mag = (float)_hypot(vx, vz);
    if (mag > f18) {
        vx = vx * (f18 / mag);
        vz = vz * (f18 / mag);
    }

    AddFixed(velocity.x, vx);
    AddFixed(velocity.z, vz);
    field_20 = velocity.Length();

    // Own block so the address-taken delta shares a stack slot with db.
    {
        Vec3 delta = velocity - old;
        ApplyBankAndPitch(unit, &delta);
    }
}

// Moves the unit one step. With a path object it snaps to the path's next
// point (raised to the draft/sea-level floor for units with the type flag at
// +0x241 bit 19) and copies the path's velocity; without one it adds the
// velocity to the position, and if the unit moves into a new cell that the
// target says is blocked it clamps the position to the current cell and caps
// the speed at half the type's range.
// FUNCTION: 0x43d6d0
void UnitMotion::UpdatePosition(Unit* u)
{
    if (u->obj != 0) {
        // Copy through the returned pointer, not an initialiser.
        Vec3 v;
        v = GetPiecePosition(u->obj, u->index);
        if (u->type->b19) {
            // MAX stays a macro over the Fixed union with a prvalue second operand.
            v.fy = MAXM_0043d6d0(v.fy, MakeFixed_0043d6d0(u->type->draft * 0xffff + g_game->seaLevel));
        }
        SetUnitPosition(u, v, mode);
        Short3 o = GetPieceAngles(u->obj, u->index);
        u->f64 = o;
        if (u->obj->field_0 != 0) {
            field_20 = u->obj->field_0->field_20;
            velocity = u->obj->field_0->velocity;
        } else {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
        }
        u->moved = 0;
        return;
    }

    // Assigned (not initialised) through the inline operator+: its result temporary
    // fixes the frame layout.
    Vec3 pos;
    pos = velocity + u->pos;
    int m = mode;
    if (pos.x == u->pos.x && pos.z == u->pos.z && pos.y == u->pos.y && m == u->mode)
        return;

    field_2a = g_game->field_38a47;
    Point draft = u->draft;
    // Field by field, with draft.x * 0x80000 (a << 19 evaluates draft.x first).
    Point cell;
    cell.x = (pos.x - draft.x * 0x80000 + 0x80000) >> 20;
    cell.y = (pos.z - draft.y * 0x80000 + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == u->mode) {
        u->pos = pos;
        u->moved = 1;
        return;
    }

    if (u->target->field_0 != 0) {
        if (u->target->type == 1 || u->target->type == 2)
            flag = FUN_0047db70(u->type, u->id, cell, m) == 0;
    }

    if (flag) {
        // ClampToCell stays an inline helper taking both Points by value.
        ClampToCell(pos, u->cell, u->draft);

        if (field_20 > (u->type->field_192 / 2)) {
            int half = u->type->field_192 / 2;
            field_20 = half;
            unsigned short angle = u->f64.y;
            Vec3 vec;
            vec.x = -FUN_004b70ef(angle, half);
            vec.y = 0;
            // z goes through an int temporary.
            int z = -FUN_004b7123(angle, half);
            vec.z = z;
            velocity = vec;
        }
        u->pos = pos;
        u->moved = 1;
        return;
    }

    RemoveUnitFromMap(u);
    u->pos = pos;
    u->cell = cell;
    // The (short) cast gives the and/and/or store of the 2-bit field.
    u->mode = (short)m;
    AddUnitToMap(u);
    u->moved = 1;
    UpdateUnitLineOfSight(u);
}

// FUNCTION: 0x43da70
void UnitMotion::UpdateMoveRate(Unit* unit)
{
    int rate;
    if ((field_2e & 4) == 0 && unit->obj == 0
        && (field_20 != 0 || field_24 != 0)) {
        if (field_20 <= unit->type->field_1ae) {
            rate = 1;
        } else {
            rate = 2 + (field_20 > unit->type->field_1b2);
        }
    } else {
        rate = 0;
    }
    if (rate == (int)((unit->flags >> 2) & 3))
        return;
    if (rate == 0) {
        unit->script->StartScript("StopMoving", rate, 1);
    } else if ((unit->flags & 0xc) == 0) {
        unit->script->StartScript("StartMoving", 0, 1);
    }
    switch (rate) {
    case 1:
        unit->script->StartScript("MoveRate1", 0, 1);
        break;
    case 2:
        unit->script->StartScript("MoveRate2", 0, 1);
        break;
    case 3:
        unit->script->StartScript("MoveRate3", 0, 1);
        break;
    }
    unit->flags = (unit->flags & 0xfffffff3) | ((rate & 3) << 2);
}

// Constructor of the 0x2f-byte behaviour holder stored at unit+0. It zeroes two
// Vec3-shaped triples and a few scalars, mirrors a value from the unit type and
// then creates one of four behaviour objects, chosen by the unit's target type
// (byte at +0x73 == 3) and bit 11 of the unit type's flags at +0x241.
// FUNCTION: 0x43dc00
UnitMotion::UnitMotion(Unit* unit)
{
    velocity = Vec3(0, 0, 0);
    field_20 = 0;
    field_24 = 0;
    field_26 = 0;
    p2 = Vec3(0, 0, 0);
    mode = 1;
    flag = 0;
    field_4 = unit->type->field_1b6;
    if (unit->target->field_0 != 0 && unit->target->type == 3) {
        if (unit->type->flag_800)
            obj = (Iface_0043dd20*)new Class_00490880(unit);
        else
            obj = (Iface_0043dd20*)new Class_0044f570(unit);
    } else {
        if (unit->type->flag_800)
            obj = (Iface_0043dd20*)new Class_004907e0(unit);
        else
            obj = (Iface_0043dd20*)new Class_0044f010(unit);
    }
}

// FUNCTION: 0x43dd10
void UnitMotion::DestroyObject()
{
    if (obj != 0) {
        obj->v0(1);
    }
}

// FUNCTION: 0x43dd20
void UnitMotion::UpdateMotion(Unit* u)
{
    obj->v2();
    if (u->type->flag_800)
        SteerAircraft(u);
    else
        SteerGroundUnit(u);
    UpdatePosition(u);
    UpdateMoveRate(u);
    ((Class_0043db50*)this)->UpdateSfxOccupy(u);
}

// FUNCTION: 0x43dd70
void UnitMotion::SaveMotion(Unit* info, HapiBank* file)
{
    char name[32];
    Record_0043dd70 hdr;
    hdr.velocity = velocity;
    hdr.p2 = p2;
    hdr.field_20 = field_20;
    hdr.field_24 = field_24;
    hdr.field_26 = field_26;
    hdr.mode = mode;
    hdr.flag = flag;
    sprintf(name, "u%04xmob", info->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 0x23);
}

// Load counterpart of 0x43dd70: reads this unit type's movement state back
// from the unit's "u%04xmob" entry.
// FUNCTION: 0x43de30
void UnitMotion::LoadMotion(Unit* unit, HapiBank* file)
{
    char name[32];
    Record_0043dd70 rec;
    sprintf(name, "u%04xmob", unit->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->ReadBox(&rec, 0x23);
    velocity = rec.velocity;
    p2 = rec.p2;
    field_20 = rec.field_20;
    field_24 = rec.field_24;
    field_26 = rec.field_26;
    mode = rec.mode;
    flag = rec.flag;
}
