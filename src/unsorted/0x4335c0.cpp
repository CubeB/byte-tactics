// Decompiled by Haiku. Names are provisional.

class Class_004335c0
{
public:
    char unknown_0[0x4];
    int field_4;
    int field_8;

    int FUN_004335c0();
};

// FUNCTION: 0x4335c0
int Class_004335c0::FUN_004335c0()
{
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 4;
}
