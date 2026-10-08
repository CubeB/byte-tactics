// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6, verified by GPT-6.1-Sol, finished by space-bunny-free, edited by deepseek-v4.1-flash, edited by Claude Opus 5.5, matched by Claude Opus 5.5. Names are provisional.
// VTOL patrol order handler for construction aircraft. State 0 starts
// patrolling ("Patrolling"; FUN_0040f200). State 1 sets the next waypoint,
// then lands on a free pad when damaged (VTOL_LANDING), helps build or guards
// a unit it can see (VTOL_HELPBUILD), or reclaims metal or energy
// (VTOL_RECLAIM).
#include <vector>

struct Vec3 { int x, y, z; };

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class Class_00410830 : public std::vector<Unit*> {};

class UnitMotion {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004388d0 { public: void SetAttachedFx(int); };
class Class_00438880 { public: void AnnounceStatusIfFlagged(const char*); };
class Class_00439e80 { public: void SetDeadlineTicks(int); };
class Class_0044e6c0 { public: void SetAltitude(int); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int maxHealth;
    char pad1fe[0x202 - 0x1fe]; short range;
    char pad204[0x21c - 0x204]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
    unsigned int flags2;
};
struct Owner {
    char pad0[0x8c]; float energy;
    float GetEnergy() { return energy; }
    char pad90[8]; float metal;
    float GetMetal() { return metal; }
    char pad9c[8]; float energyCapacity, metalCapacity;
    char padac[0x146 - 0xac]; unsigned char index;
};
struct Unit {
    UnitMotion* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Owner* owner;
    char pad9a[0x104 - 0x9a]; float progress;
    short health;
    void ClaimWeapons(int);
    void SetStateBits(int, int);
    int CanRepair(Unit*);
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x22 - 0x1a]; Vec3 pos;
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
    Class_0043a1f0(Class_00438760 type, Unit* a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

class Class_004158d0 {
public:
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
    Class_004158d0(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void FUN_004158d0(Unit*);
};

int __stdcall RandomInt(int);
void __stdcall EnsurePatrolReturnOrder(Unit*, Order*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
void __stdcall GetFactoriesInRadius(int player, Vec3* pos, int range, std::vector<Unit*>* out);
void __stdcall VisitObjectsInRange(Vec3*, int, const Class_004158d0&);
int __stdcall IssueRepairOrder(Unit*, Unit*, int);
union Fixed { int v; struct { unsigned short frac; short whole; } p; };
int __stdcall PickRandomReclaimableResourcesInRadius(Vec3*, Fixed, Vec3**, float*, Vec3**, float*);

// inline: no function may be compiled before VtolRepairPatrolOrder, or the landed test returns.
inline void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0((Order*)order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

static inline int Land(Unit* unit, Order* order)
{
    if ((unsigned int)unit->health < (unit->def->maxHealth >> 2) * 3) {
        Class_00410830 pads;
        GetFactoriesInRadius(unit->owner->index, &unit->pos, 0xf00, &pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->SetAttachedFx(0);
            Unit* pad = pads[RandomInt(pads.size())];
            AppendOrder(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
            order->flags = 0;
            return 1;
        }
    }
    // Empty statement after the health test folds the landed test away.
    do {} while (0);
    return 0;
}

static inline float Total(float base, float amount)
{
    float value = base;
    value += amount;
    return value;
}

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x4152f0
int __stdcall VtolRepairPatrolOrder(Unit* unit, Order* order, unsigned int flags)
{
    // flags is unsigned int: the repeated 0xe0 then keeps order in esi.
    if ((flags & 0x48) != 0) {
        ((Class_00439e80*)order)->SetDeadlineTicks(0x1e);
        return 0;
    }
    // The loop lets the four reclaim arms share the last arm's constructor tail.
    for (;;) {
        switch (order->state) {
        case 0:
            if (unit->type && (unit->def->flags & 0x800) && (unit->def->flags2 & 0x200)) {
                if (order->target != 0)
                    order->pos = order->target->pos;
                EnsurePatrolReturnOrder(unit, order);
                ((Class_00438880*)order)->AnnounceStatusIfFlagged("Patrolling");
                FUN_0040f200(unit, order, 0);
                return 1;
            }
            break;
        case 1: {
            if (flags & 0xe0)
                return 6;
            Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
            ((Class_0044e6c0*)obj)->SetAltitude(unit->def->field_21c);
            ((Class_004388d0*)order)->SetAttachedFx((int)obj);
            ((Class_00439e80*)order)->SetDeadlineTicks(0x2d);
            order->flags |= 0xe0;
            if (Land(unit, order))
                return 0;
            if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
                std::vector<Unit*> units;
                int range = unit->def->range << 16;
                VisitObjectsInRange(&unit->pos, range, Class_004158d0(unit->owner, &units, unit));
                if (!units.empty()) {
                    Unit* target = units[RandomInt(units.size())];
                    if (unit->CanRepair(target) && target->progress == 0.0f) {
                        if (IssueRepairOrder(unit, target, 0))
                            return 6;
                        return 3;
                    }
                    if (unit->CanRepair(target) && target->progress != 0.0f) {
                        ((Class_004388d0*)order)->SetAttachedFx(0);
                        AppendOrder(unit, new Class_0043a1f0("VTOL_HELPBUILD", target, 0, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                }
            }
            Vec3 energyPos, metalPos;
            Vec3* energy = &energyPos;
            Vec3* metal = &metalPos;
            float energyAmount, metalAmount;
            Fixed range;
            range.v = 0xf00000;
            if (PickRandomReclaimableResourcesInRadius(&unit->pos, range, &energy, &energyAmount, &metal, &metalAmount)) {
                // Amounts read through GetEnergy/GetMetal and Total: gives the x87 load order.
                if (unit->owner->GetMetal() < unit->owner->metalCapacity * 0.2 && metal) {
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (unit->owner->GetEnergy() < unit->owner->energyCapacity * 0.2 && energy) {
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (metal && Total(unit->owner->GetMetal(), metalAmount) <= unit->owner->metalCapacity) {
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (energy && Total(unit->owner->GetEnergy(), energyAmount) <= unit->owner->energyCapacity) {
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
            }
            return 2;
        }
        }
        return 7;
    }
}
