// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// std::vector<Class_0046eaa0>::_Destroy(first, last) from MSVC 5's <vector>
// (0x46eaa0 is called as vector<0x5c-byte element>::_Destroy from the inlined
// erase at 0x46db82, with ecx set to the vector). It destroys each 0x5c-byte
// element in place: the element destructor is inlined and frees its four
// std::vector members last-first, each by the inlined ~vector (free _First,
// zero _First/_Last/_End). _Destroy is protected, so a derived class takes its
// address to make the compiler emit it out of line.
//
// Best: 52.3%. What still differs:
// - The original calls vector<Elem_0046faf0>::_Destroy (0x46e870) OUT OF LINE
//   for list_c; ours inlines it away (the loop body is empty, the element
//   destructor is trivial). This is MSVC 5's /Ob2 inline budget. Probe
//   (scratch only, not in this file): if the element class carries 8 or more
//   extra members whose destructors are user-declared and empty, list_c's
//   _Destroy is left out of line with exactly the original's relocations
//   (FUN_00470030, delete, _Destroy, delete, delete, delete), but the
//   register allocation changes (esi becomes first+0x40, edi/ebx swap). 7 such
//   members still inline list_c; 9 also push list_d's _Destroy out of line.
// - Register allocation inside the loop: the original keeps the element
//   cursor in edi and 0 in ebx and bases on esi = first+0x18; ours keeps the
//   cursor in ebx, 0 in edi, and bases on esi = first+0x40 (list_c._First).
//   The original's ebp = first prologue is reproduced only without the
//   std::_Destroy(Class_0046eaa0*) overload below (which this file omits).
// - list_d's element cleanup loops over 0xe-byte elements calling
//   FUN_00470030; the element type only needs a std::_Destroy overload that
//   makes that call (0x434020.cpp's technique), so the element's own layout is
//   a guess.
#include <vector>

void __stdcall FUN_00470030(int);

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};

struct Elem_00470030 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

// The vector's destroy loop reaches the element cleanup through std::_Destroy;
// an overload for the element type inlines the call at the right level.
namespace std {
inline void _Destroy(Elem_00470030* p)
{
    FUN_00470030((int)p);
}
}

struct Class_0046eaa0 {
    int field_0;                       // +0x00
    std::vector<int> list_a;           // +0x04
    std::vector<int> list_b;           // +0x14
    char unknown_24[0x18];             // +0x24
    std::vector<Elem_0046faf0> list_c; // +0x3c
    std::vector<Elem_00470030> list_d; // +0x4c
};

typedef std::vector<Class_0046eaa0> Vec_0046eaa0;
typedef void (Vec_0046eaa0::*DestroyFn_0046eaa0)(Vec_0046eaa0::iterator, Vec_0046eaa0::iterator);

struct Access_0046eaa0 : Vec_0046eaa0 {
    static DestroyFn_0046eaa0 fn;
};

// FUNCTION: 0x46eaa0 ?_Destroy@?$vector@UClass_0046eaa0@@V?$allocator@UClass_0046eaa0@@@std@@@std@@IAEXPAUClass_0046eaa0@@0@Z
DestroyFn_0046eaa0 Access_0046eaa0::fn = &Access_0046eaa0::_Destroy;