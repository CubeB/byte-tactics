// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by Claude Opus 5.5, finished by Claude Opus 5.5. re-verified by GPT-6. Names are provisional.
// Codex GPT-6 retry for #5213 (2026-10-03): `/Gi` drops this function
// to 69.5%; the 94.6% default-flags version and prior inlining-budget
// and sibling-shape findings remain best.
// #5262 Codex retry: best remains 94.6% (1055 bytes, 97.1% excluding
// internal jump targets). A no-op visitor constructor was byte-identical;
// aggregate center initialization scored 86.9%, the reversed health compare
// 94.3%, and named health/limit locals 67.7% with the unit/order registers
// swapped. Restored the previous best.
// Claude Opus 5.5, #5142: 92.2% -> 94.6% (1055 bytes against 1051), with no
// Dummy() padding. Case 1's body is an inline helper, Patrol(), called as
// `int result=Patrol(unit,order,flags); return result;`, and the visitor is
// a named local with an implicit constructor and three member stores. That
// reproduces the original's whole mix of inlined and out-of-line vector
// calls (the landing pads' constructor, empty()'s size(), both pads
// destructors, the units' vector constructor and the `return 3` destructor
// are calls; pads.size(), units.empty()'s size() and the final destructor,
// down to `operator delete(first)`, are inline).
//
// Why, from C2.EXE (inliner FUN_004249cb, driver FUN_0042491e, call-site list
// FUN_004251e8): a function's inline budget is max(1000, 2 * its own IL
// size). Call sites are taken in source order. A callee is inlined when its
// IL size is at most the budget left, or under 41 whatever the budget; only
// callees of 41 or more subtract their size. The calls inside an inlined
// callee get budget / R, where R is the number of this level's call sites
// still to come, including this one. So empty Dummy() calls (size under 41)
// cost nothing themselves but raise R for every earlier site; that is all
// the old 98.6% padding did. Patrol() makes F small (IL 314, budget 1000)
// and leaves Patrol's own sites a budget of 1000 - 606 = 394, which puts
// every vector site on the original's side. The margins are thin: units'
// size() needs (394 - 92) / 7 >= 42, so any extra site after units.empty()
// or about 8 more IL in Patrol breaks it (a 3-argument visitor constructor,
// IL 54, does).
//
// Still differing:
//  1) the case-1 health test: def/health/limit in eax/edx/ecx against the
//     original's edx/ecx/eax. These are expression temporaries (regasg.c,
//     FUN_00435c37), and the rotating pointer arrives at Patrol's first
//     temporary on edx; with it on ecx the original's three registers follow
//     exactly. So the original has one rotating temporary fewer (or two more)
//     in the prologue or the water branch, which compiles to the same bytes:
//     16 temporaries there in every spelling tried (helpers for the water
//     test, the move creation and the center, int/short locals, Fixed local,
//     `::new`, operand order, a result local for return 5).
//  2) the visitor's vtable store comes before the units/self stores; the
//     constructor form (Y1d34) orders them right but costs 54 IL, over the
//     budget above.
//  3) the tail: pos is copied from PosOf(order) and summed in place, which
//     leaves a dead `mov [esp+0x10],ecx` and puts pos at 0x10, not 0x1c.
//     `order->pos + off` (Vec3::operator+) gives the original's tail but
//     then order wins esi over the Offset distance by one point of priority
//     (133 to 132, tools/c2prio.py) and unit and order swap esi/edi
//     everywhere (82%). PosOf adds candidates to the tail block and lifts
//     the distance to 144 against order's 141.
// The pads class keeps a declared (never inlined) constructor, as in the
// matched 0x4103e0; units is a separate class with an implicit one.
// Earlier notes (the hand-written vector specialisation at 92.2%, and the
// 98.6% Dummy() lead) are in this file's git history.
// GPT-6 retry (#5173): one-argument visitor constructors that initialized
// `self` or `owner` did not improve the 94.6% score. A two-argument constructor
// for `owner` and `self` dropped to 69.5%; the original best remains in place.
#include <vector>
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
static inline Vec3 Direction(short angle, int range) { Fixed distance; distance.value=range; return FUN_004103a0(angle,distance); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }

class Class_00410830 : public std::vector<Unit*> { public: Class_00410830(); };
void __stdcall FUN_0040b530(int, Vec3*, int, std::vector<Unit*>*);
class UnitList : public std::vector<Unit*> {};

class Class_00410c70 {
public:
    virtual void FUN_00410c70(Unit*);
    Owner* owner; std::vector<Unit*>* units; Unit* self;
};
void __stdcall FUN_0047e890(Vec3*, int, const Class_00410c70&);
static inline Vec3 PosOf(Order* o) { Vec3 r; r.x=o->pos.x; r.z=o->pos.z; r.y=o->pos.y; return r; }
static inline int Patrol(Unit* unit, Order* order, int flags)
{
    if ((unsigned int)unit->health < (unit->def->maxHealth>>2)*3) {
        Class_00410830 pads;
        FUN_0040b530(unit->owner->index,&unit->pos,0xf00,&pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Unit* pad=pads[FUN_004b6c30(pads.size())];
            FUN_0043acb0(unit,new Class_0043a1f0("VTOL_LANDING",pad,0,0,0,0));
            order->flags=0;
            return 0;
        }
    }
    UnitList units;
    int range=unit->def->searchRange<<16;
    Class_00410c70 visitor;
    visitor.owner=unit->owner;
    visitor.units=&units;
    visitor.self=unit;
    FUN_0047e890(&unit->pos,range,visitor);
    if (!units.empty()) {
        ((Class_004388d0*)order)->FUN_004388d0(0);
        Class_00438760 kind=FUN_0043f0e0(7,unit,units[0],0);
        FUN_0043acb0(unit,new Class_0043a1f0(kind,units[0],0,0,0,0));
        order->flags=0;
        return 3;
    }
    if (flags&0xe0) order->angle+=-FUN_004b6c30(0x2000)-0x4000;
    Vec3 off=Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16);
    Vec3 pos=PosOf(order);
    pos.x+=off.x;
    pos.z+=off.z;
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
