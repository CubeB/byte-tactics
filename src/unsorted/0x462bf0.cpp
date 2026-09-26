// Decompiled by Haiku. Names are provisional.

struct Class_00462bf0 {
    char unknown_0[4];
    int field_0x4;
    char unknown_8[4];
    int field_0xc;

    int FUN_00462bf0();
};

// FUNCTION: 0x462bf0
int Class_00462bf0::FUN_00462bf0()
{
    int eax = this->field_0x4;
    int ecx = this->field_0xc;
    return eax + ecx + 0x14;
}
