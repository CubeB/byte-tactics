// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol. Names are provisional.
// #1704 retry by Codex / GPT-6.1-sol: checkall reconfirmed 97.4%, no MATCH.
// Eight worker checks found no improvement over the existing source.
// GPT-6 retry: range/difference helpers, target-position addition helpers,
// declaration layout, constructor bodies and coordinate field names did not
// improve 97.4%. The first hypot and missed-attack waypoint loads still differ.
// VTOL attack order handler for a unit target. With flags 0x10008, or with
// no target and order flag 0x200, it queues VTOL_SEEKATTACK; on the map-edge
// player it heads for the map centre. State 0 prepares the order
// ("Attacking"; FUN_0040f200 is defined here because /Ob2 inlined it), state
// 1 flies to a random point halfway to the target, state 2 attacks, state 3
// circles the target, alternating sides, and lands on a free pad when
// damaged (VTOL_LANDING).
//
// Partial: 97.4%. What still differs:
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
// Suspected original bug: that same branch builds a Class_0044e2d0 waypoint
// and sets its speed, but never passes it to the order (no FUN_004388d0
// call, unlike every other branch), so the object leaks.
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
    if (order->range && (int)_hypot(order->x - unit->pos.xw, order->z - unit->pos.zw) >= order->range)
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
