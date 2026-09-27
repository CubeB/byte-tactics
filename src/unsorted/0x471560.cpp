// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_00471560
// (vtable 0x4fd5b8), derived from Class_00471cc0 (the family is listed in
// 0x471cc0.cpp). Its implicit destructor destroys the std::vector at +0xc
// (the inlined ~vector leaves the dead store of _First in the `push ecx`
// slot), then the inlined base destructor stores the base vtable, and the
// inlined operator delete returns the object to the pool.
//
// The class has no out-of-line constructor: 0x471470, 0x4720d0 and 0x472200
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

class Class_00473a00 {                 // vector element (see 0x472f90.cpp)
public:
    char unknown_0[0x30];
    void FUN_00473a00(int param_1, short x, short y);
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class Class_00471560 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473a00> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];

    Class_00471560() {}
    virtual void FUN_00472d50();                        // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void FUN_00473d50();                        // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(int, int, int);           // slot 6, 0x473b50
};

Class_00471cc0::~Class_00471cc0()
{
}

void __stdcall Class_00471cc0::operator delete(void* p)
{
    DAT_0051e610.FUN_00470ed0(p);
}

// FUNCTION: 0x471560 ??_GClass_00471560@@UAEPAXI@Z
static Class_00471560* s_object = new Class_00471560;
