// Decompiled by Sonnet. Names are provisional.

struct Elem_4b0610 {
    int value;         // +0x0
    char pad[0xa0];    // pad to stride 0xa4
};

class Class_004b0610 {
public:
    virtual void FUN_1() = 0;
    virtual void FUN_2() = 0;
    virtual void FUN_3() = 0;
    virtual void FUN_4() = 0;

    int field_4;
    int field_8;
    char unknown_c[0x10 - 0xc];
    int field_10;
    int field_14;
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];
    int field_53c;

    Class_004b0610();
};

extern int FUN_004b6330();

// FUNCTION: 0x4b0610
Class_004b0610::Class_004b0610()
{
    field_8 = 0;
    field_14 = 0;
    field_10 = 0;
    for (int i = 0; i < 8; i++) {
        arr[i].value = 0;
    }
    field_53c = 0;
    field_4 = FUN_004b6330();
}
