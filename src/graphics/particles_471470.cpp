// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Creates a NanoParticles (vtable 0x4fd5b8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
// Class family listed in 0x471cc0.cpp; operator new (0x471d10) is inlined here.
// The byte stored at +0xc is MSVC copying the vector's empty allocator
// temporary, not a constructor parameter (see 0x471340.cpp).
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

class Class_00473a00 {                 // vector element (see 0x472f90.cpp)
public:
    char unknown_0[0x30];
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class NanoParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473a00> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];

    NanoParticles() {}
    virtual void Update();                              // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(int, int, int);           // slot 6, 0x473b50
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

    void AddNanoParticles(int param_1, int param_2, int param_3, short index);
};

// FUNCTION: 0x471470
void ParticleLists::AddNanoParticles(int param_1, int param_2, int param_3, short index)
{
    NanoParticles* p = new NanoParticles;
    if (p) {
        p->FUN_00473b50(param_1, param_2, param_3);
        Add(index, p);
    }
}
