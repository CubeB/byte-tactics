// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// VTOL patrol order handler ("Patrolling"). State 0 prepares the order
// (PrepVtolClimb), state 1 clears the order's 0xe0 bits, state 2 flies to a
// point 0x140 units away along the heading to the order's position, lands on a
// free pad when damaged (VTOL_LANDING, as in 0x412710), or takes the next
// queued order.
// Kept: the flags/health block's register allocation depends on this include.
#include <windows.h>
#include <vector>

// Kept local, not util/vec3.h: the header's symbol ids change this function's registers.
struct Vec3 {
    int x, y, z;
};

#include "mission_type.h"

struct Unit;
class UnitMotion {
public:
    char unknown_0[0x2e];
    unsigned char flags;               // +0x2e
    void SetFlightMode(Unit* unit, int state);
};

class Class_0044e6c0 { public: void SetAltitude(int); };
class Class_0044e730 { public: void SetApproachRadius(short); };

#pragma pack(push, 1)
#include "../units/unit_def.h"
#include "../network/player.h"
struct Unit {
    UnitMotion* motion;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x86 - 0x76]; int carrier;
    char pad8a[8]; UnitDef* def;
    Player* player;
    char pad9a[0x108 - 0x9a]; short health;
    void ClaimWeapons(int);
    void ReleaseWeapons(int);
    void SetStateBits(int, int);
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x22 - 0xa]; Vec3 pos;
    char pad2e[0x4e - 0x2e]; unsigned int subFlags;
    void AnnounceStatusIfFlagged(const char*);
    void SetAttachedFx(int);
    void SetDeadlineTicks(int);
    Order(MissionType type, int a, Vec3* b, int c, int d, int e);
    char unknown_52[0x4];
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    ~Order();
    Order(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall AppendOrder(Unit*, Order*);
void __stdcall EnsurePatrolReturnOrder(Unit*, Order*);
Unit* __stdcall FindBestTargetIfFireAtWill(Unit*);
int __stdcall IssueAttackOrder(Unit*, Unit*, int);
void __stdcall GetFactoriesInRadius(int player, Vec3* pos, int range, std::vector<Unit*>* out);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall VtolMoveOrder(Unit*, Order*, int);
int __stdcall VtolFollowOrder(Unit*, Order*, int);
int __stdcall VtolStandbyOrder(Unit*, Order*, int);
int __stdcall VtolLandIfCanOrder(Unit*, Order*, int);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// 0x40f790, matched in 0x40f790.cpp; /Ob2 inlines it into state 2.
Vec3 __stdcall AddVec3(const Vec3& a, const Vec3& b)
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
    if ((unit->motion->flags & 3) == 1) {
        unit->motion->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude / 2);
        order->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

static inline void Dummy(void) {}
// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x410e70
int __stdcall VtolPatrolOrder(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 2: {
        if (flags & 0xe0)
            return 6;
        short angle = GetHeadingBetween(&unit->pos, &order->pos);
        Vec3 dest = AddVec3(order->pos, Offset(angle, 0x1400000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->SetApproachRadius(0x150);
        order->SetAttachedFx((int)obj);
        order->flags |= 0xe0;
        // Landing block stays inline, no helper: the landed path returns 0
        // through the plain scope-exit destructor.
        if ((unsigned int)unit->health < (unit->def->maxHealth >> 2) * 3) {
            std::vector<Unit*> v;
            GetFactoriesInRadius(unit->player->index, &unit->pos, 0xf00, &v);
            if (!v.empty()) {
                order->SetAttachedFx(0);
                Unit* target = v[RandomInt(v.size())];
                AppendOrder(unit, new Order("VTOL_LANDING", (int)target, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
        }
        // Empty Dummy() calls use up the inline budget so both vector
        // destructor sites call _Destroy out of line.
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
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Unit* next = FindBestTargetIfFireAtWill(unit);
        if (next && IssueAttackOrder(unit, next, 0)) {
            order->flags = 0;
            return 3;
        }
        order->SetDeadlineTicks(0x1e);
        return 2;
    }
    case 1:
        order->subFlags &= ~0xe0;
        return 1;
    case 0:
        if (unit->motion && (unit->def->flags1 & 0x800)) {
            EnsurePatrolReturnOrder(unit, order);
            order->AnnounceStatusIfFlagged("Patrolling");
            PrepVtolClimb(unit, order, 0);
            unit->ReleaseWeapons(3);
            return 1;
        }
        break;
    }
    return 7;
}
