// Decompiled by Haiku. Names are provisional.

class Class_0040c5b0 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_0040c5b0();
};

// FUNCTION: 0x40c5b0
int Class_0040c5b0::FUN_0040c5b0() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 3;
}
