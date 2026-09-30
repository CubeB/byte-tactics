// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6. Names are provisional.
// "Attacking" order handler of aircraft (VTOL). Interrupts hand over to a
// "VTOL_SEEKATTACK" order; the order follows its target unit and gives up
// outside its range. State 0 prepares the order (FUN_0040f200 is defined here
// because /Ob2 inlined it), states 1 and 2 make attack runs past the target,
// state 4 turns around after a pause, state 5 aims at the target again and
// state 6 flies on and, when the unit is below three quarters of its health,
// sends it to a random repair pad ("VTOL_LANDING").
//
// Partial: 96.6%, same 1980-byte size. Remaining differences include the
// random-angle LEA operand order, turn-delay register allocation and multiply
// order, and switch relocations. The delay subtraction is equivalent: VC5
// folds the sign into a -30.0f constant, then subtracts the converted result.
#include <list>
#include <windows.h>
#include <math.h>
#include <vector>

struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& other) const {
        Vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        return r;
    }
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
    char unknown_0[0x22];
    short field_22;                    // +0x22
    char unknown_24[0x2e - 0x24];
    unsigned char field_2e;            // +0x2e
    void FUN_0043d210(Unit* unit, int state);
};
class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc
    virtual ~Class_004895c0();
    void FUN_00489690(Unit* o);
};
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };
class Class_00489800 { public: void FUN_00489800(int); };
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_0048b090 { public: void FUN_0048b090(int, int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int maxHealth;
    char pad1fe[0x216 - 0x1fe]; unsigned short field_216;
    char pad218[0x21c - 0x218]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Struct_Unit96 {
    char pad0[0x146]; unsigned char field_146;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x66 - 4]; short angle;
    char pad68[2];
    union {
        Vec3 pos;                      // +0x6a
        FixedVec3 fixedPos;
    };
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Struct_Unit96* field_96;
    char pad9a[0x108 - 0x9a]; short health;
    char pad10a[0x110 - 0x10a]; unsigned int flags;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x12 - 0xa]; Class_004895c0 target;
    Vec3 pos;
    Point start;                       // +0x2e
    char pad32[0x3e - 0x32]; int range;
    unsigned int field_42;
    char pad46[0x4a - 0x46]; int field_4a;
};
struct Struct_Game391e9 {
    char pad0[0xd3c]; int field_d3c;
};
struct Game {
    char pad0[0x391e9]; Struct_Game391e9* field_391e9;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e190 {
public:
    char unknown_0[0x36];
    Class_0044e190(Order* order, Unit* unit);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, Vec3* pos, int c, int d, int e);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_0048a0a0(Unit*, Vec3*, int);
void __stdcall FUN_0048a0f0(Unit*, int);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
void __stdcall FUN_0043ad10(Unit*, Class_0043a1f0*);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Unit*>* out);

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

// FUNCTION: 0x411f50
int __stdcall FUN_00411f50(Unit* unit, Order* order, unsigned int flags)
{
    if (flags & 0x1000a) {
        if (!order->field_4a && (unit->flags & 0x300000))
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", order->target.owner, &order->pos, 0, 0, 0));
        return 5;
    }
    Unit* target = order->target.owner;
    if (!target && (order->field_42 & 0x200)) {
        if (!order->field_4a)
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", 0, &unit->pos, 0, 0, 0));
        return 5;
    }
    if (target)
        order->pos = target->pos;
    if (order->range && (int)_hypot(unit->fixedPos.x - order->start.x, unit->fixedPos.z - order->start.y) >= order->range)
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
    case 1:
        ((Class_00489800*)unit)->FUN_00489800(3);
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        if ((int)_hypot(order->pos.x - unit->pos.x, order->pos.z - unit->pos.z) < 0x1e00000) {
            int angle = FUN_0048a980(&unit->pos, &order->pos);
            Vec3 dest = unit->pos + Offset(angle, 0x8c00000);
            Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
            ((Class_0044e730*)obj)->FUN_0044e730(0x3c0);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags |= 0xe2;
            return 1;
        }
        return 1;
    case 2: {
        int dist = (int)_hypot(order->pos.x - unit->pos.x, order->pos.z - unit->pos.z);
        int angle = FUN_0048a980(&unit->pos, &order->pos);
        int radius = dist / 2;
        angle += FUN_004b6c30(0x4000) - 0x2000;
        Vec3 dest = unit->pos + Offset(angle, radius);
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x1e0);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 3:
        return 1;
    case 4: {
        if (flags & 0xe0)
            return 1;
        UnitDef* def = unit->def;
        int size = def->field_21c;
        int rate = g_game->field_391e9->field_d3c;
        if (!rate)
            break;
        int time = (int)((float)sqrt(size * 2.0 / rate) * 30.0f * unit->type->field_22) + 1 + def->field_216;
        Class_0044e2d0* obj;
        if (order->target.owner)
            obj = (Class_0044e2d0*)new Class_0044e190(order, order->target.owner);
        else
            obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e730*)obj)->FUN_0044e730(time);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        ((Class_00439e80*)order)->FUN_00439e80(1);
        order->flags |= 0x100e8;
        return 2;
    }
    case 5: {
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        FUN_0048a0a0(unit, &order->pos, 0);
        int angle = FUN_0048a980(&unit->pos, &order->pos);
        Vec3 dest = unit->pos + Offset(angle, (unit->def->field_216 + 0x3c0) << 16);
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x3c0);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe2;
        return 1;
    }
    case 6: {
        FUN_0048a0f0(unit, 0);
        Vec3 dest = unit->pos + Offset(unit->angle, 0x5a00000);
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe2;
        if (unit->health < unit->def->maxHealth / 4 * 3) {
            std::vector<Unit*> pads;
            FUN_0040b530(unit->field_96->field_146, &unit->pos, 0xf00, &pads);
            if (!pads.empty()) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                Unit* pad = pads[FUN_004b6c30(pads.size())];
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
            return 0;
        }
        order->state = 3;
        return 2;
    }
    }
    return 7;
}
