// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00406c10>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. _Ucopy is protected, so a derived class takes its
// address to make the compiler emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its callers (0x405d90, 0x40b530, 0x480250 and others)
// inline vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and
// 0x406c00 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00406c10 {
    int unknown_0;
};

typedef std::vector<Elem_00406c10> Vec_00406c10;
typedef Vec_00406c10::iterator (Vec_00406c10::*UcopyFn_00406c10)(
    Vec_00406c10::const_iterator, Vec_00406c10::const_iterator, Vec_00406c10::iterator);

struct Access_00406c10 : Vec_00406c10 {
    static UcopyFn_00406c10 fn;
};

// FUNCTION: 0x406c10 ?_Ucopy@?$vector@UElem_00406c10@@V?$allocator@UElem_00406c10@@@std@@@std@@IAEPAUElem_00406c10@@PBU3@0PAU3@@Z
UcopyFn_00406c10 Access_00406c10::fn = &Access_00406c10::_Ucopy;
