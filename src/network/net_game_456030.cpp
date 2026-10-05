// Decompiled by Opus. Names are provisional.

struct Class_00456030 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    char field_73;                     // +0x73

    int FUN_00456030();
};

// FUNCTION: 0x456030
int Class_00456030::FUN_00456030()
{
    if (field_0 != 0 && (field_73 == 1 || field_73 == 2)) {
        return 1;
    }
    return 0;
}
