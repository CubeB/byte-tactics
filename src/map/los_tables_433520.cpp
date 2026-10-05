// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_00433520 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_00433520();
};
#pragma pack(pop)

// FUNCTION: 0x433520
int Class_00433520::FUN_00433520()
{
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 4;
}
