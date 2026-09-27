// Decompiled by Opus. Names are provisional.
// Constructor of the class whose vtable is at 0x4fc990 (slot 1 is its scalar
// deleting destructor 0x4079d0). Same shape as Class_00407a90's constructor
// with one more field.

struct Arg_004079a0 {
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

class Class_004079d0 {
public:
    virtual void FUN_004079f0();       // slot 0
    virtual void* FUN_004079d0(unsigned char flag); // slot 1

    Arg_004079a0* field_4;             // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    unsigned int field_10;             // +0x10
    int field_14;                      // +0x14

    Class_004079d0(Arg_004079a0* param_1, int param_2, int param_3);
};

// FUNCTION: 0x4079a0
Class_004079d0::Class_004079d0(Arg_004079a0* param_1, int param_2, int param_3)
    : field_4(param_1), field_8(param_2), field_c(0), field_10(param_1->field_4),
      field_14(param_3)
{
}
