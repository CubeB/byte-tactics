// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of TeleportParticles
// (vtable 0x4fd588), derived from ParticleSystem (the family is listed in
// 0x471cc0.cpp). Its implicit destructor destroys the std::vector at +0xc
// (the inlined ~vector leaves the dead store of _First in the `push ecx`
// slot), then the inlined base destructor stores the base vtable, and the
// inlined operator delete returns the object to the pool.
//
// The class has no out-of-line constructor: 0x471340 and 0x471fd0 create it
// with `new`, inlining it. Neither is decompiled yet, so the global below
// exists only to make the compiler emit the vtable and with it this COMDAT.
// The base destructor and operator delete are defined again, unannotated,
// because the original file defined them and /Ob2 inlined them here.
#include <stddef.h>
#include <vector>

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

class Class_00473590 {                 // vector element (see 0x472e30.cpp)
public:
    char unknown_0[0x34];
    void DrawParticle(void* p, short a, short b);
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes.
class TeleportParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473590> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    TeleportParticles() {}
    virtual void Update();                              // slot 1, 0x472d50
    virtual void FUN_00472e30(int);                     // slot 2, 0x472e30
    virtual int FUN_00472e70();                         // slot 3, 0x472e70
    virtual void Emit();                                // slot 4, 0x4737c0
    virtual int FUN_00472e00();                         // slot 5, 0x472e00
    virtual void FUN_004736e0(int, int, int);           // slot 6, 0x4736e0
};

ParticleSystem::~ParticleSystem()
{
}

void __stdcall ParticleSystem::operator delete(void* p)
{
    DAT_0051e610.FreeSlot(p);
}

// FUNCTION: 0x471430 ??_GTeleportParticles@@UAEPAXI@Z
static TeleportParticles* s_object = new TeleportParticles;
