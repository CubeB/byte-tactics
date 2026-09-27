// Decompiled by Claude Opus 5.5. Names are provisional.
// Partial: 80.4%. Order handler "Reclaim"-style assist: with enough energy it
// looks for an allied unit to help (visitor 0x405d90); when energy or metal is
// low it asks FUN_0047ea40 for the best features to reclaim.
// What now matches (vs the earlier 64.1% attempt):
// - The visitor is a temporary passed by reference, so it gets its own frame
//   slot (0x30) that the part-2 locals do not overlap, and `int range` computed
//   after the vector declaration schedules the vector, visitor and pushes
//   exactly as the original (a named visitor inside the block shrinks the frame
//   to 0x24; one at case scope hoists its vtable store above the if).
// - `int range2` before the part-2 buffers fixes the register choice in the
//   first reclaim branch (metal pointer in ecx).
// What still differs:
// - The part-2 energy test: the original loads energy first (`fld [0x8c]; fld
//   [0xa4]; fmul`, as in the part-1 test), ours loads energyCapacity*0.2 first
//   and adds an `fxch`. `>=`/`<`/`!()`, a local Owner*, double casts, inline
//   helpers and all 64 header sets give the same.
// - The part-2 setup: the original loads unit->def first but does `movsx`
//   after the two pointer stores; with range2 ours does `movsx` before them,
//   and without it the def load moves after the pushes.
// - Branch 2 (energy below 20%) keeps its own copy of the constructor/
//   FUN_0043acb0 tail; the original jumps into the tail shared by branches 3
//   and 4. An if/else-if chain with one `Class_0043a0c0* node` and a single
//   FUN_0043acb0(unit, node) after it merges the tails exactly (1041 bytes)
//   but swaps edi/ebp for order and the new pointer (70.1%).
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
class Class_0043a0c0 {
public:
    char data[0x56];
    Class_0043a0c0(Class_00438760, int, Vec3*, int, int, int);
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
void __stdcall FUN_0047e890(Vec3*, int, const Class_00405d90&);
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
        if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
            std::vector<Unit*> units;
            int range = unit->def->range << 16;
            FUN_0047e890(&unit->pos, range, Class_00405d90(unit->owner, &units, unit));
            if (!units.empty()) {
                Unit* target = units[FUN_004b6c30(units.size())];
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
            int range2 = unit->def->range << 16;
            Vec3 energyPos, metalPos;
            Vec3* energy = &energyPos;
            Vec3* metal = &metalPos;
            float energyAmount, metalAmount;
            if (FUN_0047ea40(&unit->pos, range2, &energy, &energyAmount, &metal, &metalAmount)) {
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
