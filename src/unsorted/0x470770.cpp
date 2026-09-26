// Decompiled by Haiku. Names are provisional.

struct Class_00470770 {
    int unknown_0;
    int field_4;
    int field_8;
};

// FUNCTION: 0x470770
int __fastcall FUN_00470770(Class_00470770* param_1)
{
    if (param_1->field_4 == 0) return 0;
    return (param_1->field_8 - param_1->field_4) / 14;
}
