// Decompiled by Haiku. Names are provisional.

class Class_004897e0 {
public:
    char unknown_0[0x1f];
    unsigned char field_1f;                   // +0x1f
    char unknown_20[0x1b];
    unsigned char field_3b;                   // +0x3b
    char unknown_3c[0x1b];
    unsigned char field_57;                   // +0x57

    unsigned char FUN_004897e0();
};

// FUNCTION: 0x4897e0
unsigned char Class_004897e0::FUN_004897e0()
{
    unsigned char al = field_1f;
    unsigned int edx = 2;
    unsigned char dl = (unsigned char)edx;
    if ((al & dl) != 0) {
        return 0;
    }
    if ((field_3b & dl) != 0) {
        return 1;
    }
    return field_57 & dl;
}
