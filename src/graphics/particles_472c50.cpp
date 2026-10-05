// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_004750b0 (vtable 0x4fd638) from the object pool, initialises
// it through virtual slot 6, then appends it to the
// std::vector<Class_00471cc0*> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. The append lives in an inlined member helper, which is what leaves
// std::vector::insert (0x4732e0) out of line. Twin of 0x4728f0 and 0x4729d0,
// which pass different constants to slot 6. Class family listed in
// 0x471cc0.cpp; operator new (0x471d10) is inlined here, the constructor is not.
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

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3

    // Not memset: the dword loop plus byte tail is what operator new at
    // 0x471d10 shows, and the loop's size is what /Ob2 charges this function's
    // inline budget for, which is what leaves std::vector::insert (0x4732e0)
    // out of line here. A memset costs one builtin node and the insert gets
    // inlined instead.
    static void* __stdcall operator new(size_t size)
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->FUN_00470eb0(size);
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
        DAT_0051e610.FUN_00470ed0(p);
    }
};

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct Record_004750b0 {
    int unknown[8];
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void FUN_00472d50();                        // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void FUN_004751c0();                        // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* p, int a, int b, int c);  // slot 6
};

// The owner of the per-index lists.
struct Lists_00472c50 {
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
    Lists_00472c50* lists;              // +0x38d77
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x472c50
void __stdcall FUN_00472c50(Vec3_00475150* p, short index)
{
    Lists_00472c50& l = *g_game->lists;
    Class_004750b0* e = new Class_004750b0;
    if (e) {
        e->FUN_00475150(p, 5, 0, 0x96);
        l.Add(index, e);
    }
}
