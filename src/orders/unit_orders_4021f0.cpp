// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler: follows the order's target unit, waits a random number of
// ticks near it and, when interrupted, picks a random unit around the
// remembered position (FUN_0040ad80 fills a vector) as the new target.

#include <vector>

struct Unit;

class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    virtual ~Class_004895c0();
    void FUN_00489690(Unit* o);
};

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

class Class_00489800 {
public:
    void FUN_00489800(int param);
};

class Class_004898b0 {
public:
    void FUN_004898b0(int param);
};

struct Vec_004021f0 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6a];
    Vec_004021f0 pos;                  // +0x6a
    char unknown_76[0xff - 0x76];
    unsigned char player;              // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};

struct Order_004021f0 {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0x12 - 0xa];
    Class_004895c0 target;             // +0x12
    Vec_004021f0 pos;                  // +0x22
    char unknown_2e[0x36 - 0x2e];
    int wait;                          // +0x36
    int waitLimit;                     // +0x3a
};
#pragma pack(pop)

int __stdcall FUN_004b6c30(int range);
Unit* __stdcall FUN_0048a190(Unit* unit, int index);
void __stdcall FUN_0048a060(Unit* unit, Unit* target, int weapon);
int __stdcall FUN_0049abb0(Unit* unit, Unit* target, int param_3);
void __stdcall FUN_0040ad80(int player, Vec_004021f0* pos, int radius, int flags,
                            std::vector<Unit*>* out);

// FUNCTION: 0x4021f0
int __stdcall FUN_004021f0(Unit* unit, Order_004021f0* order, int flags)
{
    if (flags & 0x10008) {
        order->state = 3;
        return 2;
    }
    switch (order->state) {
    case 0:
        ((Class_00489800*)unit)->FUN_00489800(3);
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 1;
    case 1: {
        order->target.FUN_00489690(FUN_0048a190(unit, 0));
        Unit* t = order->target.owner;
        if (t != 0 && (t->flags & 0x10000000)) {
            order->pos = t->pos;
            ((Class_004898b0*)unit)->FUN_004898b0(0);
            FUN_0048a060(unit, order->target.owner, 0);
            order->wait = 0;
            order->waitLimit = FUN_004b6c30(3) + 3;
            return 1;
        }
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 2;
    }
    case 2:
        if (flags & 0x4000)
            order->wait = 0;
        else
            order->wait++;
        if (order->wait <= order->waitLimit && FUN_0049abb0(unit, order->target.owner, 0)) {
            order->flags |= 0x7008;
            return 2;
        }
        if (FUN_004b6c30(100) < 0x50) {
            order->wait = 0;
            return 1;
        }
        break;
    case 3: {
        std::vector<Unit*> units;
        FUN_0040ad80(unit->player, &order->pos, 0x280, 0, &units);
        if (!units.empty()) {
            order->target.FUN_00489690(units[FUN_004b6c30(units.size())]);
            FUN_0048a060(unit, order->target.owner, 0);
            order->state = 1;
            return 2;
        }
        break;
    }
    default:
        return 7;
    }
    return 0;
}
