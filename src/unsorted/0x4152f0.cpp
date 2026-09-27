// Decompiled by Claude Opus 5.5. Names are provisional.
// VTOL patrol order handler ("Patrolling"). State 0 prepares the order
// (FUN_0040f200 is defined here because /Ob2 inlined it). State 1 flies on:
// when damaged it lands on a random free pad (VTOL_LANDING); with enough
// energy it helps build or repair an allied unit nearby (visitor
// Class_004158d0, VTOL_HELPBUILD); otherwise it reclaims features for metal
// or energy (VTOL_RECLAIM).
//
// Partial: 74.9%. What still differs:
// - The landing block. The original calls vector<Unit*>'s constructor
//   (0x40c510), the size() inside empty() (0x40c560) and the destructor
//   (0x40c530, twice) out of line, but inlines the direct size() and
//   operator[], and inlines all of the second vector<Unit*> (the visitor's
//   list) further down. Written inline here (plain vector, Class_00410830
//   wrapper, wrapper with forwarding methods) MSVC inlines everything.
//   Only the Land helper below plus the Class_00410830 wrapper (a
//   vector<Unit*> subclass, as in 0x4103e0 and 0x410850, whose out-of-line
//   constructor is 0x410830) reproduces the calls,
//   but its return flag leaves a test (mov eax, 1; test eax, eax; je) that the
//   original lacks: MSVC 5 never threads the helper's last return. Probing
//   with extra FUN_0040f200 copies shows /Ob2 handles depth-1 call sites in
//   source order and deeper ones last-first, so the original probably ran out
//   of inline budget right after the visitor part's depth-2 calls. Many small
//   inline helpers (even 600) use no budget; big ones (FUN_0040f200 copies,
//   or a rejected inline Reclaim helper) do. Giving Class_00438760 its real
//   constructor body inline (as a header might) is rejected by /Ob2 but still
//   uses budget: the inline-landing version then calls the landing vector's
//   _Destroy out of line, the right direction but not far enough. What used
//   the rest of the budget in the original was not found.
// - The reclaim tail. Here branches 1 and 4 share their constructor tail but
//   2 and 3 keep their own. An if/else-if chain assigning one
//   `Class_0043a0c0* node` per branch, then one FUN_0043acb0(unit, node),
//   merges all four exactly as the original (1 and 4 at push eax, 2 and 3
//   after it), but then the new pointer takes esi and order/unit move to
//   edi/ebx; it only gets esi=order, edi=unit, ebx=new when unit and order
//   have a few more uses (for example the health test repeated in the caller),
//   which again points at the landing code being inline in the original.
// - Branches 3 and 4 load the amount before the owner's field (fld [esp+x];
//   fadd [ecx+0x98]); the original loads the field first. Operand order,
//   casts and headers do not change it. Branch 2's energy test lacks the
//   original's fld cap; fmul; fld energy; fxch order.
// What helped: FUN_0047ea40's range is a 4-byte union passed by value (the
// original does mov eax, 0xf00000; push eax), and declaring the energy
// pointer before the metal one gives the right stack slots.
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
class Class_0043a0c0 {
public:
    char unknown_0[0x56];
    Class_0043a0c0(Class_00438760 type, Unit* a, Vec3* b, int c, int d, int e);
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
void __stdcall FUN_0043acb0(Unit*, Class_0043a0c0*);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Unit*>* out);
void __stdcall FUN_0047e890(Vec3*, int, const Class_004158d0&);
int __stdcall FUN_0043b400(Unit*, Unit*, int);
union Fixed { int v; struct { unsigned short frac; short whole; } p; };
int __stdcall FUN_0047ea40(Vec3*, Fixed, Vec3**, float*, Vec3**, float*);

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
    FUN_0043acb0(unit, new Class_0043a0c0("VTOL_LANDING", pad, 0, 0, 0, 0));
    order->flags = 0;
    return 1;
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
                    FUN_0043acb0(unit, new Class_0043a0c0("VTOL_HELPBUILD", target, 0, 0, 0, 0));
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
                FUN_0043acb0(unit, new Class_0043a0c0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                order->flags = 0;
            } else if (unit->owner->energy < unit->owner->energyCapacity * 0.2 && energy) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a0c0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                order->flags = 0;
            } else if (metal && unit->owner->metal + metalAmount <= unit->owner->metalCapacity) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a0c0("VTOL_RECLAIM", 0, metal, 0, 0, 0));
                order->flags = 0;
            } else if (energy && unit->owner->energy + energyAmount <= unit->owner->energyCapacity) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit, new Class_0043a0c0("VTOL_RECLAIM", 0, energy, 0, 0, 0));
                order->flags = 0;
            } else {
                return 2;
            }
            return 3;
        }
        return 2;
    }
    }
    return 7;
}
