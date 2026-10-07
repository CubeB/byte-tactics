// Decompiled by Opus. Names are provisional.
// Order handler: asks GetOrderType for the next order kind (returned as a
// Class_00438760 by value), passes it on by value (the 0x406240 call site
// builds the same argument in place) and returns state 2.

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit_00403190;

#pragma pack(push, 1)
class Class_00438b90 {
public:
    char unknown_0[0x16];
    Unit_00403190* target;             // +0x16
    char unknown_1a[0x36 - 0x1a];
    int state;                         // +0x36

    void FUN_00438b90(Class_00438760 kind);
};
#pragma pack(pop)

Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit_00403190* unit,
                                       Unit_00403190* target, int flags);

// FUNCTION: 0x403190
int __stdcall AttackSpecialOrder(Unit_00403190* unit, Class_00438b90* order, int unused)
{
    order->FUN_00438b90(GetOrderType(3, unit, order->target, 0));
    order->state = 2;
    return 2;
}
