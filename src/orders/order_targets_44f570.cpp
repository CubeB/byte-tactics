// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fd488[];

struct Class_0044f570
{
public:
    void** vtable_ptr;
    int field_4;
    int field_8;
    char unknown_c[0xc];
    int field_18;

    Class_0044f570(int param_1);
};

// FUNCTION: 0x44f570
Class_0044f570::Class_0044f570(int param_1)
{
    field_8 = param_1;
    field_4 = 0;
    vtable_ptr = DAT_004fd488;
    field_18 = 0;
}
