// Decompiled by space-bunny-free. Names are provisional.
// Creates a SmokeParticles (vtable 0x4fd618) from the object pool, initialises
// it through virtual slot 6, then appends it to the
// std::vector<ParticleSystem*> picked by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. Twin of 0x4728f0 with the last constant of the slot 6 call changed to
// 1.
#include <stddef.h>
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

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3

    // Not memset: the dword loop plus byte tail is what operator new at
    // 0x471d10 shows, and the loop's size is what /Ob2 charges this function's
    // inline budget for, which is what leaves std::vector::insert (0x4732e0)
    // out of line here. A memset costs one builtin node and the insert gets
    // inlined instead.
    static void* __stdcall operator new(size_t size)
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->AllocSlot(size);
        if (p)
        {
            int* q = (int*)p;
            int n = size >> 2;
            while (n-- > 0)
                *q++ = 0;
            if (size & 3)
            {
                char* c = (char*)p + (size & ~3);
                int m = size & 3;
                while (m-- > 0)
                    *c++ = 0;
            }
        }
        return p;
    }

    static void __stdcall operator delete(void* p)
    {
        DAT_0051e610.FreeSlot(p);
    }
};

struct Vec3_00472810 {
    int x;
    int y;
    int z;
};

struct Record_00472810 {
    int unknown[8];
};

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Record_00472810> records;               // +0xc
    char unknown_1c[0x38 - 0x1c];

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00472810* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// The owner of the per-index lists.
class Lists_00472810 {
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
    Lists_00472810* lists;              // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x472810
void __stdcall EmitWhiteSmoke(Vec3_00472810* pos, short index)
{
    Lists_00472810* owner = g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->FUN_00474d50(pos, 0, 1, 0, 0, 0);
        owner->Add(index, e);
    }
}
