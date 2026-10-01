// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// deepseek-v4.1-flash (issue #3490) retry: baseline reconfirmed 98.0%. What
// still differs is only the state-3 misses>=2 waypoint schedule at
// 0x413950..0x413966 (17 bytes): the original emits load pos.x, push 0x36,
// add off.x (consuming edi, then reusing edi for pos.y), load pos.y, load
// pos.z, store x, add z, store y, store z, while ours hoists all three pos
// loads, sums, then pushes, using ebx for pos.y. Seven scratch variants this
// retry (off + pos operand swap 94.4, Offset temp inline in the sum 98.0
// identical, member-wise p.x/p.y/p.z fresh local 94.3, sum straight to the
// new-expression with named off 93.4, free operator+ 98.0 identical,
// operator+ folding y to a copy 90.5, Unit* target local 94.5) all scored
// 98.0 or worse and none moved the schedule pair. The only executable
// mismatch left is that 17-byte region; the jump-table hunks the checker
// prints are masked placeholder rendering and do not count.
// deepseek-v4.1-flash (issue #3435) tenth retry: baseline reconfirmed 98.0%,
// still the single region at 0x413950..0x413966 (state-3 misses>=2 waypoint,
// 17 bytes). Twenty source variants around the `pos + off` sum and the
// new-expression (member-wise sums, an AddXZ/VecAdd/AddXYZ helper, a Vec3& to
// off, a Vec3&/const Vec3& to target->pos, a named distance local, `speed *
// 0x10000`, and the sum passed straight to the constructor) scored 93.0 to
// 98.0 and none moved the register/schedule pair. tools/headers.py found no
// matching header set; an unused-extern sweep for N=0..336 was flat at 98.0;
// and defining the real preceding function 0x412d40 above this one in a
// scratch file also left region 2 unchanged, so the tie-break is not compiler
// state from the earlier translation unit either. The original consumes off.x
// (edi) and then reuses edi for pos.y, placing `push 0x36` between the loads;
// ours hoists all three pos loads and uses ebx for pos.y. Best kept: 98.0%.
// GPT-6.1-sol (issue #2635) retry: check.py reconfirmed the saved 98.0% best. A y/z/x member-wise rewrite scored 94.6% and shifted unrelated code, so it was discarded. The only remaining executable mismatch is the state-3 waypoint schedule at 0x413950-0x413966: target consumes off.x before loading pos.y, while MSVC hoists y/z loads and uses ebx. Earlier notes below document the tested variants.
// deepseek-v4.1 (issue #1897) fifth retry: baseline reconfirmed 97.4%, same two
// regions. Negating both hypot arguments (unit->pos.xw - order->x, ...) scores
// 96.8: the compiler still emits the [esi] (order) movsx first, this time into
// edx with `sub eax, edx`, and shifts the spill to [esp+0x50]/fild [esp+0x50],
// so the load pair is base-register driven, not operand driven. Sixth retry by
// deepseek-v4.1 (issue #1897) re-confirmed both spellings and their exact
// schedules: with `unit->pos.xw - order->x` first the compiler still issues the
// esi-based (order) movsx first but into edx with `sub eax, edx`, then spills
// eax through [esp+0x50] and filds it before the second spill, so even the
// destination register follows the base register, not the source operand.
// State-3 waypoint variants: sum passed straight to the constructor (92.8, temp moves down 0xc),
// member-wise p.x/p.y/p.z into a fresh local (93.7, frame -4), explicit += on a
// copy of target->pos (93.0, frame +4), all worse. Best kept: baseline.
// #1704 retry by Codex / GPT-6.1-sol: checkall reconfirmed 97.4%, no MATCH.
// Eight worker checks found no improvement over the existing source.
// GPT-6 retry: range/difference helpers, target-position addition helpers,
// declaration layout, constructor bodies and coordinate field names did not
// improve 97.4%. The first hypot and missed-attack waypoint loads still differ.
// deepseek-v4.1 (issue #2541) eighth retry: 98.0%, up from 97.4. Region 1
// (the range-check _hypot) is FIXED by binding short references to the two
// order fields, `short& ox = order->x; short& oz = order->z;`, and spelling
// the call unit-first: (int)_hypot(unit->pos.xw - ox, unit->pos.zw - oz).
// The references keep both values live so MSVC materialises the unit (edi)
// movsx pair first, exactly like the original; every direct spelling of the
// same subtraction (order-first 97.4, swapped-argument 96.8/97.4, unit-first
// 96.8, int locals 91.2, short locals 90.3, inline helper 96.8) emits the
// [esi] (order) load first instead. Only region 2 is left: the state-3
// misses>=2 waypoint at 0x41394b-0x41396a, where the original consumes edi
// (off.x) via `add edx, edi` before loading target->pos.y back into edi and
// pushes the 0x36 new size between the loads, while ours hoists all three
// pos loads, uses ebx for pos.y, then adds and pushes. Vec3& to
// target->pos (98.0, identical), declared-then-assigned off/p (98.0,
// identical), member-wise int references (94.6), member-wise p.x/p.y/p.z
// (93.7) and `p = off; p += target->pos;` (93.0) all fail. Best kept: 98.0%.
// VTOL attack order handler for a unit target. With flags 0x10008, or with
// no target and order flag 0x200, it queues VTOL_SEEKATTACK; on the map-edge
// player it heads for the map centre. State 0 prepares the order
// ("Attacking"; FUN_0040f200 is defined here because /Ob2 inlined it), state
// 1 flies to a random point halfway to the target, state 2 attacks, state 3
// circles the target, alternating sides, and lands on a free pad when
// damaged (VTOL_LANDING).
//
// Partial: 97.4%. deepseek-v4.1 (issue #1897) retry: still 97.4%, confirmed by
// disassembling our own object. MSVC5's load order in `a - b` follows the
// memory operand, not the source operand order: for this compare it always
// loads the order-based side (esi) first into edx and the unit-based side
// (edi) second, so negating the hypot arguments only flips the sub direction
// (96.8), and negating just the z difference scores 96.6. The remaining
// region-B hunk is pure scheduling: the original defers the pos.y/pos.z loads
// until after `add edx, edi` and reuses the freed edi for pos.y, while ours
// hoists both loads and uses ebx; the explicit member-wise form (93.7), a
// Vec3& reference local (96.5) and a member-wise operator+ (89.8) all differ
// more.
// What still differs:
// - The first _hypot: writing the arguments negated (order->x - unit->pos.xw)
//   fixed the sub direction and gained 96.8 -> 97.4, but the two loads of
//   each difference come out swapped (the compiler emits order->z first, the
//   original unit->z). It is the same class of compiler-state tie-break as
//   the sibling 0x411f50; all 128 header sets plus C++ headers, and a sweep
//   of 0..129 unused extern declarations, leave it unchanged. Reordering the
//   operands, reference/pointer locals and short temporaries all score worse.
// - State 3, after two misses: the original consumes edi (off.x) in the x sum
//   before loading target->pos.y into edi (`mov edx, [ecx]`; `add edx, edi`;
//   `mov edi, [ecx+4]`); ours loads all three members first and puts y in ebx.
//   Explicit per-member sums, dropping the `off` local and operator+= all
//   change the frame or fold the base pointer and score worse.
// deepseek-v4.1 (issue #1897) second retry: check.py baseline reconfirmed
// 97.4%; the operand-negation experiment (a2.cpp, 96.8%) was re-read and shows
// the compiler always loads the order (esi) side into edx first, so the
// original edx <- [edi+0x74] / eax <- [esi+0x30] pair is a register tie-break
// no source operand order reaches. The jump-table diff line
// (`jmp dword ptr [eax*4 + <addr>]` vs `+ 0x413ba8`) is separate: the checker
// renders `<addr>` for an address outside the function region, and the last
// hunk is that same 16-byte table plus padding, so that table is not byte-for
// byte where the original keeps it.
// Suspected original bug: that same branch builds a Class_0044e2d0 waypoint
// and sets its speed, but never passes it to the order (no FUN_004388d0
// call, unlike every other branch), so the object leaks.
// deepseek-v4.1-flash (issue #1897) third retry: baseline reconfirmed 97.4%.
// Exactly 25 of 1864 bytes differ, in only two regions: 0x413675..0x41368d
// (the range-check _hypot) and 0x413950..0x413966 (the state-3 first
// waypoint). The jump-table hunks the checker prints are only placeholder
// rendering of the DIR32 table entries; those bytes are masked and do not
// count. For the _hypot: unit-first operands (96.8), mixed operand order
// (97.1), two named int locals for the two differences (91.0), (short) casts
// (91.8), a Vec3& to unit->pos (81.6), a Unit* local (97.4, identical) and
// short/pointer locals (90.3) all fail. For the state-3 waypoint: explicit
// member sums (93.0), operator+= (93.0), assigning p field by field (93.5),
// an int speed<<16 local (97.4, identical), inlining Offset (97.4), a Unit*
// target local (93.5), a Vec3* target local (92.8) and passing the sum
// straight to the constructor (92.8) all fail. Both hunks are one allocator
// tie-break: MSVC fuses each difference as movsx into edx/ecx for the
// esi-based (order) operand and movsx into eax for the edi-based (unit)
// operand, and in the waypoint it reuses edi for pos.y instead of hoisting
// pos.y to ebx before consuming off.x. No source spelling tried reaches the
// opposite assignment. The sibling 0x412d40 MATCHES with the reverse roles
// (unit in esi, order in edi), so the tie-break depends on context, not on
// operand order.
// deepseek-v4.1 (issue #1897) fourth retry: baseline reconfirmed at 97.4%,
// 25 masked bytes, still the two regions above. Region 1 (range-check _hypot,
// 8 bytes): the four operand-order/negation combinations were re-measured
// (25, 25, 47, 47 bytes; 97.4, 97.4, 96.8, 96.8%) and the instruction streams
// show MSVC fixes the register pair by memory operand here, so [esi+0x30] and
// [esi+0x2e] (the order side) are always issued first into edx/ecx and only
// the sub destination follows the source. The matched siblings 0x412d40 and
// 0x412710 both load their Order object first too (it is the edi-base there),
// so the original 413470 is the odd one out: it loads [edi+0x74] (unit) first.
// Nested-if, a local range, an != 0 test and swapped arguments each lose the
// whole body (500+ byte diff, frame shrinks to 0x30), so no shape tried
// reaches it. Region 2 (state-3 waypoint, 17 bytes): a Vec3& to
// order->target->pos plus member-wise p.x/p.y/p.z assignment does reproduce
// the original's interleaved schedule (load x, push 0x36, add off.x, then load
// y and z) but accumulates into off.x's register and issues the p.x store
// early: 34 bytes, 96.5%. The matching sibling site at 0x413a1e (same sum with
// - off) uses the hoisted form with ebx for y, exactly like our build, so the
// original really scheduled the two sums differently and the source edit that
// produced it is not the one that produces this schedule. Add() as a
// per-member helper scores 97.1%. Best kept: baseline, 97.4%.
// deepseek-v4.1 (issue #2361) seventh retry: baseline reconfirmed at 97.4%,
// same two regions. Checked the matched sibling 0x4034a0, which spells this
// exact test `_hypot(unit->pos.xh - order->x, unit->pos.zh - order->z)` with
// the unit side FIRST (the negation of our line 261): that spelling here drops
// to 96.8 and emits the order (esi) movsx first in both differences, with the
// sub destination following the esi operand, so the original's unit-first load
// pair is not reachable from the operand order. <memory.h>, which the matched
// 0x412710 notes as the header that fixes its first hypot load order, was
// added in both positions (before and after <math.h>): order-first stays 97.4
// and unit-first stays 96.8, so the header set is not the lever here either.
// What still differs (both pure scheduling, 1864 bytes in both):
// - 0x413675: the range-check _hypot issues the four movsx in the order
//   unit,order / unit,order; ours (and the negated spelling) always starts
//   with the order (esi) load into edx.
// - 0x413950: the state-3 waypoint keeps off.x consumed before pos.y is
//   loaded, so it reuses edi for pos.y and ecx for pos.z and hoists push 0x36
//   between the two adds; ours loads pos.y into ebx before the adds and
//   pushes after them (member-wise p.x/p.y/p.z sums score 93.7 and shrink
//   the frame by 4 bytes, so the sum must stay a Vec3 local plus operator+).
// deepseek-v4.1 (issue #2580) ninth retry: baseline reconfirmed 98.0, up from
// 97.4 in issue #2541. An inline member-wise helper `VecAdd(a, b)` returning a
// fresh Vec3 keeps the 1864-byte frame and lands the interleaved shape closer
// (97.7), so the member-wise DAG is the right direction, but as a named local it
// still differs more than the operator+ baseline; passing that helper straight
// into the constructor drops to 93.4, plain member-wise statements to 94.3
// (frame -4) and swapping the operands to 94.4 (frame +8). Best kept: 98.0%.

#include <math.h>
#include <vector>

struct Vec3 {
    union { int x; struct { unsigned short xf; short xw; }; };
    int y;
    union { int z; struct { unsigned short zf; short zw; }; };
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    void operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r = *this; r += v; return r; }
    Vec3 operator-(const Vec3& v) const { Vec3 r = *this; r -= v; return r; }
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void FUN_0043d210(Unit* unit, int state);
};
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_00489800 { public: void FUN_00489800(int); };
class Class_0048b090 { public: void FUN_0048b090(int, int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int field_1fa;
    char pad1fe[0x21c - 0x1fe]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Mover {
    char pad0[0xdc]; int speed;
};
struct Player {
    char pad0[0x146]; unsigned char index;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x10 - 4]; Mover* mover;
    char pad14[0x6a - 0x14];
    Vec3 pos;
    char pad76[0x82 - 0x76];
    int field_82; int field_86;
    char pad8a[8]; UnitDef* def;
    Player* player;
    char pad9a[0x108 - 0x9a]; short field_108;
    char pad10a[0x110 - 0x10a]; unsigned int flags;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x22 - 0x1a]; Vec3 pos;
    short x; short z;
    char pad32[0x36 - 0x32]; int side; int misses;
    int range;
    unsigned int field_42;
    char pad46[4]; int field_4a;
};
struct Game {
    char pad0[0x1422b]; int width; int height;
    char pad14233[0x142b7 - 0x14233]; int field_142b7;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e330 {
public:
    char unknown_0[0x36];
    Class_0044e330(Order* order, Unit* unit, const Vec3& p);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, int a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0048a060(Unit*, Unit*, int);
int __stdcall FUN_0049abb0(Unit*, Unit*, int);
void __stdcall FUN_0043ad10(Unit*, Class_0043a1f0*);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Unit*>* out);
Vec3 __stdcall FUN_0040f790(const Vec3& a, const Vec3& b);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->FUN_004898b0(3);
    if (unit->field_86)
        FUN_0048aac0(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->FUN_0048b090(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->FUN_0043d210(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// FUNCTION: 0x413470
int __stdcall FUN_00413470(Unit* unit, Order* order, int flags)
{
    if (flags & 0x10008) {
        if (order->field_4a == 0 && (unit->flags & 0x300000))
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", (int)order->target, &order->pos, 0, 0, 0));
        return 5;
    }
    if (order->target == 0 && (order->field_42 & 0x200)) {
        if (order->field_4a == 0)
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", 0, &unit->pos, 0, 0, 0));
        return 5;
    }
    if (unit->field_82 == g_game->field_142b7) {
        Vec3 centre;
        centre.x = g_game->width / 2 << 16;
        centre.z = g_game->height / 2 << 16;
        short angle = FUN_0048a980(&unit->pos, &centre);
        Vec3 dest = FUN_0040f790(unit->pos, Offset(angle, 0x3200000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        order->flags |= 0xe0;
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        return 2;
    }
    short& ox = order->x;
    short& oz = order->z;
    if (order->range && (int)_hypot(unit->pos.xw - ox, unit->pos.zw - oz) >= order->range)
        return 5;
    int speed = unit->mover->speed;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->FUN_00438880("Attacking");
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        ((Class_00489800*)unit)->FUN_00489800(3);
        int dist = (int)_hypot(order->target->pos.x - unit->pos.x, order->target->pos.z - unit->pos.z);
        int angle = FUN_0048a980(&unit->pos, &order->target->pos);
        Vec3 off = Offset(FUN_004b6c30(0x4000) + angle - 0x2000, dist / 2);
        Vec3 p = unit->pos + off;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 2: {
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        FUN_0048a060(unit, order->target, 0);
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->target->pos);
        ((Class_0044e730*)obj)->FUN_0044e730(speed);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100e8;
        order->side = 0;
        order->misses = 0;
        return 1;
    }
    case 3: {
        if (!FUN_0049abb0(unit, order->target, 0))
            order->misses++;
        if (order->misses >= 2) {
            order->misses = 0;
            Vec3 off = Offset(FUN_004b6c30(0x10000), speed << 16);
            Vec3 p = order->target->pos + off;
            // The new waypoint is never given to the order (see the notes).
            Class_0044e2d0* obj = new Class_0044e2d0(order, p);
            ((Class_0044e730*)obj)->FUN_0044e730(0x80);
            order->flags |= 0x110e8;
            return 2;
        }
        int a = FUN_0048a980(&unit->pos, &order->target->pos);
        int angle;
        if (order->side) {
            angle = a + 0x2000;
            order->side = 0;
        } else {
            angle = a - 0x2000;
            order->side = 1;
        }
        Vec3 off = Offset(angle, speed * 2 / 3 << 16);
        Vec3 p = order->target->pos - off;
        Class_0044e330* obj = new Class_0044e330(order, order->target, p);
        ((Class_0044e730*)obj)->FUN_0044e730(0x10);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100e8;
        if ((unsigned int)unit->field_108 < (unit->def->field_1fa >> 2) * 3) {
            std::vector<Unit*> v;
            FUN_0040b530(unit->player->index, &unit->pos, 0xf00, &v);
            if (!v.empty()) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                int target = (int)v[FUN_004b6c30(v.size())];
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", target, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
        }
        return 2;
    }
    }
    return 7;
}
