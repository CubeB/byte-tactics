// Decompiled by Claude Opus 5.5. Names are provisional.
// VTOL strafing attack order handler. With flags 0x10008, or with no target
// and order flag 0x200, it queues VTOL_SEEKATTACK; on the map-edge player it
// heads for the map centre. State 0 prepares the order ("Attacking";
// PrepVtolClimb). State 1 attacks:
// when the target lies ahead (IsAhead) it makes a strafing run, otherwise it
// circles, leading the target by its velocity, and after 90 ticks of that
// it queues VTOL_EVADE.
#include <math.h>

struct Vec3 {
    union { int x; struct { unsigned short xf; short xw; }; };
    int y;
    union { int z; struct { unsigned short zf; short zw; }; };
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r = *this; r += v; return r; }
};

union Fixed {
    int v;
    struct { unsigned short frac; short whole; } p;
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class UnitMotion {
public:
    char unknown_0[8];
    Vec3 v;                            // +0x8
    char unknown_14[0x2e - 0x14];
    unsigned char flags;               // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004388d0 { public: void SetAttachedFx(int); };
class Class_00438880 { public: void AnnounceStatusIfFlagged(const char*); };
class Class_00439e80 { public: void SetDeadlineTicks(int); };
class Class_0044e6c0 { public: void SetAltitude(int); };
class Class_0044e730 { public: void SetApproachRadius(short); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x192]; int maxvelocity;
    char pad196[0x21c - 0x196]; short altitude;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Unit {
    UnitMotion* type;
    char pad4[0x66 - 4]; short heading;
    char pad68[2];
    union {
        Vec3 pos;
        struct { unsigned short xf; short x; int y; unsigned short zf; short z; } p;
    };
    char pad76[0x82 - 0x76];
    int spatialBucket; int carrier;
    char pad8a[8]; UnitDef* def;
    char pad96[0x110 - 0x96]; unsigned int flags;
    void ClaimWeapons(int);
    void ReleaseWeapons(int);
    void SetStateBits(int, int);
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x22 - 0x1a]; Vec3 pos;
    short x; short z;
    char pad32[0x36 - 0x32]; int field_36;
    char pad3a[0x3e - 0x3a]; int range;
    unsigned int field_42;
    char pad46[4]; int field_4a;
};
struct Game {
    char pad0[0x1422b]; int width; int height;
    char pad14233[0x142b7 - 0x14233]; int field_142b7;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, int a, Vec3* b, int c, int d, int e);
};
class Class_0044e740 {
public:
    char unknown_0[0x2c];
    Class_0044e740(Order* order, const Vec3& a, const Vec3& b);
    void SetAltitude(int);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
void __stdcall AppendOrderToTail(Unit*, Class_0043a1f0*);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);

// scale is a by-value 4-byte union: callers load the constant into a register first.
Vec3 __stdcall DirectionFromAngle(short angle, Fixed scale);
Vec3 __stdcall AddVec3(const Vec3& a, const Vec3& b);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

static inline Vec3 Add(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall PrepVtolClimb(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->carrier)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->flags & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude / 2);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Stays an inline helper: its locals then share stack slots with the later ones.
static inline int IsAhead(Unit* unit, Order* order)
{
    Vec3 toward = Offset(GetHeadingBetween(&unit->pos, &order->target->pos), 0x140000);
    Fixed dist;
    dist.v = 0x140000;
    Vec3 facing = DirectionFromAngle(unit->heading, dist);
    return (short)(toward.xw * facing.xw + toward.zw * facing.zw) > 0;
}

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x412d40
int __stdcall AirToAirOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 0x10008) {
        if (order->field_4a == 0 && (unit->flags & 0x300000))
            AppendOrderToTail(unit, new Class_0043a1f0("VTOL_SEEKATTACK", (int)order->target, &order->pos, 0, 0, 0));
        return 5;
    }
    if (order->target == 0 && (order->field_42 & 0x200)) {
        if (order->field_4a == 0)
            AppendOrderToTail(unit, new Class_0043a1f0("VTOL_SEEKATTACK", 0, &unit->pos, 0, 0, 0));
        return 5;
    }
    if (unit->spatialBucket == g_game->field_142b7) {
        Vec3 centre;
        centre.x = g_game->width / 2 << 16;
        centre.z = g_game->height / 2 << 16;
        short angle = GetHeadingBetween(&unit->pos, &centre);
        Vec3 dest = AddVec3(unit->pos, Offset(angle, 0x3200000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->SetApproachRadius(0x80);
        order->flags |= 0xe0;
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        return 2;
    }
    if (order->range && (int)_hypot(unit->p.x - order->x, unit->p.z - order->z) >= order->range)
        return 5;
    switch (order->state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Attacking");
            PrepVtolClimb(unit, order, 0);
            ((Class_00439e80*)order)->SetDeadlineTicks(1);
            order->field_36 = 0;
            return 1;
        }
        break;
    case 1:
        unit->ReleaseWeapons(3);
        unit->ClaimWeapons(0);
        SetWeaponTargetUnit(unit, order->target, 0);
        if (flags & 0xe0) {
            if (IsAhead(unit, order)) {
                Vec3 from = Add(unit->pos, Offset(unit->heading, unit->def->maxvelocity * 30));
                Vec3 to = Offset(unit->heading, unit->def->maxvelocity);
                Class_0044e740* obj = new Class_0044e740(order, from, to);
                obj->SetAltitude(unit->def->altitude);
                ((Class_004388d0*)order)->SetAttachedFx((int)obj);
                ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(0x1e) + 0x3c);
                order->field_36 = 0;
                return 2;
            }
        }
        if (!(flags & 0xe0) && order->field_36 < 0x5a) {
            if (IsAhead(unit, order))
                order->field_36 = 0;
            else
                order->field_36 += 0x2d;
            Fixed d;
            d.v = (int)_hypot(unit->pos.x - order->target->pos.x, unit->pos.z - order->target->pos.z);
            if (d.p.whole > 0xa0) {
                Vec3 p = order->target->pos;
                p.x += order->target->type->v.x * 45;
                p.z += order->target->type->v.z * 45;
                ((Class_004388d0*)order)->SetAttachedFx((int)new Class_0044e740(order, p,
                    order->target->type->v + Offset(order->target->heading, order->target->def->maxvelocity / 2)));
            }
            ((Class_00439e80*)order)->SetDeadlineTicks(0x2d);
            order->flags |= 0x100e8;
            return 2;
        }
        ((Class_004388d0*)order)->SetAttachedFx(0);
        AppendOrder(unit, new Class_0043a1f0("VTOL_EVADE", (int)order->target, 0, 0, 0, 0));
        order->field_36 = 0;
        order->flags = 0;
        return 0;
    }
    return 7;
}
