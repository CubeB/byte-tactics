// Decompiled by space-bunny-free. Names are provisional.
// Creates a ThrustParticles (vtable 0x4fd5d8) from the object pool after jittering
// the high short of each of the three pairs of the 12-byte point it is given, then
// initialises it through virtual slot 6 (0x4742c0) with that same point twice, a
// 1 and a fourth random number, and finally appends it to the
// std::vector<ParticleSystem*> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. The append lives in an inlined member helper, which is what leaves
// std::vector::insert (0x4732e0) out of line. Twin of 0x472330, which takes the
// four arguments of slot 6 as parameters instead of building them here; the
// class family is listed in 0x471cc0.cpp and thrust_particles.cpp, operator new
// (0x471d10) is inlined here and the constructor is inlined too.
// The 12-byte point is three pairs of shorts: the three `add word ptr [..], ax`
// land on the high half of each pair, at +2, +6 and +10, so the copy is one
// 12-byte struct assignment followed by three separate `+=` statements (a loop
// would have been rotated, and MSVC keeps each rand() call with its own add).
// The fourth random number is a statement of its own before the allocation:
// its value has to live in a callee-saved register (ebp) across the call to the
// pool, and its call to rand() is emitted before the load of g_game.
// Suspected original bug: the same point is passed as both of the first two
// arguments of slot 6, so 0x4742c0 copies the same data into unknown_20 and
// unknown_2c and their difference (unknown_38, the vector from the first to the
// second point, scaled by 1/b) is always zero. Both `lea` instructions compute
// the same address, which is the evidence.
#include <stddef.h>
#include <stdlib.h>
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

class Class_00474130 {                 // see thrust_particles.cpp
public:
    char unknown_0[0x3c];
    void DrawParticle(int param_1, short param_2, short param_3);
};

// The point the caller hands over: three pairs of shorts, of which the high
// half of each is the one that gets jittered.
struct Pair_00472ab0 {
    short lo;
    short hi;
};
struct Shape_00472ab0 {
    Pair_00472ab0 v[3];
};

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
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
    virtual void FUN_004742c0(Shape_00472ab0* p, Shape_00472ab0* q, int a, int b);
};

// The owner of the per-index lists.
class Lists_00472ab0 {
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
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_00472ab0* lists;                              // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x472ab0
void __stdcall FUN_00472ab0(Shape_00472ab0* param_1, short index)
{
    Shape_00472ab0 s = *param_1;
    s.v[0].hi += (int)((__int64)rand() * 3 / 0x8000) - 1;
    s.v[1].hi += (int)((__int64)rand() * 3 / 0x8000) - 1;
    s.v[2].hi += (int)((__int64)rand() * 3 / 0x8000) - 1;
    int r = (int)((__int64)rand() * 3 / 0x8000) + 1;
    Lists_00472ab0* lists = g_game->lists;
    ThrustParticles* p = new ThrustParticles;
    if (p) {
        p->FUN_004742c0(&s, &s, 1, r);
        lists->Add(index, p);
    }
}
