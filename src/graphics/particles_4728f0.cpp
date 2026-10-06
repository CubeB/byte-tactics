// Decompiled by space-bunny-free. Names are provisional.
// Creates a SmokeParticles (vtable 0x4fd618) from the object pool, initialises
// it through virtual slot 6, then appends it to the
// std::vector<ParticleSystem*> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. The append lives in an inlined member helper, which is what leaves
// std::vector::insert (0x4732e0) out of line. Class family listed in
// 0x471cc0.cpp; operator new (0x471d10) is inlined here, the constructor
// (0x474cd0) is not.
//
// This is the twin of 0x472810 (98.8 percent identical, only the internal jump
// displacements differ, and slot 6 gets (0, 1, 0, 0, 1) here against
// (0, 0, 0, 1, 0, 0) there), so 0x472810 can copy this file and change only
// the // FUNCTION: annotation, the file's names and the slot 6 constants.
// 0x4729d0 and 0x472c50 are the same shape with other constants again and are
// matched with the same two points below.
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
    //
    // The other half of it: the list owner is read through a reference before
    // the allocation (see the function below), so g_game and its +0x38d77
    // field are loaded into ebx ahead of the pool call and stay live across it.
    // Written as g_game->lists->Add(...) the compiler sinks both loads down to
    // the append, keeps the owner in eax together with the vtable, and the
    // prologue saves only esi and edi.
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

struct Vec3_00474cd0 {
    int x;
    int y;
    int z;
};

struct Class_00474b00 {
    void* image;
    Vec3_00474cd0 pos;
    int size;
    int field_14;
    int field_18;
    int field_1c;
};

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Class_00474b00> records;               // +0xc (_First +0x10)
    int unknown_1c;                                     // +0x1c
    int unknown_20;                                     // +0x20
    int unknown_24;                                     // +0x24
    void* unknown_28;                                   // +0x28
    Vec3_00474cd0 pos;                                  // +0x2c

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474cd0* p, int a, int b, int c, int d, int e);
};

// The owner of the per-index lists.
struct Lists_004728f0 {
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
    Lists_004728f0* lists;              // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4728f0
void __stdcall EmitBlackSmoke(Vec3_00474cd0* p, short index)
{
    Lists_004728f0& l = *g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->FUN_00474d50(p, 0, 1, 0, 0, 1);
        l.Add(index, e);
    }
}
