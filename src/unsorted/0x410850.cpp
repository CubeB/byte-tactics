// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by Claude Opus 5.5, finished by Claude Opus 5.5. re-verified by GPT-6, retried by Claude Opus 5.5, finished by GPT-6, retried by Claude Opus 5.5, finished by GPT-6. Names are provisional.
// #5616 (Opus, 98.6% kept). A toy with the same switch, test, pads block and
// visitor (build/scratch/0x410850/t/, gen2.py to gen7.py, compiled with /Fa)
// reproduces ours, which makes each idea a one-second test:
//  * The test's operand order depends on the rotation phase as well as on the
//    pairing. With no later `u->def` read, our phase gives the original's test
//    and block (as SearchRange returning a constant does here), and the next
//    phase gives ours. With the pairing, only the phase one step later gives the
//    original's test, and the block is then one step off. The matched 0x4103e0 is
//    health first with no CSE temp over its test (c2prio lists none), so there it
//    is the phase alone.
//  * The pairing is C2's global CSE temp for `u->def` over the test and the range
//    read (c2prio's "not in the list" shows `temp <test line>,<range line>`). It
//    forms across the calls between them; a store on one path (to a global,
//    through order, to another unit field) does not stop it; any health read
//    before (dominating, even a `cmp word ptr [esi+0x108], 0x7fff`) or after does.
//    Dead or folded health reads (unused local, `(void)`, comma, sizeof or
//    `int dbg = 0` guards, self-assignment, `&u->health`) are gone first.
//  * No temporary after the test, toy and real: `&& 1`, `== 1`, `!!`, `!= 0`,
//    `?:` on an int, `do {} while (0)`, `for (;;) {... break;}`, `while (...) {...
//    break;}`, `switch (IsDamaged(u))` with a default, a goto around the block,
//    extra braces, FindPads taking `X&`, `X*`, `Owner*` or the index, `X* pp =
//    &pads`, an index local, `X& p = pads`, `X& pads = X()`, IsDamaged on `const
//    U*`, `const U&`, a health local, Limit or Health helpers, and a pads class
//    whose inline constructor is too big for /Ob2 (an out-of-line call, as here).
//    `switch` with only `case 1:` materialises.
//  * Real file, test unchanged: `flags` as char, unsigned char, short or unsigned
//    short (97.8%), long or unsigned long; `unsigned int flags` (as in 0x4152f0) in
//    either or both functions; `for (;;)` around the switch or the whole body;
//    the water tail with `move` declared apart, a typed FUN_0044e730 pointer,
//    `flags = flags | 0xe0`, a named or-result, a named `(int)move`, an order
//    pointer local, the flags update before FUN_0044e730 (97.8%) or after
//    FUN_004388d0 (91.6%); selector locals and casts; a braced case 1. A
//    0x4152f0-style Land helper returning 1 or 0 (with or without its
//    `do {} while (0)`) breaks the inline budget (64 to 73%).
// #5592 (Claude Opus 5.5, 98.6% kept). Matched code with the same test, read with
// a c2prio --rotation that prints each temporary's tuple opcode:
//  * The test bytes (def edx, health ecx, lea eax, `jae`) occur in 0x410e70,
//    0x411f50 and 0x412710 (matched) and 0x4152f0 (partial, test region right).
//    0x410e70 and 0x412710 generate it def first from pointer edx; neither reads
//    `u->def` after the test. 0x4152f0 reads `unit->def->range` after its test
//    (the same shape as SearchRange here) and generates it health first, one
//    step later (health ecx from pointer ecx), because its own earlier code
//    leaves the pointer there; its block after the test then starts at ecx with
//    an extra lea for std::vector's allocator argument, which Class_00410830's
//    out-of-line constructor does not have. So the matched siblings give no
//    spelling to copy: each gets the bytes from its own surroundings.
//  * Toys (the test, a call, then `F(1, u->def->searchRange << 16, 2)`, read
//    with the rotation opcodes): any later read of `u->def` (an int field, the
//    short searchRange, the pointer itself, and 10 address spellings of it:
//    `((short*)u->def)[0x101]`, `*(short*)((char*)u->def + 0x202)`,
//    `(*(UnitDef**)((char*)u + 0x92))`, `u[0].def`, `(u + 0)->def`, a
//    `UnitDef**` local, `u->def[0]`, ...) makes the test health first; later
//    reads of globals, another pointer's def or `u->health` keep it def first.
//    Ten spellings of the test itself (casts, `3 *`, `3u`, `/ 4`, `!(>=)`,
//    unsigned long, a limit or health local) stay health first. What breaks it
//    is the test not dominating the later read: wrapping the test in a real
//    `if (g)` does; a `do {} while (0)` or `for (;;) {... break;}` around the test
//    or the read, a goto/label, a loop between, a redundant `if (k == 1)` after
//    `if (k != 1) return`, or `int skip = 0; if (skip) goto L;` (both folded
//    before the pairing) do not.
//  * `do {} while (0);` after the test block (0x4152f0's Land has one): test
//    unchanged, inline budget broken (1070 bytes, 92.2%).
//  * With the throwaway `g_a = g_b;`: every int-sized IsDamaged return type and
//    `!= 0`, `&& 1`, `|| 0`, `!!`, `!(== 0)`, `== true` add no temporary; short,
//    char, bool returns, `> 0`, `(bool)`, `& 1`, `== false`, `^ 1`, `!= true`,
//    `< 1` all materialise (93.2%); `?:` forms grow the code (83.4%).
//  * An inline Class_00410830 constructor (empty body, base initialiser, or
//    out-of-class inline) gets inlined: 65.7%, 70.3% with the throwaway.
//  * <windows.h> 98.6%; <math.h> or WIN32_LEAN_AND_MEAN <windows.h> 86.9%. A
//    dummy-extern sweep (1 to 300 before the includes, 1 to 3000 after) is
//    98.6% or 86.9% in 256-wide windows; the 86.9% windows only swap order
//    between edi and ebp, the test never moves.
// #5551 Codex recheck: 98.6%; only the health-test scratch-register order remains.
// #5581 (Claude Opus 5.5, 2026-10-04, 98.6% kept). c2prio --rotation with each
// temporary's tuple opcode, in codegen order after the selector (ah109): ours
// health dc5, def a1, limit c12, then &pads d12, &pos a12, owner c1, index
// d109 (the pads constructor's `this` lea is not a rotation temporary). With
// the throwaway `g_a = g_b;` after the water branch's FUN_004388d0, a bool
// `!IsHealthy(unit)` adds exactly one temporary (c108) between the limit and
// &pads: the test and the whole block then match and only the materialised
// `sbb ecx,ecx; inc ecx; test cl,cl; jne` differs from `jae` (93.2% with the
// throwaway's 12 bytes). So the after-test temporary sits right after the
// limit's lea. Flat in the rotation window, with the throwaway in place:
// IsDamaged through `int damaged`, `!= 0`, `== 1`, a limit local, a damaged
// local inside, a NeedsRepair wrapper; FindPads taking the position, owner,
// player or range as parameters, the vector by reference or as
// Class_00410830*; pads as a derived class (implicit or empty inline
// constructor) or a struct member. bool/char IsDamaged forms (if/return,
// ?:) either materialise or change nothing. Without the throwaway, no change:
// Patrol's flags parameter as any char/short/unsigned type or the argument
// cast; Class_0044e2d0 derived from Class_0044e730 with implicit upcasts
// (FUN_004388d0 taking the base pointer breaks the inline budget, 1070
// bytes); FUN_0044e730 taking short; a class operator new; a NewMove(order,
// pos) inline by reference or pointer (70-72% once it also makes the
// FUN_0044e730 call or is used in Patrol's tail).
// #5555 (Claude Opus 5.5, 2026-10-04, no gain, about 300 variants): the
// original keeps the health-first order (the def pairing with SearchRange is
// there too). Its rotation pointer is just one step later (eax, which the
// switch selector still holds), so health gets ecx, def edx and lea eax.
// c2prio --rotation on a throwaway `g_a = g_b;` at the end of the water
// branch (one rotation temporary after the ctor's `lea` for &pos, #15) shows
// exactly this. Then the FUN_0040b530 block needs one more temporary after
// the test. A bool, char or short IsDamaged supplies it (with the throwaway,
// test and block both match), but the result is then materialised
// (`sbb; neg; test; je` instead of `jae`). Flat or worse (region check), all
// with and without the throwaway:
// - water-branch tails: `return Go(order, move)`, a result local, the whole
//   branch as a helper (costs 142 IL of Patrol's budget);
// - switch shapes: a default case, case 0 returning 7, case 1 first;
// - no-op statements at the water end, before the switch or in Patrol (all
//   gone before temporaries are assigned; in Patrol most cost budget);
// - FindPads with an owner local, owner or index parameter, or as an Owner
//   method;
// - range-read shapes (array, nested struct, union with an int, `* 0x10000`,
//   short return), member forms of IsDamaged/SearchRange or UnitDef
//   accessors, dead health/def reads, and the flags parameter as
//   char/short/unsigned. A SearchRange that reads anything other than
//   `u->def` gives the right region at once (no pairing, def first), as noted
//   below.
// The matched sibling 0x4103e0 (same test and FUN_0040b530 block as ours)
// has the same pointer as ours at its test.
// #5512 (Claude Opus 5.5, 98.6% kept): re-read the test with a patched
// c2prio --rotation that prints each temporary's tuple opcode (01 load,
// 12 lea, c5 movsx, 109 movzx byte). Ours generates the test as health
// (edx), def (eax), lea (ecx) from pointer edx. The matched 0x411f50 has the
// same test bytes as the original here and is also generated health first,
// from pointer eax with eax busy (so ecx, edx, eax). So the original can be
// either def first from pointer edx, or health first one rotation step later;
// the second also needs the FUN_0040b530 block's temporaries (ours: &pads,
// &pos, owner, index) to come out in the order owner, &pads, &pos, index,
// which no spelling gave. Toy functions (build/scratch/0x410850/t/) show
// the operand order is not a value CSE: a later `u->def` read flips it to
// health first even after a store to `u->def` or through a second pointer
// `w`, a later `w->def->maxHealth` or `w->def->rangeInt` (int) read flips it
// back, a later short read can go either way, and only taking `&u` (u then
// lives in memory) or a real health read gives def first in the same
// function. Flat here, all with the test unchanged: the brief's routes for
// the def read (inline GetDef accessor, `UnitDef*&`, `UnitDef**`, a def local
// captured at the top of Patrol or passed in from FUN_00410850), IsDamaged
// returning bool or through a bool/int local, health and def as helper
// arguments in either order, Patrol/IsDamaged/SearchRange taking `Unit*&` or
// `Unit**`, case 1 before case 0, the selector as `& 0xff` or a local,
// `return Patrol(...)`, eight water-branch spellings (centre order, angle
// int or nested, Direction result named, `flags |= 0xe0` four ways) and five
// FindPads spellings (vector reference, unsigned char player, a player
// local). The locals and the bool forms also break Patrol's inline budget.
// Codex / GPT-6 retry on 2026-10-04: best remains 98.6% (1051 bytes), with
// the health-test scratch-register order as the only mismatch. A no-op
// `unit->health = unit->health` after the damaged-unit branch tried to break
// the def CSE, but raised the function to 1070 bytes and dropped to 92.2% by
// changing inlining and register allocation elsewhere. Restored the original
// best. c2prio could not run because gdb and winedbg are unavailable; prior
// notes record a 15414-candidate permuter run without improvement.
//
// #5417 (Claude Opus 5.5): still 98.6%, but the cause is now pinned down, and it
// is not missing temporaries. C2 generates the test's operands in the order
// health (movsx), def load, lea; the original's registers (def edx, health ecx,
// lea eax, with the FUN_0040b530 block unchanged) are exactly what the order
// def load, lea, health gives from the same rotation pointer (edx), with no
// extra temporaries. C2 puts health first because the test's `u->def` read is
// a common subexpression of SearchRange's later `u->def` read (the test
// dominates it; calls and other stores in between do not break it). Each of
// these throwaway changes gives the original's test and block: SearchRange
// returning a constant, or any later read or store of `unit->health`. In toy
// files: a later read of the def field makes the compare health-first; a later
// (or earlier, dominating) read of health as well, a store to health, passing
// `&u->health` to a call, or reassigning the unit pointer between the two reads
// gives def-first again. Spellings of the second read that do NOT break the
// pairing: a local pointer or reference to the field, casts, const, another
// struct with the field at the same offset, reading via `visitor.self`,
// SearchRange(unit->def), computing the range inline or before the test, and
// moving the test out of Patrol. Front-end id shifts of 0 to 60000 do not move
// it either. So the original's range read must not be dominated by the test's
// def read (or health is read again somewhere), and no natural spelling found
// does that. `tools/c2prio.py --rotation` with each temporary's tuple opcode
// (+4 of the tuple: 01 load, 12 lea, c5 movsx, 109 movzx byte) shows the order.
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
