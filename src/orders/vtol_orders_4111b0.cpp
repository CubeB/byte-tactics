// Decompiled by Claude Opus 5.5. Names are provisional.
// VTOL transport (air lift) order handler. Fails when there is no target,
// with flags 0x10048, or when the target sits below sea level. State 0
// prepares the order ("Loading"; FUN_0040f200 is defined here because /Ob2
// inlined it), state 1 flies to the target, state 2 asks the script for the
// attach piece (QueryTransport), state 3 starts BeginTransport and hovers
// down to the piece's height, state 4 ends the pickup (EndTransport when
// cancelled) and state 5 finishes.
// Match notes: the sea level test needs an inline helper that re-reads
// order->target (see BelowSeaLevel), FUN_0048aac0 takes an int p3, and the
// sea level sum's register order needs a header before the declarations
// (<math.h>, which the file's neighbours use for _hypot; tools/headers.py
// lists the others that work, but <vector> alone does not).
#include <math.h>

struct Vec3 {
    int x;
    union { int y; struct { unsigned short yf; short yw; }; };
    int z;
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
class Class_0044e730 { public: void FUN_0044e730(short); };
class Class_004b0940 { public: void FUN_004b0940(const char*, int, int); };
class Class_004b0bc0 { public: int FUN_004b0bc0(char* name, int* p2, int* p3, int* p4, int* p5); };
class Class_004b0a70 { public: int FUN_004b0a70(char*, void*, int, int, int, int, int, int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x16e]; int field_16e;
    char pad172[0x21c - 0x172]; short field_21c;
    char pad21e[0x22a - 0x21e]; unsigned char capacity;
    char pad22b[0x241 - 0x22b]; unsigned int flags;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x7e - 0x76]; short size;
    char pad80[0x86 - 0x80]; int field_86; int field_8a;
    char pad8e[4]; UnitDef* def;
    char pad96[4]; Class_004b0a70* script;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x16 - 0xa]; Unit* target;
    char pad1a[0x36 - 0x1a]; int piece;
};
struct Game {
    char pad0[0x1427f]; unsigned char seaLevel;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e190 {
public:
    char unknown_0[0x36];
    Class_0044e190(Order* order, Unit* target);
};
class Class_0044e250 {
public:
    char unknown_0[0x36];
    Class_0044e250(Order* order, Unit* target, short value);
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0047f780(Unit*, int, const char*);
// p3 is int here (its own file says char): the original pushes order->piece
// as a dword, which a char parameter would load as a byte.
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, int p3, char p4);
int __stdcall SendScriptCallByName(Unit* unit, char* name, char p3, int p4, int p5, int p6, int p7);
Vec3 __stdcall FUN_0043def0(Unit* unit, int piece);

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

// The target's top (its def's height above its position) is at or below the
// sea. An inline helper that re-reads order->target: written inline, MSVC
// keeps target->def in ecx for state 3 and every register choice changes.
static inline int BelowSeaLevel(Order* order)
{
    Unit* target = order->target;
    return target->def->field_16e + target->pos.y <= g_game->seaLevel << 16;
}

// FUNCTION: 0x4111b0
int __stdcall FUN_004111b0(Unit* unit, Order* order, int flags)
{
    Unit* target = order->target;
    if (target && !(flags & 0x10048)) {
        if (BelowSeaLevel(order)) {
            FUN_0047f780(unit, 7, "Transport mission failed");
            return 8;
        }
        if (unit->field_8a)
            return 8;
        switch (order->state) {
        case 0:
            if (unit->type && (unit->def->flags & 0x800)) {
                if (target->size > (short)unit->def->capacity) {
                    FUN_0047f780(unit, 7, "Unit is too heavy to transport");
                    return 8;
                }
                ((Class_00438880*)order)->FUN_00438880("Loading");
                FUN_0040f200(unit, order, 0);
                return 1;
            }
            break;
        case 1: {
            Class_0044e190* obj = new Class_0044e190(order, order->target);
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
            ((Class_0044e730*)obj)->FUN_0044e730(0x30);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags = 0x100e8;
            return 1;
        }
        case 2:
            ((Class_00438880*)order)->FUN_00438880("Preparing for transport");
            order->piece = -1;
            ((Class_004b0bc0*)unit->script)->FUN_004b0bc0("QueryTransport", &order->piece, 0, 0, 0);
            order->flags = 0x100e8;
            return 1;
        case 3: {
            int height = target->def->field_16e;
            unit->script->FUN_004b0a70("BeginTransport", 0, 1, 1, height, 0, 0, 0);
            SendScriptCallByName(unit, "BeginTransport", 1, height, 0, 0, 0);
            Vec3 offset = FUN_0043def0(unit, order->piece);
            Class_0044e250* obj = new Class_0044e250(order, order->target, -1);
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(-offset.yw);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags = 0x100ea;
            return 1;
        }
        case 4: {
            // Suspected original bug: the waypoint built below is never
            // handed to the order (no FUN_004388d0 call), so it leaks.
            if (flags & 0x42) {
                ((Class_004b0940*)unit->script)->FUN_004b0940("EndTransport", 0, 0);
                return 8;
            }
            FUN_0048aac0(target, unit, order->piece, 0);
            FUN_0047f780(unit, 12, 0);
            Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
            order->flags |= 0xe0;
            return 1;
        }
        case 5:
            return 5;
        }
        return 7;
    }
    FUN_0047f780(unit, 7, "Transport mission failed");
    return 8;
}
