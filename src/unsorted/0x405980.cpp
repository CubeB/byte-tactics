// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, gate polarity checked by space-bunny-free. Names are provisional.
// Partial: 90.9% (1081 of 1040 bytes, 41 too long). Three findings for the next attempt:
// 1. THE GATE IS WRONG HERE, semantically. The original's second energy compare at 0x405b18 is
//    `fld energy; fld capE; fmul 0.2; fcompp; fnstsw; test ah,0x41; je 0x405b5a`, and 0x405b5a is
//    the FIRST INSTRUCTION OF THE BODY, so the true edge jumps INTO the body: C0 and C3 both clear
//    means capE*0.2 > energy, so the body runs when energy < capE*0.2. The metal test right after
//    (0x405b39, `fld capM; fmul; fld metal; fxch; fcompp; test ah,0x41; jne 0x405d4a`) sends
//    metal >= capM*0.2 to `return 2`. So the gate is
//        if (energy < capE*0.2 && metal < capM*0.2) { ...body... }
//    ("low on both, go and find a spot to reclaim"), not the `||` written here. Writing `&&` scores
//    89.9% and does give a fresh compare, but MSVC then loads capE first, adds an `fxch` and jumps
//    to `return 2` instead of into the body, so the energy test's block has to be arranged so its
//    true edge reaches the body.
// 2. The first reclaim block (0x405bcb, metal && metal < capM*0.2) calls Class_004388d0::FUN_004388d0(0)
//    a SECOND time at 0x405c0b, after FUN_0043acb0, which is why its tail is a separate inline copy
//    while the other three all jump to the one at 0x405d01. Adding that second call costs 6 bytes
//    and no points, so the un-merged tail is not caused by it.
// 3. B, C and D share one tail (0x405d01) with the `operator new` and its null check still per
//    branch. An if/else-if chain assigning one `Class_0043a1f0*` and calling FUN_0043acb0 once after
//    it (the guide's "several new branches sharing a tail") merges them but collapses the function
//    to 1020 bytes at 75.3%, so the chain is not the original's shape either.
// Also tried and no better than 90.9%: nesting the two gate tests, `!(a >= b)`, `a >= b && a >= b`,
//    the reversed `||`, and the same four blocks as one chain with block A kept separate.
// 4. Tried writing the gate as the negated guard `if (energy >= capE*0.2 && metal >= capM*0.2)
//    return 2;` and as the nested `if (energy >= capE*0.2) { if (metal >= capM*0.2) return 2; }`.
//    Both give the original's polarity for the first test (`test ah,0x41; je <body>`) but MSVC
//    then computes capE*0.2 first, adds an `fxch st(1)`, and emits a second inline `return 2`
//    epilogue at the gate (+54 bytes, 89.1%). Still to do: get `fld energy; fld capE; fmul 0.2;
//    fcompp` (energy first, no fxch) and fold the gate's `jne <ret2>` into the shared epilogue.
// 5. The `goto ret2;` guard form (`if (energy >= capE*0.2 && metal >= capM*0.2) goto ret2;`)
//    DOES give the original's second test verbatim (`jne <shared ret2>`) and drops the duplicate
//    epilogue, but refactoring the body into one block reordered the first test's loads anyway
//    (capE first, fxch) and it ends 1083 bytes at 89.9%, so the `||` form scores higher. Moving
//    `int range2` after the pointer locals (to get the original's late `movsx`/`shl`) keeps 1081
//    bytes but drops to 85.2%; the scheduling difference is not worth chasing.
// 6. Tried the faithful low-energy gate text `energy < capE*0.2 || ...` alone: it reproduces the
//    original's compare mask at 0x405b37 (`test ah,0x41; je <body>`) but MSVC then computes
//    capE*0.2 first, loads energy second and inserts an `fxch st(1)` (+2 bytes, 1083 total, 89.9%),
//    so the frame and every following [esp+N] move by 2. Left the `capE*0.2 > energy` form, which
//    keeps 1081 bytes and 90.9%; the remaining gap is the +41 bytes below 0x405b98 (our 4th block
//    inlines the new/ctor tail the original jumps to at 0x405d01) plus this fxch.
#include <vector>
struct Vec3 { int x, y, z; };
struct Unit;
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438930 { public: void FUN_00438930(Vec3*, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
#pragma pack(push, 1)
struct UnitDef { char pad0[0x202]; short range; };
struct Owner {
    char pad0[0x8c]; float energy;
    char pad90[8]; float metal;
    char pad9c[8]; float energyCapacity, metalCapacity;
    char padac[0x108-0xac]; unsigned char allied[0x3e]; unsigned char index;
};
struct Unit { char pad0[0x6a]; Vec3 pos; char pad76[0x92-0x76]; UnitDef* def; Owner* owner; };
struct Order { char pad0[5]; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; };
#pragma pack(pop)
#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char data[0x56];
    Class_0043a1f0(Class_00438760, int, Vec3*, int, int, int);
};
#pragma pack(pop)
class Class_00405d90 {
public:
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
    Class_00405d90(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void FUN_00405d90(Unit*);
};
void __stdcall FUN_0043a020(Unit*, Order*);
void __stdcall FUN_0047e890(Vec3*, int, const Class_00405d90&);
int __stdcall FUN_004b6c30(int);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
int __stdcall FUN_0043b400(Unit*, Unit*, int);
int __stdcall FUN_0047ea40(Vec3*, int, Vec3**, float*, Vec3**, float*);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);

// FUNCTION: 0x405980
int __stdcall FUN_00405980(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (order->target) order->pos = order->target->pos;
        FUN_0043a020(unit, order);
        return 1;
    case 1: {
        if (flags & 0xe0) return 6;
        ((Class_00438930*)order)->FUN_00438930(&order->pos, 16);
        ((Class_00439e80*)order)->FUN_00439e80(60);
        order->flags |= 0xe0;
        if (unit->owner->energy >= unit->owner->energyCapacity * 0.2) {
            std::vector<Unit*> units;
            int range = unit->def->range << 16;
            FUN_0047e890(&unit->pos, range, Class_00405d90(unit->owner, &units, unit));
            if (!units.empty()) {
                Unit* target = units[FUN_004b6c30(units.size())];
                if (unit->owner->allied[target->owner->index]) {
                    Class_00438760 kind = FUN_0043f0e0(8, unit, target, 0);
                    if (kind.index) {
                        if (FUN_0043b400(unit, target, 0)) return 6;
                        return 3;
                    }
                }
            }
        }
        if (unit->owner->energyCapacity * 0.2 > unit->owner->energy ||
            unit->owner->metal < unit->owner->metalCapacity * 0.2) {
            int range2 = unit->def->range << 16;
            Vec3 energyPos, metalPos;
            Vec3* energy = &energyPos;
            Vec3* metal = &metalPos;
            float energyAmount, metalAmount;
            if (FUN_0047ea40(&unit->pos, range2, &energy, &energyAmount, &metal, &metalAmount)) {
                if (metal && unit->owner->metal < unit->owner->metalCapacity * 0.2) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a1f0("RECLAIM", 0, metal, 0, 0, 0));
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    order->flags = 0;
                    return 3;
                }
                if (energy && unit->owner->energy < unit->owner->energyCapacity * 0.2) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a1f0("RECLAIM", 0, energy, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (metal && unit->owner->metal + metalAmount <= unit->owner->metalCapacity) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a1f0("RECLAIM", 0, metal, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
                if (energy && unit->owner->energy + energyAmount <= unit->owner->energyCapacity) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    FUN_0043acb0(unit, new Class_0043a1f0("RECLAIM", 0, energy, 0, 0, 0));
                    order->flags = 0;
                    return 3;
                }
            }
        }
        return 2;
    }
    }
    return 7;
}

