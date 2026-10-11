// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by Claude Opus 5.5, finished by Claude Opus 5.5. re-verified by GPT-6, retried by Claude Opus 5.5, finished by GPT-6, retried by Claude Opus 5.5, finished by GPT-6, matched by Claude Opus 5.5. Names are provisional.
// The include set (ta_types.h in namespace ta, then the system headers) fixes
// unit's front-end id, which decides the register order of the health test.
#include <windows.h>
#include <ddraw.h>
#include <dsound.h>
#include <dplay.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <shlobj.h>
#include <imagehlp.h>
#include <math.h>
#include <tchar.h>
#include <time.h>
#include <float.h>
namespace ta {
#include <ta_types.h>
}
#include "../util/vec3.h"
struct Unit;
struct Order;
#include "mission_type.h"

#pragma pack(push, 1)
struct WeaponDef { char pad0[0xdc]; int range; char pade0[0x111-0xe0]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
#include "../units/unit_def.h"
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
class UnitMotion { public: char pad0[0x2e]; unsigned char flags; void SetFlightMode(Unit*, int); };
struct Unit {
    UnitMotion* motion; char pad4[4]; Weapon weapons[3]; Order* list;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; short depth; int spatialBucket; int carrier;
    char pad8a[8]; UnitDef* def; Owner* player; char pad9a[12]; unsigned short unitDefIndex;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
    void ReleaseWeapons(int);
};
struct Order { char pad0[4]; MissionType kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int angle, parity; char pad3e[4]; unsigned int capabilities; char pad46[4]; int next; void AnnounceStatusIfFlagged(const char*); void SetAttachedFx(int); void AttachApproachRadiusGoal(Vec3*, int); void SetDeadlineTicks(int); Order(MissionType, Unit*, Vec3*, int, int, int); char unknown_4e[0x8];           // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    ~Order();
    Order(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
};

class Class_0044e2d0 {
public:
    char data[0x36];
    Class_0044e2d0(Order*, const Vec3&);
    void SetApproachRadius(int);
};
struct Game { char pad0[0x1422b]; int width, height; char pad14233[0x142b7-0x14233]; int overflowBucket; };
#pragma pack(pop)
extern Game* g_game;
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
MissionType __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Order*);
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
Vec3 __stdcall AddVec3(const Vec3& a, const Vec3& b);
union Fixed { int value; struct { unsigned short frac; short whole; } parts; };
Vec3 __stdcall DirectionFromAngle(short angle, Fixed scale);
// A free function, not a member: its smaller IL size keeps it inlined.
static inline Vec3 operator+(const Vec3& a, const Vec3& b) { Vec3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static inline Vec3 Direction(short angle, int range) { Fixed distance; distance.value=range; return DirectionFromAngle(angle,distance); }
static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x=-FUN_004b70ef(angle,distance);
    v.y=0;
    // Names its result: keeps unit and order in esi and edi.
    int z=FUN_004b7123(angle,distance);
    v.z=-z;
    return v;
}

// Keeps a declared constructor (never inlined); UnitList below is a separate class.
class LandingPadList : public std::vector<Unit*> { public: LandingPadList(); };
void __stdcall GetFactoriesInRadius(int, Vec3*, int, std::vector<Unit*>*);
class UnitList : public std::vector<Unit*> {};

class GroundAllyVisitor {
public:
    virtual void CollectGroundAlly(Unit*);
    // Three-argument constructor: puts the vtable store after the member stores.
    GroundAllyVisitor(Owner* o, std::vector<Unit*>* u, Unit* s) : owner(o), units(u), self(s) {}
    Owner* owner; std::vector<Unit*>* units; Unit* self;
};
void __stdcall VisitObjectsInRange(Vec3*, int, const GroundAllyVisitor&);
// IsDamaged, FindPads and SearchRange stay small helpers: the vector calls
// inline or not according to the original.
static inline int IsDamaged(Unit* u) { return (unsigned int)u->health < (u->def->maxHealth>>2)*3; }
static inline void FindPads(Unit* u, std::vector<Unit*>* pads) { GetFactoriesInRadius(u->player->index,&u->pos,0xf00,pads); }
static inline int SearchRange(Unit* u) { return u->def->range<<16; }
static inline int Patrol(Unit* unit, Order* order, int flags)
{
    if (IsDamaged(unit)) {
        LandingPadList pads;
        FindPads(unit,&pads);
        if (!pads.empty()) {
            order->SetAttachedFx(0);
            Unit* pad=pads[RandomInt(pads.size())];
            AppendOrder(unit,new Order("VTOL_LANDING",pad,0,0,0,0));
            order->flags=0;
            return 0;
        }
    }
    UnitList units;
    int range=SearchRange(unit);
    GroundAllyVisitor visitor(unit->player,&units,unit);
    VisitObjectsInRange(&unit->pos,range,visitor);
    if (!units.empty()) {
        order->SetAttachedFx(0);
        MissionType kind=GetOrderType(7,unit,units[0],0);
        AppendOrder(unit,new Order(kind,units[0],0,0,0,0));
        order->flags=0;
        return 3;
    }
    if (flags&0xe0) order->angle+=-RandomInt(0x2000)-0x4000;
    Vec3 pos=order->pos+Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16);
    Class_0044e2d0* move=new Class_0044e2d0(order,pos);
    move->SetApproachRadius(128);
    order->SetAttachedFx((int)move);
    order->SetDeadlineTicks(30);
    order->flags|=0xf8;
    return 2;
}
// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x410850
int __stdcall VtolSeekGuardOrder(Unit* unit, Order* order, int flags)
{
    if (flags&0x40) return 5;
    if (unit->spatialBucket==g_game->overflowBucket) {
        Vec3 center;
        center.x=(g_game->width/2)<<16;
        center.z=(g_game->height/2)<<16;
        short angle=GetHeadingBetween(&unit->pos,&center);
        Vec3 pos=AddVec3(unit->pos,Direction(angle,0x3200000));
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        move->SetApproachRadius(128);
        order->flags|=0xe0;
        order->SetAttachedFx((int)move);
        return 2;
    }
    switch(order->state) {
    case 0:
        if (unit->motion && (unit->def->flags1&0x800)) {
            Vec3* pos=&order->pos;
            if (!pos->x && !pos->z && !pos->y) *pos=unit->pos;
            order->angle=RandomInt(0x10000);
            order->parity=order->angle&1;
            return 1;
        }
        break;
    case 1:
        int result=Patrol(unit,order,flags);
        return result;
    }
    return 7;
}
