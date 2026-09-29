// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// VTOL attack order handler ("Attacking"). With flags 0x1000a, or with no
// target and order flag 0x200, it queues VTOL_SEEKATTACK instead; when out of
// the order's range it gives up. State 0 prepares the order (FUN_0040f200 is
// defined here because /Ob2 inlined it), state 1 flies to a random point
// halfway to the target, state 2 attacks, state 3 pulls away from the target,
// state 4 lands on a free pad when damaged (VTOL_LANDING) or circles.
//
// Partial: 93.9%. The one structural difference left is the landing block's
// destructor: the original calls vector<Unit*>::_Destroy (0x406c00) out of
// line before `operator delete`, while ours inlines it (its body is empty,
// `ret 8`). The scope-end destructor on the empty path (0x412c4c) is not
// called there because the optimizer knows `_First == _Last`; that site
// already matches. The element type is `Unit*` (see 0x406c00.cpp and
// docs/consolidation.md), but spelling it that way moves registers in state
// 4's health test (93.9% to 93.0%), so `Elem_00406c10` is kept here.
// 0x410e70 (same landing code) got the call by putting the vector one inline
// level down in a TryLand helper AND having case 0 after case 2 in the
// source; 0x412710's switch bodies are emitted in source order 0..5
// (verified: moving case 4 first drops the score to 63.7%), so that trick
// does not transfer. TryLand here (one level down, any element type) scores
// 92.0% and still inlines _Destroy.
//
// A scoring artefact was rejected: an explicit `v.~vector();` before
// `return 0;` inflates the size to the original's 1572 by adding zeroing
// stores and a second no-op delete that the original does not have.
//
// Second difference: state 4's Offset call reloads the spilled distance into
// edx right after the first call (`mov edx, [esp+0x3c]` before `add esp, 8`);
// ours reloads it into eax after `mov ebx, eax`.
// What fixed most of it: `Vec3 p = base + off` with a member operator+ built
// on operator+= (states 1 and 3), a separate sum then copy in state 4, the
// literal 0 as the second VTOL_SEEKATTACK target, and <memory.h> (found with
// tools/headers.py; it fixes the first hypot's load order).
#include <math.h>
#include <memory.h>
#include <vector>

struct Vec3 {
    int x, y, z;
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r = *this; r += v; return r; }
};

struct Elem_00406c10 {
    int unknown_0;
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
struct Mover {
    char pad0[0xdc]; int speed;
};
struct Player {
    char pad0[0x146]; unsigned char index;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x10 - 4]; Mover* mover;
    char pad14[0x66 - 0x14]; short heading;
    char pad68[2];
    union {
        Vec3 pos;
        struct { unsigned short xf; short x; int y; unsigned short zf; short z; } p;
    };
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
    char pad32[0x3e - 0x32]; int range;
    unsigned int field_42;
    char pad46[4]; int field_4a;
};
struct Game {
    char pad0[0x142b7]; int field_142b7;
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

extern Game* g_game;

int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0048a060(Unit*, Unit*, int);
void __stdcall FUN_0048a0a0(Unit*, Vec3*, int);
void __stdcall FUN_0043ad10(Unit*, Class_0043a1f0*);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Elem_00406c10>* out);

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

// FUNCTION: 0x412710
int __stdcall FUN_00412710(Unit* unit, Order* order, int flags)
{
    int speed = unit->mover->speed;
    if (flags & 0x1000a) {
        if (order->field_4a == 0 && (unit->flags & 0x300000))
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", (int)order->target, &order->pos, 0, 0, 0));
        return 5;
    }
    if (order->target == 0 && (order->field_42 & 0x200)) {
        if (order->field_4a == 0)
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", 0, &unit->pos, 0, 0, 0));
        return 5;
    }
    if (order->target)
        order->pos = order->target->pos;
    if (unit->field_82 == g_game->field_142b7) {
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        order->state = 2;
    }
    if (order->range && (int)_hypot(unit->p.x - order->x, unit->p.z - order->z) >= order->range)
        return 5;
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
        int dist = (int)_hypot(order->pos.x - unit->pos.x, order->pos.z - unit->pos.z);
        int angle = FUN_0048a980(&unit->pos, &order->pos);
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
        if (order->target)
            FUN_0048a060(unit, order->target, 0);
        else
            FUN_0048a0a0(unit, &order->pos, 0);
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e730*)obj)->FUN_0044e730(speed);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 3: {
        short angle = FUN_004b715a(unit->pos.x - order->pos.x, unit->pos.z - order->pos.z);
        Vec3 off = Offset(angle, speed * 0x30000);
        Vec3 p = order->pos + off;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->FUN_0044e730(FUN_004b6c30(0x80) + 0x80);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100ea;
        return 1;
    }
    case 4: {
        if ((unsigned int)unit->field_108 < (unit->def->field_1fa >> 2) * 3) {
            std::vector<Elem_00406c10> v;
            FUN_0040b530(unit->player->index, &unit->pos, 0xf00, &v);
            if (!v.empty()) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                int target = v[FUN_004b6c30(v.size())].unknown_0;
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", target, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
        }
        short angle = FUN_004b6c30(2) ? unit->heading + 0x4000 : unit->heading - 0x4000;
        Vec3 off = Offset(angle, speed << 16);
        Vec3 sum;
        sum.x = unit->pos.x + off.x;
        sum.y = unit->pos.y + off.y;
        sum.z = unit->pos.z + off.z;
        Vec3 p = sum;
        Class_0044e2d0* obj = new Class_0044e2d0(order, p);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100ea;
        return 1;
    }
    case 5:
        order->state = 2;
        return 2;
    }
    return 7;
}
