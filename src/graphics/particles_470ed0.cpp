// Decompiled by Haiku. Names are provisional.

class Class_00470ed0 {
public:
    char unknown_0[0x14];
    int* field_14;                            // +0x14
    char unknown_18[0x8];
    int field_20;                             // +0x20

    void FUN_00470ed0(int param_1);
};

// FUNCTION: 0x470ed0
void Class_00470ed0::FUN_00470ed0(int param_1)
{
    int eax = field_20 - 1;
    field_20 = eax;
    field_14[eax] = param_1;
}
