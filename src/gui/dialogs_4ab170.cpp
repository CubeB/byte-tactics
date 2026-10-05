// Decompiled by Haiku. Names are provisional.

void __stdcall FUN_004ab0b0(void* param_1, unsigned int* param_2, int* param_3);

struct Struct_4ab170 {
    char unknown_0[0x18];
    void* field_18;
};

// FUNCTION: 0x4ab170
void __stdcall FUN_004ab170(Struct_4ab170* param_1, unsigned int* param_2, int* param_3)
{
    FUN_004ab0b0(param_1->field_18, param_2, param_3);
}
