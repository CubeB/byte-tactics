// Decompiled by Opus. Names are provisional.
// The constructor of Class_0044ef20, the base of the class family listed in
// 0x44ef60.cpp (vtable 0x4fd428).

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

// FUNCTION: 0x44ef20
Class_0044ef20::Class_0044ef20(Struct_004907e0* p)
{
    owner = p;
    field_4 = 0;
}
