// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6, verified by GPT-6.1-Sol, finished by space-bunny-free, edited by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (issue #4007) retry: 87.2%, 1604/1504 bytes
// reconfirmed; no further variants within this issue timebox.
// deepseek-v4.1-flash (issue #3633) retry: 87.2% (1604/1504 bytes) reconfirmed,
// 23 hunks; the +100 byte overflow is still the whole story (item 3 below).
// Not attempted further under this issue's 10 minute timebox; best kept.

// #2635 retry by OpenCode / GPT-6.1-sol: best remains 87.2% (1604/1504 bytes), no MATCH.
// A reordered `energyCapacity * 0.2 <= energy` compare scores 86.8%. A by-value
// Reclaim helper with Vec3* first merges the four tails but scores 65.1%; keep the
// four inline bodies. Current file restored to the previous 87.2% best.
// #1704 retry by Codex / GPT-6.1-Sol: checkall reconfirmed 87.2% (1610/1504 bytes), no MATCH.
// #1897 by deepseek-v4.1-flash: 1604/1504 bytes, still 87.2%, 23 hunks.
// #1897 by space-bunny-free: 87.2% again, 1604/1504. No improvement, but the size
// overshoot was finally pinned to ONE construct (item 3). Everything below is the
// current state; the first two items are the older attempts, kept because they are
// still the two open problems.
//
// PARTIAL. The 100 byte overshoot is the whole story; most hunks are only jmp targets
// shifted by it, so fix the size and the rest follows.
//
// What still differs:
// 1. Land()'s inline expansion keeps a dead "mov eax,1; test eax,eax; je" of the helper's
//    constant success result, and the pads vector destructor is emitted right after the
//    empty() test instead of on the shared path at 0x415474 (12 bytes). See item 6.
// 2. The four VTOL_RECLAIM branches. The original cross-jumps ALL FOUR onto one shared
//    ctor tail at 0x415773/0x415774. Here only branches 3 and 4 share a tail; branches 1
//    and 2 each carry a private full copy of it, which is the entire 100 byte overshoot.
//    (An earlier note in this file had 1 and 2 the other way round; it is the reverse.)
//    Branch 2's float compare is separately unmatched: `energy < energyCapacity * 0.2`
//    keeps the original's `test ah,0x41 / jne` but loads energy first, while the reversed
//    `energyCapacity * 0.2 > energy` reproduces the original's load schedule (fld cap,
//    fmul, fld energy) but emits `test ah,1 / je` and no `fxch st(1)`. Both score 87.2%.
//
// space-bunny-free findings (build/scratch/0x4152f0/v0..v5.cpp, all scored free with --sym):
// 3. THE 100 BYTE OVERSHOOT IS ONE CONSTRUCT. Factor the four VTOL_RECLAIM bodies into a
//    by-VALUE inline helper, `static inline int Reclaim(Unit*, Order*, Vec3* pos)`, and the
//    size collapses from 1604 to 1514 bytes (original 1504): MSVC then emits ONE shared
//    tail for all four branches, exactly as the original does. But the score FALLS to
//    65.1%, and the reason is a single allocation casualty, not the merge. With the
//    pointer arriving as a helper parameter its live range starts before the two calls,
//    so it takes EBP, and EBP is the original's ZERO CONSTANT. Everything downstream then
//    differs: no `xor ebp,ebp`, `push 0` instead of `push ebp`, `test edx,edx` instead of
//    `cmp edx,ebp`, `sub eax,0` instead of `sub eax,ebp`, and `mov ebp,[esi+6]` for
//    `order->flags = 0`. So the trade is: helper = right size, wrong EBP; four textual
//    bodies = right EBP, ~100 bytes duplicated. Winning needs the pointer back in a
//    scratch register WHILE the helper keeps the merge. That is the whole remaining task.
// 4. Taking the pointer BY REFERENCE instead (`Vec3** pp`, called as
//    `Reclaim(unit,order,&metal)`) does put the pointer back in EAX/ECX and restores EBP
//    as the zero constant, so the emission is then byte-identical to the four-textual-bodies
//    version, but MSVC stops merging again: 1593 bytes, 85.5%. Adding `int zero = 0;`
//    inside the helper and using it for every 0 changes nothing at all (65.1%, 1514 bytes):
//    MSVC 5 folds that local away completely, so it cannot be used to win the EBP contest.
// 5. `Vec3* target = 0;` with one shared body after the four-condition if/else-if chain
//    does NOT get duplicated: 1405 bytes, 58.3%. The four arms are single assignments,
//    too cheap for MSVC 5's tail duplication, so the original really does have four
//    textual bodies and the merge has to come from somewhere else.
// 6. Not the cause of item 1: the dead `mov eax,1; test eax,eax; je` is a pure consequence
//    of the `if (Land(...)) return 0;` at the call site. Inlining the landing block as
//    `if (!pads.empty()) { ...; order->flags = 0; return 0; }` (the shape 0x4103e0.cpp
//    uses, with `if ((unsigned int)unit->health < (maxHealth>>2)*3)` as the outer test)
//    does place the pads destructor correctly, but frees one stack dword (`sub esp,0x40`
//    instead of 0x44) and swaps the ESI/EDI roles, giving 56.8%. Two effects, one cause.
// 7. headers.py: 128 header sets, every one 87.2%, so the header choice is not a lever
//    here.
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
    char pad90[8]; float metal;
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
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

static inline int Land(Unit* unit, Order* order)
{
    if ((unsigned int)unit->health >= (unit->def->maxHealth >> 2) * 3)
        return 0;
    Class_00410830 pads;
    FUN_0040b530(unit->owner->index, &unit->pos, 0xf00, &pads);
    if (pads.empty())
        return 0;
    ((Class_004388d0*)order)->FUN_004388d0(0);
    Unit* pad = pads[FUN_004b6c30(pads.size())];
    FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
    order->flags = 0;
    return 1;
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
    if (flags & 0x48) {
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 0;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800) && (unit->def->flags2 & 0x200)) {
            if (order->target)
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
        ((Class_00439e80*)order)->FUN_00439e80(0x2d);
        order->flags |= 0xe0;
        if (Land(unit, order))
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
            if (unit->owner->metal < unit->owner->metalCapacity * 0.2 && metal) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                order->flags = 0;
                return 3;
            }
            if (unit->owner->energy < unit->owner->energyCapacity * 0.2 && energy) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                order->flags = 0;
                return 3;
            }
            if (metal && Total(unit->owner->metal, metalAmount) <= unit->owner->metalCapacity) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                order->flags = 0;
                return 3;
            }
            if (energy && Total(unit->owner->energy, energyAmount) <= unit->owner->energyCapacity) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                order->flags = 0;
                return 3;
            }
            return 2;
        }
        return 2;
    }
    }
    return 7;
}
