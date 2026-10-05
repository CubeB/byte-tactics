// Decompiled by Sonnet. Names are provisional.

struct Vec4_4c6ae0 { int a, b, c, d; };

class Class_004c6ae0 {
public:
    char unknown_0[0x1c];
    Vec4_4c6ae0 field_1c;    // +0x1c

    Vec4_4c6ae0* FUN_004c6ae0(Vec4_4c6ae0* param_1);
};

// FUNCTION: 0x4c6ae0
Vec4_4c6ae0* Class_004c6ae0::FUN_004c6ae0(Vec4_4c6ae0* param_1)
{
    *param_1 = field_1c;
    return param_1;
}
