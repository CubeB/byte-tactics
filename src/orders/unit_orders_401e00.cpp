// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler: state 0 waits a random time if the unit's type allows it;
// state 1 picks the nearest non-allied unit with the order's id (the squared
// distance minus a random share of itself) and queues an order on it.
// It stays in a file of its own: in the joined unit_orders.cpp the address mode
// of the allied lookup below flips ([eax+ecx+0x108] against [ecx+eax+0x108]),
// and no pad count or view permutation of that file restored it.

class MissionType {
public:
    unsigned char index;
    MissionType(const char* name);
};

struct Unit;

#include "order.h"

#pragma pack(push, 1)
struct UnitDef_00401e00 {
    char unknown_0[0x245];
    unsigned char flags;               // +0x245
};

struct Player_00401e00 {
    char unknown_0[0x108];
    unsigned char allied[0x3e];        // +0x108
    unsigned char index;               // +0x146
};

struct Unit {
    char unknown_0[0x6a];
    int x;                             // +0x6a
    int y;                             // +0x6e
    int z;                             // +0x72
    char unknown_76[0x92 - 0x76];
    UnitDef_00401e00* def;             // +0x92
    Player_00401e00* owner;            // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short id;                 // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Order_00401e00 {
    char unknown_0[5];
    unsigned char state;               // +5
    char unknown_6[0x36 - 6];
    int id;                            // +0x36
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int range);
MissionType __stdcall GetOrderType(unsigned char mode, Unit* unit,
                                       Unit* target, int flags);
void __stdcall AppendOrder(Unit* owner, Order* node);

// FUNCTION: 0x401e00
int __stdcall AttackUTypeOrder(Unit* unit, Order_00401e00* order, int unused)
{
    unsigned int s = 0;
    s = order->state;
    switch (s) {
    case 0:
        if (!(unit->def->flags & 0x10))
            return 7;
        ((Order*)order)->SetDeadlineTicks(RandomInt(0x5a) + 1);
        return 1;
    case 1: {
        Unit* best = 0;
        int bestDist = 0x7fffffff;
        for (Unit* u = g_game->units + 1; u <= g_game->unitsEnd; u++) {
            if (u->id == order->id && unit->owner->allied[u->owner->index] == 0) {
                int dz = u->z - unit->z;
                int dx = u->x - unit->x;
                int d = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
                d -= RandomInt(d / 2);
                if (d <= bestDist) {
                    best = u;
                    bestDist = d;
                }
            }
        }
        if (best) {
            MissionType kind = GetOrderType(3, unit, best, 0);
            AppendOrder(unit, new Order(kind, best, 0, 0, 0, 0));
            return 0;
        }
        return 5;
    }
    default:
        return 7;
    }
}
