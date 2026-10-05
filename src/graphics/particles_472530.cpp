// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_004717e0 (vtable 0x4fd5f8, 0x48 bytes), initialises it
// through virtual slot 6, then appends it to the std::vector<Class_00471cc0*>
// selected by the short index. When that list already holds more than 400
// entries its oldest element is deleted and erased first. The append lives in
// an inlined member helper, which is what leaves std::vector::insert (0x4732e0)
// out of line. Same shape as 0x471340, which builds a Class_00471430 instead;
// 0x471340 is a method on the lists owner, so its `this` lands in ebp, here the
// lists pointer is a local read before the `new` (without it MSVC reloads
// g_game after the virtual call instead of keeping the base in ebp).
// The byte copied to +0xc is MSVC copying the vector's empty allocator
// temporary, not a constructor parameter: the fourth argument (the index) is
// dead after that, as in 0x471340.
// Class family listed in 0x4717e0.cpp; operator new (0x471d10) is inlined here.
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

class Class_004745e0 {                 // vector element (see 0x473250.cpp)
public:
    char unknown_0[0x44];
    void FUN_004745e0(int param_1, short param_2, short param_3);
};

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
class Class_004717e0 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_004745e0> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x48 - 0x1c];

    Class_004717e0() {}
    virtual void FUN_00472d50();                        // slot 1, 0x473170
    virtual void FUN_00472e30(int);                     // slot 2, 0x473250
    virtual int FUN_00472e70();                         // slot 3, 0x473290
    virtual void FUN_00474880();                        // slot 4, 0x474880
    virtual int FUN_00473220();                         // slot 5, 0x473220
    virtual void FUN_00474760(int, int, int, int, int); // slot 6, 0x474760
};

// The owner of the per-index lists.
struct Lists_00472530 {
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
struct Game_00472530 {
    char unknown_0[0x38d77];
    Lists_00472530* lists;              // +0x38d77
};
#pragma pack(pop)

extern Game_00472530* g_game;

// FUNCTION: 0x472530
void __stdcall FUN_00472530(int param_1, int param_2, int param_3, short index)
{
    Lists_00472530* l = g_game->lists;
    Class_004717e0* p = new Class_004717e0;
    if (p) {
        p->FUN_00474760(param_1, param_2, param_3, 1, 0);
        l->Add(index, p);
    }
}
