// Decompiled by Haiku. Names are provisional.

extern void __stdcall FUN_004c91a0(int*);

class Class_00437820 {
public:
    char unknown_0[4];
    int field_4;

    Class_00437820* FUN_00437820(int*);
};

// FUNCTION: 0x437820
Class_00437820* Class_00437820::FUN_00437820(int* param_1)
{
    FUN_004c91a0(param_1);
    field_4 = param_1[1];
    return this;
}
