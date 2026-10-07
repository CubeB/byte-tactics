// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040d550>::_Ufill(first, n, value): copy-constructs n
// copies of value into raw storage at first.
// The same vector as 0x40d550 (_Ucopy): its resize (0x40c7f0) calls
// 0x40d550, 0x40d580 (_Ufill) and 0x40d4e0 (_Destroy) with ecx set to it.
#include <vector>

struct Elem_0040d550 {
    int unknown_0;
};

typedef std::vector<Elem_0040d550> Vec_0040d580;
typedef void (Vec_0040d580::*UfillFn_0040d580)(
    Vec_0040d580::iterator, Vec_0040d580::size_type, const Elem_0040d550&);

struct Access_0040d580 : Vec_0040d580 {
    static UfillFn_0040d580 fn;
};

// FUNCTION: 0x40d580 ?_Ufill@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@IAEXPAUElem_0040d550@@IABU3@@Z
UfillFn_0040d580 Access_0040d580::fn = &Access_0040d580::_Ufill;
