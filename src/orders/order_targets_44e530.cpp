// Decompiled by Opus. Names are provisional.

struct Vec3_0044e530 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Object_0044e530 {
    char unknown_0[0x66];
    unsigned short heading;            // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0044e530 pos;                 // +0x6a
};

class Class_0044e530 {
public:
    char unknown_0[8];
    unsigned short flags;              // +0x08
    char unknown_a[0xe - 0xa];
    unsigned short heading;            // +0x0e
    char unknown_10[0x12 - 0x10];
    Object_0044e530* self;             // +0x12
    char unknown_16[0x1a - 0x16];
    Object_0044e530* target;           // +0x1a
    int FUN_0044e530(unsigned short* out);
};
#pragma pack(pop)

unsigned short __stdcall GetHeadingBetween(Vec3_0044e530* from, Vec3_0044e530* to);

// FUNCTION: 0x44e530
int Class_0044e530::FUN_0044e530(unsigned short* out)
{
    unsigned short f = flags;
    if ((f & 0x84) && target != 0) {
        if (f & 2) {
            *out = GetHeadingBetween(&self->pos, &target->pos);
            return 1;
        }
        if (f & 0x40) {
            *out = heading;
            return 1;
        }
        *out = target->heading;
        return 1;
    }
    if (f & 0x40) {
        *out = heading;
        return 1;
    }
    return 0;
}
