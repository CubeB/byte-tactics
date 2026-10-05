// Decompiled by Opus. Names are provisional.

#pragma pack(push, 2)
struct Source_0044e330 {
    char unknown_0[0xe];
    int field_e;                       // +0xe
};
#pragma pack(pop)

struct Vec3_0044e330 {
    int x;
    int y;
    int z;
};

// Unit reference: vtable, unit, next link, extra value (0x10 bytes).
class Class_004895c0 {
public:
    virtual void Unknown_0();
    int field_4;
    int field_8;
    int field_c;

    Class_004895c0(int unit, int value);
    void SetUnit(int unit);
};

// The vtables are stored by hand: the base one already has a DAT_ name.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd3b8[];

#pragma pack(push, 2)
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Source_0044e330* field_4;          // +0x4

    Class_0044ce20(Source_0044e330* source)
    {
        vtable = DAT_004fd2f8;
        field_4 = source;
    }
};

class Class_0044e330 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    int field_12;                      // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e330 pos;                 // +0x26

    Class_0044e330(Source_0044e330* source, int unit, const Vec3_0044e330& p);
};
#pragma pack(pop)

// FUNCTION: 0x44e330
Class_0044e330::Class_0044e330(Source_0044e330* source, int unit, const Vec3_0044e330& p)
    : Class_0044ce20(source), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    ref.SetUnit(unit);
    pos = p;
    field_a = 0;
    field_c = 0;
    field_10 = -1;
    field_12 = source->field_e;
    field_8 = 0xa3;
}
