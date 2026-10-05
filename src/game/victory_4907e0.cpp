// Decompiled by Opus. Names are provisional.
// Constructor of a three-level class: the base constructor (0x44ef20) is out
// of line, the middle class's constructor (vtable 0x4fd980, out-of-line copy
// at 0x4905e0) is inlined. The middle class assigns its members in the body
// (struct assignments, so the zero vector is built in three registers and
// stored through a pointer).

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

class Base_00490a10 {                  // the object at +0x4 (see 0x490a10.cpp)
public:
    virtual ~Base_00490a10();
};

class Class_0044f010;                  // slot 6's result (see 0x44f450.cpp)
class Class_00415c10;                  // the bit writer slot 8 takes
class Class_00415dc0;                  // the bit reader slot 9 takes

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60 (the family is listed
// in 0x44ef60.cpp).
class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p);
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual Class_0044f010* FUN_0044eff0();         // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(Class_00415c10*);     // slot 8
    virtual void FUN_0044efd0(Class_00415dc0*);     // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

// Vtable 0x4fd980, constructor 0x4905e0, ??_G 0x490630.
class Class_00490630 : public Class_0044ef20 {
public:
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
    virtual void FUN_0044efb0();                    // slot 2, 0x490690
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4, 0x490650
};

// Vtable 0x4fd9b0, ??_G 0x490840.
class Class_004907e0 : public Class_00490630 {
public:
    Class_004907e0(Struct_004907e0* p);
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x490860
    virtual void FUN_0044efb0();                    // slot 2, 0x490880
    virtual int FUN_0044efe0();                     // slot 7, 0x4908b0
    virtual void FUN_0044efc0(Class_00415c10*);     // slot 8, 0x4908c0
};

// FUNCTION: 0x4907e0
Class_004907e0::Class_004907e0(Struct_004907e0* p)
    : Class_00490630(p)
{
    dirty = 1;
    mode = 0;
}
