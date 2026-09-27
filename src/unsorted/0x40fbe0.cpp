// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: 62.2%. Register allocation and merged order-construction tails differ.
#include <stdio.h>
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
struct WeaponDef { char pad0[0xdc]; int range; char pade0[0x111-0xe0]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[0x21c-0x1fe]; short altitude; char pad21e[0x231-0x21e]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
class Class_0043d210 { public: char pad0[0x2e]; unsigned char flags; void FUN_0043d210(Unit*, int); };
struct Unit {
    Class_0043d210* motion; char pad4[4]; Weapon weapons[3]; Order* order;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; short depth; int terrain; int busy;
    char pad8a[8]; UnitDef* def; Owner* owner; char pad9a[12]; unsigned short category;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
};
struct Order { char pad0[4]; Class_00438760 kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int angle, parity; char pad3e[4]; unsigned int capabilities; char pad46[4]; int next; };
class Class_0043a0c0 { public: char data[0x56]; Class_0043a0c0(Class_00438760, Unit*, Vec3*, int, int, int); };
class Class_0044e2d0 { public: char data[0x36]; Class_0044e2d0(Order*, const Vec3&); };
struct Game { char pad0[0x1422b]; int width, height; char pad14233[0x142b7-0x14233]; int water; };
#pragma pack(pop)
extern Game* g_game;
class Class_0044e730 { public: void FUN_0044e730(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_004899b0 { public: int FUN_004899b0(Unit*); };
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_0048b090 { public: void FUN_0048b090(int, int); };
void __stdcall FUN_0048aac0(Unit*, Unit*, char, char);
short __stdcall FUN_0048a980(Vec3*, Vec3*);
union Fixed { int value; struct { unsigned short frac; short whole; } parts; };
Vec3 __stdcall FUN_004103a0(short, Fixed);
static inline Vec3 Direction(short angle, int range) { Fixed distance; distance.value=range; return FUN_004103a0(angle,distance); }
Vec3 __stdcall FUN_0040f790(const Vec3&, const Vec3&);
void __stdcall FUN_0043ad10(Unit*, Class_0043a0c0*);

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
// FUNCTION: 0x40fbe0
int __stdcall FUN_0040fbe0(Unit* unit, Order* order, int flags)
{
    if (order->target && !(flags&0x48)) {
        if (unit->terrain==g_game->water) {
            Vec3 center;
            center.x=(g_game->width/2)<<16;
            center.z=(g_game->height/2)<<16;
            short angle=FUN_0048a980(&unit->pos,&center);
            Vec3 pos=FUN_0040f790(unit->pos,Direction(angle,0x3200000));
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->FUN_0044e730(128);
            order->flags|=0xe0;
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
            return 2;
        }
        order->pos=order->target->pos;
        unsigned int state=0; state=order->state;
        switch(state) {
        case 0:
            if (unit->motion && (unit->def->flags&0x800)) {
                ((Class_00438880*)order)->FUN_00438880("Guarding");
                ((Class_004898b0*)unit)->FUN_004898b0(3);
                if (unit->busy) FUN_0048aac0(unit,0,-1,2);
                ((Class_0048b090*)unit)->FUN_0048b090(1,1);
                if ((unit->motion->flags&3)==1) {
                    unit->motion->FUN_0043d210(unit,2);
                    Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                    ((Class_0044e6c0*)move)->FUN_0044e6c0(unit->def->altitude/2);
                    ((Class_004388d0*)order)->FUN_004388d0((int)move);
                    order->flags|=0xe0;
                }
                order->angle=FUN_004b6c30(0x10000);
                order->parity=order->angle&1;
                return 1;
            }
            return 7;
        case 1:
            ((Class_00489800*)unit)->FUN_00489800(3);
            return 1;
        case 2: {
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

            if (((Class_004899b0*)unit)->FUN_004899b0(order->target)) {
                Class_00438760 kind=FUN_0043f0e0(8,unit,order->target,0);
                if (kind.index) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit,new Class_0043a0c0(kind,order->target,0,0,0,0));
                    order->flags=0; return 3;
                }
            }
            if (order->target->order && order->target->order->kind.index && (unit->def->flags&0x40) &&
                ((Class_004899b0*)unit)->FUN_004899b0(order->target->order->target) &&
                (order->target->def->flags&0x40) && order->target->order &&
                (order->target->order->capabilities&0x100000) && unit!=order->target->order->target) {
                int building=order->target->order->kind=="MobileBuild" || order->target->order->kind=="BuildingBuild" || order->target->order->kind=="VTOL_MobileBuild";
                Order* other=order->target->order;
                int actionable=((other->capabilities&0x200) && other->target) || (other->capabilities&0x400);
                if (!building && actionable) {
                    Class_00438760 kind=other->kind;
                    if (kind=="REPAIRUNIT") kind=Class_00438760("VTOL_REPAIRUNIT");
                    if (kind=="RECLAIM") kind=Class_00438760("VTOL_RECLAIM");
                    if (kind=="RECLAIMUNIT") kind=Class_00438760("VTOL_RECLAIMUNIT");
                    if (kind=="HELPBUILD") kind=Class_00438760("VTOL_HELPBUILD");
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit,new Class_0043a0c0(kind,order->target->order->target,&order->target->order->pos,0,0,0));
                    order->flags=0; return 3;
                }
                if (building && other->target) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    Class_00438760 kind;
                    kind=Class_00438760("VTOL_HelpBuild");
                    FUN_0043acb0(unit,new Class_0043a0c0(kind,order->target->order->target,&order->target->order->pos,0,0,0));
                    order->flags=0; return 3;
                }
            }
            if (flags&0xe0) order->angle+=-FUN_004b6c30(0x2000)-0x4000;
            short angle=order->angle;
            Vec3 pos;
            if ((unsigned char)(unit->flags>>31)&1)
                pos=order->target->pos+Offset(angle,(unit->weapons[0].def->range+160)<<16);
            else pos=order->target->pos+Offset(angle,0x1400000);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->FUN_0044e730(128);
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
            ((Class_00439e80*)order)->FUN_00439e80(30);
            order->flags|=0xf8;
            return 2;
        }
        }
        return 7;
    }
    if (!order->next) FUN_0043ad10(unit,new Class_0043a0c0("VTOL_SEEKGUARD",order->target,&order->pos,0,0,0));
    return 5;
}
