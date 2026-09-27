// Decompiled by Haiku. Names are provisional.

class Class_004251f0 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_004251f0();
};

// FUNCTION: 0x4251f0
int Class_004251f0::FUN_004251f0() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 1;
}
