// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_0044ef20, the
// base of a class family (vtable 0x4fd428, 11 slots; constructor 0x44ef20).
// Its destructor is empty and inline, so only the vtable store is left.
//
// Derived classes, all with this class's declaration copied verbatim:
//   Class_0044f010  vtable 0x4fd458  ctor 0x44f010  dtor 0x44f450  ??_G 0x44f040
//   Class_0044f570  vtable 0x4fd488  ctor 0x44f570  ??_G 0x44f590
//   Class_00490630  vtable 0x4fd980  ctor 0x4905e0  ??_G 0x490630
//     Class_004907e0  vtable 0x4fd9b0  ctor 0x4907e0  ??_G 0x490840
//     Class_00490880  vtable 0x4fd9e0  ctor 0x490940  dtor 0x4909e0  ??_G 0x4909a0
//
// The static object below exists only to make the compiler emit the vtable
// (and with it this COMDAT) here; its constructor is only declared.

class Base_00490a10 {                  // the object at +0x4 (see 0x490a10.cpp)
public:
    virtual ~Base_00490a10();
};

struct Struct_004907e0;                // the owner (see 0x4907e0.cpp)

struct Vec3_004907e0;                  // a position (see 0x4907e0.cpp)
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

// FUNCTION: 0x44ef60 ??_GClass_0044ef20@@UAEPAXI@Z
static Class_0044ef20 s_obj(0);
