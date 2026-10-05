// Decompiled by Opus. Names are provisional.
// A second constructor for the class built by 0x44e330 (same vtable
// DAT_004fd3b8): it takes its position from the source's unit instead.

struct Vec3_0044e250 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Unit {
    char unknown_0[0x6a];
    Vec3_0044e250 pos;                 // +0x6a
};

struct Source_0044e250 {
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
};
#pragma pack(pop)

class Class_004895c0 {
public:
    virtual void Unknown_0();
    int field_4;
    int field_8;
    int field_c;

    Class_004895c0(int unit, int value);
    void FUN_00489690(int unit);
};

extern void* DAT_004fd2f8[];
extern void* DAT_004fd3b8[];

#pragma pack(push, 2)
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Source_0044e250* field_4;          // +0x4

    Class_0044ce20(Source_0044e250* source)
    {
        vtable = DAT_004fd2f8;
        field_4 = source;
    }
};

class Class_0044e250 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Unit* field_12;                    // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e250 pos;                 // +0x26

    Class_0044e250(Source_0044e250* source, int unit, short value);
};
#pragma pack(pop)

// FUNCTION: 0x44e250
Class_0044e250::Class_0044e250(Source_0044e250* source, int unit, short value)
    : Class_0044ce20(source), field_10(value), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    field_a = 0;
    field_c = 0;
    field_8 = 5;
    field_12 = source->unit;
    pos = field_12->pos;
    ref.FUN_00489690(unit);
}
