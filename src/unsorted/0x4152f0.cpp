// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6, verified by GPT-6.1-Sol, finished by space-bunny-free, edited by deepseek-v4.1-flash, edited by Claude Opus 5.5. Names are provisional.
// Claude Opus 5.5 (#5520): still 90.5%, code unchanged; what was measured, so
// the next attempt can skip it (all numbers from tools/c2prio.py):
//  - The do-while fold (Land returning 1/0, `do {} while (0);` after the
//    health test, FUN_0040f200 `inline`) only works when no function is
//    compiled before FUN_004152f0 in the file. A trivial function in front,
//    FUN_0040f200 compiled out of line, or the real preceding 0x415250 all
//    bring the test back (1514 bytes), and so do 0 to 1200 extra
//    declarations. So if the original folded this way, 0x4152f0 opened its
//    translation unit and FUN_0040f200 came from a header as an inline
//    function. Every Land shape tried (bool, `!Land`, a -1 sentinel, a result
//    local, `int r = Land(); if (r) return 0;`) folds either with that
//    setup or not at all.
//  - In the folded file order is 48 and case 0's unit web 76, so unit takes
//    esi. A Land with a result local (`int landed = 0; ... landed = 1; ...
//    return landed;`) gives order 81 and the original's registers, but keeps
//    the test (1509 bytes, 90.0%). The flag's extra weight is in the set-up
//    block (K 4 -> 7: the flag, the zero and the 0x56 `new` size, which is a
//    register candidate only when such a variable exists) and in each
//    reclaim arm.
//  - In the folded file, these raise order: the reclaim arms each written
//    with their own `FUN_0043acb0(unit, obj); order->flags = 0; return 3;`
//    (72, but the arms no longer cross-jump: 1590 bytes); a per-arm helper
//    `return Reclaim(unit, order, pos);` doing FUN_004388d0(0), the `new`,
//    FUN_0043acb0 and `flags = 0` (84 to 98: order esi and unit edi as in
//    the original). With that helper and the Owner accessors replaced by
//    plain field reads (fewer depth-1 sites after Land, so Land's /Ob2 share
//    stays at 92 or more and pads.size() stays inline), everything up to the
//    reclaim chain matches (81.0%, 1501 bytes). The chain is then wrong: arms
//    3 and 4 merge in the IL, the position takes ebp from the zero, and the
//    four arms no longer share one constructor tail. Helpers that end each arm
//    at `obj = ...` (which the cross-jump needs) reach only 74, and adding a
//    set-up helper (new Class_0044e2d0, FUN_0044e6c0, FUN_004388d0) on top
//    gets 80 but changes the /Ob2 budget and the set-up code (40%).
//  - No effect on the 48/76 gap: a bool or int local for the health test, an
//    IsDamaged helper, case 0 rewritten (early break, a target local, nested
//    ifs, case 1 first), a void Assign(unit, order, obj) helper (IL under 41,
//    its parameters are propagated away), SetNext/Wait/Done wrappers, an
//    `unsigned int&` to order->flags, headers.
// Codex / GPT-6 retry for #5491 (2026-10-04): permute.py found and the
// checker verified a 90.5% / 1519-byte best from 632 candidates in 17.4
// minutes (`move_stmt+cast`), up from 90.3% / 1520 bytes. The remaining
// landed-path flag is still unresolved; details below.
// VTOL patrol order handler. State 0 starts patrolling ("Patrolling";
// FUN_0040f200 is defined here because /Ob2 inlined it). State 1 sets the next
// waypoint, then lands on a free pad when damaged (VTOL_LANDING), helps build
// or guards a unit it can see (VTOL_HELPBUILD), or reclaims metal or energy
// (VTOL_RECLAIM).
//
// PARTIAL, 90.3% (Claude Opus 5.5, #4169; was 87.2%). The size is 1520
// against 1504 and every block but one lines up with the original.
//
// What fixed the rest:
// - The four VTOL_RECLAIM branches are one if/else-if chain that assigns
//   `obj = new Class_0043a1f0(...)` in each arm, followed by one shared
//   `FUN_0043acb0(unit, obj); order->flags = 0; return 3;`. MSVC then
//   cross-jumps all four constructor tails into the last arm exactly as the
//   original does (1604 -> 1518 bytes). Four separate `if (...) {...; return
//   3;}` blocks only merge their last two arms, whatever the conditions.
// - The second arm's `fld cap; fmul; fld energy; fxch st(1)` and the fourth
//   arm's load order come from reading the amounts through inline accessors
//   (Owner::GetEnergy, GetMetal). Reading the field directly loads energy
//   first. The units test above must keep the plain field read.
// - Land keeps the pads vector one inline level down: with Land's share of
//   the /Ob2 budget (tools/c2prio.py --inline) the Class_00410830
//   constructor, empty() and the destructor inline but their nested vector
//   constructor, size() and ~vector() stay out of line (0x40c510, 0x40c560,
//   0x40c530), as in the original, while the units vector below is all
//   inline.
//
// What still differs: the landed flag. The original has no flag at all: the
// landed path calls ~vector and returns 0, the empty path calls ~vector and
// falls into the energy test. Here Land reports through `int& landed`, which
// leaves `xor ebx, ebx`, `mov ebx, 1` and a `cmp ebx, ebp; je` behind.
// Measured on the way:
// - Land returning a flag (`if (Land(unit, order)) return 0;`) leaves
//   `mov eax, K; test eax, eax` (or `xor eax, eax; cmp eax, ebp`) on the path
//   whose return is textually last, in every shape tried (early returns,
//   nested ifs, if/else, a for(;;), bool or char results, the health test
//   outside Land, Land as an Order member). The test only folds with an empty
//   `do {} while (0);` after the health test in Land AND FUN_0040f200 written
//   `inline` (so it is not compiled on its own first); with either missing it
//   stays. A toy file folds with the do-while alone. That folded file has the
//   original's blocks and branches (1498 bytes with the accessors), but
//   unit/order come out in esi/edi instead of edi/esi and the zero constant
//   in ebx instead of ebp, which moves the units block's loads: c2prio gives
//   order 48 against the case 0 unit web's 76, while the `landed` flag lifts
//   order to 81 (it adds candidates to the waypoint block, K 4 -> 7). Nothing
//   else found lifts order past 76 without the flag: a case 0 helper,
//   member-function Land, Land's parameter order, case order, an IsDamaged
//   helper and the permuter (15 minutes from the folded file, stuck at 76.2%
//   with the right size) all leave it at 48.
// - Writing the landing block straight into case 1 cannot work: at depth 1
//   the share is about 140, so the pads' vector calls all inline.
#include <vector>

struct Vec3 { int x, y, z; };

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class Class_00410830 : public std::vector<Unit*> {};

class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void FUN_0043d210(Unit* unit, int state);
};
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_0048b090 { public: void FUN_0048b090(int, int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_004899b0 { public: int FUN_004899b0(Unit*); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int maxHealth;
    char pad1fe[0x202 - 0x1fe]; short range;
    char pad204[0x21c - 0x204]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
    unsigned int flags2;
};
struct Owner {
    char pad0[0x8c]; float energy;
    float GetEnergy() { return energy; }
    char pad90[8]; float metal;
    float GetMetal() { return metal; }
    char pad9c[8]; float energyCapacity, metalCapacity;
    char padac[0x146 - 0xac]; unsigned char index;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Owner* owner;
    char pad9a[0x104 - 0x9a]; float progress;
    short health;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x22 - 0x1a]; Vec3 pos;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

class Class_004158d0 {
public:
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
    Class_004158d0(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void FUN_004158d0(Unit*);
};

int __stdcall FUN_004b6c30(int);
void __stdcall FUN_0043a020(Unit*, Order*);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Unit*>* out);
void __stdcall FUN_0047e890(Vec3*, int, const Class_004158d0&);
int __stdcall FUN_0043b400(Unit*, Unit*, int);
union Fixed { int v; struct { unsigned short frac; short whole; } p; };
int __stdcall FUN_0047ea40(Vec3*, Fixed, Vec3**, float*, Vec3**, float*);

void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->FUN_004898b0(3);
    if (unit->field_86)
        FUN_0048aac0(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->FUN_0048b090(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->FUN_0043d210(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0((Order*)order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

static inline void Land(Unit* unit, Order* order, int& landed)
{
    if ((unsigned int)unit->health < (unit->def->maxHealth >> 2) * 3) {
        Class_00410830 pads;
        FUN_0040b530(unit->owner->index, &unit->pos, 0xf00, &pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Unit* pad = pads[FUN_004b6c30(pads.size())];
            FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
            order->flags = 0;
            landed = 1;
            return;
        }
    }
}

static inline float Total(float base, float amount)
{
    float value = base;
    value += amount;
    return value;
}


// FUNCTION: 0x4152f0
int __stdcall FUN_004152f0(Unit* unit, Order* order, int flags)
{
    if ((flags & 0x48) != 0) {
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 0;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800) && (unit->def->flags2 & 0x200)) {
            if (order->target != 0)
                order->pos = order->target->pos;
            FUN_0043a020(unit, order);
            ((Class_00438880*)order)->FUN_00438880("Patrolling");
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        if (flags & 0xe0)
            return 6;
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        int landed = 0;
        ((Class_00439e80*)order)->FUN_00439e80(0x2d);
        order->flags |= 0xe0;
        Land(unit, order, landed);
        if (landed)
            return 0;
        if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
            std::vector<Unit*> units;
            int range = unit->def->range << 16;
            FUN_0047e890(&unit->pos, range, Class_004158d0(unit->owner, &units, unit));
            if (!units.empty()) {
                Unit* target = units[FUN_004b6c30(units.size())];
                if (((Class_004899b0*)unit)->FUN_004899b0(target) && target->progress == 0.0f) {
                        if (FUN_0043b400(unit, target, 0))
                            return 6;
                        return 3;
                    }
                if (((Class_004899b0*)unit)->FUN_004899b0(target) && target->progress != 0.0f) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a1f0("VTOL_HELPBUILD", target, 0, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
            }
        }
        Vec3 energyPos, metalPos;
        Vec3* energy = &energyPos;
        Vec3* metal = &metalPos;
        float energyAmount, metalAmount;
        Fixed range;
        range.v = 0xf00000;
        if (FUN_0047ea40(&unit->pos, range, &energy, &energyAmount, &metal, &metalAmount)) {
            Class_0043a1f0* obj;
            if (unit->owner->GetMetal() < unit->owner->metalCapacity * 0.2 && metal) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                obj = new Class_0043a1f0("VTOL_RECLAIM", 0, (Vec3*)metal, 0, 0, 0);
            } else if (unit->owner->GetEnergy() < unit->owner->energyCapacity * 0.2 && energy) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                obj = new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0);
            } else if (metal && Total(unit->owner->GetMetal(), metalAmount) <= unit->owner->metalCapacity) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                obj = new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0);
                obj = (Class_0043a1f0*)obj;
            } else if (energy && Total(unit->owner->GetEnergy(), energyAmount) <= unit->owner->energyCapacity) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                obj = new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0);
            } else
                return 2;
            FUN_0043acb0(unit, obj);
            order->flags = 0;
            return 3;
        }
        return 2;
    }
    }
    return 7;
}
