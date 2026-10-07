// Decompiled by Opus. Names are provisional.
// std::vector<Elem_004702a0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. Its caller 0x470040 calls
// 0x4702a0 (_Ucopy) and 0x470290 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_004702a0 {
    int unknown_0;
};

typedef std::vector<Elem_004702a0> Vec_00470290;
typedef void (Vec_00470290::*DestroyFn_00470290)(Vec_00470290::iterator, Vec_00470290::iterator);

struct Access_00470290 : Vec_00470290 {
    static DestroyFn_00470290 fn;
};

// FUNCTION: 0x470290 ?_Destroy@?$vector@UElem_004702a0@@V?$allocator@UElem_004702a0@@@std@@@std@@IAEXPAUElem_004702a0@@0@Z
DestroyFn_00470290 Access_00470290::fn = &Access_00470290::_Destroy;
