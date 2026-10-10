// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Stays in its own file: in unit_orders.cpp the weapon slot loop keeps its counter in a register, where the original keeps it in memory.

#include <stdio.h>
#include "../util/vec3.h"
static inline Vec3 operator+(const Vec3& a, const Vec3& b) { Vec3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
class UnitMotion;
struct Player;
struct Unit;
struct Order;
class MissionType { public: unsigned char index; MissionType() {} MissionType(const char*); int operator==(const MissionType& v) const { return index==v.index; } };

#pragma pack(push, 1)
struct WeaponDef { char pad0[0x111]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
#include "../units/unit_def.h"
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
struct Unit {
    char pad0[8]; Weapon weapons[3]; Order* list;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; char pad80[6]; int carrier;
    char pad8a[8]; UnitDef* def; Owner* player; char pad9a[12]; unsigned short unitDefIndex;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
    void ReleaseWeapons(int);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void SetStateBits(int, int);
};
struct Order { Unit* Target() { return target; } Vec3* Position() { return &pos; } char pad0[4]; MissionType kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int radius; char pad3a[8]; unsigned int capabilities; void AnnounceStatusIfFlagged(const char*); void SetAttachedFx(int); void AttachApproachRadiusGoal(Vec3*, int); void SetDeadlineTicks(int); Order(MissionType, Unit*, Vec3*, int, int, int); char unknown_46[0x10];           // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
};

#pragma pack(pop)
int __stdcall IssueAttackOrder(Unit*, Unit*, int);
Unit* __stdcall GetWeaponTargetUnit(Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
MissionType __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Order*);
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall GetOrderTarget(int param_1);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int CheckDirectXVersion(int, int, int, int, int);
struct Feature;
// FUNCTION: 0x406300
int __stdcall FollowGroundOrder(Unit* unit, Order* order, int flags)
{
    if (!order->target) return 5;
    if (unit->carrier) return 7;
    if ((unsigned char)(order->target->def->flags1 >> 11) & 1) return 8;
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0: {
        order->AnnounceStatusIfFlagged("Guarding");
        unit->ReleaseWeapons(3);
        order->radius=(unit->width + order->target->width + 2) << 4;
        int distance=order->radius << 16;
        short angle=RandomInt(0x10000);
        order->pos=Offset(angle,distance);
        return 1;
    }
    case 1: {
        Unit* attacker=order->target->attacker;
        if (attacker && !attacker->player->allied[unit->player->index] && (flags & 0x10) &&
            !Contains(unit->def->categories,attacker->unitDefIndex)) {
            if (IssueAttackOrder(unit,attacker,1)) { order->flags=0; return 3; }
            if (unit->flags & 0x300000) {
                for (unsigned char i=0;i<3;++i) {
                    Weapon* weapon=&unit->weapons[i];
                    if ((weapon->flags&2) && (weapon->flags&0x10) && !((unsigned char)(weapon->def->flags >> 26)&1)) {
                        Unit* target=GetWeaponTargetUnit(unit,i);
                        if (!target || !WeaponCanReachUnit(unit,target,i) || Contains(unit->def->weaponCategories[i],target->unitDefIndex))
                            SetWeaponTargetUnit(unit,attacker,i);
                    }
                }
            }
        }
        if ((unsigned int)order->target->health < order->target->def->maxHealth && (unit->def->flags1&0x40)) {
            MissionType kind=GetOrderType(8,unit,order->target,0);
            if(kind.index) {
                order->SetAttachedFx(0);
                AppendOrder(unit,new Order(kind,order->target,0,0,0,0));
                order->flags=0; return 3;
            }
        }
        if (order->target->list && order->target->list->kind.index &&
            (unit->def->flags1&0x40) && (order->target->def->flags1&0x40) &&
            (order->target->list->capabilities&0x100000) && unit!=order->target->list->Target()) {
            int building=order->target->list->kind=="MobileBuild" ||
                         order->target->list->kind=="BuildingBuild";
            Order* other=order->target->list;
            int actionable=((other->capabilities&0x200) && other->target) || (other->capabilities&0x400);
            MissionType kind;
            if (!building && actionable) {
                order->SetAttachedFx(0);
                kind=order->target->list->kind;
                // Target() and Position() on the base: keeps the two order->order loads from merging.
                AppendOrder(unit,new Order(kind,order->Target()->list->target,order->Target()->list->Position(),0,0,0));
                order->flags=0; return 3;
            }
            if (building && other->target) {
                order->SetAttachedFx(0);
                kind=MissionType("HelpBuild");
                AppendOrder(unit,new Order(kind,order->Target()->list->target,order->Target()->list->Position(),0,0,0));
                order->flags=0; return 3;
            }
        }
follow:
        Vec3 pos=order->target->pos+order->pos;
        order->AttachApproachRadiusGoal(&pos,order->radius/2);
        order->SetDeadlineTicks(30);
        order->flags|=0x18;
        return 2;
    }
    }
    return 7;
}
