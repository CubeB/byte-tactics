// Decompiled by Haiku. Names are provisional.

class Class_0044ee70 {
public:
    char unknown_0[4];
    int field_4;                              // +0x4
    int field_8;                              // +0x8

    int FUN_0044ee70();
};

// FUNCTION: 0x44ee70
int Class_0044ee70::FUN_0044ee70()
{
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 2;
}
