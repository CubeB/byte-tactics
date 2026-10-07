// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_004750b0 (vtable 0x4fd638) from the object pool, initialises
// it through virtual slot 6 (0x475150) with the first four arguments, then
// appends it to the std::vector of pointers selected by the short index in the
// last argument. When that list already holds more than 400 entries its oldest
// element is deleted and erased first.
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

    static void* __stdcall operator new(size_t size)
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->AllocSlot(size);
        if (p)
            // memset, not a dword loop: keeps inline budget free for insert.
            memset(p, 0, size);
        return p;
    }

    static void __stdcall operator delete(void* p)
    {
        DAT_0051e610.FreeSlot(p);
    }
};

class Class_004750b0;

struct Elem_00473500 {                 // the vector's element (see 0x473500.cpp)
    Class_004750b0* p;
};

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct Class_00474fc0 {
    int unknown[8];
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Class_00474fc0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void Update();                              // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* p, int a, int b, int c);  // slot 6
};

// The owner of the per-index lists.
class ParticleLists {
public:
    std::vector<Elem_00473500> lists[1];                // 0x10 bytes each

    void Add(short index, Class_004750b0* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0].p;
            lists[index].erase(lists[index].begin());
        }
        // Cast to the element type the vector helper symbols are named after.
        lists[index].push_back(*(Elem_00473500*)&p);
    }

    void FUN_00471a50(Vec3_00475150* param_1, int param_2, int param_3,
                      int param_4, short index);
};

// FUNCTION: 0x471a50
void ParticleLists::FUN_00471a50(Vec3_00475150* param_1, int param_2,
                                  int param_3, int param_4, short index)
{
    Class_004750b0* e = new Class_004750b0;
    if (e) {
        e->FUN_00475150(param_1, param_2, param_3, param_4);
        Add(index, e);
    }
}
