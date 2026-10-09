// Decompiled by Claude Opus 5.5. Names are provisional.
// "Landing" order handler of air units landing on a pad unit (the order's
// target). State 0 prepares the order (PrepVtolClimb) and picks a random
// angle, state 1 looks for a free pad (FindLandingPad) and circles the pad
// unit until one is free, states 2 to 5 approach the pad and land, and state
// 6 hands the unit (or its cargo) to the pad and starts a "SELFREPAIR" order
// when the pad repairs and the unit is damaged.
// Kept: some header set is needed here or the state 6 health compare changes.
#include <windows.h>
#include <math.h>

struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& other) const {
        Vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        return r;
    }
};

struct Unit;
#include "unit_motion.h"
class PathOrderAttach {
public:
    Unit* owner;                       // +0x4
    PathOrderAttach* next;             // +0x8
    int value;                         // +0xc
    virtual ~PathOrderAttach();
    void SetUnit(Unit* o);
};
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };

class Class_0044e6c0 { public: void SetAltitude(int); };
class Class_0044e730 { public: void SetApproachRadius(short); };
#include "../units/cob_script.h"

// Unused here: these forward declarations take the symbol ids that keep VtolLandingOrder (0x4118e0) matching
// (docs/c2-regalloc.md).
struct BmpFileHeader;
struct BmpInfo;
struct BmpInfoHeader;
struct CalcedExplosion;
struct FrameTable;
struct Chunk;
class DamagedAllyCollector;
class SquadManager;
class AssaultTimer;
class EscortTimer;
class SquadScoutTimer;
class SpatialTimer;
class ScoutTimer;
class OpenHeap;
class LandingPadList;
#pragma pack(push, 1)
#include "../units/unit_def.h"
struct WeaponDef {
    char pad0[0xdc]; int range;
};
struct Unit {
    UnitMotion* type;
    char pad4[0x10 - 4]; WeaponDef* weapon;   // +0x10, weapons[0].def
    char pad14[0x6a - 0x14]; Vec3 pos;
    char pad76[8]; Point footprint;
    int spatialBucket; int carrier;
    Unit* cargo;
    char pad8e[4]; UnitDef* def;
    char pad96[4]; CobScript* script;
    char pad9e[0x104 - 0x9e]; float buildLeft;
    short health;
    void ClaimWeapons(int);
    void SetStateBits(int, int);
};
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall ClearWeaponTarget(Unit*, int);
void __stdcall DetonateUnitWeapon(Unit*, int);
void __stdcall DrawUnit(void*, Unit*);
void __stdcall KillUnit(Unit*, int);

struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x12 - 0xa]; PathOrderAttach target;
    Vec3 pos;
    char pad2e[8]; int angle;
    void AnnounceStatusIfFlagged(const char*);
    void SetAttachedFx(int);
    void SetDeadlineTicks(int);
    Order(Class_00438760 type, Unit* target, void* pos, int c, int d, int e);
    char unknown_3a[0x1c];
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    ~Order();
    Order(Unit* unit, void* file, char* name);
    int SerializeToSave(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    void AttachApproachRadiusGoal(Vec3* pos, int radius);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
};
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall DirectionFromStep(int, int);
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e250 {
public:
    char unknown_0[0x36];
    // The pad index parameters are int, not char: the original loads a full dword.
    Class_0044e250(Order* order, Unit* unit, int value);
};
#pragma pack(pop)

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall IsPadSlotFree(Unit* unit, int id);
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, int p3, int p4);
void __stdcall AppendOrder(Unit*, Order*);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
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
        order->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// 0x411840, matched in 0x411840.cpp; inlined into the cases below.
int __stdcall FindLandingPad(Unit* unit, int pad)
{
    if (pad != -1 && IsPadSlotFree(unit, pad)) {
        return pad;
    }
    int pads[4];
    pads[0] = -1;
    pads[1] = -1;
    pads[2] = -1;
    pads[3] = -1;
    unit->script->QueryScript("QueryLandingPad", &pads[0], &pads[1], &pads[2], &pads[3]);
    for (int i = 0; i < 4; i++) {
        if (pads[i] != -1 && IsPadSlotFree(unit, pads[i])) {
            return pads[i];
        }
    }
    return -1;
}

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x4118e0
int __stdcall VtolLandingOrder(Unit* unit, Order* order, int flags)
{
    int radius = unit->weapon->range;
    Unit* host = order->target.owner;
    if (!host) {
        QueueUnitSpeech(unit, 7, "Landing aborted");
        return 8;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags1 & 0x800)) {
            order->AnnounceStatusIfFlagged("Landing");
            PrepVtolClimb(unit, order, 0);
            order->angle = RandomInt(0x10000);
            return 1;
        }
        break;
    case 1: {
        if (FindLandingPad(order->target.owner, FindLandingPad(host, -1)) != -1) {
            order->state = 2;
            return 2;
        }
        Vec3 dest = order->target.owner->pos + Offset(order->angle, radius << 16);
        order->angle += 0x4000;
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->SetApproachRadius(0x80);
        order->SetAttachedFx((int)obj);
        order->flags = 0xe8;
        order->state = 1;
        return 2;
    }
    case 2: {
        Class_0044e250* obj = new Class_0044e250(order, order->target.owner, -1);
        ((Class_0044e730*)obj)->SetApproachRadius(0xa0);
        order->SetAttachedFx((int)obj);
        order->flags = 0xe8;
        return 1;
    }
    case 3: {
        int pad = FindLandingPad(host, -1);
        order->angle = pad;
        if (pad == -1) {
            QueueUnitSpeech(unit, 7, "Landing failed");
            return 0;
        }
        Class_0044e250* obj = new Class_0044e250(order, order->target.owner, order->angle);
        ((Class_0044e730*)obj)->SetApproachRadius(0x30);
        order->SetAttachedFx((int)obj);
        order->flags = 0xe8;
        return 1;
    }
    case 4:
        return 1;
    case 5: {
        if (flags & 0x20)
            return 1;
        int pad = FindLandingPad(host, order->angle);
        order->angle = pad;
        if (pad == -1) {
            QueueUnitSpeech(unit, 7, "Landing aborted: all pads are occupied");
            return 0;
        }
        Class_0044e250* obj = new Class_0044e250(order, order->target.owner, order->angle);
        if (unit->cargo)
            ((Class_0044e6c0*)obj)->SetAltitude(unit->cargo->def->field_170);
        else
            ((Class_0044e6c0*)obj)->SetAltitude(0);
        unit->script->StartScript("EndTransport", 0, 1);
        order->SetAttachedFx((int)obj);
        order->SetDeadlineTicks(0xf);
        order->state = 5;
        order->flags |= 0xe8;
        return 2;
    }
    case 6: {
        int dropped = 0;
        if (flags & 0x40)
            return 8;
        if (!IsPadSlotFree(host, order->angle)) {
            QueueUnitSpeech(unit, 7, "Landing aborted: no pads available");
            return 0;
        }
        if (unit->cargo) {
            unit->script->StartScript("EndTransport", 0, 0);
            AttachUnitToPiece(unit->cargo, order->target.owner, order->angle, 0);
            dropped = 1;
        } else {
            AttachUnitToPiece(unit, order->target.owner, order->angle, 0);
        }
        if (!dropped && unit->health < unit->def->maxHealth
            && (order->target.owner->def->flags1 & 0x200)
            && (order->target.owner->def->flags1 & 0x40)
            && order->target.owner->buildLeft == 0.0f) {
            order->SetAttachedFx(0);
            AppendOrder(unit, new Order("SELFREPAIR", order->target.owner, 0, 0, 0, 0));
        }
        return 5;
    }
    }
    return 7;
}
