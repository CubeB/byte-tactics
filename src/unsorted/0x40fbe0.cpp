// Decompiled by GPT-5.6 Astra, finished by deepseek-v4.1-flash. Names are provisional.
// Region-split skeleton, per docs/splitting-huge-functions.md (PR #1287, merged).
// Checked by space-bunny-free. Names are provisional.
//
// STATUS: partial. check.py reads 75.0% (ours 1918 bytes vs original 1976).
// Prologue `sub esp,0x54` + ebx/ebp/esi/edi is identical.
//
// Progress by deepseek-v4.1-flash: the MobileBuild/VTOL order-steal tail is now
// ONE construction site (a shared `Class_00438760 kind` plus a single
// `FUN_0043acb0(unit,new Class_0043a1f0(kind,other->target,&other->pos,...))`
// after the if/else). That merged the previously duplicated tail and took the
// score from 73.2% (2047 bytes) to 75.0% (1918 bytes), so the shrink confirms
// the original really has one tail at 0x4100d6.
//
// Remaining diff hunks (first diff first):
//   0x40fce9  `order->pos = order->target->pos`: original emits
//             `add eax,0x6a` (consumes target in eax) and copies via
//             ecx/edi; ours emits `lea ecx,[eax+0x6a]` and copies via edi/ebp.
//             Pure register-allocation, not fixable from source so far.
//   0x40fdd8-0x40fe62  three-weapon scan: original keeps a byte loop counter
//             at [esp+0x1c] AND a separate index in ebp (FUN_0049abb0 gets the
//             byte, FUN_0048a190/FUN_0048a060 get ebp) plus the weapon pointer
//             at [esp+0x20]; ours has one counter and extra reloads. Likely the
//             source walked `Weapon* weapon` alongside an index.
//   0x40fedc-0x40ffb3  zero/branch registers differ (original keeps 0 in ebp,
//             building/actionable in edi/edx; ours uses ebp for building,
//             edi for `other`).
//   0x40ffb3-0x4100d6  building/actionable: our shared `buildOrder` flag adds a
//             redundant `mov eax,1; test eax,eax`; original re-tests
//             building/actionable directly at 0x4100a9/0x4100b1. Also our
//             kind temp lands at [esp+0x1c], original at [esp+0x70] (the flags
//             argument home slot).
//   0x41013b+  aim block and the 0x4102c6 default: eax/edx and ah/ch roles
//             swapped.
//
#include <stdio.h>
// SHARED begin
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
class Class_0043a1f0 { public: char data[0x56]; Class_0043a1f0(Class_00438760, Unit*, Vec3*, int, int, int); };
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
void __stdcall FUN_0043ad10(Unit*, Class_0043a1f0*);

int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
Unit* __stdcall FUN_0048a190(Unit*, int);
int __stdcall FUN_0049abb0(Unit*, Unit*, unsigned char);
void __stdcall FUN_0048a060(Unit*, Unit*, int);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
// SHARED end

// FUNCTION: 0x40fbe0
int __stdcall FUN_0040fbe0(Unit* unit, Order* order, int flags)
{
// REGION r1 begin   0x40fbe0-0x40fcda
//   the guard, the water/terrain move, and the state dispatch
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
// REGION r2 begin   0x40fcda-0x40feda
//   case 0, the guard order, and case 1
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
// REGION r2 end
// REGION r3 begin   0x40feda-0x4100d0
//   case 2's opening: the attacker test, the three-weapon scan, and the
//   FUN_004899b0 order-kind test.
//
//   This text emits 0x40fd26-0x40fedc, which is the original's r2 range, not
//   r3's: MSVC lays a switch out as case 2, case 1, case 0, so the region
//   ranges (which follow the emitted order) and these markers (which follow
//   the written order) disagree. The code that does emit r3's range, the
//   MobileBuild order-steal chain at 0x40feda-0x4100d0, sits in the r4 block.
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
                    FUN_0043acb0(unit,new Class_0043a1f0(kind,order->target,0,0,0,0));
                    order->flags=0; return 3;
                }
            }
// REGION r3 end
// REGION r4 begin   0x4100d0-0x4102c6
//   the MobileBuild and VTOL order-steal chain, then the aim and fire block
//   The original's ONE tail at 0x4100d6 is now reproduced by a single
//   construction after the if/else (see STATUS). What still differs is the
//   `buildOrder` flag the compiler materialises (extra test eax,eax) and the
//   frame slot of `kind`.
            if (order->target->order && order->target->order->kind.index && (unit->def->flags&0x40) &&
                ((Class_004899b0*)unit)->FUN_004899b0(order->target->order->target) &&
                (order->target->def->flags&0x40) && order->target->order &&
                (order->target->order->capabilities&0x100000) && unit!=order->target->order->target) {
                Order* other=order->target->order;
                int building=other->kind=="MobileBuild" || other->kind=="BuildingBuild" || other->kind=="VTOL_MobileBuild";
                int actionable=((other->capabilities&0x200) && other->target) || (other->capabilities&0x400);
                Class_00438760 kind;
                int buildOrder=0;
                if (!building && actionable) {
                    kind=other->kind;
                    if (kind=="REPAIRUNIT") kind=Class_00438760("VTOL_REPAIRUNIT");
                    if (kind=="RECLAIM") kind=Class_00438760("VTOL_RECLAIM");
                    if (kind=="RECLAIMUNIT") kind=Class_00438760("VTOL_RECLAIMUNIT");
                    if (kind=="HELPBUILD") kind=Class_00438760("VTOL_HELPBUILD");
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    buildOrder=1;
                } else if (building && other->target) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    kind=Class_00438760("VTOL_HelpBuild");
                    buildOrder=1;
                }
                if (buildOrder) {
                    FUN_0043acb0(unit,new Class_0043a1f0(kind,other->target,&other->pos,0,0,0));
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
// REGION r4 end
        }
// REGION r1 end
// REGION r5 begin   0x4102c6-0x410398
//   the default return and the seek-guard tail
        return 7;
    }
    if (!order->next) FUN_0043ad10(unit,new Class_0043a1f0("VTOL_SEEKGUARD",order->target,&order->pos,0,0,0));
    return 5;
// REGION r5 end
}
