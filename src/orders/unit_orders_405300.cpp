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

union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

class Class_00438ad0 {
public:
    void FUN_00438ad0(Point16 cell, Point16 size);
};

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

class Class_00438880 {
public:
    void FUN_00438880(char* text);
};

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x15e];
    Box bounds;                        // +0x15e
    char unknown_176[0x1fa - 0x176];
    unsigned int maxHealth;            // +0x1fa
    unsigned short workerTime;         // +0x1fe
    char unknown_200[0x212 - 0x200];
    unsigned short buildDistance;      // +0x212
    char unknown_214[0x241 - 0x214];
    unsigned char flags;               // +0x241
};

struct Unit {
    int active;                        // +0x0
    char unknown_4[0x66 - 0x4];
    unsigned short angle;              // +0x66
    char unknown_68[0x6a - 0x68];
    union {
        Vec3 pos;                      // +0x6a
        FixedVec3 fixedPos;
    };
    Point16 cell;                      // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point16 footprint;                 // +0x7e
    char unknown_82[0x92 - 0x82];
    UnitDef* type;                     // +0x92
    char unknown_96[0xb0 - 0x96];
    int repairTime;                    // +0xb0
    char unknown_b4[0x104 - 0xb4];
    float buildLeft;                   // +0x104
    short health;                      // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int unknown_2 : 30;
        } bits;
    };
    void ClaimWeapons(int param);
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0xe - 0xa];
    Unit* source;                      // +0xe
    char unknown_12[0x16 - 0x12];
    Unit* target;                      // +0x16
    char unknown_1a[0x2e - 0x1a];
    Point16 start;                     // +0x2e
    char unknown_32[0x3e - 0x32];
    int range;                         // +0x3e
};

struct Game {
    char unknown_0[0x38a47];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
int __stdcall RandomInt(int range);
unsigned short __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
void __stdcall StartBuildingScript(Unit* unit, Order* order, short turn);
int __stdcall FUN_00438700(Unit* unit, Order* order, int flags);
void __stdcall StopBuildingScript(Unit* unit, Order* order);
int __stdcall FUN_0041bd10(Unit* builder, Unit* unit, float amount);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitNanoParticles(Vec3* from, Box* to, int count);

// Order handler "Repair" of a builder: walks up to the target unit, then
// spends worker time on it until its health is full.
// FUNCTION: 0x405300
int __stdcall RepairUnitOrder(Unit* unit, Order* order, int flags)
{
    if (!order->target) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    if (order->range && (int)_hypot(unit->fixedPos.x - order->start.x, unit->fixedPos.z - order->start.z) >= order->range)
        return 5;
    if (order->target->bits.mode != 1) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    switch (order->state) {
    case 0:
        if (unit->active && (unit->type->flags & 0x40) && order->target->buildLeft == 0.0f) {
            ((Class_00438880*)order)->FUN_00438880("Repairing");
            return 1;
        }
        break;
    case 1: {
        if (flags & 0x40)
            return 8;
        Fixed distance;
        distance.value = (int)_hypot(unit->pos.x - order->target->pos.x, unit->pos.z - order->target->pos.z);
        int gap = distance.whole - (int)(_hypot(unit->footprint.x, unit->footprint.z) * 8.0);
        gap += (int)(_hypot(order->target->footprint.x, order->target->footprint.z) * -8.0);
        unsigned int range = 0;
        range = unit->type->buildDistance;
        if (gap > (int)range) {
            ((Class_00438ad0*)order)->FUN_00438ad0(order->target->cell, order->target->footprint);
            ((Class_00439e80*)order)->FUN_00439e80(RandomInt(30) + 30);
            order->flags |= 0xe8;
            return 2;
        }
        ((Unit*)unit)->ClaimWeapons(3);
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &order->target->pos) - unit->angle);
        return 1;
    }
    case 2:
        return FUN_00438700(unit, order, 8);
    case 3:
        if ((unsigned int)order->target->health >= order->target->type->maxHealth)
            return 1;
        if (order->target->flags & 0xc) {
            StopBuildingScript(unit, order);
            ((Class_00439e80*)order)->FUN_00439e80(15);
            return 0;
        }
        unit->repairTime = g_game->ticks + 150;
        if (FUN_0041bd10(unit, order->target, (float)(unit->type->workerTime / 30))) {
            Vec3 nano;
            GetNanoPiecePosition(order->source, &nano);
            Box box;
            box.hi = order->target->pos;
            box.lo = order->target->pos;
            box.lo.x += order->target->type->bounds.lo.x;
            box.lo.z += order->target->type->bounds.lo.z;
            box.hi.x += order->target->type->bounds.hi.x;
            box.hi.z += order->target->type->bounds.hi.z;
            box.hi.y += order->target->type->bounds.hi.y;
            EmitNanoParticles(&nano, &box, 6);
        }
        ((Class_00439e80*)order)->FUN_00439e80(1);
        order->flags |= 8;
        return 2;
    case 4:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    }
    return 7;
}
