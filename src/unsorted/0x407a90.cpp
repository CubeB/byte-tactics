// Decompiled by Sonnet. Names are provisional.

struct Class_00407a90_arg {
    char unknown_0[4];
    unsigned char field_4;
};

class Class_00407a90 {
public:
    virtual void FUN_00407ae0();

    Class_00407a90_arg* field_4;
    int field_8;
    int field_c;
    unsigned int field_10;

    Class_00407a90(Class_00407a90_arg* param_1, int param_2);
};

// FUNCTION: 0x407a90
Class_00407a90::Class_00407a90(Class_00407a90_arg* param_1, int param_2)
    : field_4(param_1), field_8(param_2), field_c(0), field_10(param_1->field_4)
{
}
