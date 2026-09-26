// Decompiled by Sonnet. Names are provisional.

struct Class_004e21a0 {
    double field_0;
    char unknown_8[0x40];
    char field_48;

    void FUN_004e21a0(double param_1);
};

// FUNCTION: 0x4e21a0
void Class_004e21a0::FUN_004e21a0(double param_1)
{
    if (field_48 != 0) {
        field_0 -= param_1;
    } else {
        field_0 = param_1 + field_0;
    }
}
