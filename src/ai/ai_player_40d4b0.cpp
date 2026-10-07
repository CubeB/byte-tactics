// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040d4f0>::_Destroy(first, last): empty, since the element
// type is trivial.
// A 1-byte element type other than unsigned char (vector<unsigned char> has
// its own _Destroy, 0x40d4a0); the type is a guess. Its insert (0x40c600)
// calls 0x40d520 (_Ufill), 0x40d4f0 (_Ucopy) and 0x40d4b0 (_Destroy) with
// ecx set to the vector, the one at +0xad in the object 0x40b390 frees.
#include <vector>

struct Elem_0040d4f0 {
    char value;
};

typedef std::vector<Elem_0040d4f0> Vec_0040d4b0;
typedef void (Vec_0040d4b0::*DestroyFn_0040d4b0)(Vec_0040d4b0::iterator, Vec_0040d4b0::iterator);

struct Access_0040d4b0 : Vec_0040d4b0 {
    static DestroyFn_0040d4b0 fn;
};

// FUNCTION: 0x40d4b0 ?_Destroy@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@IAEXPAUElem_0040d4f0@@0@Z
DestroyFn_0040d4b0 Access_0040d4b0::fn = &Access_0040d4b0::_Destroy;
