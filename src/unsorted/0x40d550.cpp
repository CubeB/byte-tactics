// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040d550>::_Ucopy(first, last, dest) from MSVC 5's
// <vector> for a 4-byte element type: copies [first, last) into raw storage
// at dest and returns the end of the copies. Called by 0x40c7f0 (the
// vector's resize) with ecx set to the vector. _Ucopy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code.
#include <vector>

struct Elem_0040d550 {
    int unknown_0;
};

typedef std::vector<Elem_0040d550> Vec_0040d550;
typedef Vec_0040d550::iterator (Vec_0040d550::*UcopyFn_0040d550)(
    Vec_0040d550::const_iterator, Vec_0040d550::const_iterator, Vec_0040d550::iterator);

struct Access_0040d550 : Vec_0040d550 {
    static UcopyFn_0040d550 fn;
};

// FUNCTION: 0x40d550 ?_Ucopy@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@IAEPAUElem_0040d550@@PBU3@0PAU3@@Z
UcopyFn_0040d550 Access_0040d550::fn = &Access_0040d550::_Ucopy;
