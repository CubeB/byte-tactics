// Decompiled by Haiku. Names are provisional.

struct Class_44ec20 {
public:
    char unknown_0[8];
    unsigned char field_8;
    char unknown_9[27];
    short field_24;

    void FUN_0044ec20(short param_1);
};

// FUNCTION: 0x44ec20
void Class_44ec20::FUN_0044ec20(short param_1)
{
    field_8 |= 1;
    field_24 = param_1;
}
