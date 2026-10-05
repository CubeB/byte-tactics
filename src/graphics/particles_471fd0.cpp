// Decompiled by space-bunny-free. Names are provisional.
// The same shape as 0x471340 (ParticleLists::AddTeleportParticles), but a free
// function: the owner of the ten lists comes from g_game->lists (+0x38d77)
// instead of `this`, and it is read into ebp before anything else. The list is
// picked by the fourth argument, a short; the first three go to the virtual
// slot 6 initialiser (0x4736e0).
// The byte the inlined constructor leaves at +0xc is not a field: it is the
// junk MSVC 5's inlined std::vector constructor stores into the vector's
// allocator slot, and its "source" here is whatever the fake temporary lands
// on, which is the fourth argument's stack slot. The same byte appears in
// 0x471340. The append is an inlined helper, which is what leaves
// std::vector::insert (0x4732e0) out of line.
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

class Class_00473590 {                 // vector element (see 0x472e30.cpp)
public:
    char unknown_0[0x34];
    void DrawParticle(void* p, short a, short b);
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes.
class TeleportParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473590> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    TeleportParticles() {}
    virtual void Update();                              // slot 1, 0x472d50
    virtual void FUN_00472e30(int);                     // slot 2, 0x472e30
    virtual int FUN_00472e70();                         // slot 3, 0x472e70
    virtual void Emit();                                // slot 4, 0x4737c0
    virtual int FUN_00472e00();                         // slot 5, 0x472e00
    virtual void FUN_004736e0(int, int, int);           // slot 6, 0x4736e0
};

// The ten listener lists (see 0x471d90, 0x471f40).
struct Lists_00471fd0 {
    std::vector<ParticleSystem*> lists[10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d77];
    Lists_00471fd0* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// Appends to one list, dropping its oldest entry once it holds 400 or more.
static void __stdcall Add(Lists_00471fd0* l, short index, ParticleSystem* p)
{
    if (l->lists[index].size() > 400) {
        delete l->lists[index][0];
        l->lists[index].erase(l->lists[index].begin());
    }
    l->lists[index].push_back(p);
}

// FUNCTION: 0x471fd0
void __stdcall EmitTeleportParticles(int param_1, int param_2, int param_3, short index)
{
    Lists_00471fd0* l = g_game->lists;
    TeleportParticles* p = new TeleportParticles;
    if (p) {
        p->FUN_004736e0(param_1, param_2, param_3);
        Add(l, index, p);
    }
}
