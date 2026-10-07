// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040d4f0>::_Ucopy(first, last, dest): copies
// [first, last) into raw storage at dest and returns the end of the copies.
// A 1-byte element type other than unsigned char (vector<unsigned char> has
// its own _Destroy, 0x40d4a0); the type is a guess. Its insert (0x40c600)
// calls 0x40d520 (_Ufill), 0x40d4f0 (_Ucopy) and 0x40d4b0 (_Destroy) with
// ecx set to the vector, the one at +0xad in the object 0x40b390 frees.
#include <vector>

struct Elem_0040d4f0 {
    char value;
};

typedef std::vector<Elem_0040d4f0> Vec_0040d4f0;
typedef Vec_0040d4f0::iterator (Vec_0040d4f0::*UcopyFn_0040d4f0)(
    Vec_0040d4f0::const_iterator, Vec_0040d4f0::const_iterator, Vec_0040d4f0::iterator);

struct Access_0040d4f0 : Vec_0040d4f0 {
    static UcopyFn_0040d4f0 fn;
};

// FUNCTION: 0x40d4f0 ?_Ucopy@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@IAEPAUElem_0040d4f0@@PBU3@0PAU3@@Z
UcopyFn_0040d4f0 Access_0040d4f0::fn = &Access_0040d4f0::_Ucopy;
