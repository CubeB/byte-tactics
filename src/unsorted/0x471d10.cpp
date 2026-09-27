// Decompiled by Opus. Names are provisional.
// The class-specific operator new of Class_00471cc0 (the family is listed
// in 0x471cc0.cpp): takes a zeroed object from the pool DAT_0051e610, or
// returns null while DAT_0051e608 is set. Its operator delete is 0x471d50.
#include <stddef.h>
#include <string.h>

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* FUN_00470eb0(unsigned int size);
};

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);        // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;
extern char DAT_0051e608;

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

// FUNCTION: 0x471d10
void* __stdcall Class_00471cc0::operator new(size_t size)
{
    if (DAT_0051e608)
        return 0;
    void* p = ((Class_00470eb0*)&DAT_0051e610)->FUN_00470eb0(size);
    if (p)
        memset(p, 0, size);
    return p;
}
