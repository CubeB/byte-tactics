// Decompiled by Haiku. Names are provisional.

class Class_00470270 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_00470270();
};

// FUNCTION: 0x470270
int Class_00470270::FUN_00470270() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 2;
}
