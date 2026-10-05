// Decompiled by Opus, class family consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004716a0
// (vtable 0x4fd5d8), derived from Class_00471cc0 (the family is listed in
// 0x471cc0.cpp). Its implicit destructor destroys the std::vector at +0xc
// (the inlined ~vector leaves the dead store of _First in the `push ecx`
// slot), then the inlined base destructor stores the base vtable, and the
// inlined operator delete returns the object to the pool.
//
// The class has no out-of-line constructor: 0x4715a0, 0x472330 and 0x472ab0
// create it with `new`, inlining it. None is decompiled yet, so the global
// below exists only to make the compiler emit the vtable and with it this
// COMDAT. The base destructor and operator delete are defined again,
// unannotated, because the original file defined them and /Ob2 inlined them
// here.
#include <stddef.h>
#include <vector>

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);        // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
};

class Class_00474170 {                 // vector element (see 0x4730f0.cpp)
public:
    char unknown_0[0x3c];
    void FUN_00474170(int param_1, short param_2, short param_3);
};

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class Class_004716a0 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474170> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    Class_004716a0() {}
    virtual void FUN_00472d50();                        // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
    virtual void FUN_004742c0(int, int, int, int);      // slot 6, 0x4742c0
};

Class_00471cc0::~Class_00471cc0()
{
}

void __stdcall Class_00471cc0::operator delete(void* p)
{
    DAT_0051e610.FUN_00470ed0(p);
}

// FUNCTION: 0x4716a0 ??_GClass_004716a0@@UAEPAXI@Z
static Class_004716a0* s_object = new Class_004716a0;
