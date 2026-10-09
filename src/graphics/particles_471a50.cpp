// Decompiled by space-bunny-free. Names are provisional.
// Stays in its own file: it needs the std::vector<Elem_00473500> view of the
// lists, whose inlined insert cannot agree with particles.cpp's.
// Creates a TimedSubParticles (vtable 0x4fd638) from the object pool, initialises
// it through virtual slot 6 (0x475150) with the first four arguments, then
// appends it to the std::vector of pointers selected by the short index in the
// last argument. When that list already holds more than 400 entries its oldest
// element is deleted and erased first.
#include <stddef.h>
#include <string.h>
#include <vector>

// The object pool at g_particlePool (see 0x470a90.cpp): AllocSlot takes an
// object from the free list and FreeSlot returns one to it; the inlined
// operator new and delete need them.
#include "object_pool.h"

extern ObjectPool g_particlePool;
extern char g_fxEventPoolBlocked;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class ParticleSystem {
public:
    int deadline;                                       // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void Render(int) = 0;                       // slot 2
    virtual int IsFinished() = 0;                       // slot 3

    static void* __stdcall operator new(size_t size)
    {
        if (g_fxEventPoolBlocked)
            return 0;
        void* p = (void*)g_particlePool.AllocSlot(size);
        if (p)
            // memset, not a dword loop: keeps inline budget free for insert.
            memset(p, 0, size);
        return p;
    }

    static void __stdcall operator delete(void* p)
    {
        g_particlePool.FreeSlot((int)p);
    }
};

class TimedSubParticles;

struct Elem_00473500 {                 // the vector's element (see 0x473500.cpp)
    TimedSubParticles* p;
};

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct TimedSubParticle {
    int unknown[8];
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class TimedSubParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<TimedSubParticle> records;             // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    TimedSubParticles();
    virtual void Update();                              // slot 1, 0x475600
    virtual void Render(int);                           // slot 2, 0x475700
    virtual int IsFinished();                           // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    virtual int IsEmitDue();                            // slot 5, 0x4750f0
    virtual void Init(Vec3_00475150* p, int a, int b, int c);          // slot 6
};

// The owner of the per-index lists.
class ParticleLists {
public:
    std::vector<Elem_00473500> lists[1];                // 0x10 bytes each

    void Add(short index, TimedSubParticles* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0].p;
            lists[index].erase(lists[index].begin());
        }
        // Cast to the element type the vector helper symbols are named after.
        lists[index].push_back(*(Elem_00473500*)&p);
    }

    void AddTimedSubParticles(Vec3_00475150* param_1, int param_2, int param_3,
                              int param_4, short index);
};

// FUNCTION: 0x471a50
void ParticleLists::AddTimedSubParticles(Vec3_00475150* param_1, int param_2,
                                        int param_3, int param_4, short index)
{
    TimedSubParticles* e = new TimedSubParticles;
    if (e) {
        e->Init(param_1, param_2, param_3, param_4);
        Add(index, e);
    }
}
