// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6, verified by GPT-6.1-Sol, finished by space-bunny-free, edited by deepseek-v4.1-flash, edited by Claude Opus 5.5, matched by Claude Opus 5.5. Names are provisional.
// VTOL patrol order handler for construction aircraft. State 0 starts
// patrolling ("Patrolling"; FUN_0040f200 is defined here because /Ob2
// inlined it). State 1 sets the next waypoint, then lands on a free pad when
// damaged (VTOL_LANDING), helps build or guards a unit it can see
// (VTOL_HELPBUILD), or reclaims metal or energy (VTOL_RECLAIM).
//
// MATCH (Claude Opus 5.5, #5602). Three things were needed together:
//  - The switch sits in a `for (;;)` loop, as in the matched RECLAIM handler
//    0x405980 (no `continue`, so no back edge and every block keeps weight
//    1). Only then do all four reclaim arms, each written with its own
//    `AppendOrder(unit, new ...); order->flags = 0; return 3;`, cross-jump
//    into the last arm's constructor tail. Without the loop MSVC merges at
//    most two arms into the last one; a loop around case 1's body or around
//    the reclaim chain alone does not merge them either. Matched functions
//    with three or more arms cross-jumped into one call tail (0x408830,
//    0x4ae630, 0x405980) all have the arms inside a loop.
//  - `flags` is `unsigned int`, as in the sibling handler 0x414a80. Then the
//    0xe0 in `flags & 0xe0` and in `order->flags |= 0xe0` is one constant,
//    used twice on one path, so C2 makes it a register candidate (it still
//    ends up an immediate). That adds one candidate to the set-up block and
//    lifts order's priority above case 0's unit web (tools/c2prio.py), so
//    order takes esi and unit edi as in the original. With `int flags` the
//    two 0xe0 differ in type, neither is a candidate, and the registers swap
//    (89.1%). `(unsigned int)flags & 0xe0` gives the same MATCH.
//  - Land returns 1 or 0 and has an empty `do {} while (0);` (a debug macro
//    that compiled to nothing) after the health test, with FUN_0040f200
//    `inline` so nothing is compiled before VtolRepairPatrolOrder. The landed test
//    then folds away and the landed path returns 0 straight after ~vector.
//    Any function compiled first in the file (FUN_0040f200 out of line
//    included) brings the test back (1520 bytes). Land's /Ob2 share keeps the
//    pads vector's constructor, empty()'s size() and ~vector out of line
//    while pads.size() inlines; a Done(unit, order, obj) helper per arm adds
//    four call sites and pushes size() out of line.
// The reads of the amounts through Owner::GetEnergy/GetMetal and Total give
// the arms' x87 load order.
#include <vector>

struct Vec3 { int x, y, z; };

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class Class_00410830 : public std::vector<Unit*> {};

class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004898b0 { public: void ClaimWeapons(int); };
class Class_0048b090 { public: void SetStateBits(int, int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_004899b0 { public: int CanRepair(Unit*); };

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
    Class_0043d210* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Owner* owner;
    char pad9a[0x104 - 0x9a]; float progress;
    short health;
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

int __stdcall FUN_004b6c30(int);
void __stdcall FUN_0043a020(Unit*, Order*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
void __stdcall GetFactoriesInRadius(int player, Vec3* pos, int range, std::vector<Unit*>* out);
void __stdcall VisitObjectsInRange(Vec3*, int, const Class_004158d0&);
int __stdcall FUN_0043b400(Unit*, Unit*, int);
union Fixed { int v; struct { unsigned short frac; short whole; } p; };
int __stdcall FUN_0047ea40(Vec3*, Fixed, Vec3**, float*, Vec3**, float*);

inline void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0((Order*)order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

static inline int Land(Unit* unit, Order* order)
{
    if ((unsigned int)unit->health < (unit->def->maxHealth >> 2) * 3) {
        Class_00410830 pads;
        GetFactoriesInRadius(unit->owner->index, &unit->pos, 0xf00, &pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Unit* pad = pads[FUN_004b6c30(pads.size())];
            AppendOrder(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
            order->flags = 0;
            return 1;
        }
    }
    do {} while (0);
    return 0;
}

static inline float Total(float base, float amount)
{
    float value = base;
    value += amount;
    return value;
}

// FUNCTION: 0x4152f0
int __stdcall VtolRepairPatrolOrder(Unit* unit, Order* order, unsigned int flags)
{
    if ((flags & 0x48) != 0) {
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 0;
    }
    for (;;) {
        switch (order->state) {
        case 0:
            if (unit->type && (unit->def->flags & 0x800) && (unit->def->flags2 & 0x200)) {
                if (order->target != 0)
                    order->pos = order->target->pos;
                FUN_0043a020(unit, order);
                ((Class_00438880*)order)->FUN_00438880("Patrolling");
                FUN_0040f200(unit, order, 0);
                return 1;
            }
            break;
        case 1: {
            if (flags & 0xe0)
                return 6;
            Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            ((Class_00439e80*)order)->FUN_00439e80(0x2d);
            order->flags |= 0xe0;
            if (Land(unit, order))
                return 0;
            if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
                std::vector<Unit*> units;
                int range = unit->def->range << 16;
                VisitObjectsInRange(&unit->pos, range, Class_004158d0(unit->owner, &units, unit));
                if (!units.empty()) {
                    Unit* target = units[FUN_004b6c30(units.size())];
                    if (((Class_004899b0*)unit)->CanRepair(target) && target->progress == 0.0f) {
                        if (FUN_0043b400(unit, target, 0))
                            return 6;
                        return 3;
                    }
                    if (((Class_004899b0*)unit)->CanRepair(target) && target->progress != 0.0f) {
                        ((Class_004388d0*)order)->FUN_004388d0(0);
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
            if (FUN_0047ea40(&unit->pos, range, &energy, &energyAmount, &metal, &metalAmount)) {
                if (unit->owner->GetMetal() < unit->owner->metalCapacity * 0.2 && metal) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (unit->owner->GetEnergy() < unit->owner->energyCapacity * 0.2 && energy) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (metal && Total(unit->owner->GetMetal(), metalAmount) <= unit->owner->metalCapacity) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    AppendOrder(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (energy && Total(unit->owner->GetEnergy(), energyAmount) <= unit->owner->energyCapacity) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
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
