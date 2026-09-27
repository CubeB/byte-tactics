// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00473500>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. _Ucopy is protected, so a derived class takes its
// address to make the compiler emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its callers (0x471160, 0x471820, 0x471a50) inline
// vector::insert and call 0x473530 (_Ufill), 0x473500 (_Ucopy) and
// 0x4732d0 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00473500 {
    int unknown_0;
};

typedef std::vector<Elem_00473500> Vec_00473500;
typedef Vec_00473500::iterator (Vec_00473500::*UcopyFn_00473500)(
    Vec_00473500::const_iterator, Vec_00473500::const_iterator, Vec_00473500::iterator);

struct Access_00473500 : Vec_00473500 {
    static UcopyFn_00473500 fn;
};

// FUNCTION: 0x473500 ?_Ucopy@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEPAUElem_00473500@@PBU3@0PAU3@@Z
UcopyFn_00473500 Access_00473500::fn = &Access_00473500::_Ucopy;
