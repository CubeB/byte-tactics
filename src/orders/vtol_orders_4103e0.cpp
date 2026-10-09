// Decompiled by GPT-6 Astra. Names are provisional.
#include <vector>
struct Unit;
namespace std {
template<> class vector<Unit*,allocator<Unit*> > {
    unsigned char alloc; Unit** first; Unit** last; Unit** endStorage;
public:
    ~vector();
    unsigned int size() const;
    unsigned int count() const { return begin()==0 ? 0 : end()-begin(); }
    bool empty() const { return size()==0; }
    Unit** begin() const { return first; }
    Unit** end() const { return last; }
    Unit*& operator[](unsigned int n) { return *(begin()+n); }
};
}
#include "../util/vec3.h"
struct Order;
class Class_00438760 { public: unsigned char index; Class_00438760() {} Class_00438760(const char*); int operator==(const Class_00438760& v) const { return index==v.index; } };

#pragma pack(push, 1)
struct WeaponDef { char pad0[0xdc]; int range; char pade0[0x111-0xe0]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
#include "../units/unit_def.h"
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
#include "unit_motion.h"
// Unused here: real forward declarations whose symbol ids keep 0x4103e0 matching (docs/c2-regalloc.md).
class BitWriter;
class OpenHeap;
class ScoutTimer;
class EscortTimer;
class AssaultTimer;
class SquadManager;
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
struct Order { char pad0[4]; Class_00438760 kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int angle, parity; char pad3e[4]; unsigned int capabilities; char pad46[4]; int next; void AnnounceStatusIfFlagged(const char*); void SetAttachedFx(int); void AttachApproachRadiusGoal(Vec3*, int); void SetDeadlineTicks(int); Order(Class_00438760, Unit*, Vec3*, int, int, int); char unknown_4e[0x8];     // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    void OrStatusFlags(unsigned int flags);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
};

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
void __stdcall AppendOrderToTail(Unit*, Order*);

int __stdcall IssueAttackOrder(Unit*, Unit*, int);
Unit* __stdcall GetWeaponTargetUnit(Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
Class_00438760 __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Order*);
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }

class LandingPadList : public std::vector<Unit*> { public: LandingPadList(); };
void __stdcall GetFactoriesInRadius(int, Vec3*, int, std::vector<Unit*>*);
Unit* __stdcall FindBestTargetIfFireAtWill(Unit*);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
void __stdcall AddOrder(int kind, int remove, Unit* owner, void* id, Vec3* pos, int param_6, int param_7);
int __stdcall GetOrderName(Unit* obj);
void __stdcall DeleteOrders(Unit* owner, int all);
// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x4103e0
int __stdcall VtolSeekAttackOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 0x40) return 5;
    if (unit->terrain == g_game->overflowBucket) {
        Vec3 center;
        center.x=(g_game->width/2)<<16;
        center.z=(g_game->height/2)<<16;
        short angle=GetHeadingBetween(&unit->pos,&center);
        Vec3 pos=AddVec3(unit->pos,Offset(angle,0x3200000));
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        ((Class_0044e730*)move)->SetApproachRadius(128);
        order->flags|=0xe0;
        order->SetAttachedFx((int)move);
        return 2;
    }
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0:
        if (unit->motion && (unit->def->flags1&0x800)) {
            if (order->target) {
                if (IssueAttackOrder(unit,order->target,0)) { order->flags=0; return 0; }
            } else {
                if (!order->pos.x && !order->pos.z && !order->pos.y) order->pos=unit->pos;
                order->angle=RandomInt(0x10000);
                order->parity=order->angle&1;
                unit->ClaimWeapons(3);
                if (unit->busy) AttachUnitToPiece(unit,0,-1,2);
                unit->SetStateBits(1,1);
                if ((unit->motion->flags&3)==1) {
                    unit->motion->SetFlightMode(unit,2);
                    Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                    ((Class_0044e6c0*)move)->SetAltitude(unit->def->altitude/2);
                    order->SetAttachedFx((int)move);
                    order->flags|=0xe0;
                }
            }
            return 1;
        }
        break;
    case 1: {
        unit->ReleaseWeapons(3);
        if ((unsigned int)unit->health < (unit->def->maxHealth>>2)*3) {
            LandingPadList pads;
            GetFactoriesInRadius(unit->owner->index,&unit->pos,0xf00,&pads);
            if (!pads.empty()) {
                order->SetAttachedFx(0);
                Unit* pad=pads[RandomInt(pads.count())];
                AppendOrder(unit,new Order("VTOL_LANDING",pad,0,0,0,0));
                order->flags=0;
                return 0;
            }
        }
        Unit* target=FindBestTargetIfFireAtWill(unit);
        if (target && IssueAttackOrder(unit,target,0)) return 5;
        if (flags&0xe0) order->angle+=-RandomInt(0x2000)-0x5555;
        Vec3 pos=AddVec3(order->pos,Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16));
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        ((Class_0044e730*)move)->SetApproachRadius(128);
        order->SetAttachedFx((int)move);
        order->SetDeadlineTicks(RandomInt(30)+30);
        order->flags|=0xe0;
        return 2;
    }
    }
    return 7;
}
