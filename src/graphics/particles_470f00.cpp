// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00470f00>::_Ucopy(first, last, dest) from the <vector>
// header: copies [first, last) into raw storage at dest and returns the end of
// the copies. Its caller 0x470c10 calls 0x470f30 (_Ufill), 0x470f00 (_Ucopy)
// and 0x470ef0 (_Destroy).
#include <vector>

struct Elem_00470f00 {
    int unknown_0;
};

typedef std::vector<Elem_00470f00> Vec_00470f00;
typedef Vec_00470f00::iterator (Vec_00470f00::*UcopyFn_00470f00)(
    Vec_00470f00::const_iterator, Vec_00470f00::const_iterator, Vec_00470f00::iterator);

// _Ucopy is protected: the derived struct takes its address to emit it.
struct Access_00470f00 : Vec_00470f00 {
    static UcopyFn_00470f00 fn;
};

// FUNCTION: 0x470f00 ?_Ucopy@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEPAUElem_00470f00@@PBU3@0PAU3@@Z
UcopyFn_00470f00 Access_00470f00::fn = &Access_00470f00::_Ucopy;
