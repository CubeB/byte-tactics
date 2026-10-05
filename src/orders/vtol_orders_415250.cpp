// Decompiled by Opus. Names are provisional.
// Repair order step: report an aborted or finished repair, or keep repairing
// while the unit's health is below its type's maximum.

#pragma pack(push, 1)
struct UnitType_00415250 {
    char unknown_0[0x1fa];
    unsigned int max_health;           // +0x1fa
};

struct Unit {
    char unknown_0[0x92];
    UnitType_00415250* type;           // +0x92
    char unknown_96[0x72];
    short health;                      // +0x108
};

class Order {
public:
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flag_0 : 1;           // +0x6
    unsigned int flag_1 : 1;
    unsigned int flag_2 : 1;
    unsigned int started : 1;          // +0x6 bit 3
    unsigned int flag_rest : 28;
    char unknown_a[0xc];
    int target;                        // +0x16
};
#pragma pack(pop)

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

void __stdcall FUN_0047f780(Unit* unit, int kind, const char* text);

// FUNCTION: 0x415250
int __stdcall VtolGetRepairedOrder(Unit* unit, Order* order, int unused)
{
    if (order->target == 0) {
        FUN_0047f780(unit, 7, "Repair aborted.");
        return 8;
    }
    switch (order->state) {
    case 0:
        if (unit->health >= unit->type->max_health)
            return 1;
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        order->started = 1;
        return 2;
    case 1:
        FUN_0047f780(unit, 10, "Unit repaired");
        return 5;
    default:
        return 7;
    }
}
