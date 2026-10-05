// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of ParticleSystem
// (vtable 0x4fd5a8), the base of the family listed in 0x471cc0.cpp. It
// inlines the base destructor (0x471d00, the vtable store) and the class's
// operator delete (0x471d50, which returns the object to the pool).
//
// The constructor (0x471cc0, matched in 0x471cc0.cpp) is defined again below,
// unannotated, to emit the vtable and with it this COMDAT; the destructor and
// operator delete are defined again, unannotated, because the original file
// defined them next to it and /Ob2 inlined them here.
#include <stddef.h>

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FreeSlot(void* p);            // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class ParticleSystem {
public:
    int field_4;                                        // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
};

// FUNCTION: 0x471cd0 ??_GParticleSystem@@UAEPAXI@Z
ParticleSystem::ParticleSystem()
{
    field_4 = 0;
}

ParticleSystem::~ParticleSystem()
{
}

void __stdcall ParticleSystem::operator delete(void* p)
{
    DAT_0051e610.FreeSlot(p);
}
