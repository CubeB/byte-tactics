// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fc9b0[];

struct Param
{
public:
    char unknown_0[4];
    unsigned char field_4;
};

struct Class_004087e0
{
public:
    void** vtable;
    int field_4;
    int field_8;
    int field_c;
    int field_10;

    Class_004087e0(int param_1, int param_2);
};

// FUNCTION: 0x4087e0
Class_004087e0::Class_004087e0(int param_1, int param_2)
{
    field_8 = param_2;
    field_4 = param_1;
    field_c = 0;

    Param* p = (Param*)param_1;
    unsigned char dl = p->field_4;
    vtable = DAT_004fc9b0;
    field_10 = (unsigned int)dl;
}
