// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by Claude Opus 5.5, finished by Claude Opus 5.5. re-verified by GPT-6, retried by Claude Opus 5.5, finished by GPT-6. Names are provisional.
//
// Claude Opus 5.5, #5302: 94.6% -> 98.6% (1051 bytes, the right size). Two of
// the three residuals of the 94.6% file are gone; what fixed them:
// #5368 retry: current main confirms 98.6%; the health-test scratch-register
// order remains the only mismatch, with no natural source lever in prior sweeps.
// #5385 retry: re-confirmed 98.6%; health-test scratch-register order remains.
// #5400 retry: 256 common C-header sets stayed flat; /Gi dropped to 82.0%.
// The def/health/limit register swap is unchanged, as in matched sibling 0x4103e0.
//  * The tail is `order->pos + Offset(...)` again, with no `off` local
//    (5 IL less), and Offset names its second call's result (`int z`). The
//    extra candidate in the tail block raises the Offset distance's priority
//    from 132 to 144 against order's 141 (tools/c2prio.py), so the distance
//    takes esi and unit/order keep esi/edi. Without `int z`, order wins by one
//    point and unit and order swap everywhere (82.4%).
//  * The visitor has the 3-argument constructor, which puts the vtable store
//    after the member stores as in the original. It costs 54 IL of Patrol's
//    budget, paid for by: operator+ as a free function (65 IL, the member one
//    is 78), and three small helpers placed before units.empty() (IsDamaged,
//    FindPads, SearchRange, each under 41 IL and so free). Each helper is
//    needed: without any one of them the last ~UnitList's _Destroy goes out
//    of line (1070 bytes) or units.empty()'s size() does (1037). Margins with
//    all three: units.empty()'s size() gets 311 / 7 = 44 for its 42, the last
//    _Destroy 52 for its 49 (`c2prio.py --inline`).
//
// Still different (6 lines): the health test's scratch registers. The
// original has def/health/limit in edx/ecx/eax (`lea eax,[eax+eax*2]`), ours
// eax/edx/ecx. Measured with throwaway `g_a = g_b;` copies (separate scalar
// globals) in a scratch copy, n before the test and m between the test and
// the pads constructor: the test comes out as the original's for n = 1 or 2
// (anywhere after the water branch's `order->flags |= 0xe0`), and the
// FUN_0040b530 argument block after it stays right only when n + c + m is a
// multiple of 3, which with the test's own count c gives (n, m) = (1, 1) or
// (2, 1). So the original has one or two more rotating temporaries before
// the test and one more after it, all emitting no code. Flat (byte-identical):
// 14 spellings of the test inside IsDamaged (locals for def, health, limit,
// quarter, `/4`, casts, if/return), the matched sibling 0x4103e0's
// `unsigned int state=0; state=order->state;` selector and three other
// selector spellings, copies of unit/order/flags/move, a local for the
// return values, a `Clear(order)` helper, and the callees' own prototypes
// (FUN_004388d0 takes a pointer, FUN_0044e730 a short). Spellings that add
// IL to Patrol (helpers taking health and def as arguments, a Limit()
// helper) break the inline budget first. A 15-minute permute.py run from
// this file (15414 candidates) found nothing better. 0x4103e0 (matched, same
// family) has eax/edx/ecx here, like ours, after a FUN_00489800 call.
//
// Earlier notes (the Patrol() inline budget, read out of C2.EXE):
// a function's inline budget is max(1000, 2 * its own IL size). Call sites are
// taken in source order. A callee is inlined when its IL size is at most the
// budget left, or under 41 whatever the budget; only callees of 41 or more
// subtract their size. The calls inside an inlined callee get budget / R,
// where R is the number of this level's call sites still to come, including
// this one. Case 1's body is the inline helper Patrol(), which makes the
// function small (budget 1000) and leaves Patrol's own sites 1000 minus its
// IL, which puts every vector site on the original's side (the landing pads'
// constructor, empty()'s size(), both pads destructors, the units' vector
// constructor and the `return 3` destructor are calls; pads.size(),
// units.empty()'s size() and the final destructor are inline). The pads
// class keeps a declared (never inlined) constructor, as in the matched
// 0x4103e0; units is a separate class with an implicit one. Older notes are
// in this file's git history.
#include <vector>
struct Vec3 {
    int x, y, z;
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
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[4]; short searchRange; char pad204[0x21c-0x204]; short altitude; char pad21e[0x231-0x21e]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
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
short __stdcall FUN_0048a980(Vec3*, Vec3*);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
Vec3 __stdcall FUN_0040f790(const Vec3& a, const Vec3& b);
union Fixed { int value; struct { unsigned short frac; short whole; } parts; };
Vec3 __stdcall FUN_004103a0(short angle, Fixed scale);
static inline Vec3 operator+(const Vec3& a, const Vec3& b) { Vec3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static inline Vec3 Direction(short angle, int range) { Fixed distance; distance.value=range; return FUN_004103a0(angle,distance); }
static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x=-FUN_004b70ef(angle,distance);
    v.y=0;
    int z=FUN_004b7123(angle,distance);
    v.z=-z;
    return v;
}

class Class_00410830 : public std::vector<Unit*> { public: Class_00410830(); };
void __stdcall FUN_0040b530(int, Vec3*, int, std::vector<Unit*>*);
class UnitList : public std::vector<Unit*> {};

class Class_00410c70 {
public:
    virtual void FUN_00410c70(Unit*);
    Class_00410c70(Owner* o, std::vector<Unit*>* u, Unit* s) : owner(o), units(u), self(s) {}
    Owner* owner; std::vector<Unit*>* units; Unit* self;
};
void __stdcall FUN_0047e890(Vec3*, int, const Class_00410c70&);
static inline int IsDamaged(Unit* u) { return (unsigned int)u->health < (u->def->maxHealth>>2)*3; }
static inline void FindPads(Unit* u, std::vector<Unit*>* pads) { FUN_0040b530(u->owner->index,&u->pos,0xf00,pads); }
static inline int SearchRange(Unit* u) { return u->def->searchRange<<16; }
static inline int Patrol(Unit* unit, Order* order, int flags)
{
    if (IsDamaged(unit)) {
        Class_00410830 pads;
        FindPads(unit,&pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Unit* pad=pads[FUN_004b6c30(pads.size())];
            FUN_0043acb0(unit,new Class_0043a1f0("VTOL_LANDING",pad,0,0,0,0));
            order->flags=0;
            return 0;
        }
    }
    UnitList units;
    int range=SearchRange(unit);
    Class_00410c70 visitor(unit->owner,&units,unit);
    FUN_0047e890(&unit->pos,range,visitor);
    if (!units.empty()) {
        ((Class_004388d0*)order)->FUN_004388d0(0);
        Class_00438760 kind=FUN_0043f0e0(7,unit,units[0],0);
        FUN_0043acb0(unit,new Class_0043a1f0(kind,units[0],0,0,0,0));
        order->flags=0;
        return 3;
    }
    if (flags&0xe0) order->angle+=-FUN_004b6c30(0x2000)-0x4000;
    Vec3 pos=order->pos+Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16);
    Class_0044e2d0* move=new Class_0044e2d0(order,pos);
    ((Class_0044e730*)move)->FUN_0044e730(128);
    ((Class_004388d0*)order)->FUN_004388d0((int)move);
    ((Class_00439e80*)order)->FUN_00439e80(30);
    order->flags|=0xf8;
    return 2;
}
// FUNCTION: 0x410850
int __stdcall FUN_00410850(Unit* unit, Order* order, int flags)
{
    if (flags&0x40) return 5;
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
    switch(order->state) {
    case 0:
        if (unit->motion && (unit->def->flags&0x800)) {
            Vec3* pos=&order->pos;
            if (!pos->x && !pos->z && !pos->y) *pos=unit->pos;
            order->angle=FUN_004b6c30(0x10000);
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
