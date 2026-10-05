// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Another constructor of the class built by 0x44e250 / 0x44e2d0 / 0x44e330
// (vtable DAT_004fd3b8): it copies the position from the order's unit and
// picks a type of 7 or 1 from the unit definition's flag bit 11.

struct Vec3_0044e190 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct UnitDef_0044e190 {
    char unknown_0[0x241];
    unsigned int flags;                // +0x241
};
#pragma pack(pop)

struct Sub_0044e190 {
    char unknown_0[0xdc];
    int field_dc;                      // +0xdc
};

#pragma pack(push, 2)
struct Unit {
    char unknown_0[0x10];
    Sub_0044e190* field_10;            // +0x10
    char unknown_14[0x6a - 0x14];
    Vec3_0044e190 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0044e190* def;             // +0x92
};

struct Order {
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
};
#pragma pack(pop)

class Class_004895c0;

#pragma pack(push, 2)
struct Owner_004895c0 {
    char unknown_0[0x92];
    UnitDef_0044e190* def;             // +0x92
    char unknown_96[0xa2 - 0x96];
    Class_004895c0* head;              // +0xa2
    short flag;                        // +0xa6
};
#pragma pack(pop)

class Class_004895c0 {
public:
    Owner_004895c0* owner;             // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    Class_004895c0(Owner_004895c0* o, int v);
    virtual ~Class_004895c0();
    void FUN_00489690(Owner_004895c0* o);
};

extern void* DAT_004fd2f8[];
extern void* DAT_004fd3b8[];

#pragma pack(push, 2)
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Order* field_4;                    // +0x4

    Class_0044ce20(Order* order)
    {
        vtable = DAT_004fd2f8;
        field_4 = order;
    }
};

class Class_0044e190 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Unit* field_12;                    // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e190 pos;                 // +0x26
    int field_32;                      // +0x32

    Class_0044e190(Order* order, Unit* unit);
};
#pragma pack(pop)

// FUNCTION: 0x44e190
Class_0044e190::Class_0044e190(Order* order, Unit* unit)
    : Class_0044ce20(order), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    ref.FUN_00489690((Owner_004895c0*)unit);
    field_a = 0;
    field_c = 0;
    field_12 = order->unit;
    pos = field_12->pos;
    field_10 = -1;
    if ((unsigned char)(ref.owner->def->flags >> 11) & 1) {
        field_8 = 7;
        int t = field_12->field_10->field_dc;
        if (t != 0)
            field_32 = t << 16;
        else
            field_32 = 0x640000;
    } else {
        field_8 = 1;
    }
}
