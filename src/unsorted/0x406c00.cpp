// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00406c10>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its callers (0x405d90, 0x40b530, 0x480250 and others)
// inline vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and
// 0x406c00 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00406c10 {
    int unknown_0;
};

typedef std::vector<Elem_00406c10> Vec_00406c00;
typedef void (Vec_00406c00::*DestroyFn_00406c00)(Vec_00406c00::iterator, Vec_00406c00::iterator);

struct Access_00406c00 : Vec_00406c00 {
    static DestroyFn_00406c00 fn;
};

// FUNCTION: 0x406c00 ?_Destroy@?$vector@UElem_00406c10@@V?$allocator@UElem_00406c10@@@std@@@std@@IAEXPAUElem_00406c10@@0@Z
DestroyFn_00406c00 Access_00406c00::fn = &Access_00406c00::_Destroy;
