// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040d4f0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// A 1-byte element type other than unsigned char (vector<unsigned char> has
// its own _Destroy, 0x40d4a0); the type is a guess. Its insert (0x40c600)
// calls 0x40d520 (_Ufill), 0x40d4f0 (_Ucopy) and 0x40d4b0 (_Destroy) with
// ecx set to the vector, the one at +0xad in the object 0x40b390 frees.
#include <vector>

struct Elem_0040d4f0 {
    char value;
};

typedef std::vector<Elem_0040d4f0> Vec_0040d520;
typedef void (Vec_0040d520::*UfillFn_0040d520)(
    Vec_0040d520::iterator, Vec_0040d520::size_type, const Elem_0040d4f0&);

struct Access_0040d520 : Vec_0040d520 {
    static UfillFn_0040d520 fn;
};

// FUNCTION: 0x40d520 ?_Ufill@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@IAEXPAUElem_0040d4f0@@IABU3@@Z
UfillFn_0040d520 Access_0040d520::fn = &Access_0040d520::_Ufill;
