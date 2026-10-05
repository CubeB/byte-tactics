// Decompiled by Opus. Names are provisional.
// Out-of-line destructor of Class_00490880 (vtable 0x4fd9e0), derived from
// Class_00490630 and Class_0044ef20 (see 0x44ef60.cpp for the family). Its
// scalar deleting destructor, 0x4909a0, inlines the same body. The middle
// class's vtable store is dead, and the empty inline base destructor leaves
// only the base vtable store. Class declarations copied from 0x4909a0.cpp.

class Base_00490a10 {                  // the object at +0x4 (see 0x490a10.cpp)
public:
    virtual ~Base_00490a10();
};

struct Struct_004907e0;                // the owner (see 0x4907e0.cpp)

struct Vec3_004907e0 {
    int x, y, z;
};

class Class_0044f010;                  // slot 6's result (see 0x44f450.cpp)
class Class_00415c10;                  // the bit writer slot 8 takes
class Class_00415dc0;                  // the bit reader slot 9 takes

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
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

    Class_00490630(Struct_004907e0* p);
    virtual void FUN_0044efb0();                    // slot 2, 0x490690
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4, 0x490650
};

// Vtable 0x4fd9e0, constructor 0x490940, destructor 0x4909e0, ??_G 0x4909a0.
// Slots 2 and 4 are inherited from Class_00490630.
class Class_00490880 : public Class_00490630 {
public:
    Class_00490880(Struct_004907e0* p);
    virtual ~Class_00490880();                      // slot 0
    virtual void FUN_0044efd0(Class_00415dc0*);     // slot 9, 0x490a10
};

// FUNCTION: 0x4909e0
Class_00490880::~Class_00490880()
{
    delete field_4;
    field_4 = 0;
}
