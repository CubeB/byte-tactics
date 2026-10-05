// Decompiled by Opus. Names are provisional.
// Slot 4 of Class_0044e740 (vtable 0x4fd3f8, constructor 0x44e740): done
// when the unit is within 48 world units of the target point, or, when flag
// bit 0 is set, when the heading from the origin to the second point equals
// the stored heading.
#include <math.h>

struct Vec3_0044eb60 {
    int x;
    int y;
    int z;

    Vec3_0044eb60() {}
    Vec3_0044eb60(int ax, int ay, int az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

#pragma pack(push, 1)
struct Object_0044eb60 {
    char unknown_0[0x6a];
    Vec3_0044eb60 pos;                 // +0x6a
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0044e740 {
public:
    void* vtable;                      // +0x0
    void* field_4;                     // +0x4
    short field_8;                     // +0x8
    Vec3_0044eb60 target;              // +0xa
    Vec3_0044eb60 other;               // +0x16
    short field_22;                    // +0x22
    unsigned short heading;            // +0x24
    char unknown_26[2];
    Object_0044eb60* self;             // +0x28

    int FUN_0044eb60(Object_0044eb60* unit);
};
#pragma pack(pop)

unsigned short __stdcall GetHeadingBetween(Vec3_0044eb60* from, Vec3_0044eb60* to);

// FUNCTION: 0x44eb60
int Class_0044e740::FUN_0044eb60(Object_0044eb60* unit)
{
    if ((float)_hypot(unit->pos.x - target.x, unit->pos.z - target.z) / 65536.0f < 48.0f)
        return 1;
    if (field_8 & 1) {
        Vec3_0044eb60 origin;
        origin = Vec3_0044eb60(0, 0, 0);
        if (GetHeadingBetween(&origin, &other) == heading)
            return 1;
    }
    return 0;
}
