// Decompiled by space-bunny-free. Names are provisional.
// NOT MATCHING: 43.7% of the original 224 bytes, and the first difference is
// the prologue. Everything up to `mov ebx, [eax + 0x38d77]` and the whole
// allocation, the virtual slot 6 call and the index multiply now line up; what
// is left is the compiler's inlining of the tail, which is a compiler-state
// problem rather than a wording problem (details at the bottom of this note).
//
// Creates a Class_00474cd0 (vtable 0x4fd618, the class family is listed in
// 0x471cc0.cpp), initialises it through virtual slot 6 (0x474d50), then
// appends it to the std::vector of base-class pointers picked by the short
// index in the lists g_game holds at +0x38d77 (0x471d90.cpp). When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first. The append lives in an inlined member helper, which is what leaves
// std::vector::insert (0x4732e0) out of line. The class's operator new
// (0x471d10) is inlined here. Sibling of 0x4728f0; same shape as the matched
// 0x471340, except that the class is a free __stdcall function taking a Vec3
// instead of three ints, the constructor is called out of line, and the lists
// come from g_game.
//
// What still differs, all of it one inlining decision:
//   * `size()` (the > 400 test) is a call here, the null-guarded difference
//     divided by 4 inline in the original,
//   * `push_back` expands std::vector::insert inline here, while the original
//     calls 0x4732e0,
//   * that expansion also spills a callee-saved register (the `mov ebx, 4`
//     next to the > 400 compare), so the function gets a frame pointer
//     (`push ebp`) and every argument slot shifts by 4. Apart from those two
//     symptoms the original is reproduced instruction for instruction.
// Tried and rejected, all compile but still expand insert inline: push_back vs
// a direct `insert(end(), 1, p)`; the helper as a member, a static member
// taking the list pointer, or a free function taking the vector address; the
// helper inlined by hand; the vector holding Class_00474cd0* instead of
// Class_00471cc0*; lists[1] instead of lists[10]; struct vs class; the helper
// declared after the function; an inline base constructor and an inline
// operator delete. Making the derived constructor inline (an empty
// `Class_00474cd0() {}`) does leave insert out of line, but then the
// constructor is no longer the call to 0x474cd0 the original makes, so that is
// not a match either. This looks like the inline budget of the translation
// unit the original was built in, which also held 0x471340 and 0x471470.
#include <stddef.h>
#include <string.h>
#include <vector>

struct Vec3_00474d50 {
    int x;
    int y;
    int z;
};

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* FUN_00470eb0(unsigned int size);
};

// The object pool (see 0x470ae0.cpp); its method returns an object to the
// free list.
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

// The owner of the per-index lists.
struct Lists_004729d0 {
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
struct Game_004729d0 {
    char unknown_0[0x38d77];
    Lists_004729d0* lists;              // +0x38d77
};
#pragma pack(pop)

extern Game_004729d0* g_game;

// FUNCTION: 0x4729d0
void __stdcall FUN_004729d0(Vec3_00474d50* pos, short index)
{
    Lists_004729d0* lists = g_game->lists;
    Class_00474cd0* p = new Class_00474cd0;
    if (p) {
        p->FUN_00474d50(pos, 3, 1, 0x1e, 0, 0);
        lists->Add(index, p);
    }
}
