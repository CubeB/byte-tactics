// Decompiled by space-bunny-free. Names are provisional.
// Creates a NanoParticles (vtable 0x4fd5b8) from a 12-byte argument struct,
// initialises it through virtual slot 6 (0x473b50), then appends it to the
// std::vector<ParticleSystem*> that g_game->lists[index] selects. When that
// list already holds more than 400 entries its oldest element is deleted and
// erased first. The append lives in a small inlined helper, which is what
// leaves std::vector::insert (0x4732e0) out of line. Same shape as the matched
// siblings 0x471340 and 0x4716e0, except that the per-index lists come from
// the game state at g_game + 0x38d77 instead of from a `this` pointer, so this
// is a __stdcall free function of three arguments: a pointer to the struct, a
// second pointer passed straight to slot 6, and the list index. Class family
// listed in 0x471cc0.cpp; operator new (0x471d10) is inlined here. The byte
// stored at +0xc is MSVC copying the vector's empty allocator temporary, not a
// constructor parameter (see particles_470f80.cpp).
//
// The 12-byte argument is copied twice into one 24-byte local and only the
// first copy is read (its address is slot 6's first argument). The original
// kept the dead second copy, so the local has to be a struct whose address
// escapes, which is what stops MSVC 5 removing the second copy's stores.
#include <stddef.h>
#include <string.h>
#include <vector>

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* AllocSlot(unsigned int size);
};

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
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

class Class_004739b0 {                 // see nano_particles.cpp
public:
    char unknown_0[0x30];
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
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
    virtual void FUN_00473b50(void* a, void* b, int c);  // slot 6, 0x473b50
};

struct Vec3_004720d0 {                 // the 12-byte argument struct
    int x, y, z;
};

struct Ctx_004720d0 {                  // both copies, one 24-byte local
    Vec3_004720d0 a;
    Vec3_004720d0 b;
};

struct Lists_004720d0 { std::vector<ParticleSystem*> lists[1]; };  // 0x10 each

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_004720d0* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

static void Add_004720d0(Lists_004720d0* lists, short index, ParticleSystem* p)
{
    if (lists->lists[index].size() > 400) {
        delete lists->lists[index][0];
        lists->lists[index].erase(lists->lists[index].begin());
    }
    lists->lists[index].push_back(p);
}

// FUNCTION: 0x4720d0
void __stdcall EmitNanoParticles(Vec3_004720d0* p, void* param_2, short index)
{
    Ctx_004720d0 ctx;
    ctx.a = *p;
    ctx.b = *p;
    Lists_004720d0* lists = g_game->lists;
    NanoParticles* q = new NanoParticles;
    if (q) {
        q->FUN_00473b50(&ctx.a, param_2, 1);
        Add_004720d0(lists, index, q);
    }
}
