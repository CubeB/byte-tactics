// Decompiled by Opus. Names are provisional.
// Order handler: state 0 (only when the unit's +0x86 is clear) resets the
// order and calls FUN_00438930 with its position; state 1 posts message 6
// when bit 0x20 of the third argument is set.

struct Unit_004031d0;

#pragma pack(push, 1)
struct UnitBody_004031d0 {
    char unknown_0[0x86];
    int field_86;                      // +0x86
};

class Order {
public:
    char unknown_0[5];
    unsigned char state;               // +0x5
    int field_6;                       // +0x6
    char unknown_a[0x22 - 0xa];
    int pos[5];                        // +0x22
    int field_36;                      // +0x36
};
#pragma pack(pop)

class Class_00438880 {
public:
    void FUN_00438880(char* text);
};

class Class_00438930 {
public:
    void FUN_00438930(int* pos, int param);
};

void __stdcall FUN_0047f780(UnitBody_004031d0* unit, int kind, char* text);

// FUNCTION: 0x4031d0
int __stdcall MoveGroundOrder(UnitBody_004031d0* unit, Order* order, int flags)
{
    switch (order->state) {
    case 0:
        if (unit->field_86 != 0)
            return 7;
        ((Class_00438880*)order)->FUN_00438880(0);
        ((Class_00438930*)order)->FUN_00438930(order->pos, order->field_36 + 4);
        order->field_6 = 0xe0;
        return 1;
    case 1:
        if (flags & 0x20) {
            FUN_0047f780(unit, 6, 0);
            return 5;
        }
        return 9;
    default:
        return 7;
    }
}
