// Decompiled by Claude Opus 5.5. Names are provisional.
// "Landing" order handler of air units landing on a pad unit (the order's
// target). State 0 prepares the order (FUN_0040f200 is defined here because
// /Ob2 inlined it) and picks a random angle, state 1 looks for a free pad
// (FindLandingPad is defined here because /Ob2 inlined it) and circles the pad
// unit until one is free, states 2 to 5 approach the pad and land, and state
// 6 hands the unit (or its cargo) to the pad and starts a "SELFREPAIR" order
// when the pad repairs and the unit is damaged.
// Match notes: a header set is needed (<windows.h> and <math.h> here, several
// others work): without one the health compare in state 6 swaps ecx and edx.
// The pad index is passed to Class_0044e250's constructor and AttachUnitToPiece as
// a full dword load, so those parameters are declared int here.
#include <windows.h>
#include <math.h>

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

struct Unit;
class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc
    virtual ~Class_004895c0();
    void SetUnit(Unit* o);
};
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };
class Class_004898b0 { public: void ClaimWeapons(int); };
class Class_0048b090 { public: void SetStateBits(int, int); };
class Class_004b0940 { public: void StartScript(const char*, int, int); };
class Class_004b0bc0 {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x170]; short field_170;
    char pad172[0x1fa - 0x172]; unsigned int maxHealth;
    char pad1fe[0x21c - 0x1fe]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Class_Unit10 {
    char pad0[0xdc]; int field_dc;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x10 - 4]; Class_Unit10* field_10;
    char pad14[0x6a - 0x14]; Vec3 pos;
    char pad76[8]; Point footprint;
    int field_82; int field_86;
    Unit* cargo;
    char pad8e[4]; UnitDef* def;
    char pad96[4]; Class_004b0940* script;
    char pad9e[0x104 - 0x9e]; float buildLeft;
    short health;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x12 - 0xa]; Class_004895c0 target;
    Vec3 pos;
    char pad2e[8]; int angle;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e250 {
public:
    char unknown_0[0x36];
    Class_0044e250(Order* order, Unit* unit, int value);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, void* pos, int c, int d, int e);
};
#pragma pack(pop)

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall FUN_0047e570(Unit* unit, int id);
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, int p3, int p4);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);

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
    ((Class_004898b0*)unit)->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// 0x411840, matched in 0x411840.cpp; inlined into the cases below.
int __stdcall FindLandingPad(Unit* unit, int pad)
{
    if (pad != -1 && FUN_0047e570(unit, pad)) {
        return pad;
    }
    int pads[4];
    pads[0] = -1;
    pads[1] = -1;
    pads[2] = -1;
    pads[3] = -1;
    ((Class_004b0bc0*)unit->script)->QueryScript("QueryLandingPad", &pads[0], &pads[1], &pads[2], &pads[3]);
    for (int i = 0; i < 4; i++) {
        if (pads[i] != -1 && FUN_0047e570(unit, pads[i])) {
            return pads[i];
        }
    }
    return -1;
}

// FUNCTION: 0x4118e0
int __stdcall VtolLandingOrder(Unit* unit, Order* order, int flags)
{
    int radius = unit->field_10->field_dc;
    Unit* host = order->target.owner;
    if (!host) {
        QueueUnitSpeech(unit, 7, "Landing aborted");
        return 8;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->FUN_00438880("Landing");
            FUN_0040f200(unit, order, 0);
            order->angle = RandomInt(0x10000);
            return 1;
        }
        break;
    case 1: {
        if (FindLandingPad(order->target.owner, FindLandingPad(host, -1)) != -1) {
            order->state = 2;
            return 2;
        }
        Vec3 dest = order->target.owner->pos + Offset(order->angle, radius << 16);
        order->angle += 0x4000;
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe8;
        order->state = 1;
        return 2;
    }
    case 2: {
        Class_0044e250* obj = new Class_0044e250(order, order->target.owner, -1);
        ((Class_0044e730*)obj)->FUN_0044e730(0xa0);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe8;
        return 1;
    }
    case 3: {
        int pad = FindLandingPad(host, -1);
        order->angle = pad;
        if (pad == -1) {
            QueueUnitSpeech(unit, 7, "Landing failed");
            return 0;
        }
        Class_0044e250* obj = new Class_0044e250(order, order->target.owner, order->angle);
        ((Class_0044e730*)obj)->FUN_0044e730(0x30);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe8;
        return 1;
    }
    case 4:
        return 1;
    case 5: {
        if (flags & 0x20)
            return 1;
        int pad = FindLandingPad(host, order->angle);
        order->angle = pad;
        if (pad == -1) {
            QueueUnitSpeech(unit, 7, "Landing aborted: all pads are occupied");
            return 0;
        }
        Class_0044e250* obj = new Class_0044e250(order, order->target.owner, order->angle);
        if (unit->cargo)
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->cargo->def->field_170);
        else
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(0);
        unit->script->StartScript("EndTransport", 0, 1);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        ((Class_00439e80*)order)->FUN_00439e80(0xf);
        order->state = 5;
        order->flags |= 0xe8;
        return 2;
    }
    case 6: {
        int dropped = 0;
        if (flags & 0x40)
            return 8;
        if (!FUN_0047e570(host, order->angle)) {
            QueueUnitSpeech(unit, 7, "Landing aborted: no pads available");
            return 0;
        }
        if (unit->cargo) {
            unit->script->StartScript("EndTransport", 0, 0);
            AttachUnitToPiece(unit->cargo, order->target.owner, order->angle, 0);
            dropped = 1;
        } else {
            AttachUnitToPiece(unit, order->target.owner, order->angle, 0);
        }
        if (!dropped && unit->health < unit->def->maxHealth
            && (order->target.owner->def->flags & 0x200)
            && (order->target.owner->def->flags & 0x40)
            && order->target.owner->buildLeft == 0.0f) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            AppendOrder(unit, new Class_0043a1f0("SELFREPAIR", order->target.owner, 0, 0, 0, 0));
        }
        return 5;
    }
    }
    return 7;
}
