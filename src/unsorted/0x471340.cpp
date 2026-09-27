// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Creates a Class_00471430 (vtable 0x4fd588), initialises it through virtual
// slot 6, then appends it to the std::vector<Class_00471cc0*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, which is what leaves std::vector::insert (0x4732e0) out of line.
// Class family listed in 0x471cc0.cpp; operator new (0x471d10) is inlined here.
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

class Class_00473590 {                 // vector element (see 0x472e30.cpp)
public:
    char unknown_0[0x34];
    void FUN_00473590(void* p, short a, short b);
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes.
class Class_00471430 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473590> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    Class_00471430() {}
    virtual void FUN_00472d50();                        // slot 1, 0x472d50
    virtual void FUN_00472e30(int);                     // slot 2, 0x472e30
    virtual int FUN_00472e70();                         // slot 3, 0x472e70
    virtual void FUN_004737c0();                        // slot 4, 0x4737c0
    virtual int FUN_00472e00();                         // slot 5, 0x472e00
    virtual void FUN_004736e0(int, int, int);           // slot 6, 0x4736e0
};

// The owner of the per-index lists.
class Class_00471340 {
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

    void FUN_00471340(int param_1, int param_2, int param_3, short index);
};

// FUNCTION: 0x471340
void Class_00471340::FUN_00471340(int param_1, int param_2, int param_3, short index)
{
    Class_00471430* p = new Class_00471430;
    if (p) {
        p->FUN_004736e0(param_1, param_2, param_3);
        Add(index, p);
    }
}
