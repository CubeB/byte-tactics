// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1, matched by Claude Opus 5.5. Names are provisional.
// VTOL attack order handler for a unit target. With flags 0x10008, or with
// no target and order flag 0x200, it queues VTOL_SEEKATTACK; on the map-edge
// player it heads for the map centre. State 0 prepares the order
// ("Attacking"; PrepVtolClimb), state 1 flies to a random point halfway to
// the target, state 2 attacks, state 3 circles the target, alternating
// sides, and lands on a free pad when damaged (VTOL_LANDING).
//
// Suspected original bug: that same branch builds a Class_0044e2d0 waypoint
// and sets its speed, but never passes it to the order (no SetAttachedFx
// call, unlike every other branch), so the object leaks.
#include <math.h>
#include <vector>

struct Vec3 {
    union { int x; struct { unsigned short xf; short xw; }; };
    int y;
    union { int z; struct { unsigned short zf; short zw; }; };
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    void operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r = *this; r += v; return r; }
    Vec3 operator-(const Vec3& v) const { Vec3 r = *this; r -= v; return r; }
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class UnitMotion {
public:
    char unknown_0[0x2e];
    unsigned char flags;               // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004388d0 { public: void SetAttachedFx(int); };
class Class_00438880 { public: void AnnounceStatusIfFlagged(const char*); };
class Class_0044e6c0 { public: void SetAltitude(int); };
class Class_0044e730 { public: void SetApproachRadius(short); };

#pragma pack(push, 1)
#include "../units/unit_def.h"
struct Mover {
    char pad0[0xdc]; int speed;
};
struct Player {
    char pad0[0x146]; unsigned char index;
};
struct Unit {
    UnitMotion* type;
    char pad4[0x10 - 4]; Mover* mover;
    char pad14[0x6a - 0x14];
    Vec3 pos;
    char pad76[0x82 - 0x76];
    int spatialBucket; int carrier;
    char pad8a[8]; UnitDef* def;
    Player* player;
    char pad9a[0x108 - 0x9a]; short health;
    char pad10a[0x110 - 0x10a]; unsigned int flags;
    void ClaimWeapons(int);
    void ReleaseWeapons(int);
    void SetStateBits(int, int);
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x22 - 0x1a]; Vec3 pos;
    short x; short z;
    char pad32[0x36 - 0x32]; int side; int misses;
    int range;
    unsigned int field_42;
    char pad46[4]; int field_4a;
};
struct Game {
    char pad0[0x1422b]; int width; int height;
    char pad14233[0x142b7 - 0x14233]; int overflowBucket;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e330 {
public:
    char unknown_0[0x36];
    Class_0044e330(Order* order, Unit* unit, const Vec3& p);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, int a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, int);
void __stdcall AppendOrderToTail(Unit*, Class_0043a1f0*);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
void __stdcall GetFactoriesInRadius(int player, Vec3* pos, int range, std::vector<Unit*>* out);
Vec3 __stdcall AddVec3(const Vec3& a, const Vec3& b);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// Copies each component and adds to it before reading the next, unlike
// operator+, which copies the whole vector first. Used at the one site whose
// original schedule consumes off.x before it loads target->pos.y.
static inline Vec3 Add(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x;
    r.x += b.x;
    r.y = a.y;
    r.y += b.y;
    r.z = a.z;
    r.z += b.z;
    return r;
}

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

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x413470
int __stdcall AirToGroundHoverOrder(Unit* unit, Order* order, int flags)
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
    if (unit->spatialBucket == g_game->overflowBucket) {
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
    // short& references: they fix the first _hypot's load order.
    short& ox = order->x;
    short& oz = order->z;
    if (order->range && (int)_hypot(unit->pos.xw - ox, unit->pos.zw - oz) >= order->range)
        return 5;
    int speed = unit->mover->speed;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags1 & 0x800)) {
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Attacking");
            PrepVtolClimb(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        unit->ReleaseWeapons(3);
        int dist = (int)_hypot(order->target->pos.x - unit->pos.x, order->target->pos.z - unit->pos.z);
        int angle = GetHeadingBetween(&unit->pos, &order->target->pos);
        Vec3 off = Offset(RandomInt(0x4000) + angle - 0x2000, dist / 2);
        Vec3 p = unit->pos + off;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->SetApproachRadius(0x80);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 2: {
        unit->ClaimWeapons(0);
        SetWeaponTargetUnit(unit, order->target, 0);
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->target->pos);
        ((Class_0044e730*)obj)->SetApproachRadius(speed);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100e8;
        order->side = 0;
        order->misses = 0;
        return 1;
    }
    case 3: {
        if (!WeaponCanReachUnit(unit, order->target, 0))
            order->misses++;
        if (order->misses >= 2) {
            order->misses = 0;
            Vec3 off = Offset(RandomInt(0x10000), speed << 16);
            Vec3 p = Add(order->target->pos, off);
            // The new waypoint is never given to the order (see the header).
            Class_0044e2d0* obj = new Class_0044e2d0(order, p);
            ((Class_0044e730*)obj)->SetApproachRadius(0x80);
            order->flags |= 0x110e8;
            return 2;
        }
        int a = GetHeadingBetween(&unit->pos, &order->target->pos);
        int angle;
        if (order->side) {
            angle = a + 0x2000;
            order->side = 0;
        } else {
            angle = a - 0x2000;
            order->side = 1;
        }
        Vec3 off = Offset(angle, speed * 2 / 3 << 16);
        Vec3 p = order->target->pos - off;
        Class_0044e330* obj = new Class_0044e330(order, order->target, p);
        ((Class_0044e730*)obj)->SetApproachRadius(0x10);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100e8;
        if ((unsigned int)unit->health < (unit->def->maxHealth >> 2) * 3) {
            std::vector<Unit*> v;
            GetFactoriesInRadius(unit->player->index, &unit->pos, 0xf00, &v);
            if (!v.empty()) {
                ((Class_004388d0*)order)->SetAttachedFx(0);
                int target = (int)v[RandomInt(v.size())];
                AppendOrder(unit, new Class_0043a1f0("VTOL_LANDING", target, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
        }
        return 2;
    }
    }
    return 7;
}
