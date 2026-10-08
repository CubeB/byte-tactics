// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, matched by Claude Opus 5.5. Names are provisional.
// VTOL attack order handler ("Attacking"). With flags 0x1000a, or with no
// target and order flag 0x200, it queues VTOL_SEEKATTACK instead; when out of
// the order's range it gives up. State 0 prepares the order (PrepVtolClimb),
// state 1 flies to a random point halfway to the target, state 2 attacks,
// state 3 pulls away from the target, state 4 lands on a free pad when damaged
// (VTOL_LANDING) or circles.
// Kept: the health test's registers follow the file's symbol count, which the
// IsDamaged helper and this include set fix.


#include <math.h>
#include <vector>

struct Vec3 {
    int x, y, z;
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r = *this; r += v; return r; }
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
class Class_00439e80 { public: void SetDeadlineTicks(int); };
class Class_0044e6c0 { public: void SetAltitude(int); };
class Class_0044e730 { public: void SetApproachRadius(short); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int maxHealth;
    char pad1fe[0x21c - 0x1fe]; short altitude;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Mover {
    char pad0[0xdc]; int speed;
};
// Unused here: these forward declarations take the symbol ids that keep 0x412710 matching (docs/c2-regalloc.md).
struct Sound;
struct HapiBank;
struct TdfFile;
struct TdfRecord;
struct Mission;
struct Gadget;
struct Layer;
struct Weapon;
struct Feature;
#include "../network/player.h"
struct Unit {
    UnitMotion* type;
    char pad4[0x10 - 4]; Mover* mover;
    char pad14[0x66 - 0x14]; short heading;
    char pad68[2];
    union {
        Vec3 pos;
        struct { unsigned short xf; short x; int y; unsigned short zf; short z; } p;
    };
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
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
void __stdcall AlignUnitToGround(Unit*);

struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x22 - 0x1a]; Vec3 pos;
    short x; short z;
    char pad32[0x3e - 0x32]; int range;
    unsigned int field_42;
    char pad46[4]; int field_4a;
};
struct Game {
    char pad0[0x142b7]; int field_142b7;
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
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
void __stdcall SetWeaponTargetPos(Unit*, Vec3*, int);
void __stdcall AppendOrderToTail(Unit*, Class_0043a1f0*);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
void __stdcall GetFactoriesInRadius(int player, Vec3* pos, int range, std::vector<Unit*>* out);

// The landing pad list. 0x410830 is its constructor; its implicit destructor
// is an inline candidate under 41 IL, so ~vector sits one level down.
class LandingPadList : public std::vector<Unit*> {};

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// Empty inline call sites: see the header.
static inline void Dummy(void) {}
static inline int IsDamaged(Unit* u) { return (unsigned int)u->health < (u->def->maxHealth >> 2) * 3; }
static inline int GetSpeed(Unit* unit)
{
    return unit->mover->speed;
}

void __stdcall PrepVtolClimb(Unit* unit, Order* order, unsigned int flags)
{
    ((Unit*)unit)->ClaimWeapons(3);
    if (unit->carrier)
        AttachUnitToPiece(unit, 0, -1, 2);
    ((Unit*)unit)->SetStateBits(1, 1);
    if ((unit->type->flags & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude / 2);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x412710
int __stdcall AirToGroundOrder(Unit* unit, Order* order, int flags)
{
    int speed = GetSpeed(unit);
    if (flags & 0x1000a) {
        if (order->field_4a == 0 && (unit->flags & 0x300000))
            AppendOrderToTail(unit, new Class_0043a1f0("VTOL_SEEKATTACK", (int)order->target, &order->pos, 0, 0, 0));
        return 5;
    }
    if (order->target == 0 && (order->field_42 & 0x200)) {
        if (order->field_4a == 0)
            AppendOrderToTail(unit, new Class_0043a1f0("VTOL_SEEKATTACK", 0, &unit->pos, 0, 0, 0));
        return 5;
    }
    if (order->target)
        order->pos = order->target->pos;
    if (unit->spatialBucket == g_game->field_142b7) {
        ((Class_00439e80*)order)->SetDeadlineTicks(0x1e);
        order->state = 2;
    }
    if (order->range && (int)_hypot(unit->p.x - order->x, unit->p.z - order->z) >= order->range)
        return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Attacking");
            PrepVtolClimb(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        ((Unit*)unit)->ReleaseWeapons(3);
        int dist = (int)_hypot(order->pos.x - unit->pos.x, order->pos.z - unit->pos.z);
        int angle = GetHeadingBetween(&unit->pos, &order->pos);
        Vec3 off = Offset(RandomInt(0x4000) + angle - 0x2000, dist / 2);
        Vec3 p = unit->pos + off;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->SetApproachRadius(0x80);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 2: {
        ((Unit*)unit)->ClaimWeapons(0);
        if (order->target)
            SetWeaponTargetUnit(unit, order->target, 0);
        else
            SetWeaponTargetPos(unit, &order->pos, 0);
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e730*)obj)->SetApproachRadius(speed);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 3: {
        short angle = FUN_004b715a(unit->pos.x - order->pos.x, unit->pos.z - order->pos.z);
        Vec3 off = Offset(angle, speed * 0x30000);
        Vec3 p = order->pos + off;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->SetApproachRadius(RandomInt(0x80) + 0x80);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100ea;
        return 1;
    }
    case 4: {
        if (IsDamaged(unit)) {
            LandingPadList v;
            GetFactoriesInRadius(unit->player->index, &unit->pos, 0xf00, &v);
            if (!v.empty()) {
                ((Class_004388d0*)order)->SetAttachedFx(0);
                Unit* target = v[RandomInt(v.size())];
                AppendOrder(unit, new Class_0043a1f0("VTOL_LANDING", (int)target, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
        }
        short angle = RandomInt(2) ? unit->heading + 0x4000 : unit->heading - 0x4000;
        Vec3 off = Offset(angle, speed << 16);
        Vec3 sum;
        sum.x = unit->pos.x + off.x;
        sum.y = unit->pos.y + off.y;
        sum.z = unit->pos.z + off.z;
        Vec3 p = sum;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->SetApproachRadius(0x80);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0x100ea;
        // The empty Dummy() calls make the landed ~vector call _Destroy out of
        // line and the empty one inline it.
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        return 1;
    }
    case 5:
        order->state = 2;
        return 2;
    }
    return 7;
}
