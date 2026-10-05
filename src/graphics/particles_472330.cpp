// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_004716a0 (vtable 0x4fd5d8), initialises it through virtual
// slot 6 (0x4742c0) with the first four arguments, then appends it to the
// std::vector<Class_00471cc0*> selected by the short index in the ten
// per-index lists at g_game->lists (created by 0x471d90, walked by 0x471eb0,
// 0x471f40 and 0x471f90). When that list already holds more than 400 entries
// its oldest element is deleted and erased first. The append lives in an
// inlined member helper, which is what leaves std::vector::insert (0x4732e0)
// out of line.
// Class family listed in 0x471cc0.cpp; operator new (0x471d10) is inlined
// here. Same shape as the matched 0x471340.cpp and 0x472430.cpp.
// The byte stored at +0xc is MSVC copying the vector's empty allocator
// temporary, not a constructor parameter (see 0x471340.cpp).
// g_game->lists is bound to a local at the top: that is what makes the original
// load g_game before the prologue pushes and keep the lists pointer in ebp for
// the whole function instead of reloading it at the Add call.
#include <stddef.h>
#include <string.h>
#include <vector>

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* FUN_00470eb0(unsigned int size);
};

// The object pool (see 0x470ae0.cpp); its method returns the object to the
// free list. Needed by the inlined operator new.
class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
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

class Class_00474170 {                 // vector element (see 0x4730f0.cpp)
public:
    char unknown_0[0x3c];
    void FUN_00474170(int param_1, short param_2, short param_3);
};

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class Class_004716a0 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474170> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    Class_004716a0() {}
    virtual void FUN_00472d50();                        // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
    virtual void FUN_004742c0(int, int, int, int);      // slot 6, 0x4742c0
};

// The owner of the ten per-index lists (see 0x471d90.cpp).
class Lists_00472330 {
public:
    std::vector<Class_00471cc0*> lists[10];             // 0x10 bytes each

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
struct Game {
    char unknown_0[0x38d77];
    Lists_00472330* lists;                              // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x472330
void __stdcall FUN_00472330(int param_1, int param_2, int param_3, int param_4,
                            short index)
{
    Lists_00472330* lists = g_game->lists;
    Class_004716a0* p = new Class_004716a0;
    if (p) {
        p->FUN_004742c0(param_1, param_2, param_3, param_4);
        lists->Add(index, p);
    }
}
