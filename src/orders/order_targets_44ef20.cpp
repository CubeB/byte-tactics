// Decompiled by Opus and Haiku. Names are provisional.
// The base of a class family (vtable 0x4fd428, 11 slots; constructor
// 0x44ef20, ??_G 0x44ef60). Derived classes, all with this class's
// declaration copied verbatim:
//   Class_0044f010  vtable 0x4fd458  ctor 0x44f010  dtor 0x44f450  ??_G 0x44f040
//   Class_0044f570  vtable 0x4fd488  ctor 0x44f570  ??_G 0x44f590
//   Class_00490630  vtable 0x4fd980  ctor 0x4905e0  ??_G 0x490630
//     Class_004907e0  vtable 0x4fd9b0  ctor 0x4907e0  ??_G 0x490840
//     Class_00490880  vtable 0x4fd9e0  ctor 0x490940  dtor 0x4909e0  ??_G 0x4909a0

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

class Class_0044ced0 {
public:
    void FUN_0044ced0(int);
};

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
    virtual void FUN_0044efc0(BitWriter*);          // slot 8
    virtual void FUN_0044efd0(BitReader*);          // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

// The constructor. Its vtable reference makes the compiler emit the scalar
// deleting destructor here too; the destructor is empty and inline, so only
// the vtable store is left in it.
// FUNCTION: 0x44ef20
// FUNCTION: 0x44ef60 ??_GClass_0044ef20@@UAEPAXI@Z
Class_0044ef20::Class_0044ef20(Struct_004907e0* p)
{
    owner = p;
    field_4 = 0;
}

// Slot 3: does nothing.
// FUNCTION: 0x44ef40
void Class_0044ef20::FUN_0044ef40(Vec3_004907e0*, int, int)
{
}

// Slot 10: does nothing.
// FUNCTION: 0x44ef50
void Class_0044ef20::FUN_0044ef50(void*)
{
}

// Slot 5: whether there is an object at +0x4.
// FUNCTION: 0x44ef80
int Class_0044ef20::FUN_0044ef80()
{
    return field_4 != 0;
}

// Slot 1: sets the object at +0x4, first telling the one it replaces 0x80.
// FUNCTION: 0x44ef90
void Class_0044ef20::FUN_0044ef90(void* param)
{
    if (field_4 != 0) {
        ((Class_0044ced0*)field_4)->FUN_0044ced0(0x80);
    }
    field_4 = (Base_00490a10*)param;
}

// Slot 2: does nothing.
// FUNCTION: 0x44efb0
void Class_0044ef20::FUN_0044efb0()
{
}

// Slot 8: does nothing.
// FUNCTION: 0x44efc0
void Class_0044ef20::FUN_0044efc0(BitWriter*)
{
}

// Slot 9: does nothing.
// FUNCTION: 0x44efd0
void Class_0044ef20::FUN_0044efd0(BitReader*)
{
}

// Slot 7: 0.
// FUNCTION: 0x44efe0
int Class_0044ef20::FUN_0044efe0()
{
    return 0;
}

// Slot 6: none.
// FUNCTION: 0x44eff0
Class_0044f010* Class_0044ef20::FUN_0044eff0()
{
    return 0;
}

// Slot 4: does nothing.
// FUNCTION: 0x44f000
void Class_0044ef20::FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*)
{
}
