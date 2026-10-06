// Decompiled by Haiku, class family consolidated by Opus, Sonnet and Opus. Names are provisional.

#include <stddef.h>
#include <string.h>

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FreeSlot(void* p);            // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* AllocSlot(unsigned int size);
};

extern char DAT_0051e608;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    int time;                          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

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
    void SetLifetime(int ticks);
};

// Constructor of ParticleSystem, the base of a family of objects allocated
// from the pool DAT_0051e610 through the class's own operator new (0x471d10)
// and operator delete (0x471d50). The base vtable 0x4fd5a8 holds the virtual
// destructor and three pure virtuals; every derived class overrides those
// three and adds three more of its own (slot 6 initialises the object and
// calls 0x471d70, which sets field_4).
//
//   class           vtable    constructor  ??_G      slots 1-6
//   ParticleSystem  0x4fd5a8  0x471cc0     0x471cd0  _purecall x3
//   TeleportParticles  0x4fd588  inline       0x471430  0x472d50 0x472e30 0x472e70 0x4737c0 0x472e00 0x4736e0
//   NanoParticles  0x4fd5b8  inline       0x471560  0x472eb0 0x472f90 0x472fd0 0x473d50 0x472f60 0x473b50
//   ThrustParticles  0x4fd5d8  inline       0x4716a0  0x473010 0x4730f0 0x473130 0x4743a0 0x4730c0 0x4742c0
//   WakeParticles  0x4fd5f8  inline       0x4717e0  0x473170 0x473250 0x473290 0x474880 0x473220 0x474760
//   SmokeParticles  0x4fd618  0x474cd0     0x474d10  0x475340 0x475470 0x474f80 0x474df0 0x475440 0x474d50
//   Class_004750b0  0x4fd638  0x4750b0     0x475110  0x475600 0x475700 0x475330 0x4751c0 0x4750f0 0x475150
//
// The base destructor is 0x471d00. An override keeps the name of the base
// slot it overrides, so slots 1-3 of every derived class carry the names of
// TeleportParticles's (0x472d50, 0x472e30, 0x472e70); the matched slot methods
// are still recorded under their own placeholder classes, which the checker
// accepts for vtable slots.
//
// The first four derived classes have no out-of-line constructor: the
// functions that create them (0x471340, 0x471470, 0x4715a0, 0x4716e0 and
// others) inline it after the inlined operator new. Those functions sit in
// this class's own file, which is why their ??_G inline the base destructor
// and operator delete, while 0x474d10 and 0x475110 (compiled with
// 0x474cd0 and 0x4750b0) call them. The derived classes' files copy the
// declarations below.
// The scalar deleting destructor (0x471cd0) inlines the destructor and the
// class's operator delete, which are defined below.
// FUNCTION: 0x471cc0
// FUNCTION: 0x471cd0 ??_GParticleSystem@@UAEPAXI@Z
ParticleSystem::ParticleSystem()
{
    field_4 = 0;
}

// The out-of-line destructor of ParticleSystem (the family is listed at
// the constructor): an empty body, so only the vtable store is left. The
// scalar deleting destructors compiled in the same file inline it; 0x474d10
// and 0x475110, compiled elsewhere, call it.
// FUNCTION: 0x471d00
ParticleSystem::~ParticleSystem()
{
}

// The class-specific operator new of ParticleSystem (the family is listed
// at the constructor): takes a zeroed object from the pool DAT_0051e610, or
// returns null while DAT_0051e608 is set. Its operator delete is 0x471d50.
// FUNCTION: 0x471d10
void* __stdcall ParticleSystem::operator new(size_t size)
{
    if (DAT_0051e608)
        return 0;
    void* p = ((Class_00470eb0*)&DAT_0051e610)->AllocSlot(size);
    if (p)
        memset(p, 0, size);
    return p;
}

// The class-specific operator delete of ParticleSystem (the family is listed
// at the constructor): returns the object to the pool DAT_0051e610. It ends in
// `ret 4`, so it is __stdcall (its operator new, 0x471d10, is too). The
// scalar deleting destructors compiled in the same file inline it; 0x474d10
// and 0x475110, compiled elsewhere, call it.
// FUNCTION: 0x471d50
void __stdcall ParticleSystem::operator delete(void* p)
{
    DAT_0051e610.FreeSlot(p);
}

// FUNCTION: 0x471d70
void ParticleSystem::SetLifetime(int ticks)
{
    field_4 = g_game->time + ticks;
}
