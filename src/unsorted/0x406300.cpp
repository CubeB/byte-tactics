// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: 73.4%. Allocation tail merging, zero-register reuse, and operand order differ.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& v) const { Vec3 r; r.x=x+v.x; r.y=y+v.y; r.z=z+v.z; return r; }
};
struct Unit;
struct Order;
class Class_00438760 { public: unsigned char index; Class_00438760() {} Class_00438760(const char*); int operator==(const Class_00438760& v) const { return index==v.index; } };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438930 { public: void FUN_00438930(Vec3*, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_00489800 { public: void FUN_00489800(int); };
#pragma pack(push, 1)
struct WeaponDef { char pad0[0x111]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[0x231-0x1fe]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
struct Unit {
    char pad0[8]; Weapon weapons[3]; Order* order;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; char pad80[6]; int busy;
    char pad8a[8]; UnitDef* def; Owner* owner; char pad9a[12]; unsigned short category;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
};
struct Order { char pad0[4]; Class_00438760 kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int radius; char pad3a[8]; unsigned int capabilities; };
class Class_0043a0c0 { public: char data[0x56]; Class_0043a0c0(Class_00438760, Unit*, Vec3*, int, int, int); };
#pragma pack(pop)
int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
Unit* __stdcall FUN_0048a190(Unit*, int);
int __stdcall FUN_0049abb0(Unit*, Unit*, unsigned char);
void __stdcall FUN_0048a060(Unit*, Unit*, int);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
void __stdcall FUN_0043acb0(Unit*, Class_0043a0c0*);
int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
// FUNCTION: 0x406300
int __stdcall FUN_00406300(Unit* unit, Order* order, int flags)
{
    if (!order->target) return 5;
    if (unit->busy) return 7;
    if ((unsigned char)(order->target->def->flags >> 11) & 1) return 8;
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0: {
        ((Class_00438880*)order)->FUN_00438880("Guarding");
        ((Class_00489800*)unit)->FUN_00489800(3);
        order->radius=(unit->width + order->target->width + 2) << 4;
        int distance=order->radius << 16;
        short angle=FUN_004b6c30(0x10000);
        order->pos=Offset(angle,distance);
        return 1;
    }
    case 1: {
        Unit* attacker=order->target->attacker;
        if (attacker && !attacker->owner->allied[unit->owner->index] && (flags & 0x10) &&
            !Contains(unit->def->categories,attacker->category)) {
            if (FUN_0043b1f0(unit,attacker,1)) { order->flags=0; return 3; }
            if (unit->flags & 0x300000) {
                for (unsigned char i=0;i<3;++i) {
                    Weapon* weapon=&unit->weapons[i];
                    if ((weapon->flags&2) && (weapon->flags&0x10) && !((unsigned char)(weapon->def->flags >> 26)&1)) {
                        Unit* target=FUN_0048a190(unit,i);
                        if (!target || !FUN_0049abb0(unit,target,i) || Contains(unit->def->weaponCategories[i],target->category))
                            FUN_0048a060(unit,attacker,i);
                    }
                }
            }
        }
        if ((unsigned int)order->target->health < order->target->def->maxHealth && (unit->def->flags&0x40)) {
            Class_00438760 kind=FUN_0043f0e0(8,unit,order->target,0);
            if(kind.index) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit,new Class_0043a0c0(kind,order->target,0,0,0,0));
                order->flags=0; return 3;
            }
        }
        if (order->target->order && order->target->order->kind.index &&
            (unit->def->flags&0x40) && (order->target->def->flags&0x40) &&
            (order->target->order->capabilities&0x100000) && order->target->order->target!=unit) {
            int building=order->target->order->kind=="MobileBuild" ||
                         order->target->order->kind=="BuildingBuild";
            Order* other=order->target->order;
            int actionable=((other->capabilities&0x200) && other->target) || (other->capabilities&0x400);
            Class_00438760 kind;
            if (!building && actionable) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                kind=order->target->order->kind;
            } else if (building && other->target) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                kind=Class_00438760("HelpBuild");
            } else goto follow;
            FUN_0043acb0(unit,new Class_0043a0c0(kind,order->target->order->target,&order->target->order->pos,0,0,0));
            order->flags=0; return 3;
        }
follow:
        Vec3 pos=order->target->pos+order->pos;
        ((Class_00438930*)order)->FUN_00438930(&pos,order->radius/2);
        ((Class_00439e80*)order)->FUN_00439e80(30);
        order->flags|=0x18;
        return 2;
    }
    }
    return 7;
}
