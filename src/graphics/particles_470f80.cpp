// Decompiled by Opus, Sonnet and DeepSeek V4.1 Flash. Names are provisional.
// ParticleLists: ten lists of live particle systems (the family listed in
// 0x471cc0.cpp), updated, drawn and torn down together, and the Add methods
// that create a system, start it through its slot 6 and append it to a list,
// dropping the oldest past 400. operator new (0x471d10) is inlined into them.
#include <stddef.h>
#include <string.h>
#include <vector>

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* AllocSlot(unsigned int size);
};

// The object pool (see 0x470ae0.cpp); its method returns the object to the
// free list. Needed by the inlined operator new.
class Class_00470ed0 {
public:
    char unknown_0[4];
    void FreeSlot(void* p);
};

extern Class_00470ed0 DAT_0051e610;
extern char DAT_0051e608;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class ParticleSystem {
public:
    int field_4;                                        // +0x4

    ParticleSystem() { field_4 = 0; }
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3

    static void* __stdcall operator new(size_t size)
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->AllocSlot(size);
        if (p)
            memset(p, 0, size);
        return p;
    }

    static void __stdcall operator delete(void* p)
    {
        DAT_0051e610.FreeSlot(p);
    }
};

class Class_00473560 {                 // see particles_473560.cpp
public:
    char unknown_0[0x34];
};

class Class_004739b0 {                 // see particles_4739b0.cpp
public:
    char unknown_0[0x30];
};

class Class_00474130 {                 // see particles_474130.cpp
public:
    char unknown_0[0x3c];
};

class Class_00474580 {                 // see particles_474580.cpp
public:
    char unknown_0[0x44];
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes (teleport_particles.cpp).
class TeleportParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473560> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];
    TeleportParticles() {}
    virtual void Update();                              // slot 1, 0x472d50
    virtual void FUN_00472e30(int);                     // slot 2, 0x472e30
    virtual int FUN_00472e70();                         // slot 3, 0x472e70
    virtual void Emit();                                // slot 4, 0x4737c0
    virtual int FUN_00472e00();                         // slot 5, 0x472e00
    virtual void FUN_004736e0(int, int, int);           // slot 6, 0x4736e0
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes (nano_particles.cpp).
class NanoParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_004739b0> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];
    NanoParticles() {}
    virtual void Update();                              // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(int, int, int);           // slot 6, 0x473b50
};

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes (thrust_particles.cpp).
class ThrustParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474130> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];
    ThrustParticles() {}
    virtual void Update();                              // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
    virtual void FUN_004742c0(int, int, int, int);      // slot 6, 0x4742c0
};

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes (wake_particles.cpp).
class WakeParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474580> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x48 - 0x1c];
    WakeParticles() {}
    virtual void Update();                              // slot 1, 0x473170
    virtual void FUN_00472e30(int);                     // slot 2, 0x473250
    virtual int FUN_00472e70();                         // slot 3, 0x473290
    virtual void Emit();                                // slot 4, 0x474880
    virtual int FUN_00473220();                         // slot 5, 0x473220
    virtual void FUN_00474760(int, int, int, int, int); // slot 6, 0x474760
};

struct Elem_00473500;
struct Vec3_00474d50;
struct Vec3_00475150;

class ParticleLists {
public:
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }

    ParticleLists();
    ~ParticleLists();
    void UpdateAll();
    void DrawAll(void* param);
    void DrawList(void* param, short index);
    void AddTeleportParticles(int param_1, int param_2, int param_3, short index);
    void AddNanoParticles(int param_1, int param_2, int param_3, short index);
    void AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index);
    void AddWakeParticles(int param_1, int param_2, int param_3, int param_4,
                          short index, int param_6);
    // In particles_471160.cpp, particles_471820.cpp and particles_471a50.cpp:
    // they see a list as a std::vector<Elem_00473500> and inline its insert,
    // which comes out differently next to the methods here.
    void AddToList(Elem_00473500 x, short index);
    void AddSmoke(Vec3_00474d50* param_1, int param_2, int param_3, int param_4,
                  int param_5, short index, int param_7);
    void FUN_00471a50(Vec3_00475150* param_1, int param_2, int param_3,
                      int param_4, short index);
};

// Out-of-line constructor of the ten lists (0x471d90 inlines this same loop after its `new`). Each vector's
// empty allocator byte is copied from an uninitialised temporary.
// FUNCTION: 0x470f80
ParticleLists::ParticleLists()
{
}

// Destructor of the ten lists 0x471d90 allocates into the game
// object (used by 0x471f40 and 0x471f90): deletes every particle system, erasing it
// from the front of its list, then the vector members are destroyed.
// FUNCTION: 0x470fb0
ParticleLists::~ParticleLists()
{
    for (int i = 0; i < 10; i++) {
        std::vector<ParticleSystem*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            delete *it;
            lists[i].erase(it);
        }
    }
}

// Updates the ten lists: a particle system whose slot-3 check says it is
// finished is deleted and erased, every other one gets its slot-1
// call.
// FUNCTION: 0x471050
void ParticleLists::UpdateAll()
{
    for (int i = 0; i < 10; i++) {
        std::vector<ParticleSystem*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            ParticleSystem* l = *it;
            if (l->FUN_00472e70()) {
                delete l;
                lists[i].erase(it);
            } else {
                l->Update();
                it++;
            }
        }
    }
}

// Passes param to slot 2 (the draw) of every particle system in the ten lists.
// FUNCTION: 0x4710e0
void ParticleLists::DrawAll(void* param)
{
    for (int i = 0; i < 10; i++) {
        for (std::vector<ParticleSystem*>::iterator it = lists[i].begin(); it != lists[i].end(); it++)
            (*it)->FUN_00472e30((int)param);
    }
}

// Passes param to slot 2 (the draw) of every particle system in one list.
// FUNCTION: 0x471120
void ParticleLists::DrawList(void* param, short index)
{
    std::vector<ParticleSystem*>* s = &lists[index];
    for (std::vector<ParticleSystem*>::iterator p = s->begin(); p != s->end(); p++)
        (*p)->FUN_00472e30((int)param);
}

// Creates a TeleportParticles (vtable 0x4fd588), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
// FUNCTION: 0x471340
void ParticleLists::AddTeleportParticles(int param_1, int param_2, int param_3, short index)
{
    TeleportParticles* p = new TeleportParticles;
    if (p) {
        p->FUN_004736e0(param_1, param_2, param_3);
        Add(index, p);
    }
}

// Creates a NanoParticles (vtable 0x4fd5b8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
// The byte stored at +0xc is MSVC copying the vector's empty allocator
// temporary, not a constructor parameter (see particles_470f80.cpp).
// FUNCTION: 0x471470
void ParticleLists::AddNanoParticles(int param_1, int param_2, int param_3, short index)
{
    NanoParticles* p = new NanoParticles;
    if (p) {
        p->FUN_00473b50(param_1, param_2, param_3);
        Add(index, p);
    }
}

// Creates a ThrustParticles (vtable 0x4fd5d8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
//
// Signature: four ints forwarded to virtual slot 6, then the short index.
// ret 0x14 is five dwords, and the virtual callee (a __thiscall, so it pops its
// own four arguments) receives the slots at [esp+0x14]..[esp+0x20] while the
// index is the slot at [esp+0x24], read once as a byte (the vector's empty
// allocator temporary at +0xc) and once as a word. That is the same shape as
// 0x471340 with one extra int, so the trailing `short` really is the last
// parameter, not a leading one.
// FUNCTION: 0x4715a0
void ParticleLists::AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index)
{
    ThrustParticles* p = new ThrustParticles;
    if (p) {
        p->FUN_004742c0(param_1, param_2, param_3, param_4);
        Add(index, p);
    }
}

// Creates a WakeParticles (vtable 0x4fd5f8), initialises it through virtual
// slot 6 with five arguments, then appends it to the
// std::vector<ParticleSystem*> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. The append lives in an inlined member helper, which is what leaves
// 0x471cc0.cpp; operator new (0x471d10) is inlined here. The byte stored at
// +0xc is MSVC copying the vector's empty allocator temporary, not a
// constructor parameter (see particles_470f80.cpp).
// FUNCTION: 0x4716e0
void ParticleLists::AddWakeParticles(int param_1, int param_2, int param_3,
                                  int param_4, short index, int param_6)
{
    WakeParticles* p = new WakeParticles;
    if (p) {
        p->FUN_00474760(param_1, param_2, param_3, param_4, param_6);
        Add(index, p);
    }
}
