// Decompiled by Haiku. Names are provisional.

class Class_004c5ba0 {
public:
    char unknown_0[0x4];
    int field_4;
    int field_8;

    int FUN_004c5ba0(void);
};

// FUNCTION: 0x4c5ba0
int Class_004c5ba0::FUN_004c5ba0(void)
{
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 3;
}
