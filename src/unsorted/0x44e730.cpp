// Decompiled by Haiku. Names are provisional.

struct Class_44e730 {
public:
    char unknown_0[8];
    unsigned char field_8;
    char unknown_9[1];
    short field_a;

    void FUN_0044e730(short param_1);
};

// FUNCTION: 0x44e730
void Class_44e730::FUN_0044e730(short param_1)
{
    field_8 |= 0x10;
    field_a = param_1;
}
