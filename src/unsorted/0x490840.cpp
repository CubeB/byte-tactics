// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004907e0
// (vtable 0x4fd9b0), derived from Class_00490630 and Class_0044ef20 (see
// 0x44ef60.cpp for the family). Both derived destructors are trivial, so only
// the inlined base destructor's store of 0x4fd428 is left.
//
// A trivial destructor never stores this class's vtable, so a static object
// does not make the compiler emit it. The constructor (0x4907e0, matched in
// 0x4907e0.cpp, where the middle constructor is inlined) is defined again
// below, unannotated, to emit the vtable and with it this COMDAT.

class Base_00490a10 {                  // the object at +0x4 (see 0x490a10.cpp)
public:
    virtual ~Base_00490a10();
};

struct Struct_004907e0;                // the owner (see 0x4907e0.cpp)

struct Vec3_004907e0 {
    int x, y, z;
};

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p);
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(int, int, int);       // slot 3
    virtual void FUN_0044f000(int, int, int);       // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual int FUN_0044eff0();                     // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(int);                 // slot 8
    virtual void FUN_0044efd0(int);                 // slot 9
    virtual void FUN_0044ef50(int);                 // slot 10
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
    virtual void FUN_0044f000(int, int, int);       // slot 4, 0x490650
};

// Vtable 0x4fd9b0, constructor 0x4907e0, ??_G 0x490840.
class Class_004907e0 : public Class_00490630 {
public:
    Class_004907e0(Struct_004907e0* p);
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x490860
    virtual void FUN_0044efb0();                    // slot 2, 0x490880
    virtual int FUN_0044efe0();                     // slot 7, 0x4908b0
    virtual void FUN_0044efc0(int);                 // slot 8, 0x4908c0
};

// FUNCTION: 0x490840 ??_GClass_004907e0@@UAEPAXI@Z
Class_004907e0::Class_004907e0(Struct_004907e0* p)
    : Class_00490630(p)
{
    dirty = 1;
    mode = 0;
}
