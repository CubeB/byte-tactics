// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: 64.1%. Visitor initialization scheduling and reclaim branch tails differ.
#include <vector>
struct Vec3 { int x, y, z; };
struct Unit;
struct Elem_00406c10 { Unit* ptr; };
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
class Class_0043a0c0 {
public:
    char data[0x56];
    Class_0043a0c0(Class_00438760, Unit*, Vec3*, int, int, int);
};
#pragma pack(pop)
class Class_00405d90 {
public:
    virtual void FUN_00405d90(Unit*);
    Owner* owner;
    std::vector<Elem_00406c10>* units;
    Unit* self;
    Class_00405d90() {}
};
void __stdcall FUN_0043a020(Unit*, Order*);
void __stdcall FUN_0047e890(Vec3*, int, Class_00405d90*);
int __stdcall FUN_004b6c30(int);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
int __stdcall FUN_0043b400(Unit*, Unit*, int);
int __stdcall FUN_0047ea40(Vec3*, int, Vec3**, float*, Vec3**, float*);
void __stdcall FUN_0043acb0(Unit*, Class_0043a0c0*);
// FUNCTION: 0x405980
int __stdcall FUN_00405980(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (order->target) order->pos = order->target->pos;
        FUN_0043a020(unit, order);
        return 1;
    case 1: {
        if (flags & 0xe0) return 6;
        ((Class_00438930*)order)->FUN_00438930(&order->pos, 16);
        ((Class_00439e80*)order)->FUN_00439e80(60);
        order->flags |= 0xe0;
        Class_00405d90 visitor;
        if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
            std::vector<Elem_00406c10> units;
            visitor.owner = unit->owner;
            visitor.units = &units;
            visitor.self = unit;
            FUN_0047e890(&unit->pos, unit->def->range << 16, &visitor);
            if (!units.empty()) {
                Unit* target = units[FUN_004b6c30(units.size())].ptr;
                if (unit->owner->allied[target->owner->index]) {
                    Class_00438760 kind = FUN_0043f0e0(8, unit, target, 0);
                    if (kind.index) {
                        if (FUN_0043b400(unit, target, 0)) return 6;
                        return 3;
                    }
                }
            }
        }
        if (unit->owner->energy < unit->owner->energyCapacity * 0.2 ||
            unit->owner->metal < unit->owner->metalCapacity * 0.2) {
            Vec3 energyPos, metalPos;
            Vec3* energy = &energyPos;
            Vec3* metal = &metalPos;
            float energyAmount, metalAmount;
            if (FUN_0047ea40(&unit->pos, unit->def->range << 16, &energy, &energyAmount, &metal, &metalAmount)) {
                if (metal && unit->owner->metal < unit->owner->metalCapacity * 0.2) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a0c0("RECLAIM", 0, metal, 0, 0, 0));
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    order->flags = 0;
                    return 3;
                }
                if (energy && unit->owner->energy < unit->owner->energyCapacity * 0.2) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a0c0("RECLAIM", 0, energy, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (metal && unit->owner->metal + metalAmount <= unit->owner->metalCapacity) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a0c0("RECLAIM", 0, metal, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (energy && unit->owner->energy + energyAmount <= unit->owner->energyCapacity) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a0c0("RECLAIM", 0, energy, 0, 0, 0));
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
