// Decompiled by Sonnet. Names are provisional.

extern char* g_game;

struct Class_00474f80 {
    char unknown_0[4];
    int field_4;
    char unknown_8[8];
    int field_10;
    int field_14;

    int FUN_00474f80();
};

// FUNCTION: 0x474f80
int Class_00474f80::FUN_00474f80()
{
    int count;

    if (field_10 == 0) {
        count = 0;
    } else {
        count = (field_14 - field_10) >> 5;
    }

    bool isZero = (count == 0);
    if (isZero) {
        if ((unsigned int)field_4 < *(unsigned int*)(g_game + 0x38a47)) {
            return 1;
        }
    }

    return 0;
}
