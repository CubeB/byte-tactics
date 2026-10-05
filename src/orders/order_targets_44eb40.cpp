// Decompiled by Opus. Names are provisional.
// Virtual slot 9 (vtable at 0x4fd3f8), the same slot as 0x44e530: writes the
// heading from the owner's position (+0x6a) to the target point at +0xa.

struct Vec3_0044eb40 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Object_0044eb40 {
    char unknown_0[0x6a];
    Vec3_0044eb40 pos;                 // +0x6a
};

class Class_0044eb40 {
public:
    char unknown_0[0xa];
    Vec3_0044eb40 target;              // +0x0a
    char unknown_16[0x28 - 0x16];
    Object_0044eb40* self;             // +0x28
    int FUN_0044eb40(unsigned short* out);
};
#pragma pack(pop)

unsigned short __stdcall GetHeadingBetween(Vec3_0044eb40* from, Vec3_0044eb40* to);

// FUNCTION: 0x44eb40
int Class_0044eb40::FUN_0044eb40(unsigned short* out)
{
    *out = GetHeadingBetween(&self->pos, &target);
    return 1;
}
