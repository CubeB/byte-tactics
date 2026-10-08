// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, gate polarity checked by space-bunny-free, gate spelling changed by space-bunny-free, finished by Claude Sonnet 5.5, finished by Claude Opus 5.5. Names are provisional.
#include <vector>
struct Vec3 { int x, y, z; };
struct Unit;
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_004388d0 { public: void SetAttachedFx(int); };
class Class_00438930 { public: void AttachApproachRadiusGoal(Vec3*, int); };
class Class_00439e80 { public: void SetDeadlineTicks(int); };
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
class DamagedAllyCollector {
public:
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
    DamagedAllyCollector(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void CollectDamagedAlly(Unit*);
};
void __stdcall EnsurePatrolReturnOrder(Unit*, Order*);
void __stdcall VisitObjectsInRange(Vec3*, int, const DamagedAllyCollector&);
int __stdcall RandomInt(int);
Class_00438760 __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
int __stdcall IssueRepairOrder(Unit*, Unit*, int);
int __stdcall PickRandomReclaimableResourcesInRadius(Vec3*, int, Vec3**, float*, Vec3**, float*);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);

// FUNCTION: 0x405980
int __stdcall RepairPatrolOrder(Unit* unit, Order* order, int flags)
{
    // The loop around the switch lets the cases share one `new` tail.
    for (;;) {
        switch (order->state) {
        case 0:
            if (order->target) order->pos = order->target->pos;
            EnsurePatrolReturnOrder(unit, order);
            return 1;
        case 1: {
            if (flags & 0xe0) return 6;
            ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos, 16);
            ((Class_00439e80*)order)->SetDeadlineTicks(60);
            order->flags |= 0xe0;
            if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
                std::vector<Unit*> units;
                int range = unit->def->range << 16;
                VisitObjectsInRange(&unit->pos, range, DamagedAllyCollector(unit->owner, &units, unit));
                if (!units.empty()) {
                    Unit* target = units[RandomInt(units.size())];
                    if (unit->owner->allied[target->owner->index]) {
                        Class_00438760 kind = GetOrderType(8, unit, target, 0);
                        if (kind.index) {
                            if (IssueRepairOrder(unit, target, 0)) return 6;
                            return 3;
                        }
                    }
                }
            }
            // Owner* local with `energy < 0.2 * capacity` in this operand order and `<`.
            Owner* owner = unit->owner;
            if (unit->owner->energy < 0.2 * owner->energyCapacity ||
                unit->owner->metal < unit->owner->metalCapacity * 0.2) {
                Vec3 energyPos, metalPos;
                Vec3* energy = &energyPos;
                Vec3* metal = &metalPos;
                int range2 = unit->def->range << 16;
                // Empty statement stops range2 being folded into the call.
                do {} while (0); // emits no code; keeps range2 out of the call (see top)
                float energyAmount, metalAmount;
                if (PickRandomReclaimableResourcesInRadius(&unit->pos, range2, &energy, &energyAmount, &metal, &metalAmount)) {
                    if (metal && unit->owner->metal < unit->owner->metalCapacity * 0.2) {
                        ((Class_004388d0*)order)->SetAttachedFx(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, metal, 0, 0, 0));
                        ((Class_004388d0*)order)->SetAttachedFx(0);
                        order->flags = 0;
                        return 3;
                    }
                    if (energy && unit->owner->energy < unit->owner->energyCapacity * 0.2) {
                        ((Class_004388d0*)order)->SetAttachedFx(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, energy, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                    if (metal && unit->owner->metal + metalAmount <= unit->owner->metalCapacity) {
                        ((Class_004388d0*)order)->SetAttachedFx(0);
                        AppendOrder(unit, new Class_0043a1f0("RECLAIM", 0, metal, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                    if (energy && unit->owner->energy + energyAmount <= unit->owner->energyCapacity) {
                        ((Class_004388d0*)order)->SetAttachedFx(0);
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

