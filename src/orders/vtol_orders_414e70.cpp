// Decompiled by Claude Opus 5.5. Names are provisional.
#include <math.h>

struct Vec3 {
    int x, y, z;
};

struct Box {
    Vec3 lo;
    Vec3 hi;
};

struct Point16 {
    short x;
    short z;
};

// A 16.16 fixed-point position seen as its fraction and whole halves.
struct FixedVec3 {
    unsigned short xf;
    short x;
    unsigned short yf;
    short y;
    unsigned short zf;
    short z;
};

struct Unit;

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
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004899b0 { public: int FUN_004899b0(Unit*); };

#pragma pack(push, 1)
struct UnitType {
    char unknown_0[0x15e];
    Box bounds;                        // +0x15e
    char unknown_176[0x1fa - 0x176];
    unsigned int maxHealth;            // +0x1fa
    unsigned short workerTime;         // +0x1fe
    char unknown_200[0x21c - 0x200];
    short field_21c;                   // +0x21c
    char unknown_21e[0x241 - 0x21e];
    unsigned int flags;                // +0x241
};

struct Unit {
    Class_0043d210* active;            // +0x0
    char unknown_4[0x6a - 0x4];
    union {
        Vec3 pos;                      // +0x6a
        FixedVec3 fixedPos;
    };
    char unknown_76[0x86 - 0x76];
    int field_86;                      // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType* type;                    // +0x92
    char unknown_96[0x108 - 0x96];
    short health;                      // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int unknown_2 : 30;
        } bits;
    };
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0xe - 0xa];
    Unit* source;                      // +0xe
    char unknown_12[0x16 - 0x12];
    Unit* target;                      // +0x16
    char unknown_1a[0x22 - 0x1a];
    Vec3 pos;                          // +0x22
    Point16 start;                     // +0x2e
    char unknown_32[0x3e - 0x32];
    int range;                         // +0x3e
};

class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

void __stdcall FUN_0047f780(Unit* unit, int kind, const char* text);
int __stdcall FUN_0041bd10(Unit* builder, Unit* unit, float amount);
void __stdcall FUN_0043e400(Unit* unit, Vec3* out);
void __stdcall FUN_004720d0(Vec3* from, Box* to, int count);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->FUN_004898b0(3);
    if (unit->field_86)
        FUN_0048aac0(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->FUN_0048b090(1, 1);
    if ((unit->active->field_2e & 3) == 1) {
        unit->active->FUN_0043d210(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->type->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Order handler "Repairing" (the hovering variant of 0x405300): the order
// position follows the target, state 1 moves over it, state 2 spends worker
// time on it until its health is full.
// The order position is copied through a pointer local: MSVC then treats the
// store as possibly aliasing order->target and re-reads it in state 0, and
// keeps &order->pos in ebx for state 1. A plain `order->pos = ...` keeps the
// target in ebx instead.
// FUNCTION: 0x414e70
int __stdcall FUN_00414e70(Unit* unit, Order* order, int flags)
{
    if (!order->target) {
        FUN_0047f780(unit, 7, "Repairs unsuccessful.");
        return 8;
    }
    if (order->range && (int)_hypot(unit->fixedPos.x - order->start.x, unit->fixedPos.z - order->start.z) >= order->range)
        return 5;
    if (order->target->bits.mode != 1) {
        FUN_0047f780(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    Vec3* dst = &order->pos;
    *dst = order->target->pos;
    switch (order->state) {
    case 0:
        if (unit->active && (unit->type->flags & 0x800)) {
            if (!((Class_004899b0*)unit)->FUN_004899b0(order->target)) {
                FUN_0047f780(unit, 7, "Repair mission failed");
                return 8;
            }
            ((Class_00438880*)order)->FUN_00438880("Repairing");
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->type->field_21c);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe8;
        return 1;
    }
    case 2:
        if (flags & 0x40)
            return 8;
        if (order->target->bits.mode != 1)
            return 8;
        if (order->target->flags & 0xc) {
            ((Class_00439e80*)order)->FUN_00439e80(15);
            return 0;
        }
        if ((unsigned int)order->target->health >= order->target->type->maxHealth)
            return 1;
        {
            FUN_0041bd10(unit, order->target, (float)(unit->type->workerTime / 30));
            Vec3 nano;
            FUN_0043e400(order->source, &nano);
            Box box;
            box.hi = order->target->pos;
            box.lo = order->target->pos;
            box.lo.x += order->target->type->bounds.lo.x;
            box.lo.z += order->target->type->bounds.lo.z;
            box.hi.x += order->target->type->bounds.hi.x;
            box.hi.z += order->target->type->bounds.hi.z;
            box.hi.y += order->target->type->bounds.hi.y;
            FUN_004720d0(&nano, &box, 6);
        }
        ((Class_00439e80*)order)->FUN_00439e80(1);
        order->flags |= 8;
        return 2;
    case 3:
        FUN_0047f780(unit, 10, "Unit repaired");
        return 5;
    }
    return 7;
}
