// Decompiled by Opus. Names are provisional.

class Class_00435920 {
public:
    char unknown_0[0xa04];
    int field_a04;                      // +0xa04

    int FUN_00435920();
};

// FUNCTION: 0x435920
int Class_00435920::FUN_00435920()
{
    int v = field_a04;
    if (v < 0x3e6666)
        return 0x10;
    if (v < 0x600000)
        return 0x18;
    if (v < 0x800000)
        return 0x20;
    if (v < 0xa00000)
        return 0x30;
    if (v < 0xc00000)
        return 0x40;
    return 0x80;
}
