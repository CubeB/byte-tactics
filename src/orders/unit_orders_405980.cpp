// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, gate polarity checked by space-bunny-free, gate spelling changed by space-bunny-free, finished by Claude Sonnet 5.5, finished by Claude Opus 5.5. Names are provisional.
// MATCH. What the bytes needed, for anyone working on a sibling:
// - The switch sits in a loop (`for (;;)`; `do { } while (0)` or `while (1)`
//   give the same bytes). Without one, B and C keep their own inline copy of
//   the `new Class_0043a1f0` tail (1081 bytes) instead of jumping to D's.
// - Second gate: `energy < 0.2 * owner->energyCapacity` with an `Owner*` local
//   (energy loaded first, `test ah, 0x41`). `<=` gives `test ah, 1`; the
//   `capE * 0.2` operand order loads the product first and adds an fxch.
// - range2 is computed after the two pointer stores and kept in eax across the
//   argument pushes. Written straight before the call, MSVC folds it into the
//   call (92.2%); the empty `do {} while (0);` (a debug macro that compiled to
//   nothing) between them stops that and gives the original's rotation.
#include <vector>
struct Vec3 { int x, y, z; };
struct Unit;
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438930 { public: void FUN_00438930(Vec3*, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
#pragma pack(push, 1)
struct UnitDef { char pad0[0x202]; short range; };
struct Owner {
    char pad0[0x8c]; float energy;
    char pad90[8]; float metal;
    char pad9c[8]; float energyCapacity, metalCapacity;
    char padac[0x108-0xac]; unsigned char allied[0x3e]; unsigned char index;
};
struct Unit { char pad0[0x6a]; Vec3 pos; char pad76[0x92-0x76]; UnitDef* def; Owner* owner; };
struct Order { char pad0[5]; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; };
#pragma pack(pop)
#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char data[0x56];
    Class_0043a1f0(Class_00438760, int, Vec3*, int, int, int);
};
#pragma pack(pop)
class Class_00405d90 {
public:
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
    Class_00405d90(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void FUN_00405d90(Unit*);
};
void __stdcall FUN_0043a020(Unit*, Order*);
void __stdcall VisitObjectsInRange(Vec3*, int, const Class_00405d90&);
int __stdcall RandomInt(int);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
int __stdcall FUN_0043b400(Unit*, Unit*, int);
int __stdcall FUN_0047ea40(Vec3*, int, Vec3**, float*, Vec3**, float*);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);

// FUNCTION: 0x405980
int __stdcall RepairPatrolOrder(Unit* unit, Order* order, int flags)
{
    for (;;) {
        switch (order->state) {
        case 0:
            if (order->target) order->pos = order->target->pos;
            FUN_0043a020(unit, order);
            return 1;
        case 1: {
            if (flags & 0xe0) return 6;
            ((Class_00438930*)order)->FUN_00438930(&order->pos, 16);
            ((Class_00439e80*)order)->FUN_00439e80(60);
            order->flags |= 0xe0;
            if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
                std::vector<Unit*> units;
                int range = unit->def->range << 16;
                VisitObjectsInRange(&unit->pos, range, Class_00405d90(unit->owner, &units, unit));
                if (!units.empty()) {
                    Unit* target = units[RandomInt(units.size())];
                    if (unit->owner->allied[target->owner->index]) {
                        Class_00438760 kind = FUN_0043f0e0(8, unit, target, 0);
                        if (kind.index) {
                            if (FUN_0043b400(unit, target, 0)) return 6;
                            return 3;
                        }
                    }
                }
            }
            Owner* owner = unit->owner;
            if (unit->owner->energy < 0.2 * owner->energyCapacity ||
                unit->owner->metal < unit->owner->metalCapacity * 0.2) {
                Vec3 energyPos, metalPos;
                Vec3* energy = &energyPos;
                Vec3* metal = &metalPos;
                int range2 = unit->def->range << 16;
                do {} while (0); // emits no code; keeps range2 out of the call (see top)
                float energyAmount, metalAmount;
                if (FUN_0047ea40(&unit->pos, range2, &energy, &energyAmount, &metal, &metalAmount)) {
                    if (metal && unit->owner->metal < unit->owner->metalCapacity * 0.2) {
                        ((Class_004388d0*)order)->FUN_004388d0(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, metal, 0, 0, 0));
                        ((Class_004388d0*)order)->FUN_004388d0(0);
                        order->flags = 0;
                        return 3;
                    }
                    if (energy && unit->owner->energy < unit->owner->energyCapacity * 0.2) {
                        ((Class_004388d0*)order)->FUN_004388d0(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, energy, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                    if (metal && unit->owner->metal + metalAmount <= unit->owner->metalCapacity) {
                        ((Class_004388d0*)order)->FUN_004388d0(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, metal, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                    if (energy && unit->owner->energy + energyAmount <= unit->owner->energyCapacity) {
                        ((Class_004388d0*)order)->FUN_004388d0(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, energy, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                }
            }
            return 2;
        }
        }
        return 7;
    }
}

