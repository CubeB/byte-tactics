// Decompiled by Haiku. Names are provisional.

struct Class_44e720 {
public:
    char unknown_0[8];
    unsigned char field_8;
    char unknown_9[5];
    short field_e;

    void FUN_0044e720(short param_1);
};

// FUNCTION: 0x44e720
void Class_44e720::FUN_0044e720(short param_1)
{
    field_8 |= 0x40;
    field_e = param_1;
}
