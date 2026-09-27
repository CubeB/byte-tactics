// Decompiled by Opus. Names are provisional.

struct Vec3_00490650 {
    int x;
    int y;
    int z;
};

class Class_00490650 {
public:
    char unknown_0[0xc];
    Vec3_00490650 a;                     // +0xc
    Vec3_00490650 b;                     // +0x18
    short c;                             // +0x24

    void FUN_00490650(Vec3_00490650* outA, Vec3_00490650* outB, short* outC);
};

// FUNCTION: 0x490650
void Class_00490650::FUN_00490650(Vec3_00490650* outA, Vec3_00490650* outB, short* outC)
{
    *outA = a;
    *outB = b;
    *outC = c;
}
