// Decompiled by space-bunny-free. Names are provisional.
// Creates a SmokeParticles (vtable 0x4fd618), initialises it through virtual
// slot 6 (0x474d50) with the first five arguments and the last, then appends it
// to the std::vector<Elem_00473500> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. Class family listed in 0x471cc0.cpp.
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

class SmokeParticles;

struct Elem_00473500 {                 // the vector's element (see 0x473500.cpp)
    SmokeParticles* p;
};

struct Class_00474b00 {
    int unknown[8];
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Class_00474b00> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// The owner of the per-index lists.
class ParticleLists {
public:
    std::vector<Elem_00473500> lists[1];                // 0x10 bytes each

    void Add(short index, SmokeParticles* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0].p;
            lists[index].erase(lists[index].begin());
        }
        // Cast to the element type the vector helper symbols are named after.
        lists[index].push_back(*(Elem_00473500*)&p);
    }

    void AddSmoke(Vec3_00474d50* param_1, int param_2, int param_3, int param_4,
                      int param_5, short index, int param_7);
};

// FUNCTION: 0x471820
void ParticleLists::AddSmoke(Vec3_00474d50* param_1, int param_2, int param_3,
                                  int param_4, int param_5, short index, int param_7)
{
    SmokeParticles* p = new SmokeParticles;
    if (p) {
        p->FUN_00474d50(param_1, param_2, param_3, param_4, param_5, param_7);
        Add(index, p);
    }
}
