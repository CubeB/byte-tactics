// Decompiled by Haiku. Names are provisional.

class Class_0040ca30 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;
    int field_c;

    int FUN_0040ca30();
};

// FUNCTION: 0x40ca30
int Class_0040ca30::FUN_0040ca30() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_c - field_4) >> 3;
}
