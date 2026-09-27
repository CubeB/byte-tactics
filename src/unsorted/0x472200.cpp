// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_00471560 (vtable 0x4fd5b8), initialises it through virtual
// slot 6 with a 24-byte struct built from the position argument, then appends
// it to the std::vector<Class_00471cc0*> selected by the short index. When that
// list already holds more than 400 entries its oldest element is deleted and
// erased first. The append lives in an inlined member helper, which is what
// leaves std::vector::insert (0x4732e0) out of line. Same shape as the matched
// 0x471470, but a free function that reads the list owner out of g_game.
// The byte stored at +0xc is MSVC copying the vector's empty allocator
// temporary, not a constructor parameter (see 0x471340.cpp).
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

class Class_00473a00 {                 // vector element (see 0x472f90.cpp)
public:
    char unknown_0[0x30];
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class Class_00471560 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473a00> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];

    Class_00471560() {}
    virtual void FUN_00472d50();                        // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void FUN_00473d50();                        // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(void*, void*, int);       // slot 6, 0x473b50
};

struct Vec3 {
    int x, y, z;
};

// The 24-byte struct slot 6 (0x473b50) copies both to the object, at +0x1c
// and +0x34; here both halves are initialised from the position argument.
struct Pos_00472200 {
    Vec3 a;
    Vec3 b;
};

// The owner of the per-index lists.
class Class_00472200 {
public:
    std::vector<Class_00471cc0*> lists[1];              // 0x10 bytes each

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
struct Game_00472200 {
    char unknown_0[0x38d77];
    Class_00472200* lists;                              // +0x38d77
};
#pragma pack(pop)

extern Game_00472200* g_game;

// FUNCTION: 0x472200
void __stdcall FUN_00472200(Pos_00472200* param_1, Vec3* param_2, short param_3)
{
    Pos_00472200 pos;
    pos.a = *param_2;
    pos.b = *param_2;
    Class_00472200* lists = g_game->lists;
    Class_00471560* p = new Class_00471560;
    if (p) {
        p->FUN_00473b50(param_1, &pos, 1);
        lists->Add(param_3, p);
    }
}
