// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00473500>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its callers (0x471160, 0x471820, 0x471a50) inline
// vector::insert and call 0x473530 (_Ufill), 0x473500 (_Ucopy) and
// 0x4732d0 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00473500 {
    int unknown_0;
};

typedef std::vector<Elem_00473500> Vec_004732d0;
typedef void (Vec_004732d0::*DestroyFn_004732d0)(Vec_004732d0::iterator, Vec_004732d0::iterator);

struct Access_004732d0 : Vec_004732d0 {
    static DestroyFn_004732d0 fn;
};

// FUNCTION: 0x4732d0 ?_Destroy@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEXPAUElem_00473500@@0@Z
DestroyFn_004732d0 Access_004732d0::fn = &Access_004732d0::_Destroy;
