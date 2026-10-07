// Decompiled by Claude Opus 5.5. Names are provisional.
// VTOL transport (air lift) order handler. Fails when there is no target,
// with flags 0x10048, or when the target sits below sea level. State 0
// prepares the order ("Loading"; FUN_0040f200), state 1 flies to the target,
// state 2 asks the script for the attach piece (QueryTransport), state 3
// starts BeginTransport and hovers down to the piece's height, state 4 ends
// the pickup (EndTransport when cancelled) and state 5 finishes.
// Kept before the declarations: the sea level sum's register order needs a header.
#include <math.h>

struct Vec3 {
    int x;
    union { int y; struct { unsigned short yf; short yw; }; };
    int z;
};

struct Unit;
class UnitMotion {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };
class CobScript { public: int StartScriptWithArgs(char*, void*, int, int, int, int, int, int); void StartScript(const char*, int, int); int QueryScript(char* name, int* p2, int* p3, int* p4, int* p5); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x16e]; int field_16e;
    char pad172[0x21c - 0x172]; short field_21c;
    char pad21e[0x22a - 0x21e]; unsigned char capacity;
    char pad22b[0x241 - 0x22b]; unsigned int flags;
};
struct Unit {
    UnitMotion* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x7e - 0x76]; short size;
    char pad80[0x86 - 0x80]; int field_86; int field_8a;
    char pad8e[4]; UnitDef* def;
    char pad96[4]; CobScript* script;
    void ClaimWeapons(int);
    void SetStateBits(int, int);
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

void __stdcall QueueUnitSpeech(Unit*, int, const char*);
// p3 is int here (its own file says char): the original pushes order->piece
// as a dword, which a char parameter would load as a byte.
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, int p3, char p4);
int __stdcall SendScriptCallByName(Unit* unit, char* name, char p3, int p4, int p5, int p6, int p7);
Vec3 __stdcall GetPieceOffset(Unit* unit, int piece);

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
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
int __stdcall VtolPickupOrder(Unit* unit, Order* order, int flags)
{
    Unit* target = order->target;
    if (target && !(flags & 0x10048)) {
        if (BelowSeaLevel(order)) {
            QueueUnitSpeech(unit, 7, "Transport mission failed");
            return 8;
        }
        if (unit->field_8a)
            return 8;
        switch (order->state) {
        case 0:
            if (unit->type && (unit->def->flags & 0x800)) {
                if (target->size > (short)unit->def->capacity) {
                    QueueUnitSpeech(unit, 7, "Unit is too heavy to transport");
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
            unit->script->QueryScript("QueryTransport", &order->piece, 0, 0, 0);
            order->flags = 0x100e8;
            return 1;
        case 3: {
            int height = target->def->field_16e;
            unit->script->StartScriptWithArgs("BeginTransport", 0, 1, 1, height, 0, 0, 0);
            SendScriptCallByName(unit, "BeginTransport", 1, height, 0, 0, 0);
            Vec3 offset = GetPieceOffset(unit, order->piece);
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
                unit->script->StartScript("EndTransport", 0, 0);
                return 8;
            }
            AttachUnitToPiece(target, unit, order->piece, 0);
            QueueUnitSpeech(unit, 12, 0);
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
    QueueUnitSpeech(unit, 7, "Transport mission failed");
    return 8;
}
