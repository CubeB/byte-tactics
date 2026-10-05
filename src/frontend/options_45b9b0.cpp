// Decompiled by Opus. Names are provisional.
// Sets field_140 (a step index out of field_136 steps) from a value in the
// range 0..field_13c, rounding up; the inverse of ReadSliderValue.

struct Class_0045b9b0
{
    char unknown_0[0x136];
    short field_136;               // +0x136
    char unknown_138[0x13c - 0x138];
    int field_13c;                 // +0x13c
    short field_140;               // +0x140
};

// FUNCTION: 0x45b9b0
void __stdcall SetSliderFromValue(Class_0045b9b0* param_1, int value)
{
    int max = param_1->field_13c;
    if (value > max)
        value = max;
    float f = (float)value / (float)max * (param_1->field_136 - 1);
    if (f - (int)f != 0.0f)
        f += 1.0;
    param_1->field_140 = (short)f;
}
