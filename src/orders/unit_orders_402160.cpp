// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Unit {
public:
    void ReleaseWeapons(int param);
    void ClaimWeapons(int param);
};

class Class_00438880 {
public:
    void FUN_00438880(char* text);
};

struct Unit_00402160;

void __stdcall SetWeaponTargetUnit(char* unit, char* param_2, int param_3);

#pragma pack(push, 1)
struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    int field_6;                       // +0x6
    char unknown_a[0x16 - 0xa];
    int field_16;                      // +0x16
};
#pragma pack(pop)

// FUNCTION: 0x402160
int __stdcall AttackNoMoveOrder(Unit_00402160* unit, Order* order, int flags)
{
    if (order->field_16 == 0 || (flags & 0x10808) != 0)
        return 5;
    switch (order->state) {
    case 0:
        ((Class_00438880*)order)->FUN_00438880(0);
        return 1;
    case 1:
        ((Unit*)unit)->ClaimWeapons(0);
        SetWeaponTargetUnit((char*)unit, (char*)order->field_16, 0);
        order->field_6 = 0x11808;
        return 1;
    case 2:
        ((Unit*)unit)->ReleaseWeapons(3);
        return 9;
    default:
        return 7;
    }
}
