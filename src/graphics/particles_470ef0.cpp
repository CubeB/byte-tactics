// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00470f00>::_Destroy(first, last) from the <vector> header:
// empty, since the element type is trivial. Its caller 0x470c10 calls 0x470f30
// (_Ufill), 0x470f00 (_Ucopy) and 0x470ef0 (_Destroy).
#include <vector>

struct Elem_00470f00 {
    int unknown_0;
};

typedef std::vector<Elem_00470f00> Vec_00470ef0;
typedef void (Vec_00470ef0::*DestroyFn_00470ef0)(Vec_00470ef0::iterator, Vec_00470ef0::iterator);

// _Destroy is protected: the derived struct takes its address to emit it.
struct Access_00470ef0 : Vec_00470ef0 {
    static DestroyFn_00470ef0 fn;
};

// FUNCTION: 0x470ef0 ?_Destroy@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEXPAUElem_00470f00@@0@Z
DestroyFn_00470ef0 Access_00470ef0::fn = &Access_00470ef0::_Destroy;
