// Decompiled by Opus. Names are provisional.
// Constructor of a three-level class: the base constructor (0x44ef20) is out
// of line, the middle class's constructor (vtable 0x4fd980) is inlined. The
// middle class assigns its members in the body (struct assignments, so the
// zero vector is built in three registers and stored through a pointer).

struct Vec3_004907e0 {
    int x, y, z;

    Vec3_004907e0() {}
    Vec3_004907e0(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

#pragma pack(push, 2)
struct Struct_004907e0 {
    char unknown_0[0x66];
    short field_66;                    // +0x66
    short field_68;
    Vec3_004907e0 pos;                 // +0x6a
};
#pragma pack(pop)

class Class_0044ef20 {
public:
    virtual void Slot0();
    int field_4;                       // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p);
};

class Class_00490630 : public Class_0044ef20 {
public:
    virtual void Slot0();
    Vec3_004907e0 pos;                 // +0xc
    Vec3_004907e0 vel;                 // +0x18
    short field_24;                    // +0x24
    char field_26;                     // +0x26
    unsigned char dirty : 1;           // +0x27 bit 0
    unsigned char mode : 2;            // +0x27 bits 1-2

    Class_00490630(Struct_004907e0* p)
        : Class_0044ef20(p)
    {
        pos = p->pos;
        vel = Vec3_004907e0(0, 0, 0);
        field_24 = p->field_66;
    }
};

class Class_004907e0 : public Class_00490630 {
public:
    virtual void Slot0();

    Class_004907e0(Struct_004907e0* p);
};

// FUNCTION: 0x4907e0
Class_004907e0::Class_004907e0(Struct_004907e0* p)
    : Class_00490630(p)
{
    dirty = 1;
    mode = 0;
}
