// Decompiled by Haiku. Names are provisional.

struct Class_00415da0
{
    char unknown_0[0xc];
    void* field_c;

    void FUN_00415da0(int param_1, unsigned char param_2);
};

// FUNCTION: 0x415da0
void Class_00415da0::FUN_00415da0(int param_1, unsigned char param_2)
{
    *(unsigned char*)((char*)field_c + param_1) = param_2;
}
