// Decompiled by Sonnet. Names are provisional.

class Class_0044d7c0 {
public:
    char unknown_0[8];
    short field_8;      // +0x8
    short field_a;      // +0xa
    char unknown_c[0x14 - 0xc];
    int field_14;       // +0x14 (min distance squared)
    int field_18;       // +0x18 (max distance squared)

    bool FUN_0044d7c0(int param_1, int param_2);
};

// FUNCTION: 0x44d7c0
bool Class_0044d7c0::FUN_0044d7c0(int param_1, int param_2)
{
    int dy = param_2 - field_a;
    int dx = param_1 - field_8;
    int distSq = dx * dx + dy * dy;
    return distSq <= field_18 && distSq >= field_14;
}
