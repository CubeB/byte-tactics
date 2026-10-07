// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0046faf0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies.
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

typedef std::vector<Elem_0046faf0> Vec_0046faf0;
typedef Vec_0046faf0::iterator (Vec_0046faf0::*UcopyFn_0046faf0)(
    Vec_0046faf0::const_iterator, Vec_0046faf0::const_iterator, Vec_0046faf0::iterator);

struct Access_0046faf0 : Vec_0046faf0 {
    static UcopyFn_0046faf0 fn;
};

// FUNCTION: 0x46faf0 ?_Ucopy@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEPAUElem_0046faf0@@PBU3@0PAU3@@Z
UcopyFn_0046faf0 Access_0046faf0::fn = &Access_0046faf0::_Ucopy;
