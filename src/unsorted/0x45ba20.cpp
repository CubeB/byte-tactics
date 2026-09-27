// Decompiled by Sonnet. Names are provisional.

struct Class_0045ba20
{
    char unknown_0[0x136];
    short field_136;              // +0x136
    char unknown_138[0x13c - 0x138];
    int field_13c;                 // +0x13c
    short field_140;               // +0x140
};

// FUNCTION: 0x45ba20
int __stdcall FUN_0045ba20(Class_0045ba20* param_1)
{
    if (param_1->field_136 <= 1)
        return 0;
    return (int)((float)param_1->field_140 / (param_1->field_136 - 1) * param_1->field_13c);
}
