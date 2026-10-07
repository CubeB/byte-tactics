// Decompiled by Opus. Names are provisional.
// std::vector<Elem_004702a0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. Its caller 0x470040 calls 0x4702a0 (_Ucopy) and
// 0x470290 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_004702a0 {
    int unknown_0;
};

typedef std::vector<Elem_004702a0> Vec_004702a0;
typedef Vec_004702a0::iterator (Vec_004702a0::*UcopyFn_004702a0)(
    Vec_004702a0::const_iterator, Vec_004702a0::const_iterator, Vec_004702a0::iterator);

struct Access_004702a0 : Vec_004702a0 {
    static UcopyFn_004702a0 fn;
};

// FUNCTION: 0x4702a0 ?_Ucopy@?$vector@UElem_004702a0@@V?$allocator@UElem_004702a0@@@std@@@std@@IAEPAUElem_004702a0@@PBU3@0PAU3@@Z
UcopyFn_004702a0 Access_004702a0::fn = &Access_004702a0::_Ucopy;
