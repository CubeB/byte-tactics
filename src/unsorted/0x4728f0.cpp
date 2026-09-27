// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x471340 and 0x471470: creates a Class_00474cd0 (vtable 0x4fd618,
// 0x38 bytes), initialises it through virtual slot 6, then appends it to the
// std::vector<Class_00471cc0*> picked by the short index. When that list holds
// more than 400 entries its oldest element is deleted and erased first. The
// append is an inlined member helper, which leaves std::vector::insert
// (0x4732e0) out of line. Here the class is a free __stdcall function taking a
// Vec3 instead of three ints, and the lists come from g_game->lists.
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

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3

    static void* __stdcall operator new(size_t size)     // 0x471d10, inlined
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->FUN_00470eb0(size);
        if (p)
            memset(p, 0, size);
        return p;
    }

    static void __stdcall operator delete(void* p)       // 0x471d50
    {
        DAT_0051e610.FUN_00470ed0(p);
    }
};

struct Record_00474cd0 {
    int unknown[8];
};

struct Vec3_00474d50 {
    int x;
    int y;
    int z;
};

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class Class_00474cd0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_00474cd0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    Class_00474cd0();
    virtual void FUN_00472d50();                        // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void FUN_00474df0();                        // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

struct Lists_004728f0 {
    std::vector<Class_00471cc0*> lists[10];

    void Add(short index, Class_00471cc0* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }
};

#pragma pack(push, 1)
struct Game_004728f0 {
    char unknown_0[0x38d77];
    Lists_004728f0* lists;                             // +0x38d77
};
#pragma pack(pop)

extern Game_004728f0* g_game;

// FUNCTION: 0x4728f0
void __stdcall FUN_004728f0(Vec3_00474d50* pos, short index)
{
    Lists_004728f0* lists = g_game->lists;
    Class_00474cd0* p = new Class_00474cd0;
    if (p) {
        p->FUN_00474d50(pos, 0, 1, 0, 0, 1);
        lists->Add(index, p);
    }
}
