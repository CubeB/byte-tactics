// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Class_00489800 {
public:
    void FUN_00489800(int param);
};

class Class_004898b0 {
public:
    void FUN_004898b0(int param);
};

class Class_00438880 {
public:
    void FUN_00438880(char* text);
};

struct Unit_00402160;

void __stdcall FUN_0048a060(char* unit, char* param_2, int param_3);

#pragma pack(push, 1)
struct Order_00402160 {
    char unknown_0[5];
    unsigned char state;               // +0x5
    int field_6;                       // +0x6
    char unknown_a[0x16 - 0xa];
    int field_16;                      // +0x16
};
#pragma pack(pop)

// FUNCTION: 0x402160
int __stdcall FUN_00402160(Unit_00402160* unit, Order_00402160* order, int flags)
{
    if (order->field_16 == 0 || (flags & 0x10808) != 0)
        return 5;
    switch (order->state) {
    case 0:
        ((Class_00438880*)order)->FUN_00438880(0);
        return 1;
    case 1:
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        FUN_0048a060((char*)unit, (char*)order->field_16, 0);
        order->field_6 = 0x11808;
        return 1;
    case 2:
        ((Class_00489800*)unit)->FUN_00489800(3);
        return 9;
    default:
        return 7;
    }
}
