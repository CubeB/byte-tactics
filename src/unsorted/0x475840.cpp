// Decompiled by Haiku. Names are provisional.

struct Class_00475840 {
    int unknown_0;
    int field_4;
    int field_8;
};

// FUNCTION: 0x475840
int __fastcall FUN_00475840(Class_00475840* param_1)
{
    if (param_1->field_4 == 0) return 0;
    return (param_1->field_8 - param_1->field_4) / 48;
}
