// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040cc40>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. _Ucopy is protected, so a derived class takes its
// address to make the compiler emit it out of line.
// The element type is a guess: any 8-byte trivially copyable type compiles
// to the same code. Its callers (0x40a7b0, 0x40ca50) inline vector::insert
// and call 0x40d5b0 (_Ufill), 0x40cc40 (_Ucopy) and 0x40cc30 (_Destroy)
// with ecx set to the vector.
#include <vector>

struct Elem_0040cc40 {
    int a;                             // +0x0
    int b;                             // +0x4
};

typedef std::vector<Elem_0040cc40> Vec_0040cc40;
typedef Vec_0040cc40::iterator (Vec_0040cc40::*UcopyFn_0040cc40)(
    Vec_0040cc40::const_iterator, Vec_0040cc40::const_iterator, Vec_0040cc40::iterator);

struct Access_0040cc40 : Vec_0040cc40 {
    static UcopyFn_0040cc40 fn;
};

// FUNCTION: 0x40cc40 ?_Ucopy@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEPAUElem_0040cc40@@PBU3@0PAU3@@Z
UcopyFn_0040cc40 Access_0040cc40::fn = &Access_0040cc40::_Ucopy;
