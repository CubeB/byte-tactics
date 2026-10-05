// Decompiled by space-bunny-free. Names are provisional.
// Creates a SmokeParticles (vtable 0x4fd618), whose constructor (0x474cd0) is
// called out of line, initialises it through virtual slot 6 (0x474d50) with
// the first five arguments and the last, then appends it to the
// std::vector<Elem_00473500> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. Here the append's std::vector::insert is inlined, so its helpers
// _Ucopy (0x473500), _Ufill (0x473530), _Destroy (0x4732d0) and size
// (0x472d30) are called out of line; in the siblings (0x471340, 0x471470,
// 0x4716e0, 0x472330) the insert itself is out of line (0x4732e0).
// Class family listed in 0x471cc0.cpp; operator new (0x471d10) is inlined
// here. The element type is the one the _Ucopy/_Ufill/_Destroy symbols in
// 0x4732d0.cpp name, so the appended pointer needs a cast to it.
//
// PARTIAL: every byte matches (check.py reports 100%), but the four calls to
// size() are reported as wrong references, and the fault is a name in
// data/symbols.csv rather than in this file. Those four calls are the size()
// of the same std::vector<Elem_00473500> whose helpers symbols.csv records
// as UElem_00473500::?$vector::_Ucopy (0x473500), ::_Ufill (0x473530) and
// ::_Destroy (0x4732d0), so the consistent name for 0x472d30 would be
// UElem_00473500::?$vector::size; instead 0x472d30.cpp gave its class the
// placeholder name of the function's own address, and symbols.csv holds
// "Class_00472d30::FUN_00472d30" for 0x472d30. check.py compares mangled
// names, so the reference to the correctly named size() is refused.
// Calling 0x472d30 by that provisional name instead was tried and dropped:
// declaring the insert's size() as an out-of-line member of a class named
// Class_00472d30 (with the real std::vector<Elem_00473500> as a base, for
// the _Ucopy/_Ufill/_Destroy names) does resolve all four references, but
// writing the insert by hand instead of letting <vector> inline it changes
// the optimiser's inlining decisions: _Ucopy then gets inlined (it is not in
// the original), the erase's copy is called out of line, the element size is
// no longer hoisted into ebx, and the result drops to 36 to 66 percent. The
// fix belongs in 0x472d30.cpp: declaring the vector there (as 0x473500.cpp
// and 0x473530.cpp do for the same container) would give 0x472d30 the name
// the other three helpers have, and this file would match as it stands.
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

struct Record_00474cd0 {
    int unknown[8];
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Record_00474cd0> records;               // +0xc (_First +0x10)
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
class Class_00471820 {
public:
    std::vector<Elem_00473500> lists[1];                // 0x10 bytes each

    void Add(short index, SmokeParticles* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0].p;
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(*(Elem_00473500*)&p);
    }

    void AddSmoke(Vec3_00474d50* param_1, int param_2, int param_3, int param_4,
                      int param_5, short index, int param_7);
};

// FUNCTION: 0x471820
void Class_00471820::AddSmoke(Vec3_00474d50* param_1, int param_2, int param_3,
                                  int param_4, int param_5, short index, int param_7)
{
    SmokeParticles* p = new SmokeParticles;
    if (p) {
        p->FUN_00474d50(param_1, param_2, param_3, param_4, param_5, param_7);
        Add(index, p);
    }
}
