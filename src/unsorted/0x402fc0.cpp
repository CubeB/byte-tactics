// Decompiled by Opus. Names are provisional.
// Order handler: state 0 calls FUN_004898b0(3) on the unit, state 1 waits
// ten ticks.

class Class_004898b0 {
public:
    void FUN_004898b0(int param);
};

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

#pragma pack(push, 1)
struct Unit_00402fc0 {
    char unknown_0[0x86];
    int field_86;                      // +0x86
};

struct Order_00402fc0 {
    char unknown_0[5];
    unsigned char state;               // +0x5
};
#pragma pack(pop)

// FUNCTION: 0x402fc0
int __stdcall FUN_00402fc0(Unit_00402fc0* unit, Order_00402fc0* order, int unused)
{
    if (unit->field_86 == 0) {
        return 5;
    }
    switch (order->state) {
    case 0:
        ((Class_004898b0*)unit)->FUN_004898b0(3);
        return 1;
    case 1:
        ((Class_00439e80*)order)->FUN_00439e80(10);
        return 2;
    default:
        return 7;
    }
}
