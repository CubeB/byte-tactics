// Decompiled by Opus. Names are provisional.
// Class_00490630's override of slot 4 (vtable 0x4fd980, inherited by
// Class_004907e0 and Class_00490880; the class family is listed in
// 0x44ef60.cpp): copies out the position, the velocity and field_24 (the
// heading 0x490690 turns towards the owner).

struct Vec3_004907e0 {
    int x;
    int y;
    int z;
};

class Class_00490630 {
public:
    char unknown_4[0xc - 0x4];
    Vec3_004907e0 pos;                   // +0xc
    Vec3_004907e0 vel;                   // +0x18
    short field_24;                      // +0x24

    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
};

// FUNCTION: 0x490650
void Class_00490630::FUN_0044f000(Vec3_004907e0* outPos, Vec3_004907e0* outVel,
                                  short* outHeading)
{
    *outPos = pos;
    *outVel = vel;
    *outHeading = field_24;
}
