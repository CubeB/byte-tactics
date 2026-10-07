// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00473500>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
#include <vector>

struct Elem_00473500 {
    int unknown_0;
};

typedef std::vector<Elem_00473500> Vec_004732d0;
typedef void (Vec_004732d0::*DestroyFn_004732d0)(Vec_004732d0::iterator, Vec_004732d0::iterator);

// _Destroy is protected: a derived class takes its address to emit it out of line.
struct Access_004732d0 : Vec_004732d0 {
    static DestroyFn_004732d0 fn;
};

// FUNCTION: 0x4732d0 ?_Destroy@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEXPAUElem_00473500@@0@Z
DestroyFn_004732d0 Access_004732d0::fn = &Access_004732d0::_Destroy;
