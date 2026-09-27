// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Constructor of the 0x2f-byte behaviour holder stored at unit+0. It zeroes two
// Vec3-shaped triples and a few scalars, mirrors a value from the unit type and
// then creates one of four behaviour objects, chosen by the unit's target type
// (byte at +0x73 == 3) and bit 11 of the unit type's flags at +0x241.

#pragma pack(push, 1)
struct Target_0043dc00 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
};

struct UnitType_0043dc00 {
    char unknown_0[0x1b6];
    int field_1b6;                     // +0x1b6
    char unknown_1ba[0x241 - 0x1ba];
    unsigned int mode : 11;            // +0x241
    unsigned int flag_800 : 1;         // bit 11
};

struct Unit_0043dc00 {
    char unknown_0[0x92];
    UnitType_0043dc00* type;           // +0x92
    Target_0043dc00* target;           // +0x96
};
#pragma pack(pop)

struct Vec3_0043dc00 {
    int x, y, z;

    Vec3_0043dc00() {}
    Vec3_0043dc00(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

// The four behaviour classes, only their sizes matter here (the operands of the
// operator new calls). Their constructors are declared, not defined.
class Class_00490880 { public: char unknown[0x27]; Class_00490880(Unit_0043dc00* unit); };
class Class_004907e0 { public: char unknown[0x28]; Class_004907e0(Unit_0043dc00* unit); };
class Class_0044f570 { public: char unknown[0x1c]; Class_0044f570(Unit_0043dc00* unit); };
class Class_0044f010 { public: char unknown[0x65]; Class_0044f010(Unit_0043dc00* unit); };

#pragma pack(push, 1)
class Class_0043dc00 {
public:
    void* obj;                         // +0x0
    int field_4;                       // +0x4
    Vec3_0043dc00 p1;                  // +0x8
    Vec3_0043dc00 p2;                  // +0x14
    int field_20;                      // +0x20
    short field_24;                    // +0x24
    int field_26;                      // +0x26
    char unknown_2a[4];                // +0x2a
    unsigned char mode : 2;            // +0x2e
    unsigned char flag : 1;            // bit 2
    unsigned char rest : 5;

    Class_0043dc00(Unit_0043dc00* unit);
};
#pragma pack(pop)

// FUNCTION: 0x43dc00
Class_0043dc00::Class_0043dc00(Unit_0043dc00* unit)
{
    p1 = Vec3_0043dc00(0, 0, 0);
    field_20 = 0;
    field_24 = 0;
    field_26 = 0;
    p2 = Vec3_0043dc00(0, 0, 0);
    mode = 1;
    flag = 0;
    field_4 = unit->type->field_1b6;
    if (unit->target->field_0 != 0 && unit->target->type == 3) {
        if (unit->type->flag_800)
            obj = new Class_00490880(unit);
        else
            obj = new Class_0044f570(unit);
    } else {
        if (unit->type->flag_800)
            obj = new Class_004907e0(unit);
        else
            obj = new Class_0044f010(unit);
    }
}
