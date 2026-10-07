// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0046faf0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
// A 14-byte element type (its layout is a guess). Its callers (0x46cc10,
// 0x470390, 0x470560, 0x46ca60 and others) call 0x46fb40 (_Ufill), 0x46faf0
// (_Ucopy) and 0x46e870 (_Destroy) with ecx set to the vector.
#include <vector>

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

typedef std::vector<Elem_0046faf0> Vec_0046e870;
typedef void (Vec_0046e870::*DestroyFn_0046e870)(Vec_0046e870::iterator, Vec_0046e870::iterator);

// _Destroy is protected: a derived class takes its address to emit it out of line.
struct Access_0046e870 : Vec_0046e870 {
    static DestroyFn_0046e870 fn;
};

// FUNCTION: 0x46e870 ?_Destroy@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEXPAUElem_0046faf0@@0@Z
DestroyFn_0046e870 Access_0046e870::fn = &Access_0046e870::_Destroy;
