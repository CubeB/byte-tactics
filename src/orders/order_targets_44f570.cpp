// Decompiled by Haiku and Opus. Names are provisional.
// Class_0044f570 (vtable 0x4fd488), derived from Class_0044ef20 (see
// order_targets_44ef20.cpp for the family).

// The object at +0x4 (see victory_490940.cpp).
class Base_00490a10 {
public:
    virtual ~Base_00490a10();
};

struct Struct_004907e0;                // the owner (see 0x4907e0.cpp)

struct Vec3_004907e0;                  // a position (see 0x4907e0.cpp)
class Class_0044f010;                  // slot 6's result
class BitWriter;                       // the bit writer slot 8 takes
class BitReader;                       // the bit reader slot 9 takes

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p) { owner = p; field_4 = 0; }
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual Class_0044f010* FUN_0044eff0();         // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(BitWriter*);          // slot 8
    virtual void FUN_0044efd0(BitReader*);          // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

// Vtable 0x4fd488, constructor 0x44f570, ??_G 0x44f590.
class Class_0044f570 : public Class_0044ef20 {
public:
    char unknown_c[0x18 - 0xc];
    int field_18;                      // +0x18

    Class_0044f570(Struct_004907e0* p);
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3, 0x44f650
    virtual int FUN_0044ef80();                     // slot 5, 0x44f5b0
    virtual void FUN_0044efd0(BitReader*);          // slot 9, 0x44f5c0
};

// The constructor, with the base constructor inlined. Its vtable reference
// makes the compiler emit the scalar deleting destructor here too: the
// destructor is trivial, so the dead store of this class's vtable disappears
// and only the inlined base destructor's store of 0x4fd428 is left.
// FUNCTION: 0x44f570
// FUNCTION: 0x44f590 ??_GClass_0044f570@@UAEPAXI@Z
Class_0044f570::Class_0044f570(Struct_004907e0* p)
    : Class_0044ef20(p)
{
    field_18 = 0;
}
