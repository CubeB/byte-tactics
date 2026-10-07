// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040d550>::_Destroy(first, last): empty, since the element
// type is trivial.
// The same vector as 0x40d550 (_Ucopy): its resize (0x40c7f0) calls
// 0x40d550, 0x40d580 (_Ufill) and 0x40d4e0 (_Destroy) with ecx set to it.
#include <vector>

struct Elem_0040d550 {
    int unknown_0;
};

typedef std::vector<Elem_0040d550> Vec_0040d4e0;
typedef void (Vec_0040d4e0::*DestroyFn_0040d4e0)(Vec_0040d4e0::iterator, Vec_0040d4e0::iterator);

struct Access_0040d4e0 : Vec_0040d4e0 {
    static DestroyFn_0040d4e0 fn;
};

// FUNCTION: 0x40d4e0 ?_Destroy@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@IAEXPAUElem_0040d550@@0@Z
DestroyFn_0040d4e0 Access_0040d4e0::fn = &Access_0040d4e0::_Destroy;
