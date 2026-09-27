// Decompiled by Space Bunny Free. Names are provisional.
// Slot 16 of Class_00485e30 (vtable 0x4fd698); see src/unsorted/0x485e30.cpp
// and the sibling slots 0x480ce0, 0x480d50, 0x480db0, 0x480df0.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xba];
    unsigned char dirty;                // +0xba
    char unknown_bb[0x10f - 0xbb];
    unsigned char bit0 : 1;             // +0x10f
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
};

struct Data_00480b20 {
    int unknown_0;                      // +0x0
    int unknown_4;                      // +0x4
    int unknown_8;                      // +0x8
    Unit* unit;                         // +0xc
};
#pragma pack(pop)

class Class_0048b090 {
public:
    void FUN_0048b090(int which, int on);
};

class Class_00485e30 {
public:
    char unknown_0[0x540];
    Data_00480b20* data;                // +0x540

    void FUN_004b0670(int which, int value);
};

void __stdcall FUN_0047dac0(Unit* unit, int flag);

// FUNCTION: 0x480b20
void Class_00485e30::FUN_004b0670(int which, int value)
{
    Unit* unit = data->unit;
    switch (which) {
    case 1:
        ((Class_0048b090*)unit)->FUN_0048b090(1, value);
        break;
    case 5:
        unit->bit0 = unit->bit0 ^ value;
        break;
    case 6:
        unit->bit1 = value;
        break;
    case 18:
        FUN_0047dac0(unit, value);
        break;
    case 19:
        unit->bit3 = value;
        break;
    case 20:
        ((Class_0048b090*)unit)->FUN_0048b090(2, value);
        break;
    }
    unit->dirty |= 4;
}
