// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_00474cd0 (vtable 0x4fd618, 0x38 bytes) from the pool
// DAT_0051e610 through the inlined operator new (0x471d10), initialises it
// through virtual slot 6, then appends it to the std::vector<Class_00471cc0*>
// selected by the short index in the per-index list table g_game->lists
// (+0x38d77). When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
// Class family listed in 0x471cc0.cpp; the shape follows 0x471340.cpp.
#include <stddef.h>
#include <string.h>
#include <vector>

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* FUN_00470eb0(unsigned int size);
};

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);        // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;
extern char DAT_0051e608;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0() { field_4 = 0; }
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3

    static void* __stdcall operator new(size_t size)
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->FUN_00470eb0(size);
        if (p)
            memset(p, 0, size);
        return p;
    }

    static void __stdcall operator delete(void* p)
    {
        DAT_0051e610.FUN_00470ed0(p);
    }
};

struct Record_00474cd0 {               // 32-byte record at +0x10
    int unknown[8];
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class Class_00474cd0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_00474cd0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    Class_00474cd0();                                   // 0x474cd0, out of line
    virtual void FUN_00472d50();                        // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void FUN_00474df0();                        // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// The per-index list table (see 0x471d90.cpp).
struct Lists_00472810 {
    std::vector<Class_00471cc0*> lists[10];             // 0x10 bytes each

    void Add(short index, Class_00471cc0* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].insert(lists[index].end(), 1, p);
    }
};

#pragma pack(push, 1)
struct Game_00472810 {
    char unknown_0[0x38d77];
    Lists_00472810* lists;                              // +0x38d77
};
#pragma pack(pop)

extern Game_00472810* g_game;

// FUNCTION: 0x472810
void __stdcall FUN_00472810(void* pos, short index)
{
    Lists_00472810* lists = g_game->lists;
    Class_00474cd0* p = new Class_00474cd0;
    if (p) {
        p->FUN_00474d50((Vec3_00474d50*)pos, 0, 1, 0, 0, 0);
        lists->Add(index, p);
    }
}
