// Decompiled by GPT-5.6 Astra, finished by deepseek-v4.1-flash and GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by xiaomi/mimo-v2.6-pro, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// VTOL guard order handler ("Guarding"): the aircraft circles the guarded
// unit, fights back against its attacker, repairs it, and takes over the
// guarded unit's own build or reclaim order (as the VTOL_ variant of that
// order). The ground twin is 0x406300.
#include <stdio.h>
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& v) const { Vec3 r; r.x=x+v.x; r.y=y+v.y; r.z=z+v.z; return r; }
};
struct Unit;
struct Order;
class Class_00438760 { public: unsigned char index; Class_00438760() {} Class_00438760(const char*); int operator==(const Class_00438760& v) const { return index==v.index; } };
class Class_00438880 { public: void AnnounceStatusIfFlagged(const char*); };
class Class_004388d0 { public: void SetAttachedFx(int); };
class Class_00438930 { public: void AttachApproachRadiusGoal(Vec3*, int); };
class Class_00439e80 { public: void SetDeadlineTicks(int); };
#pragma pack(push, 1)
struct WeaponDef { char pad0[0xdc]; int range; char pade0[0x111-0xe0]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
#include "../units/unit_def.h"
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
class UnitMotion { public: char pad0[0x2e]; unsigned char flags; void SetFlightMode(Unit*, int); };
struct Unit {
    UnitMotion* motion; char pad4[4]; Weapon weapons[3]; Order* order;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; short depth; int terrain; int busy;
    char pad8a[8]; UnitDef* def; Owner* owner; char pad9a[12]; unsigned short category;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
    void ReleaseWeapons(int);
    int CanRepair(Unit*);
    void ClaimWeapons(int);
    void SetStateBits(int, int);
};
struct Order { char pad0[4]; Class_00438760 kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int angle, parity; char pad3e[4]; unsigned int capabilities; char pad46[4]; int next; };
class Class_0043a1f0 { public: char data[0x56]; Class_0043a1f0(Class_00438760, Unit*, Vec3*, int, int, int); };
class Class_0044e2d0 { public: char data[0x36]; Class_0044e2d0(Order*, const Vec3&); };
struct Game { char pad0[0x1422b]; int width, height; char pad14233[0x142b7-0x14233]; int overflowBucket; };
#pragma pack(pop)
extern Game* g_game;
class Class_0044e730 { public: void SetApproachRadius(int); };
class Class_0044e6c0 { public: void SetAltitude(int); };
void __stdcall AttachUnitToPiece(Unit*, Unit*, char, char);
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
union Fixed { int value; struct { unsigned short frac; short whole; } parts; };
Vec3 __stdcall DirectionFromAngle(short, Fixed);
static inline Vec3 Direction(short angle, int range) { Fixed distance; distance.value=range; return DirectionFromAngle(angle,distance); }
Vec3 __stdcall AddVec3(const Vec3&, const Vec3&);
void __stdcall AppendOrderToTail(Unit*, Class_0043a1f0*);

int __stdcall IssueAttackOrder(Unit*, Unit*, int);
Unit* __stdcall GetWeaponTargetUnit(Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
Class_00438760 __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,(int)distance); return v; }

// Used on the target argument of the order-steal calls: the plain chain changes codegen.
static inline Unit* OrderTarget(Order*order) { return order->target; }

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x40fbe0
int __stdcall VtolFollowOrder(Unit* unit, Order* order, int flags)
{
    if (order->target && !(flags&0x48)) {
        if (unit->terrain==g_game->overflowBucket) {
            Vec3 center;
            center.x=(g_game->width/2)<<16;
            center.z=(g_game->height/2)<<16;
            short angle=GetHeadingBetween(&unit->pos,&center);
            Vec3 pos=AddVec3(unit->pos,Direction(angle,0x3200000));
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->SetApproachRadius(128);
            order->flags|=0xe0;
            ((Class_004388d0*)order)->SetAttachedFx((int)move);
            return 2;
        }
        // Copied through a pointer: assigning order->pos directly changes codegen.
        Vec3* dst=&order->pos;
        *dst=order->target->pos;
        switch(order->state) {
        case 0:
            if (unit->motion && (unit->def->flags1&0x800)) {
                ((Class_00438880*)order)->AnnounceStatusIfFlagged("Guarding");
                unit->ClaimWeapons(3);
                if (unit->busy) AttachUnitToPiece(unit,0,-1,2);
                unit->SetStateBits(1,1);
                if ((unit->motion->flags&3)==1) {
                    unit->motion->SetFlightMode(unit,2);
                    Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                    ((Class_0044e6c0*)move)->SetAltitude(unit->def->altitude/2);
                    ((Class_004388d0*)order)->SetAttachedFx((int)move);
                    order->flags|=0xe0;
                }
                order->angle=RandomInt(0x10000);
                order->parity=order->angle&1;
                return 1;
            }
            return 7;
        case 1:
            unit->ReleaseWeapons(3);
            return 1;
        case 2: {
            Unit* attacker=order->target->attacker;
            if (attacker && !attacker->owner->allied[unit->owner->index]) {
                if (flags & 0x10) {
                    if (!Contains(unit->def->categories,attacker->category)) {
                        if (IssueAttackOrder(unit,attacker,1)) { order->flags=0; return 3; }
                        if (unit->flags & 0x300000) {
                            unsigned char i=0;
                            Weapon* weapon=unit->weapons;
                            do {
                                // the test is in this sense on purpose: it is what
                                // gives the loop the original's jump polarity, its
                                // register for the pointer and its stack slots
                                if (!((weapon->flags&2) && weapon->flags&0x10 && ((unsigned char)(weapon->def->flags >> 26)&1) == 0)) {
                                } else {
                                    Unit* target=GetWeaponTargetUnit(unit,i);
                                    if (!target || !WeaponCanReachUnit(unit,target,i) || Contains(unit->def->weaponCategories[i],target->category))
                                        SetWeaponTargetUnit(unit,attacker,i);
                                }
                                ++i;
                                weapon++;
                            } while (i<3);
                        }
                    }
                }
            }
            if (unit->CanRepair(order->target)) {
                Class_00438760 kind=GetOrderType(8,unit,order->target,0);
                if (kind.index) {
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    AppendOrder(unit,new Class_0043a1f0(kind,order->target,0,0,0,0));
                    order->flags=0; return 3;
                }
            }
            // `order->target->order` is spelled out at every use, with no
            // local for it: that is what makes MSVC reload it after each
            // Class_00438760 constructor call, and what frees the callee-saved
            // register the block's zero constant ends up in.
            if (order->target->order && order->target->order->kind.index && (unit->def->flags1&0x40) &&
                unit->CanRepair(order->target->order->target) &&
                (order->target->def->flags1&0x40) && order->target->order &&
                (order->target->order->capabilities&0x100000) && unit!=order->target->order->target) {
                int building = order->target->order->kind=="MobileBuild" || order->target->order->kind=="BuildingBuild" || order->target->order->kind=="VTOL_MobileBuild";
                int actionable = ((order->target->order->capabilities&0x200) && order->target->order->target) || (order->target->order->capabilities&0x400);
                Class_00438760 kind;
                if (!building && actionable) {
                    kind=order->target->order->kind;
                    if (kind=="REPAIRUNIT") kind=Class_00438760("VTOL_REPAIRUNIT");
                    if (kind=="RECLAIM") kind=Class_00438760("VTOL_RECLAIM");
                    if (kind=="RECLAIMUNIT") kind=Class_00438760("VTOL_RECLAIMUNIT");
                    if (kind=="HELPBUILD") kind=Class_00438760("VTOL_HELPBUILD");
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    AppendOrder(unit,new Class_0043a1f0(kind,OrderTarget(order)->order->target,&order->target->order->pos,0,0,0));
                    order->flags=0; return 3;
                }
                if (building && order->target->order->target) {
                    ((Class_004388d0*)order)->SetAttachedFx(0);
                    kind=Class_00438760("VTOL_HelpBuild");
                    AppendOrder(unit,new Class_0043a1f0(kind,OrderTarget(order)->order->target,&order->target->order->pos,0,0,0));
                    order->flags=0; return 3;
                }
            }
            if (flags&0xe0) order->angle += -RandomInt(0x2000)-0x4000;
            short angle=order->angle;
            Vec3 pos;
            if ((unsigned char)(unit->flags>>31)&1)
                pos=order->target->pos+Offset(angle,(unit->weapons[0].def->range+160)<<16);
            else pos=order->target->pos+Offset(angle,0x1400000);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->SetApproachRadius(128);
            ((Class_004388d0*)order)->SetAttachedFx((int)move);
            ((Class_00439e80*)order)->SetDeadlineTicks(30);
            order->flags|=0xf8;
            return 2;
        }
        }
        return 7;
    }
    if (!order->next) AppendOrderToTail(unit,new Class_0043a1f0("VTOL_SEEKGUARD",order->target,&order->pos,0,0,0));
    return 5;
}
