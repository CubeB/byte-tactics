// Decompiled by Haiku. Names are provisional.

class Class_004cfba0 {
public:
    char unknown_0[0x1e4];
    int field_1e4;
    char unknown_1e8[0xa0];
    int field_288;

    int FUN_004cfba0(void);
};

// FUNCTION: 0x4cfba0
int Class_004cfba0::FUN_004cfba0(void)
{
    if (field_1e4 == 0 && field_288 == -1) {
        return 0;
    }
    return 1;
}
