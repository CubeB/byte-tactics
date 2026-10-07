// Decompiled by space-bunny-free. Names are provisional.
// Creates a WakeParticles (vtable 0x4fd5f8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index in the ten per-index lists at g_game->lists (created by 0x471d90,
// walked by 0x471eb0, 0x471f40 and 0x471f90). When that list already holds
// more than 400 entries its oldest element is deleted and erased first.
// Class family listed in 0x471cc0.cpp.
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

class Class_00474580 {                 // see wake_particles.cpp
public:
    char unknown_0[0x44];
    void DrawParticle(int param_1, short param_2, short param_3);
};

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
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

// The owner of the ten per-index lists (see 0x471d90.cpp).
class Lists_00472430 {
public:
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    // Inlined member helper: leaves std::vector::insert out of line.
    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_00472430* lists;                              // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x472430
void __stdcall EmitWakeParticles(int param_1, int param_2, int param_3, short index)
{
    Lists_00472430* l = g_game->lists;
    WakeParticles* p = new WakeParticles;
    if (p) {
        p->FUN_00474760(param_1, param_2, param_3, 1, 1);
        l->Add(index, p);
    }
}
