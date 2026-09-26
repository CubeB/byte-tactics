// Decompiled by Sonnet. Names are provisional.

struct Class_00472fd0 {
    char unknown_0[0x10];
    int field_10;   // +0x10
    int field_14;   // +0x14

    bool FUN_00472fd0();
};

// FUNCTION: 0x472fd0
bool Class_00472fd0::FUN_00472fd0()
{
    int diff;
    if (field_10 == 0) {
        diff = 0;
    } else {
        diff = (field_14 - field_10) / 48;
    }
    return diff == 0;
}
