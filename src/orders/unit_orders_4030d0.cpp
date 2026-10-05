// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit_004030d0 {
    char unknown_0[0x110];
    unsigned int unknown_bits : 18;      // +0x110
    unsigned int mode2 : 2;              // +0x110, bits 18-19
    unsigned int unknown_bits2 : 12;
};

struct Order_004030d0 {
    char unknown_0[0x36];
    int mode;                            // +0x36
};
#pragma pack(pop)

// FUNCTION: 0x4030d0
int __stdcall FUN_004030d0(Unit_004030d0* unit, Order_004030d0* order, int unused)
{
    unit->mode2 = order->mode;
    return 5;
}
