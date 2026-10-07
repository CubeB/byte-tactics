// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0046faf0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first.
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

typedef std::vector<Elem_0046faf0> Vec_0046fb40;
typedef void (Vec_0046fb40::*UfillFn_0046fb40)(
    Vec_0046fb40::iterator, Vec_0046fb40::size_type, const Elem_0046faf0&);

struct Access_0046fb40 : Vec_0046fb40 {
    static UfillFn_0046fb40 fn;
};

// FUNCTION: 0x46fb40 ?_Ufill@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEXPAUElem_0046faf0@@IABU3@@Z
UfillFn_0046fb40 Access_0046fb40::fn = &Access_0046fb40::_Ufill;
