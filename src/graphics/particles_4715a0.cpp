// Decompiled by space-bunny-free. Names are provisional.
// Creates a ThrustParticles (vtable 0x4fd5d8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
// Class family listed in 0x4716a0.cpp; operator new (0x471d10) is inlined here.
//
// Signature: four ints forwarded to virtual slot 6, then the short index.
// ret 0x14 is five dwords, and the virtual callee (a __thiscall, so it pops its
// own four arguments) receives the slots at [esp+0x14]..[esp+0x20] while the
// index is the slot at [esp+0x24], read once as a byte (the vector's empty
// allocator temporary at +0xc) and once as a word. That is the same shape as
// 0x471340 with one extra int, so the trailing `short` really is the last
// parameter, not a leading one.
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

class Class_00474170 {                 // vector element (see 0x4730f0.cpp)
public:
    char unknown_0[0x3c];
};

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class ThrustParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474170> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    ThrustParticles() {}
    virtual void Update();                              // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
    virtual void FUN_004742c0(int, int, int, int);      // slot 6, 0x4742c0
};

// The owner of the per-index lists.
class ParticleLists {
public:
    std::vector<ParticleSystem*> lists[1];              // 0x10 bytes each

    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }

    void AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index);
};

// FUNCTION: 0x4715a0
void ParticleLists::AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index)
{
    ThrustParticles* p = new ThrustParticles;
    if (p) {
        p->FUN_004742c0(param_1, param_2, param_3, param_4);
        Add(index, p);
    }
}
