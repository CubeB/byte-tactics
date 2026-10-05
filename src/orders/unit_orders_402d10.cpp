// Decompiled by Opus. Names are provisional.

class Class_0048b090 {
public:
    void SetStateBits(int param_1, int param_2);
};

class Class_004898b0 {
public:
    void ClaimWeapons(int param);
};

class Class_004388d0 {
public:
    void FUN_004388d0(int param);
};

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

struct Unit_00402d10;

#pragma pack(push, 1)
struct Order {
    char unknown_0[0x36];
    int ticks;                           // +0x36
};
#pragma pack(pop)

void __stdcall ClearWeaponTarget(Unit_00402d10* unit, int weapon);

// Order handler: waits for the order's time (at most 1800 ticks).
// A char loop counter gives the separate countdown register (ebx = 3).
// FUNCTION: 0x402d10
int __stdcall ParalyzeOrder(Unit_00402d10* unit, Order* order, int unused)
{
    if (order->ticks == 0) {
        ((Class_0048b090*)unit)->SetStateBits(0x10, 0);
        return 5;
    }
    if (order->ticks > 0x708)
        order->ticks = 0x708;
    ((Class_004898b0*)unit)->ClaimWeapons(3);
    for (char i = 0; i < 3; i++)
        ClearWeaponTarget(unit, i);
    ((Class_004388d0*)order)->FUN_004388d0(0);
    ((Class_00439e80*)order)->FUN_00439e80(order->ticks);
    order->ticks = 0;
    ((Class_0048b090*)unit)->SetStateBits(0x10, 1);
    return 1;
}
