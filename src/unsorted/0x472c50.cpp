// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL (69.3%): everything from the virtual call at 0x472cb3 to the
// std::vector::insert call at 0x472d24 is instruction for instruction what the
// original has, apart from jump displacements and ebx against ebp; the object
// creation before it is not. What still differs is written out at the bottom of
// this comment; the short version: /Ob2 expands the 53-byte Class_004750b0
// constructor at the `new` here, where the original has a call to 0x4750b0, and
// that expansion also shifts the register allocation (ebp instead of ebx holds
// the list owner, esi instead of ecx holds the null pointer). The expansion
// moves the constructor body into a cold block reached by two jumps to
// 0x472cc6, which is why 270 bytes are emitted where the original has 223.
//
// This is the out-of-line twin of the helper inlined in 0x471340.cpp and
// 0x471470.cpp: it creates a Class_004750b0, initialises it through virtual
// slot 6 (0x475150), then appends it to the std::vector<Class_00471cc0*>
// selected by the short index. When that list already holds more than 400
// entries its oldest element is deleted and erased first. The owner of the
// per-index lists is reached through the pointer field at g_game + 0x38d77
// rather than through `this` (0x471340.cpp is a __thiscall method of that
// owner), which is why this one is a free __stdcall function. The class family
// is listed in 0x471cc0.cpp.
//
// The Class_004750b0 constructor is defined here, although 0x4750b0 has its own
// file, and that is deliberate: its body is the real one (0x4750b0.cpp) and it
// compiles to the same code, because the base constructor is declared, not
// defined, so it is called out of line exactly as in the original. Defining it
// makes this file emit the class's vtable and ??_G COMDATs, and that is what
// keeps std::vector::insert (0x4732e0) out of line. Measured with wcl and
// check.py --sym on variants of this file:
//
//   constructor declared only              insert expanded, 549 bytes, 43.0%
//   constructor defined in the class       insert called,   270 bytes, 69.3%
//   constructor defined, static object     insert called,   270 bytes
//   constructor defined, second caller     insert called,   270 bytes
//   plus the real preceding 0x471340       insert expanded, 566 bytes
//
// So the expansion of insert is a compiler-state effect that needs more of the
// original translation unit than this one function. In the exe the constructor
// has a second caller at 0x471a86, inside the 546-byte function 0x471a50, and
// 0x4732e0 has 14 further call sites between 0x471423 and 0x472c3e, so the
// original file holds the whole family. Reproducing that means decompiling
// 0x471a50 and its neighbours into this file, which is the next thing to try.
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
extern char* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
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

    Class_004750b0() { time = *(int*)(g_game + 0x38a47); }
    virtual void FUN_00472d50();                        // slot 1
    virtual void FUN_00472e30(int);                     // slot 2
    virtual int FUN_00472e70();                         // slot 3
    virtual void FUN_004751c0();                        // slot 4
    virtual int FUN_004750f0();                         // slot 5
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c);  // slot 6
};

typedef std::vector<Class_00471cc0*> PAVClass_00471cc0;

// The owner of the per-index lists, reached through the pointer field at
// g_game + 0x38d77 (0x471340.cpp uses it as `this`).
class Class_00471340 {
public:
    PAVClass_00471cc0 lists[1];         // 0x10 bytes each

    void Add(short index, Class_00471cc0* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }
};

// FUNCTION: 0x472c50
void __stdcall FUN_00472c50(Vec3_00475150* pos, short index)
{
    Class_00471340* owner = *(Class_00471340**)(g_game + 0x38d77);
    Class_00471cc0* p = new Class_004750b0;
    if (p) {
        ((Class_004750b0*)p)->FUN_00475150(pos, 5, 0, 0x96);
        owner->Add(index, p);
    }
}
