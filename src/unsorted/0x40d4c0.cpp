// Decompiled by Haiku. Names are provisional.

class Class_0040d4c0 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_0040d4c0();
};

// FUNCTION: 0x40d4c0
int Class_0040d4c0::FUN_0040d4c0() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 2;
}
