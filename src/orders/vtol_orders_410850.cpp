// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by Claude Opus 5.5, finished by Claude Opus 5.5. re-verified by GPT-6, retried by Claude Opus 5.5, finished by GPT-6, retried by Claude Opus 5.5, finished by GPT-6, matched by Claude Opus 5.5. Names are provisional.
// MATCH (Claude Opus 5.5, #5656). The code was right at 98.6%; the last six
// lines (the health test's def, health and limit registers) are decided by
// the front-end id of `unit`, and match when it is 65808 to 65919. The
// headers above the types put it at 65854: include/ta_types.h (the game's
// merged types, standing in for the declarations Cavedog's headers put in
// front of this file) and real headers from docs/c2-regalloc.md's plausible
// set. ta_types.h's views of the types defined below differ from this file's
// (it flattens Class_00410830's std::vector<Unit*> base into fields and splits
// Order's flags into bitfields), so it is kept in its own namespace; the
// system headers it includes come first and stay global.
//
// How the test's order is decided, read out of C2.EXE (Ghidra on the C2
// project, gdb watchpoints through a patched tools/c2prio.py):
//  * FUN_004056cf, which lays out each tree's tuples, emits a binary node's
//    second operand first when FUN_00406912 says the operator may be swapped
//    (the compares 0x141 to 0x157 are) and FUN_00408d58 finds the second
//    operand's key at +0xc larger than the first's (unsigned).
//  * FUN_004071f1 sets the keys: (level << 24) | (count << 16) | hash.
//    FUN_00405301's level is Sethi-Ullman-like and the count adds up the
//    variables; FUN_004053bf's 16-bit hash XORs the operands' hashes and the
//    opcode, down to a variable's front-end id (low 16 bits XOR high 16 bits)
//    or, for a C2 temporary, its symbol record's pool number (+0x1c, numbered
//    in blocks of 32 by FUN_0040c75a).
//  * Health and limit both have level 1 and count 1, so the hashes decide:
//    health's is id(unit) ^ 0x17c. The test's `unit + 0x92` is a common
//    subexpression of SearchRange's `unit->def` read, so limit's hash comes
//    from that temporary's pool number (0x1e2, giving 0x72) instead of from
//    unit. The limit goes first (def edx, health ecx, `lea eax`, the
//    original) only when id(unit) ^ 0x17c is below 0x72: ids 65808 to 65919
//    (a few just below; 272 to 383 by the same rule). That is why no
//    spelling of the test or of the second def read moved it, and why the
//    toy files followed u's id. Without the pairing the two hashes differ
//    only in bit 4 of id(unit).
//  * Other sets that land (unit's id): the same with <memory.h> for
//    <float.h> (65845), <time.h> alone (65818), <conio.h> for <time.h> and
//    <float.h> (65836). A regenerated ta_types.h that moves unit by more than
//    about 60 ids needs the set retuned (`tools/c2prio.py --symbols unit`).
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
// 0x4103e0; units is a separate class with an implicit one.
//  * The tail is `order->pos + Offset(...)`, with no `off` local, and Offset
//    names its second call's result (`int z`): that raises the Offset
//    distance's priority over order's so unit and order keep esi and edi.
//  * The visitor has the 3-argument constructor, which puts the vtable store
//    after the member stores. operator+ is a free function (65 IL, the
//    member one is 78), and IsDamaged, FindPads and SearchRange are small
//    helpers before units.empty() (each under 41 IL and so free); without
//    any one of them a vector call goes in or out of line.
// Older notes (ten passes of spellings, all flat) are in this file's history.
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
class Class_00489800 { public: void ReleaseWeapons(int); };
#pragma pack(push, 1)
struct WeaponDef { char pad0[0xdc]; int range; char pade0[0x111-0xe0]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[4]; short searchRange; char pad204[0x21c-0x204]; short altitude; char pad21e[0x231-0x21e]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
class Class_0043d210 { public: char pad0[0x2e]; unsigned char flags; void SetFlightMode(Unit*, int); };
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
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
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
void __stdcall GetFactoriesInRadius(int, Vec3*, int, std::vector<Unit*>*);
class UnitList : public std::vector<Unit*> {};

class Class_00410c70 {
public:
    virtual void FUN_00410c70(Unit*);
    Class_00410c70(Owner* o, std::vector<Unit*>* u, Unit* s) : owner(o), units(u), self(s) {}
    Owner* owner; std::vector<Unit*>* units; Unit* self;
};
void __stdcall FUN_0047e890(Vec3*, int, const Class_00410c70&);
static inline int IsDamaged(Unit* u) { return (unsigned int)u->health < (u->def->maxHealth>>2)*3; }
static inline void FindPads(Unit* u, std::vector<Unit*>* pads) { GetFactoriesInRadius(u->owner->index,&u->pos,0xf00,pads); }
static inline int SearchRange(Unit* u) { return u->def->searchRange<<16; }
static inline int Patrol(Unit* unit, Order* order, int flags)
{
    if (IsDamaged(unit)) {
        Class_00410830 pads;
        FindPads(unit,&pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Unit* pad=pads[FUN_004b6c30(pads.size())];
            AppendOrder(unit,new Class_0043a1f0("VTOL_LANDING",pad,0,0,0,0));
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
        AppendOrder(unit,new Class_0043a1f0(kind,units[0],0,0,0,0));
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
int __stdcall VtolSeekGuardOrder(Unit* unit, Order* order, int flags)
{
    if (flags&0x40) return 5;
    if (unit->terrain==g_game->water) {
        Vec3 center;
        center.x=(g_game->width/2)<<16;
        center.z=(g_game->height/2)<<16;
        short angle=GetHeadingBetween(&unit->pos,&center);
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
