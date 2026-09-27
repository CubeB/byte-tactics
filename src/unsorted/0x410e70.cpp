// Decompiled by Claude Opus 5.5. Names are provisional.
// VTOL patrol order handler ("Patrolling"). State 0 prepares the order
// (FUN_0040f200 is defined here because /Ob2 inlined it), state 1 clears the
// order's 0xe0 bits, state 2 flies to a point 0x140 units away along the
// heading to the order's position, lands on a free pad when damaged
// (VTOL_LANDING, as in 0x412710), or takes the next queued order.
//
// Partial: 80.6%. The original calls vector::_Destroy (0x406c00) out of line
// in both of the vector's destructors, which is MSVC 5's /Ob2 inline budget
// running out. Found with scratch probes: the inliner does all first-level
// call sites before the calls inside them, and handles later source first,
// so the budget runs out for _Destroy only when the vector sits one level
// down (the TryLand helper) and case 0 (with the inlined FUN_0040f200) comes
// after case 2 in the source. That gives exactly the original's calls.
// What still differs: the landed path. TryLand returns 1 and the caller
// returns 0, so ours sets eax = 1 after the destructor and joins the
// not-landed path's test (`mov eax, 1; xor edi, edi; jmp`); the original
// returns 0 straight after the destructor. Returning bool, 0/1 swapped, -1,
// a flag local, or putting the tail (FUN_0043b700 onward) or the whole case
// in the helper either keeps that join or changes which calls are inlined.
// With the vector directly in case 2 (no helper) the return folds but both
// _Destroy calls are inlined (72.4%), whatever the case order, and wrapping
// the vector in a struct with its own destructor does not change that. In
// that plain version a scratch probe needed about 16 extra trivial inline
// calls after the landing code before both _Destroy calls went out of line.
// Putting the whole tail in the helper (so it returns 0/3/2 directly), case
// 0 or the fly-out part in helpers, or dropping the Offset/FUN_0040f790
// helpers all move the budget the wrong way (a whole ~vector out of line,
// or FUN_0040f200 or FUN_0040f790 not inlined).
// No header set changes the result (tools/headers.py).
#include <vector>

struct Vec3 {
    int x, y, z;
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
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int field_1fa;
    char pad1fe[0x21c - 0x1fe]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Player {
    char pad0[0x146]; unsigned char index;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Player* player;
    char pad9a[0x108 - 0x9a]; short field_108;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x22 - 0xa]; Vec3 pos;
    char pad2e[0x4e - 0x2e]; unsigned int field_4e;
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
    Class_0043a1f0(Class_00438760 type, int a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
void __stdcall FUN_0043a020(Unit*, Order*);
Unit* __stdcall FUN_0043b700(Unit*);
int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Unit*>* out);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// 0x40f790, matched in 0x40f790.cpp; /Ob2 inlines it into state 2.
Vec3 __stdcall FUN_0040f790(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
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

// Shared with 0x412710 state 4: land on a free pad when damaged.
static inline int TryLand(Unit* unit, Order* order)
{
    if ((unsigned int)unit->field_108 < (unit->def->field_1fa >> 2) * 3) {
        std::vector<Unit*> v;
        FUN_0040b530(unit->player->index, &unit->pos, 0xf00, &v);
        if (!v.empty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Unit* target = v[FUN_004b6c30(v.size())];
            FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", (int)target, 0, 0, 0, 0));
            order->flags = 0;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 0x410e70
int __stdcall FUN_00410e70(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 2: {
        if (flags & 0xe0)
            return 6;
        short angle = FUN_0048a980(&unit->pos, &order->pos);
        Vec3 dest = FUN_0040f790(order->pos, Offset(angle, 0x1400000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x150);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= 0xe0;
        if (TryLand(unit, order))
            return 0;
        Unit* next = FUN_0043b700(unit);
        if (next && FUN_0043b1f0(unit, next, 0)) {
            order->flags = 0;
            return 3;
        }
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 2;
    }
    case 1:
        order->field_4e &= ~0xe0;
        return 1;
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            FUN_0043a020(unit, order);
            ((Class_00438880*)order)->FUN_00438880("Patrolling");
            FUN_0040f200(unit, order, 0);
            ((Class_00489800*)unit)->FUN_00489800(3);
            return 1;
        }
        break;
    }
    return 7;
}
