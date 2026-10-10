// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, gate polarity checked by space-bunny-free, gate spelling changed by space-bunny-free, finished by Claude Sonnet 5.5, finished by Claude Opus 5.5. Names are provisional.
// Stays in its own file: in unit_orders.cpp the stack address setup for the reclaim search arguments is scheduled differently.
#include <vector>
#include "../util/vec3.h"
struct Unit;
#include "mission_type.h"

#pragma pack(push, 1)
#include "../units/unit_def.h"
#include "../network/player.h"
struct Unit { char pad0[0x6a]; Vec3 pos; char pad76[0x92-0x76]; UnitDef* def; Player* player; };
#include "order.h"
#pragma pack(pop)
class DamagedAllyCollector {
public:
    Player* owner;
    std::vector<Unit*>* units;
    Unit* self;
    DamagedAllyCollector(Player* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void CollectDamagedAlly(Unit*);
};
void __stdcall EnsurePatrolReturnOrder(Unit*, Order*);
void __stdcall VisitObjectsInRange(Vec3*, int, const DamagedAllyCollector&);
int __stdcall RandomInt(int);
MissionType __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
int __stdcall IssueRepairOrder(Unit*, Unit*, int);
int __stdcall PickRandomReclaimableResourcesInRadius(Vec3*, int, Vec3**, float*, Vec3**, float*);
void __stdcall AppendOrder(Unit*, Order*);

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
            order->AttachApproachRadiusGoal(&order->pos, 16);
            order->SetDeadlineTicks(60);
            order->flags |= 0xe0;
            if (unit->player->energy >= unit->player->energyCapacity * 0.2) {
                std::vector<Unit*> units;
                int range = unit->def->range << 16;
                VisitObjectsInRange(&unit->pos, range, DamagedAllyCollector(unit->player, &units, unit));
                if (!units.empty()) {
                    Unit* target = units[RandomInt(units.size())];
                    if (unit->player->allied[target->player->index]) {
                        MissionType kind = GetOrderType(8, unit, target, 0);
                        if (kind.index) {
                            if (IssueRepairOrder(unit, target, 0)) return 6;
                            return 3;
                        }
                    }
                }
            }
            // Player* local with `energy < 0.2 * capacity` in this operand order and `<`.
            Player* owner = unit->player;
            if (unit->player->energy < 0.2 * owner->energyCapacity ||
                unit->player->metal < unit->player->metalCapacity * 0.2) {
                Vec3 energyPos, metalPos;
                Vec3* energy = &energyPos;
                Vec3* metal = &metalPos;
                int range2 = unit->def->range << 16;
                // Empty statement stops range2 being folded into the call.
                do {} while (0); // emits no code; keeps range2 out of the call (see top)
                float energyAmount, metalAmount;
                if (PickRandomReclaimableResourcesInRadius(&unit->pos, range2, &energy, &energyAmount, &metal, &metalAmount)) {
                    if (metal && unit->player->metal < unit->player->metalCapacity * 0.2) {
                        order->SetAttachedFx(0);
                        AppendOrder(unit, new Order("RECLAIM", 0, metal, 0, 0, 0));
                        order->SetAttachedFx(0);
                        order->flags = 0;
                        return 3;
                    }
                    if (energy && unit->player->energy < unit->player->energyCapacity * 0.2) {
                        order->SetAttachedFx(0);
                        AppendOrder(unit, new Order("RECLAIM", 0, energy, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                    if (metal && unit->player->metal + metalAmount <= unit->player->metalCapacity) {
                        order->SetAttachedFx(0);
                        AppendOrder(unit, new Order("RECLAIM", 0, metal, 0, 0, 0));
                        order->flags = 0;
                        return 3;
                    }
                    if (energy && unit->player->energy + energyAmount <= unit->player->energyCapacity) {
                        order->SetAttachedFx(0);
                        AppendOrder(unit, new Order("RECLAIM", 0, energy, 0, 0, 0));
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

