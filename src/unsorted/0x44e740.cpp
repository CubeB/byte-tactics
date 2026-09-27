// Decompiled by Opus. Names are provisional.
// Constructor of a Class_0044ce20 subclass (vtable 0x4fd3f8, 0x2c bytes)
// holding two points; slot 9 (0x44eb40) heads towards the first one.

struct Vec3_0044e740 {
    int x;
    int y;
    int z;
};

struct Object_0044e740;

#pragma pack(push, 2)
struct Source_0044e740 {
    char unknown_0[0xe];
    Object_0044e740* unit;             // +0xe
};
#pragma pack(pop)

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd3f8[];

#pragma pack(push, 2)
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Source_0044e740* field_4;          // +0x4

    Class_0044ce20(Source_0044e740* source)
    {
        vtable = DAT_004fd2f8;
        field_4 = source;
    }
};

class Class_0044e740 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    Vec3_0044e740 target;              // +0xa
    Vec3_0044e740 other;               // +0x16
    short field_22;                    // +0x22
    char unknown_24[4];                // +0x24
    Object_0044e740* self;             // +0x28

    Class_0044e740(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b);
};
#pragma pack(pop)

// FUNCTION: 0x44e740
Class_0044e740::Class_0044e740(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b)
    : Class_0044ce20(source)
{
    vtable = DAT_004fd3f8;
    self = source->unit;
    field_8 = 0;
    field_22 = 0;
    target = a;
    other = b;
}
